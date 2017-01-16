#include "Board.h"
#include "User.h"
#include <string>
#include <vector>
#include <iostream>
#include <time.h>
#include <stack>

using namespace std;
//constructor initialize board with 2 tiles with value 2, and will start the game
Board::Board(char gameType, string userName) 
{
	step = 0;

	this->gameType = gameType;
	this->userName = userName;

	//create the 4x4 board
	board.resize(4, vector<int>(4, 0));

	//add the 2 titles with value 2, randomly on the board
	placeRandomValue(2);
	placeRandomValue(2);

	//get score
	score = getScore();

	//print board
	printBoard();

	//start game
	move();
	
}
//destruct
Board::~Board(){}
//will generate either a 2 or 4 randomly 
int Board::randomNumber()
{
	int twoOrFour;
	srand(time(NULL));
	twoOrFour = rand()%2;
	if (twoOrFour == 0)
	{
		return 2;
	}
	else 
	{
		return 4;
	}
}
//add a tile, and if no empty tile and no moves to make, end the game
bool Board::placeRandomValue(int value) 
{
	//create a temp vector which holds the indices's of the zero's of the array
	vector<int> temp;

	bool isWin;

	int location, row, column;

	//find all elements with 0
	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++)
		{
			if (board[i][j] == 0) 
			{
				temp.push_back(i*4+j);
			}
		}
	}

	//check if player has lost
	if(temp.size() > 0)
	{
		//randomly choose one
		srand(time(NULL));

		//get the location
		location = temp[rand() % (temp.size())];

		row = location / 4;
		column = location % 4;

		board[row][column] = value;

		isWin = checkWinOrLose();
		if ((temp.size() == 1) && isWin == false)
		{
			return false;
		}
		else 
		{
			return true;
		}
	}
	else 
	{
		isWin = checkWinOrLose();
		if ((temp.size() == 0) && isWin == false)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	
}
//will print the board (slightly different board based on if the user is advanced or basic)
void Board::printBoard() 
{
	system("cls");

	if (gameType == 'B') 
	{
		cout << "________________________________________________________________________" << endl;
		cout << "Welcome Player : " << userName << "\tCurrent Score: " << score << "\tCurrent Step: " << step<<endl;
		cout << "________________________________________________________________________" << endl<<endl<<endl;
		for (int i = 0; i < 4; i++)
		{
			cout << "\t\t";
			for (int j = 0; j < 4; j++)
			{
				cout << left << setw(10) << board[i][j];
			}
			cout << endl << endl;
		}
		cout << "________________________________________________________________________" << endl;
		cout << "W = UP\t\tS = DOWN\tA = LEFT\tD = RIGHT\tE= EXIT" << setw(20) << endl;
		cout << "________________________________________________________________________" << endl;
	}
	else 
	{
		cout << "_________________________________________________________________________________________" << endl;
		cout << "Welcome Player: " << userName << "\t\t\tCurrent Score: " << score << "\t\tCurrent Step: " << step << endl;
		cout << "_________________________________________________________________________________________" << endl;
		for (int i = 0; i < 4; i++)
		{
			cout << "\t\t\t\t";
			for (int j = 0; j < 4; j++)
			{
				cout << left << setw(10) << board[i][j];
			}
			cout << endl << endl;
		}
		cout << "________________________________________________________________________________________" << endl;
		cout << left << setw(20) << "W = UP\t\tS = DOWN\tA = LEFT\tD = RIGHT\tE= EXIT\t\tU=UNDO" << setw(20) << endl;
		cout << "________________________________________________________________________________________" << endl;
	}
}
//will return the current score of the board
int Board::getScore() 
{
	int highestTile(0);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (board[i][j] > highestTile) 
			{
				highestTile = board[i][j];
			}
		}
	}
	return highestTile;
}
//will return true if a potential move can be made, otherwise will return false
bool Board::checkWinOrLose() 
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if ((i >= 1) && (board[i][j] == board[i - 1][j]))
			{
				return true;
			}
			if ((j >= 1) && (board[i][j] == board[i][j - 1])) 
			{
				return true;
			}
		}
	}
	return false;
}
//undo the last change
void Board::undo() 
{
	//make board equal to the last change then delete the last change
	board = oldBoards.top();
	oldBoards.pop();
}
//will add the score and step number to the users file
void Board::addResultsToFile() 
{
	if (gameType == 'A') 
	{
		outputScore.open(userName + ".txt", std::ios_base::app);
		outputScore << score << " " << step << endl;
		outputScore.close();
	}
}
//main logic of the board
void Board::move() 
{
	bool areThereMoreMoves(true), didUndo;
	while (areThereMoreMoves) 
	{
		cout << "Please enter your move:";
		cin >> moveInput;
		moveInput = toupper(moveInput);
		didUndo = false;
		goodMove = false;
		//what direction do you want to move
		switch (moveInput)
		{
		case 'W':
			addToStack();
			moveUp();
			break;
		case 'A':
			addToStack();
			moveLeft();
			break;
		case 'S':
			addToStack();
			moveDown();
			break;
		case 'D':
			addToStack();
			moveRight();
			break;
		case 'E':
			addResultsToFile();
			exit(1);
		case 'U':
			if ((gameType == 'A') && (step > 0))
			{
				didUndo = true;
				undo();
			}
			break;
		default:
			cout << "\nThat is not a valid Move. Please try again.\n";
		}
		//if the move was valid
		if (goodMove == true)
		{

			step++;
			score = getScore();
			areThereMoreMoves = placeRandomValue(randomNumber());
			//check to see if user lost game
			if (!areThereMoreMoves)
			{
				printBoard();
				cout << "\nYou Lost. Thanks For playing.\n";
				addResultsToFile();
			}
			else
			{
				printBoard();
			}
		}
		//if undo was selected
		else if (didUndo)
		{
			step--;
			score = getScore();
			printBoard();
		}
		else 
		{
			if ((gameType == 'A') && (oldBoards.size() > 0))
			{
				oldBoards.pop();
			}
			printBoard();
			cout << "That move isn't correct. Please make a correct move.\n";
		}
	}
}
//move all tiles up and merges applicable tiles
void Board::moveUp() 
{
	//move all tiles up
	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++) 
		{
			if (board[i][j] == 0) 
			{
				for (int t = i+1; t < 4; t++) 
				{
					if (board[t][j] > 0) 
					{
						goodMove = true;

						board[i][j] = board[t][j];
						board[t][j] = 0;
						
						break;
					}
				}
			}
		}
	}
	//merge the tiles
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if ((i < 3) && (board[i][j] != 0) && (board[i][j] == board[i + 1][j]))
			{
				goodMove = true;
				board[i][j] = board[i][j] * 2;
				for (int t = i + 1; t < 4; t++)
				{
					if (t == 3)
					{
						board[3][j] = 0;
					}
					else
					{
						board[t][j] = board[t + 1][j];
					}
				}
			}
		}
	}
}
//move all tiles left and merges applicable tiles
void Board::moveLeft() 
{
	//move all tiles left
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (board[j][i] == 0)
			{
				for (int t = i + 1; t < 4; t++)
				{
					if (board[j][t] > 0)
					{
						goodMove = true;

						board[j][i] = board[j][t];
						board[j][t] = 0;

						break;
					}
				}
			}
		}
	}
	//merge all tiles
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if ((i < 3) && (board[j][i] != 0) && (board[j][i] == board[j][i + 1]))
			{
				goodMove = true;
				board[j][i] = board[j][i] * 2;
				for (int t = i + 1; t < 4; t++)
				{
					if (t == 3)
					{
						board[j][3] = 0;
					}
					else
					{
						board[j][t] = board[j][t + 1];
					}
				}
			}
		}
	}
}
//move all tiles down and merges applicable tiles
void Board::moveDown() 
{
	//move all tiles down
	for (int i = 3; i >=0; i--)
	{
		for (int j = 3; j >= 0; j--)
		{
			if (board[i][j] == 0)
			{
				for (int t = i - 1; t >= 0; t--)
				{
					if (board[t][j] > 0)
					{
						goodMove = true;

						board[i][j] = board[t][j];
						board[t][j] = 0;

						break;
					}
				}
			}
		}
	}
	//merge all tiles
	for (int i = 3; i >= 0; i--)
	{
		for (int j = 3; j >= 0; j--)
		{
			if ((i > 0) && (board[i][j] != 0) && board[i][j] == board[i - 1][j])
			{
				goodMove = true;

				board[i][j] = board[i][j] * 2;
				for (int t = i - 1; t >= 0; t--)
				{
					if (t == 0)
					{
						board[0][j] = 0;
					}
					else
					{
						board[t][j] = board[t - 1][j];
					}
				}
			}
		}
	}
}
//move all tiles right and merges applicable tiles
void Board::moveRight() 
{
	//move all tiles right
	for (int i = 3; i >= 0; i--)
	{
		for (int j = 3; j >= 0; j--)
		{
			if (board[j][i] == 0)
			{
				for (int t = i - 1; t >= 0; t--)
				{
					if (board[j][t] > 0)
					{
						goodMove = true;

						board[j][i] = board[j][t];
						board[j][t] = 0;

						break;
					}
				}
			}
		}
	}
	printBoard();
	//merge all tiles
	for (int i = 3; i >= 0; i--)
	{
		for (int j = 3; j >= 0; j--)
		{
			if ((i > 0) && (board[j][i] != 0) && board[j][i] == board[j][i - 1])
			{
				goodMove = true;

				board[j][i] = board[j][i] * 2;
				for (int t = i - 1; t >= 0; t--)
				{
					if (t == 0)
					{
						board[j][0] = 0;
					}
					else
					{
						board[j][t] = board[j][t - 1];
					}
				}
			}
		}
	}
}
//save the board to stack (will only save if advanced user)
void Board::addToStack() 
{
	if (gameType == 'A')
	{
		oldBoards.push(board);
	}
}