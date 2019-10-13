/*
 * DataBase.cpp
 *
 *  Created on: 2019?9?29?
 *      Author: chad
 */

#include "DataBase.h"
shared_ptr<DataBase> DataBase::m_database = nullptr;
void DataBase::Create(DbSetting setting) {
	try {
		m_database = make_shared < DataBase > (setting);
		m_database->Init();

	} catch (const exception&e) {
		stringstream os;
		os << "[db]" << e.what();
		throw runtime_error(os.str());
	}
}
shared_ptr<DataBase> DataBase::GetInstance() {
	if (m_database != nullptr) {
		return m_database;
	} else {
		DbSetting setting;
		setting.host = "tcp://127.0.0.1:3306/test";
		setting.password = "1234";
		setting.username = "root";
		setting.connpoolsize = 5;
		setting.reconnmaxsize = 2;
		Create(setting);
		return m_database;
	}
}
void DataBase::Destory() {
	m_database.reset();
}
DataBase::DataBase(DbSetting setting) {
	// TODO Auto-generated constructor stub
	m_setting = setting;
}

DataBase::~DataBase() {
	// TODO Auto-generated destructor stub
	DestoryAll();
}
void DataBase::Init() {
	lock_guard < mutex > lck(m_mtDriver);
	m_driver = get_driver_instance();
}
Connection*DataBase::Get() {
	lock_guard < mutex > lck(m_mtpool);
	Connection*conn = nullptr;
	try {
		uint32_t reconnCnt = 0;
		do {
			if (0 < m_disableConn.size()) {
				conn = m_disableConn.front(); //获取连接
				m_disableConn.pop_front(); //未使用的连接
			} else {
				conn = Create(); //创建连接
			}
			if (nullptr != conn && conn->isValid()) {
				//连接有效
				m_enableConn.push_back(conn); //加入使用连接池
				break;
			}
			//连接无效 销毁
			Destory(conn);
			//重连次数增加
			reconnCnt++;
		} while (reconnCnt < m_setting.reconnmaxsize); //重连次数上限
		if (nullptr == conn) {
			//获取到无效连接 抛出连接
			throw runtime_error("[db] connection is invaild");
		}
	} catch (const exception&e) { //销毁连接
		Destory(conn);
		stringstream os;
		os << "[db]" << e.what();
		throw runtime_error(os.str());

	}
	return conn;
}
void DataBase::Release(Connection*&conn) {
	lock_guard < mutex > lck(m_mtpool);
	if (nullptr == conn) {
		return;
	}
	if (m_disableConn.size() >= m_setting.connpoolsize) {
		//达到上限 删除
		Destory(conn);
		return;
	}

	if (!conn->isValid()) {
		Destory(conn);
		return;
	}


	for(auto it=m_enableConn.begin();it!=m_enableConn.end();)
	{
		if(*it==conn)
		{
			m_enableConn.erase(it);
			break;
		}
	}
	cout<<"Release conn "<<conn<<endl;
	m_disableConn.push_back(conn);
}
Connection*DataBase::Create() {
	lock_guard < mutex > lck(m_mtDriver);
	Connection*conn = nullptr;
	try {
		conn = m_driver->connect(m_setting.host, m_setting.username,
				m_setting.password);

	} catch (const exception&e) {
		stringstream os;
		os << "[db]" << e.what();
		throw runtime_error(os.str());
	}
	return conn;
}
void DataBase::Destory(Connection*conn) {
	cout << "Destory start" << endl;
	if (nullptr == conn)

	{
		return;
	}
	conn = nullptr;
}
void DataBase::DestoryAll() {
	list<Connection*> temp1;
	list<Connection*> temp2;
	{
		lock_guard < mutex > lck(m_mtpool);
		m_disableConn.swap(temp1);
		m_enableConn.swap(temp2);
	}
	cout<<"temp1.size  "<<temp1.size()<<endl;
	cout<<"temp2.size  "<<temp2.size()<<endl;
	for(auto a:temp1)
	{
		a->close();
		cout<<"delete a conn "<<a<<endl;
		delete a;
		a=nullptr;
		cout<<"delete"<<endl;
		cout<<"delete a conn "<<a<<endl;
	}
	temp1.clear();
	cout<<"temp1.size  "<<temp1.size()<<endl;
	for(auto b:temp2)
	{
		cout<<b<<endl;
		b->close();
		cout<<"deletebbb"<<endl;
				delete b;
				b=nullptr;
				cout<<"deletebbb"<<endl;
	}

}
ResultSet *DataBase::executeQuery(const string& str) {
	try {
		shared_ptr < Connection> conn(Get(), [this](Connection* ptr) {Release(ptr);});
		cout<<"coutn"<<conn.use_count()<<endl;
		shared_ptr<Statement*> stmt = make_shared<Statement*>(conn->createStatement());
		return (*stmt)->executeQuery(str);
	} catch (const exception& e) {
		stringstream os;
		os << "[db]" << e.what();
		throw runtime_error(os.str());
	}
}
bool  DataBase::execute(const string& str) {
	try {
			shared_ptr < Connection> conn(Get(), [this](Connection* ptr) {Release(ptr);});
			shared_ptr<Statement*> stmt = make_shared<Statement*>(conn->createStatement());
			return (*stmt)->execute(str);
		} catch (const exception& e) {
			stringstream os;
			os << "[db]" << e.what();
			throw runtime_error(os.str());
		}

}
