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
	//���ӳ�
	queue<MysqlConnector*> connectPool;
	//������
	mutex lock;
	//��������
	condition_variable cv;
	//���/��С����
	int maxConnectCount;
	int minConnectCount;
	//��ʱϵͳ
	int timeout;
	int maxIdleTime;
};

