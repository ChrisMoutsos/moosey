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

struct HASHENTRY {
	unsigned long zKey;
	//((mF*100+mT)*10000 + abs(score)) * score/(abs)(score)
	int bestMoveAndScore;
	//depthLeft*10 plus 0 (exact), 1 (lower), 2 (upper)
	int depthAndNodeType;
};

class Bot {
	public:
		Bot();
		
		void reset();
		int think(Board& b, int depth);
		int alphaBeta(Board& b, int alpha, int beta, int depthLeft, int depthGone,
			      LINE* pline, bool allowNull, int ext);
		int quies(Board& b, int alpha, int beta, int depthGone);
		int getFromHH(int mF, int mT);

		int getLevel() const { return level; };
		void setLevel(int d) { level = d; };

	private:
		LINE prinVarLine, oldPrinVarLine;
		double totalTime;
		int nodes, qNodes, r;
		SDL_Event e;
		//History heuristic table
		int hh[64][64];
		//Killer moves
		int killers[30][2];
		int level;
		//Transposition table
		const static int TTSIZE = 131072;
		HASHENTRY transTable[TTSIZE];
};

#endif
