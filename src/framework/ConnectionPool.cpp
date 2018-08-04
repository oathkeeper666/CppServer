#include "ConnectionPool.h"
#include "Connection.h"
#include <boost/bind.hpp>
#include <iostream>

namespace bb {
    ConnectionPool::ConnectionPool(size_t max)
        : m_max(max)
    {
        m_driver = sql::mysql::get_driver_instance();
    }

    ConnectionPool::~ConnectionPool()
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_busyConns.clear();
        m_freeConns.clear();
    }

    ConnectionPool::ptr ConnectionPool::instance()
    {
        static ConnectionPool::ptr instance(new ConnectionPool(5));
        return instance;
    }

    Connection::ptr ConnectionPool::getConnection()
    {
        std::unique_lock<std::mutex> lock(m_mtx);
        if (m_freeConns.empty() && m_busyConns.size() < m_max) {
            sql::Connection *conn = m_driver->connect("tcp://127.0.0.1:3306", "wubang", "123456");
            if (conn) {
                std::cout << "connect db success !!!" << std::endl;
                m_freeConns.push_back(Connection::ptr(new Connection(conn)));
            }
        }

        while (m_freeConns.empty()) {
            m_cond.wait(lock, [&]() {
                return !m_freeConns.empty();
            });
        }

        Connection::ptr conn = *m_freeConns.begin();
        m_freeConns.erase(m_freeConns.begin());
        m_busyConns[conn.get()] = conn;

        Connection::ptr ret(conn.get(), boost::bind(&ConnectionPool::restoreConnection, shared_from_this(), _1));

        return ret;
    }

    void ConnectionPool::restoreConnection(Connection * conn)
    {
        std::unique_lock<std::mutex> lock(m_mtx);
        std::cout << "restoreConnection ... " << std::endl;
        std::map<void *, Connection::ptr>::iterator it = m_busyConns.find(conn);
        assert(it != m_busyConns.end());
        m_freeConns.push_back(it->second);
        m_busyConns.erase(it);
        m_cond.notify_one();
    }
}