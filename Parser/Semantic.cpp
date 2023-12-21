#include "semantic.h"

using namespace std;

/**
 * @brief�� ������Ԫʽ����
 * @param�� int seqNum ������Ԫʽ�����кţ�����Ψһ��ʶÿ����Ԫʽ��
 * @param�� string op ��ʾ��Ԫʽ�еĲ������ͣ���ӷ�����ֵ�ȡ�
 * @param�� string op1 ��ʾ�����ĵ�һ�������������ݲ������Ϳ����Ǳ�����������������
 * @param�� string op2 ��ʾ�����ĵڶ���������������һԪ�������ֶο���Ϊ�ա�
 * @param�� string res ��ʾ�����Ľ����Ŀ�������
 * @return  ������ֵ��������һ����ʼ������Ԫʽ����
 * */
Quaternion::Quaternion(int seqNum, string op, string op1, string op2, string res)
{
	this->sequenceNumber = seqNum;
	this->operation = op;
	this->operand1 = op1;
	this->operand2 = op2;
	this->result = res;
}

/**
 * @brief�� ����һ���µķ��ű����
 * @param�� const SymbolTableCategory tableCategory ���ű�������ȫ�ֱ�������ȣ���
 * @param�� const string tableName ���ű�����ƣ����ڱ�ʶ�Ͷ�λ���ű�
 * @return  ������ֵ��������һ����ʼ���ķ��ű����
 * */
SemanticSymbolTable::SemanticSymbolTable(const SymbolTableCategory tableCategory, const string tableName)
{
	this->tableCategory = tableCategory;
	this->tableName = tableName;
}

/**
 * @brief�� �ڷ��ű��в���ָ�����Ƶķ��š�
 * @param�� const string id_name Ҫ���ҵķ��ŵ����ơ�
 * @return  ����ҵ����ţ��򷵻ط����ڷ��ű��е����������򷵻� -1��
 * */
int SemanticSymbolTable::FindSymbol(const string id_name)
{
	for (int i = 0; i < this->symbols.size(); i++)
	{
		if (id_name == this->symbols[i].identifierName)
			return i;
	}
	return -1;
}

/**
 * @brief�� ����ű��в���һ���µķ��š�
 * @param�� const IdentifierInfo ident Ҫ����ķ�����Ϣ��
 * @return  ����ɹ������·��ţ��򷵻��·����ڷ��ű��е���������������Ѵ��ڣ��򷵻� -1��
 * */
int SemanticSymbolTable::InsertSymbol(const IdentifierInfo ident)
{
	int symbolIndex = this->FindSymbol(ident.identifierName);
	if (symbolIndex == -1)
	{
		this->symbols.push_back(ident);
		return this->symbols.size() - 1;
	}
	else
		return -1;
}

/**********************************************/
/**
 * @brief�� ����SemanticAnalysis���ʵ����
 *          ��ʼ��ȫ�ַ��ű���ʱ������������Ԫʽ�����ʼֵ���Լ���ʼ��������س�Ա��
 * @param��
 * @return
 */
SemanticAnalysis::SemanticAnalysis()
	: nextQuaternionIndex(1), // �ӱ��1��ʼ������Ԫʽ��ţ�0������ (j, -, -, main_address)
	mainFunctionIndex(-1),  // main�����ı������Ϊ-1
	backpatchingLevel(0),	  // ��ʼ������Ϊ0����ʾ����Ҫ����
	tempVarCounter(0)		  // ��ʱ��������������
{
	// ��ʼ��ӳ���
	translationMap["Program"] = &SemanticAnalysis::TranslateProgram;
	translationMap["ExtDef"] = &SemanticAnalysis::TranslateExtDef;
	translationMap["VarSpecifier"] = &SemanticAnalysis::TranslateVarSpecifier;
	translationMap["FunSpecifier"] = &SemanticAnalysis::TranslateFunSpecifier;
	translationMap["IfStmt"] = &SemanticAnalysis::TranslateIfStmt;
	translationMap["IfStmt_m1"] = &SemanticAnalysis::TranslateIfStmt_m1;
	translationMap["IfStmt_m2"] = &SemanticAnalysis::TranslateIfStmt_m2;
	translationMap["IfNext"] = &SemanticAnalysis::TranslateIfNext;
	translationMap["IfStmt_next"] = &SemanticAnalysis::TranslateIfStmt_next;
	translationMap["FunDec"] = &SemanticAnalysis::TranslateFunDec;
	translationMap["CreateFunTable_m"] = &SemanticAnalysis::TranslateCreateFunTable_m;
	translationMap["ParamDec"] = &SemanticAnalysis::TranslateParamDec;
	translationMap["Block"] = &SemanticAnalysis::TranslateBlock;
	translationMap["Def"] = &SemanticAnalysis::TranslateDef;
	translationMap["WhileStmt"] = &SemanticAnalysis::TranslateWhileStmt;
	translationMap["WhileStmt_m1"] = &SemanticAnalysis::TranslateWhileStmt_m1;
	translationMap["WhileStmt_m2"] = &SemanticAnalysis::TranslateWhileStmt_m2;
	translationMap["AssignStmt"] = &SemanticAnalysis::TranslateAssignStmt;
	translationMap["Exp"] = &SemanticAnalysis::TranslateExp;
	translationMap["AddSubExp"] = &SemanticAnalysis::TranslateAddSubExp;
	translationMap["Item"] = &SemanticAnalysis::TranslateItem;
	translationMap["Factor"] = &SemanticAnalysis::TranslateFactor;
	translationMap["CallStmt"] = &SemanticAnalysis::TranslateCallStmt;
	translationMap["CallFunCheck"] = &SemanticAnalysis::TranslateCallFunCheck;
	translationMap["Args"] = &SemanticAnalysis::TranslateArgs;
	translationMap["ReturnStmt"] = &SemanticAnalysis::TranslateReturnStmt;
	translationMap["Relop"] = &SemanticAnalysis::TranslateRelop;
	// ����ȫ�ֵķ��ű�
	symbolTables.push_back(SemanticSymbolTable(SemanticSymbolTable::GlobalTable, "global table"));
	// ��ǰ������Ϊȫ��������
	scopeStack.push_back(0);
	// ������ʱ������
	symbolTables.push_back(SemanticSymbolTable(SemanticSymbolTable::TempTable, "temp variable table"));
}

