#include "Parser.h"
#include <fstream>
#include<qdebug.h>
#include<qmessagebox.h>
#include<qfiledialog.h>
#include<fstream>
#include<cstdio>
#include<sstream>
#include<qfile.h>
#include<cstdlib>
#include<stdio.h>
#include<Windows.h>
//#include"Show.h"


#define DEFAULT_GRAMMAR "Grammar.txt"
#define CODE_FILE "Input_Code.txt"

//lexer
#define SUCCESS 1
#define CODE_FILE_OPEN_ERROR 2
#define LEXICAL_ANALYSIS_ERROR 3

//grammar
#define GRAMMAR_FILE_OPEN_ERROR 2
#define GRAMMAR_ERROR 3
#define GRAMMAR_ERROR_FILE_OPEN_ERROR 4

//lr1
#define ACTION_ERROR 2
#define GOTO_ERROR  3
#define REDUCTION_PROCESS_FILE_OPEN_ERROR 4
#define PARSE_ERROR_FILE_OPEN_ERROR 5
#define TREE_DOT_FILE_OPEN_ERROR 6


/**
* @func:   Parser���캯��
* @para:    
* @return: 
*/
Parser::Parser(QWidget *parent): QMainWindow(parent)
{   
	this->ui = new Ui::ParserClass;
    this->ui->setupUi(this);
	//resize(800, 600);
	this->compileFlag = 0;
	this->loadGrammarFlag = 0;
	this->treeFlag = 0;
	this->lexerFlag = 0;
	this->errorFlag = 0;
	this->errorLine = 0;
	this->dfaFlag = 0;
	
	
	this->grammarFile = DEFAULT_GRAMMAR;
	//this->lr1Grammar=new lr1Grammar(DEFAULT_GRAMMAR);//�ȼ��س�ʼ�﷨�ļ�
	this->codeFile = "";//codefileΪ��
	this->pageShow = new Show;

	//�����������ķ�
	connect(this->ui->action_Code, &QAction::triggered, this, &Parser::loadCode);//�ǵ����������print���,״̬ת��
	connect(this->ui->action_Grammar, &QAction::triggered, this, &Parser::loadGrammar);//д��my_grammar.txt��

	//�����������
	connect(this->ui->action_Run, &QAction::triggered, this, &Parser::compile);//�ǵ����������print���,״̬ת��
	connect(this->ui->action_Restart, &QAction::triggered, this, &Parser::restart);//д��my_grammar.txt�� 

	//չʾ�ʷ�dfa
	connect(this->ui->action_DFA, &QAction::triggered, [=]() {
		// ��ʾվ���޸�ҳ�� չʾҳ��
		if (this->lexerFlag==0) {
			QMessageBox::critical(this, "����", "������ɱ���");
		}
		else {
			this->pageShow->showDFA();
			this->pageShow->show();
		}
	});


	// չʾaction goto��
	connect(this->ui->action_Table, &QAction::triggered, [=]() {
		// ��ʾվ���޸�ҳ�� չʾҳ��
		if (this->compileFlag == 0) {
			QMessageBox::critical(this, "����", "������ɱ���");
		}
		else {
			this->pageShow->showTable();
			this->pageShow->show();
		}
		});


	// չʾ�ʷ�token
	connect(this->ui->action_Token, &QAction::triggered, [=]() {
		// ��ʾվ���޸�ҳ�� չʾҳ��
		if (this->lexerFlag==0) {
			QMessageBox::critical(this, "����", "������ɱ���");
		}
		else {
			this->pageShow->showToken();
			this->pageShow->show();
		}
	});

	// չʾ��Լ����
	connect(this->ui->action_Process, &QAction::triggered, [=]() {
		// ��ʾվ���޸�ҳ�� չʾҳ��
		if (this->compileFlag==0) {
			QMessageBox::critical(this, "����", "������ɱ���");
		}
		else {
			this->pageShow->showProcess();
			this->pageShow->show();
		}
	});

	
	// չʾ�﷨������
	connect(this->ui->action_Tree, &QAction::triggered, [=]() {
		// ��ʾվ���޸�ҳ�� չʾҳ��
		if (this->compileFlag==0) {
			QMessageBox::critical(this, "����", "������ɱ���");
		}
		else if (this->treeFlag==0) {
			QMessageBox::critical(this, "����", "���Ȱ�װgraphviz");
		}
		else {
			this->pageShow->showTree();
			this->pageShow->show();
		}
	});

	// չʾ�﷨������
	connect(this->ui->actionLR_1_DFA, &QAction::triggered, [=]() {
		// ��ʾվ���޸�ҳ�� չʾҳ��
		if (this->compileFlag == 0) {
			QMessageBox::critical(this, "����", "������ɱ���");
		}
		else if (this->dfaFlag == 0) {
			QMessageBox::critical(this, "����", "�﷨DFA��δ����");
		}
		else {
				this->pageShow->showParseDFA();
				this->pageShow->show();
		}
		});
	

	QString startTip = "*************************\n";
	       startTip += "LR(1)�ʷ��﷨����������\n";
	       startTip += "*************************\n";
	startTip += "�뵼���ķ��ļ��Ͳ�����/����Դ����";
	

	this->ui->textBrowser->setText(startTip);
}

