#pragma once
#include "LocalMessage.h"
#include <vector>
class LMStorage
{
private:
	std::vector<LocalMessage*> lms;
public:
	//LMStorage() = default;
	~LMStorage();

	void addLM(User* user1, User* user2);

	std::vector<LocalMessage*> getAllLM(std::string login);
	LocalMessage* getLM(std::string login1, std::string login2);
};

