#include "Calculator.h"
#include <stack>
#include <vector>
#include <string>
#include <cmath>

//����ֵ���Ÿ�������ż��
enum ABS_ODEVITY
{
    ABS_ODD = 1,
    ABS_EVEN = 2,
};

//������������Ȩ�ȼ�
enum PRIO_LV
{
    PRIO_LV0 = 0,
    PRIO_LV1 = 1,
    PRIO_LV2 = 2,
    PRIO_LV3 = 3,
    PRIO_LV4 = 4,
};

Calculator::Calculator()
{
    //operatorSym = "֧���������+, - , *, /, %���ٷֺţ�, ^���ݴη���, | |������ֵ���������׳ˣ����������ţ�( ) [ ] { }";
    m_result = 0.0;
}

//���㷽��
double Calculator::cal(const std::string &formula)
{
    Calculator cal;
    cal.format(formula);		//���ʽ�Զ����׼��ʽ��
    cal.postfix();				//��׺���ʽת��
    cal.calResult();			//��ȡ�������
    return cal.m_result;
}


//���ʽ�Զ����׼��ʽ��
void Calculator::format(const std::string &formula)
{
    m_stdInfix = formula;

    //ʵ��������
    for (size_t i = 0; i < m_stdInfix.size(); i++)
    {
        if (m_stdInfix[i] == '-' || m_stdInfix[i] == '+')
        {
            //-xת��Ϊ0-x��+xת��Ϊ0+x
            if (i == 0)
            {
                m_stdInfix.insert(0, 1, '0');
            }
            else if (m_stdInfix[i - 1] == '(')
            {
                m_stdInfix.insert(i, 1, '0');
            }
        }
    }
}

//��׺���ʽת��
void Calculator::postfix()
{
    int absNumeber = ABS_ODD;				//����ֵ���Ÿ�������ż��
    std::string tmp;

    for (size_t i = 0; i < m_stdInfix.size(); i++)
    {
        tmp = "";
        switch (m_stdInfix[i])
        {
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            case '^':
            case '!':
                if (m_symStack.empty() || m_symStack.top() == '(' || m_symStack.top() == '[' || m_symStack.top() == '{' || (m_symStack.top() == '|' && absNumeber == ABS_ODD))
                {
                    m_symStack.push(m_stdInfix[i]);
                }
                else
                {
                    while (!m_symStack.empty() && (getPrior(m_symStack.top()) >= getPrior(m_stdInfix[i])))
                    {
                        tmp += m_symStack.top();
                        m_postfix.push_back(tmp);
                        m_symStack.pop();
                        tmp = "";
                    }
                    m_symStack.push(m_stdInfix[i]);
                }
                break;
            case '|':

                if (absNumeber == ABS_ODD)
                {
                    m_symStack.push(m_stdInfix[i]);
                    absNumeber = ABS_EVEN;
                }
                else
                {
                    while (!m_symStack.empty() && m_symStack.top() != '|')
                    {
                        tmp += m_symStack.top();
                        m_postfix.push_back(tmp);
                        m_symStack.pop();
                        tmp = "";
                    }
                    if (!m_symStack.empty() && m_symStack.top() == '|')
                    {
                        tmp += m_symStack.top();
                        m_postfix.push_back(tmp);						//�����ֵ����'|'�����׺���ʽ�����ھ���ֵ�ļ�����
                        m_symStack.pop();
                        absNumeber = ABS_ODD;
                    }
                }
                break;
            case '(':
            case '[':
            case '{':
                m_symStack.push(m_stdInfix[i]);
                break;
            case ')':
                while (!m_symStack.empty() && m_symStack.top() != '(')
                {
                    tmp += m_symStack.top();
                    m_postfix.push_back(tmp);
                    m_symStack.pop();
                    tmp = "";
                }
                if (!m_symStack.empty() && m_symStack.top() == '(')
                {
                    m_symStack.pop();							//�������ų�ջ����
                }
                break;
            case ']':
                while (!m_symStack.empty() && m_symStack.top() != '[')
                {
                    tmp += m_symStack.top();
                    m_postfix.push_back(tmp);
                    m_symStack.pop();
                    tmp = "";
                }
                if (!m_symStack.empty() && m_symStack.top() == '[')
                {
                    m_symStack.pop();							//�������ų�ջ����
                }
                break;
            case '}':
                while (!m_symStack.empty() && m_symStack.top() != '{')
                {
                    tmp += m_symStack.top();
                    m_postfix.push_back(tmp);
                    m_symStack.pop();
                    tmp = "";
                }
                if (!m_symStack.empty() && m_symStack.top() == '{')
                {
                    m_symStack.pop();							//�������ų�ջ����
                }
                break;
            default:
                if ((m_stdInfix[i] >= '0' && m_stdInfix[i] <= '9'))
                {
                    tmp += m_stdInfix[i];
                    while (i + 1 < m_stdInfix.length() && (m_stdInfix[i + 1] >= '0' && m_stdInfix[i + 1] <= '9' || m_stdInfix[i + 1] == '.'))
                    {		//С������

                        tmp += m_stdInfix[i + 1];		//�����������֣���׷��
                        i++;
                    }
                    if (tmp[tmp.length() - 1] == '.')
                    {
                        tmp += '0';						//��x.��x.0����
                    }
                    m_postfix.push_back(tmp);
                }
                break;
        }//end switch
    }//end for

     //if(!m_symStack.empty()) {
    while (!m_symStack.empty())
    {
        //��ջ��ʣ����ż����׺���ʽ
        tmp = "";
        tmp += m_symStack.top();
        m_postfix.push_back(tmp);
        m_symStack.pop();
    }
}

