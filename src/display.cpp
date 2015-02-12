#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "sdl.h"
#include "display.h"
#include "square.h"
#include "board.h"
#include "ltexture.h"

Square squares[64];
SDL_Rect spriteClips[12];
LTexture spriteSheetTexture;

void displayBoard(const Board& b, const int& mF, const int& mT) {
	//Clear screen
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	setPiecesOnSquares(b);
	drawSquares(b, mF, mT);
	drawPieces(b, mF, mT);
	drawBorder();

	//Update screen
	SDL_RenderPresent(renderer);
}

void setPiecesOnSquares(const Board& b) {
	for (int i = 0; i < 64; i++)
		squares[i].setPiece(b.getBoard120(from64(i+1)));
}

void setSquarePositions() {
	for (int i = 0; i < 64; i++) {
		squares[i].setPos(BXSTART+(SQ_SIZE*(i%8)),
				  BYSTART+B_SIZE-(SQ_SIZE*(i/8+1)));
		squares[i].setSq(i+1);
	}
}

void setSpriteClips() {
	for (int i = 0; i < 12; i++) {
		spriteClips[i].x = i%6*SQ_SIZE;
		spriteClips[i].y = i/6*SQ_SIZE;
		spriteClips[i].w = SQ_SIZE;
		spriteClips[i].h = SQ_SIZE;
	}
}

void drawSquares(const Board& b, const int& mF, const int& mT) {
	int sq;
	SDL_Rect sqPos;
	for (int r = 1; r <= 8; r++) {
		for (int f = 1; f <= 8; f++) {
			sq = FR2SQ64(f, r)-1;
			if ((r+f)%2 == 1) {	//Light squares
				if (mF != sq+1 && mT != sq+1 && to64(b.getMoveFrom()) != sq+1 && to64(b.getMoveTo()) != sq+1) {
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				}
				else {
					SDL_SetRenderDrawColor(renderer, 255, 255, 75, 255);
				}
			}
			else { 			//Dark squares
				if (mF != sq+1 && mT != sq+1 && to64(b.getMoveFrom()) != sq+1 && to64(b.getMoveTo()) != sq+1) {
					SDL_SetRenderDrawColor(renderer, 0, 128, 128, 255);
				}
				else {
					SDL_SetRenderDrawColor(renderer, 255, 255, 55, 255);
				}
			}
			sqPos = {squares[sq].getX(),	//X start
				 squares[sq].getY(),	//Y start
				 SQ_SIZE, SQ_SIZE};	 //Width, height of square
			SDL_RenderFillRect(renderer, &sqPos);
		}
	}
}

void drawPieces(const Board& b, const int& mF, const int& mT) {
	int p, sq, x, y;
	SDL_Rect sqPos;
	SDL_Rect clipSq;
	for (int r = 1; r <= 8; r++) {
		for (int f = 1; f <= 8; f++) {
			sq = FR2SQ64(f, r)-1;
			sqPos = {squares[sq].getX(), 	//X start
				 squares[sq].getY(), 	//Y start
				 SQ_SIZE, SQ_SIZE};	//Width, height of square
		
			p = squares[sq].getPiece();
			if (p == wqR || p == wkR)
				clipSq = spriteClips[wRook];		
		    	else if (p == wqN || p == wkN)
				clipSq = spriteClips[wKnight];		
		    	else if (p == wqB || p == wkB)
				clipSq = spriteClips[wBishop];		
		    	else if (p == wQ)
				clipSq = spriteClips[wQueen];		
		    	else if (p == wK)
				clipSq = spriteClips[wKing];		
		    	else if (p >= wPa && p <= wPh) {
				if (b.getValue(b.getBoard120(from64(sq+1))) == P_VAL)
					clipSq = spriteClips[wPawn];
				else
					clipSq = spriteClips[wQueen];
			}		
			if (p == bqR || p == bkR)
				clipSq = spriteClips[bRook];		
		    	else if (p == bqN || p == bkN)
				clipSq = spriteClips[bKnight];		
		    	else if (p == bqB || p == bkB)
				clipSq = spriteClips[bBishop];		
		    	else if (p == bQ)
				clipSq = spriteClips[bQueen];		
		    	else if (p == bK)
				clipSq = spriteClips[bKing];		
		    	else if (p >= bPa && p <= bPh) {
				if (b.getValue(b.getBoard120(from64(sq+1))) == P_VAL)
					clipSq = spriteClips[bPawn];
				else
					clipSq = spriteClips[bQueen];
			}		

			if (p != empty) { 
				if (squares[sq].getDragging()) {
					SDL_GetMouseState(&x, &y);
					spriteSheetTexture.render(x-SQ_SIZE/2, y-SQ_SIZE/2, &clipSq);	
				}
				else {
					spriteSheetTexture.render(sqPos.x, sqPos.y, &clipSq);
				}
			}
		}
	}
}

void drawBorder() {
	SDL_Rect borderRect = {BXSTART, BYSTART, B_SIZE, B_SIZE};
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &borderRect);
	borderRect = {BXSTART-1, BYSTART-1, B_SIZE+2, B_SIZE+2};
	SDL_RenderDrawRect(renderer, &borderRect);
}
