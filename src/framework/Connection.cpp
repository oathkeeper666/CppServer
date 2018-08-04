#include "Connection.h"

namespace bb {
    Connection::Connection(sql::Connection *conn)
        : m_conn(conn)
    {
        m_conn->setSchema("mysql");
    }

    Connection::~Connection()
    {
        if (!m_conn->isClosed()) {
            m_conn->close();
        }
    }

    void Connection::close()
    {
        m_conn->close();
    }

    /*ResultSet *Connection::query(const std::string & sql)
    {
        PreparedStatement *statement = m_conn->prepareStatement(sql);
        statement->setString(1, "wubang");
        return statement->executeQuery();
    }*/

    ResultSet *Connection::query(const std::string & sql, const std::initializer_list<boost::any> & args)
    {
        PreparedStatement *statement = m_conn->prepareStatement(sql);
        unsigned int i = 1;
        for (std::initializer_list<boost::any>::const_iterator it = args.cbegin();
            it != args.cend(); it++) {
            switch ((*it)->type()) {
            case typeid(bool):
                statement->setBoolean(i);
                break;
            case typeid(double):
                statement->
            case typeid(int):

            case typeid(unsigned int):

            case typeid(long long):

            case typeid(unsigned long long):


            }
        }
    }
}