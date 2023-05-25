#include "MysqlConnector.h"

MysqlConnector::MysqlConnector()
{
	//初始化一个链接
	connect = mysql_init(nullptr);
	//设置字符编码 防止乱码
	mysql_set_character_set(connect, "utf8");
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



void MysqlConnector::freeResult()
{
	if (selectResult) {
		mysql_free_result(selectResult);
		selectResult = nullptr;
		resultRow = nullptr;
	}
}
