#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <string>

namespace bb
{
    class Application 
    {
    public:
        Application(int argc, char *argv[]);
        virtual ~Application();

        virtual void run();

        const std::string & path() const;
        bool isDaemon() const;

    private:
        void parseCmd(int argc, char *argv[]);

    private:
        std::string m_path;
        bool m_daemon;
    };
}

#endif // _APPLICATION_H_