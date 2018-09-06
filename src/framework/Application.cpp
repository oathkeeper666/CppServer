#include "Application.h"
#include <unistd.h>
#include <string.h>

namespace bb 
{
    Application::Application(int argc, char *argv[])
        : m_path("")
        ,m_daemon(false)
    {
        parseCmd(argc, argv);
    }

    Application::~Application() 
    {

    }

    const std::string & Application::path() const
    {
        return m_path;
    }

    bool Application::isDaemon() const
    {
        return m_daemon;
    }

    void Application::parseCmd(int argc, char *argv[])
    {
	    for (int i = 1; i < argc; i++) {
		    if (strcmp("-d", argv[i])) {
			    m_daemon = true;
		    } else if (strcmp("-c", argv[i]) && i != argc - 1) {
			    m_path = argv[i + 1];
		    }
	    }
    }

    void Application::run() 
    {
        // daemon
		if (isDaemon()) {
			daemon(1, 0);
		}
    }
}