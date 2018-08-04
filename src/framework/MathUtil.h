#ifndef _MATH_UTIL_H_
#define _MATH_UTIL_H_

#include <boost/noncopyable.hpp>
#include <vector>

namespace bb {
	class MathUtil : public boost::noncopyable
	{
	public:
		/* @detail 根据事件发生的概率来随机
		 * @param posibilities 事件发生的概率数组
		 * @param n posibilities长度
		 * @ret 返回发生的事件
		*/
		static int random(float *posibilities, size_t n);
		static int random(const std::vector<float> & vec);

		/*
		 * @detail 随机返回[m, n]之间的整数
		*/
		static int random(int m, int n);
	};
}


#endif // _MATH_UTIL_H_