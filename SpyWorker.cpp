#include "SpyWorker.h"
#include "SpyMessage.h"
#include <iostream>

static DWORD WINAPI startRun(LPVOID lp)
{
    SpyWorker *NewWork = (SpyWorker *)lp;
    NewWork->run();
    return 1;
}
SpyWorker::SpyWorker()

{
    //std::cout << __FUNCTION__ << std::endl;
    start50 = start100 = start250 = start500 = start1000 = curveTiem = 0;
    hthread = CreateThread(NULL, 0, startRun, this, REALTIME_PRIORITY_CLASS, NULL);
    //m_pThread = std::make_shared<std::thread>(&SpyWorker::run, this);
}

SpyWorker::~SpyWorker()
{
    if (m_exitLoop || m_expired || m_pThread == nullptr)
    {
        return;
    }

    m_expired = true;
    m_exitLoop = true;

    m_conditional_lock.notify_all();

    m_pThread->join();
    CloseHandle(hthread);
}

void SpyWorker::addTask(std::shared_ptr<SpyMessage> task,
                        const unsigned int &value, const int &sBit, const int &eBit)
{
    std::lock_guard<std::mutex> guard(m_mutex);

    if (task->rate() == 0)
    {
        for (auto &it : m_taskQueue0)
        {
            if (it.lock() == task)
            {
                setTaskValue(task, value, sBit, eBit);
                return;
            }
        }
        setTaskValue(task, value, sBit, eBit);
        m_taskQueue0.emplace_back(task);
    }
    else if (task->rate() == 50)
    {
        for (auto &it : m_taskQueue50)
        {
            if (it.lock() == task)
            {
                setTaskValue(task, value, sBit, eBit);
                return;
            }
        }
        setTaskValue(task, value, sBit, eBit);
        m_taskQueue50.emplace_back(task);
    }
    else if (task->rate() == 100)
    {
        for (auto &it : m_taskQueue100)
        {
            if (it.lock() == task)
            {
                setTaskValue(task, value, sBit, eBit);
                return;
            }
        }
        setTaskValue(task, value, sBit, eBit);
        m_taskQueue100.emplace_back(task);
    }
    else if (task->rate() == 250)
    {
        for (auto &it : m_taskQueue250)
        {
            if (it.lock() == task)
            {
                setTaskValue(task, value, sBit, eBit);
                return;
            }
        }
        setTaskValue(task, value, sBit, eBit);
        m_taskQueue250.emplace_back(task);
    }
    else if (task->rate() == 500)
    {
        for (auto &it : m_taskQueue500)
        {
            if (it.lock() == task)
            {
                setTaskValue(task, value, sBit, eBit);
                return;
            }
        }
        setTaskValue(task, value, sBit, eBit);
        m_taskQueue500.emplace_back(task);
    }
    else if (task->rate() >= 1000)
    {
        for (auto &it : m_taskQueue1000)
        {
            if (it.lock() == task)
            {
                setTaskValue(task, value, sBit, eBit);
                return;
            }
        }
        setTaskValue(task, value, sBit, eBit);
        m_taskQueue1000.emplace_back(task);
    }
    m_conditional_lock.notify_one(); //??
}

void SpyWorker::addTask(std::shared_ptr<SpyMessage> task)
{
    std::lock_guard<std::mutex> guard(m_mutex);

    if (task->rate() == 0)
    {
        for (auto &it : m_taskQueue0)
        {
            if (it.lock() == task)
                return;
        }
        m_taskQueue0.emplace_back(task);
    }
    else if (task->rate() == 50)
    {
        for (auto &it : m_taskQueue50)
        {
            if (it.lock() == task)
                return;
        }
        m_taskQueue50.emplace_back(task);
    }
    else if (task->rate() == 100)
    {
        for (auto &it : m_taskQueue100)
        {
            if (it.lock() == task)
                return;
        }
        m_taskQueue100.emplace_back(task);
    }
    else if (task->rate() == 250)
    {
        for (auto &it : m_taskQueue250)
        {
            if (it.lock() == task)
                return;
        }
        m_taskQueue250.emplace_back(task);
    }
    else if (task->rate() == 500)
    {
        for (auto &it : m_taskQueue500)
        {
            if (it.lock() == task)
                return;
        }
        m_taskQueue500.emplace_back(task);
    }
    else if (task->rate() >= 1000)
    {
        for (auto &it : m_taskQueue1000)
        {
            if (it.lock() == task)
                return;
        }
        m_taskQueue1000.emplace_back(task);
    }
    m_conditional_lock.notify_one(); //??
}

