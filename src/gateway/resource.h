#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include "spdlog/spdlog.h"
#include "framework/ThreadPool.h"
#include <memory>

#define LOGGER gateway::Resource::instance()->logger()

namespace gateway {
	class Resource
	{
	public:
		~Resource();
		static Resource * instance();
		void init(bool daemon = false);

		std::shared_ptr<spdlog::logger> logger();
		bb::ThreadPool::ptr schedule() const;

	private:
		Resource();

	private:
		std::shared_ptr<spdlog::logger> m_logger;
		bb::ThreadPool::ptr m_thrad_pool;
	};
}

#endif // _RESOURCE_H_