#include "Server.h"
#include "Session.h"
#include <boost/bind.hpp>
#include <boost/circular_buffer.hpp>
#include <iostream>

namespace bb {
	TcpServer::TcpServer(io_service & service, ip::tcp::endpoint & endPoint)
		: m_service(service)
		,m_acceptor(service, endPoint)
		,m_socket(service)
		,m_stop(false)
	{

	}

	TcpServer::~TcpServer()
	{
	}

	void TcpServer::accept()
	{
		if (!m_stop) {
			try {
				//tcp::socket *sock = new tcp::socket(m_service);
				m_acceptor.async_accept(m_socket, boost::bind(&TcpServer::handleConnection, this, 
					boost::asio::placeholders::error));
			}
			catch (std::exception e) {
				std::cout << "async accept exception: " << e.what() << std::endl;
			}
		}
	}

	void TcpServer::handleConnection(const boost::system::error_code & error)
	{
		if (!error) {
			std::cout << "connect success: " << m_socket.remote_endpoint().address().to_string() << std::endl;
			Session::ptr sess(new Session(m_socket));
			m_sesses.insert(sess);
			sess->registerDisconnect(boost::bind(&TcpServer::handleDisconnection, this, _1));
			sess->start();
		} 

		// again
		accept();
	}

	void TcpServer::handleDisconnection(boost::shared_ptr<Session> sess)
	{
		m_sesses.erase(sess);
	}
}
