#include "bitboards.h"
#include <iostream>

Bitboards::Bitboards() {
	for (int s = BLACK; s <= WHITE; s++) {
		for (int pType = ROOK; pType <= PAWN; pType++) {
			pieces[s][pType] = 0;
		}
	}
	allPieces[BLACK] = allPieces[WHITE] = 0;

	U64 x = 1;
	x <<= 7;
	for (int s = 0; s < 64; s++) {
		sq[s] = x;
		if ((s+1)%8 == 0) {
			x <<= 15;
		}
		else {
			x >>= 1;
		}
	}
}
