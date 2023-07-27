#include <stdio.h>
#include <winsock2.h>

#include <string>
#include <unordered_map>
#include <vector>
#include <QDebug>
#include "SpyFunction.h"
#include "SpyManage.h"

#pragma comment(lib, "ws2_32.lib") //加载 ws2_32.dll

#define BUF_SIZE 100

int main()
{
    SpyManage spy;
    auto list = spy.getDeviceList();
    spy.connectDevice(0);

    std::unordered_map<std::string, std::list<FunInfo>> m_FunList;
    loadCanInfo(m_FunList);
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //创建套接字
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

    //绑定套接字
    sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));       //每个字节都用0填充
    servAddr.sin_family = PF_INET;                //使用IPv4地址
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); //自动获取IP地址
    servAddr.sin_port = htons(1234);              //端口
    bind(sock, (SOCKADDR *)&servAddr, sizeof(SOCKADDR));

    //接收客户端请求
    SOCKADDR clntAddr; //客户端地址信息
    int nSize = sizeof(SOCKADDR);
    char buffer[BUF_SIZE]; //缓冲区

    while (1)
    {
        int strLen = recvfrom(sock, buffer, BUF_SIZE, 0, &clntAddr, &nSize);

        puts(buffer);

        std::string recData(buffer, strLen);
        std::string pattern = "\t";
        std::vector<std::string> res = split(recData, pattern);

        auto search = m_FunList.find(res[0]);
        if (search != m_FunList.end() && res.size() == 2)
        {
            printf("find one");
            std::list<FunInfo> funinfo = search->second;
            spy.setMessageInfo(funinfo.front().IDNetWork, funinfo.front().ExternFlag);
            for (auto s : funinfo)
            {
                unsigned int data = 0;
                if (!s.formula.empty())
                {
                    std::string::size_type pos = s.formula.find("X");

                    if (pos != s.formula.npos)
                        s.formula.replace(pos, 1, res[1]);

                    data = static_cast<unsigned int>(Calculator::cal(s.formula));
                }
                else
                {
                    data = std::stoi(res[1]);
                }

                if (!spy.existMessage(s.funID))
                    spy.createMessage(s.funID, 0, s.sendMode, s.Hz, data, s.sBit, s.eBit);
                else
                    spy.setMessageRate(s.funID, data, s.sBit, s.eBit);

                //                spy.setMessageData(s.funID, data, s.sBit, s.eBit);

                sendto(sock, buffer, strLen, 0, &clntAddr, nSize);
            }
        }
        else
        {
            char res[30] = "lzd:don't find function";
            sendto(sock, res, 30, 0, &clntAddr, nSize);
            // sendto(sock, buffer, strLen, 0, &clntAddr, nSize);
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