Parser::~Parser()
{

}

/**
* @func:   �����ķ�
* @para:   
* @return: 
*/
void Parser::loadGrammar()
{
	QString fileName = QFileDialog::getOpenFileName(this, "�����ļ�", ".", "files(*.txt)");
	if (!fileName.isEmpty()) {
		this->grammarFile=fileName;
		this->loadGrammarFlag = 1;
		this->ui->textBrowser->append("\n�ķ��ļ�����ɹ�");
	}
	else {
		this->ui->textBrowser->append("\n���ļ�������ʧ��");
		QMessageBox::critical(this, "����", "�����ķ��ļ�ʧ��");
	}
}

/**
* @func:   ������뵽�ı�����
* @para:   
* @return:
*/
void Parser::loadCode()
{
	QString fileName = QFileDialog::getOpenFileName(this, "�����ļ�", ".", "files(*.txt *.cpp *.c)");
	if (!fileName.isEmpty()) {
		this->codeFile = fileName;
		fstream fcode;
		fcode.open((string)fileName.toLocal8Bit());
		if (fcode.is_open() == 0) {
			QString tip = "\nԴ���ļ�" + fileName + "�޷���";
			this->ui->textBrowser->append(tip);
			QMessageBox::critical(this, "����", "Դ���ļ��޷���");
			return;
		}
		//���
		this->ui->textEdit->clear();
		while (!fcode.eof() && fcode) {
			string line;
			getline(fcode,line);
			this->ui->textEdit->append(QString::fromLocal8Bit(line.c_str()));
		}
		fcode.close();
		this->ui->textBrowser->append("\n�ɹ�����Դ���ļ�");
	}
	else {
		this->ui->textBrowser->append("\n���ļ�������ʧ��");
		QMessageBox::critical(this, "����", "����Դ�����ļ�ʧ��");
	}
}

