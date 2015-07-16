/*
 ---------------------------------
	~Moosey Chess Engine~
	   By Chris Moutsos
	       main.cpp
 ---------------------------------
*/

/* 
 * ##To-Do List##
 * Add en passants to quiescent search
 * Reimplement killer moves
 * Make sure FEN loading works perfectly
 * Transposition table
 * 
 */

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <chrono>
#include "common.h"
#include "board.h"

bool quit = false, muted = true, start = false;

using namespace std;

int main(int argc, char* args[]) {
	int mF = -1, mT = -1;
	SDL_Event e; //Event handler

	Board board;

	std::cout << "Current FEN (start): " << board.getFEN() << '\n';
	std::cout << "Current Zobrist (start): " << board.getZobrist() << '\n';
//	board.eval(true);

	/*
	typedef std::chrono::duration<float> fsec;
	fsec diff;
	int nodes;
	std::cout << "Perftesting....";
	for (int i = 1; i <= 10; i++) {
		auto beginTime = std::chrono::high_resolution_clock::now();
		nodes = board.perft(i);
		std::cout << "board.perft(" << i << "): " << nodes << "\n";
		auto endTime = std::chrono::high_resolution_clock::now();
		diff = endTime - beginTime;
		std::cout << "Took: " << diff.count() << " seconds" << '\n';
		std::cout << "Nodes/sec: " << nodes/diff.count() << '\n';
	}
	*/


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
						else start = true;
					break;
					case SDLK_m:
						muted = muted ? 0 : 1;
					break;
					case SDLK_w:
						board.setWhiteIsBot(!board.getWhiteIsBot());
					break;
					case SDLK_b:
						board.setBlackIsBot(!board.getBlackIsBot());
					break;
					case SDLK_UP:
						if (board.whiteBot.getLevel() < 9 &&
						    board.blackBot.getLevel() < 9) {
							board.whiteBot.setLevel(board.whiteBot.getLevel()+1);
							board.blackBot.setLevel(board.blackBot.getLevel()+1);
						}
					break;
					case SDLK_DOWN:
						if (board.whiteBot.getLevel() > 1 &&
						    board.blackBot.getLevel() > 1) {
							board.whiteBot.setLevel(board.whiteBot.getLevel()-1);
							board.blackBot.setLevel(board.blackBot.getLevel()-1);
						}
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

	return 0;
}
