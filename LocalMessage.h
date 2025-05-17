#pragma once
#include "User.h"
#include <string>
#include <vector>
#include <memory>

struct Message
{
	Message(std::string name, std::string login, std::string message);
	std::string _name;
	std::string _login;
	std::string _message;
};

class LocalMessage
{
private:
	std::vector<Message*> _message;
public:
	User* _user1;
	User* _user2;
	LocalMessage();
	LocalMessage(User* user1, User* user2);
	~LocalMessage();



	void PrintAllMessage();
	void SendMessage(std::string login, std::string message);
};

