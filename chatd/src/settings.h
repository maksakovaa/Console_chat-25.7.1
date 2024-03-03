#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <new>
#include <fstream>
#include <ctime>
#if defined (_WIN32) || defined (_WIN64)
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <mysql.h>
#include <stdlib.h>
#include <stdio.h>
#pragma comment (lib, "Ws2_32.lib")
#include <string.h>
#define bzero(b,len) (memset((b), '0', (len)), (void) 0)
#pragma warning(disable : 4996)
#elif defined (__linux__)
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdint.h>
#include <mysql/mysql.h>
#endif

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::cin;

string curDateTime();

class Settings
{
public:
	Settings();
	~Settings();
	const string& getMysqlSrvIP();
	const string& getMysqlLogin();
	const string& getMysqlPass();
	const string& getServerIP();
	const string& getChatPort();
	const string& getMysql_database();
	const string& getMysql_table_users();
	const string& getMysql_table_messages();
	const string& getMysql_table_private_msg();
private:
	void saveConfig();
	string delim_settings = " = ";
	string mysql_srv_ip;
	string mysql_login;
	string mysql_pass;
	string server_ip;
	string chat_port;
	string mysql_database;
	string mysql_table_users;
	string mysql_table_messages;
	string mysql_table_private_msg;
#if defined (_WIN32) || defined (_WIN64)
	string cfgPath = "settings.ini";
#elif defined (__linux__)
	string cfgPath = "/var/lib/chatd/settings.ini";
#endif
};