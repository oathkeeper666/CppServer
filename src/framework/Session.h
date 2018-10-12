#ifndef _SESSION_H_
#define _SESSION_H_

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signals2.hpp>
#include <boost/asio.hpp>
#include <queue>
#include <mutex>

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
		typedef signals2::signal<void (int)> signal_t;

		Session(boost::shared_ptr<tcp::socket> & sock);
		virtual ~Session();

		/*
			开启通信
		*/
		void start();

		/*
			将数据发给对端
		*/
		void sendData(const char * buffer, size_t n);
		void sendData(const boost::shared_ptr<Pack> & pack);

		bool isShutdown() const { return m_close; }
		void shutdown();
		void check();
		void setSessId(int id) { m_sessId = id; }
		int sessId() const { return m_sessId; }

		/* @detail 注册一个slot，在当前session close之后，执行subscriber
		 * @param slot
		 * @ret signal-slot connection
		*/
		signals2::connection registerDisconnect(const signal_t::slot_type & subscriber);

		/**
		 * @desc 处理数据包
		 */
		void handlePacks();

		/**
		 * @desc 发送数据包
		 */
		void writePacks();

	private:
		virtual void processMsg(const boost::shared_ptr<Pack> & pack);
		void writeHandler(const boost::system::error_code & error, size_t transferred);
		void readHandler(const boost::system::error_code & error, size_t transferred);
		void asyncRead(void * begin, size_t n);

		/**
		 * @desc 从buff中分割出数据包
		 */ 
		void partPacks();

		/**
		 * @desc 添加待处理的数据包
		 */
		void addPendingPack(const boost::shared_ptr<Pack> & pack);

	private:
		boost::shared_ptr<tcp::socket> m_sock;
		time_t m_lastHeartTime;
		bool m_close;
		int m_sessId;
		boost::shared_ptr<Buffer> m_buf;
		std::mutex m_pd_mtx;						// for pending
		std::queue<shared_ptr<Pack> > m_pending;	// waiting to handle
		std::queue<shared_ptr<Pack> > m_sending;	// waiting to sending
		signal_t m_sig;								// close signal
		std::mutex m_sock_mtx;						// for socket
	};
}

#endif 

