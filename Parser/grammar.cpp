#include "grammar.h"
#include <fstream>



/**
 * @brief�� ���캯��
 * @param�� 
 * @return 
 * */
grammar::grammar()
{
}

grammar::~grammar() {

}


/**
 * @brief�� �ķ���ʼ����ת��Ϊ��չ�ķ�
 * @param�� ofstream  ����ķ�������Ϣ
 * @return  ������ķ��ļ��Ƿ���ȷ
 * */
bool  grammar::checkGrammar(ofstream &ferr)
{
	if (symbolIndex(EndToken) == -1)
	{
		symbols.push_back(symbol(symbolType::end, EndToken));
		terminals.insert(symbols.size() - 1);
	}
	if (symbolIndex(StartToken) == -1)
	{
		ferr << "���ķ�û�п�ʼ����!\n";
		return false;
	}
	if (terminals.size()==0)
	{
		ferr << "���ķ�û��û���ս����\n";
		return false;
	}
	//������չ��ʼ��
	if (symbolIndex(ExtendStartToken) == -1)
	{
		int left_symbol;
		vector<int>right_symbol_index;
		symbols.push_back(symbol(symbolType::nonterminal, ExtendStartToken));
		left_symbol = symbols.size() - 1;
		int right = symbolIndex(StartToken);
		right_symbol_index.push_back(right);
		this->productions.push_back(production(left_symbol, right_symbol_index));
	}
	return true;
}


/**
 * @brief�� ��ȡ�ķ��ļ�������first��
 * @param�� const string file_path ������ķ��ļ�
 * @return  �����ķ��ļ��Ĵ�������
 * */
int grammar::ReadGrammar(const string file_path) {

	ifstream fgrammar;//������ķ��ļ�
	fgrammar.open(file_path, ios::in);
	if (!fgrammar.is_open())
	{
		return GRAMMAR_FILE_OPEN_ERROR;
	}

	ofstream ferr;
	ferr.open(GRAMMAR_ERROR_FILE, ios::out);
	{
		if (!ferr.is_open())
		{
			return GRAMMAR_ERROR_FILE_OPEN_ERROR;
		}
	}

	symbols.push_back(symbol(symbolType::epsilon, EpsilonToken));

	int production_index = 0;
	int row = 0;
	string production_str;

	while (getline(fgrammar, production_str, '\n'))
	{
		row++;
		if (production_str.empty())
			continue;
		Trimmed(production_str);//ע�ʹ���
		if (production_str.find("$") != production_str.npos)
			production_str.erase(production_str.find_first_of("$"));//��$��һ�γ��ֵ�λ�� ɾ����β
		if (production_str.empty())
			continue;

		//�������ʽ
		vector<string> process = split(production_str, ProToken);
		if (process.size() != 2) {
			ferr<<"��"<<row<<"�еĲ���ʽ��ʽ����ÿ��Ӧ����ֻ��һ��\""<<ProToken<<"\"���ţ�\n";
			ferr<<process[0]<<process[1]<<"\n";
			ferr<<process.size()<<production_str;
			return GRAMMAR_ERROR;
		}
		string left = process[0];
		string right = process[1];
	
		Trimmed(left);
		int left_symbol = -1;
		if (left != AllTerminalToken)
		{
			left_symbol = symbolIndex(left);
			if (left_symbol == -1)//û�ҵ�
			{
				Trimmed(left);
				symbols.push_back(symbol(symbolType::nonterminal, left));
				left_symbol = symbols.size() - 1;
				non_terminals.insert(left_symbol);
			}
		}
		//�󲿷��ս��������� 
		//����Ҳ��ķָ�
		vector<string>right_part = split(right, SplitToken);
		if (right_part.size() == 0)
		{
			ferr<<"��"<<row<<"�еĲ���ʽ��ʽ����û���ķ��ţ�\n";
			return GRAMMAR_ERROR;
		}

		for (auto it = right_part.begin(); it != right_part.end(); it++)
		{
			if (left_symbol == -1)
			{
				Trimmed(*it);
				symbols.push_back(symbol(symbolType::terminal, *it));
				terminals.insert(symbols.size() - 1);
			}
			else {
				vector<int>right_symbol_index;
				//�ָ��Ҳ�
				vector<string>right_symbol_name = split(*it, " ");
				for (auto right_it = right_symbol_name.begin(); right_it != right_symbol_name.end(); right_it++)
				{
					int right_index_now = symbolIndex(*right_it);
					if (right_index_now == -1) {
						Trimmed(*right_it);
						if ((*right_it).length() == 0)
							continue;
						symbols.push_back(symbol(symbolType::nonterminal, *right_it));
						right_index_now = symbols.size() - 1;
						non_terminals.insert(right_index_now);
					}

					right_symbol_index.push_back(right_index_now);
				}
				this->productions.push_back(production(left_symbol, right_symbol_index));
				//��չ�ķ�����ʽ��production�е�λ��
				if (symbols[left_symbol].name == ExtendStartToken)
				{
					start_index = productions.size() - 1;
				}

			}
		}
	}

    //������չ�ķ�
	if (!checkGrammar(ferr)) {
		return GRAMMAR_ERROR;
	}
	//����չ�ķ�������ļ���
	printExtendedGrammar();

	//��ʼ��first��
	initFirst();
	fgrammar.close();
	ferr.close();
	return SUCCESS;
}

