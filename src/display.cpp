#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include "sdl.h"
#include "display.h"
#include "board.h"
#include "ltexture.h"

SDL_Rect spriteClips[12];
LTexture spriteSheetTexture;
LTexture turnText, checkText, moveText, rankText, fileText;
SDL_Color textColor;

void displayBoard(Board& b, const int& mF, const int& mT) {
	using std::string;

	static bool sidey = !b.getSide();
	static string moveStr = "", rankStr;
	int mF2, mT2;
	string temp = "";

	//Clear screen
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	setPiecesOnSquares(b);
	drawSquares(b, mF, mT);
	drawPieces(b, mF, mT);
	drawBorder();

	drawMoveTable();
	for (int m = 0; m <= b.getPly()/2; m++) {
		moveStr = std::to_string(m+1) + ". ";
		if (b.getPly() > m*2) {
			mF2 = b.getMoveMade(m*2)/100;
			moveStr += intToSquare(mF2) + " to ";
			mT2 = b.getMoveMade(m*2)%100;	
			moveStr += intToSquare(mT2) + ", ";
		}
		if (b.getPly() > m*2+1) {
			mF2 = b.getMoveMade(m*2+1)/100;
			moveStr += intToSquare(mF2) + " to ";
			mT2 = b.getMoveMade(m*2+1)%100;	
			moveStr += intToSquare(mT2) + " ";
		}
		
		moveText.loadFromRenderedText(moveStr, textColor, font3);
		moveText.render(BXSTART+(m/21*250)+B_SIZE+40, BYSTART+10+(m%21)*30); 
	}
	
	if (sidey != b.getSide()) {
		sidey = b.getSide();
		if (b.getSide()) 
			turnText.loadFromRenderedText("White to move", textColor, font);
		else
			turnText.loadFromRenderedText("Black to move", textColor, font);
		b.checkCheck(b.getSide(), 1);
		fileText.loadFromRenderedText("a         b        c         d         e         f         g         h", textColor, font2);
	}
	turnText.render(BXSTART, BYSTART+B_SIZE+40);
	checkText.render(BXSTART+B_SIZE-200, BYSTART+B_SIZE+40);
	fileText.render(BXSTART+33, BYSTART+B_SIZE+10);
	for (int i = int('8'); i >= int('1'); i--) {
		rankStr = char(i);
		rankText.loadFromRenderedText(rankStr, textColor, font2);
		rankText.render(BXSTART-35, BYSTART+30+75*(int('8')-i));
	}

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
					SDL_SetRenderDrawColor(renderer, 0, 153, 153, 255);
				}
				else {
					SDL_SetRenderDrawColor(renderer, 255, 255, 55, 255);
				}
			}
			sqPos = {b.squares[sq].getX(),	//X start
				 b.squares[sq].getY(),	//Y start
				 SQ_SIZE, SQ_SIZE};	 //Width, height of square
			SDL_RenderFillRect(renderer, &sqPos);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			//SDL_RenderDrawRect(renderer, &sqPos);
			/*
			SDL_RenderDrawLine(renderer, 
					sqPos.x, sqPos.y,
					sqPos.x+SQ_SIZE, sqPos.y);
			SDL_RenderDrawLine(renderer, 
					sqPos.x, sqPos.y, 
					sqPos.x, sqPos.y+SQ_SIZE);
			*/
		}
	}
}

void drawPieces(const Board& b, const int& mF, const int& mT) {
	int p, sq, x, y, putOnTop = -1;
	SDL_Rect sqPos;
	SDL_Rect clipSq, pOTClipSq;
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
					putOnTop = sq;
					pOTClipSq = clipSq;
				}
				else {
					spriteSheetTexture.render(sqPos.x, sqPos.y, &clipSq);
				}
			}
		}
	}
	if (putOnTop != -1) {
		SDL_GetMouseState(&x, &y);
		if (x < BXSTART + SQ_SIZE/2) 
			x = BXSTART + SQ_SIZE/2;
		if (x > BXSTART + B_SIZE - SQ_SIZE/2) 
			x = BXSTART + B_SIZE - SQ_SIZE/2;
		if (y < BYSTART + SQ_SIZE/2) 
			y = BYSTART + SQ_SIZE/2;
		if (y > BYSTART + B_SIZE - SQ_SIZE/2) 
			y = BYSTART + B_SIZE - SQ_SIZE/2;
		spriteSheetTexture.render(x-SQ_SIZE/2, y-SQ_SIZE/2, &pOTClipSq);
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
	SDL_Rect borderRect = {BXSTART+B_SIZE+25, BYSTART, 500, 650};
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &borderRect);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &borderRect);
	borderRect = {BXSTART+B_SIZE+24, BYSTART-1, 500, 650};
	SDL_RenderDrawRect(renderer, &borderRect);
}