/**
 * @brief�� ��һ��������ӵ������б�
 *          ���б����ڴ洢������������������ķ��š�
 * @param�� const SemanticSymbol symbol Ҫ��ӵķ��š�
 * @return  ��
 */
void SemanticAnalysis::AddSymbolToList(const SemanticSymbol symbol)
{
	symbolList.push_back(symbol);
}

/**
 * @brief�� ����ʶ���Ƿ����ڵ�ǰ���ϲ�������ķ��ű��ж��塣
 *          �ӵ�ǰ������ʼ��������ϲ��ұ�ʶ����
 * @param�� SemanticSymbol identifier Ҫ���ı�ʶ����
 * @param�� int* tb_index ��������������ҵ���ʶ���ķ��ű��ڱ����е�������
 * @param�� int* tb_index_index ��������������ҵ���ʶ���ڷ��ű��е�������
 * @return  bool ��ʾ�Ƿ��ҵ���ʶ��������ҵ�����true�����򷵻�false��
 */
bool SemanticAnalysis::CheckIdDefine(SemanticSymbol identifier, int* tb_index, int* tb_index_index)
{
	// �ӵ�ǰ�����ϱ���
	for (int now_layer = scopeStack.size() - 1; now_layer >= 0; now_layer--)
	{
		SemanticSymbolTable  now_table = symbolTables[scopeStack[now_layer]]; // �����ÿ����Ż�����
		int indexInTable = now_table.FindSymbol(identifier.value);

		if (indexInTable != -1) // �ҵ��˱�ʶ��
		{
			*tb_index = scopeStack[now_layer];
			*tb_index_index = indexInTable;
			return true;
		}
	}

	// ���û���ҵ���ʶ��
	return false;
}

/**
 * @brief�� ��麯�����õĲ��������Ƿ�������һ�¡�
 *          �Ƚϸ�����ʶ���Ĳ����������ṩ��ֵ��
 * @param�� SemanticSymbol check Ҫ���ĺ�����ʶ����
 * @param�� int* value ����������ṩ�Ĳ���������
 * @return  int ����ֵ��ʾ���������ļ������
 *              - 0 ��ʾ��������һ�£�
 *              - 1 ��ʾ�ṩ�Ĳ�����������������
 *              - 2 ��ʾ�ṩ�Ĳ�����������������
 */
int SemanticAnalysis::CheckParNum(SemanticSymbol check, int* value)
{
	// ֱ�����ò�������
	int declaredParamCount = symbolTables[check.tableIndex].symbols[check.symbolIndex].parameterCount;

	if (declaredParamCount > *value) // �ṩ�Ĳ���������������
		return 1;
	else if (declaredParamCount < *value) // �ṩ�Ĳ���������������
		return 2;
	return 0; // ��������һ��
}

/**
 * @brief�� ������������������ĸ��ִ���
 *          ���ݴ������ͣ�ִ�в�ͬ�ļ��ʹ��󱨸档
 * @param�� SemanticSymbol identifier Ҫ���ı�ʶ����
 * @param�� int* tb_index ָ��������ڵķ��ű�������ָ�롣
 * @param�� int* tb_index_index ָ������ڷ��ű��е�������ָ�롣
 * @param�� ErrorProcess type �������͡�
 * @return  ��
 */
int SemanticAnalysis::ProcessError(SemanticSymbol identifier, int* tb_index, int* tb_index_index, ErrorProcess type,int pos)
{
	fstream ferr(SEMANTIC_ERROR_FILE, ios::out);
	if (!ferr.is_open()) {
	      //�ļ��޷���
		return SEMANTIC_ERROR_FILE_NOT_OPEN;
	}
	
	
	if (type == UNDEFINED_IDENTIFIER) {
		if (!CheckIdDefine(identifier, tb_index, tb_index_index))
		{

			ferr << "��������з�������\"" << identifier.value << "\"δ����" << ",λ�ڵ�" << pos << "��" << endl;
			return SEMANTIC_ERROR_UNDEFINED;
		}
	}
	else if(type== INVALID_PARAMETER_COUNT)
		{
			int res = CheckParNum(identifier, tb_index);
			if (res != 0)
			{
				string error_msg = res == 2 ? "����" : "����";
				ferr<< "��������з������󣺺������ò���" << error_msg
					<< "����������Ϊ " << identifier.value <<",λ�ڵ�"<<pos<<"��"<<endl;
				return SEMANTIC_ERROR_PARAMETER_NUM;
			}
		}
	else if(type== FUNCTION_REDEFINED||type== PARAMETER_REDEFINED||type== IDENTIFIER_REDEFINED)
		{
			SemanticSymbolTable table = type == FUNCTION_REDEFINED ? symbolTables[0] : symbolTables[*tb_index];
			if (table.FindSymbol(identifier.value) != -1)
			{
				string error_type = (type == FUNCTION_REDEFINED) ? "����" : (type == PARAMETER_REDEFINED) ? "��������"
					: "����";
				ferr << "��������з�������" << error_type << identifier.value << "�ض���, λ�ڵ�"<<pos<<"��"<<endl;
				return SEMANTIC_ERROR_REDEFINED;
			}
		}
	else if (type == MAIN_FUNCTION_MISSING)

	{
		ferr<< "��������з�������main����δ����" <<"��λ�ڵ�"<<pos<<"��"<<endl;
		return SEMANTIC_ERROR_MAIN_UNDEFINED;
	}
	else if (type == NO_RETURN) {
		ferr << "û�з���ֵ, λ�ڵ�"<<pos<<"��"<<endl;
		return SEMANTIC_ERROR_NO_RETURN;
	}
	else if (type == CALL_FUNCTION_UNDIFINED) {
		ferr << "���õĺ���" << identifier.value << "δ����,λ�ڵ�" << pos << "��" << endl;
		return SEMANTIC_ERROR_CALL_FUNCTION_FAIL;
	}
	else if (type == CALL_NOT_FUNCTION) {
		ferr << identifier.value << "���Ǻ���,�޷�����,λ�ڵ�" << pos << "��" << endl;
		return SEMANTIC_ERROR_CALL_FUNCTION_FAIL;
	}
	return 1;
}

/**
 * @brief�� �ӷ����б��ĩβ�Ƴ�ָ�������ķ��š�
 *          ���ڻ���������������м���ķ��ţ�ͨ���ڴ���ֲ�������ʱʹ�á�
 * @param�� int count Ҫ�Ƴ��ķ���������
 * @return  ��
 */
