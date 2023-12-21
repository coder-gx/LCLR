#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <string.h>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>

using namespace std;

//�ļ�����
#define SEMANTIC_ERROR_FILE_NOT_OPEN  -11
#define  QUATERNION_FILE_NOT_OPEN  -21


#define SEMANTIC_ERROR_UNDEFINED 13
#define SEMANTIC_ERROR_PARAMETER_NUM 14
#define SEMANTIC_ERROR_NO_RETURN 15
#define SEMANTIC_ERROR_REDEFINED 16
#define SEMANTIC_ERROR_MAIN_UNDEFINED 17
#define SEMANTIC_ERROR_CALL_FUNCTION_FAIL 19


#define  QUATERNION_FILE  "results/Quaternion_List.txt"
#define SEMANTIC_ERROR_FILE "error_log/Semantic_Error_List.txt"

// ��������еķ���
struct SemanticSymbol
{
	string dataType; // �������͡�ò��û�õ���
	string value;	 // ���ŵľ���ֵ
	int tableIndex;	 // �������ڵķ��ű������б��е�����λ�á�
	int symbolIndex; // ���������������ű��ڵ�����λ�á�
	SemanticSymbol(string d,string v,int t,int s) {
		dataType = d;
		value = v;
		tableIndex = t;
		symbolIndex = s;
	};
};

// ��������п��������Ĵ�������
enum ErrorProcess
{
	UNDEFINED_IDENTIFIER,	 // ��ʶ��δ�������
	INVALID_PARAMETER_COUNT, // ����Ĳ�������
	FUNCTION_REDEFINED,		 // ������ʶ���ض������
	PARAMETER_REDEFINED,	 // ������ʶ���ض������
	IDENTIFIER_REDEFINED,	 // ��ʶ���ض������
	MAIN_FUNCTION_MISSING,	 // main����δ�������
	NO_RETURN, //û�з���ֵ
	CALL_FUNCTION_UNDIFINED, //���ú���û����
	CALL_NOT_FUNCTION //���õĲ��Ǻ���
};

// ��Ԫʽ
struct Quaternion
{
	int sequenceNumber; // ��Ԫʽ���б��е�Ψһ��ʶ����
	string operation;	// ��Ԫʽ�еĲ�������
	string operand1;	// ������1
	string operand2;	// ������2
	string result;		// �����Ľ����Ŀ�������

	// ���캯�������ڳ�ʼ����Ԫʽ�ĸ�����Ա������
	Quaternion(int seqNum, string op, string op1, string op2, string res);
};

// ��ʶ����Ϣ������������������ʱ�����������ľ�����Ϣ
struct IdentifierInfo
{
	// IdentifierCategory ö�ٶ����˱�ʶ���Ĳ�ͬ��𣨺�������������ʱ����������������ֵ��
	enum IdentifierCategory
	{
		Function,	// ����
		Variable,	// ����
		Temporary,	// ��ʱ������ò��û�õ���
		Constant,	// ������ò��û�õ���
		ReturnValue // ����ֵ
	};

	IdentifierCategory category; // ��𣺱�ʶ������𣨺����������ȣ���
	string specifierType;		 // �������ͣ������������ص��������͡�
	string identifierName;		 // ���ƣ���ʶ�������ƻ�����ֵ��
	int parameterCount;			 // ���������������Ĳ���������
	int entryIndex;				 // ���������������ڵ���Ԫʽ��š���ò��û�õ���
	int tableIndex;				 // ���������������ű���ȫ�ַ��ű��б��е���������ò��û�õ���
};

// ��������еķ��ű�
struct SemanticSymbolTable
{
	// SymbolTableCategory ö�ٶ����˷��ű�Ĳ�ͬ���ȫ�ֱ��������鼶����ʱ��
	enum SymbolTableCategory
	{
		GlobalTable,
		FunctionTable,
		BlockTable, // ��ò��û�õ���
		TempTable
	};

	SymbolTableCategory tableCategory; // ���ű�����
	vector<IdentifierInfo> symbols;	   // ���ż����洢�ڴ˱��е����б�ʶ����Ϣ��
	string tableName;				   // ���������ű�����ƣ����ڱ�ʶ�Ͷ�λ��

	// ���캯������ʼ�����ű���������ơ�
	SemanticSymbolTable(const SymbolTableCategory category, const string tableName);

	// ���ݷ��������ڱ��в��ҷ��ŵ�����λ�á�����ҵ����ţ��򷵻����ڷ��ż��е����������򷵻� -1��
	int FindSymbol(const string symbolName);

