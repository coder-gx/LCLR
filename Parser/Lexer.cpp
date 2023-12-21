#include "lexer.h"
#include<qdebug.h>


#define SUCCESS 1
#define CODE_FILE_OPEN_ERROR 2
#define LEXICAL_ANALYSIS_ERROR 3


/**
 * @brief�� elem�Ĺ��캯��
 * @param�� string tp elem������
 *          string v  elem��ֵ
 * @return 
 */
elem::elem(string tp, string v,int line_count)
{
    this->type = tp;//����
    this->value = v;//ֵ
    this->line = line_count;//��������
}

/**
 * @brief�� lexer�Ĺ��캯��
 * @param�� 
 * @return
 */
lexer::lexer()
{
    this->current_buffer = 0;
    this->comment_flag = 0;
}

lexer::~lexer()
{

}



/**
 * @brief�� �﷨������
 * @param�� string file_name  Դ�����ļ�
 * @return  �������
 */
int lexer::lexicalAnalysis(string file_name) {
  
    if ((this->fcode = fopen(file_name.c_str(), "r")) == NULL)
    {
        return  CODE_FILE_OPEN_ERROR;
    }

    this->ftoken = fopen(TOKEN_LIST_FILE, "w");//�洢�ʷ��������
    fprintf(this->ftoken, "Token����    Tokenֵ      ��������\n");
    this->ferror = fopen(ERROR_LIST_FILE, "w");
    
    char c = '\0';
    int buffer_flag = 0;//�������Ƿ���Ҫ��ת
    int line_count = 0;//��¼��ǰ����

    while (1)
    {
        c = fgetc(this->fcode);
        if (c == EOF)//�����ȡ����
        {
            line_count++;
            this->deleComments(line_count);// ɾ��ע��
            this->deleSpaces(line_count); // ɾ������ո�
            if (this->double_buffer[this->current_buffer].count > 0)
            {
                strcpy(this->final_buffer.buffer, this->double_buffer[this->current_buffer].buffer);
                this->final_buffer.count = this->double_buffer[this->current_buffer].count;
                //����״̬������ 
                this->double_buffer[current_buffer].count = 0;
                if (!this->DFA(line_count)) {
                    fclose(fcode);
                    fclose(ftoken);
                    fclose(ferror);
                    this->error_line = line_count;
                    return LEXICAL_ANALYSIS_ERROR;
                }
            }
            break;
        }
        //���������
        if (this->double_buffer[this->current_buffer].count == BUFFER_SIZE - 2)
        {
            this->double_buffer[this->current_buffer].buffer[this->double_buffer[this->current_buffer].count] = c;
            int i;
            for (i = 0; i < this->double_buffer[this->current_buffer].count; i++)
            {
                if (isDelimiter(double_buffer[this->current_buffer].buffer[i]))
                {
                    int j;//�ֽ��
                    int k;
                    // ת�ƻ���������
                    for (j = 0, k = i + 1; k <= double_buffer[current_buffer].count; k++, j++)
                    {
                        double_buffer[1 - current_buffer].buffer[j] = double_buffer[current_buffer].buffer[k];
                    }
                    //count��С��������
                    double_buffer[1 - current_buffer].count = j;
                    double_buffer[current_buffer].count = i;
                    //�����ս��
                    double_buffer[1 - current_buffer].buffer[j] = '\0';
                    double_buffer[current_buffer].buffer[i + 1] = '\0';
                    buffer_flag = 1; // ���û�������ת��־
                    break;
                }
            }
        }
        else if (c == '\n')
        {
            line_count++;
            double_buffer[current_buffer].buffer[double_buffer[current_buffer].count] = '\0';
        }
        else {
            double_buffer[current_buffer].buffer[double_buffer[current_buffer].count++] = c;
            continue;//����
        }
        //���������к�/���������Ĵ���
        deleComments(line_count);
        deleSpaces(line_count);

        if (double_buffer[current_buffer].count > 0)
        {
            strcpy(final_buffer.buffer, double_buffer[current_buffer].buffer);
            final_buffer.count = double_buffer[current_buffer].count;
            double_buffer[current_buffer].count = 0;
            if (!this->DFA(line_count)) {
                fclose(fcode);
                fclose(ftoken);
                fclose(ferror);
                this->error_line = line_count;
                return LEXICAL_ANALYSIS_ERROR;
            }
        }

        if (buffer_flag == 1)
        {
            //��һ�� ��������ת
            double_buffer[current_buffer].count = 0;
            current_buffer = 1 - current_buffer;
            buffer_flag = 0;
        }
    }
    fclose(fcode);
    fclose(ftoken);
    fclose(ferror);
    return SUCCESS;
}


