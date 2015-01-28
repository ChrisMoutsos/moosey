#ifndef GLOBAL_H
#define GLOBAL_H

#define FR2SQ64(file, rank) ((rank-1)*8 + file) //Gives 64 index from file,rank
#define SQ642R(sq64) ((int)(sq64-1)/8 + 1) //Gives rank (1-8) from square64
#define SQ642F(sq64) (sq64 - ((sq64-1)/8)*8) //Gives file (1-8) from square64

#endif