void SemanticAnalysis::PopSymbolList(int cnt)
{
	while (cnt--)
		symbolList.pop_back();
}

/**
 * @brief�� �����﷨����ʽ����ѡ����Ӧ�ķ��뷽���������������
 *          �ú������ݲ�ͬ�Ĳ���ʽӦ�ò�ͬ�ķ������������Ԫʽ��ִ���������嶯����
 * @param�� const string production_left ����ʽ���󲿣������������ַ��뷽����
 * @param�� const vector<string> production_right ����ʽ���Ҳ����ṩ��������ľ���Ԫ�ء�
 * @return  ��
 */
int SemanticAnalysis::Analysis(const string production_left, const vector<string> production_right,int pos)
{
	auto it = translationMap.find(production_left);
	//cout << 444;
	int res=1;
	if (it != translationMap.end())
	{
		// ������Ӧ�ķ��뺯��
		res=(this->*(it->second))(production_left, production_right,pos);
	}
	else
	{
		if (production_right[0] != "@")
		{
			int count = production_right.size();
			while (count--)
				symbolList.pop_back();
		}
		symbolList.push_back({ production_left, "", -1, -1 });
	}
	return res;
}

/**
 * @brief�� ����Ԫʽ�б������ָ���ļ���
 *          ������Ԫʽ�б���ÿ����Ԫʽ�����ݸ�ʽ��������ļ���
 * @param�� const string file_path ����ļ���·����
 * @return  ��
 */
int SemanticAnalysis::PrintQuaternion(const string file_path)
{
	ofstream ofs(file_path, ios::out);
	if (!ofs.is_open()) {
		return QUATERNION_FILE_NOT_OPEN;
	}

	for (const Quaternion& temp : quaternionList)
	{
		ofs << temp.sequenceNumber << "(";
		ofs << temp.operation << "," << temp.operand1 << "," << temp.operand2 << "," << temp.result << ")" << endl;
	}

	ofs.close();
	return 1;
}

/**
 * @brief�� ����������ڵ㡣
 *          ��� main �����Ƿ��壬�����ʼ��ת��Ԫʽ�������·�������
 * @param�� const string production_left ����ʽ���󲿣�����ǰ�﷨�ṹ��
 * @param�� const vector<string> production_right ����ʽ���Ҳ�������������﷨��Ԫ��
 * @return  ��
 */
int SemanticAnalysis::TranslateProgram(const string production_left, const vector<string> production_right,int pos)
{
	int res = 1;
	if (mainFunctionIndex == -1)
		res=ProcessError({ "", "", -1, -1 }, nullptr, nullptr, MAIN_FUNCTION_MISSING,pos);

	// �����ʼ��ת��Ԫʽ
	quaternionList.insert(quaternionList.begin(), { 0, "j", "-", "-", std::to_string(mainFunctionIndex) });

	// �޸ķ�����
	PopSymbolList(production_right.size());

	symbolList.emplace_back(production_left, "", -1, -1); // ʹ��emplace_back����push_back�����Ч�ʣ�
	return res;
}

/**
 * @brief�� �����ⲿ���壬���������ͺ����Ķ��塣
 *          ���ݲ���ʽ�Ҳ������һ��Ԫ���ж��Ǳ������廹�Ǻ������塣
 *          ���ڱ������壬������ӵ����ű����ں������壬������Ӧ��������仯��
 * @param�� const string production_left ����ʽ���󲿣���ʾ��ǰ������﷨�ṹ��
 * @param�� const vector<string> production_right ����ʽ���Ҳ�������������﷨��Ԫ��
 * @return  ��
 * @term    ExtDef ::= VarSpecifier <ID> ; | FunSpecifier FunDec Block
 */
int SemanticAnalysis::TranslateExtDef(const string production_left, const vector<string> production_right, int pos)
{
	int res=1;
	if (production_right.back() == ";") // ��������
	{
		// ��ȡ������������
		SemanticSymbol varName = symbolList[symbolList.size() - 2];
		SemanticSymbol varType = symbolList[symbolList.size() - 3];

		// ������ܵ��ض������
		res=ProcessError(varName, &(scopeStack.back()), nullptr, IDENTIFIER_REDEFINED,pos);

		// ֮ǰû�ж��壬��ӱ��������ű�
		IdentifierInfo variable{ IdentifierInfo::Variable, varType.value, varName.value };
		symbolTables[scopeStack.back()].InsertSymbol(variable);

		// ���·�����
		PopSymbolList(production_right.size());
		symbolList.emplace_back(production_left, varName.value, -1, -1);
	}
	else // ��������
	{
		SemanticSymbol funcName = symbolList[symbolList.size() - 2];

		// ������ǰ����������
		scopeStack.pop_back();

		// ���·�����
		PopSymbolList(production_right.size());
		symbolList.emplace_back(production_left, funcName.value, -1, -1);
	}
	return res;
}
/* 	ע�⣡����
	��������������������һ���ģ����Կ��Ǻϲ���һ�������ģ�һ��������
*/
/**
 * @brief�� �����������������ָ�����֡�
 *          �ӷ���������ȡ������Ϣ�����滻�Ҳ�Ĳ���ʽΪ�����﷨�ṹ��
 * @param�� const string production_left ����ʽ���󲿣���ʾ��ǰ������﷨�ṹ��
 * @param�� const vector<string> production_right ����ʽ���Ҳ�������������﷨��Ԫ��
 * @return  ��
 * @term    VarSpecifier ::= int
 */
int SemanticAnalysis::TranslateVarSpecifier(const string production_left, const vector<string> production_right, int pos)
{
	// ֱ���������һ�����ţ����⸴��
	SemanticSymbol lastC = symbolList.back();

	// ���·�����
	PopSymbolList(production_right.size());
	symbolList.emplace_back(production_left, lastC.value, -1, -1);
	return 1;
}

/**
 * @brief�� ���뺯������������ָ�����֡�
 *          �ӷ���������ȡ����������Ϣ�����滻�Ҳ�Ĳ���ʽΪ�����﷨�ṹ��
 * @param�� const string production_left ����ʽ���󲿣���ʾ��ǰ������﷨�ṹ��
 * @param�� const vector<string> production_right ����ʽ���Ҳ�������������﷨��Ԫ��
 * @return  ��
 * @term    FunSpecifier ::= void | int
 */
