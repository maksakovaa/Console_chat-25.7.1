#include "netd.h"
net::net()
{
    getConfig();
    bzero(package, package_length);
#if defined (_WIN32) || (_WIN64)
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        cout << "[" << curDateTime() << "] " << "[NET] ERROR: WSAStartup failed with error: " << iResult << endl;
        exit(1);
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

#elif defined (__linux__)
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        cout << "[" << curDateTime() << "] " << "[NET] ERROR: Ошибка создания сокета." << endl;
        exit(1);
    }
    srvaddress.sin_addr.s_addr = htonl(INADDR_ANY);
    srvaddress.sin_port = htons(std::stoi(chat_port));
    srvaddress.sin_family = AF_INET;

    bind_status = bind(socket_fd, (struct sockaddr*)&srvaddress, sizeof(srvaddress));
    if (bind_status == -1)
    {
        cout << "[" << curDateTime() << "] " << "[NET] ERROR: Ошибка привязки сокета." << endl;
        exit(1);
    }
#endif
}

net::~net()
{
#if defined (_WIN32) || defined (_WIN64)
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        cout << "[" << curDateTime() << "] " << "[NET] ERROR: shutdown failed with error:" << WSAGetLastError() << endl;
        closesocket(ClientSocket);
        WSACleanup();
        exit(1);
    }

    closesocket(ClientSocket);
    WSACleanup();
#elif defined (__linux__)
    close(socket_fd);
#endif
}

void net::getConfig()
{
    server_ip = Config->getServerIP();
    chat_port = Config->getChatPort();
}

void net::netGateway()
{
    cout << "[" << curDateTime() << "] " << "[NET] Сервер приложения Chat запущен." << endl;
    while (true)
    {
#if defined (_WIN32) || defined (_WIN64)
        iResult = getaddrinfo(NULL, chat_port.data(), &hints, &result);
        if (iResult != 0)
        {
            cout << "[" << curDateTime() << "] " << "[NET] ERROR: getaddrinfo failed with error: " << iResult << endl;
            WSACleanup();
            exit(1);
        }

        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (ListenSocket == INVALID_SOCKET)
        {
            cout << "[" << curDateTime() << "] " << "[NET] ERROR: socket failed with error:" << WSAGetLastError() << endl;
            freeaddrinfo(result);
            WSACleanup();
            exit(1);
        }

        iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
        if (iResult == SOCKET_ERROR)
        {
            cout << "[" << curDateTime() << "] " << "[NET] ERROR: bind failed with error:" << WSAGetLastError() << endl;
            freeaddrinfo(result);
            closesocket(ListenSocket);
            WSACleanup();
            exit(1);
        }

        freeaddrinfo(result);

        iResult = listen(ListenSocket, SOMAXCONN);

        if (iResult == SOCKET_ERROR)
        {
            cout << "[" << curDateTime() << "] " << "[NET] ERROR: Ошибка при постановке на приём данных: " << WSAGetLastError() << endl;
            closesocket(ListenSocket);
            WSACleanup();
            exit(1);
        }

        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET)
        {
            cout << "[" << curDateTime() << "] " << "[NET] ERROR: Ошибка принятия покдключения: " << WSAGetLastError() << endl;
            closesocket(ListenSocket);
            WSACleanup();
            exit(1);
        }

        closesocket(ListenSocket);
        iResult = recv(ClientSocket, package, package_length, 0);
#elif defined (__linux__)
        connect_status = listen(socket_fd, 20);
        if (connect_status == -1)
        {
            cout << "[" << curDateTime() << "] " << "[NET] ERROR: Ошибка при постановке на приём данных." << endl;
            exit(1);
        }
        else
        {
            cout << "[" << curDateTime() << "] " << "[NET] Ожидание подключений..." << endl;
        }
        length = sizeof(client);
        connection = accept(socket_fd, (struct sockaddr*)&client, &length);
        if (connection == -1)
        {
            cout << "[" << curDateTime() << "] " << "[NET] ERROR: Сервер несмог принять данные от клиента." << endl;
            exit(1);
        }
        bzero(package, package_length);
        read(connection, package, sizeof(package));
#endif
        if (strncmp("GET_USRBASE", package, sizeof("GET_USRBASE") - 1) == 0)
        {
            sendUsrBase();
        }
        else if (strncmp("GET_MSGBASE", package, sizeof("GET_MSGBASE") - 1) == 0)
        {
            sendMsgBase();
        }
        else if (strncmp("REG_USER", package, sizeof("REG_USER") - 1) == 0)
        {
            regUser();
        }
        else if (strncmp("SND_MSG", package, sizeof("SND_MSG") - 1) == 0)
        {
            regMSG();
        }
        else if (strncmp("DEL_USER", package, sizeof("DEL_USER") - 1) == 0)
        {
            delUsr();
        }
        else if (strncmp("CHG_PWD", package, sizeof("CHG_PWD") - 1) == 0)
        {
            chgPwd();
        }

    }
}

