#include"token.h"
#include<map>
#include<vector>
#include<stdio.h>

using namespace std;

//��������С����
#define BUFFER_SIZE 300

//�ʷ���������ļ�
#define TOKEN_LIST_FILE "results/Token_List.txt" 
#define ERROR_LIST_FILE "error_log/Lexical_Error_List.txt"


//�洢ÿ��������Ԫ�����ͺ�ֵ
typedef struct elem
{
    string type;
    string value;
    int line;
    elem(string tp, string v, int line);//���캯��
}elem;


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



class lexer :public token {
protected:
    Buffer double_buffer[2];//˫������
    Buffer final_buffer;//��ǰ������buffer
    int current_buffer;//��ǰѡ�еĻ��������
    int comment_flag;//��ǰע�ͱ�־λ


    FILE* fcode;
    FILE* ftoken;
    FILE* ferror;

public:
    vector<elem> lexer_res;//�洢�ʷ������Ľ��
    int error_line;//��¼��������

    lexer();
    virtual ~lexer();

    int lexicalAnalysis(string filename);//�ʷ���������Ҫ����
    void deleComments(int line_count);//���ע�� 
    void deleSpaces(int line_count);//ɾ���ո�

    bool DFA(int line_count);//״̬�� ���ýṹ���buffer���� ���ص�int ת����kindJudge����
    bool tokenJudge(char* str,int line_count);//kindJudge���� �жϴ���� str ���� �������ת���� print����������
    
    
    elem elemGenerate(int kind, char* str,int line_count);//�����﷨������Ԫ���͸��﷨������
};