/*
----------------------------------
	~Moosey Chess Engine~
	      search.h
----------------------------------
*/

#ifndef SEARCH_H
#define SEARCH_H

#include <vector>
#include <SDL2/SDL.h>

class Board;

struct LINE {
	int count;
	int move[20];
};

class Bot {
	public:
		Bot();
		~Bot();
		
		void reset();
		int think(Board& b, int depth);
		int alphaBeta(Board& b, int alpha, int beta, int depthLeft, int depthGone,
			      LINE* pline, bool allowNull, int ext);
		int quies(Board& b, int alpha, int beta, int depthGone);
		int getFromHH(int mF, int mT);

	private:
		LINE prinVarLine, oldPrinVarLine;
		double totalTime;
		int nodes, qNodes, r;
		SDL_Event e;
		//History heuristic table
		int hh[64][64];
		//Killer moves
		int killers[30][2];
};

#endif