/**
 * @brief�� ʶ��ע��
 * @param�� int line_count  ��ǰ����
 * @return  
 * */
void lexer::deleComments(int line_count) {
    char comment[BUFFER_SIZE];
    int comment_count = 0;
    bool flag_qoute = 0;
    //״̬�� �����ǡ���������/����ѭ��
    for (int i = 0; double_buffer[current_buffer].buffer[i] != '\0'; i++)
    {
        if (double_buffer[current_buffer].buffer[i] == '"')
        {
            flag_qoute = 1 - flag_qoute;
            if (comment_flag != 1)
                continue;
        }
        if (flag_qoute == 1)
            if (comment_flag != 1)
                continue;
        if (double_buffer[current_buffer].buffer[i] == '/' || comment_flag == 1)
        {
            if (double_buffer[current_buffer].buffer[i + 1] == '\0')
            {
                continue;
            }
            if (double_buffer[current_buffer].buffer[i + 1] == '/' && !comment_flag)
            {
                //���� //״̬ ֱ��\0ֹͣ
                int j;

                for (j = i; double_buffer[current_buffer].buffer[j] != '\0'; j++)
                {
                    comment[comment_count++] = double_buffer[current_buffer].buffer[j];
                    double_buffer[current_buffer].buffer[j] = '\0';
                }
                comment[comment_count] = '\0';
                fprintf(ftoken, "ע��         %s     %d\n", comment,line_count);
                double_buffer[current_buffer].count -= comment_count;
                comment_count = 0;
                break;
            }
            if (double_buffer[current_buffer].buffer[i + 1] == '*' || comment_flag == 1)
            {
                //����/* ״̬ 
                comment_flag = 1;
                int j;
                for (j = i + 2 * (1 - comment_flag); double_buffer[current_buffer].buffer[j] != '\0'; j++)
                {

                    comment[comment_count++] = double_buffer[current_buffer].buffer[j];
                    if (!flag_qoute && double_buffer[current_buffer].buffer[j] == '*' && double_buffer[current_buffer].buffer[j + 1] == '/')
                    {
                        comment_flag = 0;
                        comment[comment_count++] = '/';
                        comment[comment_count] = '\0';
                        fprintf(ftoken, "ע��         %s    %d\n", comment,line_count);
                        double_buffer[current_buffer].count -= comment_count;
                        comment_count = 0;
                        break;
                    }
                }

                if (comment_flag == 0)
                    j = j + 2;

                //��ʼǰ��
                for (; double_buffer[current_buffer].buffer[j] != '\0'; j++, i++)
                {
                    if (double_buffer[current_buffer].buffer[j] == '\n')
                    {
                        i--;
                        continue;
                    }
                    double_buffer[current_buffer].buffer[i] = double_buffer[current_buffer].buffer[j];
                }

                if (comment_flag) {
                    //��ζ�Ŷ���ע�ͣ�ֱ��printf
                    comment[comment_count] = '\0';
                    fprintf(ftoken, "ע��         %s   %d\n", comment,line_count);
                    double_buffer[current_buffer].buffer[i] = '\0';
                    double_buffer[current_buffer].count -= comment_count;
                    break;
                }
                double_buffer[current_buffer].buffer[i] = '\0';
            }

        }
    }
}



/**
 * @brief�� ɾ���ո�
 * @param�� int line_count  ��ǰ����
 * @return
 * */
