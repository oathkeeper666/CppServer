#include "framework/Server.h"
#include "framework/MathUtil.h"
#include "tiny/tinyxml2.h"
#include <iostream>
#include <boost/uuid/uuid.hpp>  
#include <boost/uuid/uuid_generators.hpp>  
#include <boost/uuid/uuid_io.hpp>  

using namespace boost::uuids;  
using namespace std;  

void test_random();
void test_random2();

int main()
{
	boost::asio::io_service io_service;

	tcp::endpoint p(tcp::v4(), 9384);
	bb::TcpServer server(io_service, p);
	server.accept();

	io_service.run();
	test_random2();

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