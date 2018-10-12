#ifndef _GWCONF_H_
#define _GWCONF_H_

#include <boost/shared_ptr.hpp>
#include "common/loader.h"
#include <string>

namespace gateway
{
	class GwConf : public common::ILoader
	{
	public:
		typedef boost::shared_ptr<GwConf> ptr;
		GwConf();
		~GwConf();

		bool loadRes(const std::string & doc = 0);
		std::string toString();

		const std::string & logPath() const { return m_log; }
		const std::string & listenIp() const { return m_ip; }
		int port() const { return m_port; }
		
	private:
		std::string m_log;	// log path
		std::string m_ip;
		int m_port;
	};
}

#define GATE_INS bb::Singleton<gateway::GwConf>::instance()

#endif // _GWCONF_H_
