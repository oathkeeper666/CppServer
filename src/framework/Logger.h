#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "spdlog/spdlog.h"
#include "Singleton.h"
#include <memory>
#include <string>

namespace bb 
{
    struct LogConfig 
    {
        std::string m_module;
        std::string m_path;
        int m_level;
        bool m_daemon;
        int m_flush_sec;

        LogConfig() {
            m_module = "";
            m_path = "";
            m_level = 0;
            m_daemon = false;
            m_flush_sec = 10;
        }
    };

    class Logger 
    {
    public:
        Logger();
        ~Logger();

        void init(const LogConfig & info);

    private:
        std::shared_ptr<spdlog::logger> m_logger;
    };
}

#endif // _LOGGER_H_