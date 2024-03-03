#include "sql_db.h"

SQL_DB::SQL_DB()
{
	getConfig();
	mysql_init(&mysql);
	if (&mysql == NULL)
	{
		cout << "[" << curDateTime() << "] " << "[MySQL] ERROR: Ошибка создания MySQL дескриптора" << endl;
	}

	if (!mysql_real_connect(&mysql, mysql_srv_ip.data(), mysql_login.data(), mysql_pass.data(), NULL, 0, NULL, 0))
	{
		cout << "[" << curDateTime() << "] " << "[MySQL] ERROR: Ошибка подключения к базе данных " << mysql_error(&mysql) << endl;
	}
	else
	{
		cout << "[" << curDateTime() << "] " << "[MySQL] Соединение установлено." << endl;
	}
	mysql_set_character_set(&mysql, "utf8");
	firstRun();
}

SQL_DB::~SQL_DB()
{
	mysql_close(&mysql);
	cout << "[" << curDateTime() << "] " "[MySQL] Соединение закрыто." << endl;
}

void SQL_DB::getConfig()
{
	mysql_srv_ip = Config->getMysqlSrvIP();
	mysql_login = Config->getMysqlLogin();
	mysql_pass = Config->getMysqlPass();
	mysql_database = Config->getMysql_database();
	mysql_table_users = Config->getMysql_table_users();
	mysql_table_messages = Config->getMysql_table_messages();
	mysql_table_private_msg = Config->getMysql_table_private_msg();
}

void SQL_DB::firstRun()
{
	m_query = "CREATE DATABASE IF NOT EXISTS " + mysql_database;
	sendRequest("CREATE DATABASE");

	m_query = "USE " + mysql_database;
	sendRequest("USE DATABASE");

	m_query = "CREATE TABLE IF NOT EXISTS " + mysql_table_users + " ";
	m_query += "(id SERIAL NOT NULL,";
	m_query += "uuid BINARY(16) PRIMARY KEY DEFAULT(UUID_TO_BIN(UUID())), ";
	m_query += "name VARCHAR(100) NOT NULL, ";
	m_query += "login VARCHAR(100) UNIQUE NOT NULL, ";
	m_query += "pwd VARCHAR(200), ";
	m_query += "deleted INTEGER NOT NULL DEFAULT 0)";
	sendRequest("CREATE TABLE USERS");

	m_query = "CREATE TABLE IF NOT EXISTS " + mysql_table_messages;
	m_query += " (id SERIAL PRIMARY KEY, ";
	m_query += "msg_from BINARY(16), ";
	m_query += "date DATETIME NOT NULL, ";
	m_query += "message VARCHAR(255) NOT NULL,";
	m_query += "FOREIGN key (msg_from) REFERENCES " + mysql_table_users + "(uuid))";
	sendRequest("CREATE TABLE ALL_MSGBASE");

	m_query = "CREATE TABLE IF NOT EXISTS " + mysql_table_private_msg;
	m_query += " (id SERIAL PRIMARY KEY, ";
	m_query += "msg_from BINARY(16), ";
	m_query += "msg_to BINARY(16), ";
	m_query += "date DATETIME NOT NULL, ";
	m_query += "message VARCHAR(255) NOT NULL,";
	m_query += "FOREIGN key (msg_from) REFERENCES " + mysql_table_users + "(uuid),";
	m_query += "FOREIGN key (msg_to) REFERENCES " + mysql_table_users + "(uuid))";
	sendRequest("CREATE TABLE PRIVATE_MSGBASE");

	getUserBase();
	getMsgBase();
}

void SQL_DB::getRequest(const string& request)
{
	mysql_query(&mysql, m_query.data());
	if (result = mysql_store_result(&mysql))
	{
		cout << "[" << curDateTime() << "] " << "[MySQL] Запрос " << request << " выполнен успешно." << endl;
	}
	else
	{
		cout << "[" << curDateTime() << "] " << "[MySQL] Ошибка MySQL " << mysql_error(&mysql) << endl;
	}
}

