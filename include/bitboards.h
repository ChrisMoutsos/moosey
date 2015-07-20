#ifndef BITBOARDS_H
#define BITBOARDS_H

#include "common.h"

class Bitboards {
	friend class Board;

	public:
		Bitboards();
		
		//Bit ranges from 0-63
		void setBit(unsigned int bit, U64& bb);
		void unsetBit(unsigned int bit, U64& bb);
		void flipBit(unsigned int bit, U64& bb);
		bool queryBit(unsigned int bit, U64& bb) const;
		int bitScanForward(U64& bb) const;
		int bitScanReverse(U64& bb) const;

	private:
		//Holds step increment (64) for each direction
		int step[8];
		//Each rank
		U64 rank[8];
		//Each file
		U64 file[8];
		//pieces[color][piecetype]
		U64 pieces[2][6];
		//Holds locations of all pieces of a color
		U64 allPieces[2];
		//Holds an empty bitboard with only sq on
		U64 sq[64];
		//Holds ray attacks for each square, in 8 directions
		U64 rayAttacks[8][64];
		//Holds horizontal attacks for each square
		U64 hozAttacks[64];
		//Holds diagonal attacks for each square
		U64 diagAttacks[64];
		//Holds king attacks for each square
		U64 kingAttacks[64];
		//Holds knights attacks for each square
		U64 knightAttacks[64];
		//Holds pawn attacks for each square
		U64 pawnAttacks[2][64];

};

#endif
