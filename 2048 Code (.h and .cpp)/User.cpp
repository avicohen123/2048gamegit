#include "User.h"
#include "Board.h"
#include <string>
#include <vector>
#include <iostream>

User::User()
{
	gameType = '\0';
	yourUsername = "";
}
User::~User() {}
void User::advancedOrBasic() 
{
	cout << "How would you like to play the game.\nPress A to play as an Advanced User or press B to play as a basic user:";
	cin >> gameType;

	gameType = toupper(gameType);

	if (gameType == 'A')
	{
		//go to sign up or login
		signupOrLogin();
	}
	else if (gameType == 'B')
	{
		//set default username
		yourUsername = "Basic User";
		//forward to the main menu
		basicMenu();
	}
	else
	{
		cout << "\nPlease enter a correct value for the program mode and try again.\n\n";
		advancedOrBasic();
	}
}
char User::getAdvancedOrBasic() 
{
	return gameType;
}
string User::getUserName() 
{
	return yourUsername;
}
//asks the user for signup or logic
void User::signupOrLogin() 
{
	//ask to signup or login
	cout << "\nPress S to signup or L to login:";
	cin >> signupOrLoginChar;

	signupOrLoginChar = toupper(signupOrLoginChar);

	if (signupOrLoginChar == 'S')
	{
		signup();
	}
	else if (signupOrLoginChar == 'L')
	{
		login();
	}
	else
	{
		cout << "\nPlease enter a correct value for the signup or Login.\n";
		signupOrLogin();
	}
}
//asks the user for signup details
void User::signup()
{

	//ask user for user name and password
	cout << "\nPlease enter a username and password:";
	cin >> yourUsername >> yourPassword;

	//check if username exists
	inputUsernamesAndPasswords.open("userInfo.txt");
	while (inputUsernamesAndPasswords >> username >> password) 
	{
		if (yourUsername == username)
		{
			cout << "That username already exists. Please select a different username";
			inputUsernamesAndPasswords.close();
			signup();
		}
	}
	inputUsernamesAndPasswords.close();
	//open userInfo .txt file
	outputUsernamesAndPasswords.open("userInfo.txt", std::ios_base::app);
	outputUsernamesAndPasswords << left << setw(15) << yourUsername << setw(15) << yourPassword << endl;

	//create a file for the user
	createUserFile();

	//close file
	outputUsernamesAndPasswords.close();

	//forward to main
	advancedMenu();

}
//asks the user for login details
void User::login()
{
	bool correctUsernameAndPassword=false;
	//open userInfo .txt file
	inputUsernamesAndPasswords.open("userInfo.txt");
	//ask for login credentials
	cout << "\nPlease enter you username and password:";

	cin >> yourUsername >> yourPassword;

	//go back to beginning of file for potential new search
	inputUsernamesAndPasswords.clear();
	inputUsernamesAndPasswords.seekg(0, ios::beg);

	//check login credentials with file
	while (inputUsernamesAndPasswords >> username >> password)
	{
		if (username == yourUsername && password == yourPassword)
		{
			correctUsernameAndPassword = true;
		}
	}
	//close .txt file
	inputUsernamesAndPasswords.close();

	if (correctUsernameAndPassword == true)
	{
		advancedMenu();
	}
	else 
	{
		cout << "\nyour username and password were incorrect.";
		signupOrLogin();
	}
}
//this will create a user file with the naming scheme username.txt
void User::createUserFile()
{
	//create and open outputUserFile
	ofstream outputUserFile;
	outputUserFile.open(yourUsername + ".txt");

	//close user file
	outputUserFile.close();
}
//basic menu for basic users
void User::basicMenu()
{
	cout << "Main Menu:\nPress S to play 2048\nPress R to check the ranking list:";
	cin >> mainMenuSelect;

	mainMenuSelect = toupper(mainMenuSelect);

	if (mainMenuSelect == 'S')
	{
		//run game
		gameSetup();
	}
	else if (mainMenuSelect == 'R')
	{
		//check the ranking list
		printRankingList();
	}
	else
	{
		cout << "\nPlease try again and enter a valid value.\n";
		basicMenu();
	}
}
//advanced menu for advanced users
void User::advancedMenu()
{
	cout << "Main Menu:\nPress S to play 2048\nPress H to print game history\nPress B to print best record\nPress R to check ranking list:";
	cin >> mainMenuSelect;

	mainMenuSelect = toupper(mainMenuSelect);

	if (mainMenuSelect == 'S')
	{
		gameSetup();
	}
	else if (mainMenuSelect == 'H')
	{
		printGameHistory();
	}
	else if (mainMenuSelect == 'B')
	{
		printBestRecord();
	}
	else if (mainMenuSelect == 'R')
	{
		printRankingList();
	}
	else 
	{
		cout << "\nPlease try again and enter a valid value.\n";
		advancedMenu();
	}
}
//this will print the ranking list (both basic and advanced users can access this
void User::printRankingList()
{
	//set variables
	string name, data, password;
	int score, step, bestScore(0), bestStep(0), userNumber(0);
	int numberOfUsers(0);

	//get number of users and create a vector of a vector (nets a 2d vector)
	//of type string with the rows as the user and column as username, score, and step
	inputUsernamesAndPasswords.open("userInfo.txt");
	while (getline(inputUsernamesAndPasswords, data))
	{
		numberOfUsers++;
	}
	vector<vector<string> > ranking;
	ranking.resize(numberOfUsers);
	for (auto &i : ranking)
	{
		i.resize(3);
	}	

	//clear eof and go back to beginning of file
	inputUsernamesAndPasswords.clear();
	inputUsernamesAndPasswords.seekg(0, ios::beg);
//!!!!! Maybe have method for checking for error when opening file
	//check if error opening file
	if (inputUsernamesAndPasswords.fail())
	{
		cerr << "error opening file \n";
		exit(1);
	}
//!!!!! Maybe have method for checking for error when opening file

	//get each username and open their individual file to find their best score. add it to the vector.
	while (inputUsernamesAndPasswords >> name >> password)
	{
		readUserFile.open(name + ".txt");
		if (readUserFile.fail())
		{
			cerr << "error opening file \n";
		}
		else
		{
			while (readUserFile >> score >> step)
			{
				if (score > bestScore)
				{
					bestScore = score;
					bestStep = step;
				}
				else if (score == bestScore)
				{
					if (step < bestStep)
					{
						bestStep = step;
					}
				}
			}
		}
		readUserFile.close();

		ranking[userNumber][0] = name;
		ranking[userNumber][1] = to_string(bestScore);
		ranking[userNumber][2] = to_string(bestStep);

		bestScore = 0, bestStep = 0;

		userNumber++;

	}
	inputUsernamesAndPasswords.close();

	//use sort to arrange the players in the right order
	for (int i = 0; i < numberOfUsers; i++)
	{
		for (int j = i+1; j < numberOfUsers; j++)
		{
			if (atoi(ranking[i][1].c_str()) < atoi(ranking[j][1].c_str()))
			{
				for (int t = 0; t < 3; t++)
				{
					string tempArray[3];
					tempArray[t] = ranking[j][t];
					ranking[j][t] = ranking[i][t];
					ranking[i][t] = tempArray[t];
				}
			}
			//if score is the same check step
			else if (atoi(ranking[i][1].c_str()) == atoi(ranking[j][1].c_str()))
			{
				if (atoi(ranking[i][2].c_str()) > atoi(ranking[j][2].c_str()))
				{
					for (int t = 0; t < 3; t++)
					{
						string tempArray[3];
						tempArray[t] = ranking[j][t];
						ranking[j][t] = ranking[i][t];
						ranking[i][t] = tempArray[t];
					}
				}
				//if score and step are the same
				else if (atoi(ranking[i][2].c_str()) == atoi(ranking[j][2].c_str()))
				{
					if (atoi(ranking[i][0].c_str()) < atoi(ranking[j][0].c_str()))
					{
						for (int t = 0; t < 3; t++)
						{
							string tempArray[3];
							tempArray[t] = ranking[j][t];
							ranking[j][t] = ranking[i][t];
							ranking[i][t] = tempArray[t];
						}
					}
				}
			}
		}
	}
	// add the rank # and print out ranking list
	cout << "Ranking List:\n\n";
	cout << left << setw(15) << "Ranking #" << setw(15) << "Name" << setw(15) << "Score" << setw(15) << "Step #" << "\n";
	int numOfSameScore(0);
	for (int i = 0; i < numberOfUsers; i++)
	{
		if (i>0 && ranking[i][1] == ranking[i - 1][1] && ranking[i][2] == ranking[i - 1][2])
		{
			numOfSameScore++;
			cout << left << setw(15) << i - numOfSameScore + 1 << setw(15) << ranking[i][0] << setw(15) << ranking[i][1] << setw(15) << ranking[i][2] << "\n";
		}
		else 
		{
			numOfSameScore=0;
			cout<< left << setw(15) << i+1 << setw(15) << ranking[i][0] << setw(15) << ranking[i][1] << setw(15) << ranking[i][2] << "\n";
		}
	}
	forwardToMenu();
}
//this will print the game history
void User::printGameHistory() 
{
	int gameNumber = 1, step, score;

	ifstream outputUserFile(yourUsername + ".txt");
	cout<<"\n"<< left << setw(15) << "Game Number" << setw(15) << "Score" << setw(15) << "Step #" << "\n";
	while (outputUserFile >> score >> step)
	{
		cout << left << setw(15) << gameNumber << setw(15) << score << setw(15) << step << "\n";
		gameNumber++;
	}
	if(gameNumber==1)
	{
		cout << endl << "You have no past game history." << endl<<endl;
	}
	outputUserFile.close();
	forwardToMenu();
}
//this will print the best record
void User::printBestRecord() 
{
	int step, score, gameNumber=1, bestGameNumber=0, bestScore = 0, beststep = 0;
	ifstream outputUserFile(yourUsername + ".txt");
	cout << "\n" << left << setw(20) << "Best Game Number" << setw(20) << "Best Score" << setw(20) << "Best Step #" << "\n";
	while (outputUserFile >> score >> step)
	{
		if (score > bestScore)
		{
			bestScore = score;
			beststep = step;
			bestGameNumber = gameNumber;
		}
		else if(score == bestScore) 
		{
			if (step < beststep)
			{
				bestScore = score;
				beststep = step;
				bestGameNumber = gameNumber;
			}
		}
		gameNumber++;
	}
	if(bestGameNumber==0)
	{
		cout << endl << "You have not played any games yet" << endl<<endl;
	}
	else 
	{
		cout << left << setw(20) << bestGameNumber << setw(20) << bestScore << setw(20) << beststep << "\n";
	}
	outputUserFile.close();
	forwardToMenu();
}
//this initializes the board object
void User::gameSetup() 
{
	Board myBoard(gameType,yourUsername);
}
//depending on user will forward to different menu
void User::forwardToMenu() 
{
	if (gameType == 'A') 
	{
		advancedMenu();
	}
	else 
	{
		basicMenu();
	}
}