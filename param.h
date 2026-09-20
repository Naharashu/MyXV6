#define NPROC        128  // maximum number of processes
#define KSTACKSIZE 8192  // size of per-process kernel stack
#define NCPU          16  // maximum number of CPUs
#define NOFILE       128  // open files per process
#define NFILE       200  // open files per system
#define NINODE       100  // maximum number of active i-nodes
#define NDEV         20  // maximum major device number
#define ROOTDEV       1  // device number of file system root disk
#define MAXARG       32  // max exec arguments
#define MAXOPBLOCKS  18  // max # of blocks any FS op writes
#define LOGSIZE      (MAXOPBLOCKS*3)  // max data blocks in on-disk log
#define NBUF         (MAXOPBLOCKS*3)  // size of disk block cache
#define FSSIZE       100000  // size of file system in blocks

