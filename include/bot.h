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
	//ply*10 plus 0 (exact), 1 (lower), 2 (upper)
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


		void storeTTEntry(Board& b, int move, int depthLeft, int score, int nodeType);
		HASHENTRY getTTEntry(unsigned long long key);
		void clearTT();

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
		const static int TTSIZE = 2*2*2*1048576;
		HASHENTRY* transTable;
};

#endif
