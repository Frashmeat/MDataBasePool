#pragma once
#include <string>
#include "mysql.h"
using namespace std;
class MysqlConnector
{
public:
	MysqlConnector();
	~MysqlConnector();
	//���ݿ����ӵĳ�ʼ��
	bool initConnect();
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
private:
	//���ڻ��ղ�ѯ��������ڴ�
	void freeResult();
	//�����ݿ����ӵ���
	MYSQL* connect = nullptr;
	//��ѯ�Ľ����
	MYSQL_RES* selectResult = nullptr;
	//�������һ��
	MYSQL_ROW* resultRow = nullptr;

};

