#pragma once
#include "MsgBase.h"

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#include <processthreadsapi.h>
#elif defined(__linux__)
#include <unistd.h>
#endif
#include <limits>

extern UsrBase* UserBase;
extern MsgBase* AllMsgBase;
extern int userId, choice;
extern bool auth;

void logOut();
void incorrect();
bool nonLatinChk(string& text);
bool noSpacesChk(string& text);
bool lengthChk(string& text);
bool regLoginChk(string& _login);
bool regPwdChk(string& _pwd);
bool authPwdChk(string& _login, string& _pwd);
bool authStatChk();
bool authAdminStatChk();
bool exit();
string getOsName();
int getProcId();
string middle(const string& str);
void printHeader(const string& header);
#if defined(_WIN32) || defined(_WIN64)
string cp1251toUtf8(const char* str);
#endif