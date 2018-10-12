#ifndef _IRUNNABLE_H_
#define _IRUNNABLE_H_

namespace bb 
{
    class IRunnable 
    {
    public:
        virtual void update() = 0;
    };
}

#endif // _IRUNNABLE_H_