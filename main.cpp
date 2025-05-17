#include <iostream>
#include <string>
#include "User.h"
#include "LMStorage.h"
#include "UserStorage.h"
#include "LocalMessage.h"
#include "GlobalMessage.h"
using namespace std;

int registration(string& name, string& login, string& password, UserStorage& userstorage)
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
				if (userstorage.registerUser(login, password, name))
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

				User* user = userstorage.get_user(login);
				if (user == nullptr)
				{
					cout << "Мы не нашли такого пользователя :(\nМожет просто опечатка\nПопробуй еще? (1 - да  0 - нет): ";
					bool tryit;
					cin >> tryit;
					if (!tryit)
						break;
				}
				else
				{
					name = user->get_name();
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
	UserStorage userstorage; // классы чисто для взаимодействия
	LMStorage lmstorage; // хранилище локальных сообщений
	GlobalMessage globalMessage(&userstorage); // хранилище глобальных сообщений
	
	while (true)
	{
		string name;
		string login;
		string password;

		if (registration(name, login, password, userstorage) == 3)
			return 0; // ебаные баги на 4
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
				globalMessage.PrintAll();
				cout << "ваше сообщение: ";
				string message;
				cin >> message;
				globalMessage.SendMessage(login, message);
				break;
			}

			case 2: // здеся багули )
			{
				for (int i = 0; i < userstorage.get_length(); i++) // вывожу всех users для выбора
				{
					cout << i << " - " << userstorage[i]->get_name() << endl;
				}
				int user_number;
				while (true) {
					cout << "ваш выбор: ";
					cin >> user_number;
					if (!(user_number < userstorage.get_length()))
						cout << "вы ввели немного не то, еще разок" << endl;
					else
						break;
				}
				
				LocalMessage* lm = lmstorage.getLM(login, userstorage[user_number]->get_login());
				if (lm != nullptr) { lm->PrintAllMessage(); }
				else { lmstorage.addLM(userstorage.get_user(login), userstorage[user_number]); }
				cout << "ваше сообщение: ";
				string message;
				cin >> message;

				cout << endl;
				lmstorage.getLM(login, userstorage[user_number]->get_login())->SendMessage(login, message);
				break;
			}

			case 3:
				globalMessage.PrintAll();
				break;

			case 4: // тута багуязина
			{
				for (int i = 0; i < userstorage.get_length(); i++) // вывожу всех users для выбора
				{
					cout << i << " - " << userstorage[i]->get_name() << endl;
				}
				int user_number;
				while (true) {
					cout << "ваш выбор: ";
					cin >> user_number;
					if (!(user_number < userstorage.get_length()))
						cout << "вы ввели немного не то, еще разок" << endl;
					else
						break;
				}

				cout << endl;
				LocalMessage* lm = lmstorage.getLM(login, userstorage[user_number]->get_login());
				if (lm != nullptr) { lm->PrintAllMessage(); }
				else { cout << "Пока между вами переписки нет" << endl; }
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
