#ifndef _IOTHREAD_H_
#define _IOTHREAD_H_

#include <boost/shared_ptr.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include "ThreadBase.h"

using namespace boost::asio;

namespace bb 
{
    class IoThread : public ThreadBase 
    {
    public:
        typedef boost::shared_ptr<IoThread> ptr;
        explicit IoThread(io_service * service);
        ~IoThread();

        void stop();

    protected:
        void threadFunc();

    private:
        io_service * m_service;
    };
}

#endif // _IOTHREAD_H_