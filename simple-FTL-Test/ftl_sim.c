//----------------------------------------------------------
//
// A Simple Page-mapping FTL Simulator
//
// Feb. 2, 2017
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
#include "ftl.h"
#include <time.h>



char *bit2cap (int b)
{
	static char *unit[] = {"", "K", "M", "G", "T", "P", "E"};
	static char buf[32];

	int u = b / 10;
    if (u > 6)
        return "<Too Big>";
	sprintf (buf, "%d%s", (1 << (b - u*10)), unit[u]);
	return buf;
}

void show_info (void)
{
	printf ("SSD capacity:\t\t%sB\n", bit2cap (SSD_SHIFT));
	printf ("Page size:\t\t%sB\n", bit2cap (PAGE_SHIFT));
	printf ("Pages / Block:\t\t%d pages\n", N_PAGES_PER_BLOCK);
    printf ("Block size:\t\t%sB\n", bit2cap (PAGES_PER_BLOCK_SHIFT + PAGE_SHIFT));
	printf ("OP ratio:\t\t%d%%\n", OP_RATIO);
	printf ("Physical Blocks:\t%s (%d)\n", bit2cap(BLOCKS_SHIFT), N_BLOCKS);
    printf ("User Blocks:\t\t%d\n", N_USER_BLOCKS);
	printf ("OP Blocks:\t\t%d\n", N_OP_BLOCKS);
	printf ("PPNs:\t\t\t%s (%d)\n", bit2cap (PPN_SHIFT), N_PPNS);
	printf ("LPNs:\t\t\t%d\n", N_LPNS);
    printf ("Total runs:\t\tx%d\n", N_RUNS);
	printf ("Actual capacity:\t%ld Bytes\n\n", (long) N_LPNS * N_PAGE_SIZE);
	printf ("Workload:\t\tRandom\n");
	printf ("FTL:\t\t\tGREEDY\n");
	//printf ("FTL:\t\t\tCB\n");
	//printf ("FTL:\t\t\tCAT\n");
	printf ("Program start time:\t\t%ld\n",start_time);
}

void show_stat (void)
{
	printf ("\nResults ------\n");
	printf ("Host writes:\t\t%ld\n", s.host_write);
	printf ("GC writes:\t\t%ld\n", s.gc_write);
	printf ("Number of GCs:\t\t%d\n", s.gc);
	//printf ("Valid pages per GC:\t%.2f pages\n", (double) s.gc_write / (double) s.gc);
	printf ("WAF:\t\t\t%.2f\n", (double) (s.host_write + s.gc_write) / (double) s.host_write);
	printf ("Finish time:\t\t\t%ld\n",end_time);
}


void init (void)
{
	ppn=0;
    s.gc = 0;
    s.host_write = 0;
    s.gc_write = 0;
	s.free_blk = N_BLOCKS;
	nand_init (N_BLOCKS, N_PAGES_PER_BLOCK);
	l2p_map(N_LPNS);
	srand (0);

}

long get_lpn ()
{
	
	return (rand() << 15 | rand()) % N_LPNS;	//32k
	
}

void write_lpn (unsigned long lpn)
{
	if((lpn>=0&&(lpn<N_LPNS))){
		ftl_write(lpn);
	}
	
}


void sim()
{
	
	while (s.host_write < LPN_COUNTS)
	{
		lpn = get_lpn ();
		write_lpn (lpn);
		s.host_write++;
		/*
		if (s.host_write % N_LPNS == 0)
			printf ("[Run %d] host %ld, valid page copy %ld, GC# %d, WAF=%.2f\n",
					(int) s.host_write / N_LPNS, 
                    s.host_write, s.gc_write, s.gc, 
					(double) (s.host_write + s.gc_write) / (double) s.host_write);
		*/
	}	
}

int main (void)
{
	start_time=clock();
	init ();
	show_info ();
	sim ();
	end_time=clock();
	show_stat ();

	return 0;
}


