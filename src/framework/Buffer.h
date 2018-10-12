#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <boost/shared_ptr.hpp>

namespace bb {
	class Buffer {
	public:
		typedef boost::shared_ptr<Buffer> ptr;

		Buffer(size_t cap);
		~Buffer();

		void * peek() const;
		void * begin() const;
		void * end() const;
		size_t capacity() const;
		size_t size() const;
		bool full() const;
		bool empty() const;
		void offset(int n);
		size_t available() const;
		void clear();
		void clearHead(size_t n);
		void writeData(void * p, size_t n);
		void resize();

	private:
		size_t m_cap;
		void *m_addr;
		void *m_offset;
	};
}

#endif // _BUFFER_H_