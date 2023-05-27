#pragma once
#include<iostream>
#include<queue>
#include<mutex>

#include"MysqlConnector.h"
using namespace std;
class __declspec(dllexport) ConnectPool
{
public:
	//静态方法, 确保 只有 一个线程池
	static ConnectPool* getConnectionPool();
	//取出一个 连接
	shared_ptr<MysqlConnector> getConnection();
	//拷贝构造和赋值构造都delete 防止唯一性被破坏
	ConnectPool(const ConnectPool& obj) = delete;
	ConnectPool& operator=(const ConnectPool& obj) = delete;
	~ConnectPool();
private:
	//放入私有 或者直接delete 防止 直接创建 打破唯一性
	ConnectPool();

	//从json文件中获取数据
	bool parseJsonFile();
	//添加与数据库的连接
	bool addConnection();
	//当连接数量小于min 或者 连接数量不够时 生成连接 
	void produceConnect();
	//监听被加入的连接是否空闲时间太长 
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

