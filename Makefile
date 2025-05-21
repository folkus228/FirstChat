all: main

main:	client.cpp UserStorage LocalMessage LMStorage GlobalMessage ConectToServer
		g++ -o client client.cpp -L. -lUser -lUserStorage -lLocalMessage -lLMStorage -lGlobalMessage -lConectToServer
		g++ -o server server.cpp -L. -lUser -lUserStorage -lLMStorage -lLocalMessage -lGlobalMessage


ConectToServer:	ConectToServer.h ConectToServer.cpp
	g++ -o ConectToServer.o ConectToServer.cpp -c
	ar rc libConectToServer.a ConectToServer.o

GlobalMessage:	UserStorage GlobalMessage.h GlobalMessage.cpp
	g++ -o GlobalMessage.o GlobalMessage.cpp -c
	ar rc libGlobalMessage.a GlobalMessage.o


LMStorage:	LocalMessage
	g++ -o LMStorage.o LMStorage.cpp -c
	ar rc libLMStorage.a LMStorage.o


LocalMessage:	User LocalMessage.h LocalMessage.cpp
	g++ -o LocalMessage.o LocalMessage.cpp -c
	ar rc libLocalMessage.a LocalMessage.o


UserStorage:	User UserStorage.h UserStorage.cpp
	g++ -o UserStorage.o UserStorage.cpp -c
	ar rc libUserStorage.a UserStorage.o


User:	User.h
	g++ -o User.o User.cpp -c
	ar rcs libUser.a User.o

clean:
	rm *.o
	rm *.a
