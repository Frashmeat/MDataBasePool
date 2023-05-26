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
	//���ӳ�
	queue<MysqlConnector> connectPool;
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

