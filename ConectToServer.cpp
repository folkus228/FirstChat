#include "ConectToServer.h"
#include <iostream>
#include <string>
#include <algorithm>

ConectToServer::ConectToServer()
{
	initAddr();
}

ConectToServer::~ConectToServer()
{
	
}

int ConectToServer::initAddr()
{
	// 2. Устанавливаем адрес сервера
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_port = htons(PORT);
	if (inet_pton(AF_INET, SERVER_IP.c_str(), &_server_addr.sin_addr) <= 0) {
		std::cerr << "Invalid address or address not supported\n";
		return -1;
	}
	return 0;
}

int ConectToServer::initSocket()
{
	// 1. Создаем сокет
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1) {
		std::cerr << "Socket creation failed\n";
		return -1;
	}

	return 0;
}

int ConectToServer::userRegistration(std::string login, std::string password, std::string name)
{
	try
	{	
		std::cout << "create request ..." << std::endl;
		std::string request = "c " + login + " " + password + " " + name;
		std::cout << "send request" << std::endl;
		initSocket();
		connect(_socket, (struct sockaddr*)&_server_addr, sizeof(_server_addr));
		send(_socket, &request.c_str()[0], request.size(), 0);
		std::cout << "wait for answer ..." << std::endl;
		ssize_t len = recv(_socket, buffer, sizeof(buffer), 0);
		close(_socket);
		std::cout << "response received: " << (int)buffer[0] - 48 << std::endl;
		return (int)buffer[0] - 48; // 0 - удачно  1 - неудачно
		
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	return 1;
}

std::string ConectToServer::login(std::string login, std::string password)
{
	std::string request = "i " + login + " " + password;
	initSocket();
	connect(_socket, (struct sockaddr*)&_server_addr, sizeof(_server_addr));
	send(_socket, request.c_str(), request.size(), 0);
	ssize_t len = recv(_socket, buffer, sizeof(buffer), 0);
	close(_socket);
	if (buffer[0] == '0') // 0 - удачно  1 - нету пользователя 2 - неверный пароль
	{
		return std::string(buffer[2], len-2);
	}
	else if (buffer[0] == '1')
	{
		return std::string("1");
	}
	else
		return std::string("2");
}

int ConectToServer::sendLM(std::string message, std::string login_sender, std::string login_recipient)
{// расчет на неповторяющиеся пробелы
	int word_count = std::count(message.begin(), message.end(), ' ') + 1;
	std::string request = "l " + std::to_string(word_count) + " " + login_sender + " " + login_recipient + " " + message;
	initSocket();
	connect(_socket, (struct sockaddr*)&_server_addr, sizeof(_server_addr));
	send(_socket, request.c_str(), request.size(), 0);
	ssize_t len = recv(_socket, buffer, sizeof(buffer), 0);
	close(_socket);
	return (int)buffer[0] - 48; // 0 - удачно  1 - неудачно
}

int ConectToServer::sendGM(std::string message, std::string login_sender)
{// расчет на неповторяющиеся пробелы
	int word_count = std::count(message.begin(), message.end(), ' ') + 1;
	std::string request = "g " + std::to_string(word_count) + " " + login_sender + " " + message;
	initSocket();
	connect(_socket, (struct sockaddr*)&_server_addr, sizeof(_server_addr));
	send(_socket, request.c_str(), request.size(), 0);
	ssize_t len = recv(_socket, buffer, sizeof(buffer), 0);
	close(_socket);
	return (int)buffer[0] - 48; // 0 - удачно  1 - неудачно
}

std::string ConectToServer::getLM(std::string login1, std::string login2)
{
	std::string request = "p " + login1 + " " + login2;
	initSocket();
	connect(_socket, (struct sockaddr*)&_server_addr, sizeof(_server_addr));
	send(_socket, request.c_str(), request.size(), 0);
	ssize_t len = recv(_socket, buffer, sizeof(buffer), 0);
	close(_socket);

	return std::string(buffer, len);
}

std::string ConectToServer::getGM()
{
	std::string request = "P";
	initSocket();
	connect(_socket, (struct sockaddr*)&_server_addr, sizeof(_server_addr));
	send(_socket, request.c_str(), request.size(), 0);
	ssize_t len = recv(_socket, buffer, sizeof(buffer), 0);
	close(_socket);

	return std::string(buffer, len);
}