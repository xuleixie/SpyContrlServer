#pragma once
#include <stack>
#include <vector>
#include <string>

class Calculator
{
public:
    Calculator();
    static double cal(const std::string &formula);   //���㷽��

private:
    void format(const std::string &formula);        //���ʽ�Զ����׼��ʽ��
    void postfix();						            //��׺���ʽת��
    void calResult();					            //�����׺���ʽ
    int getPrior(char c);				            //��ȡ�����������ȼ�

    std::vector<std::string> m_postfix;	            //��׺���ʽ����
    std::stack<char> m_symStack;		            //����ջ
    std::stack<double> m_figStack;		            //����ջ
    std::string m_stdInfix;				            //�Զ����׼��ʽ�����ʽ
    double m_result;					            //���ռ�����
};
