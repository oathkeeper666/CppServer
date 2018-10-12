#ifndef _TIME_UTIL_H_
#define _TIME_UTIL_H_

#include <boost/noncopyable.hpp>

extern time_t g_now;

namespace bb {
	class TimeUtil : public boost::noncopyable
	{
	public:
		static time_t now();
	};
}

#endif // _TIME_UTIL_H_