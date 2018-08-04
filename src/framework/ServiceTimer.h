#ifndef _SERVICE_TIMER_H_
#define _SERVICE_TIMER_H_

#include <mutex>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/asio.hpp>
#include <map>
#include <string>

using namespace boost::asio;

namespace bb {
	class ServiceTimer
	{
	public:
		typedef boost::shared_ptr<ServiceTimer> ptr;
		typedef boost::function<void(void *para)> cb;

		struct CbItem {
			typedef boost::shared_ptr<CbItem> ptr;
			cb f;
			long long expire_time;
			void *para;
			bool isRepeate;
		};
	
		ServiceTimer(io_service & service);
		~ServiceTimer();

		std::string registerTimer(cb f, long long mics, bool repeate = false);
		void cancelTimer(const std::string & id);

	private:
		void onTimer(const boost::system::error_code& error);

	private:
		deadline_timer m_timer;
		// std::mutex m_mtx;
	};
}

#endif // _SERVICE_TIMER_H_

