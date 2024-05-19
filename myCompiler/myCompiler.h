#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_myCompiler.h"
#include<qevent.h>
#include<qcolor.h>
#include <string>

#include "Show.h"
#include "Objector.h"



class myCompiler : public QMainWindow
{
    Q_OBJECT
    
public:
    myCompiler(QWidget* parent = nullptr);
    ~myCompiler();
private:
    Ui::myCompilerClass* ui;
    QString codeFile;//Դ���ļ���
    QString grammarFile;//�ķ��ļ���
    bool errorFlag;
    int errorLine;
    bool loadGrammarFlag;
    bool grammarFlag;
    bool lexerFlag;
    bool compileFlag;
    bool treeFlag;//�Ѿ�������
    bool dfaFlag;//�Ѿ�����dfa

    Show* pageShow;
    GrammaAnalysis* my_lr1Grammar;
    lexer* my_lexer;
    Optimizer* my_optimizer;
    ObjectCodeGenerator* my_objector;

    void loadCode();//����Դ��
    void loadGrammar();//�����ķ�
    void restart(); //�����������
    void compile(); //��ʼ����
    void showErrorLine(int n, QColor color);//չʾ������
    void printTime(int ms);//��ӡ�����ʱʱ��
    int readGrammar();
};
