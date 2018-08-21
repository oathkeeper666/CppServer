#include "ThreadPool.h"

namespace bb {
	ThreadPool::ThreadPool(unsigned int count)
		: m_pool(count)
	{

	}

	ThreadPool::ThreadPool()
		: m_pool()
	{

	}

	ThreadPool::~ThreadPool()
	{

	}
}