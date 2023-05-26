#include "MysqlConnector.h"

MysqlConnector::MysqlConnector()
{
	//��ʼ��һ������
	connect = mysql_init(nullptr);
	//�����ַ����� ��ֹ��
	
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

bool MysqlConnector::initConnect(string&& ip, string&& user, string&& passwd, string&& db, unsigned short&& port)
{
	//��������
	mysql_set_character_set(connect, "gbk");
	connect = mysql_real_connect(connect, ip.c_str(), user.c_str(), passwd.c_str(), db.c_str(), port, nullptr, 0);

	return connect!=nullptr;
}

bool MysqlConnector::updateData(string sql)
{
	//ִ��sql���
	if (mysql_query(connect, sql.c_str())) {
		return false;
	}
	return true;
}

bool MysqlConnector::selectData(string sql)
{
	//�ͷ�֮ǰ�Ľ����
	freeResult();
	if (mysql_query(connect, sql.c_str())) {
		return false;
	}
	//��������
	selectResult = mysql_store_result(connect);
	return true;
}

bool MysqlConnector::next()
{	//�жϽ�����Ƿ����
	if (selectResult) {
		//ȡ��������е�һ��
		resultRow = mysql_fetch_row(selectResult);
		//�ɹ�
		if (resultRow != nullptr) {
			return true;
		}
	}
	return false;
}

string MysqlConnector::value(int index)
{
	int allRowCount = mysql_num_fields(selectResult);
	if (index >= allRowCount || index < 0) {
		return string();
	}
	char* val = resultRow[index];
	//���ص�ǰ�е� �����е� ���ݵ� ���ȵ� ����
	auto length = mysql_fetch_lengths(selectResult)[index];

	return string(val,length);
}

bool MysqlConnector::transaction()
{
	return mysql_autocommit(connect,false);
}

bool MysqlConnector::commit()
{
	return mysql_commit(connect);
}

bool MysqlConnector::rollback()
{
	return mysql_rollback(connect);
}

void MysqlConnector::refrashTime()
{

}



void MysqlConnector::freeResult()
{
	if (selectResult) {
		mysql_free_result(selectResult);
		selectResult = nullptr;
		resultRow = nullptr;
	}
}
