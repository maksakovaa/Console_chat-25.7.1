#pragma once
#include "sql_db.h"

extern Settings* Config;
extern UsrBase* UserBase;
extern MsgBase* AllMsgBase;
extern SQL_DB* db_connect;

class net
{
public:
	net();
	~net();
	void netGateway();
	void sendRequest(string& reqName);
	void sendUsrBase();
	void sendMsgBase();
	void regUser();
	void regMSG();
	void delUsr();
	void chgPwd();
    void getConfig();
private:
	string reqName;
	string delim = "<|>";
	static const int package_length = 1024;
	char package[package_length];
    string server_ip;
    string chat_port;
#if defined (_WIN32) || defined (_WIN64)
	WSADATA wsaData;
	int iResult;
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL;
	struct addrinfo hints;
#elif defined (__linux__)
	struct sockaddr_in srvaddress, client;
	socklen_t length;
	int socket_fd, connection, bind_status, connect_status;
#endif
};