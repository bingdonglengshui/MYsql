/*
 * DataBase.h
 *
 *  Created on: 2019?9?29?
 *      Author: chad
 */

#ifndef DATABASE_H_
#define DATABASE_H_
#include<iostream>
#include<memory>
#include<map>
#include<functional>
#include<list>
#include<string>
#include<sstream>
#include<mutex>
//mysql
#include<cppconn/connection.h>
#include<cppconn/driver.h>
#include<cppconn/statement.h>
#include<cppconn/prepared_statement.h>
#include<cppconn/metadata.h>
#include<cppconn/exception.h>
using namespace std;
using namespace sql;
typedef  unsigned int   uint32_t;
typedef  unsigned long  uint64_t;
typedef  unsigned char  uint8_t;
typedef  unsigned short uint16_t;
struct DbSetting
{
	string host;
	string username;
	string password;
	uint32_t connpoolsize;//连接池上限
	uint32_t reconnmaxsize;//重连次数


};
class DataBase:public enable_shared_from_this<DataBase>{
public:
protected:
	static shared_ptr<DataBase> m_database;
public:
	//创建
	static void Create(DbSetting setting);
	//销毁
	static void Destory();
	//获得实例
	static  shared_ptr<DataBase>GetInstance();
public:
	DataBase(DbSetting setting);
	virtual ~DataBase();
private:
	mutex m_mtDriver;
	mutex m_mtpool;
	DbSetting m_setting;
	Driver* m_driver;
	list<Connection*> m_disableConn;//未使用的连接
	list<Connection*> m_enableConn;//正在使用的连接
private:
	//初始化
	void Init();
	//获取一个连接
	Connection* Get();
	//释放一个连接
	void Release(Connection* &conn);
	//创建一个连接
	Connection* Create();
    //销毁一个连接
	void Destory(Connection*conn);
	//销毁所有连接
	void DestoryAll();
public:
	//封装数据库的操作
	ResultSet *executeQuery(const string& str);
	bool execute(const string& str);
};

#endif /* DATABASE_H_ */
