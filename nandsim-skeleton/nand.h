//----------------------------------------------------------
//
// A simple NAND simulator
//
// Feb. 1, 2017
//
// Jin-Soo Kim (jinsookim@skku.edu)
// Computer Systems Laboratory
// Sungkyunkwan University
// http://csl.skku.edu
//
//----------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define N_BLOCKS	8
#define N_PAGES		8
typedef unsigned int u32;

struct nand_page
{
	u32 	data;
	u32 	spare;
};

struct nand_page Flash[N_BLOCKS*N_PAGES];

// function prototypes
int nand_init (int nblks, int npages);
int nand_read (int blk, int page, u32 *data, u32 *spare);
int nand_write (int blk, int page, u32 data, u32 spare);
int nand_erase (int blk);
int nand_blkdump (int blk);

