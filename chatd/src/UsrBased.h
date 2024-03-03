#pragma once
#include "TmplBased.h"

class SQL_DB;
extern SQL_DB* db_connect;

struct User
{
    string uuid;
    string name;
    string login;
    string pwd;
    User(const string& newName, const string& newLogin, const string& newPwd) : name(newName), login(newLogin), pwd(newPwd) { uuid = "-"; }
    User(const string& newUUID, const string& newName, const string& newLogin, const string& newPwd) : uuid(newUUID), name(newName), login(newLogin), pwd(newPwd) {}
};

class UsrBase: public Base<User>
{
public:
	UsrBase();
	~UsrBase();
    User splitUsrPkg(string& usrPkg);
    User getUser(int id);
    User getUser(const string& uuid);
    string packUsrPkg(User& account) const;
private:
    string delim = "<|>";
    static const int usrAttr{ 4 };
    enum class m_Attr { indUUID, indName, indLogin, indPwd };
    string sepBeg[usrAttr]{ "<uuid>", "<name>", "<login>", "<pwd>" };
    string sepEnd[usrAttr] = { "</uuid>", "</name>", "</login>", "</pwd>"};
    SHA256 sha256;
public:
    string tagStructParam(m_Attr id, string value) const;
};