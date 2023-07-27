#include "SpyMessage.h"
#include "Calculator.h"
#include "icsneo40DLLAPI.h"
#include <iostream>

SpyMessage::SpyMessage(int hObject, int MesID, int NetworkID, long ExternFlag)
{
    m_SpyMsg = new icsSpyMessage;
    memset(m_SpyMsg, 0, sizeof(icsSpyMessage));
    //Set the ID
    m_SpyMsg->ArbIDOrHeader = MesID;
    // The number of Data Bytes
    m_SpyMsg->NumberBytesData = 8;
    m_SpyMsg->StatusBitField = ExternFlag;

    m_hObject = hObject;
    m_NetworkID = NetworkID;
}

SpyMessage::~SpyMessage()
{
    if (m_SpyMsg)
        delete m_SpyMsg;
}

void SpyMessage::setHandleAndNetworkID(int h, int n)
{
    m_hObject = h;
    m_NetworkID = n;
}

void SpyMessage::setMsgData(unsigned int value, int sBit, int eBit)
{
    int data_len = eBit - sBit;
    for (int index = sBit; index <= eBit; index++)
    {
        if ((value >> data_len) & 0x1)
        {
            m_SpyMsg->Data[(index - 1) / 8] |= (0x1 << (7 - ((index - 1) % 8)));
        }
        else
        {
            m_SpyMsg->Data[(index - 1) / 8] &= ~(0x1 << (7 - ((index - 1) % 8)));
        }
        data_len--;
    }

    std::cout << __FUNCTION__ << " value: " << value << std::endl;
}

void SpyMessage::send()
{
    int result = icsneoTxMessages(m_hObject, m_SpyMsg, m_NetworkID, 1);
    std::cout << __FUNCTION__ << " " << this << " result: " << result << std::endl;
    std::cout << "HEX: ";
    for (int index = 0; index < 8; index++)
    {
        std::cout << std::hex << static_cast<unsigned int>(m_SpyMsg->Data[index]) << " ";
    }
    std::cout << std::endl;
}

bool SpyMessage::tx()
{
    return m_bTx;
}

void SpyMessage::setTx(bool tx)
{
    if (m_bTx != tx)
    {
        //std::cout << __FUNCTION__ << " " << tx << std::endl;
        m_bTx = tx;
    }
}

SpyMessage::AutoTxMode SpyMessage::mode()
{
    return m_Mode;
}

void SpyMessage::setMode(AutoTxMode m)
{
    if (m_Mode != m)
    {
        //std::cout << __FUNCTION__ << " " << m << std::endl;
        m_Mode = m;
    }
}

unsigned int SpyMessage::rate()
{
    return m_Rate;
}

void SpyMessage::setRate(unsigned int r)
{
    if (m_Rate != r)
    {
        //std::cout << __FUNCTION__ << " " << r << std::endl;
        m_Rate = r;
    }
}
