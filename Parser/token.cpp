#include"token.h"


token::~token()
{
}

/**
* @func:   �ж��ַ�����
* @para:   char c     ������ַ�
* @return: ������ַ�����
*/
int token::toState(char c) {
    if (isalpha(c))
        return 1;
    else if (isdigit(c))
        return 2;
    else if (c == '$' || c == '_')
        return 3;
    else if (c == '\\')//���ת������
        return 4;
    else if (c == '"')
        return 5;
    else if (c == '\'')
        return 6;
    //Ҫ�������ַ�
    else if (c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ';' || c == ',' || c == '.')//������Ҫע��.���ţ����������ֺ������Ҫ���Ǹ������������������
        return 7;
    else if (c == ' ')
        return 10;
    return 8;
}

/**
* @func:   �ж��ַ��ǲ��ǽ��
* @para:   char c     ������ַ�
* @return: �Ƿ�Ϊ���
*/
int token::isDelimiter(char c) {

    if (Delimiter == c)
        return 1;
    return 0;
}


int token::isDelimiter(string c) {
    if (c.length() == 1)
    {
        if (Delimiter == c[0])
            return 1;
    }
    return 0;
}


/**
* @func:   �ж��ַ��ǲ��ǽ��
* @para:   char c     ������ַ�
* @return: �Ƿ�Ϊ���
* @note:   �����жϣ���Ϊ�п������˫Ŀ������������߿ո񲻿�ɾ����������f = 5���ֿո�Ҳ����ɾ����
*          ��Ҫ���=��������= =����
*/
bool token::spaceRemovable(char c)
{
    if (c == '=' || c == '+' || c == '-' || c == '*' || c == '/' || c == '>' || c == '<' || c == '!')
        return false;
    if ((c > 'z' || (c < 'a' && c > 'Z') || (c < 'A' && c > '9') || (c < '0')) && c != '_' && c != '$')
        return true;
    return false;
}


/**
* @func:   �ж��ַ����ǲ��ǿ��Ա�ʾ����
* @para:   char *str     ������ַ���
* @return:  ����ֵΪ0 ��ʾ ������ַ�������һ����Ч��������
*           ����ֵΪ1 ��ʾ ��һ����Ч��������
*           ����ֵΪ2 ��ʾ ������һ����Ч��������������һЩ��������
*/

int token::isInt(string str) {
    const int OCT = 8;
    const int DEC = 10;
    const int HEX = 16;
    int intType = DEC;
    int len = 0;
    string t;
    if (str[0] == '+' || str[0] == '-')
        t = str.substr(1);
    else
        t = str;
    len = t.length();
    // �ж����Ľ���
    if (isdigit(t[0])) {
        if (t[0] == '0' && t[1] != '\0') {  //�ж��ǰ˽��ƻ���ʮ�����ƣ�����λ��0�����ֲ�ֹһλʱ������һλ��
            if (isdigit(t[1]))
                intType = OCT;
            else if (t[1] == 'x' || t[1] == 'X')
                intType = HEX;
            else
                return 0;
        }
        else
            intType = DEC;
    }
    else
        return 0;
    //������ĸ�ʽ�Ƿ���ȷ
    switch (intType)
    {
        case OCT:
            for (int i = 1; i < len; i++) {//�˽�������λΪ0����������Ϊ2.
                if (isdigit(t[i])) {
                    if (t[i] >= '0' && t[i] <= '7')
                        ;
                    else
                        return 2;
                }
                else
                    return 0;
            }
            return 1;
            break;
        case DEC:
            for (int i = 0; i < len; i++) {
                if (isdigit(t[i]))
                    ;
                else
                    return 0;
            }
            return 1;
            break;
        case HEX:
            for (int i = 2; i < len; i++) {  // ʮ��������ǰ��λΪ0x\0X����������Ϊ3.
                if (isdigit(t[i]) || (t[i] >= 'a' && t[i] <= 'f') || (t[i] >= 'A' && t[i] <= 'F'))
                    ;
                else {
                    if (!isalpha(t[i]))//ʮ���������г��ַ���ĸ
                        return 0;
                    else
                        return 2;
                }
            }
            return 1;
            break;
    }
    return 0;
}




//�ж�xx.xx���������С������Ϊ��С���������޷�����������
int token::hasDot(string str)
{
    int len = 0;
    int dotAppearNum = 0;
    string t;
    if (str[0] == '+' || str[0] == '-')
        t=str.substr(1);
    else
        t=str;
    len = t.length();
    for (int i = 0; i < len; i++)
    {
        if (len > 0)//ȷ������λ�����һλ
        {
            if (i == 0)//�ж���λ
            {
                if (!isdigit(t[i]))//��������
                {
                    if (t[i] == '.')
                        return 2;
                    else
                        return 0;
                }
            }
            if (i < len - 1)//�жϷ����һλ�Ҳ�����λ
            {
                if (!isdigit(t[i]) && t[i] != '.')//�Ȳ�������Ҳ����С����
                    return 0;
                if (t[i] == '.')//�����С����
                {
                    dotAppearNum++;
                    if (dotAppearNum > 1)//���ֶ��С��������
                        return 2;
                }
            }
            if (i == len - 1)//�ж�ĩλ
            {
                //�Ȳ�������Ҳ����f\F��d\D
                if (!isdigit(t[i]) && t[i] != 'f' && t[i] != 'F' && t[i] != 'd' && t[i] != 'D')
                    return 0;
            }
        }
    }
    if (dotAppearNum < 1)
        return 0;
    return 1;
}



