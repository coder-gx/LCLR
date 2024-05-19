#include "myCompiler.h"
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
#include<ctime>
//#include"Show.h"
 
#define OBJECT_CODE_FILE "results/Object_Code.txt"
#define DEFAULT_GRAMMAR "Grammar.txt"
#define CODE_FILE "Input_Code.txt"
//#define QUESTION_PATH "question.txt"

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
* @func:   myCompiler���캯��
* @para:
* @return:
*/
 myCompiler::myCompiler(QWidget* parent) : QMainWindow(parent)
{

	this->ui = new Ui::myCompilerClass;
	this->ui->setupUi(this);
	//resize(800, 600);
	this->compileFlag = 0;
	this->loadGrammarFlag = 0;
	this->treeFlag = 0;
	this->lexerFlag = 0;
	this->errorFlag = 0;
	this->errorLine = 0;
	this->dfaFlag = 0;


	
	this->codeFile = "";//codefileΪ��
	this->pageShow = new Show;

	this->my_lr1Grammar = new GrammaAnalysis;

	this->grammarFile = DEFAULT_GRAMMAR;


	int ans = readGrammar();
	if (ans == 0) {
		QString tip = "����ʧ��!!!";
		this->ui->textBrowser->append(tip);
	}

	//�����������ķ�
	connect(this->ui->action_Code, &QAction::triggered, this, &myCompiler::loadCode);//�ǵ����������print���,״̬ת��
	connect(this->ui->action_Grammar, &QAction::triggered, this, &myCompiler::loadGrammar);//д��my_grammar.txt��

	//�����������
	connect(this->ui->action_Run, &QAction::triggered, [=]()
		{
			if (this->grammarFlag == 0) {
				QMessageBox::critical(this, "����", "û��֧�ֱ������Ч�ķ�");
			}
			else {
				//�ķ��ļ�
				if (this->loadGrammarFlag == 0) {
					if (QMessageBox::No == QMessageBox::question(this, "����", "��ǰ��û�е����ķ��ļ���ȷ��ʹ��Ĭ���ļ�" + QString::fromLocal8Bit(DEFAULT_GRAMMAR), QMessageBox::Yes | QMessageBox::No)) {
						return;
					}
				}
				this->compile();
			}
		});//�ǵ����������print���,״̬ת��
	connect(this->ui->action_Restart, &QAction::triggered, this, &myCompiler::restart);//д��my_grammar.txt�� 

	//չʾ�ʷ�dfa
	connect(this->ui->action_DFA, &QAction::triggered, [=]() {
		// ��ʾվ���޸�ҳ�� չʾҳ��
		if (this->lexerFlag == 0) {
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
			//this->pageShow->showloading();
			this->pageShow->show();
		}
		});


	// չʾ�ʷ�token
	connect(this->ui->action_Token, &QAction::triggered, [=]() {
		// ��ʾվ���޸�ҳ�� չʾҳ��
		if (this->lexerFlag == 0) {
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
		if (this->compileFlag == 0) {
			QMessageBox::critical(this, "����", "������ɱ���");
		}
		else {
			this->pageShow->showProcess();
			//this->pageShow->showloading();
			this->pageShow->show();
		}
		});


	// չʾ�﷨������
	connect(this->ui->action_Tree, &QAction::triggered, [=]() {
		// ��ʾվ���޸�ҳ�� չʾҳ��
		if (this->compileFlag == 0) {
			QMessageBox::critical(this, "����", "������ɱ���");
		}
		else if (this->treeFlag == 0) {
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

	// չʾ�м�������ɵ���Ԫʽ
	connect(this->ui->action_Quaternion, &QAction::triggered, [=]() {
		// ��ʾվ���޸�ҳ�� չʾҳ��
		if (this->compileFlag == 0) {
			QMessageBox::critical(this, "����", "������ɱ���");
		}
		else {
			this->pageShow->showQuaternion();
			this->pageShow->show();
		}
		});
	// չʾ������
	connect(this->ui->action_Var, &QAction::triggered, [=]() {
		// ��ʾվ���޸�ҳ�� չʾҳ��
		if (this->compileFlag == 0) {
			QMessageBox::critical(this, "����", "������ɱ���");
		}
		else {
			this->pageShow->showVar();
			this->pageShow->show();
		}
		});
	// չʾ������
	connect(this->ui->action_Func, &QAction::triggered, [=]() {
		// ��ʾվ���޸�ҳ�� չʾҳ��
		if (this->compileFlag == 0) {
			QMessageBox::critical(this, "����", "������ɱ���");
		}
		else {
			this->pageShow->showFunc();
			this->pageShow->show();
		}
		});

	// չʾĿ�����
	connect(this->ui->actionMIPS_code, &QAction::triggered, [=]() {
		// ��ʾվ���޸�ҳ�� չʾҳ��
		if (this->compileFlag == 0) {
			QMessageBox::critical(this, "����", "������ɱ���");
		}
		else {
			this->pageShow->showObjectCode();
			this->pageShow->show();
		}
		});






	QString startTip = "*************************\n";
	startTip += "LR(1)�ʷ��﷨����������\n";
	startTip += "*************************\n";
	startTip += "�뵼���ķ��ļ��Ͳ�����/����Դ����";


	this->ui->textBrowser->setText(startTip);

}

myCompiler::~myCompiler()
{

}

/**
* @func:   �����ķ�
* @para:
* @return:
*/
void myCompiler::loadGrammar()
{
	if (QMessageBox::Yes == QMessageBox::question(this, "��ʾ", "�����ķ�ʱ����ܽϳ��������ĵȴ���", QMessageBox::Yes | QMessageBox::No))
	{
		QString fileName = QFileDialog::getOpenFileName(this, "�����ļ�", ".", "files(*.txt)");
		if (!fileName.isEmpty()) {




			this->grammarFile = fileName;
			this->loadGrammarFlag = 1;
			this->ui->textBrowser->append("\n�ķ��ļ�����ɹ�");
			delete this->my_lr1Grammar;
			this->my_lr1Grammar = new GrammaAnalysis;
			int ans = readGrammar();
			if (ans == 0) {
				QString tip = "����ʧ��!!!";
				this->ui->textBrowser->append(tip);
			}
			QMessageBox::information(this, "��ʾ", "�����ķ��ļ��ɹ�");

		}
		else {
			this->ui->textBrowser->append("\n���ļ�������ʧ��");
			QMessageBox::critical(this, "����", "�����ķ��ļ�ʧ��");
		}


	}

}

/**
* @func:   ������뵽�ı�����
* @para:
* @return:
*/
void myCompiler::loadCode()
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
			getline(fcode, line);
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
void myCompiler::compile()
{
	time_t start, end;
	start = clock();

	this->compileFlag = 0;
	this->treeFlag = 0;
	this->lexerFlag = 0;
	if (this->errorFlag == 1) {
		this->showErrorLine(this->errorLine, Qt::white);
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
	
	int lexer_error = this->my_lexer->lexicalAnalysis(CODE_FILE);

	/*for (int i = 0; i < int(this->my_lexer->token_vec.size()); i++)
	{
		cout << "(" << this->my_lexer->token_vec[i].word << ", " << this->my_lexer->token_vec[i].type << ")" << endl;
	}

	cout << "end" << endl;*/


	if (lexer_error == SUCCESS) {
		tip = "�ʷ���������\n";
	}
	else if (lexer_error == LEXICAL_ANALYSIS_ERROR) {
		end = clock();

		tip = "�ʷ���������,������־�ļ���������\n";
		this->ui->textBrowser->append(tip);
		this->showErrorLine(this->my_lexer->error_line, Qt::red);
		this->errorLine = this->my_lexer->error_line;
		this->errorFlag = 1;
		//�Ѽ�¼������ļ�������
		QFile file(ERROR_LIST_FILE);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream in(&file);
			in.setCodec("UTF-8");
			// ��ȡ�ļ����ݲ�д�� TextBrowser
			this->ui->textBrowser->append(in.readAll());
			file.close();
		}
		else {
			this->ui->textBrowser->append("Error: Unable to open the file.");
			return;
		}
		printTime(int(end - start));
		QMessageBox::critical(this, "����", "�ʷ�����ʧ�ܣ�����ʧ��!");
		tip += "����ʧ��!!!";
		this->ui->textBrowser->append(tip);

		return;
	}
	else {
		this->ui->textBrowser->append("\nԴ�����ļ�����ʧ��");
		QMessageBox::critical(this, "����", "����Դ�����ļ�ʧ��");
		return;
	}
	tip += "�ʷ������ѽ��������ڵ�ǰĿ¼�²鿴\n�ʷ���������ļ�results/Token_List.txt\n";
	this->ui->textBrowser->append(tip);
	this->lexerFlag = 1;



	tip = "\n���ڽ����﷨���������Ժ�...\n";
	this->ui->textBrowser->append(tip);

	int lr1_error = my_lr1Grammar->Forward(my_lexer);
	//��ӡ�м�����������������
	
	if (lr1_error == SUCCESS) {
		this->my_lr1Grammar->SemanticProcess.PrintCodeTable();
		this->my_lr1Grammar->SemanticProcess.PrintVarTable();
		this->my_lr1Grammar->SemanticProcess.PrintFuncTable();


		tip = "\n���ڽ����Ż�������Ŀ����룬���Ժ�...\n";
		this->ui->textBrowser->append(tip);

		this->my_optimizer = new Optimizer(this->my_lr1Grammar->SemanticProcess.VarTable, this->my_lr1Grammar->SemanticProcess.FuncTable, this->my_lr1Grammar->SemanticProcess.MiddleCodeTable);
	    

		for (int i = 0; i < (int)my_optimizer->MiddleCodeTable.size(); i++)
		{
			cout << i << ":";
			cout << "(";
			cout << my_optimizer->MiddleCodeTable[i].op<<' '<< my_optimizer->MiddleCodeTable[i].linkres<<' '<< my_optimizer->MiddleCodeTable[i].op1 <<' '<< my_optimizer->MiddleCodeTable[i].op2<< ")" << endl;
		}

		this->my_objector = new ObjectCodeGenerator(my_optimizer->VarTable, my_optimizer->FuncTable, my_optimizer->MiddleCodeTable, my_optimizer->CodeBlock, OBJECT_CODE_FILE);
		

		my_objector->Forward();
	}


	end = clock();

	if (lr1_error == ACTION_ERROR) {
		tip = "�ķ�����������ACTION����ҳ���\n";
		this->ui->textBrowser->append(tip);
		QMessageBox::critical(this, "����", "ACTION�����ʧ��");
	}
	else if (lr1_error == GOTO_ERROR) {
		tip = "�ķ�����������GOTO����ҳ���\n";
		this->ui->textBrowser->append(tip);
		QMessageBox::critical(this, "����", "GOTO�����ʧ��");
	}
	else if (lr1_error == REDUCTION_PROCESS_FILE_OPEN_ERROR) {
		this->ui->textBrowser->append("\n��Լ���̼�¼ʧ��");
		QMessageBox::warning(this, "����", "��Լ���̼�¼ʧ��");
		return;
	}
	else if (lr1_error == PARSE_ERROR_FILE_OPEN_ERROR) {
		this->ui->textBrowser->append("\n�﷨���������¼ʧ��");
		QMessageBox::warning(this, "����", "�﷨���������¼ʧ��");
		return;
	}
	else if (lr1_error ==SEMANTIC_ERROR) {
		tip = "�����������\n";
		this->ui->textBrowser->append(tip);
		QMessageBox::critical(this, "����", "����������ִ���");
	}
	else {

		tip = "�﷨�����ѽ��������ڵ�ǰĿ¼�²鿴��\n�ķ���Լ�����ļ�results/Reduction_Process.txt\n\n";
		printTime(int(end - start));
		tip += "����ɹ�������\n";
		this->ui->textBrowser->append(tip);

		this->compileFlag = 1;
		this->errorFlag = 0;

		if (lr1_error == TREE_DOT_FILE_OPEN_ERROR) {
			this->ui->textBrowser->append("\n�﷨������ʧ��");
			QMessageBox::warning(this, "����", "�﷨������ʧ��");
			return;
		}
		else {
			//const wchar_t* command = L"dot --version";
			// ʹ�� popen ִ�������ȡ���
			//HWND consoleWindow = GetConsoleWindow();

			// ���ؿ���̨����
			//ShowWindow(consoleWindow, SW_HIDE);

			//int a = WinExec("dot -Tpng results/Parse_Tree.dot -o picture/Parse_Tree.png", SW_HIDE);
			int a = system("dot -Tpng results/Parse_Tree.dot -o picture/Parse_Tree.png");
			if (a != 0) {
				QMessageBox::warning(this, "����", "û�а�װgraphviz,�޷������﷨������");
				return;
			}
			tip = "�﷨�����������ɣ��ɲ鿴picture/Parse_Tree.png��";
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

				tip = "��������DFA...";
				this->ui->textBrowser->append(tip);
				system("dot -Tpng results/Parse_DFA.dot -o picture/Parse_DFA.png");

				tip = "�﷨����DFA�����ɣ��ɲ鿴picture/Parse_DFA.png��";
				this->ui->textBrowser->append(tip);
				this->dfaFlag = 1;
			}
		}

	}
	//std����cout << "lr1" << lr1_error;
	if (lr1_error != SUCCESS) {
		this->showErrorLine(this->my_lr1Grammar->ErrorLine, Qt::red);
		this->errorLine = this->my_lr1Grammar->ErrorLine;
		this->errorFlag = 1;
		QFile file(PARSE_ERROR_FILE);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream in(&file);
			in.setCodec("UTF-8");
			// ��ȡ�ļ����ݲ�д�� TextBrowser
			this->ui->textBrowser->append(in.readAll());
			file.close();
		}
		else {
			this->ui->textBrowser->append("Error: Unable to open the file.");
			return;
		}
		if (lr1_error == SEMANTIC_ERROR) {
			QFile file2(SEMANTIC_ERROR_FILE);

			if (file2.open(QIODevice::ReadOnly | QIODevice::Text)) {
				QTextStream in(&file2);
				// ��ȡ�ļ����ݲ�д�� TextBrowser
				in.setCodec("UTF-8");
				this->ui->textBrowser->append(in.readAll());
				file2.close();
			}
			else {
				this->ui->textBrowser->append("Error: Unable to open the file.");
				return;
			}
		}
		printTime(int(end - start));
		tip = "����ʧ��!!!";
		this->ui->textBrowser->append(tip);

		return;
	}


}
int myCompiler::readGrammar()
{
	this->grammarFlag = 0;
	//�ķ�����
	QString tip = "���ڶ�ȡ�ķ��ļ������Ժ�...\n";
	this->ui->textBrowser->append(tip);

	//���ķ�����ȡ����
	int grammar_error = this->my_lr1Grammar->ProduceFormsGenerate((string)this->grammarFile.toLocal8Bit());
	if (grammar_error == SUCCESS) {
		this->my_lr1Grammar->BuildDFA();
		grammar_error = this->my_lr1Grammar->BuildActionGo();
	}
	
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
			in.setCodec("UTF-8");
			// ��ȡ�ļ����ݲ�д�� TextBrowser
			this->ui->textBrowser->append(in.readAll());
			file.close();
		}
		else {
			this->ui->textBrowser->append("Error: Unable to open the file.");
			return 0;
		}
		QMessageBox::critical(this, "����", "�ķ�����!");
		return 0;
	}
	else if (grammar_error == GRAMMAR_FILE_OPEN_ERROR) {
		this->ui->textBrowser->append("\n�ķ��ļ�����ʧ��");
		QMessageBox::critical(this, "����", "�����ķ��ļ�ʧ��");
		return 0;
	}
	
	this->my_lr1Grammar->PrintActionGoToFile();

	tip = "�ķ��Ѷ��룬����results/Tables.csv�²鿴��Action Goto��\n";
	this->ui->textBrowser->append(tip);
	this->grammarFlag = 1;
	return 1;
}



