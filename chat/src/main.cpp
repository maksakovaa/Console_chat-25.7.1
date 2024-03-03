#include "navigation.h"

UsrBase* UserBase = new UsrBase;
MsgBase* AllMsgBase = new MsgBase;
int userId, choice;
bool auth;

int main()
{
    menu();
    delete UserBase, AllMsgBase;
    return 0;
}