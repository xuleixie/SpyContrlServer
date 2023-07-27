#include "SpyManage.h"
#include "SpyManagePrivate.h"
#include "SpyMessage.h"
#include "SpyWorker.h"
#include <iostream>

SpyManage::SpyManage()
    : d_ptr(std::make_shared<SpyManagePrivate>())
{
    //Load DLL functions
    if (LoadDLLAPI(d_ptr->hDLL))
    {
        d_ptr->loadedDll = true;
        int iVerNumber = icsneoGetDLLVersion();
        printf("ICS icsneo40.dll version %d\r\n\r\n", iVerNumber);
    }
    else
    {
        //problem, close the application
        printf("Problem loading Library\r\nMake sure icsneo40.dll is installed and accessable\r\nPress any key to Exit");
        //char cInputCommand = _getch();
    }
}

SpyManage::~SpyManage()
{
    int iResult;
    int iNumberOfErrors;

    //Call close port
    iResult = icsneoClosePort(m_hObject, &iNumberOfErrors);
    if (iResult == 0)
    {
        printf("Problem Closing Port\r\n");
    }
    else
    {
        printf("Port Closed\r\n");
    }

    //clear open flag
    d_ptr->deviceList.clear();
    d_ptr->workerMap.clear();
    d_ptr->msgMap.clear();

    if (d_ptr->loadedDll)
        UnloadDLLAPI(d_ptr->hDLL);
    d_ptr = nullptr;
}

bool SpyManage::isInit()
{
    return d_ptr->loadedDll;
}

bool SpyManage::isConnected()
{
    return d_ptr->bIsConnect;
}

std::vector<std::string> SpyManage::getDeviceList()
{
    std::vector<std::string> deviceList;

    if (d_ptr->loadedDll == false)
        return deviceList;

    NeoDevice ndNeoToOpen[10];
    int iNumberOfDevices = 10;

    //Search for attached devices
    int iResult = icsneoFindNeoDevices(65535, ndNeoToOpen, &iNumberOfDevices);
    if (iResult == false)
    {
        printf("Problem Finding Device\r\n");
        return deviceList;
    }

    if (iNumberOfDevices < 1)
    {
        printf("No Devices Found\r\n");
        return deviceList;
    }

    d_ptr->deviceList.clear();

    std::string sTempString;
    //Print list of devices
    for (int iCounter = 0; iCounter < iNumberOfDevices; iCounter++)
    {
        switch (ndNeoToOpen[iCounter].DeviceType)
        {
        case 1:
            sTempString = " - DeviceType: neoVI Blue";
            break;
        case 4:
            sTempString = " - DeviceType: ValueCAN 2";
            break;
        case 8:
            sTempString = " - DeviceType: neoVI FIRE";
            break;
        case 16:
            sTempString = " - DeviceType: ValueCAN 3";
            break;
        default:
            sTempString = " - DeviceType: Unknown neoVI";
        }
        //printf("SN:%d %s\r\n", ndNeoToOpen[iCounter].SerialNumber, sTempString.c_str());
        deviceList.emplace_back("SN: " + std::to_string(ndNeoToOpen[iCounter].SerialNumber) + sTempString);
        d_ptr->deviceList.emplace_back(ndNeoToOpen[iCounter]);
    }
    return deviceList;
}

void SpyManage::connectDevice(int index)
{
    if (d_ptr->loadedDll == false)
        return;

    //Make sure the device is NOT open
    if (d_ptr->bIsConnect == true || d_ptr->deviceList.empty())
    {
        //printf("Device already opened\r\n");
        return;
    }

    d_ptr->hObject = 0;
    d_ptr->currentDevice = d_ptr->deviceList.at(index);
    //Connect to the first device found
    NeoDevice *pNeoDevice = &(d_ptr->currentDevice);
    int *h = &(d_ptr->hObject);
    int iResult = icsneoOpenNeoDevice(pNeoDevice, h, NULL, 1, 0);
    m_hObject = d_ptr->hObject;
    if (iResult == false)
    {
        printf("Problem Opening Port\r\n");
        return;
    }
    printf("Port Opened OK!\r\n");

    d_ptr->bIsConnect = true;
}

void SpyManage::disconnectDevice()
{
    if (d_ptr->loadedDll == false)
        return;

    int iNumberOfErrors;

    //Call close port
    int iResult = icsneoClosePort(d_ptr->hObject, &iNumberOfErrors);
    if (iResult == 0)
    {
        printf("Problem Closing Port\r\n");
    }
    else
    {
        printf("Port Closed\r\n");
    }
    //clear open flag
    d_ptr->bIsConnect = false;
}

void SpyManage::DisconnectFromDevice(void)
{
    int iResult;
    int iNumberOfErrors;

    //Call close port
    iResult = icsneoClosePort(m_hObject, &iNumberOfErrors);
    if (iResult == 0)
    {
        printf("Problem Closing Port\r\n");
    }
    else
    {
        printf("Port Closed\r\n");
    }
    //clear open flag
    d_ptr->deviceList.clear();
    d_ptr->msgMap.clear();
    d_ptr->workerMap.clear();
}