int SemanticAnalysis::TranslateFunSpecifier(const string production_left, const vector<string> production_right, int pos)
{
	// ֱ���������һ�����ţ����⸴��
	SemanticSymbol lastC = symbolList.back();

	// ���·�����
	PopSymbolList(production_right.size());
	symbolList.emplace_back(production_left, lastC.value, -1, -1);
	return 1;
}

/**
 * @brief�� �������������͵����巭�롣
 *          ��ȡsymbolList�����һ��Ԫ����Ϊ�����ķ������ͣ������·�������
 *          ���Ҳ����ʽ�ķ����滻Ϊ������ʽ�ķ��š�
 * @param�� const string production_left ����ʽ���󲿣���ʾ��ǰ������﷨�ṹ��
 * @param�� const vector<string> production_right ����ʽ���Ҳ�������������﷨��Ԫ��
 * @return  ��
 */
int SemanticAnalysis::TranslateFunDec(const string production_left, const vector<string> production_right, int pos)
{
	// ֱ���������һ�����ţ����⸴��
	SemanticSymbol lastType = symbolList[symbolList.size() - 1];

	// ���·�����
	PopSymbolList(production_right.size());
	symbolList.emplace_back(production_left, lastType.value, -1, -1);
	return 1;
}

/**
 * @brief�� ����������д������������ķ��ű�
 *          �ӷ���������ȡ�������Ͳ�����Ϣ����麯���Ƿ��ض��壬
 *          �������µĺ������ű����Ӧ�ĺ��������Ԫʽ��
 * @param�� const string production_left ����ʽ���󲿣�����ǰ������﷨�ṹ��
 * @param�� const vector<string> production_right ����ʽ���Ҳ�������������﷨��Ԫ��
 * @return  ��
 */
int SemanticAnalysis::TranslateCreateFunTable_m(const string production_left, const vector<string> production_right, int pos)
{
	int res = 1;
	// ����������funcName��������funcPara��������
	SemanticSymbol funcName = symbolList[symbolList.size() - 1];
	SemanticSymbol funcPara = symbolList[symbolList.size() - 2];

	// �жϺ������Ƿ��ض���
	res=ProcessError(funcName, nullptr, nullptr, FUNCTION_REDEFINED,pos);

	// ��ȫ�ַ��ű�����ǰ�����ķ�������������������ڵ�ַ����л��
	symbolTables.emplace_back(SemanticSymbolTable::FunctionTable, funcName.value);
	symbolTables[0].InsertSymbol({ IdentifierInfo::Function, funcPara.value, funcName.value, 0, 0, int(symbolTables.size() - 1) });

	// ������ѹ��ջ
	scopeStack.push_back(symbolTables.size() - 1);

	// ����ֵ
	IdentifierInfo return_value({ IdentifierInfo::ReturnValue, funcName.value + "_return_value", funcPara.value });
	if (funcName.value == "main")
		mainFunctionIndex = nextQuaternionIndex;
	quaternionList.emplace_back(nextQuaternionIndex++, funcName.value, "-", "-", "-");
	symbolTables[scopeStack.back()].InsertSymbol(return_value);
	symbolList.emplace_back(production_left, funcName.value, 0, int(symbolTables[0].symbols.size() - 1));
	return res;
}

/**
 * @brief�� ���뺯���Ĳ���������
 *          �ӷ���������ȡ�������ƺ����ͣ��������Ƿ��ض��壬
 *          ����������Ϣ��ӵ���ǰ�����ķ��ű��С�
 * @param�� const string production_left ����ʽ���󲿣���ʾ��ǰ������﷨�ṹ��
 * @param�� const vector<string> production_right ����ʽ���Ҳ�������������﷨��Ԫ��
 * @return  ��
 */
int SemanticAnalysis::TranslateParamDec(const string production_left, const vector<string> production_right, int pos)
{
	int res = 1;
	SemanticSymbol name = symbolList.back();
	SemanticSymbol type = symbolList[symbolList.size() - 2];

	SemanticSymbolTable &function_table = symbolTables[scopeStack.back()];

	res=ProcessError(name, &(scopeStack.back()), nullptr, PARAMETER_REDEFINED,pos);

	int new_position = function_table.InsertSymbol({ IdentifierInfo::Variable, type.value, name.value, -1, -1, -1 });
	int table_position = symbolTables[0].FindSymbol(function_table.tableName);
	symbolTables[0].symbols[table_position].parameterCount++;

	quaternionList.emplace_back(nextQuaternionIndex++, "defpar", "-", "-", name.value);

	PopSymbolList(production_right.size());
	symbolList.emplace_back(production_left, name.value, scopeStack.back(), new_position);
	return res;
}

/**
 * @brief�� �������顣
 *          ���·��������������Ĵ���������������С�
 * @param�� const string production_left ����ʽ���󲿣���ʾ��ǰ������﷨�ṹ��
 * @param�� const vector<string> production_right ����ʽ���Ҳ�������������﷨��Ԫ��
 * @return  ��
 */
int SemanticAnalysis::TranslateBlock(const string production_left, const vector<string> production_right, int pos)
{
	// ֱ�����÷����������һ��Ԫ��
	SemanticSymbol type = symbolList.back();

	// ���·�����
	PopSymbolList(production_right.size());
	symbolList.emplace_back(production_left, std::to_string(nextQuaternionIndex), -1, -1);
	return 1;
}

/**
 * @brief�� ����������塣
 *          �ӷ���������ȡ�����������ͣ��������Ƿ��ض��壬����������Ϣ��ӵ���ǰ������ķ��ű��С�
 * @param�� const string production_left ����ʽ���󲿣���ʾ��ǰ������﷨�ṹ��
 * @param�� const vector<string> production_right ����ʽ���Ҳ�������������﷨��Ԫ��
 * @return  ��
 */
int SemanticAnalysis::TranslateDef(const string production_left, const vector<string> production_right, int pos)
{
	int res = 1;
	SemanticSymbol name = symbolList[symbolList.size() - 2];
	SemanticSymbol type = symbolList[symbolList.size() - 3];
	SemanticSymbolTable& current_table = symbolTables[scopeStack.back()];

	res=ProcessError(name, &(scopeStack.back()), nullptr, IDENTIFIER_REDEFINED,pos);

	current_table.InsertSymbol({ IdentifierInfo::Variable, type.value, name.value, -1, -1, -1 });

	// ���·�����
	PopSymbolList(production_right.size());
	symbolList.emplace_back(production_left, name.value, scopeStack.back(), int(current_table.symbols.size() - 1));
	return res;
}

