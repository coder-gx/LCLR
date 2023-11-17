#include "LR1.h"

#define SUCESS 1
#define ACTION_ERROR 2
#define GOTO_ERROR  3
#define REDUCTION_PROCESS_FILE_OPEN_ERROR 4
#define PARSE_ERROR_FILE_OPEN_ERROR 5
#define TREE_DOT_FILE_OPEN_ERROR 6
#define DFA_DOT_FILE_OPEN_ERROR 7


/*********************************LR1 Item********************************/

/**
 * @brief�� lr1Item���캯��
 * @param��  int l   ����ʽ��ߵķ���
             vector<int> &r   ����ʽ�ұߵķ���
			 int ds     �����λ��
			 int fw     ��ǰ���ķ���
			 int gi      ������Item���ķ�ʽ���

 * @return 
 * */
lr1Item::lr1Item(int l, vector<int>& r, int ds, int fw, int gi)
{
	this->left = l;
	this->right = r;
	this->dot_pos = ds;
	this->forward = fw;
	this->grammar_index = gi;
}

/**
 * @brief�� lr1Item��ʼ������
 * @param��  int l   ����ʽ��ߵķ���
			 vector<int> &r   ����ʽ�ұߵķ���
			 int ds     �����λ��
			 int fw     ��ǰ���ķ���
			 int gi      ������Item���ķ�ʽ���

 * @return
 * */
void lr1Item::lr1ItemInit(int l, vector<int>& r, int ds, int fw, int gi)
{
	this->left = l;
	this->right = r;
	this->dot_pos = ds;
	this->forward = fw;
	this->grammar_index = gi;
}

/**
 * @brief��   ����������ж�lr1Item�ȼ�
 * @param��   const lr1Item& item
 * @return   �ж�lr1Item�Ƿ�ȼ�
 * */
bool lr1Item::operator==(const lr1Item& item)
{
	if (this->grammar_index == item.grammar_index && this->dot_pos == item.dot_pos && this->forward == item.forward)
		return true;
	return false;
}


/*********************************LR1 Closure********************************/
/**
 * @brief��   item�Ƿ��ڵ�ǰ�հ�
 * @param��   lr1Item item
 * @return   item�Ƿ��ڵ�ǰ�հ�
 * */
bool lr1Closure::isIn(lr1Item item)
{
	for (int i = 0; i < this->closure.size(); i++)
	{
		if (item == this->closure[i])
			return true;
	}
	return false;
}

/**
 * @brief��   ����������ж�lr1Closure�ȼ�
 * @param��   lr1Closure clos
 * @return   �ж�lr1Closure�Ƿ�ȼ�
 * */
bool lr1Closure::operator==(lr1Closure& clos)
{
	if (this->closure.size() != clos.closure.size())
		return false;

    //����size����ȵĲŻᵽ����
	for (int i = 0; i < clos.closure.size(); i++)
	{
		if (!this->isIn(clos.closure[i]))
			return false;
	}
	return true;
}

/**
 * @brief��   �õ����ƽ����ַ��Լ���Ŀ�ڱհ��е�λ��
 * @param��  
 * @return   
 * */
unordered_map<int, vector<int>> lr1Closure::getShiftinSymbol()
{
	unordered_map<int, vector<int>> MAP;
	for (int i = 0; i < this->closure.size(); i++)
	{
		lr1Item temp = this->closure[i];
		if (temp.dot_pos >= temp.right.size())//�㵽ͷ�ˣ��ǹ�Լ��Ŀ
			continue;
		int present_symbol = temp.right[temp.dot_pos];//��ǰ�������ַ�
		//���ڸ��ַ�
		if (MAP.find(present_symbol) != MAP.end())
			MAP[present_symbol].push_back(i);//������Ŀ�����
		else
		{
			vector<int> target;
			target.push_back(i);
			MAP[present_symbol] = target;//����
		}
	}
	return MAP;
}

/**
 * @brief��   �õ����Թ�Լ�ķ��źͶ�Ӧ�Ĳ���ʽ�����
 * @param��
 * @return
 * */
vector<pair<int, int>> lr1Closure::getReduceSymbol()
{
	vector<pair<int, int>> a;
	for (int i = 0; i < this->closure.size(); i++)
	{
		lr1Item temp = this->closure[i];
		if (temp.dot_pos >= temp.right.size())//�㵽ͷ�ˣ��ǹ�Լ��Ŀ
		{
			a.push_back(pair<int, int>(temp.forward, temp.grammar_index));
		}
		else
			continue;
	}
	return a;
}


