#include "MysqlConnector.h"

MysqlConnector::MysqlConnector()
{
	//��ʼ��һ������
	connect = mysql_init(nullptr);
	//�����ַ����� ��ֹ����
	mysql_set_character_set(connect, "utf8");
}

MysqlConnector::~MysqlConnector()
{
	//�ر�����
	if (connect!=nullptr) {
		mysql_close(connect);
	}
	//�ͷŽ�����������
	freeResult();
}



void MysqlConnector::freeResult()
{
	if (selectResult) {
		mysql_free_result(selectResult);
		selectResult = nullptr;
		resultRow = nullptr;
	}
}
