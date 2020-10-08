#include <stdio.h>
#include <stdlib.h>
#include "nand.h"
#include "ftl.h"

int nand_init (int nblks, int npages)
{
	// initialize the NAND flash memory 
	// "blks": the total number of flash blocks
	// "npages": the number of pages per block
	// returns 0 on success
	// returns -1 on errors with printing appropriate error message
	//struct nand_page Flash[nblks*npages];
	int i,j;

	if ((nblks<0) || (npages<0)){
		printf("Invalid Block/Page !\n");
		return -1;
	}
	
	else{
		for (j=0;j<nblks;j++){
			for (i = 0;i<npages;i++){
				Flash[j*npages+i].data = 0xff;
				Flash[j*npages+i].spare = 0x00;
				Flash[j*npages+i].ps = EMPTY;
				
			}
			Blk[j].valid_page = npages;
			Blk[j].bs=FREE;
		}
		blk_info.free_blk = nblks;
	}


	return 0;
	
}




	