void SpyManage::GetMessagesFromDevice(icsSpyMessage stMessages[], int &lNumberOfMessages, int &iNumberOfErrors)
{

    long lResult;
    //Call Get Messages
    lResult = icsneoGetMessages(m_hObject, stMessages, &lNumberOfMessages, &iNumberOfErrors); //Call get message function
    if (lResult == 0)                                                                         //See if Call was successful
    {
        printf("Problem Getting Messages\r\n");
        return;
    }

    //if there are no messages get out of function
    if (lNumberOfMessages == 0)
        return;
}

void SpyManage::setMessageInfo(int NetworkID, long ExternFlag, long RemoteFlag)
{
    m_NetworkID = NetworkID;
    m_ExternFlag = ExternFlag;
    m_RemoteFlag = RemoteFlag;
}

bool SpyManage::existMessage(long id)
{
    auto search = d_ptr->msgMap.find(id);
    if (search != d_ptr->msgMap.end())
        return true;
    return false;
}

void SpyManage::createMessage(const long &id, const bool &tx, const int &mode,
                              const unsigned int &rate, const unsigned int &value, const int &sBit, const int &eBit)
{
    auto search = d_ptr->msgMap.find(id);
    if (search == d_ptr->msgMap.end())
    {
        auto msg = std::make_shared<SpyMessage>(d_ptr->hObject, id, m_NetworkID, m_ExternFlag);
        msg->setTx(tx);
        msg->setMode(static_cast<SpyMessage::AutoTxMode>(mode));
        msg->setRate(rate);
        // emplace判断是否已存在该报文和id 存在替代不存在加入
        d_ptr->msgMap.emplace(id, msg);

        if ((tx && mode == SpyMessage::Periodic_ONOFF) || (mode == SpyMessage::Periodic))
        {
            auto worker = d_ptr->getWorker();
            if (worker)
                worker->addTask(msg, value, sBit, eBit);
        }
    }
}

void SpyManage::setMessageTx(const long &id, const bool &tx)
{
    auto search = d_ptr->msgMap.find(id);
    if (search != d_ptr->msgMap.end())
    {
        if (search->second->mode() == SpyMessage::AtStart)
        {
            search->second->send();
        }
        else if (search->second->mode() == SpyMessage::Periodic_ONOFF)
        {
            if (search->second->rate() != 0)
            {
                auto worker = d_ptr->getWorker();
                if (worker)
                {
                    if (tx)
                        worker->addTask(search->second);
                    else
                        worker->delTask(search->second);
                }
            }
        }
        search->second->setTx(tx);
    }
}

void SpyManage::setMessageMode(const long &id, const int &mode)
{
    auto search = d_ptr->msgMap.find(id);
    if (search != d_ptr->msgMap.end())
    {
        auto worker = d_ptr->getWorker();
        if (mode == SpyMessage::AtStart)
        {
            if (worker)
                worker->delTask(search->second);
        }
        else
        {
            if (worker)
            {
                if (mode == SpyMessage::Periodic)
                {
                    worker->addTask(search->second);
                }
                else
                {
                    if (search->second->tx())
                        worker->addTask(search->second);
                    else
                        worker->delTask(search->second);
                }
            }
            else
            {
                // worker->delTask(search->second); ??
            }
        }
        search->second->setMode(static_cast<SpyMessage::AutoTxMode>(mode));
    }
}

void SpyManage::setMessageRate(const long &id,
                               const unsigned int &value, const int &sBit, const int &eBit)
{
    auto search = d_ptr->msgMap.find(id);
    if (search != d_ptr->msgMap.end())
    {

        auto worker = d_ptr->getWorker();
        if (worker)
        {
            worker->delTask(search->second);

            worker = d_ptr->getWorker();
            if (worker)
                worker->addTask(search->second, value, sBit, eBit);
        }
    }
}

void SpyManage::setMessageData(const long &id, const unsigned int &value, const int &sBit, const int &eBit)
{
    auto search = d_ptr->msgMap.find(id);
    if (search != d_ptr->msgMap.end())
    {
        auto worker = d_ptr->getWorker();
        if (worker)
        {
            if (worker->existTask(search->second))
                worker->setTaskValue(search->second, value, sBit, eBit);
            else
                search->second->setMsgData(value, sBit, eBit);
        }
    }
}

void SpyManage::stopWorker(const long &id)
{
    auto search = d_ptr->msgMap.find(id);
    auto worker = d_ptr->getWorker();
    worker->delTask(search->second);
}

void SpyManage::updataAllBaseData()
{
    for (auto &it : d_ptr->msgMap)
    {
        it.second->setHandleAndNetworkID(d_ptr->hObject, d_ptr->NetworkID);
    }
}
