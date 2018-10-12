#ifndef _PACK_H_
#define _PACK_H_

#include <boost/shared_ptr.hpp>

namespace bb {
	class Buffer;

	struct PackHeader 
	{
		int len;
		int cmd;
	};

	class Pack
	{
	public:
		typedef boost::shared_ptr<Pack> ptr;

		Pack(int cmd, size_t len);
		~Pack();

		void writeData(void *s, size_t n);
		void * data() const;
		size_t size() const;

	private:
		int m_cmd;							// 协议号					
		boost::shared_ptr<Buffer> m_buf;				
	};
}

#endif // _PACK_H_

