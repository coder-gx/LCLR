#pragma once
#pragma execution_character_set("utf-8") 
#include<iostream>
#include<fstream>
#include<vector>
#include<set>
#include<cstring>
#include"tokenList.h"
using namespace std;


//�ʷ���������ļ�
#define TOKEN_LIST_FILE "results/Token_List.txt" 
#define ERROR_LIST_FILE "error_log/Lexical_Error_List.txt"


#define SUCCESS 1
#define CODE_FILE_OPEN_ERROR 2
#define LEXICAL_ANALYSIS_ERROR 3
#define BUFFER_SIZE 300



class GrammaAnalysis;

struct token
{
    string word;//token������
    int type;//token������
    int line; //��
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
    Buffer double_buffer[2];//˫������
    Buffer final_buffer;//��ǰ������buffer
    int current_buffer;//��ǰѡ�еĻ��������
    int comment_flag;//��ǰע�ͱ�־
    FILE* fcode;
    FILE* ftoken;
    FILE* ferror;
    //ifstream fin;//�����ļ���
    int error_line;
    vector<token>token_vec;//���شʷ��������
    int lexicalAnalysis(string filename);//�ʷ���������Ҫ����
    void deleComments(int line_count);//���ע�� 
    void deleSpaces(int line_count);//ɾ���ո�

    bool DFA(int line_count);//״̬�� ���ýṹ���buffer���� ���ص�int ת����kindJudge����
    bool tokenJudge(char* str, int line_count);//kindJudge���� �жϴ���� str ���� �������ת���� print����������

    token lexer::elemGenerate(int kind, char* str, int line_count);

    lexer();
    ~lexer();

};