/**
 * @brief�� ���븳ֵ��䡣
 *          �ӷ���������ȡ��ֵ������ɲ��֣�����ʶ���Ƿ��Ѷ��壬��������Ӧ����Ԫʽ��
 * @param�� const string production_left ����ʽ���󲿣���ʾ��ǰ������﷨�ṹ��
 * @param�� const vector<string> production_right ����ʽ���Ҳ�������������﷨��Ԫ��
 * @return  ��
 * @ term   AssignStmt ::= <ID> = Exp
 */
int SemanticAnalysis::TranslateAssignStmt(const string production_left, const vector<string> production_right, int pos)
{
	int res = 1;
	SemanticSymbol exp = symbolList.back();
	SemanticSymbol idtf = symbolList[symbolList.size() - 3];

	int tbIndex = -1, tbIndexIndex = -1;
	res=ProcessError(idtf, &tbIndex, &tbIndexIndex, UNDEFINED_IDENTIFIER,pos);

	quaternionList.emplace_back(nextQuaternionIndex++, "=", exp.value, "-", idtf.value);

	// ���·�����
	PopSymbolList(production_right.size());
	symbolList.emplace_back(production_left, idtf.value, tbIndex, tbIndexIndex);
	return res;
}

/**
 * @brief�� ������ʽ��
 *          ���ݱ��ʽ���Ҳ��ֳ�����ȷ���Ǽ򵥱��ʽ���ǰ�����ϵ������ĸ��ϱ��ʽ��
 *          ���ڼ򵥱��ʽ��ֱ�Ӵӷ�������ȡ�������ڸ��ϱ��ʽ��������Ӧ����Ԫʽ��
 * @param�� const string production_left ����ʽ���󲿣���ʾ��ǰ������﷨�ṹ��
 * @param�� const vector<string> production_right ����ʽ���Ҳ�������������﷨��Ԫ��
 * @return  ��
 * @term    Exp ::= AddSubExp | Exp Relop AddSubExp
 */
int SemanticAnalysis::TranslateExp(const string production_left, const vector<string> production_right, int pos)
{
	if (production_right.size() == 1)
	{
		SemanticSymbol add_sub_exp = symbolList.back();
		PopSymbolList(1);
		symbolList.emplace_back(production_left, add_sub_exp.value, add_sub_exp.tableIndex, add_sub_exp.symbolIndex);
	}
	else
	{
		SemanticSymbol exp1 = symbolList[symbolList.size() - 3];
		SemanticSymbol op = symbolList[symbolList.size() - 2];
		SemanticSymbol  exp2 = symbolList.back();
		string var = "T" + to_string(tempVarCounter++);
		int lable_num_next = nextQuaternionIndex++;

		quaternionList.emplace_back(lable_num_next, "j" + op.value, exp1.value, exp2.value, to_string(lable_num_next + 3));
		quaternionList.emplace_back(nextQuaternionIndex++, "=", "0", "-", var);
		quaternionList.emplace_back(nextQuaternionIndex++, "j", "-", "-", to_string(lable_num_next + 4));
		quaternionList.emplace_back(nextQuaternionIndex++, "=", "1", "-", var);
		PopSymbolList(production_right.size());
		symbolList.emplace_back(production_left, var, -1, -1);
	}
	return 1;
}

/**
 * @brief�� ����Ӽ����ʽ��
 *          ���ݲ���ʽ�Ҳ��ĳ�����ȷ���ǵ�����ǰ����ӷ������������ı��ʽ��
 *          ���ڵ����ֱ�Ӵӷ�������ȡ�������ڰ���������ı��ʽ��������Ӧ����Ԫʽ��
 * @param�� const string production_left ����ʽ���󲿣���ʾ��ǰ������﷨�ṹ��
 * @param�� const vector<string> production_right ����ʽ���Ҳ�������������﷨��Ԫ��
 * @return  ��
 * @term    AddSubExp ::= Item | Item + Item | Item - Item
 */
int SemanticAnalysis::TranslateAddSubExp(const string production_left, const vector<string> production_right, int pos)
{
	if (production_right.size() == 1)
	{
		SemanticSymbol add_sub_exp = symbolList.back();
		PopSymbolList(1);
		symbolList.emplace_back(production_left, add_sub_exp.value, add_sub_exp.tableIndex, add_sub_exp.symbolIndex);
	}
	else
	{
		SemanticSymbol exp1 = symbolList[symbolList.size() - 3];
		SemanticSymbol  op = symbolList[symbolList.size() - 2];
		SemanticSymbol exp2 = symbolList.back();
		string var = "T" + to_string(tempVarCounter++);

		quaternionList.emplace_back(nextQuaternionIndex++, op.value, exp1.value, exp2.value, var);
		PopSymbolList(production_right.size());
		symbolList.emplace_back(production_left, var, -1, -1);
	}
	return 1;
}

/**
 * @brief�� ����˳����ʽ�
 *          ���ݲ���ʽ�Ҳ��ĳ���ȷ���ǵ������ӻ��ǰ����˷������������ı��ʽ��
 *          ���ڵ������ӣ�ֱ�Ӵӷ�������ȡ�������ڰ���������ı��ʽ��������Ӧ����Ԫʽ��
 * @param�� const string production_left ����ʽ���󲿣���ʾ��ǰ������﷨�ṹ��
 * @param�� const vector<string> production_right ����ʽ���Ҳ�������������﷨��Ԫ��
 * @return  ��
 * @term    Item ::= Factor | Factor * Factor | Factor / Factor
 */
int SemanticAnalysis::TranslateItem(const string production_left, const vector<string> production_right, int pos)
{
	if (production_right.size() == 1)
	{
		SemanticSymbol item_exp = symbolList.back();
		PopSymbolList(1);
		symbolList.emplace_back(production_left, item_exp.value, item_exp.tableIndex, item_exp.symbolIndex);
	}
	else {

		SemanticSymbol exp1 = symbolList[symbolList.size() - 3];
		SemanticSymbol op = symbolList[symbolList.size() - 2];
		SemanticSymbol exp2 = symbolList.back();
		string var = "T" + to_string(tempVarCounter++);

		quaternionList.emplace_back(nextQuaternionIndex++, op.value, exp1.value, exp2.value, var);
		PopSymbolList(production_right.size());
		symbolList.emplace_back(production_left, var, -1, -1);
	}
	return 1;
}

