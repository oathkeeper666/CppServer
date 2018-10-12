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
        std::shared_ptr<spdlog::logger> & logger();

    private:
        std::shared_ptr<spdlog::logger> m_logger;
        LogConfig m_conf;
    };
}

#define LOGGER bb::Singleton<Logger>::instance()
#define LOG_TRACE(...) LOGGER->logger()->trace(VA_ARGS)
#define LOG_DEBUG(...) LOGGER->logger()->debug(VA_ARGS)
#define LOG_INFO(...) LOGGER->logger()->info(VA_ARGS)
#define LOG_WARN(...) LOGGER->logger()->warn(VA_ARGS)
#define LOG_ERR(...) LOGGER->logger()->err(VA_ARGS)
#define LOG_CRIT(...) LOGGER->logger()->critical(VA_ARGS)

#endif // _LOGGER_H_