void myCompiler::printTime(int ms)
{
	struct tm cur_time;
	time_t now;
	time(&now);
	localtime_s(&cur_time, &now);

	string allt = "compile time consumed: " + to_string(ms) + "ms" + "\ncompile moment: " + to_string((int)cur_time.tm_year % 100 + 2000) +
		"/" + to_string((int)(cur_time.tm_mon + 1)) + "/" + to_string((int)cur_time.tm_mday) + "   " + to_string((int)cur_time.tm_hour) + ":"
		+ to_string((int)cur_time.tm_min) + ":" + to_string((int)cur_time.tm_sec);
	//cout << allt;
	QString tip = QString::fromLocal8Bit(allt.c_str());
	this->ui->textBrowser->append(tip);
}


/**
* @func:   ����
* @para:
* @return:
*/
void myCompiler::restart()
{
	if (QMessageBox::No == QMessageBox::question(this, "����", "����ʱ��ϳ�(Լ10s)��ȷ��������", QMessageBox::Yes | QMessageBox::No)) {
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
	delete this->my_lr1Grammar;
	this->my_lr1Grammar = new GrammaAnalysis;
	int ans = readGrammar();
	if (ans == 0) {
		QString tip = "����ʧ��!!!";
		this->ui->textBrowser->append(tip);
	}
}


/**
* @func:   չʾ�����λ��
* @para:   int n ����
*          QColor colorҪչʾ����ɫ
* @return:
*/
void myCompiler::showErrorLine(int n, QColor color)
{

	// ��ȡ�����е�λ��
	QTextCursor cursor = this->ui->textEdit->textCursor();
	cursor.movePosition(QTextCursor::Start);
	for (int i = 0; i < n - 1; ++i) {
		cursor.movePosition(QTextCursor::NextBlock);
	}

	QTextBlockFormat blockFormat = cursor.blockFormat();
	blockFormat.setBackground(color);

	// Ӧ�ø�ʽ������
	cursor.setBlockFormat(blockFormat);
}