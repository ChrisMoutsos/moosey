/*
----------------------------------
	~Moosey Chess Engine~
	      display.h
----------------------------------
*/

#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

class Board;
class Square;
class Button;
class LTexture;

extern Button buttons[26];

const int B_SIZE = 600;
const int SQ_SIZE = 75;
const int BXSTART = 50;
const int BYSTART = 25;

void displayBoard(Board& b, const int& mF = -1, const int& mT = -1);
void setButtons();
void setSpriteClips();
void displayBotText(const Board& b);
void updateText(const Board& b, bool& sidey);
void drawButtons(const Board& b);
void drawSquares(const Board& b, const int& mF = -1, const int& mT = -1);
void drawPieces(Board& b, const int& mF = -1, const int& mT = -1);
void drawBorder();
void drawMoveTable(const Board&);
void drawTitleScreen(const Board&);

extern SDL_Rect spriteClips[12], buttonClips[6], titleTextClips[27];
extern LTexture spriteSheetTexture, buttonTexture, titleTexture, titleTextTexture;
extern Mix_Chunk * mFSound, * mTSound;

#endif