/**
 * @brief�� �������ӡ�
 *          ���ݲ���ʽ�Ҳ�������������ͬ���͵����ӣ����������������ڱ��ʽ����ʶ���������á�
 *          ���ڱ�ʶ����������Ƿ��Ѷ��塣
 * @param�� const string production_left ����ʽ���󲿣���ʾ��ǰ������﷨�ṹ��
 * @param�� const vector<string> production_right ����ʽ���Ҳ�������������﷨��Ԫ��
 * @return  ��
 * @term    Factor ::= <INT> | ( Exp ) | <ID> | CallStmt
 */
int SemanticAnalysis::TranslateFactor(const string production_left, const vector<string> production_right, int pos)
{
	int res = 1;
	if (production_right.size() == 1)
	{
		SemanticSymbol exp = symbolList.back();
		if ("<ID>" == production_right[0])
		{
			int tb_index = -1, tb_index_index = -1;
			res=ProcessError(exp, &tb_index, &tb_index_index, UNDEFINED_IDENTIFIER,pos);
		}
		PopSymbolList(production_right.size());
		symbolList.emplace_back(production_left, exp.value, exp.tableIndex, exp.symbolIndex);
	}
	else
	{
		SemanticSymbol  exp = symbolList[symbolList.size() - 2];
		PopSymbolList(production_right.size());
		symbolList.emplace_back(production_left, exp.value, exp.tableIndex, exp.symbolIndex);
	}
	return res;
}

/**
 * @brief�� ���뺯��������䡣
 *          ��鱻���õĺ����Ƿ��Ѷ����Լ����������Ƿ���ȷ��
 *          ���ɶ�Ӧ����Ԫʽ�����������ã������·�������
 * @param�� const string production_left ����ʽ���󲿣���ʾ��ǰ������﷨�ṹ��
 * @param�� const vector<string> production_right ����ʽ���Ҳ�������������﷨��Ԫ��
 * @return  ��
 * @term	CallStmt ::= <ID> ( CallFunCheck Args )
 */
int SemanticAnalysis::TranslateCallStmt(const string production_left, const vector<string> production_right, int pos)
{
	int res = 1;
	SemanticSymbol functionId = symbolList[symbolList.size() - 5];
	SemanticSymbol check = symbolList[symbolList.size() - 3];
	SemanticSymbol args = symbolList[symbolList.size() - 2];

	int argCount = stoi(args.value);
	int dummy = -1;
	res=ProcessError(check, &argCount, &dummy, INVALID_PARAMETER_COUNT,pos);

	string tempVar = "T" + to_string(tempVarCounter++);
	quaternionList.emplace_back(nextQuaternionIndex++, "call", functionId.value, "-", tempVar);

	PopSymbolList(production_right.size());
	symbolList.emplace_back(production_left, tempVar, -1, -1);
	return res;
}

/**
 * @brief�� ��麯�����õ���ȷ�ԡ�
 *          ȷ�Ϻ�����ʶ���Ƿ��Ѷ�����������ȷ���Ƿ�Ϊ�������ͣ���
 *          ������ʧ�ܣ��׳���Ӧ���������
 * @param�� const string production_left ����ʽ���󲿣���ʾ��ǰ������﷨�ṹ��
 * @param�� const vector<string> production_right ����ʽ���Ҳ���ͨ����һ���հ׵�ȷ�ϱ�ǡ�
 * @return  ��
 */
int SemanticAnalysis::TranslateCallFunCheck(const string production_left, const vector<string> production_right, int pos)
{
	int res = 1;
	SemanticSymbol  functionId = symbolList[symbolList.size() - 2];
	int functionPosition = symbolTables[0].FindSymbol(functionId.value);

	if (functionPosition == -1)
	{
		//cout << "δ����ĺ�����" << endl;
		//throw(SEMANTIC_ERROR_UNDEFINED);
		int invalid = 0;
		res=ProcessError(functionId, &invalid, &invalid, CALL_FUNCTION_UNDIFINED, pos);
	}
	else if (symbolTables[0].symbols[functionPosition].category != IdentifierInfo::Function)
	{
		//cout << "ID���壬�����Ͳ��Ǻ���" << endl;
		//throw(SEMANTIC_ERROR_UNDEFINED);
		int invalid = 0;
		res = ProcessError(functionId, &invalid, &invalid, CALL_NOT_FUNCTION, pos);

	}

	symbolList.emplace_back(production_left, functionId.value, 0, functionPosition);
	return res;
}

/**
 * @brief�� ���뺯�����õĲ����б�
 *          ��������б���ÿ�����ʽ�����������������
 *          ����ÿ��������������Ӧ����Ԫʽ�������·������еĲ���������
 *          ���ݲ����б�ĳ��������ֲ�ͬ�Ĵ���ʽ����������������������޲�����
 * @param�� const string production_left ����ʽ���󲿣���ʾ��ǰ������﷨�ṹ��
 * @param�� const vector<string> production_right ����ʽ���Ҳ�������������﷨��Ԫ��
 * @return  ��
 * @term    Args ::= Exp , Args | Exp | @
 */
int SemanticAnalysis::TranslateArgs(const string production_left, const vector<string> production_right, int pos)
{
	int symbol_list_size = symbolList.size();
	int right_size = production_right.size();
	if (right_size == 3)
	{
		int args_num = stoi(symbolList[symbol_list_size - 1].value) + 1;
		SemanticSymbol test_exp = symbolList[symbol_list_size - 3];
		PopSymbolList(right_size);
		quaternionList.push_back({ nextQuaternionIndex++, "param", test_exp.value, "-", "-" });
		symbolList.push_back({ production_left, to_string(args_num), -1, -1 });
	}
	else
	{
		if (production_right[0] == "Exp")
		{
			SemanticSymbol exp = symbolList[symbol_list_size - 1];
			PopSymbolList(right_size);
			quaternionList.push_back({ nextQuaternionIndex++, "param", exp.value, "-", "-" });
			symbolList.push_back({ production_left, "1", -1, -1 });
		}
		if (production_right[0] == "@")
		{
			symbolList.push_back({ production_left, "0", -1, -1 });
		}
	}
	return 1;
}