/*********************************LR1 Grammar********************************/


/**
 * @brief��   �жϱհ��������Ƿ��иñհ���
 * @param��    lr1Closure& clos�հ�
 * @return   ���з�����ţ���û�з���-1
 * */
int lr1Grammar::getClosureIndex(lr1Closure& clos)
{
	for (int i = 0; i < this->closure_sum.size(); i++)
	{
		if (clos == this->closure_sum[i])
			return i;
	}
	return -1;
}



/**
 * @brief��   ����0����Ŀ���հ�
 * @param��   
 * @return
 * */
void lr1Grammar::initClosure0()
{
	start_item.lr1ItemInit(productions[0].left_symbol, productions[0].right_symbol, 0, symbolIndex(EndToken), start_index);
	vector<lr1Item>lr1;
	lr1.push_back(start_item);
	start_closure = generateClosure(lr1);
}

/**
 * @brief��   ������Ŀ���հ�
 * @param��   vector<lr1Item> lr  ���е�lr1����ʽ
 * @return    ���ɵıհ�
 * */
lr1Closure lr1Grammar::generateClosure(vector<lr1Item> lr1)
{
	// ���� LR(1) �հ�����
	lr1Closure current_closure;
	current_closure.closure = lr1;

	// ����������
	for (int i = 0; i < current_closure.closure.size(); i++) {
		// ����ǰ LR(1) ��
		lr1Item item_now = current_closure.closure[i];

		// ����������һ��λ�ã���������
		if (item_now.dot_pos >= item_now.right.size()) {
			continue;
		}

		// ��ǰ��Ĳ���ʽ�У�������ķ��Ŷ�Ӧ������
		int dot_next_symbol_index = item_now.right[item_now.dot_pos];
		symbol dot_next_symbol = symbols[dot_next_symbol_index];

		// ����������ǿմ���������ǰ�ƶ�һλ
		if (dot_next_symbol.type == symbolType::epsilon) {
			current_closure.closure[i].dot_pos++;
			continue;
		}

		// ������������ս������������
		if (dot_next_symbol.type == symbolType::terminal) {
			continue;
		}

		// ��ȡ��A����������ķ��ż���֮��ķ���
		vector<int> BetaA(item_now.right.begin() + item_now.dot_pos + 1, item_now.right.end());
		BetaA.push_back(item_now.forward);

		// ��ȡ��A �� FIRST ����
		set<int> betaA_first_set = getFirst(BetaA);

		// ���ڲ���ʽ A->����B��,a
		// ���ҷ��� B �Ĳ���ʽ B->XX �������� FIRST(��,a)
		for (int j = 0; j < productions.size(); j++) {
			production production_now = productions[j];
			if (dot_next_symbol_index == production_now.left_symbol) {
				for (auto it = betaA_first_set.begin(); it != betaA_first_set.end(); it++) {
					// ���հ����Ƿ��Ѿ����ڸ� LR(1) ��
					bool have_exist = false;
					bool is_epsilon = false;
					is_epsilon = (symbols[production_now.right_symbol[0]].type == symbolType::epsilon);

					for (auto temp = current_closure.closure.begin(); temp != current_closure.closure.end(); temp++) {
						if (*temp == lr1Item(production_now.left_symbol, production_now.right_symbol, 0, *it, j)) {
							have_exist = true;
							break;
						}
					}

					// ��������ڣ�����ӵ��հ���
					if (!have_exist) {
						current_closure.closure.push_back(lr1Item(production_now.left_symbol, production_now.right_symbol, is_epsilon, *it, j));
					}
				}
			}
		}
	}

	return current_closure;
}


/**
 * @brief��   ����DFA
 * @param��   
 * @return  
 * */
