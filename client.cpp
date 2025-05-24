#include <iostream>
#include <string>
#include <cstring>
#include "User.h"
#include "LMStorage.h"
#include "UserStorage.h"
#include "LocalMessage.h"
#include "GlobalMessage.h"
#include "ConectToServer.h"
using namespace std;

int registration(string& name, string& login, string& password, ConectToServer& CTS)
{
	while (true)
	{
		int branch;
		cout << "1 - регистрация\n2 - вход\n3 - выход из приложения\nваш ответ: ";
		cin >> branch;

		switch (branch) // регистрвация
		{
		case 1:
			while (true)
			{
				cout << "Введите имя нового пользователя: ";
				cin >> name;
				cout << "Введите логин нового пользователя: ";
				cin >> login;
				cout << "Введите пароль нового пользователя: ";
				cin >> password;
				cout << endl;
				if (!CTS.userRegistration(login, password, name))
				{
					cout << "Вы зарегестрировались!" << endl;
					return 1;
				}
				else
					cout << "Мы не смогли вас зарегестрировать :(\nПопробуйте изменить логин" << endl;
			}
		case 2:
			while (true)
			{
				cout << "Введите логин: ";
				cin >> login;
				cout << "Введите пароль: ";
				cin >> password;

				cout << "waiting request ..." << endl;
				name = CTS.login(login, password);
				cout << "response: " << name << endl;

				if (name == "1")
				{
					cout << "Мы не нашли такого пользователя :(\nМожет просто опечатка\nПопробуй еще? (1 - да  0 - нет): ";
					bool tryit;
					cin >> tryit;
					if (!tryit)
						break;
				}
				else if (name == "2")
				{
					cout << "Неверный пароль :(\nМожет просто опечатка\nПопробуй еще? (1 - да  0 - нет): ";
					bool tryit;
					cin >> tryit;
					if (!tryit)
						break;
				}
				else
				{
					cout << "Вы вошли!" << endl;
					return 2;
				}
			}
			break;
		case 3:
			return 3;
			break;

		default:
			cout << "Ты уверен что там такой выбор есть?" << endl;
			break;
		}
	}	
}

int main()
{
	setlocale(LC_ALL, "ru");
	ConectToServer CTS; // помошник для общения с сервером

	while (true)
	{
		string name;
		string login;
		string password;

		if (registration(name, login, password, CTS) == 3)
			return 0;
		while (true)
		{
			int branch;
			cout << "\n1 - написать в глобальные сообщение\n2 - написать в личные сообщения\n3 - вывести глобальные сообщения" 
				<< "\n4 - вывести личное сообщение\n5 - выйти из аккаунта\nваш выбор: ";
			cin >> branch;

		
			switch (branch)
			{
			case 1:
			{
				cout << CTS.getGM() << endl;
				cout << "ваше сообщение: ";
				string message;
				cin >> message;
				CTS.sendGM(message, login);
				break;
			}

			case 2:
			{
				string login_recipient;
				cout << "login получателя: ";
				cin >> login_recipient;
					
				string message;
				cout << "ваше сообщение: ";
				cin >> message;
				cout << "Введено: " << message << endl;
				
				if (CTS.sendLM(message, login, login_recipient))
					cout << "Что-то не так :(" << endl;
				else
					cout << "Сообщение улетело ;)" << endl;

				cout << endl;
				break;
			}

			case 3:
				cout << endl << CTS.getGM();
				break;

			case 4:
			{
				string user_login;
				cout << "login этого пользователя: ";
				cin >> user_login;	
				cout << endl;

				cout << CTS.getLM(login, user_login) << endl;
				
				break;
			}


			case 5:
				login = "";
				break;

			default:
				cout << "Давай без опечаток, еще разок ;)" << endl;
				break;
			}
			cout << endl;
			if (login == "") { break; }
		}
	}
}
