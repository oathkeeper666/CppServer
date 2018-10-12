#include "Buffer.h"
#include <cstdlib>
#include <cstring>

namespace bb {
	Buffer::Buffer(size_t cap)
		: m_cap(cap)
	{
		if (m_cap < 256) {
			m_cap = 256;
		}
		m_cap &= 0xfffe;
		m_addr = malloc(m_cap);
		memset(m_addr, 0, m_cap);
		m_offset = m_addr;
	}

	Buffer::~Buffer()
	{
		free(m_addr);
	}

	void * Buffer::peek() const
	{
		return m_offset;
	}

	size_t Buffer::capacity() const
	{
		return m_cap;
	}

	size_t Buffer::size() const
	{
		return (size_t) ((char *)m_offset - (char *)m_addr);
	}

	bool Buffer::full() const
	{
		return size() == capacity();
	}

	bool Buffer::empty() const
	{
		return size() == 0;
	}

	void Buffer::offset(int n)
	{
		m_offset = (char *)m_offset + n;
		if (m_offset < begin()) {
			m_offset = begin();
		}
		if (m_offset > end()) {
			m_offset = end();
		}
	}

	void * Buffer::begin() const
	{
		return m_addr;
	}

	void * Buffer::end() const
	{
		return (char *)m_addr + m_cap;
	}

	size_t Buffer::available() const
	{
		return m_cap - size();
	}

	void Buffer::clear()
	{
		memset(m_addr, 0, m_cap);
		m_offset = m_addr;
	}

	void Buffer::clearHead(size_t n)
	{
		memset(m_addr, 0, n);
		memcpy(m_addr, (char *)m_addr + n, size() - n);
		m_offset = (char *)m_offset - n;
	}

	void Buffer::writeData(void * p, size_t n)
	{
		if (size() + n > m_cap) {
			resize();
		}
		memcpy(peek(), p, n);
		m_offset = (char *)m_offset + n;
	}

	void Buffer::resize()
	{
		size_t n = size();
		m_cap *= 2;
		void * old = m_addr;
		m_addr = malloc(m_cap);
		memset(m_addr, 0, m_cap);
		memcpy(m_addr, old, n);
		m_offset = (char *)m_addr + n;
		free(old);
	}
}