/*
----------------------------------
	~Moosey Chess Engine~
	      display.cpp
----------------------------------
*/

#include <iostream>
#include <iomanip>
#include "display.h"
#include "board.h"

void displayBoard(const Board& board) {
	using namespace std;

	bool color = 0; //0 for white, 1 for black
	int square = _A8, counter = 0, oldprecision = cout.precision();
	char oldfill = cout.fill();
	ios_base::fmtflags oldflags = cout.flags();

	cout << "  ." << setfill('-') << setw(65) << ".\n";
	while (square >= _A1) {
		emptyRow(color, counter);
		cout << (int)(square/10)-1;
		printRow(board, color, square);
		emptyRow(color, counter);
		if (square!=_A1) cout << "  |" << setw(65) << "|\n";
		square -= 10;
	}
	cout << "  `" << setw(65) << "`\n";
	cout << setfill(' ') << setw(6) << "";
	for (int i = (int)'a'; i <= (int)'h'; i++)
		cout << setfill(' ') << left << setw(8) << (char)i;
	cout << '\n';
	
	cout.precision(oldprecision);
	cout.fill(oldfill);
	cout.flags(oldflags);
}

void printRow(const Board& b, bool& color, int startingSquare) {
	using namespace std;

	int x;
	char a;
	cout << " ";
	for (int i = 1; i < 9; i++) {
	  	x = startingSquare+i-1;
		a = b.getPieceAbbr(b.getBoard120(x));
		if (color) cout << "|**"; 
		else cout << "|  ";
		if (b.getBoard120(x) != empty) {
			if (b.getPieceMoved() == b.getBoard120(x)) 
				cout << "~" << a << "~";
			else 
				cout << " " << a << " ";
		}
		else {
			if (b.getMoveFrom() == x) 
				color ? cout << " / " : cout << " \\ ";
			else 
				color ? cout << "***" : cout << "   ";
		}
		color ? cout << "**" : cout << "  ";
		color = color ? 0 : 1;
	}
	cout << "|\n";
}

void emptyRow (bool& color, int& counter) {
	using namespace std;

	for (int i = 1; i < 9; i++) {
		if (i == 1) cout << "  ";
		color ? cout << "|*******" : cout << "|       ";
		color = color ? 0 : 1;
	}
	cout << "|\n";
	counter++;
	if (!(counter%2)) {
		color = (color) ? 0 : 1;
		counter = 0;
	}
}