/**
 * @brief�� �����ǰ���ս���ͷ��ս������
 * @param�� 
 * @return  
 * */
void grammar::printExtendedGrammar() {
	
	fstream file_open;
	file_open.open(EXTENDED_GRAMMAR_FILE, ios::out);
	file_open << symbols[0].name << " ";
	file_open << "�ս��" << endl;
	for (set<int>::iterator i = terminals.begin(); i != terminals.end(); i++)
	{
		int index = *i;
		file_open << symbols[index].name << " ";
	}
	file_open << endl;
	file_open << "���ս��" << endl;
	for (set<int>::iterator i = non_terminals.begin(); i != non_terminals.end(); i++)
	{
		int index = *i;
		file_open << symbols[index].name << "  ";
	}
	file_open << endl;

	//������е�productions
	file_open << "�﷨����" << endl;

	for (int i = 0; i < productions.size(); i++)
	{
		file_open << "production" << i << "  ";
		file_open << symbols[productions[i].left_symbol].name;
		file_open << "  --->  ";
		for (int j = 0; j < productions[i].right_symbol.size(); j++) {
			file_open << symbols[productions[i].right_symbol[j]].name << " ";
		}
		file_open << endl;
	}

}


/**
 * @brief����Ӧ������symbol�е�����
 * @param�� onst string token ��Ԫ����
 * @return ���ض�Ӧ������symbol�е���������������ڣ�����-1
 * */
int grammar::symbolIndex(const string token) {

	//�������ű� �ҵ���Ӧ��λ��
	for (int i = 0; i < symbols.size(); i++)
	{
		if (token == symbols[i].name)
		{
			return i;
		}
	}
	return -1;
}

/**
 * @brief������first��
 * @param�� 
 * @return 
 * */
void grammar::initFirst() {

	initFirstTerm();
	initFirstNonTerm();
	printFirst();
}


/**
 * @brief�������ս����first����
 * @param��
 * @return 
 * */
void grammar::initFirstTerm() {
	//������ս����first���Ͼ������Ǳ���
	for (set<int>::iterator i = terminals.begin(); i != terminals.end(); i++)
	{
		symbols[*i].first_set.insert(*i);
	}
}

/**
 * @brief�����ɷ��ս����first����
 * @param��
 * @return
 * */
