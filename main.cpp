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
		cout << "1 - �����������\n2 - ����\n3 - ����� �� ����������\n��� �����: ";
		cin >> branch;

		switch (branch) // ������������
		{
		case 1:
			while (true)
			{
				cout << "������� ��� ������ ������������: ";
				cin >> name;
				cout << "������� ����� ������ ������������: ";
				cin >> login;
				cout << "������� ������ ������ ������������: ";
				cin >> password;
				if (userstorage.registerUser(login, password, name))
				{
					cout << "�� ������������������!" << endl;
					return 1;
				}
				else
					cout << "�� �� ������ ��� ���������������� :(\n���������� �������� �����" << endl;
			}
		case 2:
			while (true)
			{
				cout << "������� �����: ";
				cin >> login;
				cout << "������� ������: ";
				cin >> password;

				User* user = userstorage.get_user(login);
				if (user == nullptr)
				{
					cout << "�� �� ����� ������ ������������ :(\n����� ������ ��������\n�������� ���? (1 - ��  0 - ���): ";
					bool tryit;
					cin >> tryit;
					if (!tryit)
						break;
				}
				else
				{
					name = user->get_name();
					cout << "�� �����!" << endl;
					return 2;
				}
			}
			break;
		case 3:
			return 3;
			break;

		default:
			cout << "�� ������ ��� ��� ����� ����� ����?" << endl;
			break;
		}
	}	
}

int main()
{
	setlocale(LC_ALL, "ru");
	UserStorage userstorage; // ������ ����� ��� ��������������
	LMStorage lmstorage; // ��������� ��������� ���������
	GlobalMessage globalMessage(&userstorage); // ��������� ���������� ���������
	
	while (true)
	{
		string name;
		string login;
		string password;

		if (registration(name, login, password, userstorage) == 3)
			return 0; // ������ ���� �� 4
		while (true)
		{
			int branch;
			cout << "\n1 - �������� � ���������� ���������\n2 - �������� � ������ ���������\n3 - ������� ���������� ���������" 
				<< "\n4 - ������� ������ ���������\n5 - ����� �� ��������\n��� �����: ";
			cin >> branch;

		
			switch (branch)
			{
			case 1:
			{
				globalMessage.PrintAll();
				cout << "���� ���������: ";
				string message;
				cin >> message;
				globalMessage.SendMessage(login, message);
				break;
			}

			case 2: // ����� ������ )
			{
				for (int i = 0; i < userstorage.get_length(); i++) // ������ ���� users ��� ������
				{
					cout << i << " - " << userstorage[i]->get_name() << endl;
				}
				int user_number;
				while (true) {
					cout << "��� �����: ";
					cin >> user_number;
					if (!(user_number < userstorage.get_length()))
						cout << "�� ����� ������� �� ��, ��� �����" << endl;
					else
						break;
				}
				
				LocalMessage* lm = lmstorage.getLM(login, userstorage[user_number]->get_login());
				if (lm != nullptr) { lm->PrintAllMessage(); }
				else { lmstorage.addLM(userstorage.get_user(login), userstorage[user_number]); }
				cout << "���� ���������: ";
				string message;
				cin >> message;

				cout << endl;
				lmstorage.getLM(login, userstorage[user_number]->get_login())->SendMessage(login, message);
				break;
			}

			case 3:
				globalMessage.PrintAll();
				break;

			case 4: // ���� ���������
			{
				for (int i = 0; i < userstorage.get_length(); i++) // ������ ���� users ��� ������
				{
					cout << i << " - " << userstorage[i]->get_name() << endl;
				}
				int user_number;
				while (true) {
					cout << "��� �����: ";
					cin >> user_number;
					if (!(user_number < userstorage.get_length()))
						cout << "�� ����� ������� �� ��, ��� �����" << endl;
					else
						break;
				}

				cout << endl;
				LocalMessage* lm = lmstorage.getLM(login, userstorage[user_number]->get_login());
				if (lm != nullptr) { lm->PrintAllMessage(); }
				else { cout << "���� ����� ���� ��������� ���" << endl; }
				break;
			}


			case 5:
				login = "";
				break;

			default:
				cout << "����� ��� ��������, ��� ����� ;)" << endl;
				break;
			}
			cout << endl;
			if (login == "") { break; }
		}
	}
}
