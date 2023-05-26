#pragma once
#include<iostream>
#include<queue>
#include<mutex>
#include"MysqlConnector.h"
using namespace std;
class ConnectPool
{
public:
	ConnectPool();
	~ConnectPool();
private:
	string ip;
	string user;
	string passwd;
	string db;
	unsigned short port;
	//连接池
	queue<MysqlConnector> connectPool;
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

