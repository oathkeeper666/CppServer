#ifndef _SERVER_H_
#define _SERVER_H_

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <queue>
#include <map>
#include "IRunnable.h"

using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

namespace bb {
	class Session;
	class WorkThread;
	class IoThread;

	struct ServerConf 
	{
		std::string ip;
		int port;
		bool ipv6;
		size_t io_count;

		std::string address() 
		{
			return ip + ":" + std::to_string(port);
		}

		ServerConf(const std::string & ip, int port, size_t count, bool ipv6 = false) 
		{
			this->ip = ip;
			this->port = port;
			this->io_count = count;
			this->ipv6 = ipv6;
		}

		ServerConf() 
		{
			ip = "";
			port = 0;
			io_count = 0;
			ipv6 = false;
		}
	};

	class TcpServer : public IRunnable
	{
	public:
		typedef shared_ptr<TcpServer> ptr;
		TcpServer(const ServerConf & conf);
		virtual ~TcpServer();

		void aync_accept();
		virtual void run();
		virtual void stop();
		virtual void update();

	private:
		void handleConnection(boost::shared_ptr<tcp::socket> & sock, const boost::system::error_code & error);
		void handleDisconnection(int sessId);

		void saveSession(boost::shared_ptr<tcp::socket> & sock);

	private:
		io_service m_service;
		tcp::acceptor m_acceptor;
		tcp::socket m_socket;							
		int m_sess_id;						// auto increase
		ServerConf m_conf;
		std::mutex m_sess_mtx;
		std::map<int, boost::shared_ptr<Session> > m_sesses;
		std::mutex m_accept_mxt;
		std::queue<boost::shared_ptr<tcp::socket> > m_accept_socks;
		boost::shared_ptr<WorkThread> m_work_thread;
		std::vector<boost::shared_ptr<IoThread> > m_io_threads;
	};
}

#endif

