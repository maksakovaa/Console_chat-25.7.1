#pragma once
#include "TmplBase.h"
#include <iomanip>
#include <new>

using std::setw;

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
    void getUsrBase();
    void printUsrBase();
    User splitUsrPkg(string& usrPkg);
    string packUsrPkg(const User& account) const;
    string getUsrName(int id);
    string getUsrName(string& uuid);
    string getUsrUUID(int id);
    string getUsrLogin(int id);
    int getUsrId(string& name);
    void setUsrPwd(int id, string& newPwd);
    void delUsr(int id);
    void regUsr(string& name, string& login, string& pwd);
    bool logInChk(const string& login);
    bool pwdChk(int id, const string& pwd);
private:
    string delim = "<|>";
    static const int usrAttr = { 4 };
    enum class m_Attr { indUUID, indName, indLogin, indPwd };
    string sepBeg[usrAttr]{ "<uuid>", "<name>", "<login>", "<pwd>" };
    string sepEnd[usrAttr] = { "</uuid>", "</name>", "</login>", "</pwd>" };
    SHA256 sha256;
public:
    string tagStructParam(m_Attr id, string value) const;
};