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
    QString codeFile;//源码文件名
    QString grammarFile;//文法文件名
    bool errorFlag;
    int errorLine;
    bool loadGrammarFlag;
    bool grammarFlag;
    bool lexerFlag;
    bool compileFlag;
    bool treeFlag;//已经生成树
    bool dfaFlag;//已经生成dfa

    Show* pageShow;
    GrammaAnalysis* my_lr1Grammar;
    lexer* my_lexer;
    Optimizer* my_optimizer;
    ObjectCodeGenerator* my_objector;

    void loadCode();//导入源码
    void loadGrammar();//导入文法
    void restart(); //重启编译程序
    void compile(); //开始编译
    void showErrorLine(int n, QColor color);//展示错误行
    void printTime(int ms);//打印编译耗时时间
    int readGrammar();
};
