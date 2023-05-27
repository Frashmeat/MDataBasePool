#pragma once
#include <string>
#include <chrono>
#include "mysql.h"
using namespace std;

class __declspec(dllexport) MysqlConnector
{
public:
	MysqlConnector();
	~MysqlConnector();
	//数据库链接的初始化
	bool initConnect(string&& ip,string&& user,string&& passwd,string&& db,unsigned short&& port);
	//增删改
	/*bool deleteData();
	bool updateData();
	bool addData();*/
	bool updateData(string sql);
	//查询数据
	bool selectData(string sql);
	//查询结束后对结果集进行操作
	bool next(); //移动结果集的行指针
	string value(int index); //对行的数据进行获取
	//事务管理操作
	//初始化事务
	bool transaction();
	//提交事务
	bool commit();
	//事务回滚
	bool rollback();
	//更新启动时间
	void refrashTime();
	//计算持续时间
	long long getAliveTime();
private:
	//用于回收查询结果集的内存
	void freeResult();
	//与数据库链接的类
	MYSQL* connect = nullptr;
	//查询的结果集
	MYSQL_RES* selectResult = nullptr;
	//结果集的一行
	MYSQL_ROW resultRow = nullptr;
	std::chrono::steady_clock::time_point startActiveTime;
};

