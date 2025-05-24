#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "User.h"
#include "UserStorage.h"
#include "LocalMessage.h"
#include "LMStorage.h"
#include "GlobalMessage.h"
using namespace std;

const int PORT = 8080;

int main()
{
	setlocale(LC_ALL, "ru");
	UserStorage userstorage; // просто vector юзеров
	LMStorage lmstorage; // хранилище локальных сообщений и просто vector LM
	GlobalMessage globalMessage(&userstorage); // хранилище глобальных сообщений (чат)
	
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (server_socket == -1) {
			std::cerr << "Socket creation failed\n";
			return -1;
		}

	// 2. Устанавливаем адрес для сервера
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;  // Позволяет принимать соединения на всех интерфейсах
	server_addr.sin_port = htons(PORT);

	// 3. Привязываем сокет к адресу
	if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
		std::cerr << "Bind failed\n";
		return -1;
	}
	
	// 4. Слушаем входящие соединения
	if (listen(server_socket, 3) == -1) {
		std::cerr << "Listen failed\n";
		return -1;
	}
	char buffer[1024];
	sockaddr_in client_addr;
	while (true)
	{
		

		std::cout << "Server is listening on port " << PORT << "...\n";

	
		string name;
		string login;
		string password;		

		// 5. Принимаем соединение от клиента
		socklen_t addr_len = sizeof(client_addr);
		int client_fd = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
		if (client_fd == -1) {
			std::cerr << "Accept failed\n";
			return -1;
		}

		std::cout << "Client request:\n";

		// 6. Принимаем сообщение от клиента
		memset(buffer, 0, sizeof(buffer));
		ssize_t len = recv(client_fd, buffer, sizeof(buffer), 0);
		if (len <= 0) {
			std::cerr << "Connection closed or error\n";
			break;
		}		
		std::cout << "Client: " << buffer << "\n";

		std::istringstream iss(buffer); // класс для пробежки по слову
		switch (buffer[0]) // обработка запроса
		{
		case 'i': // login to account
		{
			string login;
			string password;
			iss >> login; // скипаем первый символ

			iss >> login;
			iss >> password;

			cout << "debug: login = " << login << endl;
			cout << "debug: password = " << password << endl;

			User* user = userstorage.get_user(login);
			if (user == nullptr){
				send(client_fd, new char('1'), 1, 0); // неудачно
				cout << "user is not found" << endl;
			}
			else if(user->get_password() == password) { // вход
				string message = "0 " + user->get_name();
				send(client_fd, &message[0], message.size(), 0);
				cout << "user login successfully!" << endl;
			}
			else{
				send(client_fd, new char('2'), 1, 0); // неудачно
				cout << "password is not true" << endl;
			}
			break;
		}

		case 'c': // Create new user
		{
			cout << "\"c\" is working ..." << endl;
			string* login = new string;
			string* password = new string;
			string* name = new string;
			iss >> *login; // скипаем первый символ

			iss >> *login;
			iss >> *password;
			iss >> *name;
			cout << "debug: login = " << *login << endl;
			cout << "debug: password = " << *password << endl;
			cout << "debug: name = " << *name << endl;

			if (userstorage.registerUser(*login, *password, *name)){
				send(client_fd, new char('0'), 1, 0); // успешно
				cout << "user added successfully!" << endl;
			}
			else{
				send(client_fd, new char('1'), 1, 0); // неудачно
				cout << "user not added." << endl;
			}
			break;
		}

		case 'l': // send in Local message
		{
			int words_count;
			string message;
			string login_sender;
			string login_recipient;
			iss >> message; // скипаем первый символ

			iss >> words_count;
			iss >> login_sender;
			iss >> login_recipient;
			iss >> message;
			for (int i = 0; i < words_count - 1; i++)
			{
				string str_buf;
				iss >> str_buf;
				message = message + " " + str_buf;
			}

			try
			{
				cout << "debug: words_count = " << words_count << endl;
				cout << "debug: login_sender = " << login_sender << endl;
				cout << "debug: login_recipient = " << login_recipient << endl;
				cout << "debug: message = " << message << endl;

				LocalMessage* lm = lmstorage.getLM(login_sender, login_recipient);
				if (lm != nullptr) { lm->PrintAllMessage(); }
				else 
				{ 
					lmstorage.addLM(userstorage.get_user(login_sender), userstorage.get_user(login_recipient)); 
				}
				lmstorage.getLM(login_sender, login_recipient)->SendMessage(login_sender, message);

				send(client_fd, new char('0'), 1, 0);
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
				send(client_fd, new char('1'), 1, 0);
			}

			break;
		}

		case 'g': // send in Global message
		{
			int words_count;
			string message;
			string login_sender;
			iss >> message; // скипаем первый символ

			iss >> words_count;
			iss >> login_sender;
			iss >> message;
			for (int i = 0; i < words_count - 1; i++)
			{
				string str_buf;
				iss >> str_buf;
				message = message + " " + str_buf;
			}
			
			cout << "debug: words_count = " << words_count << endl;
			cout << "debug: login_sender = " << login_sender << endl;
			cout << "debug: message = " << message << endl;

			if(!userstorage.get_user(login_sender))
				send(client_fd, new char('1'), 1, 0);
			else
			{
				globalMessage.SendMessage(login_sender, message);
				send(client_fd, new char('0'), 1, 0);
			}
			break;
		}
		
		case 'p': // send local message text to user
		{
			string login1;
			string login2;
			iss >> login1; // скипаем

			iss >> login1;
			iss >> login2;

			cout << "debug: login1 = " << login1 << endl;
			cout << "debug: login2 = " << login2 << endl;

			try
			{
				LocalMessage* lm = lmstorage.getLM(login1, login2);
				string answer = lm->getAllMsg();
			
				send(client_fd, &answer[0], answer.size(), 0);
			}
			catch(const std::exception& e)
			{
				cerr << e.what() << '\n';
				string answer = "Login is not valid";
				send(client_fd, &answer[0], answer.size(), 0);
			}		

			break;
		}
		
		case 'P': // send global message text to user
		{
			string tag;
			iss >> tag; // скипаем

			string answer = globalMessage.createStringChat();

			send(client_fd, &answer[0], answer.size(), 0);
			
			break;
		}
		
		default:
			break;
		}		

		// 7. Закрываем соединение
		close(client_fd);
		//close(server_socket);
		cout << "\nRestart cycle\n" << endl;
	}
    return 0;
}
