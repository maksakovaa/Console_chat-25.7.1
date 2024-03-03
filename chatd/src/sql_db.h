#pragma once
#include "MsgBased.h"

extern Settings* Config;
extern UsrBase* UserBase;
extern MsgBase* AllMsgBase;
class SQL_DB
{
public:
	SQL_DB();
	~SQL_DB();
	void getConfig();
	void firstRun();
	void sendRequest(const string& request);
	void getRequest(const string& request);
	void getUserBase();
	void getMsgBase();
	void delUser(int id);
	void regUser(User& newUser);
	void chgPwd(const string& uuid, const string& pwd);
	void regPrivateMsg(Message& newMsg);
	void regAllMsg(Message& newMsg);
private:
	MYSQL mysql;
	MYSQL_RES* result;
	MYSQL_ROW row;
	string m_query;
	string mysql_srv_ip;
	string mysql_login;
	string mysql_pass;
	string mysql_database;
	string mysql_table_users;
	string mysql_table_messages;
	string mysql_table_private_msg;
};