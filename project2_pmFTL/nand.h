#define SSD_SHIFT	          	32 //32 4G
#define PAGE_SHIFT	           	12 //2 SHIFT 12 4K
#define PAGES_PER_BLOCK_SHIFT	7 //PAGES_PRR_BLOCK 2 SHIFT 7=128
#define OP_RATIO				7
#define N_RUNS                  10 //10 requests

#define PPN_SHIFT				(SSD_SHIFT - PAGE_SHIFT) //20 1M
#define BLOCKS_SHIFT			(PPN_SHIFT - PAGES_PER_BLOCK_SHIFT)//13 8K

#define N_PAGE_SIZE				(1 << PAGE_SHIFT)//4096 4K
#define N_PAGES_PER_BLOCK		(1 << PAGES_PER_BLOCK_SHIFT) //128
#define N_PPNS					(1 << PPN_SHIFT) //1M 1,048,576
#define N_BLOCKS				(1 << BLOCKS_SHIFT) // 8K 8192
#define N_USER_BLOCKS           (N_BLOCKS * 100 / (100 + OP_RATIO))//7656
#define N_OP_BLOCKS				(N_BLOCKS - N_USER_BLOCKS)//536
#define N_LPNS					(N_USER_BLOCKS * N_PAGES_PER_BLOCK)//7656*128
#define LPN_COUNTS				(N_LPNS * N_RUNS)

unsigned long lpn;
unsigned long ppn;
unsigned long written_ppn;

struct nand_page
{
	volatile unsigned long 		data;
	volatile unsigned long		spare;
	enum page_state{EMPTY, VALID, INVALID}ps;
}Flash[N_PPNS];

unsigned long l2p[N_LPNS];


struct nand_blk
{
	int free_blk;
	int valid_page;
	enum block_state{FREE, ACTIVE, INACTIVE}bs;
}blk_info,Blk[N_BLOCKS];



struct pm_stat {
	int gc;
	long host_write;
	long gc_write;
} s; 

int nand_init (int nblks, int npages);

