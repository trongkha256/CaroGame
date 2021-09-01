#include <iostream>
#include <fstream>
#include <Windows.h>
#include <stdlib.h>
#include <conio.h>
#include <string>

#define BS 12
#define TOP 1 
#define LEFT 10
#define SECOND_TOP 1
#define SECOND_LEFT 62
#define LIST "GAMESAVEFILESLIST.CARO"

using namespace std;

struct Player { string Name = ""; int Step = 0, Win = 0, Color = 240; };

struct CELL { int x, y, c; }; // define position x, y and status of cell. c = 0 is empty, -1 is P1, 1 is P2

void FixConsoleWindows();

void Color(int color);
void GotoXY(int x, int y);
void PrintXY(int x, int y, const std::string& message);
void Cursor(bool is_up, int POSX, int POSY);
void DrawFrame(int x, int y, int w, int h);

int MainScreen(int& POSX, int& POSY);
void Help();
void About();
void changeTurn(bool& TURN);
void ChooseTextColor(int& playerColor);
void changeColor(Player& PLAYER1, Player& PLAYER2);

void DrawInGameInfo(Player PLAYER1, Player PLAYER2, bool TURN);
void DrawBoard();

void WinEffect(const std::string& Name, int x, int y, int cnt, int dir, CELL BOARD[][BS]);
int AskContinue();

