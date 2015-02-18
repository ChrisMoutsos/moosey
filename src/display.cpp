#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "sdl.h"
#include "display.h"
#include "board.h"
#include "ltexture.h"

SDL_Rect spriteClips[12];
LTexture spriteSheetTexture;
LTexture turnText;
LTexture checkText;
SDL_Color textColor;

void displayBoard(Board& b, const int& mF, const int& mT) {
	static bool sidey = !b.getSide();

	//Clear screen
	SDL_SetRenderDrawColor(renderer, 200, 200, 255, 255);
	SDL_RenderClear(renderer);

	setPiecesOnSquares(b);
	drawSquares(b, mF, mT);
	drawPieces(b, mF, mT);
	drawBorder();

	drawMoveTable();
	
	//Turn text
	if (sidey != b.getSide()) {
		sidey = b.getSide();
		if (b.getSide()) 
			turnText.loadFromRenderedText("White to move", textColor);
		else
			turnText.loadFromRenderedText("Black to move", textColor);
	}
	turnText.render(BXSTART, BYSTART+B_SIZE+15);
	//Check text
	b.checkCheck(b.getSide(), 1);
	checkText.render(BXSTART+B_SIZE-200, BYSTART+B_SIZE+15);

	//Update screen
	SDL_RenderPresent(renderer);
}

void setPiecesOnSquares(Board& b) {
	for (int i = 0; i < 64; i++)
		b.squares[i].setPiece(b.getBoard120(from64(i+1)));
}

void setSquarePositions(Board& b) {
	for (int i = 0; i < 64; i++) {
		b.squares[i].setPos(BXSTART+(SQ_SIZE*(i%8)),
				  BYSTART+B_SIZE-(SQ_SIZE*(i/8+1)));
		b.squares[i].setSq(i+1);
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
			sqPos = {b.squares[sq].getX(),	//X start
				 b.squares[sq].getY(),	//Y start
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
			sqPos = {b.squares[sq].getX(), 	//X start
				 b.squares[sq].getY(), 	//Y start
				 SQ_SIZE, SQ_SIZE};	//Width, height of square
		
			p = b.squares[sq].getPiece();
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
				if (b.squares[sq].getDragging()) {
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

void drawMoveTable() {
	SDL_Rect borderRect = {BXSTART+B_SIZE+75, BYSTART, 400, 600};
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &borderRect);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &borderRect);
	borderRect = {BXSTART+B_SIZE+74, BYSTART-1, 400, 600};
	SDL_RenderDrawRect(renderer, &borderRect);
}
