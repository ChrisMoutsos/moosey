/*
----------------------------------
	~Moosey Chess Engine~
	      button.h
----------------------------------
*/

#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>

class Board;

class Button {
	public:
		Button();

		bool handleEvent(SDL_Event* e);

		//ACCESSORS
		int getX() const { return pos.x; };
		int getY() const { return pos.y; };
		int getW() const { return w; };
		int getH() const { return h; };
		bool getInside() const { return inside; };
		bool getClicking() const { return clicking; };
		//MUTATORS
		void setInside(bool b) { inside = b; };
		void setClicking(bool b) { clicking = b; };
		void setButt(int b) { butt = b; };
		void setPos(int x, int y);
		void setW(int x) { w = x > 0 ? x : 0; };
		void setH(int x) { h = x > 0 ? x : 0; };
		void setSize(int x, int y);
		void setBoardPtr(Board * b) { boardPtr = b; };
	private:
		Board * boardPtr;
		int butt;	//Which button it is
		int w, h;
		bool inside, clicking;
		SDL_Point pos;
};

#endif
