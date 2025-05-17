#include"LocalMessage.h"
#include<iostream>
LocalMessage::LocalMessage()
{
}
LocalMessage::LocalMessage(User* user1, User* user2) : _user1(user1), _user2(user2){}

LocalMessage::~LocalMessage()
{
	for (int i = 0; i < _message.size(); i++)
	{
		delete _message[i];
	}
}

void LocalMessage::PrintAllMessage()
{
	if (!(0 < _message.size()))
		std::cout << "ERR: MESSAGE IS NOT INITIALIZE!" << std::endl;
	for (int i = 0; i < _message.size(); i++)
	{
		std::cout << _message[i]->_name << ": " << _message[i]->_message << std::endl;
	}
}

void LocalMessage::SendMessage(std::string login, std::string message)
{
	_message.push_back(_user1->get_login() == login ? new Message(_user1->get_name(), _user1->get_login(), message) : new Message(_user2->get_name(), _user2->get_login(), message));
}

Message::Message(std::string name, std::string login, std::string message) : _name(name), _login(login), _message(message) {}
