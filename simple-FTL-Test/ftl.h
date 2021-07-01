unsigned long read_data;

int l2p_map(unsigned long lpn);
int ftl_read(unsigned long lpn);
int ftl_write(unsigned long lpn);
int ftl_erase (int blk);
int GreedyGC(int nblks,int npages);
int CBGC(int nblks,int npages);
int CatGC(int nblks,int npages);