/**
 * @brief�� ���뷵����䡣
 *          ���ݷ����������ͣ����޷���ֵ�����д���
 *          �����������Ϊ��void������ȱ�ٷ���ֵ�����׳�����
 *          ������Ӧ����Ԫʽ��������ֵ��ֱ�ӷ��ء�
 * @param�� const string production_left ����ʽ���󲿣���ʾ��ǰ������﷨�ṹ��
 * @param�� const vector<string> production_right ����ʽ���Ҳ�������������﷨��Ԫ��
 * @return  ��
 * @term    ReturnStmt ::= return Exp | return
 */
int SemanticAnalysis::TranslateReturnStmt(const string production_left, const vector<string> production_right, int pos)
{
	int res = 1;
	int right_size = production_right.size();
	int symbol_list_size = symbolList.size();
	SemanticSymbolTable symbol_table = symbolTables[scopeStack.back()];

	if (right_size == 1)
	{
		auto type = symbolTables[0].symbols[symbolTables[0].FindSymbol(symbol_table.tableName)].specifierType;
		if (type != "void")
		{
			//cout << "ȱ�ٷ���ֵ" << endl;
			int invalid = 0;
			res = ProcessError(symbolList[0],&invalid , &invalid, NO_RETURN, pos);
		}
		PopSymbolList(right_size);
		quaternionList.push_back({ nextQuaternionIndex++, "return", "-", "-", symbol_table.tableName });
		symbolList.push_back({ production_left, "", - 1, -1 });
	}
	else
	{
		SemanticSymbol exp = symbolList[symbol_list_size - 1];

		quaternionList.push_back({ nextQuaternionIndex++, "=", exp.value, "-", symbol_table.symbols[0].identifierName });
		quaternionList.push_back({ nextQuaternionIndex++, "return", symbol_table.symbols[0].identifierName, "-", symbol_table.tableName });

		PopSymbolList(right_size);
		symbolList.push_back({ production_left, exp.value, -1, -1 });
	}
	return res;
}


/**
 * @brief�� �����ϵ�������
 *          ����ϵ������ӷ���������ȡ�����������·�������
 *          ���漰�����飬��Ϊ��ϵ������ĺϷ������﷨������֤��
 * @param�� const string production_left ����ʽ���󲿣���ʾ��ǰ������﷨�ṹ��
 * @param�� const vector<string> production_right ����ʽ���Ҳ���������ϵ�������
 * @return  ��
 * @term    Relop ::= > | < | >= | <= | == | !=
 */
int  SemanticAnalysis::TranslateRelop(const string production_left, const vector<string> production_right, int pos)
{
	int right_size = production_right.size();
	int symbol_list_size = symbolList.size();
	SemanticSymbol exp = symbolList[symbol_list_size - 1];
	PopSymbolList(right_size);
	symbolList.push_back({ production_left, exp.value, -1, -1 });
	return 1;
}

/**
 * @brief�� ����if��䡣
 *          ����if�������嶯��������������ת��ַ��
 *          ����if����Ƿ���else���֣����ͬ��������Ԫʽ��
 * @param�� const string production_left ����ʽ���󲿣���ʾ��ǰ������﷨�ṹ��
 * @param�� const vector<string> production_right ����ʽ���Ҳ�������if���ĸ�����ɲ��֡�
 * @return  ��
 * @term    IfStmt ::= if IfStmt_m1 ( Exp ) IfStmt_m2 Block IfNext
 */
int SemanticAnalysis::TranslateIfStmt(const string production_left, const vector<string> production_right, int pos)
{
	SemanticSymbol IfStmt_m2 = symbolList[symbolList.size() - 3];
	SemanticSymbol IfNext = symbolList[symbolList.size() - 1];

	// ���û��else��ֻ��Ҫ����IfStmt_m2��һ����Ԫʽ
	if (IfNext.value.empty())
	{
		// ����ٳ��ڣ�����if�������next_quaternary_index���������һ��
		quaternionList[backpatchingList.back()].result = to_string(nextQuaternionIndex);
		backpatchingList.pop_back();
	}
	else
		// ��else����Ҫ����������Ԫʽ
	{
		// ����if����ڣ������������ڣ���ӦBlockִ�����������jump����תĿ��
		quaternionList[backpatchingList.back()].result = to_string(nextQuaternionIndex);
		backpatchingList.pop_back();

		// ����if�ٳ���
		quaternionList[backpatchingList.back()].result = IfNext.value;
		backpatchingList.pop_back();
	}

	// �޸�symbol list
	PopSymbolList(production_right.size());
	symbolList.push_back({ production_left, "", -1, -1 });
	return 1;
}

/**
 * @brief�� ����if���ı�ǲ��֣�IfStmt_m1����
 *          �ò�����Ҫ�����﷨�ṹ��ռλ�������漰��������嶯����
 *          �����Ҳ��ǿգ���������ӷ�������popԪ�ء�
 *          ��¼��ǰ��һ����Ԫʽ��λ�ã���ʵ���ϴ�λ����Ϣ�ڴ��������в���ʹ�á�
 * @param�� const string production_left ����ʽ���󲿣���ʾ��ǰ������﷨�ṹ��
 * @param�� const vector<string> production_right ����ʽ���Ҳ����˴�Ϊ�ա�
 * @return  ��
 * @term    IfStmt_m1 ::= @
 */
int SemanticAnalysis::TranslateIfStmt_m1(const string production_left, const vector<string> production_right, int pos)
{
	//��Ϊ�Ҳ��ǿգ����Բ���pop
	symbolList.push_back({ production_left, to_string(nextQuaternionIndex), -1, -1 });
	return 1;
}

/**
 * @brief�� ����if���ĵڶ�����ǲ��֣�IfStmt_m2����
 *          ���ڴ���if��������в������ʽ����ٳ����߼���
 *          ����������Ԫʽ��һ�����ڲ������ʽ�ٳ��ڵĴ�������Ԫʽ����һ��������ڵ�ֱ����ת��
 * @param�� const string production_left ����ʽ���󲿣���ʾ��ǰ������﷨�ṹ��
 * @param�� const vector<string> production_right ����ʽ���Ҳ���ͨ��Ϊ�ա�
 * @return  ��
 * @term    IfStmt_m2 ::= @
 */
