#pragma once
#include "UsrBase.h"

extern UsrBase* UserBase;

struct Message
{
    string msgFrom;
    string msgTo;
    string msgDate;
    string msgText;
    Message(string _msgFrom, string _msgTo, string _msgText) : msgFrom(_msgFrom), msgTo(_msgTo), msgText(_msgText)
    {
        msgDate = curDateTime();
    }
    Message(string _msgFrom, string _msgTo, string _msgDate, string _msgText) : msgFrom(_msgFrom), msgTo(_msgTo), msgDate(_msgDate), msgText(_msgText) {}
};

class MsgBase: public Base<Message>
{
public:
	MsgBase();
	~MsgBase();
	void getMsgBase();
    Message splitMsgPkg(string& msgPkg);
    string packMsgPkg(Message& msgPkg) const;
    void printPers(const string& uuidFrom, const string& uuidTo);
    void printMain();
    void printEmpty();
    void getMsgComp(vector<int>& msgComp, const string& msgTo = "ALL");
    void getMsgComp(vector<int>& msgComp, const string& uuidFrom, const string& uuidTo);
    void printMsgComp(vector<int>& msgComp);
	void printMsgStruct(Message& mesg);
    void sendMsg(const string& msgTo, const string& msgFrom, const string& msg);
private:
    string delim = "<|>";
    static const int msgAttr{ 4 };
    enum class m_Attr {indFrom, indTo, indDate, indMsg};
    string sepBeg[msgAttr] = {"<from>", "<to>", "<date>", "<text>"};
    string sepEnd[msgAttr] = { "</from>", "</to>", "</date>", "</text>" };
public:
    string tagStructParam(m_Attr id, string& value) const;
};