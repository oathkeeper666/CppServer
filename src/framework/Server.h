#ifndef _SERVER_H_
#define _SERVER_H_

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <set>

using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

namespace bb {
	class Session;

	class TcpServer
	{
	public:
		typedef shared_ptr<TcpServer> ptr;

		TcpServer(io_service & service, tcp::endpoint & endPoint);
		~TcpServer();

		void accept();
		void stop() { m_stop = true; }

	private:
		void handleConnection(const boost::system::error_code & error);
		void handleDisconnection(boost::shared_ptr<Session> sess);

	private:
		io_service & m_service;
		tcp::acceptor m_acceptor;
		tcp::socket m_socket;								
		std::set<boost::shared_ptr<Session> > m_sesses;
		bool m_stop;
	};
}

#endif