/**
* @func:   �ж��ַ����ǲ��Ǳ�ʾ������
* @para:   char *str     ������ַ���
* @return: ����ֵΪ0 ��ζ�� ѹ������
*          ����ֵΪ1 ��ζ�� �� ����xx.xx��xx e\E xx�������
*          ����ֵΪ2 ��ζ�� �ǵ�float�������� �������� 0.0.1
*/
int token::isFloat(string str) {
    int len = 0;
    int dotAppearNum = 0;
    len = str.length();
    string t;
    //�ж�78e\E56�����
    int pos = -1;
    for (int i = 0; i < len; i++)
    {
        if (str[i] == 'e' || str[i] == 'E')
        {
            pos = i;
            break;
        }
    }
    if (pos != -1)//������e/E
    {
        int j = 0;
        if (pos == 0 || pos == len - 1)//e����λ��ĩβ
            return 0;
        if (str[0] == '+' || str[0] == '-')
        {
            if (str[1] == 'e' || str[1] == 'E')//��λ�Ƿ���λʱ��֤e���ڷ���λ֮��
                return 0;
        }
        t=str.substr(j, pos + 1);//���һλ����β��
        t[pos] = '\0';
        if (hasDot(t) != 1 && isInt(t) != 1)
            return 0;
        j = pos + 1;//���ֵ���ʼλ        
        t=str.substr(j, len - pos);//���һλ��β��
        if (hasDot(t) != 1 && isInt(t) != 1)
            return 0;
        return 1;
    }
    return hasDot(str);
}


/**
* @func:   �ж��ַ����ǲ��ǿ��Ա�ʾ��ʶ��
* @para:   char *str     ������ַ���
* @return:  ����ֵΪ0 ��ζ�� ѹ������
*           ����ֵΪ1 ��ζ�� ��
*           ����ֵΪ2 ��ζ�� �ǵ���ʶ����������
*/

int token::isSignWord(string str) {

    if (str[0] == '_' || isalpha(str[0]) || str[0] == '$')
    {
        for (int i = 0; str[i] != '\0'; i++)
        {
            if (str[i] != '_' && str[i] != '$' && !isdigit(str[i]) && !isalpha(str[i]))
                return 2;
        }
        return 1;
    }
    return 0;
}



/**
*@func:   �ж��ַ����ǲ��ǹؼ���
*@para :  char* str     ������ַ���
*@return: 1 �ǣ�0����
*/
int token::isKey_Word(string str) {

    for (int i = 0; i < 50; i++)
    {
        if (str==Key_Word[i])
            return 1;
    }
    return 0;
}

/**
*@func:   �ж��ַ����ǲ���˫Ŀ�����
*@para :  char* str     ������ַ���
*@return: 1 �ǣ�0����
*/
int token::isBinocularOperator(string str) {
    for (int i = 0; i < BINOCULAR_OPERATOR_SIZE; i++)
    {
        if (str==Binocular_Operator[i])
            return 1;
    }
    return 0;
}


/**
*@func:   �ж��ַ����ǲ��ǵ�Ŀ�����
*@para :  char* str     ������ַ���
*@return: 1 �ǣ�0����
*/
int token::isMonocularOperator(string str) {
    if (str.length() == 1) {
        for (int i = 0; i < MONOCULAR_OPERATOR_SIZE; i++)
        {
            if (str[0] == Monocular_Operator[i][0])
                return 1;
        }
    }
    return 0;
}

/**
*@func:   �ж��ַ����ǲ��ǿո�
*@para :  char* str     ������ַ���
*@return: 1 �ǣ�0����
*/
int token::isBlank(string str)
{
    for (int i = 0;i < str.length();  i++)
    {
        if (str[i] != ' ')
            return 0;
    }
    return 1;
}



/**
 * @brief �ж������ַ��Ƿ�Ϊ�ָ���
 * @param: c ������ַ�
 * @return: int 1��ʾ�Ƿָ�����0��ʾ����
 */
int token::isSeparator(char c)
{
    if (Seprater == c)
        return 1;
    return 0;
}

/**
 * @brief: �ж������ַ������Ƿ�Ϊ������
 * @param: c ������ַ�
 * @return: int 1��ʾ�������ţ�0��ʾ����
 */
int token::isBracketsLeft(char c)
{
    if (Brackets_Left == c)
        return 1;
    return 0;
}

/**
 * @brief �ж������ַ������Ƿ�Ϊ������
 * @param c ������ַ�
 * @return int 1��ʾ�������ţ�0��ʾ����
 */
int token::isBracketsRight(char c)
{
    if (Brackets_Right == c)
        return 1;
    return 0;
}

