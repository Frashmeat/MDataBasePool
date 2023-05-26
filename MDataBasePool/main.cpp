#include<iostream>
#include<threadpool.h>
#include"MysqlConnector.h"
using namespace std;
void func(int index) {
	for (int i = 0; i < index; i++)
	{
		cout << i << endl;
		this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

int main() {
	MysqlConnector mc;
	if (mc.initConnect("127.0.0.1", "root", "Ljk0068.", "db2", 3306))
		cout << "connected" << endl;
	string sql = "select * from customer where id = 4";
	if (mc.selectData(sql)) {
		cout << "sql done" << endl;
	}

	while (mc.next()) {
		cout << mc.value(1) << endl;
	}
	
	system("pause");
	return 0;
}