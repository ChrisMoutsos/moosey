/*
 ---------------------------------
	~Moosey Chess Engine~
	   By Chris Moutsos
	       main.cpp
 ---------------------------------
*/

/* 
 * ##To-Do List##
 * Add en passants to quiescent search maybe
 * Make sure killer moves are being implemented correctly
 * Write dupMove code for Queens
 * Make sure FEN loading works perfectly
 * 
 */

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "common.h"
#include "sdl.h"
#include "board.h"

bool quit = false, muted = false, start = false;

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
	SDL_Event e; //Event handler

	Board board;

	std::cout << "Current FEN (start): " << board.getFEN() << '\n';
	std::cout << "Current Zobrist (start): " << board.getZobrist() << '\n';

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
					case SDLK_SPACE:
						if (start) {
							mF = mT = -1;
							board.updateDisplay(mF, mT);
							board.botMove();
						}
					break;
					case SDLK_m:
						muted = muted ? 0 : 1;
					break;
					case SDLK_LEFT:
						board.undoMove();
						board.updateDisplay(mF, mT);
					break;
					case SDLK_BACKSPACE:
						board.restart();
						board.updateDisplay(mF, mT);
					break;
				}
			}
			board.handleInput(mF, mT, &e);
			board.updateDisplay(mF, mT);
		}
		if (start) {
			if ((board.getSide() && board.getWhiteIsBot()) || 
			    (!board.getSide() && board.getBlackIsBot())) {
				board.botMove();
				board.updateDisplay(mF, mT);
			}
		}
		else
			mF = mT = -1;
	}

	close_SDL();
	return 0;
}
