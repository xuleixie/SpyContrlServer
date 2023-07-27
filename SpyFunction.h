#pragma once
#include "Calculator.h"
#include "icsneo40DLLAPI.h"
#include <fstream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

#include "SpyManage.h"

struct FunInfo
{
    long funID;
    int sBit;
    int eBit;
    std::string formula;
    int sendMode;
    int Hz;
    unsigned int IDNetWork;
    BYTE ExternFlag; //ID格式
//    BYTE RemoteFlag; //data格式
};

bool getCanInfo(std::string name, long &id,
                int &sBit, int &eBit, std::string &formula);
void loadCanInfo(std::unordered_map<std::string, std::list<FunInfo>> &m_FunList);
std::vector<std::string> split(std::string str, std::string pattern);
