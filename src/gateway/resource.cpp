#include "resource.h"
#include "gwconf.h"
#include "gateway.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

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

	void Resource::init(bool daemon)
	{
		GwConf *conf = GwConf::instance();

		// log
		if (daemon) {
 			m_logger = spdlog::daily_logger_mt(MODULE_NAME, conf->logPath() + MODULE_NAME + ".log", 4, 0);
 		} else {
			m_logger = spdlog::stdout_color_mt(MODULE_NAME);
 		}
 		m_logger->set_level(spdlog::level::trace);
		spdlog::flush_every(std::chrono::seconds(REFRESH_INTERVAL));

		// thread pool
		m_thrad_pool.reset(new bb::ThreadPool(THREAD_COUNT));
	}

	std::shared_ptr<spdlog::logger> Resource::logger()
	{
		return m_logger;
	}

	bb::ThreadPool::ptr Resource::schedule() const
	{
		return m_thrad_pool;
	}
}