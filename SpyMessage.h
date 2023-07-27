#pragma once
#include <atomic>
#include "icsnVC40.h"

class SpyMessage
{
public:
    enum AutoTxMode
    {
        Periodic = 0,
        AtStart,
        Periodic_ONOFF
    };
    SpyMessage(int hObject, int MesID, int NetworkID, long ExternFlag);
    ~SpyMessage();

    void setHandleAndNetworkID(int h, int n);

    void setMsgData(unsigned int value, int sBit, int eBit);

    void send();

    bool tx();
    void setTx(bool tx);

    AutoTxMode mode();
    void setMode(AutoTxMode m);

    unsigned int rate();
    void setRate(unsigned int r);
          
private:
    bool m_bTx = false;
    AutoTxMode m_Mode = Periodic;
    unsigned int m_Rate = 0;

    icsSpyMessage* m_SpyMsg;
    int m_hObject;
    int m_NetworkID;

};

