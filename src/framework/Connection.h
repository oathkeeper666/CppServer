#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>
#include <jdbc/cppconn/connection.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <initializer_list>
#include <string>

using namespace sql;

namespace bb {
    class Connection 
    {
        public:
            typedef boost::shared_ptr<Connection> ptr;

            Connection(sql::Connection *conn);
            ~Connection();

            void close();

            ResultSet *query(const std::string & sql, const std::initializer_list<boost::any> & args);

            template<typename T, typename T2>
            ResultSet *query()

        private:
            sql::Connection *m_conn;
    };
}


#endif // _CONNECTION_H_