void SpyWorker::delTask(std::shared_ptr<SpyMessage> task)
{
    std::lock_guard<std::mutex> guard(m_mutex);

    if (task->rate() == 50)
    {
        for (auto it = m_taskQueue50.begin(); it != m_taskQueue50.end(); it++)
        {
            if ((*it).lock() == task)
            {
                m_taskQueue50.erase(it);
                m_conditional_lock.notify_one();
                return;
            }
        }
    }
    else if (task->rate() == 100)
    {
        for (auto it = m_taskQueue100.begin(); it != m_taskQueue100.end(); it++)
        {
            if ((*it).lock() == task)
            {
                m_taskQueue100.erase(it);
                m_conditional_lock.notify_one();
                return;
            }
        }
    }
    else if (task->rate() == 250)
    {
        for (auto it = m_taskQueue250.begin(); it != m_taskQueue250.end(); it++)
        {
            if ((*it).lock() == task)
            {
                m_taskQueue250.erase(it);
                m_conditional_lock.notify_one();
                return;
            }
        }
    }
    else if (task->rate() == 500)
    {
        for (auto it = m_taskQueue500.begin(); it != m_taskQueue500.end(); it++)
        {
            if ((*it).lock() == task)
            {
                m_taskQueue500.erase(it);
                m_conditional_lock.notify_one();
                return;
            }
        }
    }
    else if (task->rate() >= 1000)
    {
        for (auto it = m_taskQueue1000.begin(); it != m_taskQueue1000.end(); it++)
        {
            if ((*it).lock() == task)
            {
                m_taskQueue1000.erase(it);
                m_conditional_lock.notify_one();
                return;
            }
        }
    }
}

void SpyWorker::setTaskValue(std::shared_ptr<SpyMessage> task, const unsigned int &value, const int &sBit, const int &eBit)
{

    task->setMsgData(value, sBit, eBit);
}

bool SpyWorker::existTask(std::shared_ptr<SpyMessage> task)
{

    if (task->rate() == 0)
    {
        for (auto &it : m_taskQueue0)
        {
            if (it.lock() == task)
                return true;
        }
    }
    else if (task->rate() == 50)
    {
        for (auto &it : m_taskQueue50)
        {
            if (it.lock() == task)
                return true;
        }
    }
    else if (task->rate() == 100)
    {
        for (auto &it : m_taskQueue100)
        {
            if (it.lock() == task)
                return true;
        }
    }
    else if (task->rate() == 250)
    {
        for (auto &it : m_taskQueue250)
        {
            if (it.lock() == task)
                return true;
        }
    }
    else if (task->rate() == 500)
    {
        for (auto &it : m_taskQueue500)
        {
            if (it.lock() == task)
                return true;
        }
    }
    else if (task->rate() >= 1000)
    {
        for (auto &it : m_taskQueue1000)
        {
            if (it.lock() == task)
                return true;
        }
    }
    return false;
}

void SpyWorker::run()
{
    std::cout << __FUNCTION__ << " thread id: " << std::this_thread::get_id() << std::endl;
    while (!m_expired)
    {

        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_taskQueue0.empty() && m_taskQueue50.empty() && m_taskQueue100.empty() && m_taskQueue250.empty() && m_taskQueue500.empty() && m_taskQueue1000.empty())
        {
            m_conditional_lock.wait(lock);
        }

        for (auto it = m_taskQueue0.begin(); it != m_taskQueue0.end();)
        {
            if ((*it).lock() != nullptr)
            {
                (*it).lock()->send();

                it = m_taskQueue0.erase(it);
            }
        }

        if (GetTickCount() >= ((start50 + 50)))
        {
            start50 = GetTickCount();
            for (auto it = m_taskQueue50.begin(); it != m_taskQueue50.end();)
            {
                if ((*it).lock() != nullptr)
                {
                    curveTiem = GetTickCount();
                    (*it).lock()->send();
                    it++;
                }
                else
                {
                    m_taskQueue50.erase(it);
                }
            }
        }

        if (GetTickCount() >= (start100 + 100))
        {
            start100 = GetTickCount();
            for (auto it = m_taskQueue100.begin(); it != m_taskQueue100.end();)
            {
                if ((*it).lock() != nullptr)
                {
                    curveTiem = GetTickCount();
                    (*it).lock()->send();
                    it++;
                }
                else
                {
                    m_taskQueue100.erase(it);
                }
            }
        }
        if (GetTickCount() >= (start250 + 250))
        {
            start250 = GetTickCount();
            for (auto it = m_taskQueue250.begin(); it != m_taskQueue250.end();)
            {
                if ((*it).lock() != nullptr)
                {
                    curveTiem = GetTickCount();
                    (*it).lock()->send();
                    it++;
                }
                else
                {
                    m_taskQueue100.erase(it);
                }
            }
        }
        if (GetTickCount() >= (start500 + 500))
        {
            start500 = GetTickCount();
            for (auto it = m_taskQueue500.begin(); it != m_taskQueue500.end();)
            {
                if ((*it).lock() != nullptr)
                {
                    curveTiem = GetTickCount();
                    (*it).lock()->send();
                    it++;
                }
                else
                {
                    m_taskQueue100.erase(it);
                }
            }
        }
        if (GetTickCount() >= (start1000 + 1000))
        {
            start1000 = GetTickCount();
            for (auto it = m_taskQueue1000.begin(); it != m_taskQueue1000.end();)
            {
                if ((*it).lock() != nullptr)
                {
                    curveTiem = GetTickCount();
                    (*it).lock()->send();
                    it++;
                }
                else
                {
                    m_taskQueue100.erase(it);
                }
            }
        }
    }

    std::cout << __FUNCTION__ << " thread id: " << std::this_thread::get_id() << " finished!" << std::endl;
}
