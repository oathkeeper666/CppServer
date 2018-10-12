#include "TimeUtil.h"
#include <boost/date_time/posix_time/posix_time.hpp>

static boost::posix_time::ptime EPOCH_TIME(boost::gregorian::date(1970, 1, 1));

time_t g_now;

namespace bb {
	time_t TimeUtil::now()
	{
		return (boost::posix_time::second_clock::universal_time() - EPOCH_TIME).total_seconds();
	}
}