#pragma once
#include<iostream>
#include<queue>
#include<mutex>

#include"MysqlConnector.h"
using namespace std;
class __declspec(dllexport) ConnectPool
{
public:
	static ConnectPool* getConnectionPool();
	shared_ptr<MysqlConnector> getConnection();
	ConnectPool(const ConnectPool& obj) = delete;
	ConnectPool& operator=(const ConnectPool& obj) = delete;
	~ConnectPool();
private:
	ConnectPool();
	bool parseJsonFile();
	bool addConnection();
	void produceConnect();
	void recycleConnect();
	string ip;
	string user;
	string passwd;
	string db;
	unsigned short port;
	//连接池
	queue<MysqlConnector*> connectPool;
	//互斥锁
	mutex lock;
	//条件变量
	condition_variable cv;
	//最大/最小容量
	int maxConnectCount;
	int minConnectCount;
	//计时系统
	int timeout;
	int maxIdleTime;
};

