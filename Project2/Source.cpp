#include<iostream>
#include<fstream>
#include<conio.h>
#include<windows.h>
#include"Header.h"
using namespace std;

int LEVEL = 1;
void setColor(unsigned short color)
{
	// SET THE COLOR ON CONSLOLE FOR GOOD INTERFACE
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void Difficulty()
{
	setColor(6);
	int level = 1;
	bool flag = false;
	while (!flag) {
		system("cls");
		cout << "PLEASE CHOOSE GAME LEVEL :\n";
		cout << "1. FOR EASY\n";
		cout << "2. FOR HARD\n";
		cout << "ENTER YOUR CHOISE:";
		setColor(14);
		cin >> level;

		if (level == 2) {
			LEVEL = LEVEL * 10;
			flag = true;
		}
		else if (level == 1) {
			LEVEL = LEVEL;
			flag = true;
		}
		else
			flag = false;
	}
}
bool validMove(int moveX[], int moveY[],int moveNum,int x, int y)
{
	for (int i = 0; i < moveNum; i++)
	{
		if (moveX[i] == x && moveY[i] == y)
			return true;
	}
	return false;
}
void border()
{
	// A SIMPLE FUNCTION TO DRAW THE BORDER
	int x1 = 380, x2 = 650, y1 = 20, y2=30;
	myRect(x1, y1, x2, y2, 139, 69, 19, 139, 69, 19);

	y1 = 20, y2 = 290, x1=380,x2= 390;
	myRect(x1, y1, x2, y2, 139, 69, 19, 139, 69, 19);

	x1 = 640, x2= 650;
	myRect(x1, y1, x2, y2, 139, 69, 19, 139, 69, 19);

	x1 = 380, x2 = 650, y1 = 280, y2 = 290;
	myRect(x1, y1, x2, y2, 139, 69, 19, 139, 69, 19);
}
void print(char board[][8], int row, int col)
{
	// DRAW THE BOARD FOR THE OTHELLO
	system("cls");
	border();
	int x1 = 400, x2 = 420, y1 = 40, y2 = 60;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (board[i][j] == 'w') { // white
				myRect(x1, y1, x2, y2, 244, 164, 96, 244, 164, 96);
				myEllipse(x1, y1, x2, y2, 0,0,0, 255, 255, 255);
			}
			else if (board[i][j] == 'b') { // black
				myRect(x1, y1, x2, y2, 244, 164, 96, 244, 164, 96);
				myEllipse(x1, y1, x2, y2, 0,0,0, 0, 0, 0);
			}
			else if (board[i][j] == '*') // tile
				myRect(x1, y1, x2, y2, 244,164,96, 244, 164, 96);

			x1 = x1 + 30;
			x2 = x2 + 30;
		}

		y1 = y1 + 30;
		y2 = y2 + 30;
		x1 = 400;
		x2 = 420;
	}
}
void readMyboard(char board[][8], const char file[])
{
	// READ THE INITIAL BOARD FROM THE .TXT FILE
	ifstream fin;
	fin.open(file);

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			fin >> board[i][j];
		}
	}
}
int score(char board[][8], char whoseTurn)
{
	// CALCULATE THE VALUE OF THE PLAYER WHOSE VALUE IS PASSED
	int s = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == whoseTurn)
				s++;

		}
	}
	return s;
}
int hueristic(char board[][8], char whoseTurn)
{
	// IF THE FLAG PASSED IS BLACK THEN THE OTHER AUTOMATICALLY BECOME OPPONENT AND REVERSE
	char opponent = 'b';
	if (whoseTurn == 'b')
		opponent = 'w';
	int ourScore = score(board, whoseTurn);
	int opponentScore = score(board, opponent);
	return (ourScore - opponentScore);

}
void copyBoard(char board[][8],char dest[][8])
{
	// COPY THE ORIGINAL BOARD TO AN OTHER BOARD FOR COMPUTATION
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			dest[i][j] = board[i][j];
		}
	}
}
void getMoveList(char board[][8], int moveX[],int moveY[],int& numMoves,char key)
{
	// GENRATE MOVE FOR WHITE

	if (key == 'w')
	{
		numMoves = 0; // NO MOVES AT THE START

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (board[i][j] == 'w') // WE GOT THE FIRST WHITE NOW WE WILL CHECK IN ALL THE DIRECTIONS FOR POSSIBLE MOVE
				{
					int x = i;
					int y = j;

					if (y >= 1 && board[x][y - 1] == 'b') // IF THERE IS BLACK ONE POINT BACKWARD
					{
						while (y >= 1 && board[x][y - 1] == 'b')
						{
							y--;
						}

						if (y >= 1 && board[x][y - 1] == '*') // IF THERE IS AN EMPTY SLOT WE CAN USE THAT MOVE
						{

							moveX[numMoves] = x;
							moveY[numMoves] = y - 1;
							numMoves++;
						}
					}

					// RESET VALUES
					x = i;
					y = j;

					if (y < 7 && board[x][y + 1] == 'b') // IF NEXT VALUE IS BLACK
					{
						while (y < 7 && board[x][y + 1] == 'b')
						{
							y++;
						}

						if (y < 7 && board[x][y + 1] == '*')
						{
							moveX[numMoves] = x;
							moveY[numMoves] = y + 1;
							numMoves++;
						}
					}

					// RESET THE VALUES
					x = i;
					y = j;

					if (x >= 1 && board[x - 1][y] == 'b') // BLACK IS PRESENT UPWARD
					{
						while (x >= 1 && board[x - 1][y] == 'b')
						{
							x--;
						}

						if (x >= 1 && board[x - 1][y] == '*') 
						{
							moveX[numMoves] = x-1;
							moveY[numMoves] = y;
							numMoves++;
						}
					}

					// RESET VALUES
					x = i;
					y = j;

					if (x < 7 && board[x + 1][y] == 'b') // BLACK IS PRESENT ONE STEP DOWNWARD
					{
						while (x < 7 && board[x + 1][y] == 'b')
						{
							x++;
						}

						if (x < 7 && board[x + 1][y] == '*') 
						{

							moveX[numMoves] = x+1;
							moveY[numMoves] = y;
							numMoves++;
						}
					}

					// RESET THE VALUES
					x = i;
					y = j;

					if (x >= 1 && y >= 1 && board[x - 1][y - 1] == 'b') // DIAGONALLY UP LEFT
					{
						while (x >= 1 && y >= 1 && board[x - 1][y - 1] == 'b')
						{
							x--;
							y--;
						}

						if (x >= 1 && y >= 1 && board[x - 1][y - 1] == '*') 
						{
							moveX[numMoves] = x-1;
							moveY[numMoves] = y - 1;
							numMoves++;
						}
					}

					// RESET THE VALUES
					x = i;
					y = j;

					if (x < 7 && y < 7 && board[x + 1][y + 1] == 'b') // DIAGONALLY DOWNWARD RIGHT
					{
						while (x < 7 && y < 7 && board[x + 1][y + 1] == 'b')
						{
							x++;
							y++;
						}

						if (x < 7 && y < 7 && board[x + 1][y + 1] == '*')
						{

							moveX[numMoves] = x+1;
							moveY[numMoves] = y + 1;
							numMoves++;
						}
					}

					// RESET THE VALUES
					x = i;
					y = j;

					if (x >= 1 && y < 7 && board[x - 1][y + 1] == 'b') // DIAGONALLY UPWARD RIGHT
					{
						while (x >= 1 && y < 7 && board[x - 1][y + 1] == 'b')
						{
							x--;
							y++;
						}

						if (x >= 1 && y < 7 && board[x - 1][y + 1] == '*')
						{

							moveX[numMoves] = x-1;
							moveY[numMoves] = y + 1;
							numMoves++;
						}
					}

					// RESET VALUES
					x = i;
					y = j;

					if (x < 7 && y >= 1 && board[x + 1][y - 1] == 'b') // DIAGONALLY DOWNWARD LEFT
					{
						while (x <7 && y >= 1 && board[x + 1][y - 1] == 'b')
						{
							x++;
							y--;
						}

						if (x <7 && y >= 1 && board[x + 1][y - 1] == '*')
						{

							moveX[numMoves] = x + 1;
							moveY[numMoves] = y - 1;
							numMoves++;
						}
					}
				}
			}
		}
	}

	// GENRATE MOVE FOR BLACK

	if (key == 'b') 
	{
		numMoves = 0;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (board[i][j] == 'b') // GOT THE FIRST BLACK NOW WE WILL KEEP CHECKING IN ALL DIRECTIONS
				{
					int x = i;
					int y = j;

					if (y >= 1 && board[x][y - 1] == 'w') // PREVIOUS
					{
						while (y >= 1 && board[x][y - 1] == 'w')
						{
							y--;
						}

						if (y >= 1 && board[x][y - 1] == '*') 
						{
							moveX[numMoves] = x;
							moveY[numMoves] = y - 1;
							numMoves++;
						}
					}

					// RESET THE VALUES
					x = i;
					y = j;

					if (y < 7 && board[x][y + 1] == 'w') // NEXT
					{
						while (y < 7 && board[x][y + 1] == 'w')
						{
							y++;
						}

						if (y < 7 && board[x][y + 1] == '*') 
						{
							moveX[numMoves] = x;
							moveY[numMoves] = y + 1;
							numMoves++;
						}
					}

					// RESET THE VALUES
					x = i;
					y = j;

					if (x >= 1 && board[x - 1][y] == 'w') // UPWARD
					{
						while (x >= 1 && board[x - 1][y] == 'w')
						{
							x--;
						}

						if (x >= 1 && board[x - 1][y] == '*') 
						{
							moveX[numMoves] = x - 1;
							moveY[numMoves] = y;
							numMoves++;
						}
					}

					// RESET
					x = i;
					y = j;

					if (x < 7 && board[x + 1][y] == 'w') // DOWNWARD
					{
						while (x < 7 && board[x + 1][y] == 'w')
						{
							x++;
						}

						if (x < 7 && board[x + 1][y] == '*')
						{

							moveX[numMoves] = x + 1;
							moveY[numMoves] = y;
							numMoves++;
						}
					}

					// RESET
					x = i;
					y = j;

					if (x >= 1 && y >= 1 && board[x - 1][y - 1] == 'w') // UP LEFT
					{
						while (x >= 1 && y >= 1 && board[x - 1][y - 1] == 'w')
						{
							x--;
							y--;
						}

						if (x >= 1 && y >= 1 && board[x - 1][y - 1] == '*')
						{

							moveX[numMoves] = x - 1;
							moveY[numMoves] = y - 1;
							numMoves++;
						}
					}

					// RESET
					x = i;
					y = j;

					if (x < 7 && y < 7 && board[x + 1][y + 1] == 'w') // DOWN RIGHT
					{
						while (x < 7 && y < 7 && board[x + 1][y + 1] == 'w')
						{
							x++;
							y++;
						}

						if (x < 7 && y < 7 && board[x + 1][y + 1] == '*')
						{

							moveX[numMoves] = x+1;
							moveY[numMoves] = y+1;
							numMoves++;
						}
					}

					// RESET THE VALUE
					x = i;
					y = j;

					if (x >= 1 && y < 7 && board[x - 1][y + 1] == 'w') // UP RIGHT
					{
						while (x >= 1 && y < 7 && board[x - 1][y + 1] == 'w')
						{
							x--;
							y++;
						}

						if (x >= 1 && y < 7 && board[x - 1][y + 1] == '*')
						{

							moveX[numMoves] = x-1;
							moveY[numMoves] = y+1;
							numMoves++;
						}
					}

					// RESET
					x = i;
					y = j;


					if (x <7 && y >= 1 && board[x + 1][y - 1] == 'w') // DOWN LEFT
					{
						while (x <7 && y >= 1 && board[x + 1][y - 1] == 'w')
						{
							x++;
							y--;
						}

						if (x<7 && y >= 1 && board[x + 1][y - 1] == '*')
						{
							moveX[numMoves] = x+1;
							moveY[numMoves] = y-1;
							numMoves++;
						}
					}
				}
			}
		}

	}
}
void makeMove(char board[][8], int x, int y, char key)
{
	// CHANGE ALL FOR W
	if (key == 'w')
	{
		board[x][y] = 'w'; // PLACE ON INDEX

		int x1 = x;
		int y1 = y;

		// UPWARD
		if (x1 >= 1 && board[x1 - 1][y1] == 'b') {
			while (x1 >= 1 && board[x1 - 1][y1] == 'b')
			{
				x1--;
			}

			if (x1 >= 1 && board[x1 - 1][y1] == 'w') // W-----W REPLACE
			{
				for (int i = x; i >= x1 - 1; i--)
					board[i][y1] = 'w';
			}
		}

		// RESET
		x1 = x;
		y1 = y;

		// DWONWARD
		if (x1 < 7 && board[x1 + 1][y1] == 'b') {
			while (x1 < 7 && board[x1 + 1][y1] == 'b')
			{
				x1++;
			}

			if (x1 < 7 && board[x1 + 1][y1] == 'w')
			{
				for (int i = x; i < x1 + 1; i++)
					board[i][y1] = 'w';
			}
		}

		// RESET
		x1 = x;
		y1 = y;

		// LEFT
		if (y1 >= 1 && board[x1][y1 - 1] == 'b') {
			while (y1 >= 1 && board[x1][y1 - 1] == 'b')
			{
				y1--;
			}

			if (y1 >= 1 && board[x1][y1 - 1] == 'w')
			{
				for (int i = y; i >= y1 - 1; i--)
					board[x1][i] = 'w';
			}
		}

		// RESET
		x1 = x;
		y1 = y;

		// RIGHT
		if (y1 < 7 && board[x1][y1 + 1] == 'b') {
			while (y1 < 7 && board[x1][y1 + 1] == 'b')
			{
				y1++;
			}

			if (y1 < 7 && board[x1][y1 + 1] == 'w') // if last is w then replace all
			{
				for (int i = y; i < y1 + 1; i++)
					board[x1][i] = 'w';
			}
		}

		// RESET
		x1 = x;
		y1 = y;

		// LEFT UP
		if (x1 >= 1 && y1 >= 1 && board[x1 - 1][y1 - 1] == 'b')
		{
			while (x1 >= 1 && y1 >= 1 && board[x1 - 1][y1 - 1] == 'b')
			{
				x1--;
				y1--;
			}

			if (x1 >= 1 && y1 >= 1 && board[x1 - 1][y1 - 1] == 'w')
			{
				while (x1 - 1 < x && y1 - 1 < y)
				{
					board[x1 - 1][y1 - 1] = 'w';
					x1++;
					y1++;
				}
			}

		}

		// RESET
		x1 = x;
		y1 = y;

		// RIGHT UP
		if (x1 >= 1 && y1 < 7 && board[x1 - 1][y1 + 1] == 'b')
		{
			while (x1 >= 1 && y1 < 7 && board[x1 - 1][y1 + 1] == 'b')
			{
				x1--;
				y1++;
			}

			if (x1 >= 1 && y1 < 7 && board[x1 - 1][y1 + 1] == 'w')
			{
				while (x1 - 1 < x && y1 + 1 > y)
				{
					board[x1 - 1][y1 + 1] = 'w';
					x1++;
					y1--;
				}
			}
		}

		// RESET
		x1 = x;
		y1 = y;

		// LEFT DOWN
		if (x1 < 7 && y1 >= 1 && board[x1 + 1][y1 - 1] == 'b')
		{
			while (x1 < 7 && y1 >= 1 && board[x1 + 1][y1 - 1] == 'b')
			{
				x1++;
				y1--;
			}

			if (x1 < 7 && y1 >= 1 && board[x1 + 1][y1 - 1] == 'w')
			{
				while (x1 + 1 > x && y1 - 1 < y)
				{
					board[x1][y1] = 'w';
					x1--;
					y1++;
				}
			}
		}

		// RESET
		x1 = x;
		y1 = y;

		// RIGHT DOWN
		if (y1 < 7 && x1 < 7 && board[x1 + 1][y1 + 1] == 'b')
		{
			while (y1 < 7 && x1 < 7 && board[x1 + 1][y1 + 1] == 'b')
			{
				y1++;
				x1++;
			}

			if (y1 < 7 && x1 < 7 && board[x1 + 1][y1 + 1] == 'w')
			{
				while (x1 + 1 > x && y1 + 1 > y)
				{
					board[x1 + 1][y1 + 1] = 'w';
					x1--;
					y1--;
				}
			}
		}




	}

	// CHANGE ALL FOR B
	if (key == 'b')
	{
		board[x][y] = 'b'; 

		int x1 = x;
		int y1 = y;

		// UPWRAD
		if (x1 >= 1 && board[x1 - 1][y1] == 'w') {
			while (x1 >= 1 && board[x1 - 1][y1] == 'w')
			{
				x1--;
			}

			if (x1 >= 1 && board[x1 - 1][y1] == 'b') // B----B
			{
				for (int i = x; i >= x1 - 1; i--)
					board[i][y1] = 'b';
			}
		}

		// RESET
		x1 = x;
		y1 = y;

		// DOWNWARD
		if (x1 < 7 && board[x1 + 1][y1] == 'w') {
			while (x1 < 7 && board[x1 + 1][y1] == 'w')
			{
				x1++;
			}

			if (x1 < 7 && board[x1 + 1][y1] == 'b') 
			{
				for (int i = x; i < x1 + 1; i++)
					board[i][y1] = 'b';
			}
		}

		// RESET
		x1 = x;
		y1 = y;

		// LEFT
		if (y1 >= 1 && board[x1][y1 - 1] == 'w') {
			while (y1 >= 1 && board[x1][y1 - 1] == 'w')
			{
				y1--;
			}

			if (y1 >= 1 && board[x1][y1 - 1] == 'b') 
			{
				for (int i = y; i >= y1; i--)
					board[x1][i] = 'b';
			}
		}

		// RESET
		x1 = x;
		y1 = y;

		// RIGHT
		if (y1 < 7 && board[x1][y1 + 1] == 'w') {
			while (y1 < 7 && board[x1][y1 + 1] == 'w')
			{
				y1++;
			}

			if (y1 < 7 && board[x1][y1 + 1] == 'b') // if last is w then replace all
			{
				for (int i = y; i < y1 + 1; i++)
					board[x1][i] = 'b';
			}
		}

		// RESET
		x1 = x;
		y1 = y;

		// LEFT UP
		if (x1 >= 1 && y1 >= 1 && board[x1 - 1][y1 - 1] == 'w')
		{
			while (x1 >= 1 && y1 >= 1 && board[x1 - 1][y1 - 1] == 'w')
			{
				x1--;
				y1--;
			}

			if (x1 >= 1 && y1 >= 1 && board[x1 - 1][y1 - 1] == 'b')
			{
				while (x1 - 1 < x && y1 - 1 < y)
				{
					board[x1 - 1][y1 - 1] = 'b';
					x1++;
					y1++;
				}
			}

		}

		// RESET
		x1 = x;
		y1 = y;

		// RIGHT UP
		if (x1 >= 1 && y1 < 7 && board[x1 - 1][y1 + 1] == 'w')
		{
			while (x1 >= 1 && y1 < 7 && board[x1 - 1][y1 + 1] == 'w')
			{
				x1--;
				y1++;
			}

			if (x1 >= 1 && y1 < 7 && board[x1 - 1][y1 + 1] == 'b')
			{
				while (x1 - 1 < x && y1 + 1 > y)
				{
					board[x1 - 1][y1 + 1] = 'b';
					x1++;
					y1--;
				}
			}
		}

		// RESET 
		x1 = x;
		y1 = y;

		// LEFT DOWN
		if (x1 < 7 && y1 >= 1 && board[x1 + 1][y1 - 1] == 'w')
		{
			while (x1 < 7 && y1 >= 1 && board[x1 + 1][y1 - 1] == 'w')
			{
				x1++;
				y1--;
			}

			if (x1 < 7 && y1 >= 1 && board[x1 + 1][y1 - 1] == 'b')
			{
				while (x1 + 1 > x && y1 - 1 < y)
				{
					board[x1][y1] = 'b';
					x1--;
					y1++;
				}
			}
		}

		// RESET
		x1 = x;
		y1 = y;

		// RIGHT DOWN
		if (y1 < 7 && x1 < 7 && board[x1 + 1][y1 + 1] == 'w')
		{
			while (y1 < 7 && x1 < 7 && board[x1 + 1][y1 + 1] == 'w')
			{
				y1++;
				x1++;
			}

			if (y1 < 7 && x1 < 7 && board[x1 + 1][y1 + 1] == 'b')
			{
				while (x1 + 1 > x && y1 + 1 > y)
				{
					board[x1 + 1][y1 + 1] = 'b';
					x1--;
					y1--;
				}
			}
		}

	}
}
bool gameOver(char board[][8])
{
	
	int score1 = score(board, 'b');
	int score2 = score(board, 'w');

	// ALL SLOTS ARE FULL
	if (score1 + score2 == 64)
		return true;

	// NO MOVE FOR BLACK OR WHITE
	if (score1 == 0 || score2 == 0)
		return true;

	int x[60];
	int y[60];

	int moveCountB = 0;
	getMoveList(board, x, y, moveCountB, 'b');
	int moveCountW = 0;
	getMoveList(board, x, y, moveCountW, 'w');

	// NO MOVE FOR BLACK AND WHITE
	if (moveCountB == 0 && moveCountW == 0)
		return true;

	return false;
}
int minimaxValue(char board[][8], char originalTurn, char currentTurn, int depth)
{
	if (depth == LEVEL || gameOver(board))
	{
		return hueristic(board, originalTurn); // RETURN THE HUERISTIC VALUE WHEN DEPTH IS REACHED OR IT IS IMPOSSIBLE TO EVALUATE FURTHER
	}

	int moveX[60], moveY[60];
	int numMoves;
	char opponent = 'b';
	if (currentTurn == 'b') // CHECK FOR WHOM WE CALL THE FUNCTION AND SET THE OTHER AS OPPONENT
		opponent = 'w';

	getMoveList(board, moveX, moveY, numMoves, currentTurn);

	if (numMoves == 0)
	{
		return minimaxValue(board, originalTurn, opponent, depth + 1); // CURRENT CAN NOT GENRATE STATES SO CALL OPPONENT
	}
	else
	{
		int bestMoveVal = -9999; // FOR MAX

		if (originalTurn != currentTurn) 
		{
			bestMoveVal = 9999; //  FOR MIN

			for (int i = 0; i < numMoves; i++)
			{
				char tempBoard[8][8];
				copyBoard(board, tempBoard);
				makeMove(tempBoard, moveX[i], moveY[i], currentTurn);

				int val = minimaxValue(tempBoard, originalTurn, opponent, depth + 1);

				if (originalTurn == currentTurn) // MAX LEVEL
				{
					if (val > bestMoveVal)
						bestMoveVal = val;
				}
				else // MIN LEVEL
				{
					if (val < bestMoveVal)
						bestMoveVal = val;
				}
			}
			return bestMoveVal;
		}
		return -1;
	}

}
void minimaxDecision(char board[][8], char whoseTurn, int& x, int& y)
{
	int moveX[60], moveY[60];
	int numMoves;
	char opponent = 'b';
	if (whoseTurn == 'b')
		opponent = 'w';

	getMoveList(board, moveX, moveY, numMoves, whoseTurn);
	if (numMoves == 0)
	{
		x = -1;
		y = -1;
	}

	else
	{
		int bestMoveVal = -9999;
		int bestX = moveX[0], bestY = moveY[0]; // ASSUME FIRST MOVE IS THE BEST MOVE

		for (int i = 0; i < numMoves; i++)
		{
			char tempBoard[8][8];
			copyBoard(board, tempBoard);
			makeMove(tempBoard, moveX[i], moveY[i], whoseTurn);

			int val = minimaxValue(tempBoard, whoseTurn, opponent, 1);

			if (val > bestMoveVal)
			{
				bestMoveVal = val;
				bestX = moveX[i];
				bestY = moveY[i];
			}
			
		}

		x = bestX;
		y = bestY;
	}
}

