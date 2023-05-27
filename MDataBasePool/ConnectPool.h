#pragma once
#include<iostream>
#include<queue>
#include<mutex>

#include"MysqlConnector.h"
using namespace std;
class __declspec(dllexport) ConnectPool
{
public:
	//��̬����, ȷ�� ֻ�� һ���̳߳�
	static ConnectPool* getConnectionPool();
	//ȡ��һ�� ����
	shared_ptr<MysqlConnector> getConnection();
	//��������͸�ֵ���춼delete ��ֹΨһ�Ա��ƻ�
	ConnectPool(const ConnectPool& obj) = delete;
	ConnectPool& operator=(const ConnectPool& obj) = delete;
	~ConnectPool();
private:
	//����˽�� ����ֱ��delete ��ֹ ֱ�Ӵ��� ����Ψһ��
	ConnectPool();

	//��json�ļ��л�ȡ����
	bool parseJsonFile();
	//��������ݿ������
	bool addConnection();
	//����������С��min ���� ������������ʱ �������� 
	void produceConnect();
	//����������������Ƿ����ʱ��̫�� 
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