void lr1Grammar::generateDFA()
{
	// ����ʼ�հ�����հ�����
	this->closure_sum.push_back(this->start_closure);

	// ʹ�ö���ѭ������������
	queue<lr1Closure> q;
	q.push(this->start_closure);

	while (!q.empty())
	{
		// ȡ�������е�һ���հ���Ϊ��ǰԴ�հ�
		lr1Closure source = q.front();
		q.pop();

		// ��ȡԴ�հ��������ƽ������Լ���Ӧ����Ŀ����
		unordered_map<int, vector<int>> shift_symbol = source.getShiftinSymbol();

		// ���������ƽ����ż���Ӧ����Ŀ����
		for (auto it = shift_symbol.begin(); it != shift_symbol.end(); it++)
		{
			// ���˴�ʱ�ƽ����ַ�
			// ��Ҫ��õ�����ƶ�һλ�����Ŀ��Ȼ����� vector ����հ�
			vector<lr1Item> next_closure_key;
			for (int j = 0; j < it->second.size(); j++)
			{
				lr1Item temp = source.closure[it->second[j]];
				temp.dot_pos++; // ������ƶ�һλ������������
				next_closure_key.push_back(temp); // �����ڸ��ַ��¿����ƽ��� LR1 ��Ŀ
			}

			// ������һ���հ�
			lr1Closure next_closure = generateClosure(next_closure_key);
			int next_closure_site = getClosureIndex(next_closure);

			// �����һ���հ������ڣ����������кͱհ�����
			if (next_closure_site == -1)
			{
				q.push(next_closure);
				this->closure_sum.push_back(next_closure);
			}

			// �����ڲ��ڣ�DFA ��Ҫ���ϱߣ���¼�ӵ�ǰ�հ�ͨ���ƽ����ŵ�����һ���հ�
			this->DFA[pair<int, int>(getClosureIndex(source), it->first)] = getClosureIndex(next_closure);
		}
	}
}


/**
 * @brief��   ����ACTION GOTO��
 * @param��  
 * @return   
 * */
void lr1Grammar::generateACTION_GOTO()
{
	//����DFA�õ��ƽ�����
	for (auto it = this->DFA.begin(); it != this->DFA.end(); it++)
	{
		//ת�Ʒ������ս������¼��ACTION����
		if (terminals.find(it->first.second) != terminals.end())
		{
			ACTIONItem act_item(actionOption::SHIFT_IN, it->second);
			this->ACTION[pair<int, int>(it->first.first, it->first.second)] = act_item;
		}
		else
		{
			//���ս����¼��GOTO����
			GOTOItem GOTOItem(gotoOption::GO, it->second);
			this->GOTO[pair<int, int>(it->first.first, it->first.second)] = GOTOItem;
		}
	}

	//���ݱհ��Ĺ�Լ��õ���Լ/���ܶ���
	for (int i = 0; i < this->closure_sum.size(); i++)
	{
		vector<pair<int, int>> reduce_line = this->closure_sum[i].getReduceSymbol();
		for (int j = 0; j < reduce_line.size(); j++)
		{
			if (reduce_line[j].second == start_index)
			{
				ACTIONItem act_item(actionOption::ACCEPT, reduce_line[j].second);
				this->ACTION[pair<int, int>(i, reduce_line[j].first)] = act_item;
			}
			else
			{
				ACTIONItem act_item(actionOption::REDUCE, reduce_line[j].second);
				this->ACTION[pair<int, int>(i, reduce_line[j].first)] = act_item;
			}
		}

	}
}

/**
 * @brief��   ��ӡaction goto��csv�ļ���
 * @param��    
 * @return   
 * */
void lr1Grammar::printACTION_GOTO()
{
	const int width = 10;
	const int interval = 10;
	const int start_state = 5;
	const int state_action = 20;
	const int action_goto = 230;

	ofstream ofs(Table_FILE, ios::out);
	ofs.setf(std::ios::left);
	ofs << "STATE" << "," << "ACTION" << ",";
	for (int i = 0; i < terminals.size(); i++) {
		ofs << ",";
	}
	ofs << "GOTO" << endl;
	int rowNum = this->closure_sum.size();
	int columnNum = this->symbols.size();
	int actionNum = this->terminals.size();
	int gotoNum = this->non_terminals.size();
	vector<int> terminal_site;//��¼�ս����symbols��������
	vector<int> non_terminal_site;//��¼���ս����symbols��������

	ofs << ",";

	for (int i = 0; i < columnNum; i++)
	{
		if (terminals.find(i) != terminals.end())
		{
			if (symbols[i].name == ",")
				ofs << "\"" << symbols[i].name << "\"" << ",";
			else
				ofs << symbols[i].name << ",";
			terminal_site.push_back(i);
		}
	}

	for (int i = 0; i < columnNum; i++)
	{
		if (non_terminals.find(i) != non_terminals.end())
		{

			ofs << symbols[i].name << ",";
			non_terminal_site.push_back(i);
		}
	}

	ofs << endl;

	for (int i = 0; i < rowNum; i++)
	{
		ofs << i << ",";
		for (int j = 0; j < actionNum; j++)
		{
			auto it = ACTION.find(pair<int, int>(i, terminal_site[j]));
			if (it != ACTION.end())
			{
				if (it->second.op == actionOption::SHIFT_IN)
					ofs << "s" << it->second.serial << ",";
				else if (it->second.op == actionOption::REDUCE)
					ofs << "r" << it->second.serial << ",";
				else if (it->second.op == actionOption::ACCEPT)
					ofs << "acc" << ",";
			}
			else
				ofs << ",";
		}
		for (int j = 0; j < gotoNum; j++)
		{
			auto it = GOTO.find(pair<int, int>(i, non_terminal_site[j]));
			if (it != GOTO.end())
				ofs << it->second.serial << ",";
			else
				ofs << ",";
		}

		ofs << endl;
	}
	ofs.close();
}


