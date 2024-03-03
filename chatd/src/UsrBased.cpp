#include "UsrBased.h"

UsrBase::UsrBase() {}
UsrBase::~UsrBase() {}

User UsrBase::splitUsrPkg(string& usrPkg)
{
    string s[usrAttr-1];
    for (int i = 1; i < usrAttr; i++)
    {
        s[i-1] = usrPkg.substr(usrPkg.find(sepBeg[i]) + sepBeg[i].length(), usrPkg.find(sepEnd[i]) - sepBeg[i].length());
        usrPkg.erase(0, usrPkg.find(sepEnd[i]) + sepEnd[i].length());
    }
    User newUser(s[0], s[1], s[2]);
    return newUser;
}

User UsrBase::getUser(int id)
{
    return base->at(id);
}

User UsrBase::getUser(const string& uuid)
{
    for (User account: *base)
    {
        if (account.uuid == uuid)
        {
            return account;
        }
    }
}
string UsrBase::packUsrPkg(User& account) const
{
    string result = tagStructParam(m_Attr::indUUID, account.uuid);
    result += tagStructParam(m_Attr::indName, account.name);
    result += tagStructParam(m_Attr::indLogin, account.login);
    result += tagStructParam(m_Attr::indPwd, account.pwd);
    return result;
}

string UsrBase::tagStructParam(m_Attr id, string value) const
{
    string result = sepBeg[static_cast<int>(id)];
    result += value;
    result += sepEnd[static_cast<int>(id)];
    return result;
}