int SemanticAnalysis::TranslateIfStmt_m2(const string production_left, const vector<string> production_right, int pos)
{
	SemanticSymbol expression = symbolList[symbolList.size() - 2];

	quaternionList.push_back({ nextQuaternionIndex++, "j=", expression.value, "0", "" });
	backpatchingList.push_back(quaternionList.size() - 1);

	
	quaternionList.push_back({ nextQuaternionIndex++, "j=", "-", "-", to_string(nextQuaternionIndex) });

	// �޸�symbol list
	symbolList.push_back({ production_left, to_string(nextQuaternionIndex), -1, -1 });
	return 1;
}

/**
 * @brief�� ����if����else���֡�
 *          �����Ƿ����else���֣��ɲ���ʽ�Ҳ������жϣ���������Ӧ�ķ���������
 *          ������else���ֵ���������·������Է�ӳ��ת�߼���������else���ֵ����������Ϊռλ����
 * @param�� const string production_left ����ʽ���󲿣���ʾ��ǰ������﷨�ṹ��
 * @param�� const vector<string> production_right ����ʽ���Ҳ������ܰ���else���ֵ������Ϣ��
 * @return  ��
 * @term    IfNext ::= @ | IfStmt_next else Block
 */
int SemanticAnalysis::TranslateIfNext(const string production_left, const vector<string> production_right, int pos)
{
	if (production_right.size() == 3)
	{
		SemanticSymbol IfStmt_next = symbolList[symbolList.size() - 3];
		// �޸�symbol list
		PopSymbolList(production_right.size());
		symbolList.push_back({ production_left, IfStmt_next.value, -1, -1 });
	}
	else // �ǿյ����
	{
		symbolList.push_back({ production_left, "", -1, -1 });
	}
	return 1;
}

/**
*@brief�� ����if����һ��ִ����֮�����һ��������
* ���һ����������ת����Ԫʽ������תλ���ں��������л��
* ���ڴ���if����еķ�֧��ת�߼���
* @param�� const string production_left ����ʽ���󲿣���ʾ��ǰ������﷨�ṹ��
* @param�� const vector<string> production_right ����ʽ���Ҳ����˴�ͨ��Ϊ�ա�
* @return  ��
* @term    IfStmt_next :: = @
*/
int SemanticAnalysis::TranslateIfStmt_next(const string production_left, const vector<string> production_right, int pos)
{
	// ��������ת���
	quaternionList.push_back({ nextQuaternionIndex++, "j", "-", "-", "" });
	backpatchingList.push_back(quaternionList.size() - 1);

	// �޸�symbol list,�浱ǰ�������һ����Ԫʽλ��
	symbolList.push_back({ production_left, to_string(nextQuaternionIndex), -1, -1 });
	return 1;
}

/**
*@brief�� ����while��䡣
* ��whileѭ������ʱ�����������ת�����ص�ѭ���������жϲ��֡�
* ����ѭ���������ںͼٳ��ڣ���ȷ��ѭ������ȷִ�С�
* @param�� const string production_left ����ʽ���󲿣���ʾ��ǰ������﷨�ṹ��
* @param�� const vector<string> production_right ����ʽ���Ҳ�������while���ĸ�����ɲ��֡�
* @return  ��
* @term    WhileStmt :: = while WhileStmt_m1(Exp) WhileStmt_m2 Block
*/
int SemanticAnalysis::TranslateWhileStmt(const string production_left, const vector<string> production_right, int pos)
{
	SemanticSymbol whilestmt_m1 = symbolList[symbolList.size() - 6];
	SemanticSymbol whilestmt_m2 = symbolList[symbolList.size() - 2];

	// ��������ת�� while �������ж���䴦
	quaternionList.push_back({ nextQuaternionIndex++, "j", "-", "-", whilestmt_m1.value });

	// ���������
	quaternionList[backpatchingList.back()].result = whilestmt_m2.value;
	backpatchingList.pop_back();

	// ����ٳ���
	quaternionList[backpatchingList.back()].result = to_string(nextQuaternionIndex);
	backpatchingList.pop_back();

	backpatchingLevel--;

	PopSymbolList(production_right.size());

	symbolList.push_back({ production_left, "", -1, -1 });
	return 1;
}

/**
 * @brief�� ����while���ĵ�һ����ǲ��֣�WhileStmt_m1����
 *          ��Ҫ�����ڽ���whileѭ��ʱ����¼��ǰ��Ԫʽ���������Ա���ѭ�����������ص�ѭ����ʼ��
 *          ͬʱ�����ӻ���㼶�Դ���ѭ���ڲ�����ת�߼���
 * @param�� const string production_left ����ʽ���󲿣���ʾ��ǰ������﷨�ṹ��
 * @param�� const vector<string> production_right ����ʽ���Ҳ����˴�ͨ��Ϊ�ա�
 * @return  ��
 * @term    WhileStmt_m1 ::= @
 */
int SemanticAnalysis::TranslateWhileStmt_m1(const string production_left, const vector<string> production_right, int pos)
{
	backpatchingLevel++;
	symbolList.push_back({ production_left, to_string(nextQuaternionIndex), -1, -1 });
	return 1;
}

/**
 * @brief�� ����while���ĵڶ�����ǲ��֣�WhileStmt_m2����
 *          ���ڴ���whileѭ���������ж��߼���
 *          ����������Ԫʽ��һ����������Ϊ��ʱ����ת���ٳ��ڣ�����һ����ѭ������������������ת��
 *          ��Щ��Ԫʽ��Ŀ���ַ����ѭ����������л��
 * @param�� const string production_left ����ʽ���󲿣���ʾ��ǰ������﷨�ṹ��
 * @param�� const vector<string> production_right ����ʽ���Ҳ���ͨ��Ϊ�ա�
 * @return  ��
 * @term    WhileStmt_m2 ::= @
 */
int SemanticAnalysis::TranslateWhileStmt_m2(const string production_left, const vector<string> production_right, int pos)
{
	// �ٳ���
	SemanticSymbol while_exp = symbolList[symbolList.size() - 2];

	// �ٳ���
	quaternionList.push_back({ nextQuaternionIndex++, "j=", while_exp.value, "0", "" });
	backpatchingList.push_back(quaternionList.size() - 1);
	// �����
	quaternionList.push_back({ nextQuaternionIndex++, "j", "-", "-", "" });
	backpatchingList.push_back(quaternionList.size() - 1);

	symbolList.push_back({ production_left, to_string(nextQuaternionIndex), -1, -1 });
	return 1;
}