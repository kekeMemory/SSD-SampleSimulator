unsigned long read_data;

int ftl_open(unsigned long lpn);
int ftl_read(unsigned long lpn);
int ftl_write(unsigned long lpn);
int ftl_erase (int blk);
int GreedyGC(int nblks,int npages);

