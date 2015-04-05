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
 * Make sure threefold repetition detection works
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

//	Board board("r1bqkbnr/ppp2ppp/2np4/4p3/2B1P3/2N2N2/PPPP1PPP/R1BQK2R b KQkq - 1 4");
//	Board board("8/8/p1p5/1p5p/1P5p/8/PPP2K1p/4R1rk w - - 0 1");
	Board board;
//	Board board("r2b1rk1/pbpq1ppp/2pp1n2/5N2/4P3/2N2Q2/PPP2PPP/R1B1R1K1 w - - 5 13");
//	Board board("6k1/ppp2ppp/8/r7/8/3KN3/8/8 b - - 0 2");
//	Board board("8/p3R2k/6pp/2p3r1/3pQ3/3P1P2/PPPq2PP/4R1K1 b - - 1 35");
	std::cout << "Current FEN: " << board.getFEN() << '\n';
	std::cout << "Current Zobrist: " << board.getZobrist() << '\n';

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
