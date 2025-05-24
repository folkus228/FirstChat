#pragma once
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>




class ConectToServer // класс для взаимодействия с сервером и соединением
{
    private:
        std::string SERVER_IP = "192.168.0.25";
        sockaddr_in _server_addr;
        int _socket;

        char buffer[2048];
        
    public:
        int PORT = 8080;

        ConectToServer(/* args */);
        ~ConectToServer();
        
        int initSocket();

        int userRegistration(std::string login, std::string password, std::string name);
        std::string login(std::string login, std::string password);
        int sendLM(std::string message, std::string login_sender, std::string login_recipient);
        int sendGM(std::string message, std::string login_sender);
        std::string getLM(std::string login1, std::string login2);
        std::string getGM();
};
