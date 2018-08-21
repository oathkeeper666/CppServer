#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include "spdlog/spdlog.h"
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

	private:
		Resource();

	private:
		std::shared_ptr<spdlog::logger> m_logger;
	};
}

#endif // _RESOURCE_H_