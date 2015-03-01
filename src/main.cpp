/*
 ---------------------------------
	~Moosey Chess Engine~
	   By Chris Moutsos
	      main.cpp
 ---------------------------------
*/
/*   ##To-Do List##
 *	SDL
 *	- Add mute button
 *	- Splash screen
 *	FEN parsing
 * 
 * Known issues:
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

		//	showMoveLists(board);
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
	for (int i = wqR; i <= wPh; i++) {
		cout << board.piece[i].getName() << ": ";
		for (int j = 0; j < board.piece[i].getMoveListSize(); j++) {
			if (board.piece[i].getFromMoveList(j) != null) {
				mF = board.piece[i].getPos();
				mT = board.piece[i].getFromMoveList(j)%100;
				cout << intToSquare(mF) << " to " << intToSquare(mT) << ", ";
			}
		}
		cout << "...";
	}
	cout << endl;
}