void grammar::initFirstNonTerm() {
	while (1) {
		//�������ս��
		bool useful_action = 0;;
		for (set<int>::iterator i = non_terminals.begin(); i != non_terminals.end(); i++)
		{
			//����production
			for (int j = 0; j < productions.size(); j++)
			{
				if (productions[j].left_symbol == *i) {

					if (symbols[productions[j].right_symbol[0]].type == symbolType::terminal || symbols[productions[j].right_symbol[0]].type == symbolType::epsilon)
					{
						useful_action = symbols[*i].first_set.insert(productions[j].right_symbol[0]).second || useful_action;
						continue;
					}
					//���ս��
					bool isAllNull = 1;
					int k = 0;
					for (k = 0; k < productions[j].right_symbol.size(); k++)
					{
						//����Ҳ����ս��
						if (symbols[productions[j].right_symbol[k]].type == symbolType::terminal)
						{
							useful_action = symbols[*i].first_set.insert(productions[j].right_symbol[k]).second || useful_action;

							isAllNull = false;
							break;
							//����һ���ķ�
						}
						else {
							//����Ҳ��Ƿ��ս��
							useful_action = mergeSet(symbols[productions[j].right_symbol[k]].first_set, symbols[*i].first_set, symbolIndex(EpsilonToken)) || useful_action;
							isAllNull = symbols[productions[j].right_symbol[k]].first_set.count(symbolIndex(EpsilonToken)) && isAllNull;

							if (!isAllNull)
								break;
						}
					}
					if (k == productions[j].right_symbol.size() && isAllNull)
						useful_action = symbols[*i].first_set.insert(symbolIndex(EpsilonToken)).second || useful_action;
				}
			}

		}
		if (useful_action == 0)
			break;
	}
	
}

/**
 * @brief����ӡfirst��
 * @param�� 
 * @return 
 * */
void grammar::printFirst()
{
	fstream file_open;
	file_open.open(FIRST_SET_FILE, ios::out);
	//���ÿ��symbol��first����
	for (set<int>::iterator it = terminals.begin(); it != terminals.end(); it++)
	{
		
		file_open << symbols[*it].name;
		file_open << ": first set: {";

		for (set<int>::iterator i = symbols[*it].first_set.begin(); i != symbols[*it].first_set.end(); i++)
		{
			file_open << symbols[*i].name << " ";
		}
		file_open <<"}"<<endl;
	}
	for (set<int>::iterator it = non_terminals.begin(); it != non_terminals.end(); it++)
	{
		
		file_open << symbols[*it].name;
		file_open << ": first set: {";

		for (set<int>::iterator i = symbols[*it].first_set.begin(); i != symbols[*it].first_set.end(); i++)
		{
			file_open << symbols[*i].name << " ";
		}
		file_open <<"}"<< endl;
	}
}


/**
 * @brief����ȡfirst��
 * @param��const vector<int>& str  ���봮
 * @return �������������first��
 * */
set<int>grammar::getFirst(const vector<int>& str) {
	set<int>first_set;
	
	if (str.empty()) {
		return first_set;
	}
	bool is_epsilon = true;
	int empty_location = symbolIndex(EpsilonToken);
	for (auto i = str.begin(); i != str.end(); i++)
	{
		if (symbols[*i].type == symbolType::terminal)
		{
			is_epsilon = false;
			mergeSet(symbols[*i].first_set, first_set, empty_location);

			break;
		}
		
		mergeSet(symbols[*i].first_set, first_set, empty_location);
		is_epsilon = symbols[*i].first_set.count(empty_location) && is_epsilon;
		if (!is_epsilon)
			break;
	}
	if (is_epsilon)
		first_set.insert(empty_location);
	return first_set;
}



//���ߺ���
//�ַ����ָ��  
vector<string> split(const string& str, const string& pattern) {
	vector<string> res;
	if (str == "")
		return res;
	//���ַ���ĩβҲ����ָ����������ȡ���һ��
	string strs = str + pattern;
	size_t pos = strs.find(pattern);

	while (pos != strs.npos)
	{
		string temp = strs.substr(0, pos);
		res.push_back(temp);
		//ȥ���ѷָ���ַ���,��ʣ�µ��ַ����н��зָ�
		strs = strs.substr(pos + pattern.length(), strs.size());
		pos = strs.find(pattern);
	}
	return res;
}


//ȥ����β �ո��
void Trimmed(std::string& str) {
	str.erase(0, str.find_first_not_of(" \r\n\t\v\f"));
	str.erase(str.find_last_not_of(" \r\n\t\v\f") + 1);
}

int mergeSet(set<int>& src, set<int>& dst, int null_index)
{
	set<int>temp;
	set_difference(src.begin(), src.end(), dst.begin(), dst.end(), inserter(temp, temp.begin()));
	//���dst��û�пմ����Ѳ����մ���ɾ��
	bool isNullExist = dst.find(null_index) != dst.end();
	int pre_size = dst.size();
	dst.insert(temp.begin(), temp.end());
	if (!isNullExist)
	{
		dst.erase(null_index);
	}
	return pre_size < dst.size();
}