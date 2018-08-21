#include "resource.h"
#include "spdlog/sinks/daily_file_sink.h"

namespace gateway {
	Resource::Resource()
	{
		
	}

	Resource::~Resource()
	{

	}

	Resource * Resource::instance()
	{
		static Resource res;
		return &res;
	}

	void Resource::init()
	{
		// log
		m_logger = spdlog::daily_logger_mt("gateway", "../log/gateway.log", 4, 0);
		m_logger->set_level(spdlog::level::trace);
	}

	std::shared_ptr<spdlog::logger> Resource::logger()
	{
		return m_logger;
	}
}