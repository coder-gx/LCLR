#pragma once

#include<iostream>
#include<vector>
#include<set>
#include<string>
#include<fstream>
#include<algorithm>


using namespace std;

//�ķ���������
#define GRAMMAR_FILE_OPEN_ERROR 2
#define SUCCESS 1
#define GRAMMAR_ERROR 3
#define GRAMMAR_ERROR_FILE_OPEN_ERROR 4

//�ļ�����
#define GRAMMAR_ERROR_FILE "error_log/Grammar_Error_List.txt"
#define EXTENDED_GRAMMAR_FILE "results/Extended_Grammar.txt"
#define FIRST_SET_FILE "results/First_Set.txt"

//�ķ�����
const string EpsilonToken = "@";
const string SplitToken = " | ";// ����ʽ�Ҳ��ָ���
const string ProToken = "::=";// ����ʽ���Ҳ��ָ���
const string EndToken = "#";// βtoken ��ֹ����
const string StartToken = "Program";// �ķ���ʼ����
const string ExtendStartToken = "S";// ��չ�ķ���ʼ����
const string AllTerminalToken = "%token";//���е��ս��

//�ķ����ŵ�����
typedef enum
{
    unknown,   //δ����
    terminal,	   //�ս��
    nonterminal,	   //���ս��
    epsilon,       //��
    end             //������
}symbolType;


//���������������
typedef struct symbol {
    symbolType type;//�ķ���������
    set<int> first_set;//��¼��symbol��first��,�����¼��������
    string name;//������
    symbol(symbolType type, const string name) {
        this->type = type;
        this->name = name;
    }
}symbol;

//��������ʽ�Ľṹ
typedef struct production {
    int left_symbol;
    vector<int> right_symbol;//�����¼��������

    production(const int left, const vector<int>& right) {
        this->left_symbol = left;
        this->right_symbol = right;
    }
}production;



class grammar {
public:

    vector<symbol>symbols;//���еķ��ű�
    set<int>terminals;//�ս����symbol�е��±�
    set<int>non_terminals;//���ս����symbol�е��±�
    vector<production>productions;//���еĲ���ʽ

    int start_index;//��ʼ����ʽ��productions�е�λ��

    grammar();
    ~grammar();

    //��file�ж���grammar
    int ReadGrammar(const string file_path);
    bool checkGrammar(ofstream& ferr); //�����ع��ķ�
    void printExtendedGrammar();   //��ӡ��չ�ķ�

    int symbolIndex(const string token);//�ҵ��÷��ŵ�����

    //��ʼ��first����
    void initFirst();
    void initFirstTerm();
    void initFirstNonTerm();
    void printFirst();
   
    //����һ�����Ŵ���first����
    set<int>getFirst(const vector<int>& str);
};


//���ߺ���
vector<string> split(const string &str, const string& pattern);
void Trimmed(std::string& str);
int mergeSet(set<int>& src, set<int>& dst, int null_index);