void lexer::deleSpaces(int line_count) 
{
    bool inString= true, inChar = true;
    for (int i = 0; double_buffer[current_buffer].buffer[i] != '\0'; i++)
    {
        char cur_char = double_buffer[current_buffer].buffer[i];
        //����ɾ���ַ����ڵĿո�
        if ( cur_char== '"')
            inString = !inString;
        //����ɾ���ո��ַ�
        if (cur_char == '\'')
            inChar = !inChar;
        if ((cur_char == ' ' || cur_char == '\t') && inString && inChar)
        {
            // ���������ո���Ʊ���Ľ���λ��
            int j = i + 1;
            for (; double_buffer[current_buffer].buffer[j] != '\0' && (double_buffer[current_buffer].buffer[j] == ' ' || double_buffer[current_buffer].buffer[j] == '\t'); j++)
            {
            }
            //����ո���Ʊ���ڻ�����ĩβ����ֱ�ӽض��ַ���
            if (double_buffer[current_buffer].buffer[j] == '\0')
            {
                double_buffer[current_buffer].buffer[i] = '\0';
                double_buffer[current_buffer].count -= (j - i);
                break;
            }
            // ����ո���Ʊ���ڻ�������ͷ����ȫ��ɾ��
            if (i == 0)
            {
                int k = i;
                double_buffer[current_buffer].count -= (j - i);
                for (; double_buffer[current_buffer].buffer[j] != '\0'; j++, k++)
                    double_buffer[current_buffer].buffer[k] = double_buffer[current_buffer].buffer[j];
                double_buffer[current_buffer].buffer[k] = '\0';
                i--; // ����һ��λ��
            }
            else
            {
                //���֮���ж���ո���ɾ��ֻ��һ��
                if (j - i >= 2)
                {
                    int k = i + 1;
                    double_buffer[current_buffer].count -= (j - (i + 1));
                    for (; double_buffer[current_buffer].buffer[j] != '\0'; j++, k++)
                        double_buffer[current_buffer].buffer[k] = double_buffer[current_buffer].buffer[j];
                    double_buffer[current_buffer].buffer[k] = '\0';
                    j = i + 1;
                }
                if (double_buffer[current_buffer].buffer[j] != '\0' && ((spaceRemovable(double_buffer[current_buffer].buffer[j]) || 
                    (i > 0 && spaceRemovable(double_buffer[current_buffer].buffer[i - 1])))))
                {
                    int k = i;
                    double_buffer[current_buffer].count -= (j - i);
                    for (; double_buffer[current_buffer].buffer[j] != '\0'; j++, k++)
                        double_buffer[current_buffer].buffer[k] = double_buffer[current_buffer].buffer[j];
                    double_buffer[current_buffer].buffer[k] = '\0';
                    i--;
                }

            }
        }

    }

}

/**
 * @brief�� DFA״̬��
 * @param�� int line_count  ��ǰ����
 * @return  DFA�Ƿ���������
 * */