int main()
{

	Difficulty();
	setColor(4);
	cout << "***DIFFICULTY IS SET TO : " << LEVEL<<"***";
	Sleep(1000);
	char board[8][8];
	readMyboard(board, "board.txt");
	char player = 'w';
	print(board,8,8);

	while (!gameOver(board))
	{
		if (player == 'w')
		{
			int Xmove[60];
			int Ymove[60];
			int moveNum = 0;

			getMoveList(board, Xmove, Ymove, moveNum, 'w');

			if (moveNum != 0) {
				
				while (1) {
				setColor(6);

				cout << "SCORE : \n"; setColor(15);
				cout << "BLACK : " << score(board, 'b') << " WHITE : " << score(board, 'w')<<endl;
				setColor(6);
				cout << "POSSIBLE MOVES : \n"; setColor(14);
				for (int i = 0; i < moveNum; i++)
					cout <<"{ "<< Xmove[i] << " " << Ymove[i]<<" }" << endl;

				int x=-1;
				int y=-1;
				setColor(6);
				
					cout << "PLEASE ENTER YOUR MOVE : \n"; setColor(14);
					cout << "X:";
					cin >> x;
					cout << "Y:";
					cin >> y;

					if (validMove(Xmove, Ymove, moveNum, x, y)) {
						makeMove(board, x, y, 'w');
						break;
					}
					else
					{
						setColor(4);
						cout << "**PLEASE ENTER A VALID MOVE**\n";
						Sleep(500);
						print(board, 8, 8);

					}
				}
				print(board, 8, 8);
				Sleep(500);
			}
			player = 'b';
		}

		if (player == 'b')
		{
			int x;
			int y;

			minimaxDecision(board, 'b', x, y);
			makeMove(board, x, y, 'b');
			print(board, 8, 8);
			Sleep(500);

			player = 'w';
		}
	}

	int Black = score(board, 'b');
	int White = score(board, 'w');

	if (Black < White)
	{
		cout << "WHITE WINS....";
	}
	else if (Black > White)
	{
		cout << "BLACK WINS....";
	}
	else
		cout << "DRAW....";


	_getch();
}