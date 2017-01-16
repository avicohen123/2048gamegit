#pragma once
#include <fstream>
#include <iomanip>

using namespace std;

class User
{
public:
	User();
	~ User();
	void advancedOrBasic();
	char getAdvancedOrBasic();
	string getUserName();
private:
	char gameType,signupOrLoginChar, mainMenuSelect;
	string yourUsername, yourPassword, username, password;
	ifstream inputUsernamesAndPasswords, readUserFile;
	ofstream outputUsernamesAndPasswords, outputUserFile;
	void signupOrLogin();
	void signup();
	void login();
	void createUserFile();
	void basicMenu();
	void advancedMenu();
	void printRankingList();
	void printGameHistory();
	void printBestRecord();
	void gameSetup();
	void forwardToMenu();
};