bool lexer::DFA(int line_count)
{
    char dfa_token[BUFFER_SIZE];
    int count = 0;//��ǰdfa_token�е��ַ�����
    bool finish = false;
    int state = 0;//��̬��stateΪ0�ͱ�ʾ���ڳ�̬

    for (int i = 0; i <= final_buffer.count; i++)
    {
        switch (state)
        {
            //��̬
            case 0:
                switch (toState(final_buffer.buffer[i]))
                {
                    case 1://��ĸ
                        dfa_token[count++] = final_buffer.buffer[i];
                        state = 1;
                        break;
                    case 2://����
                        dfa_token[count++] = final_buffer.buffer[i];
                        state = 2;
                        break;
                    case 3://$,_
                        dfa_token[count++] = final_buffer.buffer[i];
                        state = 3;
                        break;
                    case 4://ת���ֻ�����ַ����ڲ�ʹ�ã�������Ϊһ���ַ���������
                        dfa_token[count++] = final_buffer.buffer[i];
                        state = 4;
                        break;
                    case 5:
                        dfa_token[count++] = final_buffer.buffer[i];
                        state = 5;
                        break;
                    case 6:
                        dfa_token[count++] = final_buffer.buffer[i];
                        state = 6;
                        break;
                    case 7:
                        dfa_token[count++] = final_buffer.buffer[i];
                        state = 7;
                        break;
                    case 8:
                        dfa_token[count++] = final_buffer.buffer[i];
                        state = 8;
                        break;
                    case 10:
                        dfa_token[count++] = final_buffer.buffer[i];
                        state = 10;
                        break;
                    default:
                        dfa_token[count++] = final_buffer.buffer[i];
                        break;
                }
                break;
            case 1:
                switch (toState(final_buffer.buffer[i]))
                {
                    case 1:case 2:case 3:
                        dfa_token[count++] = final_buffer.buffer[i];
                        break;
                    default:
                        dfa_token[count] = '\0';
                        i--;
                        finish = 1;
                        state = 9;//����״̬
                }
                break;
            case 2:
                switch (toState(final_buffer.buffer[i]))
                {
                    case 1:
                    case 2:
                        dfa_token[count++] = final_buffer.buffer[i];
                        break;
                    case 7:
                        if (final_buffer.buffer[i] == '.')
                        {
                            dfa_token[count++] = final_buffer.buffer[i];
                            break;
                        }
                        else
                        {
                            dfa_token[count] = '\0';
                            i--;
                            finish = 1;
                            state = 9;//����״̬
                        }
                        break;
                    case 8:
                        //������+-��ǰ����Ee
                        if ((final_buffer.buffer[i] == '+' || final_buffer.buffer[i] == '-') && (final_buffer.buffer[i - 1] == 'e' || final_buffer.buffer[i - 1] == 'E'))
                        {
                            dfa_token[count++] = final_buffer.buffer[i];
                            break;
                        }
                        else
                        {
                            dfa_token[count] = '\0';
                            i--;
                            finish = 1;
                            state = 9;//����״̬
                            break;
                        }
                    default:
                        dfa_token[count] = '\0';
                        i--;
                        finish = 1;
                        state = 9;//����״̬
                        break;
                }
                break;
            case 3://����$_����ĸ��һ���Ĳ���
                switch (toState(final_buffer.buffer[i]))
                {
                    case 1:case 2:case 3:
                        dfa_token[count++] = final_buffer.buffer[i];
                        break;
                    default:
                        dfa_token[count] = '\0';
                        i--;
                        finish = 1;
                        state = 9;//����״̬
                        break;
                }
                break;
            case 4:
                //�ַ�����ת����������5̬�ڲ��������ﴦ������'\'
                dfa_token[count] = '\0';
                i--;
                finish = 1;
                state = 9;//����״̬
                break;
            case 5:
                dfa_token[count++] = final_buffer.buffer[i];
                if (final_buffer.buffer[i] == '"')
                {
                    //��ʱһ�����ǳ�̬�����Բ���Ҫ�ж�i��1�Ĺ�ϵ
                    if (final_buffer.buffer[i - 1] == '\\')
                    {
                    }
                    else
                    {
                        dfa_token[count] = '\0';
                        finish = 1;
                        state = 9;
                    }
                }
                break;
            case 6:
                dfa_token[count++] = final_buffer.buffer[i];
                if (final_buffer.buffer[i] == '\'')
                {
                    //����һ�������'\''�����ǵ��ж�
                    if (final_buffer.buffer[i - 1] == '\\')
                    {
                    }
                    else
                    {
                        dfa_token[count] = '\0';
                        finish = 1;
                        state = 9;
                    }
                }
                break;
            case 7:
                //Ҫ�������ַ���ֱ�ӽ���
                dfa_token[count] = '\0';
                i--;
                finish = 1;
                state = 9;
                break;
            case 8:
                switch (toState(final_buffer.buffer[i]))
                {
                    case 8:case 11:
                        dfa_token[count++] = final_buffer.buffer[i];
                        break;
                    default:
                        dfa_token[count] = '\0';
                        i--;
                        finish = 1;
                        state = 9;
                        break;
                }
                break;
            case 9://����̬
                //��ʱdfa_token�Ѿ��õ������������\0��β����״̬���ɳ�ʼ״̬
                state = 0;
                count = 0;
                finish = 0;
                i--;
                //cout << dfa_token<<endl;
                if (!tokenJudge(dfa_token, line_count)) {
                    return false;
                }
                break;
            case 10://�ո����
                switch (toState(final_buffer.buffer[i]))
                {
                    case 10:
                        dfa_token[count++] = final_buffer.buffer[i];
                        break;
                    default:
                        dfa_token[count] = '\0';
                        i--;
                        finish = 1;
                        state = 9;
                        break;
                }
                break;

            default:
                break;
        }
        //���ʣ��
        if (final_buffer.buffer[i + 1] == '\0')
        {
            dfa_token[count] = '\0';
            if (!tokenJudge(dfa_token, line_count)) {
                return false;
            }
            break;
        }
    }
    return true;
}


