#pragma once
#include <deque>
#include <memory>
#include <string>
#include <unordered_map>

#include "icsnVC40.h"
#include "icsneo40DLLAPI.h"

class SpyMessage;
class SpyWorker;
class SpyManage;
class SpyManagePrivate
{
  public:
    SpyManagePrivate();
    ~SpyManagePrivate();

    bool existWorker(unsigned int interval);

    SpyWorker *getWorker();

  private:
    friend class SpyManage;
    HINSTANCE hDLL;
    bool loadedDll = false;

    bool bIsConnect = false;
    int hObject = 0;
    int NetworkID = NETID_HSCAN;

    NeoDevice currentDevice;
    //设备容器
    std::deque<NeoDevice> deviceList;
    //报文容器
    std::unordered_map<long, std::shared_ptr<SpyMessage>> msgMap;
    //线程池
    std::unordered_map<unsigned int, std::shared_ptr<SpyWorker>> workerMap;

    SpyWorker *ThreadWork;
};
