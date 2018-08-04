#include "MathUtil.h"
#include "TimeUtil.h"
#include <boost/random.hpp>

using namespace boost;

static random::mt19937 gen(bb::TimeUtil::now());

namespace bb {
	int MathUtil::random(float *posibilities, size_t n)
	{
		random::discrete_distribution<> dist(posibilities, posibilities + n);
		return dist(gen);
	}

	int random(const std::vector<float> & vec) 
	{
		random::discrete_distribution<> dist(vec);
		return dist(gen);
	}

	int MathUtil::random(int m, int n)
	{
		if (n < m) {
			return 0;
		}

		random::uniform_int_distribution<> dist(m, n);
    	return dist(gen);
	}
}