/**
 * @brief�� ��Ԫ�����ж�
 * @param�� int line_count  ��ǰ����
 * @return  �Ƿ����޷�ʶ����ַ�
 * @note:   �жϴ��Զ��������token��������ļ�������<����,ԭֵ,����>��ͬʱ���lexer_res�ĸ�ֵ
 * */
bool lexer::tokenJudge(char* str, int line_count)
{
    int kind = 0;
    if (isKey_Word(str) == 1) //�ж��Ƿ�Ϊ�ؼ���
    {
        kind = my_Key_Word;
        fprintf(ftoken, "�ؼ���       %-13s%d\n", str, line_count);
    }
    else if (isSignWord(str) == 1)  //�ж��Ƿ�Ϊ��ʶ��
    {
        kind = my_SignWord;
        fprintf(ftoken, "��ʶ��       %-13s%d\n", str, line_count);
    }
    else if (isInt(str) == 1)//�ж��Ƿ�Ϊ����
    {
        kind = my_Integer;
        fprintf(ftoken, "����         %-13s%d\n", str, line_count);
    }
    else if (isFloat(str) == 1)//�ж��Ƿ�Ϊ������
    {
        kind = my_FloatPoint;
        fprintf(ftoken, "������       %-13s%d\n", str, line_count);
    }
    else if (isMonocularOperator(str) == 1)//�ж��Ƿ�Ϊ��Ŀ�����
    {
        kind = my_MonocularOperator;
        fprintf(ftoken, "��Ŀ�����   %-13s%d\n", str, line_count);
    }
    else if (isBinocularOperator(str) == 1)//�ж��Ƿ�Ϊ˫Ŀ�����
    {
        kind = my_BinocularOperator;
        fprintf(ftoken, "˫Ŀ�����   %-13s%d\n", str, line_count);
    }
    else if (isDelimiter(str) == 1)//�ж��Ƿ�Ϊ���
    {
        kind = my_Delimiter;
        fprintf(ftoken, "���         %-13s%d\n", str, line_count);
    }
    else if (isBlank(str) == 1)//�ж��Ƿ��ǿո�
    {
        kind = my_Blank;
        fprintf(ftoken, "�ո�         %-13s%d\n", str, line_count);
    }
    else if (isSeparator(str[0]) == 1 && strlen(str) == 1)//�ָ���
    {
        kind = my_Separator;
        fprintf(ftoken, "�ָ���       %-13s%d\n", str, line_count);
    }
    else if (isBracketsLeft(str[0]) == 1 && strlen(str) == 1) {
        kind = my_BracketsLeft;
        fprintf(ftoken, "������       %-13s%d\n", str, line_count);
    }
    else if (isBracketsRight(str[0]) == 1 && strlen(str) == 1) {
        kind = my_BracketsRight;
        fprintf(ftoken, "������       %-13s%d\n", str, line_count);
    }
    else if (isBracketsLeftBig(str[0]) == 1 && strlen(str) == 1) {
        kind = my_BracketsLeftBig;
        fprintf(ftoken, "�������     %-13s%d\n", str, line_count);
    }
    else if (isBracketsRightBig(str[0]) == 1 && strlen(str) == 1) {
        kind = my_BracketsRightBig;
        fprintf(ftoken, "�������     %-13s%d\n", str, line_count);
    }
    else if (isEnd(str[0]) == 1 && strlen(str) == 1) {
        kind = my_End;
        fprintf(ftoken, "������       %-13s%d\n", str, line_count);
    }
    else if (isBracketsLeftSquare(str[0]) == 1 && strlen(str) == 1) {
        kind = my_Brackets_Left_Square;
        fprintf(ftoken, "������     %-13s%d\n", str, line_count);
    }
    else if (isBracketsRightSquare(str[0]) == 1 && strlen(str) == 1) {
        kind = my_Brackets_Right_Square;
        fprintf(ftoken, "�ҷ�����     %-13s%d\n", str, line_count);
    }
    else if (isPointArrow(str) == 1) {
        kind = my_Point_Arrow;
        fprintf(ftoken, "��ͷ         %-13s%d\n", str, line_count);
    }
    else if (isRegion(str) == 1) {
        kind = my_Region;
        fprintf(ftoken, "���         %-13s%d\n", str, line_count);
    }
    else if (isRegionXigou(str) == 1) {
        kind = my_Region_Xigou;
        fprintf(ftoken, "������       %-13s%d\n", str, line_count);
    }
    else if (strlen(str) == 1 && isColon(str[0]) == 1) {
        kind = my_Colon;
        fprintf(ftoken, "ð��         %-13s%d\n", str, line_count);
    }
    else if (isPoint(str[0]) == 1 && strlen(str) == 1) {
        kind = my_Point;
        fprintf(ftoken, "���         %-13s%d\n", str, line_count);
    }
    else if (isStr(str)) {
        kind = my_Str;
        fprintf(ftoken, "�ַ���       %-13s%d\n", str, line_count);
    }
    else if (isChar(str)) {
        kind = my_Char;
        fprintf(ftoken, "�ַ�         %-13s%d\n", str, line_count);
    }
    else if (strlen(str) == 1 && isQuestion_Mark(str[0]) == 1) {
           kind = my_Question_Mark;
           fprintf(ftoken, "�ʺ�         %-13s%d\n", str, line_count);
    }
    else {
        kind = my_WrongWord;
        fprintf(ftoken, "����         %-13s%d\n", str, line_count);
        fprintf(ferror, "�޷�ʶ��%s---------Դ�����%d��\n", str, line_count);
    }
    if (kind != my_Blank)//�͵��﷨�������е��м��������ո�
    {
        elem t = elemGenerate(kind, str, line_count);
        lexer_res.push_back(t);
    }

    return (kind != my_WrongWord);
}


