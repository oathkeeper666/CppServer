#ifndef _SESSION_H_
#define _SESSION_H_

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signals2.hpp>
#include <boost/asio.hpp>
#include <list>

using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

namespace bb {
	class Pack;
	class Buffer;

	class Session : public enable_shared_from_this<Session>
	{
	public:
		typedef boost::shared_ptr<Session> ptr;
		typedef signals2::signal<void (Session::ptr)> signal_t;

		Session(tcp::socket * sock);
		virtual ~Session();

		/*
			开启通信
		*/
		void start();

		/*
			将数据发给对端
		*/
		void sendData(const char * buffer, size_t n);
		void sendData(const shared_ptr<Pack> & pack);

		bool isShutdown() const { return m_close; }
		void shutdown();
		void check();

		/* @detail 注册一个slot，在当前session close之后，执行subscriber
		 * @param slot
		 * @ret signal-slot connection
		*/
		signals2::connection registerDisconnect(const signal_t::slot_type & subscriber);

	private:
		virtual void processMsg() {}
		void writeHandler(const boost::system::error_code & error, size_t transferred);
		void readHandler(const boost::system::error_code & error, size_t transferred);
		void asyncRead(void * begin, size_t n);

	private:
		tcp::socket *m_sock;
		time_t m_lastHeartTime;
		bool m_close;
		shared_ptr<Buffer> m_buf;
		shared_ptr<Pack> m_pack;				// incomplete pack
		std::list<shared_ptr<Pack> > m_pending;	// waiting to handle
		signal_t m_sig;							// close signal
	};
}

#endif 

