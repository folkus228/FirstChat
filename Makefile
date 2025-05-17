all: clean

main:	main.cpp UserStorage LocalMessage LMStorage GlobalMessage
		g++ -o main main.cpp -L. -lUser -lUserStorage -lLocalMessage -lLMStorage -lGlobalMessage



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

clean: main
	rm *.o
	rm *.a
