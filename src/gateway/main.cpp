#include "framework/Server.h"
#include "framework/MathUtil.h"
#include "tiny/tinyxml2.h"
#include <iostream>
#include "framework/ConnectionPool.h"
#include "framework/Connection.h"
#include "gwconf.h"
#include "resource.h"
#include <unistd.h>

using namespace std;  

void test_random();
void test_random2();
void test_db_connection();

int main(int argc, char *argv[])
{
	// cmd flag
	bool isDaemon = false;
	std::string path = "../conf/conf.json";
	for (int i = 1; i < argc; i++) {
		if (std::strcmp("-d", argv[i])) {
			isDaemon = true;
		} else if (std::strcmp("-c", argv[i]) && i != argc - 1) {
			path = argv[i + 1];
		}
	}

	// load conf
	gateway::GwConf *conf = gateway::GwConf::instance();
	if (!conf->loadRes(path)) {
		exit(1);
	}

	// global resource
	gateway::Resource * res = gateway::Resource::instance();
	res->init(isDaemon);

	// daemon
	if (isDaemon) {
		daemon(1, 0);
	}

	/*boost::asio::io_service io_service;
	tcp::endpoint p(tcp::v4(), 9384);
	bb::TcpServer server(io_service, p);
	server.accept();
	io_service.run();*/

	return 0;
}

void test_random()
{
	for (int i = 0; i < 20; i++) {
		std::cout << "random num: " << bb::MathUtil::random(1, 100) << std::endl;
	}
}

void test_random2()
{
	float poss[] = { 0.25, 0.1, 0.05, 0.5, 0.1 };

	for (int i = 0; i < 100; i++) {
		std::cout << "random num: " << bb::MathUtil::random(poss, 5) << std::endl;
	}
}

/*void test_db_connection()
{
	bb::Connection::ptr conn = bb::ConnectionPool::instance()->getConnection();
	ResultSet *res = conn->query("select Host, user from user where user = ?;");
	while (res->next()) {
		int host = res->getInt(1);
		std::string user = res->getString(2);
		std::cout << "host, user: " << host << ", " << user << std::endl;
	}	
}*/