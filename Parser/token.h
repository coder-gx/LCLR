#include<iostream>
#include<string>
#include<vector>
#define _CRT_SECURE_NO_WARNINGS
//#pragma warning(disable: 4996)

using namespace std;

//�����ս���ı��
#define my_Key_Word  1
#define my_SignWord 2
#define my_Integer 3
#define my_FloatPoint 4
#define my_MonocularOperator 5    // ��Ŀ�����
#define my_BinocularOperator 6    // ˫Ŀ�����
#define my_Delimiter 7            // ���
#define my_WrongWord 8            // ����
#define my_Blank 9                // �ո�
#define my_Separator 10           // �ָ���
#define my_BracketsLeft 11        // ������
#define my_BracketsRight 12       // ������
#define my_BracketsLeftBig 13     // �������
#define my_BracketsRightBig 14    // �Ҵ�����
#define my_End 15                 // ������
#define my_Str 16                 //�ַ���
#define my_Char 17                   //�ַ�
#define my_Brackets_Left_Square 18   //������
#define my_Brackets_Right_Square 19   //�ҷ�����
#define my_Point_Arrow 20             //��ͷ
#define my_Region 21                  //���
#define my_Region_Xigou 22            //������
#define my_Point 23                   //���
#define my_Colon 24                   //ð��
#define my_Question_Mark 25

//������ż���С
#define  KEY_WORD_SIZE  24
#define  MONOCULAR_OPERATOR_SIZE  13
#define  BINOCULAR_OPERATOR_SIZE  14

//�������token������   //�ؼ���


const string Key_Word[KEY_WORD_SIZE] = {"break","case","char","class","continue","do","default","double","define",
"else","float","for","if","int","include","long","main","return","switch","typedef","void","unsigned","while","iostream" };//24��

const char Delimiter = ';' ; //���
const char Seprater = ',';
const char Brackets_Left = '(';
const char Brackets_Right = ')';
const char Brackets_Left_Big = '{';
const char Brackets_Right_Big = '}';
const char End = '#';
const char Point = '.';
const char Brackets_Left_Square = '[';
const char Brackets_Right_Square = ']';
const char Point_Arrow[5] = "->";
const char Region[5] = "::";
const char Region_Xigou[5] = "::~";
const char Colon = ':';
const char Question_Mark = '?';


const string Monocular_Operator[MONOCULAR_OPERATOR_SIZE] = {"+","-","*","/","!","%","~","&","|","^", "=" ,">","<"};   //��Ŀ����� 13��
const string Binocular_Operator[BINOCULAR_OPERATOR_SIZE]={"++","--","&&","||","<=","!=","==",">=","+=","-=","*=","/=","<<",">>"}; //˫Ŀ����� 14��

/****************************************
 * token ��
 * ����ս�������������ж�
 * ***************************************/

class token {
public:
    
    virtual ~token();     // ����������������̳�
    int toState(char c);// �жϵ�ǰDFAҪת���״̬
    bool spaceRemovable(char c);//�жϿո��ܷ�ɾ��


    //���������ж�
    int isDelimiter(char c);//���
    int isDelimiter(string c);//���(���أ��ж��ַ�����)
    int isSeparator(char c);//�ָ���
    int isBracketsLeft(char c);//������
    int isBracketsRight(char c);//������
    int isBracketsLeftBig(char c);//�������
    int isBracketsRightBig(char c);//�Ҵ�����
    int isPoint(char c);//.
    int isBracketsLeftSquare(char c);//[
    int isBracketsRightSquare(char c);//]
    int isPointArrow(string str);//->
    int isRegion(string str);//::
    int isRegionXigou(string str);//::~
    int isColon(char c);//:
    int isEnd(char c);//������
    int isStr(string str);//�ַ���
    int isChar(string str);//�ִ����ַ�
    int isInt(string str);//����

    int isFloat(string str);//float �� +-xx.xx e +-xx.xx
    int hasDot(string str);//float�� +-xx.xx
    
    int isSignWord(string str);// ��ʶ�� 
    int isKey_Word(string str);//������  �ؼ��� 


    int isBinocularOperator(string str);//�ж�˫Ŀ�����
    int isMonocularOperator(string str);//�жϵ�Ŀ�����

    int isBlank(string str);//�жϿո�
    int isQuestion_Mark(char c);
};