/**
 * @brief �ж������ַ������Ƿ�Ϊ�������
 * @param c ������ַ�
 * @return int 1��ʾ��������ţ�0��ʾ����
 */
int token::isBracketsLeftBig(char c)
{
    if (Brackets_Left_Big == c)
        return 1;
    return 0;
}

/**
 * @brief �ж������ַ������Ƿ�Ϊ�Ҵ�����
 * @param c ������ַ�
 * @return int 1��ʾ���Ҵ����ţ�0��ʾ����
 */
int token::isBracketsRightBig(char c)
{
    if (Brackets_Right_Big == c)
        return 1;
    return 0;
}

/**
 * @brief �ж������ַ������Ƿ�Ϊ������
 * @param c ������ַ�
 * @return int 1��ʾ�ǽ�������0��ʾ����
 */
int token::isEnd(char c)
{
    if (End == c)
        return 1;
    return 0;
}

/**
 * @brief �ж������ִ������Ƿ�Ϊ�ַ��� "xxxxxx"
 * @param str ������ַ���
 * @return int 1��ʾ���ַ�����0��ʾ����
 */
int token::isStr(string str)
{
    //С��״̬��
    int i = 0;
    for (i = 0; i < str.length(); i++)
    {
        if (str[i] == ' ')
            continue;
        else
            break;
    }
    int flag = 0;
    //��ʱiΪ��' '�����ַ�
    if (str[i] == '"')
    {
        i++;
        flag = 1;//��һ��"
    }
    if (flag == 1)
    {
        for (i; i < str.length(); i++)
        {
            if (str[i] == '"' && str[i - 1] != '\\')
                return 1;
        }
    }
    return 0;
}

/**
 * @brief �ж������ִ������Ƿ�Ϊ�ַ� 'x'
 * @param str ������ַ���
 * @return int 1��ʾ���ַ���0��ʾ����
 */
int token::isChar(string str)
{
    //С��״̬��
    int i = 0;
    for (i = 0; i < str.length(); i++)
    {
        if (str[i] == ' ')
            continue;
        else
            break;
    }
    int flag = 0;
    //��ʱiΪ��' '�����ַ�
    if (str[i] == '\'')
    {
        i++;
        flag = 1;//��һ��"
    }
    if (flag == 1)
    {
        //ֻ���� '\x'
        if (str[i] == '\\')
        {
            if (isdigit(str[i]))
            {
                flag = 2;//10����
            }
            else if (str[i] == 'x')
            {
                flag = 3;//16����
            }
            else
            {
                if (str[i + 2] == '\'')
                    return 1;

            }
            if (flag == 2 && isInt(&str[i]) && strlen(&str[i]) == 3)
                return 1;
            if (flag == 3 && isInt(&str[i]) && strlen(&str[i]) == 5)
                return 1;

        }
        else {
            if (str[i + 1] == '\'')
                return 1;
            return 0;
        }

    }
    return 0;
}

/**
 * @brief �ж������ַ������Ƿ�Ϊ�ַ� '.'
 * @param c ������ַ�
 * @return int 1��ʾ���ַ�'.'��0��ʾ����
 */
int token::isPoint(char c)
{
    if (Point == c)
        return 1;
    return 0;
}

/**
 * @brief �ж������ַ������Ƿ�Ϊ�ַ� '['
 * @param c ������ַ�
 * @return int 1��ʾ���ַ�'[',0��ʾ����
 */
int token::isBracketsLeftSquare(char c)
{
    if (Brackets_Left_Square == c)
        return 1;
    return 0;
}

/**
 * @brief �ж������ַ������Ƿ�Ϊ�ַ� ']'
 * @param c ������ַ�
 * @return int 1��ʾ���ַ�']'��0��ʾ����
 */
int token::isBracketsRightSquare(char c)
{
    if (Brackets_Right_Square == c)
        return 1;
    return 0;
}

/**
 * @brief �ж������ִ������Ƿ�Ϊ�ַ� '->'
 * @param str ������ַ���
 * @return int 1��ʾ���ַ�'->'��0��ʾ����
 */
int token::isPointArrow(string str)
{
    if (str==Point_Arrow)
        return 1;
    return 0;
}

/**
 * @brief �ж������ִ������Ƿ�Ϊ�ַ� '::'
 * @param str ������ַ���
 * @return int 1��ʾ���ַ�'::'��0��ʾ����
 */
int token::isRegion(string str)
{
    if (str==Region)
        return 1;
    return 0;
}

/**
 * @brief �ж������ִ������Ƿ�Ϊ�ַ� '::~'
 * @param str ������ַ���
 * @return int 1��ʾ���ַ�'::~'��0��ʾ����
 */
int token::isRegionXigou(string str)
{
    if (str==Region_Xigou)
        return 1;
    return 0;
}

/**
 * @brief �ж������ַ������Ƿ�Ϊ�ַ� ':'
 * @param c ������ַ�
 * @return int 1��ʾ���ַ�':'��0��ʾ����
 */
int token::isColon(char c)
{
    if (c == Colon)
        return 1;
    return 0;
}


int token::isQuestion_Mark(char c) {
    if (c == Question_Mark)
        return 1;
    return 0;
}
