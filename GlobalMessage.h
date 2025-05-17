#pragma once
#include<iostream>
#include<string>
#include<vector>
#include"UserStorage.h"

struct GMessage
{
	GMessage(std::string name, std::string login, std::string message);
	std::string _name;
	std::string _login;
	std::string _message;
};

class GlobalMessage
{
private:
	UserStorage* _userstorage;
	std::vector<GMessage> _vm;
public:
	GlobalMessage(UserStorage* userstorage);

	void SendMessage(std::string login, std::string message);
	void PrintAll();
};

