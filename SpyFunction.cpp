#include "SpyFunction.h"

bool getCanInfo(std::string name, long &id, int &sBit, int &eBit, std::string &formula, std::unordered_map<std::string, FunInfo> &m_FunList)
{

    auto search = m_FunList.find(name);
    if (search != m_FunList.end())
    {
        FunInfo funinfo = search->second;

        id = funinfo.funID;
        sBit = funinfo.sBit;
        eBit = funinfo.eBit;
        formula = funinfo.formula;
        return true;
    }
    else
    {
        return false;
    }
}

void loadCanInfo(std::unordered_map<std::string, std::list<FunInfo>> &m_FunList)
{

    std::string lineString;
    std::fstream fileStream;
    FunInfo fun_ex;

    fileStream.open("functionList.txt");

    if (!fileStream.is_open())
    {
        printf("file open fail");
    }
    std::string pattern = "\t";
    while (std::getline(fileStream, lineString))
    {

        std::vector<std::string> ret = split(lineString, pattern);

        fun_ex.funID = std::stol(ret.at(1), nullptr, 0);
        fun_ex.sBit = std::stoi(ret.at(2));
        fun_ex.eBit = std::stoi(ret.at(3));
        fun_ex.formula = ret.at(4);
        fun_ex.sendMode = std::stoi(ret.at(5)); //发送模式
        fun_ex.Hz = std::stoi(ret.at(6));       //发送频率
        fun_ex.IDNetWork=std::stoi(ret.at(7));
        fun_ex.ExternFlag = std::stoi(ret.at(8)); //扩展帧
//        fun_ex.RemoteFlag = std::stoi(ret.at(8)); //远程帧

        auto l = m_FunList.find(ret.at(0));
        if (l != m_FunList.end())
        {
            l->second.push_back(fun_ex);
        }
        else
        {
            std::list<FunInfo> s;
            s.push_back(fun_ex);
            m_FunList.emplace(ret.at(0), s);
        }
    }
    fileStream.close();
}

std::vector<std::string> split(std::string str, std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str += pattern; //扩展字符串以方便操作
    int size = str.size();
    for (int i = 0; i < size; i++)
    {
        pos = str.find(pattern, i);
        if (pos < size)
        {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}
