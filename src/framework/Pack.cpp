#include "Pack.h"

namespace bb {
	Pack::Pack(int type, size_t len)
		: m_type(type)
		,m_len(len)
	{
		m_buf.reset(new Buffer(len));
	}


	Pack::~Pack()
	{
	}

}
