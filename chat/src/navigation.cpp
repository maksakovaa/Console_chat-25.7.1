#include "navigation.h"

void menu()
{
	if (authStatChk())
	{
		menuAuth();
		return;
	}
	consoleClear();
	printHeader("[Главное меню]");
	cout << "Добро пожаловать в консольный чат!\nДля просмотра и отправки сообщений необходимо войти или зарегистрироваться. " << endl;
	cout << "Выберите действие:\n 1 - Регистрация пользователя \n 2 - Авторизация \n 3 - Список пользователей \n 4 - О программе \n 5 - Выход из программы" << endl;
	(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	while (choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice !=5)
	{
		incorrect();
		(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	}
	switch (choice)
	{
	case 1:
	{
		menuRegUser();
		break;
	}
	case 2:
	{
		menuLogin();
		break;
	}
	case 3:
	{
		menuUserList();
		break;
	}
	case 4:
	{
		menuAbout();
		break;
	}
	case 5:
	{
		exit();
		break;
	}
	default:
		break;
	}
}

void menuRegUser()
{
	string name, login, pwd;
	consoleClear();
	printHeader("[Регистрация]");
	cout << "Введите имя пользователя:" << endl;
	getline(cin, name);
#if defined(_WIN32) || defined(_WIN64)
	name = cp1251toUtf8(name.data());
#endif
	cout << "Введите логин для пользователя " << name << ":" << endl;
	getline(cin, login);
#if defined(_WIN32) || defined(_WIN64)
	login = cp1251toUtf8(login.data());
#endif
	while (!regLoginChk(login))
	{
		getline(cin, login);
#if defined(_WIN32) || defined(_WIN64)
		login = cp1251toUtf8(login.data());
#endif
	}
	cout << "Введите желаемый пароль для " << name << ":" << endl;
	getline(cin, pwd);
#if defined(_WIN32) || defined(_WIN64)
	pwd = cp1251toUtf8(pwd.data());
#endif
	while (!regPwdChk(pwd))
	{
		getline(cin, pwd);
#if defined(_WIN32) || defined(_WIN64)
		pwd = cp1251toUtf8(pwd.data());
#endif
	}
	UserBase->regUsr(name, login, pwd);
	cout << "Пользователь " << name << " успешно зарегистрирован. \n 1 - Главное меню\n 2 - Авторизация" << endl;
	(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	while (choice != 1 && choice != 2)
	{
		incorrect();
		(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	}
	switch (choice)
	{
	case 1:
	{
		if (auth && userId == 0)
		{
			menuAuth();
			break;
		}
		else
		{
			menu();
			break;
		}
		break;
	}
	case 2:
	{
		menuLogin();
		break;
	}
	default:
		break;
	}
}

void menuLogin()
{
	string name, login, pwd;
	consoleClear();
	printHeader("[Авторизация]");
	cout << "Вход для зарегистрированных пользователей" << endl;
	cout << "Введите логин:" << endl;
	cin.clear();
	getline(cin, login);
#if defined(_WIN32) || defined(_WIN64)
	login = cp1251toUtf8(login.data());
#endif
	if (UserBase->logInChk(login))
	{
		cout << "Введите пароль:" << endl;
		getline(cin, pwd);
#if defined(_WIN32) || defined (_WIN64)
		pwd = cp1251toUtf8(pwd.data());
#endif
		while (!authPwdChk(login, pwd))
		{
			cout << "Неверный пароль, повторите:" << endl;
			getline(cin, pwd);
#if defined(_WIN32) || defined (_WIN64)
			pwd = cp1251toUtf8(pwd.data());
#endif
		}
		if (authPwdChk(login, pwd))
		{
			auth = true;
			userId = UserBase->getUsrId(login);
			menuAuth();
		}
	}
	else
	{
		cout << "Неверное имя пользователя" << endl;
		cout << "1 - вернуться в главное меню, 2 - выход из программы, 3 - повторить" << endl;
		(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		while (choice != 1 && choice != 2 && choice != 3)
		{
			incorrect();
			(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		}
		switch (choice)
		{
		case 1:
		{
			menu();
			break;
		}
		case 2:
		{
			exit();
			break;
		}
		case 3:
		{
			menuLogin();
			break;
		}
		default:
			break;
		}
	}
}

void menuUserList()
{
	consoleClear();
	printHeader("[Список пользователей]");
	UserBase->printUsrBase();
	cout << "1 - вернуться в главное меню, 2 - выход из программы";
	if (authStatChk())
		cout << " 3 - Просмотр сообщений";
	cout << endl;

	(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	if (authStatChk())
	{
		while (choice != 1 && choice != 2 && choice != 3)
		{
			incorrect();
			(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		}
	}
	else
	{
		while (choice != 1 && choice != 2)
		{
			incorrect();
			(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		}
	}
	switch (choice)
	{
	case 1:
	{
		if (authStatChk())
		{
			menuAuth();
		}
		else
		{
			menu();
		}
		break;
	}
	case 2:
	{
		exit();
		break;
	}
	case 3:
	{
		menuPrivateSendMsg();
		break;
	}
	default:
		break;
	}
}

void menuUserMgmt()
{
	if (!authAdminStatChk())
	{
		menuAuth();
		return;
	}
	consoleClear();
	cout << "Управление пользователями. \n";
	UserBase->printUsrBase();
	cout << "\n Выберите действие: \n 1 - Добавить пользователя \n 2 - Удалить пользователя \n 3 - Сменить пароль пользователя \n 4 - Главное меню" << endl;
	(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	while (choice != 1 && choice != 2 && choice != 3 && choice != 4)
	{
		incorrect();
		(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	}
	switch (choice)
	{
	case 1:
	{
		menuRegUser();
		break;
	}
	case 2:
	{
		menuDelUser();
		break;
	}
	case 3:
	{
		menuUsrMgmtSetPwd();
		break;
	}
	case 4:
	{
		menuAuth();
		break;
	}
	default:
		break;
	}
}

void chkUsersAv()
{
	if (UserBase->getCount() <= 1)
	{
		cout << "Нет доступных для измениения пользователей. Продолжить? \n 1 - Да 2 - нет" << endl;
		(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		while (choice != 1 && choice != 2)
		{
			incorrect();
			(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		}
		switch (choice)
		{
		case 1:
		{
			menuUserMgmt();
			break;
		}
		case 2:
		{
			menuAuth();
			break;
		}
		default:
			break;
		}
	}
}

void menuDelUser()
{
	if (!authAdminStatChk())
	{
		menuAuth();
		return;
	}
	if (UserBase->getCount() > 1)
	{
		cout << "Учётную запись \"Администратор\" удалить нельзя. Для удаления пользователя введите его ID : " << endl;
		(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		while (choice <= 0 || choice >= UserBase->getCount())
		{
			incorrect();
			(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		}
		cout << "Вы уверены что хотите удалить " << UserBase->getUsrName(choice) << "? \n 1 - да, 2 - нет" << endl;
		int temp;
		(cin >> temp).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		while (temp != 1 && temp != 2)
		{
			incorrect();
			(cin >> temp).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		}
		switch (temp)
		{
		case 1:
		{
			string curUser = UserBase->getUsrLogin(userId);
			UserBase->delUsr(choice);
			if (choice < userId)
			{
				userId = UserBase->getUsrId(curUser);
			}
			menuUserMgmt();
			break;
		}
		case 2:
		{
			menuUserMgmt();
			break;
		}
		default:
			break;
		}
	}
	else
	{
		chkUsersAv();
	}
}

void menuUsrMgmtSetPwd()
{
	if (UserBase->getCount() > 1)
	{
		cout << "Изменить пароль учётной записи \"Администратор\" нельзя. Для изменения пароля пользователя введите его ID : " << endl;
		(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		while (choice <= 0 || choice >= UserBase->getCount())
		{
			incorrect();
			(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		}
		menuChgPwd(choice);
	}
	else
	{
		chkUsersAv();
	}
}

void menuChgPwd(int choice = userId)
{
	string pwd;
	cout << "Введите новый пароль:" << endl;
	cin.clear();
	getline(cin, pwd);
#if defined(_WIN32) || defined (_WIN64)
	pwd = cp1251toUtf8(pwd.data());
#endif
	while (!regPwdChk(pwd))
	{
		getline(cin, pwd);
#if defined(_WIN32) || defined (_WIN64)
		pwd = cp1251toUtf8(pwd.data());
#endif
	}
	UserBase->setUsrPwd(choice, pwd);
	cout << "Пароль пользователя " << UserBase->getUsrName(choice) << " успешно изменен. Введите 1 чтобы продолжить, 2 чтобы выйти из программы" << endl;
	(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	while (choice != 1 && choice != 2)
	{
		incorrect();
		(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	}
	switch (choice)
	{
		case 1:
		{
			if(userId == 0)
				menuUserMgmt();
			if (userId > 0)
				menuAuth();
			break;
		}
		case 2:
		{
			exit();
			break;
		}
		default:
			break;
	}
}

void menuAuth()
{
	if (!authStatChk())
	{
		menu();
		return;
	}
	consoleClear();
	printHeader("[Главное меню]");
	cout << UserBase->getUsrName(userId) << " добро пожаловать в консольный чат!" << endl;
	cout << "Выберите действие:\n 1 - Общий чат\n 2 - Личные сообщения" << endl;
	if (userId == 0)
	{
		cout << " 3 - Управление пользователями" << endl;
	}
	else if (userId > 0)
	{
		cout << " 3 - Изменить пароль" << endl;
	}
	cout << " 4 - Выйти из учетной записи\n 5 - О программе\n 6 - Выход из программы" << endl;
	(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	while (choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice != 6)
	{
		incorrect();
		(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	}
	switch (choice)
	{
	case 1:
	{
		menuMainChat();
		break;
	}
	case 2:
	{
		menuUserList();
		break;
	}
	case 3:
	{
		if (userId == 0)
		{
			menuUserMgmt();
			break;
		}
		else if (userId > 0)
		{
			menuChgPwd();
			break;
		}
	}
	case 4:
	{
		logOut();
		menu();
		break;
	}
	case 5:
	{
		menuAbout();
		break;
	}
	case 6:
	{
		exit();
		break;
	}
	default:
		break;
	}
}

void menuPrivateSendMsg()
{
	if (!authStatChk())
	{
		menu();
		return;
	}
	int temp;
	cout << "Введите ID пользователя:" << endl;
	(cin >> temp).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	while (cin.fail() || temp < 0 || temp >= UserBase->getCount())
	{
		incorrect();
		(cin >> temp).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	}
	while (choice == 3)
	{
		consoleClear();
		string header = "[Чат с " + UserBase->getUsrName(temp) + "]";
		printHeader(header);

		AllMsgBase->printPers(UserBase->getUsrUUID(temp), UserBase->getUsrUUID(userId));

		cout << " 1 - Вернуться в главное меню\n 2 - Выход из программы\n 3 - Отправка сообщения\n 4 - Вернуться к списку пользователей" << endl;
		(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		while (choice != 1 && choice != 2 && choice != 3 && choice != 4)
		{
			incorrect();
			(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		}
		switch (choice)
		{
		case 1:
		{
			menuAuth();
			break;
		}
		case 2:
		{
			exit();
			break;
		}
		case 3:
		{
			cout << "Введите сообщение:" << endl;
			string msgText;
			cin.clear();
			getline(cin, msgText);
#if defined(_WIN32) || defined (_WIN64)
			msgText = cp1251toUtf8(msgText.data());
#endif
			AllMsgBase->sendMsg(UserBase->getUsrUUID(userId), UserBase->getUsrUUID(temp), msgText);
			break;
		}
		case 4:
		{
			menuUserList();
			break;
		}
		default:
			break;
		}
	}
}

void menuMainChat()
{
	if (!authStatChk())
	{
		menu();
		return;
	}
	while (choice == 1)
	{
		consoleClear();
		printHeader("[Общий чат]");
		AllMsgBase->printMain();
		cout << " 1 - Отправить сообщение\n 2 - Вернуться в главное меню\n 3 - Выход из программы" << endl;
		(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		while (choice != 1 && choice != 2 && choice != 3)
		{
			incorrect();
			(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		}
		switch (choice)
		{
		case 1:
		{
			cout << "Введите сообщение:" << endl;
			string msgText;
			cin.clear();
			getline(cin, msgText);
#if defined(_WIN32) || defined (_WIN64)
			msgText = cp1251toUtf8(msgText.data());
#endif
			AllMsgBase->sendMsg(UserBase->getUsrUUID(userId), "ALL",  msgText);
			break;
		}
		case 2:
		{
			menuAuth();
			break;
		}
		case 3:
		{
			exit();
			break;
		}
		default:
			break;
		}
	}
}

void menuAbout()
{
	consoleClear();
	printHeader("О программе");
	cout << "Программа консольный чат. [0.59]\n© Максаков А.А., 2024\nСборка для " << getOsName() << endl;
	cout << "PID процесса: " << getProcId() << endl;
	cout << "1 - Вернуться в главное меню\n2 - Выход из программы" << endl;
	(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	while (choice != 1 && choice != 2)
	{
		incorrect();
		(cin >> choice).ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	}
	switch (choice)
	{
	case 1:
	{
		menu();
		break;
	}
	case 2:
	{
		exit();
		break;
	}
	default:
		break;
	}
}
