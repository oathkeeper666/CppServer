#include "Server.h"
#include "Session.h"
#include <boost/bind.hpp>
#include <boost/circular_buffer.hpp>
#include <iostream>

namespace bb {
	TcpServer::TcpServer(io_service & service, ip::tcp::endpoint & endPoint)
		: m_service(service), m_acceptor(service, endPoint), m_stop(false)
	{

	}

	TcpServer::~TcpServer()
	{
	}

	void TcpServer::accept()
	{
		if (!m_stop) {
			try {
				tcp::socket *sock = new tcp::socket(m_service);
				m_acceptor.async_accept(*sock, boost::bind(&TcpServer::handleConnection, this, 
					sock, boost::asio::placeholders::error));
			}
			catch (std::exception e) {
				std::cout << "async accept exception: " << e.what() << std::endl;
			}
		}
	}

	void TcpServer::handleConnection(tcp::socket * socket, const boost::system::error_code & error)
	{
		if (!error) {
			std::cout << "connect success: " << socket->remote_endpoint().address().to_string() << std::endl;
			Session::ptr sess(new Session(socket));
			m_sesses.insert(sess);
			sess->registerDisconnect(boost::bind(&TcpServer::handleDisconnection, this, _1));
			sess->start();
		} else {
			delete socket;
		}

		// again
		accept();
	}

	void TcpServer::handleDisconnection(boost::shared_ptr<Session> sess)
	{
		m_sesses.erase(sess);
	}
}
