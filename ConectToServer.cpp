#include "ConectToServer.h"
#include <iostream>
#include <string>
#include <algorithm>

ConectToServer::ConectToServer()
{

}

ConectToServer::~ConectToServer()
{
	
}

int ConectToServer::initSocket()
{
	// 1. Создаем сокет
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1) {
		std::cerr << "Socket creation failed\n";
		return -1;
	}

	// 2. Устанавливаем адрес сервера
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_port = htons(PORT);
	if (inet_pton(AF_INET, SERVER_IP.c_str(), &_server_addr.sin_addr) <= 0) {
		std::cerr << "Invalid address or address not supported\n";
		return -1;
	}

	return 0;
}

int ConectToServer::Conect()
{
	return 0;
}
//int setSocketData();

int ConectToServer::userRegistration(std::string login, std::string password, std::string name)
{
	std::string request = "c " + login + " " + password + " " + name;
	send(_socket, request.c_str(), request.size(), 0);
	ssize_t len = recv(_socket, buffer, sizeof(buffer), 0);
	return (int)buffer[0]; // 0 - удачно  1 - неудачно
}

int ConectToServer::login(std::string login, std::string password)
{
	std::string request = "i " + login + " " + password;
	send(_socket, request.c_str(), request.size(), 0);
	ssize_t len = recv(_socket, buffer, sizeof(buffer), 0);
	return (int)buffer[0]; // 0 - удачно  1 - неудачно 2 - неверный пароль
}

int ConectToServer::sendLM(std::string message, std::string login_sender, std::string login_recipient)
{// расчет на неповторяющиеся пробелы
	int word_count = std::count(message.begin(), message.end(), ' ') + 1;
	const char* TEST_VALUE = (const char*)word_count;
	std::cout << "TEST_VALUE = " << TEST_VALUE << std::endl;
	std::string request = (const char*)word_count + login_sender + " " + login_recipient + " " + message;
	send(_socket, request.c_str(), request.size(), 0);
	ssize_t len = recv(_socket, buffer, sizeof(buffer), 0);
	return (int)buffer[0]; // 0 - удачно  1 - неудачно
}

int ConectToServer::sendGM(std::string message, std::string login_sender)
{
	return 1;
}