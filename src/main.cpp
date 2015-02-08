/*
 ---------------------------------
	~Moosey Chess Engine~
	   By Chris Moutsos
	      main.cpp
 ---------------------------------
*/
/*
 *	SDL
 *	FEN parsing
 *	Vectors for various member data of Board,
 *	in anticipation of the bot making multiple moves.
 *	Similarly, add vector of all moves made
 */

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "sdl.h"
#include "board.h"
#include "display.h"
#include "input.h"
#include "square.h"

void showMoveLists(Board& board);

using namespace std;

int main(int argc, char* args[]) {
	if (!init_SDL()) {
		cout << "Failure to initialize SDL.\n";
		return -1;
	}
	if (!loadMedia()) {
		cout << "Failure to load media.\n";
		return -1;
	}
	bool quit = false;
	int mF = 0, mT = 0;
	SDL_Event e; //Event handler

	Board board;
	setSquarePositions();
	setSpriteClips();	

	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) 
				quit = true;
		for (int i = 0; i < 64; i++) {
			squares[i].handleEvent(&e);
		}	

		}
		displayBoard(board);


	//	userInput(board, mF, mT, quit);
	//	if (quit) break; 
	//	board.setMove(mF, mT);
	//	board.movePiece();
	//	board.changeTurn();
	//	board.moveInfo();
	//	board.generateMoveLists();

//		if (board.checkCheck(board.getSide(), 1)) 
//			quit = true;

//		showMoveLists(board);
	}

	return 0;
}

void showMoveLists(Board &board) {
	int mF, mT;
	cout << "White movelist: " << endl;
	for (int i = 0; i < (int)board.whiteMoveList.size(); i++) {
		mF = board.whiteMoveList[i]/100;
		mT = board.whiteMoveList[i]%100;
		cout << intToSquare(mF) << " to " << intToSquare(mT) << ", ";
	}
	cout << endl << endl;
	for (int i = 0; i <= wPh; i++) {
		cout << board.getName(i) << ": ";
		for (int j = 0; j < (int)board.piece[i].moveListSize; j++) {
			if (board.piece[i].moveList[j] != null) {
				mF = board.getPos(i);
				mT = board.piece[i].moveList[j]%100;
				cout << intToSquare(mF) << " to " << intToSquare(mT) << ", ";
			}
		}
		cout << "...";
	}
	cout << endl;
}
