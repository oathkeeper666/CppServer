#include "Session.h"
#include "Pack.h"
#include "Buffer.h"
#include "TimeUtil.h"
#include <cstdlib>
#include <iostream>
#include <boost/asio/write.hpp>
#include <boost/bind.hpp>

const int TIME_OUT = 60;							// 可以等待心跳信号的最大时间
const size_t BUFF_SIZE = 512;						// 每次异步从socket中读取的数据长度
const size_t LEN_SIZE = 4;							// 存储长度的字节数
const size_t TYPE_SIZE = 4;							// 存储类型字段的字节数
const size_t HEADER_SIZE = LEN_SIZE + TYPE_SIZE;	// 包头部大小
const int MAX_PACK_LEN = 1024 * 10;					// 可以解析的最大包大小

#define HEADER_SIZE sizeof(struct PackHeader)

#define OPER_CHECK(f)	\
	if (f) {			\
		return;			\
	}

namespace bb {
	Session::Session(boost::shared_ptr<tcp::socket> & sock)
		: m_sock(sock)
		,m_close(false)
		,m_sessId(0)
	{
		m_buf.reset(new Buffer(BUFF_SIZE));
		m_lastHeartTime = TimeUtil::now();
		m_sock->non_blocking(true);
		m_sock->set_option(tcp::no_delay(true));
		m_sock->set_option(socket_base::keep_alive(true));
		m_sock->set_option(socket_base::linger(true, 0));
	}

	Session::~Session()
	{
	
	}

	void Session::start()
	{
		asyncRead(m_buf->peek(), m_buf->available());
	}

	void Session::asyncRead(void *begin, size_t n)
	{
		m_sock->async_receive(boost::asio::buffer(begin, n),
			boost::bind(&Session::readHandler, this, boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}

	void Session::sendData(const char * buffer, size_t n)
	{
		OPER_CHECK(m_close);
		boost::asio::async_write(*m_sock, boost::asio::buffer(buffer, n), 
			boost::bind(&Session::writeHandler, this, boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}

	void Session::sendData(const Pack::ptr & pack)
	{
		/*OPER_CHECK(m_close);
		assert(pack->full());

		Buffer::ptr buffer(new Buffer(HEADER_SIZE + pack->len()));
		size_t len = pack->len();
		buffer->writeData(&len, LEN_SIZE);
		int type = pack->type();
		buffer->writeData(&type, TYPE_SIZE);
		buffer->writeData(pack->begin(), pack->len());

		boost::asio::async_write(m_sock, boost::asio::buffer(buffer->begin(), buffer->size()),
			boost::bind(&Session::writeHandler, this, boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));*/
	}

	void Session::writeHandler(const boost::system::error_code & code, size_t transferred)
	{
		if (code) {
			std::cout << "write error: " << code.value() << std::endl;
			shutdown();
			return;
		}
		std::cout << "send data to peer success.";
	}

	void Session::partPacks()
	{
		void * p;
		size_t n;

		p = m_buf->begin();
		n = 0;
		while (m_buf->size() - n >= HEADER_SIZE) {
			PackHeader *header = reinterpret_cast<PackHeader *>(p);
			p = (char *)p + HEADER_SIZE;
			n += HEADER_SIZE;
			if (m_buf->size() - n < (size_t) header->len) {
				n -= HEADER_SIZE;
				break;
			}
			Pack::ptr pack(new Pack(header->cmd, header->len));
			pack->writeData(p, header->len);
			p = (char *)p + header->len;
			n += header->len;
			addPendingPack(pack);
		}

		m_buf->clearHead(n);
	}

	void Session::addPendingPack(const Pack::ptr & pack)
	{
		std::lock_guard<std::mutex> lock(m_pd_mtx);
		m_pending.push(pack);
	}

	void Session::readHandler(const boost::system::error_code & code, size_t transferred)
	{
		std::lock_guard<std::mutex> lock(m_sock_mtx);
		std::cout << "readHandler, code: " << code << ", transferred: " << transferred << std::endl;
		m_buf->offset(transferred);
		if (code) {
			std::cout << "read error: " << code.value() << std::endl;
			m_buf->clear();
			shutdown();
			return;
		}

		// 读取的数据很多，为了避免多次异步读取，扩大容量
		if (m_buf->full()) {
			m_buf->resize();
		}

		// part package
		partPacks();

		m_lastHeartTime = TimeUtil::now();
		asyncRead(m_buf->peek(), m_buf->available());

		/*if (!m_pack) {
			std::cout << "brach 1" << std::endl;
			// read pack len
			int len;
			memcpy(&len, m_buf->begin(), LEN_SIZE);
			if (len + HEADER_SIZE > transferred) {
				// warning
				m_buf->clear();
				shutdown();
				std::cout << "the pack size not enough." << std::endl;
				return;
			}
			if (len > MAX_PACK_LEN) {
				m_buf->clear();
				shutdown();
				std::cout << "the pack len is too bigger." << std::endl;
				return;
			}

			// read pack type
			int type;
			memcpy(&type, (char *)m_buf->begin() + LEN_SIZE, TYPE_SIZE);
			std::cout << "len, type = " << len << ", " << type << std::endl;

			m_pack.reset(new Pack(type, len));
			if (m_buf->size() - HEADER_SIZE > m_pack->len()) {
				std::cout << "brach 2" << std::endl;
				m_pack->writeData((char *)m_buf->begin() + HEADER_SIZE, len);
				m_buf->clearHead(HEADER_SIZE + len);
				asyncRead(m_buf->peek(), m_buf->available());
			}
			else {
				std::cout << "brach 3" << std::endl;
				m_pack->writeData((char *)m_buf->begin() + HEADER_SIZE, m_buf->size() - HEADER_SIZE);
				m_buf->clear();
				asyncRead(m_buf->peek(), m_buf->available());
			}
		}
		else {
			std::cout << "brach 4" << std::endl;
			size_t remain = m_pack->available();
			if (m_buf->size() > remain) {
				std::cout << "brach 5" << std::endl;
				m_pack->writeData(m_buf->begin(), remain);
				m_buf->clearHead(remain);
				asyncRead(m_buf->peek(), m_buf->available());
			}
			else {
				std::cout << "brach 6" << std::endl;
				m_pack->writeData(m_buf->begin(), m_buf->size());
				m_buf->clear();
				asyncRead(m_buf->peek(), m_buf->available());
			}
		}

		std::cout << "pack size = " << m_pack->size() << std::endl;
		std::cout << "pack len = " << m_pack->len() << std::endl;

		if (m_pack->full()) {
			std::cout << "receive data: " << std::endl;
			m_pending.push(m_pack);
			m_pack.reset();
		}*/
	}

	void Session::check()
	{
		if (TimeUtil::now() - m_lastHeartTime > TIME_OUT) {
			shutdown();
		}
	}

	void Session::shutdown()
	{
		// m_sock->shutdown(socket_base::shutdown_both);
		try {
			m_sock->close();
			m_close = true;
			m_sig(m_sessId);	// emit signal
		}
		catch (boost::system::system_error & code) {
			std::cout << "close socket error: " << code.what() << std::endl;
		}
	}

	signals2::connection Session::registerDisconnect(const signal_t::slot_type & subscriber)
	{
		return m_sig.connect(subscriber);
	}

	void Session::handlePacks()
	{
		std::lock_guard<std::mutex> lock(m_pd_mtx);
		while (!m_pending.empty()) {
			processMsg(m_pending.front());
			m_pending.pop();
		}
	}

	void Session::writePacks() 
	{

	}

	void Session::processMsg(const boost::shared_ptr<Pack> & pack)
	{

	}

}