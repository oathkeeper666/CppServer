#ifndef _CONNECTION_POOL_H_
#define _CONNECTION_POOL_H_

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <jdbc/mysql_driver.h>
#include <condition_variable>
#include <list>
#include <map>
#include <mutex>

namespace bb {
    class Connection;

    // template<typename T, U>

    class ConnectionPool : public boost::noncopyable, public boost::enable_shared_from_this<ConnectionPool>
    {
        public:
            typedef boost::shared_ptr<ConnectionPool> ptr;

            static ConnectionPool::ptr instance();
            ~ConnectionPool();

            boost::shared_ptr<Connection> getConnection();

        private:
            ConnectionPool(size_t max = 10);  
            void restoreConnection(Connection * conn);

        private:
            std::mutex m_mtx;
            std::condition_variable m_cond;
            std::map<void *, boost::shared_ptr<Connection> > m_busyConns;
            std::list<boost::shared_ptr<Connection> > m_freeConns;
            sql::mysql::MySQL_Driver *m_driver;
            size_t m_max;
            size_t m_count;
    };
}

#endif // _CONNECTION_POOL_H_