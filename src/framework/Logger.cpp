#include "Logger.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace bb
{
    Logger::Logger()
    {
    
    }

    Logger::~Logger()
    {

    }

    void Logger::init(const LogConfig & info)
    {
		if (info.m_daemon) {
 			m_logger = spdlog::daily_logger_mt(info.m_module, info.m_path + info.m_module + ".log", 4, 0);
 		} else {
			m_logger = spdlog::stdout_color_mt(info.m_module);
 		}
 		m_logger->set_level((spdlog::level::level_enum) info.m_level);
		spdlog::flush_every(std::chrono::seconds(info.m_flush_sec));
    }

    
}