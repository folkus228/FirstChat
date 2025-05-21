#include "LocalMessage.h"
#include "LMStorage.h"

void LMStorage::addLM(User* user1, User* user2) // �������� ���������
{
	lms.push_back(new LocalMessage(user1, user2));
}

std::vector<LocalMessage*> LMStorage::getAllLM(std::string login) // ���������� ��� ������ ��������� ������������
{
	std::vector<LocalMessage*> returnLM;
	for (size_t i = 0; i < lms.size(); i++)
	{
		if (lms[i]->_user1->get_login() == login)
		{
			returnLM.push_back(lms[i]);
		}
		else if (lms[i]->_user2->get_login() == login)
		{
			returnLM.push_back(lms[i]);
		}
	}
	return returnLM;
}

LocalMessage* LMStorage::getLM(std::string login1, std::string login2) // ���������� ���������� ���������
{
	for (size_t i = 0; i < lms.size(); i++)
	{
		if (lms[i]->_user1->get_login() == login1 && lms[i]->_user2->get_login() == login2)
			return lms[i];
		else if (lms[i]->_user1->get_login() == login2 && lms[i]->_user2->get_login() == login1)
			return lms[i];
	}
	return nullptr;
}

LMStorage::~LMStorage()
{
	for (size_t i = 0; i < lms.size(); i++)
	{
		delete lms[i];
	}
}