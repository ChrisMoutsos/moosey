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

bool getInput(Board& b, int& mF, int& mT) {
	using namespace std;
	
	string input;
	getline(cin, input);

	if (input == "o-o" || input == "O-O" || input == "0-0") {
		if (b.canCastle(KINGSIDE, b.getSide())) {
			mF = b.getSide() ? b.getPos(wK) : b.getPos(bK);
			mT = b.getSide() ? _G1 : _G8;
			return true;
		}
		else {
			cout << "Illegal move!\n";
			return false;
		}
	}
	if (input == "o-o-o" || input == "O-O-O" || input == "0-0-0") {
		if (b.canCastle(QUEENSIDE, b.getSide())) {
			mF = b.getSide() ? b.getPos(wK) : b.getPos(bK);
			mT = b.getSide() ? _B1 : _B8;
			return true;
		}
		else {
			cout << "Illegal move!\n";
			return false;
		}
	}

	if ((int)input.size() != 5 || input[2] != ' ') {
		cout << "Invalid input.\n";
		return false;
	}

	for (int i = 0; i <= 3; i+=3) //Makes sure input is of the form [a-h][1-9] [a-h][1-9] 
		if (!((int)input[i] < 105 && (int)input[i] > 96) || !((int)input[i+1] < 57 && (int)input[i+1] > 48)) {
			cout << "Invalid input.\n";
			return false;
		}

	mF = int(input[0])-96 + ((int(input[1])-47))*10;
	mT = int(input[3])-96 +  ((int(input[4])-47))*10;
	
	return true;
}

void userInput(Board& b, int& mF, int& mT) {	
	using namespace std;
	
	do {
		b.getSide() ? cout << "White" : cout << "Black";
		cout << " to move:\n\t";
	} while (!getInput(b, mF, mT) || !b.legalMove(mF, mT, b.getSide(), 1));
}
