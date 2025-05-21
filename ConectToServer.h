#pragma once
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>




class ConectToServer // класс для взаимодействия с сервером и соединением
{
    private:
        std::string SERVER_IP = "127.0.0.1";
        sockaddr_in _server_addr;
        int _socket;

        char buffer[1024];
        
    public:
        int PORT = 8080;

        ConectToServer(/* args */);
        ~ConectToServer();
        
        int initSocket();
        int Conect();
        //int setSocketData();

        int userRegistration(std::string login, std::string password, std::string name);
        int login(std::string login, std::string password, std::string name);
        int sendLM(std::string message, std::string login_sender, std::string login_recipient);
        int sendGM(std::string message, std::string login_sender);
};