	// ��һ���µı�ʶ����Ϣ���뵽���ű��С����ؼ����ʶ��������λ�á�
	int InsertSymbol(const IdentifierInfo newIdentifier);
};

// SemanticAnalysis �ඨ����C���Ե������������
// ���������������ɺ͹�����Ԫʽ�����ű��Լ����д�����ķ�����
class SemanticAnalysis
{
private:
	// ���庯��ָ������
	using TranslationFunction = int (SemanticAnalysis::*)(const string, const vector<string>,int pos);

	// ����һ���Ӳ���ʽ�󲿵���������ӳ���
	unordered_map<string, TranslationFunction> translationMap;

public:
	vector<Quaternion> quaternionList; // ��Ԫʽ��
	int mainFunctionIndex;			   // main������Ӧ����Ԫʽ���
	int backpatchingLevel;			   // ������

	vector<int> backpatchingList; // ���������Ԫʽ����Ԫʽ��quaternion_list�е����
	int nextQuaternionIndex;	  // ��ǰλ����һ����Ԫʽ���
	int tempVarCounter;			  // ��ʱ��������

	vector<SemanticSymbol> symbolList;		  // ����������̵ķ��������������Ϊ����������еķ���ջ�������﷨�����е�Ҫ����
	vector<SemanticSymbolTable> symbolTables; // �������з��ű�
	vector<int> scopeStack;					  // ��ǰ�������Ӧ�ķ��ű�����ջ

	// ���캯��
	SemanticAnalysis();
	// �����еķ�����Ϣ����symbol_list
	void AddSymbolToList(const SemanticSymbol symbol);
	// ��������
	int Analysis(const string production_left, const vector<string> production_right,int pos);
	// ��ӡ��Ԫʽ��
	int PrintQuaternion(const string file_path);
	//=====================tool======================================
	bool CheckIdDefine(SemanticSymbol identifier, int* tb_index, int* tb_index_index);
	int CheckParNum(SemanticSymbol check, int* value);
	int ProcessError(SemanticSymbol identifier, int* tb_index, int* tb_index_index, ErrorProcess type,int pos);
	void PopSymbolList(int count);

private:
	// ÿ�η��붼��Ӧ���﷨�����е�һ����Լʽ
	int TranslateProgram(const string production_left, const vector<string> production_right,int pos);
	int TranslateExtDef(const string production_left, const vector<string> production_right,int pos);
	int TranslateVarSpecifier(const string production_left, const vector<string> production_right,int pos);
	int TranslateFunSpecifier(const string production_left, const vector<string> production_right,int pos);
	int TranslateIfStmt(const string production_left, const vector<string> production_right,int pos);
	int TranslateIfStmt_m1(const string production_left, const vector<string> production_right,int pos);
	int TranslateIfStmt_m2(const string production_left, const vector<string> production_right,int pos);
	int TranslateIfNext(const string production_left, const vector<string> production_right,int pos);
	int TranslateIfStmt_next(const string production_left, const vector<string> production_right,int pos);

	//===============================================================
	int TranslateFunDec(const string production_left, const vector<string> production_right,int pos);
	int TranslateCreateFunTable_m(const string production_left, const vector<string> production_right, int pos);
	int TranslateParamDec(const string production_left, const vector<string> production_right, int pos);
	int TranslateBlock(const string production_left, const vector<string> production_right, int pos);
	int TranslateDef(const string production_left, const vector<string> production_right, int pos);
	int TranslateWhileStmt(const string production_left, const vector<string> production_right, int pos);
	int TranslateWhileStmt_m1(const string production_left, const vector<string> production_right, int pos);
	int TranslateWhileStmt_m2(const string production_left, const vector<string> production_right, int pos);

	//==============================================================
	int TranslateAssignStmt(const string production_left, const vector<string> production_right, int pos);
	int TranslateExp(const string production_left, const vector<string> production_right, int pos);
	int TranslateAddSubExp(const string production_left, const vector<string> production_right, int pos);
	int TranslateItem(const string production_left, const vector<string> production_right, int pos);
	int TranslateFactor(const string production_left, const vector<string> production_right, int pos);
	int TranslateCallStmt(const string production_left, const vector<string> production_right, int pos);
	int TranslateCallFunCheck(const string production_left, const vector<string> production_right, int pos);
	int TranslateArgs(const string production_left, const vector<string> production_right, int pos);
	int TranslateReturnStmt(const string production_left, const vector<string> production_right, int pos);
	int TranslateRelop(const string production_left, const vector<string> production_right, int pos);
};