#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "nand.h"
#include "ftl.h"



int ftl_read(unsigned long lpn){

    if (l2p[lpn]==0xffffffff){
        printf("Read Error!\n");
        return -1;
    }

    else{
        ppn = l2p[lpn];
        return 0;
    } 
    
}

int ftl_write(unsigned long lpn){
    unsigned long old_ppn;
    current_time=clock();
    if((s.host_write ) % N_PAGES_PER_BLOCK ==0) {
        s.free_blk = s.free_blk-1;
        Blk[ppn/N_PAGES_PER_BLOCK].bs = INACTIVE;

        if (s.free_blk==N_OP_BLOCKS){
            GreedyGC(N_BLOCKS,N_PAGES_PER_BLOCK);
            //CBGC(N_BLOCKS,N_PAGES_PER_BLOCK);
            //CatGC(N_BLOCKS,N_PAGES_PER_BLOCK);   
        }

        else if (s.free_blk>N_OP_BLOCKS){
            //printf("No GC\n");
            if(l2p[lpn]==0xffffffff){
                Flash[ppn].spare = lpn;
                Flash[ppn].ps = VALID;
                Blk[ppn/N_BLOCKS].valid_page++;
                Blk[ppn/N_BLOCKS].age = (float)current_time/CLOCKS_PER_SEC;
                l2p[lpn]= ppn;
            }
            else {
                old_ppn = l2p[lpn];
                Flash[old_ppn].ps= INVALID;
                Flash[old_ppn].spare=-1;
                Flash[ppn].ps = VALID;
                Flash[ppn].spare = lpn;
                Blk[ppn/N_BLOCKS].valid_page++;
                Blk[ppn/N_BLOCKS].age =  (float)current_time/CLOCKS_PER_SEC;
                l2p[lpn]= ppn;
            }
            ppn=ppn+1;
        }
             
    }

    //The block of the current ppn is ACTIVE
    else{
        if(l2p[lpn]==0xffffffff){
            Flash[ppn].spare = lpn;
            Flash[ppn].ps = VALID;
            Blk[ppn/N_PAGES_PER_BLOCK].valid_page++;
            Blk[ppn/N_PAGES_PER_BLOCK].age =  (float)current_time/CLOCKS_PER_SEC;
            l2p[lpn]= ppn;
        }
        else{
            old_ppn = l2p[lpn];
            Flash[old_ppn].ps= INVALID;
            Flash[old_ppn].spare=-1;
            Flash[ppn].ps = VALID;
            Flash[ppn].spare = lpn;
            Blk[ppn/N_PAGES_PER_BLOCK].valid_page ++;
            Blk[ppn/N_PAGES_PER_BLOCK].age = (float)current_time/CLOCKS_PER_SEC;
            l2p[lpn]= ppn;
        }        
        ppn=ppn+1;
    }
       
    return 0;
}
int ftl_erase (int blk)
{
	
	int i;
	for(i=0;i<N_PAGES_PER_BLOCK;i++){
		Flash[blk*N_PAGES_PER_BLOCK+i].spare = 0xffffffff;
		Flash[blk*N_PAGES_PER_BLOCK+i].ps = EMPTY;
	}
    Blk[blk].valid_page = 0;
	Blk[blk].bs=FREE;
    Blk[blk].nErase = Blk[blk].nErase + 1;

	s.free_blk = s.free_blk + 1;
	
    ppn = blk*N_PAGES_PER_BLOCK;

	return 0;
	
}

int GreedyGC(int nblks,int npages){
    //printf("Go To Greedy!"); //ok
    //Select the victim block from other blocks
    int i ;
    int Victim_blk=0;
    int max=0;
    for(i=0;i<nblks;i++){
        if((1-(Blk[i].valid_page)>max)){
            max = 1-Blk[i].valid_page;
            Victim_blk = i;
        }
    }
    //Copy valid page from victim block to free block
    for(i=0;i<npages;i++){
        if(Flash[Victim_blk*N_PAGES_PER_BLOCK+i].ps==VALID){
            lpn = Flash[Victim_blk*N_PAGES_PER_BLOCK+i].spare;
            Flash[ppn].spare = lpn;
            Flash[ppn].ps=VALID;
            s.gc_write=s.gc_write+1;
            Blk[ppn/N_PAGES_PER_BLOCK].valid_page ++;
            Blk[ppn/N_PAGES_PER_BLOCK].bs=ACTIVE;
            l2p[lpn]= ppn;
        }
        ppn=ppn+1;
    }

    s.gc++;
    //Erase victim block
    ftl_erase(Victim_blk);
    return 0;
}

int CBGC(int nblks,int npages){
    int i ;
    int Victim_blk=0;
    float min=__DBL_MAX__;
    float age;
    float CB;
    float u=0.00;
    
    for(i=0;i<nblks;i++){
        u=(float)Blk[i].valid_page/N_PAGES_PER_BLOCK;
        age=Blk[i].age;
        CB = (1.00-u/(2.00*u))*age;
        if (CB<min){
            min=CB;
            Victim_blk = i;
        }
    }

    for(i=0;i<npages;i++){
        current_time=clock();
        if(Flash[Victim_blk*N_PAGES_PER_BLOCK+i].ps==VALID){
            lpn = Flash[Victim_blk*N_PAGES_PER_BLOCK+i].spare;
            Flash[ppn].spare = lpn;
            Flash[ppn].ps = VALID;
            s.gc_write++;
            Blk[ppn/N_PAGES_PER_BLOCK].valid_page ++;
            Blk[ppn/N_PAGES_PER_BLOCK].age = (float)current_time/CLOCKS_PER_SEC;
            Blk[ppn/N_PAGES_PER_BLOCK].bs=ACTIVE;
            l2p[lpn]= ppn;
        }
        ppn=ppn+1;
    }

    s.gc++;
    ftl_erase(Victim_blk);


    return 0;
}

int CatGC(int nblks,int npages){
    int i ;
    int Victim_blk=0;
    float max=0.00;
    float age;
    float cat;
    float u=0.00;
    
    for(i=0;i<nblks;i++){
        u=(float)Blk[i].valid_page/N_PAGES_PER_BLOCK;
        age=Blk[i].age;
        cat = u/((1-u)*age*Blk[i].nErase);
        //cat = (u/(1-u))*(1.00/age)*(Blk[i].nErase);
        if (cat>max){
            max=cat;
            Victim_blk = i;
        }
    }

    for(i=0;i<npages;i++){
        current_time=clock();
        if(Flash[Victim_blk*N_PAGES_PER_BLOCK+i].ps==VALID){
            lpn = Flash[Victim_blk*128+i].spare;
            Flash[ppn].spare = lpn;
            Flash[ppn].ps = VALID;
            s.gc_write++;
            Blk[ppn/N_PAGES_PER_BLOCK].valid_page ++;
            Blk[ppn/N_PAGES_PER_BLOCK].age = (float)current_time/CLOCKS_PER_SEC;
            Blk[ppn/N_PAGES_PER_BLOCK].bs=ACTIVE;
            l2p[lpn]= ppn;
        }
        ppn=ppn+1;
    }

    s.gc++;
    ftl_erase(Victim_blk);

    return 0;
}
