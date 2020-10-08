//----------------------------------------------------------
//
// NAND simulator tester
//
// Feb. 1, 2017.
//
// Jin-Soo Kim (jinsookim@skku.edu)
// Computer Systems Laboratory
// Sungkyunkwan University
// http://csl.skku.edu
//
//---------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nand.h"



int main (int argc, char *argv[])
{
	int i;
	u32 d, s;
	struct nand_page Flash[64];
	memset(Flash,0,sizeof(struct nand_page)*64);
	nand_init (N_BLOCKS, N_PAGES);

	// write a block in full
	for (i = 0; i < N_PAGES; i++)
		nand_write (3, i, i, i<<16);

	// read the block in the reverse order
	for (i = N_PAGES - 1; i >= 0; i--)
		nand_read (3, i, &d, &s);

	nand_write (4, 0, 0xcafe, 0xbabe);

	// trying to overwrite 
	nand_write (4, 0, 0xdead1, 0xbeef1);

	// trying to write non-sequentially
	nand_write (4, 2, 0xdead2, 0xbeef2);

	// trying to read an empty page
	nand_read (4, 3, &d, &s);
	nand_read (0, 0, &d, &s);
	nand_read (N_BLOCKS - 1, 0, &d, &s);

	// erase a block
	nand_erase (3);
	nand_erase (4);
	
	// trying to erase an empty block
	nand_erase (0);

	// trying to write the erased block
	for (i = 0; i < N_PAGES; i++)
		nand_write (3, i, i<<24, i);

	nand_write (4, 0, 0xbad, 0xc0ffee);
	nand_write (4, 1, 0xface, 0xb00c);
	nand_read (3, 0, &d, &s);
	nand_read (4, 0, &d, &s);

	// invalid page number
	nand_write (4, -1, 0xdead3, 0xbeef3);
	nand_read (4, -1, &d, &s);
	nand_write (4, N_PAGES, 0xdead4, 0xbeef4);
	nand_read (4, N_PAGES, &d, &s);
	
	// invalid block number
	nand_write (-1, 0, 0xdead5, 0xbeef5);
	nand_read (-1, 0, &d, &s);
	nand_write (N_BLOCKS, 0, 0xdead6, 0xbeef6);
	nand_read (N_BLOCKS, 0, &d, &s);
	nand_erase (-1);
	nand_erase (N_BLOCKS);
	nand_erase (N_BLOCKS + 1000000);

	for (i = 0; i < N_BLOCKS; i++)
		nand_blkdump (i);

	return 0;
}



