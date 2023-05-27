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
	//���ݿ����ӵĳ�ʼ��
	bool initConnect(string&& ip,string&& user,string&& passwd,string&& db,unsigned short&& port);
	//��ɾ��
	/*bool deleteData();
	bool updateData();
	bool addData();*/
	bool updateData(string sql);
	//��ѯ����
	bool selectData(string sql);
	//��ѯ������Խ�������в���
	bool next(); //�ƶ����������ָ��
	string value(int index); //���е����ݽ��л�ȡ
	//����������
	//��ʼ������
	bool transaction();
	//�ύ����
	bool commit();
	//����ع�
	bool rollback();
	//��������ʱ��
	void refrashTime();
	//�������ʱ��
	long long getAliveTime();
private:
	//���ڻ��ղ�ѯ��������ڴ�
	void freeResult();
	//�����ݿ����ӵ���
	MYSQL* connect = nullptr;
	//��ѯ�Ľ����
	MYSQL_RES* selectResult = nullptr;
	//�������һ��
	MYSQL_ROW resultRow = nullptr;
	std::chrono::steady_clock::time_point startActiveTime;
};

