#include "ServiceTimer.h"
#include "TimeUtil.h"
#include <boost/bind.hpp>

namespace bb {
	ServiceTimer::ServiceTimer(io_service & service)
		: m_timer(service)
	{
		m_timer.expires_from_now(boost::posix_time::microseconds(1000));
		m_timer.async_wait(boost::bind(&ServiceTimer::onTimer, this, boost::asio::placeholders::error));
	}

	ServiceTimer::~ServiceTimer()
	{
	}

	std::string ServiceTimer::registerTimer(cb f, long long mics, bool repeate)
	{
		//std::lock_guard<std::mutex> lock(m_mtx);
		return std::string();
	}

	void ServiceTimer::cancelTimer(const std::string & id)
	{
		//std::lock_guard<std::mutex> lock(m_mtx);
	}

	void ServiceTimer::onTimer(const boost::system::error_code & error)
	{
		// handle register event
		//std::lock_guard<std::mutex> lock(m_mtx);
		m_timer.expires_from_now(boost::posix_time::microseconds(1000));
		m_timer.async_wait(boost::bind(&ServiceTimer::onTimer, this, boost::asio::placeholders::error));
	}
}
