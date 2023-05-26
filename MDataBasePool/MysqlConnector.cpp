#include "MysqlConnector.h"

MysqlConnector::MysqlConnector()
{
	//初始化一个链接
	connect = mysql_init(nullptr);
	//设置字符编码 防止乱
	
}

MysqlConnector::~MysqlConnector()
{
	//关闭链接
	if (connect!=nullptr) {
		mysql_close(connect);
	}
	//释放结果集里的数据
	freeResult();
}

bool MysqlConnector::initConnect(string&& ip, string&& user, string&& passwd, string&& db, unsigned short&& port)
{
	//建立链接
	mysql_set_character_set(connect, "gbk");
	connect = mysql_real_connect(connect, ip.c_str(), user.c_str(), passwd.c_str(), db.c_str(), port, nullptr, 0);

	return connect!=nullptr;
}

bool MysqlConnector::updateData(string sql)
{
	//执行sql语句
	if (mysql_query(connect, sql.c_str())) {
		return false;
	}
	return true;
}

bool MysqlConnector::selectData(string sql)
{
	//释放之前的结果集
	freeResult();
	if (mysql_query(connect, sql.c_str())) {
		return false;
	}
	//储存结果集
	selectResult = mysql_store_result(connect);
	return true;
}

bool MysqlConnector::next()
{	//判断结果集是否存在
	if (selectResult) {
		//取出结果集中的一行
		resultRow = mysql_fetch_row(selectResult);
		//成功
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
	//返回当前行的 所有列的 数据的 长度的 集合
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
