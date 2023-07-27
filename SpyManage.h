#pragma once
#include <icsnVC40.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class SpyMessage;
class SpyManagePrivate;

class SpyManage
{
  public:
    SpyManage();
    ~SpyManage();

    bool isInit();

    bool isConnected();

    //Device Functions
    std::vector<std::string> getDeviceList();

    void connectDevice(int index);

    void disconnectDevice();

    //Message Functions
    bool existMessage(long id);

    void createMessage(const long &id, const bool &tx, const int &mode, const unsigned int &rate, const unsigned int &value, const int &sBit, const int &eBit);

    void setMessageTx(const long &id, const bool &tx);

    void setMessageMode(const long &id, const int &mode);

    void setMessageRate(const long &id,  const unsigned int &value, const int &sBit, const int &eBit);

    void setMessageData(const long &id, const unsigned int &value, const int &sBit, const int &eBit);

    void stopWorker(const long &id);

    void updataAllBaseData();
    void DisconnectFromDevice();

    void GetMessagesFromDevice(icsSpyMessage stMessages[], int &lNumberOfMessages, int &iNumberOfErrors);

    void setMessageInfo(int NetworkID, long ExternFlag, long RemoteFlag=0);

  private:
    std::shared_ptr<SpyManagePrivate> d_ptr = nullptr;

    int m_NetworkID;   //网络类型
    long m_ExternFlag; //是否为扩展帧
    long m_RemoteFlag; //是否为远程帧
    int m_hObject;
};
