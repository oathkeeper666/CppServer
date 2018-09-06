#ifndef _SINGLETON_H_
#define _SINGLETON_H_

namespace bb 
{
    template<typename T>
    class Singleton
    {
    public:
        static T * instance() {
            static T ins;
            return &ins;
        }
    };
}


#endif // _SINGLETON_H_
