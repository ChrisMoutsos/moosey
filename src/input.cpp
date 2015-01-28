#include <iostream>
#include <string>
#include "macros.h"
#include "board.h"
#include "input.h"
#include "pieces.h"
#include "legal.h"

using namespace std;

int moveFrom = 0, moveTo = 0; //SQUARE NUMBER ON BOARD64

void userInput() {
	while (!getInput() || !legalMove()) {
		displayBoard();
		cout << "Illegal!" << endl;
		if (inCheck()) 
			side ? cout << "White is in check!\n" : "Black is in check!\n";
	}
}

bool getInput() {
	string input;

	side ? cout << "White to move: \n\t" : cout << "Black to move: \n\t";
	getline(cin, input);

	if ((int)input.size() != 5 || input[2] != ' ') return false;

	for (int i = 0; i <= 3; i+=3) //Makes sure input is of the form [a-h][1-9] [a-h][1-9] 
		if (!((int)input[i] < 105 && (int)input[i] > 96) || !((int)input[i+1] < 57 && (int)input[i+1] > 48)) return false;
	

	moveFrom = FR2SQ64((int)input[0]-96, (int)input[1]-48);
	moveTo = FR2SQ64((int)input[3]-96, (int)input[4]-48);

	if (side) { //Ensures white picks white piece to move onto black or empty square
		if (board64[moveFrom] >= bqR || board64[moveFrom] == -1 || (board64[moveTo] <= wPh && board64[moveTo] >= wqR)) return false;
	}
	else //Ensures black picks black piece to move onto white or empty square
		if (board64[moveFrom] <= wPh || board64[moveTo] >= bqR) return false;

	return true;
}
