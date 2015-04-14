/*
----------------------------------
	~Moosey Chess Engine~
	      ltexture.h
----------------------------------
*/

#include <SDL2/SDL_ttf.h>
#include <string>

#ifndef LTEXTURE_H
#define LTEXTURE_H

class LTexture {
	friend class Display;
	
	public:
		LTexture();
		~LTexture();
		bool loadFromFile(SDL_Renderer*, std::string path);
		bool loadFromRenderedText(SDL_Renderer*, std::string textureText, SDL_Color textColor, TTF_Font* font);
		void free();
		void setColor(Uint8 red, Uint8 green, Uint8 blue);
		void setBlendMode(SDL_BlendMode blending);
		void setAlpha(Uint8 alpha);
		void render(SDL_Renderer*, int x, int y, SDL_Rect* clip = NULL,
			    double angle = 0.0, SDL_Point* center = NULL,
			    SDL_RendererFlip fljip = SDL_FLIP_NONE);
		
		int getWidth();
		int getHeight();
	
	private:
		SDL_Texture* texture;
		int width;
		int height;
};

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern TTF_Font* Garamond26, *Garamond28, *Cicero22, *Cicero26;

#endif