//Fix the size of the console
void FixConsoleWindows()
{
	HWND ConsoleWindow = GetConsoleWindow();
	LONG	style = GetWindowLong(ConsoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(ConsoleWindow, GWL_STYLE, style);
}

//Set color for text;
void Color(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

//Goto to the (x, y) position in console
void GotoXY(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void PrintXY(int x, int y, const std::string& message)
{
	GotoXY(x, y);
	std::cout << message;
}

void DrawFrame(int x, int y, int w, int h)
{
	GotoXY(x, y);
	for (int i = x; i <= x + w; ++i)
	{
		std::cout << "=";
		Sleep(1);
	}

	GotoXY(x, y + h + 1);
	for (int i = x; i <= x + w; ++i)
	{
		std::cout << "=";
		Sleep(1);
	}
	for (int j = y; j <= h + 1; ++j)
	{
		GotoXY(x, j);
		std::cout << "|";
		Sleep(1);
	}
	for (int j = y; j <= h + 1; ++j)
	{
		GotoXY(x + w, j);
		std::cout << "|";
		Sleep(1);
	}
}

//Draw the visual - interactive board
void DrawBoard()
{
	system("cls");
	Color(240);
	DrawFrame(LEFT - 2, TOP - 1, BS * 4 + 4, BS * 2 + 1);
	for (int i = 0; i < BS + 1; ++i)
	{
		for (int j = 0; j < BS + 1; ++j)
		{
			GotoXY(LEFT + 4 * i, TOP + 2 * j);
			std::cout << ".";
			Sleep(1);
		}
	}
}

//Draw a cursor
void Cursor(bool is_up, int POSX, int POSY)
{
	if (is_up)
	{
		PrintXY(POSX - 3, POSY + 1, "   ");
		PrintXY(POSX - 3, POSY, "-> ");
		GotoXY(POSX - 4, POSY);
	}
	else
	{
		PrintXY(POSX - 3, POSY - 1, "   ");
		PrintXY(POSX - 3, POSY, "-> ");
		GotoXY(POSX - 4, POSY);
	}
}

void changeTurn(bool& TURN)
{
	int POSX, POSY, COMMAND;
	bool validenter;
	const int top = 13;
	const int left = 45;
	system("cls");
	DrawFrame(left - 2, top - 1, 40, 4);
	PrintXY(left, top, "Player 1 (X) goes first");
	PrintXY(left, top + 1, "Player 2 (O) goes first");
	PrintXY(left, top + 3, "Press ESC to return");
	POSX = left;
	POSY = top;
	PrintXY(POSX - 4, POSY, "-> ");
	GotoXY(POSX - 4, POSY);
	while (1)
	{
		validenter = 0;
		COMMAND = _getch();
		switch (COMMAND)
		{
		case 13: case 32:
			if (POSY == top)
			{
				TURN = 1;
			}
			else TURN = 0;
			validenter = 1;
			break;
		case 87: case 119: case 72:
			if (POSY == top + 1)
			{
				POSY -= 1;
				Cursor(1, POSX - 1, POSY);
			}
			break;
		case 83: case 115: case 80:
			if (POSY == top)
			{
				POSY += 1;
				Cursor(0, POSX - 1, POSY);
			}
			break;
		case 27: case 8:
			return;
		default:
			break;
		}
		if (validenter == 1)
		{
			return;
		}
	}
}

void ChooseTextColor(int& playerColor)
{
	int COMMAND, POSX, POSY;
	const int top = 7;
	const int left = 45;

	system("cls");
	Color(240); PrintXY(left, top - 2, "Please choose your color:");
	PrintXY(left - 2, top - 1, "-----------------------------");
	PrintXY(left + 2, top + 0, "BLACK       : X O");
	Color(241); PrintXY(left + 2, top + 1, "DARK BLUE   : X O");
	Color(242); PrintXY(left + 2, top + 2, "DARK GREEN  : X O");
	Color(243); PrintXY(left + 2, top + 3, "DARK CYAN   : X O");
	Color(244); PrintXY(left + 2, top + 4, "DARK RED    : X O");
	Color(245); PrintXY(left + 2, top + 5, "DARK PINK   : X O");
	Color(246);	PrintXY(left + 2, top + 6, "DARK YELLOW : X O");
	Color(247);	PrintXY(left + 2, top + 7, "");
	Color(248);	PrintXY(left + 2, top + 8, "GREY        : X O");
	Color(249);	PrintXY(left + 2, top + 9, "LIGHT BLUE  : X O");
	Color(250);	PrintXY(left + 2, top + 10, "LIGHT GREEN : X O");
	Color(251);	PrintXY(left + 2, top + 11, "LIGHT CYAN  : X O");
	Color(252);	PrintXY(left + 2, top + 12, "LIGHT RED   : X O");
	Color(253);	PrintXY(left + 2, top + 13, "LIGHT PINK  : X O");
	Color(254);	PrintXY(left + 2, top + 14, "LIGHT YELLOW: X O");
	//DrawFrame(left - 2, top - 3, 40, 16);

	POSX = left;
	POSY = top;
	Color(240);
	PrintXY(POSX - 3, POSY, "-> ");
	GotoXY(POSX - 4, POSY);
	Color(240);

	while (1)
	{
		COMMAND = _getch();
		switch (COMMAND)
		{
		case 13: case 32:
			if (POSY != top + 7)
			{
				playerColor = 240 + POSY - top;
				return;
			}
			break;
		case 87: case 119: case 72:
			if (POSY > top)
			{
				POSY -= 1;
				Color(240 + POSY - top);
				Cursor(1, POSX - 1, POSY);
			}
			break;
		case 83: case 115: case 80:
			if (POSY < top + 14)
			{
				POSY += 1;
				Color(240 + POSY - top);
				Cursor(0, POSX - 1, POSY);
			}
			break;
		case 27: case 8:
			return;
		default:
			break;
		}
	}
}

void changeColor(Player& PLAYER1, Player& PLAYER2)
{
	int COMMAND, POSX, POSY;
	const int top = 14;
	const int left = 45;
	bool validEnter = 1;

	while (1)
	{
		if (validEnter)
		{
			Color(240);
			system("cls");
			DrawFrame(left - 2, top - 1, 30, 4);
			PrintXY(left, top, "Change X player's color");
			PrintXY(left, top + 1, "Change O player's color");
			PrintXY(left, top + 3, "Press ESC to return");
			POSX = left;
			POSY = top;
			PrintXY(POSX - 3, POSY, "-> ");
			GotoXY(POSX - 4, POSY);
			validEnter = 0;
		}
		COMMAND = _getch();
		switch (COMMAND)
		{
		case 13: case 32:
			validEnter = 1;
			if (POSY == top)
			{
				ChooseTextColor(PLAYER1.Color);
				break;
			}
			if (POSY == top + 1)
			{
				ChooseTextColor(PLAYER2.Color);
				break;
			}
			break;
		case 87: case 119: case 72:
			if (POSY > top)
			{
				POSY -= 1;
				Cursor(1, POSX - 1, POSY);
			}
			break;
		case 83: case 115: case 80:
			if (POSY < top + 1)
			{
				POSY += 1;
				Cursor(0, POSX - 1, POSY);
			}
			break;
		case 27: case 8:
			return;
		default:
			break;
		}
	}
}

void Setting(bool& TURN, Player& PLAYER1, Player& PLAYER2)
{
	int COMMAND, POSX, POSY;
	const int top = 14;
	const int left = 45;
	bool validEnter = 1;

	while (1)
	{
		if (validEnter)
		{
			Color(240);
			system("cls");
			DrawFrame(left - 2, top - 1, 30, 4);
			PrintXY(left, top, "Change first player's turn");
			PrintXY(left, top + 1, "Change players' color");
			PrintXY(left, top + 3, "Press ESC to return");
			POSX = left;
			POSY = top;
			PrintXY(POSX - 4, POSY, "-> ");
			GotoXY(POSX - 4, POSY);
			validEnter = 0;
		}
		COMMAND = _getch();
		switch (COMMAND)
		{
		case 13: case 32:
			validEnter = 1;
			if (POSY == top)
			{
				changeTurn(TURN);
			}
			else
			{
				changeColor(PLAYER1, PLAYER2);
			}
		case 87: case 119: case 72:
			if (POSY == top + 1)
			{
				POSY -= 1;
				Cursor(1, POSX - 1, POSY);
			}
			break;
		case 83: case 115: case 80:
			if (POSY == top)
			{
				POSY += 1;
				Cursor(0, POSX - 1, POSY);
			}
			break;
		case 27: case 8:
			return;
		default:
			break;
		}
	}
}

void DrawInGameInfo(Player PLAYER1, Player PLAYER2, bool TURN)
{
	Color(240);
	DrawFrame(SECOND_LEFT - 2, SECOND_TOP - 1, BS * 4 + 4, BS * 2 + 1);

	Color(240); PrintXY(65, 3, "Player 1: ");
	std::cout << PLAYER1.Name;
	Color(240); PrintXY(65, 4, "Victories: ");
	Color(PLAYER1.Color);  std::cout << PLAYER1.Win;
	Color(240); PrintXY(65, 5, "Steps: ");
	Color(PLAYER1.Color);  std::cout << PLAYER1.Step;

	Color(240); PrintXY(65, 8, "Player 2: ");
	std::cout << PLAYER2.Name;
	Color(240); PrintXY(65, 9, "Victories: ");
	Color(PLAYER2.Color);  std::cout << PLAYER2.Win;
	Color(240); PrintXY(65, 10, "Steps: ");
	Color(PLAYER2.Color);  std::cout << PLAYER2.Step;

	Color(240);
	PrintXY(65, 13, "Turn of: ");
	if (TURN == 1)
	{
		Color(PLAYER1.Color);
		std::cout << "X";
	}
	else
	{
		Color(PLAYER2.Color);
		std::cout << "O";
	}

	Color(240);
	PrintXY(65, 22, "Press ESC to return to Menu");
	PrintXY(65, 23, "Press L to save game");
	PrintXY(65, 24, "Press B to save game as another name");
}

//Draw the mainscreen
int MainScreen(int& POSX, int& POSY)
{
	bool validenter;
	const int top = 18;
	const int left = 55;

	Color(240);
	system("cls");

	PrintXY(left - 23, top - 11, "  .g8\"\"\"bgd        db       `7MM\"\"\"Mq.     .g8\"\"8q.  ");
	PrintXY(left - 23 - 9, top - 10, "_________.dP'     `M       \;MM:        MM   `MM.  .dP'    `YM.0101   ");
	PrintXY(left - 23 - 5, top - 9, "_____dM'       `      ,V^MM.       MM   ,M9   dM'      `MM0100101101010110    ");
	PrintXY(left - 23 - 20, top - 8, "____________________MM              ,M  `MM       MMmmdM9    MM        MM1");
	PrintXY(left - 23 - 3, top - 7, "___MM.             AbmmmqMA      MM  YM.    MM.      ,MP01010111 ");
	PrintXY(left - 23, top - 6, "`Mb.     ,'    A'     VML     MM   `Mb.  `Mb.    ,dP'   ");
	PrintXY(left - 23, top - 5, "  `\"bmmmd'   .AMA.   .AMMA. .JMML. .JMM.   `\"bmmd\"'         ");

	PrintXY(left, top, "New game");
	PrintXY(left, top + 1, "Load Game");
	PrintXY(left, top + 2, "Help");
	PrintXY(left, top + 3, "About");
	PrintXY(left, top + 4, "Setting");
	PrintXY(left, top + 5, "Exit Game");
	DrawFrame(left - 5, top - 1, 15, 6);
	POSX = left;	POSY = top;
	PrintXY(POSX - 3, POSY, "-> ");
	GotoXY(POSX - 4, POSY);
	while (1)
	{
		validenter = 0;
		int COMMAND = _getch();
		switch (COMMAND)
		{
		case 13: case 32:
			validenter = 1;
			break;
		case 87: case 119: case 72:
			if (POSY > top)
			{
				POSY -= 1;
				Cursor(1, POSX, POSY);
			}
			break;
		case 83: case 115: case 80:
			if (POSY < top + 5)
			{
				POSY += 1;
				Cursor(0, POSX, POSY);
			}
			break;
		case 27:
			exit(0);
		default:
			break;
		}

		if (validenter == 1) return POSY - top + 1;
	}
}

void WinEffect(const std::string& Name, int x, int y, int cnt, int dir, CELL BOARD[][BS])
{
	//GotoXY(112, 18); 
	//if (Name != "Draw match !!!") Name = Name + " has won !!!";
	string s;
	s.resize(Name.size());

	int X = 86 - Name.size() / 2;
	int Y = 18;

	int color = 240;
	int loop = 0;
	int Dirx[] = { 0, 1, 0, 1, 1 };
	int Diry[] = { 0, 0, 1, 1, -1 };

	while (loop < 25)
	{

		GotoXY(X, Y);
		Color(color);
		int u = x, v = y;

		if (color == 254) color = 240;
		cout << Name;

		for (int i = 0; i < cnt; ++i)
		{
			GotoXY(BOARD[u][v].x, BOARD[u][v].y);
			if (BOARD[u][v].c == -1) cout << "X";
			if (BOARD[u][v].c == 1)	cout << "O";
			u += Dirx[dir];
			v += Diry[dir];
		}

		color++;
		loop++;
		Sleep(100);

		GotoXY(X, Y);
		cout << s;

		u = x, v = y;
		for (int i = 0; i < cnt; ++i)
		{
			GotoXY(BOARD[u][v].x, BOARD[u][v].y);
			cout << " ";
			u += Dirx[dir];
			v += Diry[dir];
		}

		Sleep(100);
	}

	for (int i = 0; i < cnt; ++i)
	{
		GotoXY(BOARD[x][y].x, BOARD[x][y].y);
		Color(252);
		if (BOARD[x][y].c == -1) cout << "X";
		if (BOARD[x][y].c == 1)	cout << "O";
		x += Dirx[dir];
		y += Diry[dir];
	}
}

//Ask if players still want to play
int AskContinue()
{
	Color(240);
	PrintXY(SECOND_LEFT + 5, SECOND_TOP * (BS + 2) + 1, "Do you want to continue?");
	PrintXY(SECOND_LEFT + 5, SECOND_TOP * (BS + 2) + 2, "Press Y to continue, press N to exit");
	int c;
	do
	{
		c = _getch();
	} while (c != 78 && c != 110 && c != 89 && c != 121);

	return c;
}


//Model functions
int CheckPlus(CELL BOARD[][BS], int x1, int y1, int x2, int y2)
{
	// 2 - Da chan 2 dau
	// 1 - Da chan 1 dau
	// 0 - Chua chan
	if ((x1 < 0 || y1 < 0) && (BOARD[x2][y2].c != 0)) return 2;      // Neu nam trong vach va dau kia bi chan
	if ((x2 >= BS || y2 >= BS) && (BOARD[x1][y1].c != 0)) return 2;  // Neu nam trong vach va dau kia bi chan
	if (x1 < 0 || y1 < 0 || x2 >= BS || y2 >= BS) return 1;          // Neu nam trong vach va dau kia khong bi chan

	if (BOARD[x1][y1].c == 0 && BOARD[x2][y2].c == 0) return 0; // 2 dau trong
	if (BOARD[x1][y1].c == BOARD[x2][y2].c) return 2; // Da bi chan

	return 1; // cac truong hop on lai
}

bool CheckRow(CELL BOARD[][BS], int x, int y, int& cnt)
{
	if (x + 4 < BS)
	{
		if (BOARD[x][y].c != 0)
		{
			int c = BOARD[x][y].c;
			int dem = 1; // da co 1 o tren 5 o
			for (int i = 1; i < 5; ++i)    // Duyet 4 o tiep theo
			{
				if (BOARD[x + i][y].c == c) dem++;
				else
					return 0;
				if (dem == 4)						  // 4 o lien tuc	
					if (CheckPlus(BOARD, x - 1, y, x + i + 1, y) == 0) { cnt = 4; return 1; }// chua bi chan
				if (dem == 5)
					if (CheckPlus(BOARD, x - 1, y, x + i + 1, y) != 2) { cnt = 5;  return 1; }
			}
		}
	}
	return 0; // cac truong hop khac
}

//Check if the is a continious ray of 5 same elements in a coloumn
bool CheckColumn(CELL BOARD[][BS], int x, int y, int& cnt)
{
	if (y + 4 < BS)
	{
		if (BOARD[x][y].c != 0)
		{
			int c = BOARD[x][y].c;
			int dem = 1;
			for (int j = 1; j < 5; ++j)
			{
				if (BOARD[x][y + j].c == c) dem++;
				else return 0;
				if (dem == 4)
					if (CheckPlus(BOARD, x, y - 1, x, y + j + 1) == 0) { cnt = 4; return 1; }
				if (dem == 5)
					if (CheckPlus(BOARD, x, y - 1, x, y + j + 1) != 2) { cnt = 5; return 1; }
			}
		}
	}
	return 0;
}

//Check if the is a continious ray of 5 same elements in a diagonal line
bool CheckDiagonal1(CELL BOARD[][BS], int x, int y, int& cnt)
{
	if ((x + 4 < BS) && (y + 4 < BS))
	{
		if (BOARD[x][y].c != 0)
		{
			int c = BOARD[x][y].c;
			int dem = 1;
			for (int i = 1, j = 1; i < 5 && j < 5; ++i, ++j)
			{
				if (BOARD[x + i][y + j].c == c) dem++;
				else
					return 0;
				if (dem == 4)
					if (CheckPlus(BOARD, x - 1, y - 1, x + i + 1, y + j + 1) == 0) { cnt = 4; return 1; }
				if (dem == 5)
					if (CheckPlus(BOARD, x - 1, y - 1, x + i + 1, y + j + 1) != 2) { cnt = 5; return 1; }
			}
		}
	}
	return 0;
}

bool CheckDiagonal2(CELL BOARD[][BS], int x, int y, int& cnt)
{
	if (x + 4 < BS && y - 4 >= 0)
	{
		if (BOARD[x][y].c != 0)
		{
			int c = BOARD[x][y].c;
			int dem = 1;
			for (int i = 1, j = 1; i < 5 && j < 5; ++i, ++j)
			{
				if (BOARD[x + i][y - j].c == c) dem++;
				else
					return 0;
				if (dem == 4)
					if (CheckPlus(BOARD, x - 1, y + 1, x + i + 1, y - j - 1) == 0) { cnt = 4; return 1; }
				if (dem == 5)
					if (CheckPlus(BOARD, x - 1, y + 1, x + i + 1, y - j - 1) != 2) { cnt = 5; return 1; }
			}
		}
	}
	return 0;
}

//Check if there is a ray of 5 same elements in a row, column or diagonal line
bool CheckWin(CELL BOARD[][BS], int& x, int& y, int& cnt, int& dir)
{
	for (int i = 0; i < BS; ++i)
		for (int j = 0; j < BS; ++j)
		{
			dir = -1;
			if (CheckRow(BOARD, i, j, cnt)) dir = 1;
			if (CheckColumn(BOARD, i, j, cnt)) dir = 2;
			if (CheckDiagonal1(BOARD, i, j, cnt)) dir = 3;
			if (CheckDiagonal2(BOARD, i, j, cnt)) dir = 4;

			if (dir != -1)
			{
				x = i;
				y = j;
				return 1;
			}
		}
	return 0;
}

//Check if the matrix board is fullfllied
bool CheckFullBoard(CELL BOARD[][BS])
{
	for (int i = 0; i < BS; ++i)
		for (int j = 0; j < BS; ++j)
			if (BOARD[i][j].c == 0) return 0;
	return 1;
}

//Set all the the data to the intitial condition
void ResetData(CELL BOARD[][BS], int& LISTSIZE, string NAMELIST[], int& POSX, int& POSY)
{
	for (int i = 0; i < BS; ++i)
	{
		for (int j = 0; j < BS; ++j)
		{
			BOARD[i][j].x = 4 * i + LEFT + 2;
			BOARD[i][j].y = 2 * j + TOP + 1;
			BOARD[i][j].c = 0;
		}
	}

	//TURN = 1;		//int COMMAND = -1;
	POSX = BOARD[0][0].x;			POSY = BOARD[0][0].y;
	GotoXY(BOARD[0][0].x, BOARD[0][0].y);

	std::fstream infile;
	infile.open(LIST, std::ios::in);
	infile >> LISTSIZE;
	for (int i = 0; i < LISTSIZE; ++i)
		infile >> NAMELIST[i];
	infile.close();
}

//Check if there is a win player
int TestBoard(CELL BOARD[][BS], bool TURN, int& x, int& y, int& cnt, int& dir)
{
	if (CheckFullBoard(BOARD)) return 0;
	else
	{
		if (CheckWin(BOARD, x, y, cnt, dir))
		{
			if (TURN != 1) return -1;
			else return 1;
		}
		else return 2;
	}
}

// Check if the current cell is empty or not. If not, fill in
int CheckBoard(CELL BOARD[][BS], int pX, int pY, bool& TURN)
{
	for (int i = 0; i < BS; ++i)
	{
		for (int j = 0; j < BS; ++j)
			if (BOARD[i][j].x == pX && BOARD[i][j].y == pY && BOARD[i][j].c == 0)
			{
				if (TURN == 1) BOARD[i][j].c = -1;
				else BOARD[i][j].c = 1;
				TURN = !TURN;
				return BOARD[i][j].c;
			}
	}
	return 0;
}


//Control fucntions

//Draw visual board, set initial data
void StartGame(Player& PLAYER1, Player& PLAYER2, CELL BOARD[][BS], bool TURN, int& LISTSIZE, string NAMELIST[], int& POSX, int& POSY)
{
	system("cls");
	PLAYER1.Step = 0;
	PLAYER2.Step = 0;
	DrawBoard();
	DrawInGameInfo(PLAYER1, PLAYER2, TURN);
	ResetData(BOARD, LISTSIZE, NAMELIST, POSX, POSY);
}

//Clear screen for now
bool ExitGame()
{
	system("cls");
	const int left = 35;
	const int top = 10;
	DrawFrame(left, top, 30, 5);
	PrintXY(left, top + 2, "Do you really want to exit ?\n");
	PrintXY(left, top + 4, "Press Y (Yes) or N (No)");

	while (1)
	{
		int COMMAND = _getch();
		if (COMMAND == 89 || COMMAND == 121) return 1;
		if (COMMAND == 78 || COMMAND == 110) return 0;
	}
}

bool ExistingFile(string Name)
{
	const int left = 35;
	const int top = 10;
	GotoXY(left, top + 3);
	cout << "File " << Name << " has already existed. Do you want to replace it ?\n";
	GotoXY(left, top + 4);
	cout << "Press Y (Yes) or N (Enter other name)\n";
	while (1)
	{
		int COMMAND = _getch();
		if (COMMAND == 89 || COMMAND == 121) return 1;
		if (COMMAND == 78 || COMMAND == 110) return 0;
	}
}

std::string FileName(int& LISTSIZE, string NAMELIST[])
{
	const int left = 35;
	const int top = 10;

	std::string Name;
	while (1)
	{
		system("cls");
		DrawFrame(left - 2, top - 1, 55, 5);

		PrintXY(left, top + 1, "What is the file's name ? (No special characters):\n");
		// Kiem tra xem ten hop le hay khong 

		GotoXY(left, top + 2);
		getline(std::cin, Name); //Doc ten file
		bool check = 1;
		for (auto i : Name)
			if (!isalnum(i)) check = 0; // TRUE neu chi co chu cai va so

		if (!check || Name.size() == 0 || Name.size() > 10)
		{
			PrintXY(left, top + 3, "Invalid name!!! Try again\n");
			GotoXY(left, top + 4);
			system("pause");
		}
		else
		{
			bool fl = 0;
			int indx = -1;
			for (int i = 0; i < LISTSIZE; ++i)
				if (Name == NAMELIST[i])
				{
					fl = ExistingFile(Name);
					indx = i;
					break;
				}

			if (fl == 1) break;
			else
				if (indx == -1)
				{
					NAMELIST[LISTSIZE++] = Name;
					break;
				}
		}
	}

	Name = Name + ".caro";
	return Name;
}

void SaveGame(Player PLAYER1, Player PLAYER2, bool TURN, CELL BOARD[][BS], int& LISTSIZE, string NAMELIST[], string& Name, bool type)
{
	const int left = 35;
	const int top = 10;
	//std::string Name;
	std::ofstream outfile;

	if (Name.size() == 0)
	{
		system("cls");
		if (type == 1)
		{
			type = 0;
			DrawFrame(left - 2, top - 1, 47, 5);

			PrintXY(left, top + 1, "Do you want to save the match?\n");
			PrintXY(left, top + 3, "Press S to save, Press ESC to exit.");
			char tempcommand = _getch();
			system("cls");
			if (tempcommand != 's' && tempcommand != 'S') return;
		}
		Name = FileName(LISTSIZE, NAMELIST);

		outfile.open(LIST, std::ios::out);
		outfile.clear();

		outfile << LISTSIZE << "\n";
		for (int i = 0; i < LISTSIZE; ++i)
			outfile << NAMELIST[i] << "\n";
		outfile.close();
	}
	//else Name = NAMELIST[LISTSIZE - 1];


	outfile.open(Name, std::ios::out); //Mo 1 file ten Name.caro

									   //In ban co
	for (int i = 0; i < BS; ++i)
	{
		for (int j = 0; j < BS; ++j)
			outfile << BOARD[i][j].c << " ";
		outfile << "\n";
	}
	// In ten ng choi
	outfile << PLAYER1.Name << "\n";
	outfile << PLAYER2.Name << "\n";

	//In so buoc di chuyen
	outfile << PLAYER1.Step << " " << PLAYER2.Step << "\n";
	// In so ban thang
	outfile << PLAYER1.Win << " " << PLAYER2.Win << "\n";
	outfile << PLAYER1.Color << " " << PLAYER2.Color << "\n";

	outfile << TURN;

	outfile.close(); //Dong file dang mo
}

std::string NameChosen(int LISTSIZE, string NAMELIST[], int& POSX, int& POSY)
{
	system("cls");

	const int left = 30;
	const int top = 5;

	PrintXY(left, top, "These are recent saved files: ");

	for (int i = 0; i < LISTSIZE; ++i)
	{
		GotoXY(left + 4, top + i + 1);
		std::cout << i + 1 << ". " << NAMELIST[i] << std::endl;
	}

	POSX = left + 4;
	POSY = top + 1;
	PrintXY(left + 1, top + 1, "-> ");
	GotoXY(left, top + 1);
	while (1)
	{
		bool validenter = 0;
		int COMMAND = _getch();
		switch (COMMAND)
		{
		case 13: case 32:
			validenter = 1;
			break;
		case 87: case 119: case 72:
			if (POSY > top + 1)
			{
				POSY -= 1;
				Cursor(1, POSX, POSY);
			}
			break;
		case 83: case 115: case 80:
			if (POSY < top + LISTSIZE)
			{
				POSY += 1;
				Cursor(0, POSX, POSY);
			}
			break;
		default:
			break;
		}
		if (validenter == 1)
			return NAMELIST[POSY - top - 1] + ".CARO";
	}
}

bool LoadGame(Player& PLAYER1, Player& PLAYER2, CELL BOARD[][BS], bool& TURN, int& LISTSIZE, string NAMELIST[], int& POSX, int& POSY, string& Name)
{
	if (Name.size() == 0)
	{
		ResetData(BOARD, LISTSIZE, NAMELIST, POSX, POSY); //also read LISTSIZE from gamesavefilelist
		if (LISTSIZE <= 0)
		{
			system("cls");
			PrintXY(35, 5, "There are no recent saved files");
			PrintXY(35, 7, "Press ESC to return.");
			GotoXY(0, 0);

			while (1)
			{
				int COMMAND = _getch();
				if (COMMAND == 27 || COMMAND == 8) return 1;
			}
		}
		else
			//string Name;
			Name = NameChosen(LISTSIZE, NAMELIST, POSX, POSY);
	}


	ifstream infile;
	infile.open(Name, std::ios::in); //Mo file ten Name.caro

									 //Doc du lieu cua ban co
	for (int i = 0; i < BS; ++i)
		for (int j = 0; j < BS; ++j) infile >> BOARD[i][j].c;

	//Doc ten nguoi choi
	getline(infile, PLAYER1.Name);
	getline(infile, PLAYER1.Name);
	getline(infile, PLAYER2.Name);

	infile >> PLAYER1.Step >> PLAYER2.Step; //Doc so buoc di chuyen
	infile >> PLAYER1.Win >> PLAYER2.Win;   //Doc so ban thang
	infile >> PLAYER1.Color >> PLAYER2.Color;
	infile >> TURN;

	infile.close(); //Dong file dang mo

	system("cls");
	DrawBoard();
	for (int i = 0; i < BS; ++i)
		for (int j = 0; j < BS; ++j)
		{
			Sleep(2);
			int x = BOARD[i][j].x, y = BOARD[i][j].y;
			GotoXY(x, y);
			if (BOARD[i][j].c == -1)
			{
				Color(PLAYER1.Color);
				std::cout << "X";
			}
			if (BOARD[i][j].c == 1)
			{
				Color(PLAYER2.Color);
				std::cout << "O";
			}
		}
	POSX = BOARD[0][0].x;
	POSY = BOARD[0][0].y;
	DrawInGameInfo(PLAYER1, PLAYER2, TURN);

	return 0;
}

void NameInput(Player& PLAYER1, Player& PLAYER2, bool TURN)
{
	system("cls");
	const int top = 10;
	const int left = 35;

	DrawFrame(left - 2, top - 1, 47, 5);

	while (1)
	{
		PrintXY(left, top + 1, "Enter player 1's name: ");

		getline(std::cin, PLAYER1.Name);
		int sz = PLAYER1.Name.size();
		if (sz <= 20) break;
		else
		{
			PLAYER1.Name.clear();
			PLAYER1.Name.resize(sz);
			PrintXY(left + 23, top + 1, PLAYER1.Name);
		}
	}

	while (1)
	{
		PrintXY(left, top + 3, "Enter player 2's name: ");

		getline(std::cin, PLAYER2.Name);
		int sz = PLAYER2.Name.size();
		if (sz <= 20) break;
		else
		{
			PLAYER2.Name.clear();
			PLAYER2.Name.resize(sz);
			PrintXY(left + 23, top + 3, PLAYER2.Name);
		}
	}

	system("cls");
}

void Help()
{
	const int top = 11;
	const int left = 12;
	system("cls");
	PrintXY(left, top, "Players take turn to fill in the cells they want.");
	PrintXY(left, top + 1, "Players can't fill in the cell that've been filled before.");
	PrintXY(left, top + 2, "Player who has 5 continious elements of his/ her in a row, a column a diagonal line is the winner.");
	PrintXY(left, top + 3, "You can change the players' color in the Setting.");
	PrintXY(left, top + 5, "Press ECS to return.");

	DrawFrame(left - 2, top - 1, 100, 5);
	GotoXY(0, 0);

	while (1)
	{
		int COMMAND = _getch();
		if (COMMAND == 27 || COMMAND == 8) return;
	}
}

void About()
{
	const int top = 10;
	const int left = 25;
	system("cls");
	PrintXY(left, top, "This game is developed by THETEAM team of HCMUS using Visual Studio Community.");
	PrintXY(left, top + 1, "Nguyen Thanh An\n");
	PrintXY(left, top + 2, "Tran Nhat Hao\n");
	PrintXY(left, top + 3, "Nguyen Trong Kha\n");
	PrintXY(left, top + 4, "Huynh Ngoc Bao\n");
	PrintXY(left, top + 5, "Truong Quoc Bao\n");
	PrintXY(left, top + 7, "Press ESC to return.");
	DrawFrame(left - 2, top - 1, 81, 8);
	GotoXY(0, 0);
	while (1)
	{
		int COMMAND = _getch();
		if (COMMAND == 27 || COMMAND == 8) return;
	}
}

//Move the visual and matrix pointer to next-left cell
void MoveLeft(CELL BOARD[][BS], int& POSX, int& POSY)
{
	if (POSX > BOARD[0][0].x)
	{
		POSX -= 4;
		GotoXY(POSX, POSY);
	}
}

//Move the visual and matrix pointer to next-right cell
void MoveRight(CELL BOARD[][BS], int& POSX, int& POSY)
{
	if (POSX < BOARD[BS - 1][BS - 1].x)
	{
		POSX += 4;
		GotoXY(POSX, POSY);
	}
}

//Move the visual and matrix pointer to next-up cell
void MoveUp(CELL BOARD[][BS], int& POSX, int& POSY)
{
	if (POSY > BOARD[0][0].y)
	{
		POSY -= 2;
		GotoXY(POSX, POSY);
	}
}

//Move the visual and matrix pointer to next-down cell
void MoveDown(CELL BOARD[][BS], int& POSX, int& POSY)
{
	if (POSY < BOARD[BS - 1][BS - 1].y)
	{
		POSY += 2;
		GotoXY(POSX, POSY);
	}
}

bool MenuProgress(Player& PLAYER1, Player& PLAYER2, CELL BOARD[][BS], bool& TURN, int& LISTSIZE, string NAMELIST[], int& POSX, int& POSY, string& Name)
{
	Name.clear();
	bool inprogress = 1;
	while (inprogress)
	{
		switch (MainScreen(POSX, POSY))
		{
		case 1:
			inprogress = 0;
			PLAYER1.Win = 0;
			PLAYER2.Win = 0;
			NameInput(PLAYER1, PLAYER2, TURN);
			StartGame(PLAYER1, PLAYER2, BOARD, TURN, LISTSIZE, NAMELIST, POSX, POSY);
			break;
		case 2:
			inprogress = LoadGame(PLAYER1, PLAYER2, BOARD, TURN, LISTSIZE, NAMELIST, POSX, POSY, Name);
			break;
		case 3:
			Help();
			break;
		case 4:
			About();
			break;
		case 5:
			Setting(TURN, PLAYER1, PLAYER2);
			break;
		case 6:
			bool fl = ExitGame();
			if (fl == 1) return 0;
		}
	}
	return 1;
}

int main()
{
	FixConsoleWindows();
	SetConsoleTitle(TEXT("Caro"));
	Player PLAYER1, PLAYER2;
	CELL BOARD[BS][BS];
	bool TURN = 1;
	int LISTSIZE = 0;
	std::string NAMELIST[20];
	int POSX = 0, POSY = 0;
	string Name;

	if (MenuProgress(PLAYER1, PLAYER2, BOARD, TURN, LISTSIZE, NAMELIST, POSX, POSY, Name) == 0) return 0;

	bool validenter = 1;
	while (1)
	{
		int COMMAND = _getch();
		switch (COMMAND)
		{
		case 13: case 32:
			switch (CheckBoard(BOARD, POSX, POSY, TURN))
			{
			case -1:
				Color(PLAYER1.Color);
				std::cout << "X";

				Color(240); PrintXY(65, 5, "Steps: ");
				Color(PLAYER1.Color);  std::cout << ++PLAYER1.Step;

				Color(240);
				PrintXY(65, 13, "Turn of: ");
				Color(PLAYER2.Color);
				std::cout << "O";
				GotoXY(POSX, POSY);
				break;
			case 1:
				Color(PLAYER2.Color);
				std::cout << "O";

				Color(240); PrintXY(65, 10, "Steps: ");
				Color(PLAYER2.Color);  std::cout << ++PLAYER2.Step;

				Color(240);
				PrintXY(65, 13, "Turn of: ");
				Color(PLAYER1.Color);
				std::cout << "X ";
				GotoXY(POSX, POSY);
				break;
			default:
				validenter = 0;
				break;
			}
			break;
		case 65: case 97: case 75:
			MoveLeft(BOARD, POSX, POSY);
			break;
		case 68: case 100: case 77:
			MoveRight(BOARD, POSX, POSY);
			break;
		case 87: case 119: case 72:
			MoveUp(BOARD, POSX, POSY);
			break;
		case 83: case 115: case 80:
			MoveDown(BOARD, POSX, POSY);
			break;
		case 27: case 8:
			Color(240);
			SaveGame(PLAYER1, PLAYER2, TURN, BOARD, LISTSIZE, NAMELIST, Name, 1);
			if (MenuProgress(PLAYER1, PLAYER2, BOARD, TURN, LISTSIZE, NAMELIST, POSX, POSY, Name) == 0) return 0;
			break;
		case 66: case 98:
			Name.clear();
		case 76: case 108:
			Color(240);
			SaveGame(PLAYER1, PLAYER2, TURN, BOARD, LISTSIZE, NAMELIST, Name, 0);
			if (Name.size() > 0)
				LoadGame(PLAYER1, PLAYER2, BOARD, TURN, LISTSIZE, NAMELIST, POSX, POSY, Name);
			break;
		default:
			validenter = 0;
			break;
		}
		if (validenter == 1)
		{
			int x, y, cnt, dir;
			int winplayer = TestBoard(BOARD, TURN, x, y, cnt, dir);
			string Name;
			if (winplayer == 2) continue;
			if (winplayer == -1)
			{
				Name = PLAYER1.Name + " has won !!!";
				Color(PLAYER1.Color);  PrintXY(65, 4, "Victories: ");
				std::cout << ++PLAYER1.Win;
			}
			else if (winplayer == 1)
			{
				Name = PLAYER2.Name + " has won !!!";
				Color(PLAYER2.Color);  PrintXY(65, 9, "Victories: ");
				std::cout << ++PLAYER2.Win;
			}
			else if (winplayer == 0) Name = "Draw match !!!";
			WinEffect(Name, x, y, cnt, dir, BOARD);

			GotoXY(POSX, POSY);
			switch (winplayer)
			{
			case -1: case 0: case 1:
				int ask = AskContinue();
				if ((ask != 89) && (ask != 121))
				{
					if (MenuProgress(PLAYER1, PLAYER2, BOARD, TURN, LISTSIZE, NAMELIST, POSX, POSY, Name) == 0) return 0;
				}
				else StartGame(PLAYER1, PLAYER2, BOARD, TURN, LISTSIZE, NAMELIST, POSX, POSY);
			}
		}
		validenter = 1;
	}
}