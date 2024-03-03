#include "MsgBase.h"

MsgBase::MsgBase()
{
	getMsgBase();
}

MsgBase::~MsgBase() {}

void MsgBase::getMsgBase()
{
	base->clear();
	net* online = new net;
	vector<string>* servBaseMsg = new vector<string>;
	online->getMsgBase(servBaseMsg);
	for (string msgStr : *servBaseMsg)
	{
		Message newMsg = splitMsgPkg(msgStr);
		addInBase(newMsg);
	}
	delete servBaseMsg, online;
}

Message MsgBase::splitMsgPkg(string& msgPkg)
{
	cout << msgPkg << endl;
	string s[msgAttr];
	for (int i = 0; i < msgAttr; i++)
	{
		s[i] = msgPkg.substr(msgPkg.find(sepBeg[i]) + sepBeg[i].length(), msgPkg.find(sepEnd[i]) - sepBeg[i].length());
		msgPkg.erase(0, msgPkg.find(sepEnd[i]) + sepEnd[i].length());
	}
	Message newMsg(s[0], s[1], s[2], s[3]);
	return newMsg;
}

string MsgBase::packMsgPkg(Message& msgPkg) const
{
	string result = tagStructParam(m_Attr::indFrom, msgPkg.msgFrom);
	result += tagStructParam(m_Attr::indTo, msgPkg.msgTo);
	result += tagStructParam(m_Attr::indDate, msgPkg.msgDate);
	result += tagStructParam(m_Attr::indMsg, msgPkg.msgText);
	return result;
}

void MsgBase::printPers(const string& uuidFrom, const string& uuidTo)
{
	if (getCount() == 0)
	{
		printEmpty();
		return;
	}
	vector<int> msgComp;
	getMsgComp(msgComp, uuidFrom, uuidTo);
	printMsgComp(msgComp);
}

void MsgBase::printMain()
{
	if (getCount() == 0)
	{
		printEmpty();
		return;
	}
	vector<int> msgComp;
	getMsgComp(msgComp, "ALL");
	printMsgComp(msgComp);
}

void MsgBase::printEmpty()
{
	cout << "Пока здесь еще никто ничего не писал" << endl;
}

void MsgBase::getMsgComp(vector<int>& msgComp, const string& uuidTo)
{
	for (int i = 0; i < base->size(); i++)
	{
		if (base->at(i).msgTo == uuidTo)
		{
			msgComp.push_back(i);
		}
	}
}

void MsgBase::getMsgComp(vector<int>& msgComp, const string& uuidFrom, const string& uuidTo)
{
	for (int i = 0; i < base->size(); i++)
	{
		if (base->at(i).msgFrom == uuidFrom && base->at(i).msgTo == uuidTo || base->at(i).msgFrom == uuidTo && base->at(i).msgTo == uuidFrom)
		{
			msgComp.push_back(i);
		}
	}
}

void MsgBase::printMsgComp(vector<int>& msgComp)
{
	if (msgComp.empty())
	{
		printEmpty();
		return;
	}
	else
	{
		for (int i = 0; i < msgComp.size(); i++)
		{
			printMsgStruct(base->at(msgComp[i]));
		}
	}
}

void MsgBase::printMsgStruct(Message& mesg)
{
	string border(80, '-');
	size_t count = 80 - UserBase->getUsrName(mesg.msgFrom).length() - mesg.msgDate.length() -2;

	printf("[%s | %s]\n\n%s\n%s\n", mesg.msgDate.data() , UserBase->getUsrName(mesg.msgFrom).data(), mesg.msgText.data(), border.data());
}

void MsgBase::sendMsg(const string& msgTo, const string& msgFrom, const string& msg)
{
	net* online = new net;
	Message newMsg(msgTo, msgFrom, msg);
	string req = "SND_MSG<|>";
	req.append(packMsgPkg(newMsg));
	online->sendReq(req);
	delete online;
	getMsgBase();
	return;
}

string MsgBase::tagStructParam(m_Attr id, string& value) const
{
	string result = sepBeg[static_cast<int>(id)];
	result += value;
	result += sepEnd[static_cast<int>(id)];
	return result;
}