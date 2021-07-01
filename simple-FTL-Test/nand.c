#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
				Flash[j*npages+i].spare =0;
				Flash[j*npages+i].ps = EMPTY;
				
			}

			Blk[j].valid_page = 0;
			Blk[j].bs=FREE;
			Blk[j].nErase=0;
			Blk[j].age=(float)start_time;
		}
		
	}

	return 0;
	
}


int l2p_map(unsigned long lpn){
    //Initialize Nand
    //Initialize data structure 
    int i;
    for (i=0;i<lpn;i++){
        l2p[i]=0xffffffff;
    }
    return 0;
}
