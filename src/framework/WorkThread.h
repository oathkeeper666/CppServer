#ifndef _WORKTHREAD_H_
#define _WORKTHREAD_H_

#include <boost/shared_ptr.hpp>
#include "ThreadBase.h"

namespace bb 
{
    class IRunnable;

    class WorkThread : public ThreadBase 
    {
    public:
        typedef boost::shared_ptr<WorkThread> ptr;
        explicit WorkThread(IRunnable * task);
        ~WorkThread();

    protected:
        void threadFunc();

    private:
        IRunnable *m_task;
    };
}

#endif // _WORKTHREAD_H_