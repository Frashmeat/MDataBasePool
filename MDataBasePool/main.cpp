#include<iostream>
#include"ConnectPool.h"
#include"MysqlConnector.h"
#include<json.h>
#include <fstream>
#include<threadpool.h>
using namespace std;
using namespace Json;

int main() {
	
	/*MysqlConnector mc;
	if (mc.initConnect("127.0.0.1", "root", "Ljk0068.", "db2", 3306)) {
		cout << "connect" << endl;
	};
	string sql = "select * from customer where id = 4";
	mc.selectData(sql);
	mc.next();
	cout << mc.value(1) << endl;*/
	/*ConnectPool* pool = ConnectPool::getConnectionPool();
	auto mc = pool->getConnection();
	string sql = "select * from customer where id = 4";
	mc.get()->selectData(sql);
	mc.get()->next();
	cout << mc.get()->value(1) << endl;*/
	//{
	//	threadpool tp(6);
	//	ConnectPool* pool = ConnectPool::getConnectionPool();



	//
	//}
	/*ifstream ifs("dbconfig.json");
	Reader r;
	Value v;
	r.parse(ifs, v);
	if (v.isObject()) {
		cout << v["ip"].asString() << endl;
	}*/

	ConnectPool* cp = ConnectPool::getConnectionPool();
	//auto connect = cp->getConnection();
	//auto connect2 = cp->getConnection();
	//auto connect3 = cp->getConnection();
	//auto connect4 = cp->getConnection();
	//auto connect5 = cp->getConnection();
	threadpool tp(10);
	auto f1 = tp.commit(bind(&ConnectPool::getConnection,cp));
	auto f2 = tp.commit(bind(&ConnectPool::getConnection,cp));
	auto f3 = tp.commit(bind(&ConnectPool::getConnection,cp));
	auto f4 = tp.commit(bind(&ConnectPool::getConnection,cp));
	auto f5 = tp.commit(bind(&ConnectPool::getConnection,cp));

	//this_thread::sleep_for(chrono::milliseconds(6000));
	string sql = "select * from customer where id = 4";
	auto con = f4.get().get();
	auto con1 = f2.get().get();
	con->selectData(sql);
	con1->selectData(sql);
	con->next();
	con1->next();
	cout << con->value(1) << endl;
	cout << con1->value(1) << endl;




	system("pause");
	return 0;
}