#ifndef BITBOARDS_H
#define BITBOARDS_H

#include "common.h"

class Bitboards {
	friend class Board;

	public:
		Bitboards();

	private:
		//pieces[color][piecetype]
		U64 pieces[2][6];
		U64 allPieces[2];
		U64 sq[64];
};

#endif
