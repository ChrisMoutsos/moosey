#include "bitboards.h"
#include <iostream>

Bitboards::Bitboards() {
	step[NORTH] = 8;
	step[NORTHEAST] = 9;
	step[EAST] = 1;
	step[SOUTHEAST] = -7;
	step[SOUTH] = -8;
	step[SOUTHWEST] = -9;
	step[WEST] = -1;
	step[NORTHWEST] = 7;

	//Ranks
	U64 rank1 = 0x00000000000000FF;
	for (int r = 1; r <= 8; r++, rank1 <<= 8) {
		rank[r - 1] = rank1;
	}

	//Files
	U64 aFile = 0x0101010101010101;
	for (int f = 1; f <= 8; f++, aFile <<= 1) {
		file[f - 1] = aFile;
	}


	//Generate piece bitboards
	for (int s = BLACK; s <= WHITE; s++) {
		for (int pType = ROOK; pType <= PAWN; pType++) {
			pieces[s][pType] = 0;
		}
	}
	allPieces[BLACK] = allPieces[WHITE] = 0;

	//Generate sq[64]
	U64 x = 1;
	//x <<= 7;
	for (int s = 0; s < 64; s++) {
		sq[s] = x;
		x <<= 1;
	}

	//Generate ray attacks
	//North
	U64 north = 0x0101010101010100;
	for (int s = 0; s < 64; s++, north <<= 1) {
		rayAttacks[NORTH][s] = north;
	}

	//South
	U64 south = 0x0080808080808080;
	for (int s = 63; s >= 0; s--, south >>= 1) {
		rayAttacks[SOUTH][s] = south;

	}
	//East
	/*
	U64 east;
	for (int r = 8; r >= 1; r--) {
		east = 0xFE00000000000000;
		east >>= (8 - r) * 8;
		for (int f = 1; f <= 8; f++) {
			rayAttacks[EAST][FR2SQ64(f, r) - 1] = east;
			east <<= 1;
		}
	}
	*/
	U64 east;
	for (int f = 1; f <= 8; f++) {
		east = 0xFF00000000000000;
		east <<= (f - 1);
		for (int cancelFile = 1; cancelFile <= f; cancelFile++) {
			east &= ~file[cancelFile - 1];
		}
		for (int r = 8; r >= 1; r--) {
			rayAttacks[EAST][FR2SQ64(f, r) - 1] = east;
			east >>= 8;
		}
	}

	//West
	/*
	U64 west;
	for (int r = 8; r >= 1; r--) {
		west = 0x7F00000000000000;
		west >>= (r - 8) * 8;
		for (int f = 8; f >= 1; f--) {
			rayAttacks[WEST][FR2SQ64(f, r) - 1] = west;
			west >>= 1;
		}
	}
	*/
	U64 west;
	for (int f = 8; f >= 1; f--) {
		west = 0xFF00000000000000;
		west >>= (8 - f);
		for (int cancelFile = 8; cancelFile >= f; cancelFile--) {
			west &= ~file[cancelFile - 1];
		}
		for (int r = 8; r >= 1; r--) {
			rayAttacks[WEST][FR2SQ64(f, r) - 1] = west;
			west >>= 8;
		}
	}

	//Northeast
	U64 northeast;
	for (int f = 1; f <= 8; f++) {
		northeast = 0x8040201008040201;
		northeast <<= (f - 1);
		for (int cancelFile = 1; cancelFile <= f; cancelFile++) {
			northeast &= ~file[cancelFile-1];
		}
		for (int r = 1; r <= 8; r++) {
			rayAttacks[NORTHEAST][FR2SQ64(f, r) - 1] = northeast;
			northeast <<= 8;
		}
	}

	//Northwest
	U64 northwest;
	for (int f = 8; f >= 1; f--) {
		northwest = 0x0102040810204080;
		northwest >>= (8 - f);
		for (int cancelFile = 8; cancelFile >= f; cancelFile--) {
			northwest &= ~file[cancelFile-1];
		}
		for (int r = 1; r <= 8; r++) {
			rayAttacks[NORTHWEST][FR2SQ64(f, r) - 1] = northwest;
			northwest <<= 8;
		}
	}

	//Southeast
	U64 southeast;
	for (int f = 1; f <= 8; f++) {
		southeast = 0x0102040810204080;
		southeast <<= (f - 1);
		for (int cancelFile = 1; cancelFile <= f; cancelFile++) {
			southeast &= ~file[cancelFile-1];
		}
		for (int r = 8; r >= 1; r--) {
			rayAttacks[SOUTHEAST][FR2SQ64(f, r) - 1] = southeast;
			southeast >>= 8;
		}
	}

	//Southwest
	U64 southwest;
	for (int f = 8; f >= 1; f--) {
		southwest = 0x8040201008040201;
		southwest >>= (8 - f);
		for (int cancelFile = 8; cancelFile >= f; cancelFile--) {
			southwest &= ~file[cancelFile-1];
		}
		for (int r = 8; r >= 1; r--) {
			rayAttacks[SOUTHWEST][FR2SQ64(f, r) - 1] = southwest;
			southwest >>= 8;
		}
	}

	//Horizontal attacks
	for (int s = 0; s < 64; s++) {
		hozAttacks[s] = rayAttacks[NORTH][s] |
			rayAttacks[SOUTH][s] |
			rayAttacks[EAST][s] |
			rayAttacks[WEST][s];
	}

	//Diagonal attacks
	for (int s = 0; s < 64; s++) {
		diagAttacks[s] = rayAttacks[NORTHEAST][s] |
			rayAttacks[NORTHWEST][s] |
			rayAttacks[SOUTHEAST][s] |
			rayAttacks[SOUTHWEST][s];
	}

	//King attacks
	for (int s = 0; s < 64; s++) {
		kingAttacks[s] = 0;
		for (int d = NORTH; d <= NORTHWEST; d++) {
			if (s + step[d] < 64 && s + step[d] >= 0) {
				kingAttacks[s] |= sq[s + step[d]];
			}
		}
		for (int f = 1; f <= 8; f++) {
			if (f < from64(s + 1) % 10 - 1 || f > from64(s + 1) % 10 + 1) {
				kingAttacks[s] &= ~file[f - 1];
			}
		}
	}

	//Knight attacks
	int extra;
	for (int s = 0; s < 64; s++) {
		knightAttacks[s] = 0;
		for (int i = 1; i <= 8; i++) {
			extra = i == 1 ? 17 : i == 2 ? 10 : i == 3 ? -6 : i == 4 ? -15 : i == 5 ? -17 : i == 6 ? -10 : i == 7 ? 6 : 15;
			if (s + extra < 64 && s + extra >= 0) {
				knightAttacks[s] |= sq[s + extra];
			}
			for (int f = 1; f <= 8; f++) {
				if (f < from64(s + 1) % 10 - 2 || f > from64(s + 1) % 10 + 2) {
					knightAttacks[s] &= ~file[f - 1];
				}
			}
		}
	}

	//Pawn moves
	int f;
	for (int s = 0; s < 64; s++) {
		pawnAttacks[WHITE][s] = 0;
		pawnAttacks[BLACK][s] = 0;
		if (s < 8 || s > 55) continue;
		f = from64(s + 1) % 10;
		if (f != 1) {
			pawnAttacks[WHITE][s] |= sq[s + 7];
			pawnAttacks[BLACK][s] |= sq[s - 9];
		}
		if (f != 8) {
			pawnAttacks[WHITE][s] |= sq[s + 9];
			pawnAttacks[BLACK][s] |= sq[s - 7];
		}
	}
}

void Bitboards::setBit(unsigned int bit, U64& bb) {
	bb |= sq[bit];
}

void Bitboards::unsetBit(unsigned int bit, U64& bb) {
	bb &= ~sq[bit];
}

void Bitboards::flipBit(unsigned int bit, U64& bb) {
	bb ^= sq[bit];
}

bool Bitboards::queryBit(unsigned int bit, U64& bb) const {
	return ((bb & sq[bit]) != 0);
}

int Bitboards::bitScanForward(U64& bb) const {
	int b = 0;
	while (!queryBit(b, bb)) {
		b++;
	}
	return b;
}

int Bitboards::bitScanReverse(U64& bb) const {
	int b = 63;
	while (!queryBit(b, bb)) {
		b--;
	}
	return b;
}
