#include "MsgBased.h"

MsgBase::MsgBase(){}
MsgBase::~MsgBase(){}

Message MsgBase::splitMsgPkg(string& msgPkg)
{
	string s[msgAttr];
	for (int i = 0; i < msgAttr; i++)
	{
		s[i] = msgPkg.substr(msgPkg.find(sepBeg[i]) + sepBeg[i].length(), msgPkg.find(sepEnd[i]) - sepBeg[i].length());
		msgPkg.erase(0, msgPkg.find(sepEnd[i]) + sepEnd[i].length());
	}
	Message newMsg(s[0], s[1], s[2], s[3]);
	return newMsg;
}

string MsgBase::packMsgPkg(Message& msgPkg)
{
	string result = tagStructParam(m_Attr::indFrom, msgPkg.msgFrom);
	result += tagStructParam(m_Attr::indTo, msgPkg.msgTo);
	result += tagStructParam(m_Attr::indDate, msgPkg.msgDate);
	result += tagStructParam(m_Attr::indMsg, msgPkg.msgText);
	return result;
}

Message MsgBase::getMsg(int id)
{
	Message temp = base->at(id);
	return temp;
}

string MsgBase::tagStructParam(m_Attr id, string value)
{
	string result = sepBeg[static_cast<int>(id)];
	result += value;
	result += sepEnd[static_cast<int>(id)];
	return result;
}
