/*
----------------------------------
	~Moosey Chess Engine~
	      input.cpp
----------------------------------
*/

#include <iostream>
#include <string>
#include "board.h"
#include "input.h"

bool getInput(int& moveFrom, int& moveTo) {
	std::string input;
	getline(std::cin, input);

	if ((int)input.size() != 5 || input[2] != ' ') {
		std::cout << "Invalid input.\n";
		return false;
	}

	for (int i = 0; i <= 3; i+=3) //Makes sure input is of the form [a-h][1-9] [a-h][1-9] 
		if (!((int)input[i] < 105 && (int)input[i] > 96) || !((int)input[i+1] < 57 && (int)input[i+1] > 48)) {
			std::cout << "Invalid input.\n";
			return false;
		}

	moveFrom = FR2SQ64((int)input[0]-96, (int)input[1]-48);
	moveTo = FR2SQ64((int)input[3]-96, (int)input[4]-48);
	
	return true;
}

void userInput(Board& board, int& mF, int& mT) {	
	do {
		board.getSide() ? std::cout << "White" : std::cout << "Black";
		std::cout << " to move:\n\t";
	} while (!getInput(mF, mT) || !board.legalMove(mF, mT, 1));
	if (board.inCheck()) {
		board.getSide() ? std::cout << "White" : std::cout << "Black";
		std::cout << " is in check!";
	}
}
