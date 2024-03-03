#include "functions.h"

void logOut()
{
	auth = false;
	userId = -1;
}

void incorrect()
{
	cin.clear();
	cout << "Некорректный ввод, повторите:" << endl;
}

bool nonLatinChk(string& text)
{
	string letters = "абвгдеёжщийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
	if (text.find_first_of(letters) != string::npos)
	{
		cout << "ERROR: Некорректный ввод. Используйте латинницу." << endl;
		return false;
	}
	return true;
}

bool noSpacesChk(string& text)
{
	if (text.find_first_of(" ") != string::npos)
	{
		cout << "ERROR: Некорректный ввод. Не используйте пробел." << endl;
		return false;
	}
	return true;
}

bool lengthChk(string& text)
{
	if (text.size() < 4)
	{
		cout << "ERROR: Длина должна быть не менее 4 символов." << endl;
		return false;
	}
	return true;
}

bool regLoginChk(string& _login)
{
	int errCount = 0;
	if (!nonLatinChk(_login))
		errCount++;
	if (!noSpacesChk(_login))
		errCount++;
	if (!lengthChk(_login))
		errCount++;

	if (UserBase->logInChk(_login))
	{
		cout << "ERROR: Имя пользователя уже занято." << endl;
		errCount++;
	}

	if (errCount == 0)
	{
		return true;
	}
	else
	{
		cout << "Повторите ввод:" << endl;
		return false;
	}
}

bool regPwdChk(string& _pwd)
{
	int errCount = 0;
	if (!nonLatinChk(_pwd))
		errCount++;
	if (!noSpacesChk(_pwd))
		errCount++;
	if (!lengthChk(_pwd))
		errCount++;

	if (errCount == 0)
	{
		return true;
	}
	else
	{
		cout << "Повторите ввод:" << endl;
		return false;
	}
}

bool authPwdChk(string& _login, string& _pwd)
{
	int id = UserBase->getUsrId(_login);
	return UserBase->pwdChk(id, _pwd);
}

bool authStatChk()
{
	if (auth && userId >= 0 && userId < UserBase->getCount())
	{
		return true;
	}
	return false;
}

bool authAdminStatChk()
{
	if (auth && userId == 0)
		return true;

	return false;
}

bool exit()
{
	return 0;
}

string getOsName()
{
#if defined(_WIN32) || defined (_WIN64)
	return "Windows";
#elif defined(__linux__)
	return "Linux";
#endif
}

int getProcId()
{
#if defined(__linux__)
	return getpid();
#elif defined(_WIN32) || defined(_WIN64)
	return GetCurrentProcessId();
#endif
}

string middle(const string& str)
{
	const int max_len = 80;

	size_t spaces_needed, str_len;
	str_len = str.length();
	spaces_needed = (max_len - str_len) / 2;

	string buffer(spaces_needed, ' ');
	buffer += str;

	return buffer;
}

void printHeader(const string& header)
{
	string border(80, '-');
	printf("%s\n%s\n", middle(header).data(), border.data());
}

#if defined(_WIN32) || defined(_WIN64)
string cp1251toUtf8(const char* str)
{
	string res;
	int result_u, result_c;
	result_u = MultiByteToWideChar(1251, 0, str, -1, 0, 0);
	if (!result_u) { return 0; }
	wchar_t* ures = new wchar_t[result_u];
	if (!MultiByteToWideChar(1251, 0, str, -1, ures, result_u)) {
		delete[] ures;
		return 0;
	}
	result_c = WideCharToMultiByte(65001, 0, ures, -1, 0, 0, 0, 0);
	if (!result_c) {
		delete[] ures;
		return 0;
	}
	char* cres = new char[result_c];
	if (!WideCharToMultiByte(65001, 0, ures, -1, cres, result_c, 0, 0)) {
		delete[] cres;
		return 0;
	}
	delete[] ures;
	res.append(cres);
	delete[] cres;
	return res;
}
#endif