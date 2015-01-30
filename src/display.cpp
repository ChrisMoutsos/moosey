/*
----------------------------------
	~Moosey Chess Engine~
	      display.cpp
----------------------------------
*/

#include <iostream>
#include <iomanip>
#include "board.h"
#include "display.h"

void displayBoard(Board board) {
	bool color = 0; //0 for white, 1 for black
	int square = 57, counter = 0, oldprecision = std::cout.precision();
	char oldfill = std::cout.fill();
	std::ios_base::fmtflags oldflags = std::cout.flags();

	std::cout << "  ." <<std::setfill('-') << std::setw(65) << ".\n";
	while (square > 0) {
		emptyRow(color, counter);
		std::cout << (int)(square/8)+1;
		printRow(board, color, square);
		emptyRow(color, counter);
		if (square!=1) std::cout << "  |" << std::setw(65) << "|\n";
		square -= 8;
	}
	std::cout << "  `" << std::setw(65) << "`\n";
	std::cout << std::setfill(' ') << std::setw(6) << "";
	for (int i = (int)'a'; i <= (int)'h'; i++)
		std::cout << std::setfill(' ') << std::left << std::setw(8) << (char)i;
	std::cout << '\n';
	
	std::cout.precision(oldprecision);
	std::cout.fill(oldfill);
	std::cout.flags(oldflags);
}

void printRow(Board b, bool& color, int startingSquare) {
	int x;
	char a;
	std::cout << " ";
	for (int i = 1; i < 9; i++) {
	  	x = startingSquare+i-1;
		a = b.piece[b.board64[x]].abbr;
		if (color) std::cout << "|**"; 
		else std::cout << "|  ";
		if (b.board64[x] != empty) {
			if (b.pieceMoved == b.board64[x]) std::cout << "~" << a << "~";
			else std::cout << " " << a << " ";
		}
		else {
			if (b.pieceMovedFrom == x ) color ? std::cout << " / " : std::cout << " \\ ";
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
