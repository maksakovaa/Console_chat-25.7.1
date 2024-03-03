#pragma once
#include "UsrBased.h"

struct Message
{
    string msgFrom;
    string msgTo;
    string msgDate;
    string msgText;
    Message(const string& _msgFrom, const string& _msgTo, const string& _msgText) : msgFrom(_msgFrom), msgTo(_msgTo), msgText(_msgText)
    {
        msgDate = curDateTime();
    }
    Message(const string& _msgFrom, const string& _msgTo, const string& _msgDate, const string& _msgText) : msgFrom(_msgFrom), msgTo(_msgTo), msgDate(_msgDate), msgText(_msgText) {}
};

class MsgBase: public Base<Message>
{
public:
	MsgBase();
	~MsgBase();
    Message splitMsgPkg(string& msgPkg);
    string packMsgPkg(Message& msgPkg);
    Message getMsg(int id);
private:
    string delim = "<|>";
    static const int msgAttr{ 4 };
    enum class m_Attr { indFrom, indTo, indDate, indMsg };
    string sepBeg[msgAttr] = { "<from>", "<to>", "<date>", "<text>"};
    string sepEnd[msgAttr] = { "</from>", "</to>", "</date>", "</text>" };
public:
    string tagStructParam(m_Attr id, string value);
};