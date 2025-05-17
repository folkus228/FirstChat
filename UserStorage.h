 #pragma once
#include "User.h"
#include <string>
#include <vector>
#include <iostream>
class UserStorage : public User {
private:
    std::vector<User*> users;
    int _userValue = 0;
public:
    UserStorage();
    ~UserStorage();

    bool registerUser(const std::string& login, const std::string& password, const std::string& name);

    User* login(const std::string& login, const std::string& password);

    int get_length();
    User* get_user(const std::string& login);
    User* get_user(const std::string&& login);

    void  setUserValue(int userValue) { _userValue = userValue; }

    User* operator[](int i);
};