#include "UsrBase.h"

UsrBase::UsrBase()
{
    getUsrBase();
}

UsrBase::~UsrBase()
{

}

void UsrBase::getUsrBase()
{
#if defined(_WIN32) || defined (_WIN64)
    SetConsoleOutputCP(65001);
    SetConsoleCP(1251);
#elif defined(__linux__)
    setlocale(LC_ALL, "ru_RU.utf8");
#endif
    base->clear();
    net* online = new net;
    vector<string>* servBase = new vector<string>;
    online->getUsrBase(servBase);
    for (string perStr: *servBase)
    {
        User newUser = splitUsrPkg(perStr);
        addInBase(newUser);
    }
    delete servBase, online;
}

void UsrBase::printUsrBase()
{
    printf("%-*s %-*s %-*s \n", 5, "ID", 12, "Логин", 32, "Имя пользователя");
    for (int i = 0; i < base->size(); i++)
    {
        printf("%-*i %-*s %-*s \n", 5, i, 12, base->at(i).login.data(), 32, base->at(i).name.data());
    }
}

User UsrBase::splitUsrPkg(string& usrPkg)
{
     string s[usrAttr];
    for (int i = 0; i < usrAttr; i++)
    {
        s[i] = usrPkg.substr(usrPkg.find(sepBeg[i]) + sepBeg[i].length(), usrPkg.find(sepEnd[i]) - sepBeg[i].length());
        usrPkg.erase(0, usrPkg.find(sepEnd[i]) + sepEnd[i].length());
    }
    User newUser(s[0], s[1], s[2], s[3]);
    return newUser;
}

string UsrBase::packUsrPkg(const User& account) const
{
    string result = tagStructParam(m_Attr::indName, account.name);
    result += tagStructParam(m_Attr::indLogin, account.login);
    result += tagStructParam(m_Attr::indPwd, account.pwd);
    return result;
}

string UsrBase::getUsrName(int id)
{
    return base->at(id).name;
}

string UsrBase::getUsrName(string& uuid)
{
    for (User account: *base)
    {
        if (account.uuid == uuid)
        {
            return account.name;
        }
    }
    return "deleted";
}

string UsrBase::getUsrUUID(int id)
{
    return base->at(id).uuid;
}

string UsrBase::getUsrLogin(int id)
{
    return base->at(id).login;
}

int UsrBase::getUsrId(string& login)
{
    for (int i = 0; i < (int)base->size(); i++)
    {
        if (base->at(i).login == login)
        {
            return i;
        }
    }
    return -1;
}

void UsrBase::setUsrPwd(int id, string& newPwd)
{
    net* online = new net;
    string usrPkg = "CHG_PWD";
    usrPkg += delim;
    usrPkg += base->at(id).uuid;
    usrPkg += delim;
    usrPkg += sha256(newPwd);
    online->sendReq(usrPkg);
    delete online;
    getUsrBase();
}

void UsrBase::delUsr(int id)
{
    net* online = new net;
    string usrPkg = "DEL_USER";
    usrPkg += delim;
    usrPkg += std::to_string(id);
    online->sendReq(usrPkg);
    delete online;
    getUsrBase();
}

void UsrBase::regUsr(string& name, string& login, string& pwd)
{
    User newUser(name, login, sha256(pwd));
    net* online = new net;
    string usrPkg = "REG_USER";
    usrPkg += delim;
    usrPkg += packUsrPkg(newUser);
    online->sendReq(usrPkg);
    delete online;
    getUsrBase();
}

bool UsrBase::logInChk(const string& login)
{
    for (User person: *base)
    {
        if (person.login == login)
        {
            return true;
        }
    }
    return false;
}

bool UsrBase::pwdChk(int id, const string& pwd)
{
    string temp = sha256(pwd);
    if (base->at(id).pwd == temp)
    {
        return true;
    }
    else
    {
        return false;
    }
}

string UsrBase::tagStructParam(m_Attr id, string value) const
{
    string result = sepBeg[static_cast<int>(id)];
    result += value;
    result += sepEnd[static_cast<int>(id)];
    return result;
}