/**
* @func:   ��ʼ����
* @para:   
* @return: 
*/
void Parser::compile()
{
	this->compileFlag = 0;
	this->treeFlag = 0;
	this->lexerFlag = 0;
	if (this->errorFlag == 1) {
		this->showErrorLine(this->errorLine, Qt::white);
	}
	//�ķ��ļ�
	if (this->loadGrammarFlag == 0) {
		if (QMessageBox::No == QMessageBox::question(this, "����", "��ǰ��û�е����ķ��ļ���ȷ��ʹ��Ĭ���ļ�" + QString::fromLocal8Bit(DEFAULT_GRAMMAR), QMessageBox::Yes | QMessageBox::No)) {
			return;
		}
	}
	//���ı�����������
	QString codeInput = this->ui->textEdit->toPlainText();
	if (codeInput.size() > 0) {
		// ������ȡ
		QFile file(CODE_FILE);
		if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QTextStream out(&file);
			out << codeInput;
			file.close();
			this->ui->textBrowser->append("\nԴ������д�뵽�ļ�Input_Code.txt��");
		}
		else {
			QMessageBox::warning(this, "����", "���뱣��ʧ��");
		}
	}
	else {
		this->ui->textBrowser->append("\n��ǰû�е����������Դ����");
		QMessageBox::critical(this, "����", "û�е�����������");
		return;
	}


	//�ʷ�����
	QString tip = "���ڽ��дʷ����������Ժ�...\n";
	this->ui->textBrowser->append(tip);
	this->my_lexer = new lexer;

	int lexer_error=this->my_lexer->lexicalAnalysis(CODE_FILE);
	if (lexer_error == SUCCESS) {
		tip = "�ʷ���������\n";
	}
	else if(lexer_error==LEXICAL_ANALYSIS_ERROR) {
		tip = "�ʷ���������,������־�ļ���������\n";
		this->ui->textBrowser->append(tip);
		this->showErrorLine(this->my_lexer->error_line,Qt::red);
		this->errorLine = this->my_lexer->error_line;
		this->errorFlag = 1;
		//�Ѽ�¼������ļ�������
		QFile file(ERROR_LIST_FILE);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream in(&file);

			// ��ȡ�ļ����ݲ�д�� TextBrowser
			this->ui->textBrowser->append(in.readAll());
			file.close();
		}
		else {
			this->ui->textBrowser->append("Error: Unable to open the file.");
			return;
		}
		QMessageBox::critical(this, "����", "�ʷ�����ʧ�ܣ�����ʧ��!");
		tip += "����ʧ��!!!";
		return ;
	}
	else {
		this->ui->textBrowser->append("\nԴ�����ļ�����ʧ��");
		QMessageBox::critical(this, "����", "����Դ�����ļ�ʧ��");
		return;
	}
	tip += "�ʷ������ѽ��������ڵ�ǰĿ¼�²鿴\n�ʷ���������ļ�Token_List.txt\n";
	this->ui->textBrowser->append(tip);
	this->lexerFlag = 1;

	//�ķ�����
	tip = "���ڶ�ȡ�ķ��ļ������Ժ�...\n";
	this->ui->textBrowser->append(tip);
	

	this->my_lr1Grammar = new lr1Grammar;

	int grammar_error= this->my_lr1Grammar->ReadGrammar((string)this->grammarFile.toLocal8Bit());

	if (grammar_error == SUCCESS) {
		tip = "�ķ��ļ�����\n";
		this->ui->textBrowser->append(tip);
	}
	else if (grammar_error == GRAMMAR_ERROR) {
		tip = "�ķ�����,������־�ļ���������\n";
		this->ui->textBrowser->append(tip);
		//�Ѽ�¼������ļ�������
		QFile file(GRAMMAR_ERROR_FILE);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream in(&file);
			// ��ȡ�ļ����ݲ�д�� TextBrowser
			this->ui->textBrowser->append(in.readAll());
			file.close();
		}
		else {
			this->ui->textBrowser->append("Error: Unable to open the file.");
			return;
		}
		QMessageBox::critical(this, "����", "�ķ�����!");
		tip = "����ʧ��!!!";
		this->ui->textBrowser->append(tip);
		return;
	}
	else if (grammar_error == GRAMMAR_FILE_OPEN_ERROR) {
		this->ui->textBrowser->append("\n�ķ�������Ϣ��¼ʧ��");
		QMessageBox::warning(this, "����", "�ķ�������Ϣ��¼ʧ��");
		return;
	}
	else {
		this->ui->textBrowser->append("\n�ķ��ļ�����ʧ��");
		QMessageBox::critical(this, "����", "�����ķ��ļ�ʧ��");
		return;
	}
	
	this->my_lr1Grammar->initClosure0();
	this->my_lr1Grammar->generateDFA();
	this->my_lr1Grammar->generateACTION_GOTO();
	this->my_lr1Grammar->printACTION_GOTO();
	tip = "�ķ��Ѷ��룬���ڵ�ǰĿ¼�²鿴��\n1.�ع��ķ�Extended_Grammar.txt\n2.�ķ���Fisrt��First_Set.txt\n3.Action Goto��Tables.csv";
	this->ui->textBrowser->append(tip);
	
	tip = "\n���ڽ����﷨���������Ժ�...\n";
	this->ui->textBrowser->append(tip);

	int lr1_error = my_lr1Grammar->reduction(my_lexer->lexer_res);

	if (lr1_error == ACTION_ERROR) {
		tip = "�ķ�����������ACTION����ҳ���\n";
		this->ui->textBrowser->append(tip);
		QMessageBox::critical(this, "����", "ACTION�����ʧ��");
	}
	else if(lr1_error==GOTO_ERROR) {
		tip = "�ķ�����������GOTO����ҳ���\n";
		this->ui->textBrowser->append(tip);
		QMessageBox::critical(this, "����", "GOTO�����ʧ��");
	}
	else if (lr1_error==REDUCTION_PROCESS_FILE_OPEN_ERROR) {
		this->ui->textBrowser->append("\n��Լ���̼�¼ʧ��");
		QMessageBox::warning(this, "����", "��Լ���̼�¼ʧ��");
		return;
	}
	else if (lr1_error == PARSE_ERROR_FILE_OPEN_ERROR) {
		this->ui->textBrowser->append("\n�﷨���������¼ʧ��");
		QMessageBox::warning(this, "����", "�﷨���������¼ʧ��");
		return;
	}
	else {
		tip = "�﷨�����ѽ��������ڵ�ǰĿ¼�²鿴��\n�ķ���Լ�����ļ�Reduction_Process.txt\n\n";
		tip += "����ɹ�������\n";
		this->ui->textBrowser->append(tip);
		this->compileFlag = 1;
		this->errorFlag =0;

		if (lr1_error == TREE_DOT_FILE_OPEN_ERROR) {
			this->ui->textBrowser->append("\n�﷨������ʧ��");
			QMessageBox::warning(this, "����", "�﷨������ʧ��");
			return;
		}
		else {
			const char* command = "dot --version";
			// ʹ�� popen ִ�������ȡ���
			FILE* fp = _popen(command, "r");
			if (fp == nullptr) {
				QMessageBox::warning(this, "����", "û�а�װgraphviz,�޷������﷨������");
				return;
			}
			_pclose(fp);

			system("dot -Tpng results/Parse_Tree.dot -o picture/Parse_Tree.png");
			tip = "�﷨�����������ɣ����ڵ�ǰĿ¼�²鿴Parse_Tree.png��";
			this->ui->textBrowser->append(tip);
			this->treeFlag = 1;
		}
			
		if (my_lr1Grammar->printParseDFA() != SUCCESS) {
			this->ui->textBrowser->append("\nLR(1)�ķ�DFA����ʧ��");
			QMessageBox::warning(this, "����", "LR(1)�ķ�DFA����ʧ��");
		}
		else {
			
			if (QMessageBox::Yes == QMessageBox::question(this, "��ʾ", "�﷨DFA����ʱ��ϳ���ȷ���������ɣ�", QMessageBox::Yes | QMessageBox::No))
			{

				tip = "��������DFA";
				this->ui->textBrowser->append(tip);
				system("dot -Tpng results/Parse_DFA.dot -o picture/Parse_DFA.png");

				tip = "�﷨����DFA�����ɣ����ڵ�ǰĿ¼�²鿴Parse_DFA.png��";
				this->ui->textBrowser->append(tip);
				this->dfaFlag = 1;
			}
		}
		
	}
	if (lr1_error !=SUCCESS) {
		this->showErrorLine(this->my_lr1Grammar->error_line,Qt::red);
		this->errorLine = this->my_lr1Grammar->error_line;
		this->errorFlag = 1;
		QFile file("Parse_Error.txt");
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream in(&file);
			// ��ȡ�ļ����ݲ�д�� TextBrowser
			this->ui->textBrowser->append(in.readAll());
			file.close();
		}
		else {
			this->ui->textBrowser->setPlainText("Error: Unable to open the file.");
			return;
		}
		tip = "����ʧ��!!!";
		this->ui->textBrowser->append(tip);
		return;
	}
	
	
}