//�����׺���ʽ
void Calculator::calResult()
{
    std::string tmp;
    double number = 0;
    double op1 = 0, op2 = 0;

    for (int i = 0; i < m_postfix.size(); i++)
    {
        tmp = m_postfix[i];
        if (tmp[0] >= '0' && tmp[0] <= '9')
        {
            number = atof(tmp.c_str());
            m_figStack.push(number);
        }
        else if (m_postfix[i] == "+")
        {
            if (!m_figStack.empty())
            {
                op2 = m_figStack.top();
                m_figStack.pop();
            }
            if (!m_figStack.empty())
            {
                op1 = m_figStack.top();
                m_figStack.pop();
            }
            m_figStack.push(op1 + op2);
        }
        else if (m_postfix[i] == "-")
        {
            if (!m_figStack.empty())
            {
                op2 = m_figStack.top();
                m_figStack.pop();
            }
            if (!m_figStack.empty())
            {
                op1 = m_figStack.top();
                m_figStack.pop();
            }
            m_figStack.push(op1 - op2);
        }
        else if (m_postfix[i] == "*")
        {
            if (!m_figStack.empty())
            {
                op2 = m_figStack.top();
                m_figStack.pop();
            }
            if (!m_figStack.empty())
            {
                op1 = m_figStack.top();
                m_figStack.pop();
            }
            m_figStack.push(op1 * op2);
        }
        else if (m_postfix[i] == "/")
        {
            if (!m_figStack.empty())
            {
                op2 = m_figStack.top();
                m_figStack.pop();
            }
            if (!m_figStack.empty())
            {
                op1 = m_figStack.top();
                m_figStack.pop();
            }
            if (op2 != 0)
            {
                ///������Ϊ0��δ������Ĭ��
            }
            m_figStack.push(op1 / op2);
        }
        else if (m_postfix[i] == "%")
        {
            if (!m_figStack.empty())
            {
                op2 = m_figStack.top();
                m_figStack.pop();
            }
            if (!m_figStack.empty())
            {
                op1 = m_figStack.top();
                m_figStack.pop();
            }
            m_figStack.push(fmod(op1, op2));			//�ɽ���С������
        }
        else if (m_postfix[i] == "^")
        {
            if (!m_figStack.empty())
            {
                op2 = m_figStack.top();
                m_figStack.pop();
            }
            if (!m_figStack.empty())
            {
                op1 = m_figStack.top();
                m_figStack.pop();
            }
            m_figStack.push(pow(op1, op2));
        }
        else if (m_postfix[i] == "|")
        {
            if (!m_figStack.empty())
            {
                op1 = m_figStack.top();
                m_figStack.pop();
            }
            m_figStack.push(abs(op1));
        }
        else if (m_postfix[i] == "!")
        {
            if (!m_figStack.empty())
            {
                op1 = m_figStack.top();
                m_figStack.pop();
            }
            if (op1 > 0)
            {
                //�׳���Ӧ���ڣ�ΪС��ʱ(ת��Ϊ�������)
                double factorial = 1;
                for (int i = 1; i <= op1; ++i)
                {
                    factorial *= i;
                }
                op1 = factorial;
            }
            m_figStack.push(op1);
        }
    }//end for
    if (!m_figStack.empty())
    {
        m_result = m_figStack.top();
    }
}

//��ȡ�����������ȼ�
int Calculator::getPrior(char c)
{
    if (c == '+' || c == '-')
    {
        return PRIO_LV1;
    }
    else if (c == '*' || c == '/')
    {
        return PRIO_LV2;
    }
    else if (c == '%' || c == '^')
    {
        return PRIO_LV3;
    }
    else if (c == '!')
    {
        return PRIO_LV4;
    }
    else
    {
        return PRIO_LV0;
    }
    //else { cout << c << �Ƿ����ţ� << endl; }
}
