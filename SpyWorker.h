#pragma once
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <thread>
#include <windows.h>

class SpyMessage;

class SpyWorker
{
  public:
    SpyWorker();
    ~SpyWorker();

    void addTask(std::shared_ptr<SpyMessage> task, const unsigned int &value, const int &sBit, const int &eBit);
    void addTask(std::shared_ptr<SpyMessage> task);
    void delTask(std::shared_ptr<SpyMessage> task);

    void setTaskValue(std::shared_ptr<SpyMessage> task, const unsigned int &value, const int &sBit, const int &eBit);

    bool existTask(std::shared_ptr<SpyMessage> task);

    void run();

  private:
    std::deque<std::weak_ptr<SpyMessage>> m_taskQueue0;
    std::deque<std::weak_ptr<SpyMessage>> m_taskQueue50;
    std::deque<std::weak_ptr<SpyMessage>> m_taskQueue100;
    std::deque<std::weak_ptr<SpyMessage>> m_taskQueue250;
    std::deque<std::weak_ptr<SpyMessage>> m_taskQueue500;
    std::deque<std::weak_ptr<SpyMessage>> m_taskQueue1000;

    unsigned long start50;
    unsigned long start100;
    unsigned long start250;
    unsigned long start500;
    unsigned long start1000;
    unsigned long curveTiem;

    std::shared_ptr<std::thread> m_pThread = nullptr;

    std::mutex m_mutex;
    std::condition_variable m_conditional_lock;
    bool m_exitLoop = false;
    bool m_expired = false;
    HANDLE hthread;
};
