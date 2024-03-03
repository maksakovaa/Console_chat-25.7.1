#pragma once
#include "functions.h"

#if defined (_WIN32) || defined (_WIN64)
static void consoleClear() {
    system("cls");
}
#elif defined (__linux__)
#include <stdlib.h>
static void consoleClear() {
    system("clear");
}
#endif

extern UsrBase* UserBase;
extern MsgBase* AllMsgBase;
extern int userId, choice;
extern bool auth;

void menu();
void menuRegUser();
void menuLogin();
void menuUserList();
void menuUserMgmt();
void menuDelUser();
void chkUsersAv();
void menuUsrMgmtSetPwd();
void menuChgPwd(int choice);
void menuAuth();
void menuPrivateSendMsg();
void menuMainChat();
void menuAbout();
