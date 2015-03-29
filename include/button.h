/*
----------------------------------
	~Moosey Chess Engine~
	      button.h
----------------------------------
*/

#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>

enum ButtonSprite { noButton = -1, undo = 0, restart = 0
};

//class LTexture;
class Board;

class Button {
	public:
		Button();
		void setPos(int x, int y);
		bool handleEvent(SDL_Event* e, Board& b);
		void render();
		int getX() const { return pos.x; };
		int getY() const { return pos.y; };
		bool getInside() const { return inside; };
		bool getClicking() const { return clicking; };
		void setInside(bool b) { inside = b; };
		void setClicking(bool b) { clicking = b; };
		void setButt(int b) { butt = b; };
	private:
		int butt;	//Which button it is
		bool inside, clicking;
		SDL_Point pos;
};

/*
extern SDL_Rect buttonClips[6];
extern LTexture buttonTexture;
*/

#endif
