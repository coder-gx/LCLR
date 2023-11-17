#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Parser.h"
#include<qevent.h>
#include <string>

#include "Show.h"
#include "LR1.h"
#include"grammar.h"
#include<qcolor.h>




class Parser : public QMainWindow
{
    Q_OBJECT

public:
    Parser(QWidget *parent = nullptr);
    ~Parser();
private:
    Ui::ParserClass* ui;
    QString codeFile;//Դ���ļ���
    QString grammarFile;//�ķ��ļ���
    bool errorFlag;
    int errorLine;
    bool loadGrammarFlag;
    bool lexerFlag;
    bool compileFlag;
    bool treeFlag;
    bool dfaFlag;

    Show* pageShow;
    lr1Grammar *my_lr1Grammar;
    lexer *my_lexer;
    void loadCode();//����Դ��
    void loadGrammar();//�����ķ�
    void restart(); //�����������
    void compile(); //��ʼ����
    void showErrorLine(int n,QColor color);//չʾ������
};
