#ifndef _THREADBASE_H_
#define _THREADBASE_H_

#include <thread>
#include <string>

namespace bb
{
    class ThreadBase 
    {
    public:
        ThreadBase();
        ThreadBase(const std::string & name);
        virtual ~ThreadBase();

        void run();
        virtual void stop();

        std::string name() const { return m_name; }
        void name(const std::string & name) { m_name = name; }

    protected:
        virtual void threadFunc() = 0;

    protected:
        std::thread m_thread;
        std::string m_name;
        bool m_stop;
    };  
} // bb

#endif // _THREADBASE_H_