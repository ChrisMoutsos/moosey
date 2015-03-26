/*
 ---------------------------------
	~Moosey Chess Engine~
	   By Chris Moutsos
	       main.cpp
 ---------------------------------
*/

/* 
 * ##To-Do List##
 * SDL:
 * - Splash screen
 * Add en passants and promotions to quiescent search
 * Threefold repetition
 * - FEN parsing
 * Finish null move heuristic edge cases (?)
 * Write dupMove code for Queens
 * Handle stalemates
 * 
 */

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "common.h"
#include "sdl.h"
#include "board.h"
#include "display.h"

bool quit = false, muted = false;;

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
	std::cout << "Current FEN: " << board.getFEN() << '\n';

	while (!quit) {
		while (SDL_PollEvent(&e)) {
		/*	if (board.getSide()) {
				mF = mT = -1;
				displayBoard(board, mF, mT);
				if (!board.getSideInCheckmate())
					board.botMove();

			}
*/
			if (e.type == SDL_QUIT)
				quit = true;
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
					case SDLK_SPACE:
						mF = mT = -1;
						displayBoard(board, mF, mT);
						if (!board.getSideInCheckmate())
							board.botMove();
					break;
					case SDLK_m:
						muted = muted ? 0 : 1;
					break;
					case SDLK_LEFT:
						board.undoMove();
						displayBoard(board, mF, mT);
					break;
					case SDLK_BACKSPACE:
						board.restart();
						displayBoard(board, mF, mT);
					break;
				}
			}
			board.handleInput(mF, mT, &e);
			displayBoard(board, mF, mT);
		}
	}

	close_SDL();
	return 0;
}
