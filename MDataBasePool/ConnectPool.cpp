#include "ConnectPool.h"
#include <fstream>
#include <json.h>
using namespace Json;

ConnectPool* ConnectPool::getConnectionPool()
{
	static ConnectPool cp;

	return &cp;
}
//
shared_ptr<MysqlConnector> ConnectPool::getConnection()
{
	unique_lock<mutex> lock(this->lock);
	while (connectPool.empty()) {
		if (cv_status::timeout == cv.wait_for(lock, chrono::milliseconds(timeout))) {
			if (connectPool.empty()) {
				cout << "can't create new connect\n";
				return nullptr;
			}
		}
	}

	shared_ptr<MysqlConnector> conntion(connectPool.front(), [this](MysqlConnector* mc) {
		lock_guard<mutex> lock(this->lock);
		mc->refrashTime();
		connectPool.push(mc);
		});
	return conntion;
}
//
ConnectPool::~ConnectPool()
{
	while (!connectPool.empty()) {
		MysqlConnector* temp = connectPool.front();
		connectPool.pop();
		delete temp;
	}


}
//
ConnectPool::ConnectPool() {
	//1.加载json文件
	/*ip = "127.0.0.1";
	user = "root";
	passwd = "Ljk0068.";
	db = "db2";
	maxConnectCount = 20;
	minConnectCount = 5;
	timeout = 1000;
	maxIdleTime = 5000;*/
	if (!parseJsonFile()) {
		cout << "json file doesn't load\n";
		return;
	}


	//添加 数据库 链接
	for (int i = 0; i < minConnectCount; i++)
	{
		if (!addConnection()) {
			cout << "add connection fail\n";
			return;
		}
	}
	//开启多线程进行监听
	thread produce(&ConnectPool::produceConnect, this);
	thread recycle(&ConnectPool::recycleConnect, this);
	//将其与主线程分离
	produce.detach();
	recycle.detach();
}

bool ConnectPool::parseJsonFile()
{
	ifstream ifs("dbconfig.json");
	Reader r;
	Value v;
	r.parse(ifs, v);
	if (v.isObject()) {
		ip = v["ip"].asString();
		cout << ip << endl;
		user = v["user"].asString();
		cout << user << endl;
		passwd = v["passwd"].asString();
		cout << passwd << endl;
		db = v["db"].asString();
		cout << db << endl;
		maxConnectCount = v["maxConnectCount"].asInt();
		cout << maxConnectCount << endl;
		minConnectCount = v["minConnectCount"].asInt();
		cout << minConnectCount << endl;
		timeout = v["timeout"].asInt();
		cout << timeout << endl;
		maxIdleTime = v["maxIdleTime"].asInt();
		cout << maxIdleTime << endl;
		return true;
	}
	return false;
}


bool ConnectPool::addConnection()
{
	if (connectPool.size() < maxConnectCount) {
		MysqlConnector* temp = new MysqlConnector();
		if (temp->initConnect(move(ip), move(user), move(passwd), move(db), 3306)) {
			cout << "done add" << endl;
			temp->refrashTime();
			connectPool.push(temp);
			return true;
		}
		else {
			cout << "initConnect fail";
			return false;
		}
	}
	return false;
}
//
void ConnectPool::produceConnect()
{
	unique_lock<mutex> lock(this->lock);
	while (true) {
		cv.wait(lock, [=]() {
			return connectPool.size() < minConnectCount;
			});
		addConnection();
		cv.notify_all();
	}
}
//
void ConnectPool::recycleConnect()
{
	while (true) {
		this_thread::sleep_for(chrono::seconds(1));
		lock_guard<mutex> lock(this->lock);
		while (connectPool.size() > minConnectCount) {
			auto tempConnect =  connectPool.front();
			if (tempConnect->getAliveTime() >= maxIdleTime) {
				connectPool.pop();
				delete tempConnect;
			}
			else
				break;
		}
	}
}
