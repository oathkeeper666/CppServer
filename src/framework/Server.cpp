#include "Server.h"
#include "Session.h"
#include "ThreadBase.h"
#include "WorkThread.h"
#include "IoThread.h"
#include "TimeUtil.h"
#include <boost/bind.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/make_shared.hpp>
#include <iostream>

namespace bb {
	TcpServer::TcpServer(const ServerConf & conf)
		: m_service()
		,m_acceptor(m_service)
		,m_socket(m_service)
		,m_sess_id(0)
		,m_conf(conf)
	{
		// set socket option
		tcp::endpoint endpoint(m_conf.ipv6 ? ip::tcp::v6() : ip::tcp::v4(), m_conf.port);
		m_acceptor.open(endpoint.protocol());
		m_acceptor.set_option(tcp::acceptor::reuse_address(true));
		if (m_conf.ipv6) {
			m_acceptor.set_option(ip::v6_only(false));
		}
		m_acceptor.bind(endpoint);
		m_acceptor.listen();

		// init work thread
		m_work_thread.reset(new WorkThread(static_cast<IRunnable *>(this)));

		// init IO thread
		m_io_threads.resize(m_conf.io_count);
		for (size_t i = 0; i < m_conf.io_count; i++) {
			m_io_threads.push_back(boost::make_shared<IoThread>(&m_service));
		}
	}

	TcpServer::~TcpServer()
	{
		
	}

	void TcpServer::aync_accept()
	{
		try {
			//boost::shared_ptr<tcp::socket> sock = boost::make_shared<tcp::socket>(m_service);
			boost::shared_ptr<tcp::socket> sock(new tcp::socket(m_service));
			m_acceptor.async_accept(m_socket, boost::bind(&TcpServer::handleConnection, this, 
			sock, boost::asio::placeholders::error));
		}
		catch (std::exception e) {
			std::cout << "async accept exception: " << e.what() << std::endl;
		}
	}

	void TcpServer::run() 
	{
		m_work_thread->run();
		for (IoThread::ptr & th : m_io_threads) {
			th->run();
		}
		m_service.run();
	}

	void TcpServer::stop()
	{
		m_work_thread->stop();
		for (IoThread::ptr & th : m_io_threads) {
			th->stop();
		}
		if (!m_service.stopped()) {
			m_service.stop();
		}
	}

	void TcpServer::saveSession(boost::shared_ptr<tcp::socket> & sock)
	{
		std::lock_guard<std::mutex> lock(m_sess_mtx);
		Session::ptr sess(new Session(sock));
		++m_sess_id;
		sess->setSessId(m_sess_id);
		m_sesses[sess->sessId()] = sess;
		sess->registerDisconnect(boost::bind(&TcpServer::handleDisconnection, this, _1));
		sess->start();
	}

	void TcpServer::handleConnection(boost::shared_ptr<tcp::socket> & sock, const boost::system::error_code & error)
	{
		if (error) {
			std::cout << "accept failed: " << error.message() << std::endl;
			sock->close();	
			return;
		} 
		std::cout << sock->remote_endpoint().address().to_string() << "connect success." << std::endl;
		{
			std::lock_guard<std::mutex> lock(m_accept_mxt);
			m_accept_socks.push(sock);
		}
		
		// again
		aync_accept();
	}

	void TcpServer::handleDisconnection(int sessId)
	{
		std::lock_guard<std::mutex> lock(m_sess_mtx);
		m_sesses.erase(sessId);
	}

	void TcpServer::update()
	{
		g_now = TimeUtil::now();

		// handler connected socket
		{
			std::lock_guard<std::mutex> lock(m_accept_mxt);
			while (!m_accept_socks.empty()) {
				saveSession(m_accept_socks.front());
				m_accept_socks.pop();
			}
		}
		
		// session handle message & session write message
		{
			std::lock_guard<std::mutex> lock(m_sess_mtx);
			std::map<int, boost::shared_ptr<Session> >::const_iterator it = m_sesses.cbegin();
			for (; it != m_sesses.cend(); it++) {
				it->second->handlePacks();
				it->second->writePacks();
			}
		}

		// logic update

	}
}
