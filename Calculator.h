#pragma once
#include <stack>
#include <vector>
#include <string>

class Calculator
{
public:
    Calculator();
    static double cal(const std::string &formula);   //计算方法

private:
    void format(const std::string &formula);        //表达式自定义标准格式化
    void postfix();						            //后缀表达式转换
    void calResult();					            //计算后缀表达式
    int getPrior(char c);				            //获取算术符号优先级

    std::vector<std::string> m_postfix;	            //后缀表达式向量
    std::stack<char> m_symStack;		            //符号栈
    std::stack<double> m_figStack;		            //数字栈
    std::string m_stdInfix;				            //自定义标准格式化表达式
    double m_result;					            //最终计算结果
};
