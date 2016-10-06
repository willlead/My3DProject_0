#pragma once


//user.h

// network.h

class Select
{
public:
	User m_SessionList[64];
	std::list<User*> m_pUserList;
public: 
	bool InitSelectServert();
	bool Run();
	User* GetSession();
	bool RemvoeSession(User* pUser);
public:
	Select();
	virtual ~Select();
};

