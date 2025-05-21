#include "GlobalMessage.h"

GlobalMessage::GlobalMessage(UserStorage* userstorage) : _userstorage(userstorage) {}

void GlobalMessage::SendMessage(std::string login, std::string message)
{
	_vm.push_back(GMessage(_userstorage->get_user(login)->get_name(), _userstorage->get_user(login)->get_login(), message));
}

std::string& GlobalMessage::createStringChat()
{
	std::string* result = new std::string;
	*result = "";
	for (size_t i = 0; i < _vm.size(); i++)
	{
		*result += _vm[i]._name + ": " + _vm[i]._message + "\n";
	}
	return *result;
}

void GlobalMessage::PrintAll()
{
	for (size_t i = 0; i < _vm.size(); i++)
	{
		std::cout << _vm[i]._name << ": " << _vm[i]._message << std::endl;
	}
}

GMessage::GMessage(std::string name, std::string login, std::string message) : _name(name), _login(login), _message(message)
{
}