void net::sendRequest(string& reqName)
{
#if defined (_WIN32) || defined (_WIN64)
    int iSendResult = send(ClientSocket, package, sizeof(package), 0);
    if (iSendResult == SOCKET_ERROR)
    {
        cout << "[" << curDateTime() << "] " << "[NET] ERROR: Ошибка отправки запроса: " << WSAGetLastError() << endl;
        closesocket(ClientSocket);
        WSACleanup();
        exit(1);
    }
    else
    {
        cout << "[" << curDateTime() << "] [NET] " << reqName << " send" << endl;
    }
#elif defined (__linux__)
    ssize_t bytes = write(connection, package, sizeof(package));
    if (bytes >= 0)
    {
        cout << "[" << curDateTime() << "] [NET] " << reqName << " send" << endl;
    }
#endif
}

void net::sendUsrBase()
{
    cout << "[" << curDateTime() << "] " << "[NET] GET_USRBASE request accepted" << endl;
    if (UserBase->getCount() == 0)
    {
        db_connect->getUserBase();
    }
    for (int i = 0; i < UserBase->getCount(); i++)
    {
        bzero(package, package_length);
        User account = UserBase->getUser(i);
        strcpy(package, UserBase->packUsrPkg(account).data());
        reqName = "USRBASE package ";
        sendRequest(reqName.append(std::to_string(i)));
        if (i == UserBase->getCount() - 1)
        {
            bzero(package, package_length);
            strcpy(package, "USRBASE_END");
            reqName = "USRBASE_END";
            sendRequest(reqName);
        }
    }
}

void net::sendMsgBase()
{
    cout << "[" << curDateTime() << "] " << "[NET] GET_MSGBASE request accepted" << endl;

    if (AllMsgBase->getCount() == 0)
    {
        bzero(package, package_length);
        strcpy(package, "MSGBASE_EMPTY");
        reqName = "MSGBASE_EMPTY";
        sendRequest(reqName);
        return;
    }

    for (int i = 0; i < AllMsgBase->getCount(); i++)
    {
        bzero(package, package_length);
        Message msg = AllMsgBase->getMsg(i);
        strcpy(package, AllMsgBase->packMsgPkg(msg).data());
        reqName = "MSGBASE package ";
        sendRequest(reqName.append(std::to_string(i)));
        if (i == AllMsgBase->getCount() - 1)
        {
            bzero(package, package_length);
            strcpy(package, "MSGBASE_END");
            reqName = "MSGBASE_END";
            sendRequest(reqName);
        }
    }
}

void net::regUser()
{
    cout << "[" << curDateTime() << "] " << "[NET] REG_USER request accepted" << endl;
    string temp = package;
    temp.erase(0, temp.find(delim) + delim.length());
    User newUser = UserBase->splitUsrPkg(temp);
    db_connect->regUser(newUser);
    db_connect->getUserBase();
    cout << "[" << curDateTime() << "] " << "[NET] User " << newUser.name << " registered" << endl;
}

void net::regMSG()
{
    cout << "[" << curDateTime() << "] " << "[NET] SND_MSG request accepted" << endl;
    string temp = package;
    temp.erase(0, temp.find(delim) + delim.length());
    Message newMsg = AllMsgBase->splitMsgPkg(temp);
    if (newMsg.msgTo == "ALL")
    {
        db_connect->regAllMsg(newMsg);
    }
    else
    {
        db_connect->regPrivateMsg(newMsg);
    }
    db_connect->getMsgBase();
    cout << "[" << curDateTime() << "] " << "[NET] Message " << newMsg.msgText << " send" << endl;
}

void net::delUsr()
{
    cout << "[" << curDateTime() << "] " << "[NET] DEL_USER request accepted" << endl;
    string temp = package;
    temp.erase(0, temp.find(delim) + delim.length());
    int userId = stoi(temp);
    db_connect->delUser(userId);
    db_connect->getUserBase();
    cout << "[" << curDateTime() << "] " << "[NET] User with ID " << userId << " deleted." << endl;
}

void net::chgPwd()
{
    cout << "[" << curDateTime() << "] " << "[NET] CHG_PWD request accepted" << endl;
    string temp = package;
    temp.erase(0, temp.find(delim) + delim.length());
    string uuid = temp.substr(0, temp.find(delim));
    string pwd = temp.erase(0, temp.find(delim) + delim.length());
    db_connect->chgPwd(uuid, pwd);
    db_connect->getUserBase();
    cout << "[" << curDateTime() << "] " << "[NET] Password for user " << UserBase->getUser(uuid).name << " changed" << endl;
}
