#include "gateway.h"

void test_random();
void test_random2();
void test_db_connection();

class GateApp : public bb::Application
{
public:
	GateApp(int argc, char *argv[]) 
		: bb::Application(argc, argv)
	{

	}
	~GateApp() {}

	void initLogger(gateway::GwConf *conf)
	{
		bb::LogConfig info;
		info.m_module = "gateway";
    	info.m_path = conf->logPath();
    	info.m_level = 1;
    	info.m_daemon = isDaemon();
    	info.m_flush_sec = 10;
		bb::Logger * logger = bb::Singleton<bb::Logger>::instance();
		logger->init(info);
	}

	void run() override 
	{
		// load conf
		std::string conf_path = "../conf/conf.json";
		if (path() != "") {
			conf_path = path();
		}
		gateway::GwConf *conf = bb::Singleton<gateway::GwConf>::instance();
		if (!conf->loadRes(conf_path)) {
			exit(1);
		}

		// init logger
		initLogger(conf);

		// global resource
		gateway::Resource * res = bb::Singleton<gateway::Resource>::instance();
		if (!res->load()) {
			exit(1);
		}

		bb::Application::run();
	}

private:

};

int main(int argc, char *argv[])
{
	GateApp app(argc, argv);
	app.run();
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