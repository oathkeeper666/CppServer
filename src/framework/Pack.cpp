#include "Pack.h"
#include "Buffer.h"

namespace bb {
	Pack::Pack(int cmd, size_t len)
		: m_cmd(cmd)
	{
		m_buf.reset(new Buffer(len));
	}

	Pack::~Pack()
	{

	}

	void Pack::writeData(void *s, size_t n)
	{
		m_buf->writeData(s, n);
	}

	void * Pack::data() const
	{
		return m_buf->begin();
	}

	size_t Pack::size() const
	{
		return m_buf->size();
	}
}
