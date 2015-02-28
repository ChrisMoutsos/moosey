/*
 ---------------------------------
	~Moosey Chess Engine~
	   By Chris Moutsos
	      main.cpp
 ---------------------------------
*/
/*   ##To-Do List##
 *	SDL
 *	- Sounds
 *	- Splash screen
 *	FEN parsing
 * 
 * Known issues:
 * Internal:
 * Are pawn movelists correct after promoting, unpromoting?
 * Algebraic notation:
 * Write dupMove code for Queens
 */

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "sdl.h"
#include "board.h"
#include "display.h"

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

	int mF = -1, mT = -1;
	bool quit = false;
	SDL_Event e; //Event handler

	Board board;

	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) 
				quit = true;
			board.handleInput(mF, mT, &e);
			displayBoard(board, mF, mT);
		}
	}

	close_SDL();
	return 0;
}

void showMoveLists(Board &board) {
	int mF, mT;
	cout << "White movelist: " << endl;
	for (int i = 0; i < board.getMoveListSize(WHITE); i++) {
		mF = board.getFromMoveList(WHITE, i)/(100);
		mT = board.getFromMoveList(WHITE, i)%(100);
		cout << intToSquare(mF) << " to " << intToSquare(mT) << ", ";
	}
	cout << endl << endl;
	for (int i = 0; i <= wPh; i++) {
		cout << board.getName(i) << ": ";
		for (int j = 0; j < board.getPieceMoveListSize(i); j++) {
			if (board.getFromPieceMoveList(i, j) != null) {
				mF = board.getPos(i);
				mT = board.getFromPieceMoveList(i, j)%100;
				cout << intToSquare(mF) << " to " << intToSquare(mT) << ", ";
			}
		}
		cout << "...";
	}
	cout << endl;
}
