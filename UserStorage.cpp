#include "UserStorage.h"
#include<vector>

UserStorage::UserStorage() {}

UserStorage::~UserStorage() 
{
    for (int i = 0; i < users.size(); i++)
    {
        delete users[i];
    }
}

bool UserStorage::registerUser(const std::string& login, const std::string& password, const std::string& name) // создает пользователя // если может то true если нет то false
{
    users.push_back(new User(login ,password, name));
    for (int i = 0; i < users.size()-1; i++)
    {    
        if (users[i]->get_login() == users[users.size() - 1]->get_login())
        {
            users.pop_back();
            return false;
        }
    }
    
    return true;
}

int UserStorage::get_length()
{
    return users.size();
}

User* UserStorage::get_user(const std::string& login)
{

    for (int i = 0; i < users.size(); i++)
    {
        if (users[i]->get_login() == login)
        {
            return users[i];; // если успешно то возвращает номер user'а из массива
        }
    }
    return nullptr; // возвращает при ошибке
}

User* UserStorage::get_user(const std::string&& login)
{
    for (int i = 0; i < users.size(); i++)
    {
        if (users[i]->get_login() == login)
        {
            return users[i];; // если успешно то возвращает номер user'а из массива
        }
    }
    return nullptr; // возвращает при ошибке
    
}

User* UserStorage::login(const std::string& login, const std::string& password) {
    for (int i = 0; i < users.size(); i++) {
        if (users[i]->get_login() == login && users[i]->get_password() == password) {
            std::cout << "Welcome, " << users[i]->get_name() << "!" << std::endl;
            return users[i];
        }
    }
    throw std::runtime_error("Invalid login or password");
}

User* UserStorage::operator[](int i) // возвращает ссылку, но есл нет доступа то nullptr
{
    if (i < users.size())
        return users[i];
    return nullptr;
}