/**
 * @brief��   ��Լ����
 * @param��   vector<elem>& lexical_result  �ʷ������Ľ��
 * @return   ��Լ��������
 * */
int lr1Grammar::reduction(vector<elem>& lexical_result)
{
	// ��ʼ��״̬ջ������ջ�����ջ
	vector<int> status_stack;
	vector<int> symbol_stack;
	vector<int> serial_stack;
	int step = 0;
	int node_serial = 0;
	int err_code = 1;  // �����룬Ĭ��Ϊ1

	// �����봮�������Ͻ�������
	elem end(EndToken, EndToken, 0);
	lexical_result.push_back(end);

	// ��ʼ״̬ջ�ͷ���ջ
	status_stack.push_back(0);  // ״̬ջ��ѹ��״̬0
	symbol_stack.push_back(symbolIndex(EndToken));  // �ڷ���ջ���ȷ����������

	// ���ļ���������
	ofstream ofs(REDUCTION_PROCESS_FILE, ios::out);
	if (ofs.is_open() == 0) {
		err_code = REDUCTION_PROCESS_FILE_OPEN_ERROR;
	}

	ofstream fwrong(PARSE_ERROR_FILE, ios::out);
	if (fwrong.is_open() == 0) {
		err_code = PARSE_ERROR_FILE_OPEN_ERROR;
	}

	ofstream ftree(TREE_DOT_FILE, ios::out);
	if (!ftree.is_open()) {
		err_code = TREE_DOT_FILE_OPEN_ERROR;
	}
	ftree << "digraph mygraph {\n";

	// �����һ��
	ofs << "STEP: " << step << endl;
	ofs << "STATUS_STACK: ";
	for (int t = 0; t < status_stack.size(); t++)
		ofs << " " << status_stack[t];
	ofs << endl;
	ofs << "SYMBOL_STACK: ";
	for (int t = 0; t < symbol_stack.size(); t++)
		ofs << symbols[symbol_stack[t]].name;
	ofs << endl << "INPUT: ";
	for (int t = 0; t < lexical_result.size(); t++)
		ofs << lexical_result[t].value;
	ofs << endl << endl;
	step++;

	int reduce_error_status = -1;
	int reduce_error_symbol = -1;

	// ��ʼ�����﷨����
	for (int i = 0; i < lexical_result.size(); i++)
	{
		string present_terminal = lexical_result[i].type;
		int present_terminal_serial = symbolIndex(present_terminal);
		int present_status = status_stack.back();
		auto it = ACTION.find(pair<int, int>(present_status, present_terminal_serial));

		// �����ڣ���REJECT�������˳�
		if (it == ACTION.end())
		{
			err_code = ACTION_ERROR;
		}
		else
		{
			switch (it->second.op)
			{
				case actionOption::SHIFT_IN:
				{
					// �ƽ�
					status_stack.push_back(it->second.serial);  // ��״̬��ջ
					symbol_stack.push_back(present_terminal_serial);  // ������ս��ѹջ
					serial_stack.push_back(node_serial);
					ftree << "n" << node_serial++ << "[label=\"" << lexical_result[i].value << "\",color=red];" << endl;
					break;
				}
				case actionOption::REDUCE:
				{
					// ��Լ��Ҫ��Լ��ǰ���봮����һ����
					i--;
					production production_need = productions[it->second.serial];  // Ҫʹ�õĲ���ʽ
					int right_length = production_need.right_symbol.size();  // Ҫ��Լ���ĳ���
					if (right_length == 1)
					{
						// ���� epsilon����Ϊ��� size ��1����ʵ�� length ��0
						if (production_need.right_symbol[0] == symbolIndex("@"))
							right_length = 0;
					}
					vector<int> drop;
					for (int k = 0; k < right_length; k++)
					{
						status_stack.pop_back();  // ״̬ջ�Ƴ�
						symbol_stack.pop_back();  // ����ջ�Ƴ�
						drop.push_back(serial_stack.back());
						serial_stack.pop_back();
					}
					symbol_stack.push_back(production_need.left_symbol);  // ����ջѹ����ս��
					int temp_status = status_stack.back();

					// ��Լ֮��鿴 GOTO ��
					auto goto_it = GOTO.find(pair<int, int>(temp_status, production_need.left_symbol));
					if (goto_it == GOTO.end())  // ������ת�ƣ���Ӧ�˳� GOTO���������
					{
						err_code = GOTO_ERROR;
						reduce_error_status = temp_status;
						reduce_error_symbol = production_need.left_symbol;
						break;
					}
					else
					{
						if (goto_it->second.op == gotoOption::GO)
						{
							status_stack.push_back(goto_it->second.serial);  // ����״̬ѹջ
							serial_stack.push_back(node_serial);
						}
						else  // �������
						{
							err_code = GOTO_ERROR;
							break;
						}
					}

					ftree << "n" << node_serial++ << "[label=\"" << symbols[production_need.left_symbol].name << "\"];\n";
					if (right_length == 0)
					{
						ftree << "e" << node_serial << "[label=\"@\"];\n";
						ftree << "n" << node_serial - 1 << " -> " << "e" << node_serial << ";\n";
					}
					else
					{
						for (auto t = drop.begin(); t != drop.end(); t++)
							ftree << "n" << node_serial - 1 << " -> " << "n" << *t << ";\n";
					}
					break;
				}
				case actionOption::ACCEPT:
				{
					ftree << "}";
					ftree.close();
					// ����״̬��ֱ�ӷ���
					ofs << endl << "Parse successfully!" << endl;
					ofs.close();
					fwrong.close();
					return SUCESS;
				}
				case actionOption::REJECT:
				{
					err_code = ACTION_ERROR;
					break;
				}
				default:
					break;
			}
		}

		// �� error��ֱ���˳�
		if (err_code == ACTION_ERROR)
		{
			fwrong << "�Ҳ���" << lexical_result[i].value << "��ת��"
				<< ", �÷�������Ϊ" << lexical_result[i].type << ",����λ��Դ�����" << lexical_result[i].line << "��" << endl;
			this->error_line = lexical_result[i].line;
			ofs << endl << "�﷨��������:�����ڵ�ǰ��ACTION!" << endl;
			ofs << "��ǰ״̬: " << present_status << endl;
			ofs << "��ǰ�ս������: " << present_terminal << endl;
			break;
		}
		else if (err_code == GOTO_ERROR)
		{
			fwrong << "�Ҳ���" << lexical_result[i].value << "��ת��"
				<< ", �÷�������Ϊ" << lexical_result[i].type << ",����λ��Դ�����" << lexical_result[i].line << "��" << endl;
			this->error_line = lexical_result[i].line;
			ofs << endl << "�﷨��������:�����ڵ�ǰ��GOTO!" << endl;
			ofs << "��ǰ״̬: " << reduce_error_status << endl;
			ofs << "��ǰ�ս������: " << symbols[reduce_error_symbol].name << endl;
			break;
		}

		// �����һ��
		ofs << "STEP: " << step << endl;
		ofs << "STATUS_STACK: ";
		for (int t = 0; t < status_stack.size(); t++)
		{
			ofs << " " << status_stack[t];
		}
		ofs << endl << "STATUS_SYMBOL: ";
		for (int t = 0; t < symbol_stack.size(); t++)
		{
			ofs << symbols[symbol_stack[t]].name;
		}
		ofs << endl << "INPUT: ";
		for (int t = i + 1; t < lexical_result.size(); t++)
		{
			ofs << lexical_result[t].type;
		}
		ofs << endl << endl;
		step++;
	}

	ofs.close();
	fwrong.close();
	ftree.close();
	return err_code;
}




int lr1Grammar::printParseDFA() {


	ofstream fdot(DFA_DOT_FILE,ios::out);
	if (!fdot.is_open()) {
		return DFA_DOT_FILE_OPEN_ERROR;
	}
	
	fdot << "digraph G {\n";
	fdot << "node [shape=rectangle, fontname=\"SimSun\"];\n";
    
	for (const auto& entry : DFA) {
		fdot << "  \"I" << entry.first.first << "\" -> \"I" << entry.second << "\" [label=\"";
		// �˴�������ſ���ֱ�������ʵ�ʿ�����Ҫת��
		fdot << symbols[entry.first.second].name << "\"];\n";
	}

	fdot << "}\n";
	fdot.close();
	return SUCESS;
}




