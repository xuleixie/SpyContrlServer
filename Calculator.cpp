#include "Calculator.h"
#include <stack>
#include <vector>
#include <string>
#include <cmath>

//绝对值符号个数的奇偶性
enum ABS_ODEVITY
{
    ABS_ODD = 1,
    ABS_EVEN = 2,
};

//算术符号优先权等级
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
    //operatorSym = "支持运算符：+, - , *, /, %（百分号）, ^（幂次方）, | |（绝对值），！（阶乘）。其他符号：( ) [ ] { }";
    m_result = 0.0;
}

//计算方法
double Calculator::cal(const std::string &formula)
{
    Calculator cal;
    cal.format(formula);		//表达式自定义标准格式化
    cal.postfix();				//后缀表达式转换
    cal.calResult();			//获取算术结果
    return cal.m_result;
}


//表达式自定义标准格式化
void Calculator::format(const std::string &formula)
{
    m_stdInfix = formula;

    //实现正负数
    for (size_t i = 0; i < m_stdInfix.size(); i++)
    {
        if (m_stdInfix[i] == '-' || m_stdInfix[i] == '+')
        {
            //-x转换为0-x，+x转化为0+x
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

//后缀表达式转换
void Calculator::postfix()
{
    int absNumeber = ABS_ODD;				//绝对值符号个数的奇偶性
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
                        m_postfix.push_back(tmp);						//左绝对值符号'|'加入后缀表达式，用于绝对值的检测计算
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
                    m_symStack.pop();							//将左括号出栈丢弃
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
                    m_symStack.pop();							//将左括号出栈丢弃
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
                    m_symStack.pop();							//将左括号出栈丢弃
                }
                break;
            default:
                if ((m_stdInfix[i] >= '0' && m_stdInfix[i] <= '9'))
                {
                    tmp += m_stdInfix[i];
                    while (i + 1 < m_stdInfix.length() && (m_stdInfix[i + 1] >= '0' && m_stdInfix[i + 1] <= '9' || m_stdInfix[i + 1] == '.'))
                    {		//小数处理

                        tmp += m_stdInfix[i + 1];		//是连续的数字，则追加
                        i++;
                    }
                    if (tmp[tmp.length() - 1] == '.')
                    {
                        tmp += '0';						//将x.做x.0处理
                    }
                    m_postfix.push_back(tmp);
                }
                break;
        }//end switch
    }//end for

     //if(!m_symStack.empty()) {
    while (!m_symStack.empty())
    {
        //将栈中剩余符号加入后缀表达式
        tmp = "";
        tmp += m_symStack.top();
        m_postfix.push_back(tmp);
        m_symStack.pop();
    }
}

//计算后缀表达式
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
                ///除数不为0，未做处理，默认
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
            m_figStack.push(fmod(op1, op2));			//可进行小数求余
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
                //阶乘数应大于；为小数时(转化为整数求阶)
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

//获取算术符号优先级
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
    //else { cout << c << 非法符号！ << endl; }
}
