/*
----------------------------------
	~Moosey Chess Engine~
	      display.cpp
----------------------------------
*/

#include "display.h"
#include "board.h"
#include "move.h"
#include "pieces.h"

void displayBoard() {
	bool color = 0; //0 for white, 1 for black
	int square = 57, counter = 0;
	std::cout << "  .---------------------------------------------------------------.\n";
	while (square > 0) {
		emptyRow(color, counter);
		std::cout << (int)(square/8)+1;
		printRow(color, square);
		emptyRow(color, counter);
		if (square!=1) std::cout << "  |---------------------------------------------------------------|\n";
		square -= 8;
	}
	std::cout << "  `---------------------------------------------------------------`\n";
	std::cout << "      a       b       c       d       e       f       g       h \n\n";	
}

void printRow(bool& color, int startingSquare) {
	int x;
	char a;
	std::cout << " ";
	for (int i = 1; i < 9; i++) {
	  	x = startingSquare+i-1;
		a = piece[board64[x]].abbr;
		if (color) std::cout << "|**"; 
		else std::cout << "|  ";
		if (board64[x] != empty) {
			if (pieceMoved == board64[x]) std::cout << "~" << a << "~";
			else std::cout << " " << a << " ";
		}
		else {
			if (pieceMovedFrom == x ) color ? std::cout << " / " : std::cout << " \\ ";
			else color ? std::cout << "***" : std::cout << "   ";
		}
		color ? std::cout << "**" : std::cout << "  ";
		color = color ? 0 : 1;
	}
	std::cout << "|\n";
}

void emptyRow (bool& color, int& counter) {
	for (int i = 1; i < 9; i++) {
		if (i == 1) std::cout << "  ";
		color ? std::cout << "|*******" : std::cout << "|       ";
		color = color ? 0 : 1;
	}
	std::cout << "|\n";
	counter++;
	if (!(counter%2)) {
		color = (color) ? 0 : 1;
		counter = 0;
	}
}
