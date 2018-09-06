#include "gwconf.h"
#include "dist/json/json.h"
#include <boost/assert.hpp>
#include <boost/filesystem.hpp>
#include <sstream>
#include <iostream>

namespace gateway 
{
	GwConf::GwConf()
	{
		
	}

	GwConf::~GwConf()
	{

	}

	bool GwConf::loadRes(const std::string & doc)
	{
		char buf[2048] = { '\0' };
		std::string errs;
		Json::CharReaderBuilder builder;
		Json::CharReader *reader = builder.newCharReader();

		boost::filesystem::ifstream fs(doc);
		std::streamsize sz = fs.readsome(buf, sizeof(buf));
		Json::Value value;
		if (!reader->parse(buf, buf + sz, &value, &errs)) {
			std::cout << "parse json failed." << std::endl;
			BOOST_ASSERT(false);
		}

		m_log = value["log_path"].asString();
		m_ip = value["ip"].asString();
		m_port = value["port"].asInt();

		return true;
	}

	std::string GwConf::toString()
	{
		std::stringstream ss;
		ss << "log path: " << m_log << ", "
		   << "ip: " << m_ip << ", "
		   << "port: " << m_port << '\n';
		return ss.str();
	}

}
