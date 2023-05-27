#include "ConnectPool.h"
#include <fstream>
#include <json.h>
using namespace Json;

ConnectPool* ConnectPool::getConnectionPool()
{
	//��̬����ֻ�ᴴ��һ��, ȷ�����ӳص�Ψһ��
	static ConnectPool cp;
	return &cp;
}

shared_ptr<MysqlConnector> ConnectPool::getConnection()
{
	unique_lock<mutex> lock(this->lock,defer_lock);
	//�����Ӳ���ʱ,֪ͨ ���� ����
	while (connectPool.empty()) {
		//����޷���timeout ʱ������������ ��ֱ��return , ���޷����� ����
		produceConnect();
		lock.lock();
		if (cv_status::timeout == cv.wait_for(lock, chrono::milliseconds(timeout))) {
			if (connectPool.empty()) {
				cout << "can't create new connect\n";
				return nullptr;
			}
		}
		lock.unlock();
	}

	//�����ӳض����л�ȡ ���� ��ָ�� share_ptr�� ��������,������������
	lock.lock();
	shared_ptr<MysqlConnector> conntion(connectPool.front(), [this](MysqlConnector* mc) {
		lock_guard<mutex> lock(this->lock);
		mc->refrashTime();
		connectPool.push(mc);
		});
	connectPool.pop();
	lock.unlock();
	return conntion;
}
ConnectPool::~ConnectPool()
{
	while (!connectPool.empty()) {
		MysqlConnector* temp = connectPool.front();
		connectPool.pop();
		delete temp;
	}


}
ConnectPool::ConnectPool() {
	//��ȡjson����,ָ�����ݿ�ĵ�ַ�û�����ȵ�
	if (!parseJsonFile()) {
		cout << "json file doesn't load\n";
		return;
	}
	//��� ���ݿ� ����
	for (int i = 0; i < minConnectCount; i++)
	{
		if (!addConnection()) {
			cout << "add connection fail\n";
			return;
		}
	}
	//�������߳̽��м���
	thread produce(&ConnectPool::produceConnect, this);
	thread recycle(&ConnectPool::recycleConnect, this);
	//���������̷߳���
	produce.detach();
	recycle.detach();
}

bool ConnectPool::parseJsonFile()
{
	ifstream ifs("dbconfig.json");
	Reader r;
	Value v;
	r.parse(ifs, v);
	if (v.isObject()) {
		ip = move(v["ip"].asString());
		cout << ip << endl;
		user = move(v["user"].asString());
		cout << user << endl;
		passwd = move(v["passwd"].asString());
		cout << passwd << endl;
		db = move(v["db"].asString());
		cout << db << endl;
		maxConnectCount = move(v["maxConnectCount"].asInt());
		cout << maxConnectCount << endl;
		minConnectCount = move(v["minConnectCount"].asInt());
		cout << minConnectCount << endl;
		timeout = move(v["timeout"].asInt());
		cout << timeout << endl;
		maxIdleTime = move(v["maxIdleTime"].asInt());
		cout << maxIdleTime << endl;
		return true;
	}
	return false;
}


bool ConnectPool::addConnection()
{
	if (connectPool.size() < maxConnectCount) {
		MysqlConnector* temp = new MysqlConnector();
		if (temp->initConnect(move(ip), move(user), move(passwd), move(db), 3306)) {
			cout << "done add" << endl;
			temp->refrashTime();
			connectPool.push(temp);
			return true;
		}
		else {
			cout << "initConnect fail";
			return false;
		}
	}
	return false;
}
//
void ConnectPool::produceConnect()
{
	while (connectPool.size() < minConnectCount) {
		{
			unique_lock<mutex> lock(this->lock);
			addConnection();
			cv.notify_all();
		}
	}
}
//
void ConnectPool::recycleConnect()
{
	while (true) {
		//cout << "done check\n";
		this_thread::sleep_for(chrono::milliseconds(1000));
		{
			lock_guard<mutex> lock(this->lock);
			while (connectPool.size() < minConnectCount) {
				if (!connectPool.empty()) {
					auto tempConnect = connectPool.front();
					if (tempConnect->getAliveTime() >= maxIdleTime) {
						cout << tempConnect->getAliveTime() << endl;
						connectPool.pop();
						delete tempConnect;
					}
					else
						break;
				}
			}
		}
	}
}
