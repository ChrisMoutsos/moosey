/*
----------------------------------
	~Moosey Chess Engine~
	     zobrist.cpp
----------------------------------
*/

#include <stdlib.h>
#include "zobrist.h"

Zobrist::Zobrist() {
	//Euler lends his seed
	srand(2718281828);

	//Fill each index with random numbers
	for (int p = 0; p < 6; p++)
		for (int s = 0; s < 2; s++)
			for (int sq = 0; sq < 64; sq++)
				piece[p][s][sq] = (rand() << sizeof(long)/2 * sizeof(char))
						   + rand();

	side = (rand() << sizeof(long)/2 * sizeof(char)) + rand();
	for (int s = 0; s < 2; s++)
		for (int d = 0; d < 2; d++)
			castling[s][d] = (rand() << sizeof(long)/2 * sizeof(char))
					 + rand();
	for (int f = 0; f < 8; f++)
		enPassant[f] = (rand() << sizeof(long)/2 * sizeof(char))
				+ rand();
}
