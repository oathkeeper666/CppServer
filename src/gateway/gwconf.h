#ifndef _GWCONF_H_
#define _GWCONF_H_

#include "common/loader.h"
#include <string>

namespace gateway
{
	class GwConf : public common::Loader
	{
	public:
		~GwConf();
		static GwConf * instance();
		bool loadRes(const std::string & doc = 0);
		std::string toString();

		const std::string & logPath() const { return m_log; }
		const std::string & listenIp() const { return m_ip; }
		int port() const { return m_port; }

	private:
		GwConf();

	private:
		std::string m_log;
		std::string m_ip;
		int m_port;
	};
}

#endif // _GWCONF_H_
