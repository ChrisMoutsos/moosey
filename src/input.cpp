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
	using namespace std;
	
	string input;
	getline(cin, input);

	if ((int)input.size() != 5 || input[2] != ' ') {
		cout << "Invalid input.\n";
		return false;
	}

	for (int i = 0; i <= 3; i+=3) //Makes sure input is of the form [a-h][1-9] [a-h][1-9] 
		if (!((int)input[i] < 105 && (int)input[i] > 96) || !((int)input[i+1] < 57 && (int)input[i+1] > 48)) {
			cout << "Invalid input.\n";
			return false;
		}

	moveFrom = int(input[0])-96 + ((int(input[1])-47))*10;
	moveTo = int(input[3])-96 +  ((int(input[4])-47))*10;
	
	return true;
}

void userInput(Board& board, int& mF, int& mT) {	
	using namespace std;
	
	do {
		board.checkCheck(board.getSide(), 1);
		board.getSide() ? cout << "White" : cout << "Black";
		cout << " to move:\n\t";
	} while (!getInput(mF, mT) || !board.legalMove(mF, mT, board.getSide(), 1));
}
