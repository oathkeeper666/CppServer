#include "WorkThread.h"
#include "IRunnable.h"
#include <boost/chrono.hpp>

namespace bb 
{
    WorkThread::WorkThread(IRunnable * task)
        : ThreadBase()
        ,m_task(task)
    {

    }

    WorkThread::~WorkThread()
    {

    }

    void WorkThread::threadFunc()
    {
        while (!m_stop) {
            if (m_task) {
                m_task->update();
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
    }
}