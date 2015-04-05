/*
----------------------------------
	~Moosey Chess Engine~
	      display.h
----------------------------------
*/

#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "button.h"
#include "ltexture.h"

class Board;

class Display {
	public:
		Display(Board * b);
		~Display();
	
		void displayBoard(const int& mF = -1, const int& mT = -1);
		void setButtons();
		void setSpriteClips();
		void displayBotText();
		void updateText();
		void drawButtons();
		void drawSquares(const int& mF = -1, const int& mT = -1);
		void drawPieces(const int& mF = -1, const int& mT = -1);
		void drawBorder();
		void drawMoveTable();
		void drawTitleScreen();
		void handleButtons(SDL_Event* e);

		//ACCESSORS
		int getBoardXStart() { return BXSTART; };
		int getBoardYStart() { return BYSTART; };
		int getSqSize() { return SQ_SIZE; };
		int getBoardSize() { return B_SIZE; };
		
	private:
		Board * boardPtr;
		bool sideFlag;
		const int B_SIZE, SQ_SIZE, BXSTART, BYSTART;
		SDL_Rect spriteClips[12], buttonClips[6], titleTextClips[27];
		Button buttons[26];
		LTexture spriteSheetTexture, buttonTexture, titleTexture,
			 titleTextTexture, turnText, checkText, moveText,
			 rankText, fileText;
		SDL_Color textColor;
		std::string rankStr, fileStr;
};

extern Mix_Chunk * mFSound, * mTSound;

#endif
