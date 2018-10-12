#include "ThreadBase.h"
#include <boost/bind.hpp>

namespace bb {
    ThreadBase::ThreadBase(const std::string & name)
        : m_name(name)
        ,m_stop(true)
    {

    }

    ThreadBase::ThreadBase()
        : m_name("")
        ,m_stop(true)
    {

    }

    ThreadBase::~ThreadBase()
    {

    }

    void ThreadBase::run()
    {
        m_stop = false;
        m_thread = std::thread(boost::bind(&ThreadBase::threadFunc, this));
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }

    void ThreadBase::stop()
    {
        m_stop = true;
    }
}