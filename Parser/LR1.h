#pragma once
#include "grammar.h"
#include "lexer.h"
#include <vector>
#include <unordered_map>
#include <queue>
#include <fstream>
#include <iomanip>


#define Table_FILE  "results/ACTION_GOTO_Table.csv" //������ַ
#define REDUCTION_PROCESS_FILE "results/Reduction_Process.txt" //�����Լ��ַ
#define TREE_DOT_FILE  "results/Parse_Tree.dot"   //��ͼdot�ļ���ַ
#define TREE_PNG_FILE   "picture/Parse_Tree.png" //�﷨��ͼƬ��ַ
#define PARSE_ERROR_FILE "error_log/Parse_Error.txt"
#define DFA_DOT_FILE   "results/Parse_DFA.dot"


// �ֱ������������͵� Hash Value Ȼ������ǽ��� Combine �õ�һ����ϣֵ
// һ��ֱ�Ӳ�����λ�����XOR���õ���ϣֵ
struct HashFunc
{
	template<typename T, typename U>
	size_t operator()(const std::pair<T, U>& p) const {
		return std::hash<T>()(p.first) ^ std::hash<U>()(p.second);
	}
};

// ��ֵ�Ƚϣ���ϣ��ײ�ıȽ϶��壬��Ҫֱ�������Զ�������Ƿ����
struct EqualKey {
	template<typename T, typename U>
	bool operator ()(const std::pair<T, U>& p1, const std::pair<T, U>& p2) const {
		return p1.first == p2.first && p1.second == p2.second;
	}
};



//LR1��ÿһ���Ƶ�ʽ
class lr1Item
{
public:
	int left;//��������ű��
	vector<int> right;//�Ҳ������ű��
	int dot_pos;//���ĵ��λ��
	int forward;//��ǰ���ķ��ű�� 
	int grammar_index;//���LR1�����ĸ�����ʽ������,��ʵ�������࣬�����index���Ѿ�����left��right
public:
	
	lr1Item() { left = 0; dot_pos = 0; forward = 0; grammar_index = 0; right.push_back(0); };
	lr1Item(int l, vector<int>& r, int ds, int fw, int gi);
	bool operator==(const lr1Item& item);
	void lr1ItemInit(int l, vector<int>& r, int ds, int fw, int gi);
};


//LR(1)�հ�
class lr1Closure
{
public:
	vector<lr1Item> closure;//��Ŀ�հ�
public:
	bool isIn(lr1Item item);//���ظ���Ŀ�Ƿ��ڸñհ���
	bool operator==(lr1Closure& clos);
	unordered_map<int, vector<int>> getShiftinSymbol();//�õ����ƽ����ַ��Լ���Ŀ�ڱհ��е�λ��
	vector<pair<int, int>> getReduceSymbol();//�õ����Թ�Լ�ķ��źͶ�Ӧ�Ĳ���ʽ�����
	
};


//ACTION���п��Դ��ڵĶ���
enum actionOption
{
	SHIFT_IN,//�ƽ�
	REDUCE,//��Լ
	ACCEPT,//����
	REJECT//�ܾ�
};

//GOTO���п��Դ��ڵĶ���
enum gotoOption
{
	GO,
	REJECT_
};

class ACTIONItem
{
public:
	actionOption op;
	int serial;//��ţ�����op���ƽ�����Լ����ܶ��в�ͬ�ĺ���
	ACTIONItem(actionOption ac_op, int num) {
		this->op = ac_op;
		this->serial = num;
	}
	ACTIONItem(){}
	
};

class GOTOItem
{
public:
	gotoOption op;
	int serial;//��ţ���GOTO����ֻ��ת�Ʊ�ţ����Ǳհ����еı��
	GOTOItem(gotoOption goto_op, int num) {
	   this->op = goto_op;
	   this->serial = num;
	}
	GOTOItem(){}
	
};

//LR(1)�ķ��ܹ���
class lr1Grammar :public grammar
{
public:
	int error_line;

	vector<lr1Item> item_sum;//�����е���Ŀ��setû�б��
	vector<lr1Closure> closure_sum;//���п��ܳ��ֵıհ����൱�ڱ����
	unordered_map<pair<int, int>, int,HashFunc,EqualKey> DFA;//map<<closure�ı�ţ����ŵı��>��Ŀ��closure���>
	
	unordered_map<pair<int, int>, ACTIONItem, HashFunc, EqualKey> ACTION;//ACTION��
	unordered_map<pair<int, int>, GOTOItem, HashFunc, EqualKey> GOTO;

	lr1Item start_item; //��ʼ��Ŀ
	lr1Closure start_closure; //��ʼ��Ŀ�հ�
public:
	lr1Grammar() {};

	void initClosure0(); //��grammar�̳е�productions���ӿ�ʼ����ʽ��ʼ��ʹ����Ŀ���е�һ���Ǳհ�
	lr1Closure generateClosure(vector<lr1Item>);//������Ŀ����հ�
	int getClosureIndex(lr1Closure& clos);//�жϱհ��������Ƿ��иñհ������з�����ţ���û�з���-1
	void generateDFA();//�õ����бհ�����ʼ�հ���0�űհ���������ͬʱȷ��DFA


	void generateACTION_GOTO();//����ACTION���GOTO��
	void printACTION_GOTO();//��ӡACTION��GOTO��Ϊcsv�ļ�
    
	int reduction(vector<elem>& lexical_res);//���й�Լ���ڹ����н��д�ӡ

	int printParseDFA();//����DFA
};