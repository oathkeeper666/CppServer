#ifndef _GWCONF_H_
#define _GWCONF_H_

#include "loader.h"
#include <string>

namespace common
{
	class GwConf : public Loader
	{
	public:
		~GwConf();
		static GwConf * instance();
		bool loadRes(const std::string & doc = 0);
		std::string toString();

	private:
		GwConf();

	private:
		std::string m_log;
		std::string m_ip;
		int m_port;
	};
}

#endif // _GWCONF_H_