#include "IoThread.h"

namespace bb 
{
    IoThread::IoThread(io_service * service)
        : ThreadBase()
    {
        m_service = service;
    }

    IoThread::~IoThread()
    {

    }

    void IoThread::threadFunc()
    {
        executor_work_guard<io_context::executor_type> work = make_work_guard(*m_service);
    }

    void IoThread::stop()
    {
        //m_work.reset(); // Allow run() to exit.
        ThreadBase::stop();
    }
}