/**
 * @brief�� ���ɴ�Ԫ��Ϣ�����浽������
 * @param�� int line_count  ��ǰ����
 *          char* str �ַ���
 *          int  kind Ҫ����Ĵ�Ԫ���� 
 * @return  ���ɵĴ�Ԫ
 * */
elem lexer::elemGenerate(int kind, char* str,int line_count) {
    //�ж��Ƿ�������
    if (kind == my_Integer)
    {
        elem a("<INT>", str, line_count);
        return a;
    }
    else if (kind == my_FloatPoint)
    {
        elem a("<FLOAT>", str,line_count);
        return a;
    }
    else if (kind == my_Str)
    {
        elem a("<STRING>", str,line_count);
        return a;
    }
    else if (kind == my_Char)
    {
        elem a("<Char>", str,line_count);
        return a;
    }
    //�ж��Ƿ��Ǳ�ʶ��
    else if (kind == my_SignWord)
    {
        elem a("<ID>", str,line_count);
        return a;
    }
    //�ж��Ƿ����
    else if (kind == my_WrongWord)
    {
        elem a("<WRONG>", str, line_count);
        return a;
    }
    //���⴦��main
    else if (kind == my_Key_Word && !strcmp(str, "main"))
    {
        elem a("<ID>", str,line_count);
        return a;
    }
    //������ַ���
    else
    {
        elem a(str, str,line_count);
        return a;
    }
}


