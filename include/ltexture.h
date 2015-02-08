#ifndef LTEXTURE_H
#define LTEXTURE_H

class LTexture {
	public:
		LTexture();
		~LTexture();
		bool loadFromFile(std::string path);

		#ifdef _SDL_TTF_H
		bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
		#endif
		void free();
		void setColor(Uint8 red, Uint8 green, Uint8 blue);
		void setBlendMode(SDL_BlendMode blending);
		void setAlpha(Uint8 alpha);
		void render(int x, int y, SDL_Rect* clip = NULL,
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

#endif