/**
* @func:   ����
* @para:   
* @return: 
*/
void Parser::restart()
{
	if (QMessageBox::No == QMessageBox::question(this, "����", "ȷ��������", QMessageBox::Yes | QMessageBox::No)) {
		return;
	}
	//���
	this->ui->textBrowser->clear();
	this->ui->textEdit->clear();
	this->grammarFile = DEFAULT_GRAMMAR;
	this->loadGrammarFlag = 0;
	this->codeFile = "";
	this->compileFlag = 0;
	this->treeFlag = 0;
	this->lexerFlag = 0;
	this->errorFlag = 0;
	this->dfaFlag = 0;

	QString startTip = "*************************\n";
	startTip += "LR(1)�ʷ��﷨����������\n";
	startTip += "*************************\n";
	startTip += "�뵼���ķ��ļ��Ͳ�����/����Դ����";


	this->ui->textBrowser->setText(startTip);
}


/**
* @func:   չʾ�����λ��
* @para:   int n ����
*          QColor colorҪչʾ����ɫ
* @return: 
*/
void Parser::showErrorLine(int n, QColor color)
{

	// ��ȡ�����е�λ��
	QTextCursor cursor = this->ui->textEdit->textCursor();
	cursor.movePosition(QTextCursor::Start);
	for (int i = 0; i < n-1; ++i) {
		cursor.movePosition(QTextCursor::NextBlock);
	}

	QTextBlockFormat blockFormat = cursor.blockFormat();
	blockFormat.setBackground(color);

	// Ӧ�ø�ʽ������
	cursor.setBlockFormat(blockFormat);
}