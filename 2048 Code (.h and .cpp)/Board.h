#pragma once
#include <fstream>
#include <vector>
#include <stack>

using namespace std;

class Board
{
public:
	Board(char gameType, string userName);
	~Board();
private:
	vector<vector<int> > board;
	char gameType, moveInput;
	string userName;
	int score, step;
	stack <vector<vector<int> > > oldBoards;
	ofstream outputScore;
	bool goodMove;
	bool placeRandomValue(int value);
	void move();
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	int getScore();
	void printBoard();
	bool checkWinOrLose();
	void addToStack();
	void undo();
	int randomNumber();
	void addResultsToFile();
};