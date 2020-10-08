#include <stdio.h>
#include <stdlib.h>
#include "ftl.h"
#include "nand.h"

int ftl_open(unsigned long lpn){
    //Initialize Nand
    //Initialize data structure 
    int i;
    for (i=0;i<lpn;i++){
        l2p[i]=0x00;
    }
    
    return 0;
}

int ftl_read(unsigned long lpn){
    //Get the PPN through the L2P table
    //L2P table is DRAM
    //Read it from NAND to DRAM
    if (l2p[lpn]==0x00){
        printf("Read Error!\n");
        return -1;
    }
    else {
        ppn = l2p[lpn];
        read_data=Flash[ppn].data;
        return 0;
    }
    
}

int ftl_write(unsigned long lpn){
     
    //Check GC
    //If old data already exist, invalid old data
    //Write it from DRAM to NAND
    //Set the PPN into the L2P table
    
    //printf("FTL  WRITE\n");
    
    //When full the page in one blk,block state of the current block is INACTIVE
    if((s.host_write ) % N_PAGES_PER_BLOCK ==0x00) {
        blk_info.free_blk = blk_info.free_blk-1;
        Blk[ppn/N_PAGES_PER_BLOCK].bs = INACTIVE;

        if (blk_info.free_blk>1){
            //printf("No GC\n");
            if(l2p[lpn]==0x00){
            
                //printf("lpn : %ld",lpn); //ok
                Flash[ppn].spare = lpn;
                //printf("    lpn : %d",Flash[ppn].spare); //ok
                Flash[ppn].ps = VALID;
                Blk[ppn/N_BLOCKS].valid_page = Blk[ppn/N_BLOCKS].valid_page + 1;
                l2p[lpn]= ppn;
                //printf("  ppn : %ld\n",ppn); //ok
                //printf("    l2p[lpn] : %d\n",l2p[lpn]); //ok
            }
            else {
                //printf("Mapping Table is not empty!\n"); //ok
                //printf("lpn : %ld",lpn); //ok
                written_ppn = l2p[lpn];
                //printf("  ppn : %ld\n",written_ppn); //ok
                //printf("    l2p[%ld] : %d\n",lpn,l2p[lpn]); //ok
                Flash[written_ppn].ps= INVALID;
                Flash[ppn].spare = lpn;
                //printf("    lpn : %d",Flash[ppn].spare); //ok
                Flash[ppn].ps = VALID;
                Blk[ppn/N_BLOCKS].valid_page = Blk[ppn/N_BLOCKS].valid_page + 1;
                l2p[lpn]= ppn;
                //printf("  ppn : %ld\n",ppn); //ok
                //printf("    l2p[%ld] : %d\n",lpn,l2p[lpn]); //ok
            }
            ppn=ppn+1;
        }
        
        else if (blk_info.free_blk==1){
            //printf("GC Starting...\n"); error
            GreedyGC(N_BLOCKS,N_PAGES_PER_BLOCK);
            
        }
    
    }

    //The block of the current ppn is ACTIVE
    else{
        Blk[ppn/N_PAGES_PER_BLOCK].bs = ACTIVE;
        if(l2p[lpn]==0x00){
            Flash[ppn].spare = lpn;
            Flash[ppn].ps = VALID;
            Blk[ppn/N_BLOCKS].valid_page = Blk[ppn/N_BLOCKS].valid_page + 1;
            l2p[lpn]= ppn;
        }

        else{
            written_ppn = l2p[lpn];
            Flash[written_ppn].ps= INVALID;
            Flash[ppn].spare = lpn;
            Flash[ppn].ps = VALID;
            Blk[ppn/N_BLOCKS].valid_page = Blk[ppn/N_BLOCKS].valid_page + 1;
            l2p[lpn]= ppn;
        }        
        ppn=ppn+1;
    }
       
    return 0;
}
int ftl_erase (int blk)
{
	// erase the NAND flash memory block "blk"
	// returns 0 on success
	// returns -1 on errors with printing appropriate error message
	
	
	int i;
	for(i=0;i<N_PAGES_PER_BLOCK;i++){
		Flash[blk*N_PAGES_PER_BLOCK+i].data = 0xff;
		Flash[blk*N_PAGES_PER_BLOCK+i].spare = 0x00;
		Flash[blk*N_PAGES_PER_BLOCK+i].ps = VALID;
	}
    Blk[blk].valid_page = N_PAGES_PER_BLOCK;
	Blk[blk].bs=FREE;

	blk_info.free_blk = blk_info.free_blk + 1;
	
    ppn = blk*N_PAGES_PER_BLOCK;

	return 0;
	
}

int GreedyGC(int nblks,int npages){
    //printf("Go To Greedy!"); //ok
    //Select the victim block from other blocks
    int i ;
    int Victim_blk=0;
    float max=0;
    for(i=0;i<nblks;i++){
        if((1-((Blk[i].valid_page)/N_PAGES_PER_BLOCK))>max){
            max = 1-(Blk[i].valid_page)/N_PAGES_PER_BLOCK;
            Victim_blk = i;
            //printf("Victim Block: %d\n",Victim_blk);
        }
    }
    // printf("Victim Block: %d\n",Victim_blk); error
    //Copy valid page from victim block to free block
    for(i=0;i<npages;i++){
        if(Flash[Victim_blk*128+i].ps==VALID){
            lpn = Flash[Victim_blk*128+i].spare;
            Flash[ppn].spare = lpn;
            s.gc_write++;
            Flash[ppn].ps = VALID;
            Blk[ppn/N_BLOCKS].valid_page = Blk[ppn/N_BLOCKS].valid_page + 1;
            l2p[lpn]= ppn;
        }
        ppn=ppn+1;
    }

    s.gc++;

    //Erase victim block
    ftl_erase(Victim_blk);
    return 0;
}