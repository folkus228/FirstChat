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

	std::cout << "Server is listening on port " << PORT << "...\n";

	char buffer[1024];
	sockaddr_in client_addr;
	while (true)
	{
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
			if (user == nullptr)
				send(client_fd, new char('1'), 1, 0); // неудачно
			else if(user->get_password() == password) { // вход
				string message = "0 " + user->get_name();
				send(client_fd, &message[0], message.size(), 0);
			}
			else
				send(client_fd, new char('2'), 1, 0); // неудачно

			break;
		}

		case 'c': // Create new user
		{
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

			if (userstorage.registerUser(*login, *password, *name))
				send(client_fd, new char('0'), 1, 0); // успешно
			else
				send(client_fd, new char('1'), 1, 0); // неудачно

			break;
		}
		case 'l': // send Local message
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
			
			cout << "debug: words_count = " << words_count << endl;
			cout << "debug: login_sender = " << login_sender << endl;
			cout << "debug: login_recipient = " << login_recipient << endl;
			cout << "debug: message = " << message << endl;

			break;
		}

		case 'g': // send Global message
		{
			string curent_s;
			iss >> curent_s;
			cout << "debug: curent_s = " << curent_s << endl;
			break;
		}
		
		case 'p': // send local message
		{
			string curent_s;
			iss >> curent_s;
			cout << "debug: curent_s = " << curent_s << endl;
			break;
		}
		
		case 'P': // send global message
		{
			string curent_s;
			iss >> curent_s;
			cout << "debug: curent_s = " << curent_s << endl;
			break;
		}
		
		default:
			break;
		}

		// Отправляем подтверждение обратно клиенту
		std::string response = "Received: " + std::string(buffer);
		send(client_fd, response.c_str(), response.size(), 0);
		

		// 7. Закрываем соединение
		close(client_fd);
		close(server_socket);
		cout << "\nRestart cycle\n" << endl;
	}
    return 0;
}
