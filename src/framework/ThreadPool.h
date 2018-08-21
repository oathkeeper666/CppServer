#ifndef _THRREAD_POOL_H_
#define _THRREAD_POOL_H_

#include <boost/thread/executors/basic_thread_pool.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

namespace bb {
	template<typename Para>
	struct Task
	{
		boost::function<void (Para)> func;
		Para para;

		Task(boost::function<void (Para)> & f, Para p) {
			func = f;
			para = p;
		}

		void operator()() {
			func(para);
		}
	};

	class ThreadPool
	{
	public:
		typedef boost::shared_ptr<ThreadPool> ptr;
		ThreadPool(unsigned int count);
		ThreadPool();
		~ThreadPool();

		/*
			@detail 添加一个等待处理的task
		*/
		template<typename T>
		void addTask(Task<T> & task) {
			m_pool.submit<Task<T> >(task);
		}

	private:
		boost::basic_thread_pool m_pool;
	};
}

#endif // _THRREAD_POOL_H_