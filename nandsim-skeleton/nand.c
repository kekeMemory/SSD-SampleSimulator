//----------------------------------------------------------
//
// A simple NAND simulator
//
// Feb. 1, 2017.
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
#include "nand.h"


int nand_init (int nblks, int npages)
{
	// initialize the NAND flash memory 
	// "blks": the total number of flash blocks
	// "npages": the number of pages per block
	// returns 0 on success
	// returns -1 on errors with printing appropriate error message
	
	int i,j;
	for (j=0;j<nblks;j++){
		for (i = 0;i<npages;i++){
			Flash[j*8+i].data = 0xff;
			Flash[j*8+i].spare = 0xff;
			//printf("Flash[%d]=0x%08x\n",j*8+i,Flash[j*8+i].data);
		}
	}
	
	if ((nblks>0) && (npages>0)){
		printf("NAND:%d blocks, %d pages per block, %d pages\n",nblks,npages,nblks*npages);
		return 0;
	}
	
	else{
		printf("Invalid Block/Page !\n");
		return -1;
	}
}
	
int nand_write (int blk, int page, u32 data, u32 spare)
{
	// write "data" and "spare" into the NAND flash memory pointed to by "blk" and "page"
	// returns 0 on success
	// returns -1 on errors with printing appropriate error message
	
	printf("write(%d,%d ): ",blk,page);
	//printf("Flash(%d)=0x%08x\n",blk*8+page,Flash[blk*8+page].data);

	if ((page<0)||(page>7)){
		printf("failed, invalid page number\n");
		return -1;
	}
	
	if ((blk<0) || (blk>7)){
		printf("failed, invalid block number\n");
		return -1;
	}

	if (data<0){
		printf("failed, invalid data\n");
		return -1;
	}

	if(spare<0){
		printf("failed, invalid spare\n");
		return -1;
	}

	if (page>0){
		if(Flash[blk*8+page-1].data == 0xff){
			printf("failed, the page is not being sequentially written\n");
			return -1;
		}
		if(Flash[blk*8+page].data!=0xff){
			printf("failed, the page was already written\n");
			return -1;
		}
		else{
			Flash[blk*8+page].data = data;
			Flash[blk*8+page].spare = spare;
			printf("data = 0x%08x, spare = 0x%08x\n",Flash[blk*8+page].data,Flash[blk*8+page].spare);
			return 0;
		}
	}

	else 
	{
		if(Flash[blk*8+page].data!=0xff){
			printf("failed, the page was already written\n");
			return -1;
		}
		else{
			Flash[blk*8+page].data = data;
			Flash[blk*8+page].spare = spare;
			printf("data = 0x%08x, spare = 0x%08x\n",Flash[blk*8+page].data,Flash[blk*8+page].spare);
			return 0;
		}
	}
}

int nand_read (int blk, int page, u32 *data, u32 *spare)
{
	// read "data" and "spare" from the NAND flash memory pointed to by "blk" and "page"
	// returns 0 on success
	// returns -1 on errors with printing appropriate error message

	printf("read(%d,%d ): ",blk,page);
	if ((page<0)||(page>7)){
		printf("failed, invalid page number\n");
		return -1;
	}
	
	if ((blk<0) || (blk>7)){
		printf("failed, invalid block number\n");
		return -1;
	}

	if (data<0){
		printf("failed, invalid data\n");
		return -1;
	}

	if(spare<0){
		printf("failed, invalid spare\n");
		return -1;
	}

	if(Flash[blk*8+page].data == 0xff){
		printf("failed,trying to read an empty page\n");
		return -1;
	}

	else{
		*data=Flash[blk*8+page].data;
		*spare=Flash[blk*8+page].spare;
		printf("data = 0x%08x, spare = 0x%08x\n",*data,*spare);
		return 0;
	}
}

int nand_erase (int blk)
{
	// erase the NAND flash memory block "blk"
	// returns 0 on success
	// returns -1 on errors with printing appropriate error message

	printf("erase(%d): ",blk);
	
	if ((blk<0) || (blk>7)){
		printf("failed, invalid block number\n");
		return -1;
	}

	else{
		int i;
		for(i=0;i<N_PAGES;i++){
			Flash[blk*8+i].data = 0xff;
			Flash[blk*8+i].spare = 0xff;
		}
		printf("block erased\n");
		return 0;
	}
}


int nand_blkdump (int blk)
{
	// dump the contents of the NAND flash memory block "blk" (for debugging purpose)
	// returns 0 on success
	// returns -1 on errors with printing appropriate error message

	int i;
	int CNT=0;
	for(i=0;i<N_PAGES;i++){
		if(Flash[blk*8+i].data != 0xff){
			CNT+=1;
		}
	}
	if (CNT ==0)
	{
		printf("Blk		%d: FREE\n",blk);
	}
	else{
		printf("Blk		%d: Total %d page(s) written\n",blk,CNT);
		for(i=0;i<N_PAGES;i++){
			if(Flash[blk*8+i].data != 0xff){
				printf("	Page   %d: data = 0x%08x, spare = 0x%08x\n",i,Flash[blk*8+i].data,Flash[blk*8+i].spare);
			}
		}
	}
	return 0;
}