void SQL_DB::sendRequest(const string& request)
{
	int query = mysql_query(&mysql, m_query.data());
	if (query == 0)
	{
		cout << "[" << curDateTime() << "] " << "[MySQL] Запрос " << request << " выполнен успешно." << endl;
	}
	else
	{
		cout << "[" << curDateTime() << "] " << "[MySQL] Запрос " << request << " выполнен c ошибкой: " << query << endl;
	}
}


void SQL_DB::getUserBase()
{
	UserBase->resetBase();
	m_query = "SELECT BIN_TO_UUID(uuid) uuid, name, login, pwd  FROM " + mysql_table_users + " WHERE deleted = 0 ";
	getRequest("SELECT FOR USERBASE");
	while (row = mysql_fetch_row(result))
	{
		User newUser(row[0], row[1], row[2], row[3]);
		UserBase->addInBase(newUser);
		cout << "[" << curDateTime() << "] " << "[MySQL] Пользователь " << newUser.name << " добавлен в базу" << endl;
	}
	if (UserBase->getCount() == 0)
	{
		User newUser("Administrator", "admin", "da23890e111536e631be13a069ebc5432c9cf28cdbc5deb2a70770ec9597db6d");
		regUser(newUser);
	}
}

void SQL_DB::getMsgBase()
{
	AllMsgBase->resetBase();
	m_query = "SELECT BIN_TO_UUID(msg_from) msg_from,";
	m_query += "date, message ";
	m_query += "FROM " + mysql_table_messages;
	getRequest("SELECT FOR ALL_MSGBASE");
	while (row = mysql_fetch_row(result)) {
		Message newMsg(row[0], "ALL", row[1], row[2]);
		AllMsgBase->addInBase(newMsg);
	}

	m_query = "SELECT BIN_TO_UUID(msg_from) msg_from,";
	m_query += "BIN_TO_UUID(msg_to) msg_to,";
	m_query += "date, message ";
	m_query += "FROM " + mysql_table_private_msg;
	getRequest("SELECT FOR PRIVATE_MSGBASE");
	while (row = mysql_fetch_row(result)) {
		Message newMsg(row[0], row[1], row[2], row[3]);
		AllMsgBase->addInBase(newMsg);
	}
}

void SQL_DB::delUser(int id)
{
	m_query = "UPDATE " + mysql_table_users + " ";
	m_query += "SET deleted = 1 ";
	m_query += "WHERE BIN_TO_UUID(id) = '" + UserBase->getUser(id).uuid + "'";
	sendRequest("DELETE FOR USER");
}

void SQL_DB::regUser(User& newUser)
{
	m_query = "INSERT INTO " + mysql_table_users + " ";
	m_query += "(name, login, pwd) ";
	m_query += "VALUES('" + newUser.name + "', '" + newUser.login + "', '" + newUser.pwd + "')";
	sendRequest("INSERT USER IN USERBASE");
}

void SQL_DB::chgPwd(const string& uuid, const string& pwd)
{
	m_query = "UPDATE " + mysql_table_users + " ";
	m_query += "SET pwd = '" + pwd + "'";
	m_query += "WHERE uuid = UUID_TO_BIN('" + uuid + "')";
	sendRequest("UPDATE PWD FOR USER");
}

void SQL_DB::regPrivateMsg(Message& newMsg)
{
	m_query = "INSERT INTO " + mysql_table_private_msg + " ";
	m_query += "(msg_from, msg_to, date, message) ";
	m_query += "VALUES(UUID_TO_BIN('" + newMsg.msgFrom + "'), UUID_TO_BIN('" + newMsg.msgTo + "'), '" + newMsg.msgDate + "', '" + newMsg.msgText + "')";
	sendRequest("INSERT MSG IN PRIVATE_MSGBASE");
}

void SQL_DB::regAllMsg(Message& newMsg)
{
	m_query = "INSERT INTO " + mysql_table_messages + " ";
	m_query += "(msg_from, date, message) ";
	m_query += "VALUES(UUID_TO_BIN('" + newMsg.msgFrom + "'), '" + newMsg.msgDate + "', '" + newMsg.msgText + "')";
	sendRequest("INSERT MSG IN ALL_MSGBASE");
}
