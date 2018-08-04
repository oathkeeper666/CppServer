#ifndef _PACK_H_
#define _PACK_H_

#include <boost/shared_ptr.hpp>
#include "Buffer.h"

namespace bb {
	class Pack
	{
	public:
		typedef boost::shared_ptr<Pack> ptr;

		Pack(int type, size_t len);
		~Pack();

		bool empty() const {
			return m_buf->empty();
		}

		bool full() const {
			return size() >= m_len;
		}

		size_t len() const {
			return m_len;
		}

		size_t size() const {
			return m_buf->size();
		}

		int type() const {
			return m_type;
		}

		void writeData(void *p, size_t n) {
			m_buf->writeData(p, n);
		}

		size_t available() const {
			return m_len - size();
		}

		void * begin() const {
			return m_buf->begin();
		}

	private:
		int m_type;
		size_t m_len;
		boost::shared_ptr<Buffer> m_buf;
	};
}

#endif // _PACK_H_

