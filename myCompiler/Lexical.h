#pragma once
#pragma execution_character_set("utf-8") 
#include<iostream>
#include<fstream>
#include<vector>
#include<set>
#include<cstring>
#include"tokenList.h"
using namespace std;


//词法分析相关文件
#define TOKEN_LIST_FILE "results/Token_List.txt" 
#define ERROR_LIST_FILE "error_log/Lexical_Error_List.txt"


#define SUCCESS 1
#define CODE_FILE_OPEN_ERROR 2
#define LEXICAL_ANALYSIS_ERROR 3
#define BUFFER_SIZE 300



class GrammaAnalysis;

struct token
{
    string word;//token的内容
    int type;//token的类型
    int line; //行
    token(string word, int type, int line);
};


class Buffer {
public:
    char* buffer;
    int count;
    Buffer() {
        count = 0;
        buffer = new char[BUFFER_SIZE];
    }
    ~Buffer() {
        delete buffer;
    }
};


class myCompiler;
class lexer
{
    friend myCompiler;
    friend GrammaAnalysis;

public:
    Buffer double_buffer[2];//双缓冲区
    Buffer final_buffer;//当前分析的buffer
    int current_buffer;//当前选中的缓冲区编号
    int comment_flag;//当前注释标志
    FILE* fcode;
    FILE* ftoken;
    FILE* ferror;
    //ifstream fin;//输入文件流
    int error_line;
    vector<token>token_vec;//返回词法分析结果
    int lexicalAnalysis(string filename);//词法分析的主要函数
    void deleComments(int line_count);//清除注释 
    void deleSpaces(int line_count);//删除空格

    bool DFA(int line_count);//状态机 调用结构体的buffer变量 返回的int 转交给kindJudge函数
    bool tokenJudge(char* str, int line_count);//kindJudge函数 判断传入的 str 类型 并将结果转交给 print函数完成输出

    token lexer::elemGenerate(int kind, char* str, int line_count);

    lexer();
    ~lexer();

};

