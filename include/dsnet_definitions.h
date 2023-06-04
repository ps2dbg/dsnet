
#ifndef DSNET_VERSION_STRING
#define DSNET_VERSION_STRING "1.23.1"
#endif

#ifndef DSNET_STAMP_STRING
#define DSNET_STAMP_STRING "Version " DSNET_VERSION_STRING " " __DATE__ " " __TIME__
#endif

#include <string.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <stdint.h>
#include <fcntl.h>
#include <signal.h>
#include <setjmp.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdio.h>
#include <signal.h>
#include <stddef.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

#ifdef _WIN32
#include <winsock.h>
#else
#include <sys/select.h>
#include <termios.h>
#include <netdb.h>
#include <pwd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#endif

#define qmemcpy memcpy

#ifndef __cdecl
#define __cdecl
#endif

#define __int8 char
#define __int16 short
#define __int32 int
#ifndef _WIN32
typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint64_t QWORD;
#endif
typedef uint8_t _BYTE;
typedef uint16_t _WORD;
typedef uint32_t _DWORD;
typedef uint64_t _QWORD;

#define LAST_IND(x,part_type)    (sizeof(x)/sizeof(part_type) - 1)
#define HIGH_IND(x,part_type)  LAST_IND(x,part_type)
#define LOW_IND(x,part_type)   0

#define BYTEn(x, n)   (*((_BYTE*)&(x)+n))
#define WORDn(x, n)   (*((_WORD*)&(x)+n))
#define DWORDn(x, n)  (*((_DWORD*)&(x)+n))

#ifdef LOBYTE
#undef LOBYTE
#endif
#define LOBYTE(x)  BYTEn(x,LOW_IND(x,_BYTE))
#ifdef LOWORD
#undef LOWORD
#endif
#define LOWORD(x)  WORDn(x,LOW_IND(x,_WORD))
#define LODWORD(x) DWORDn(x,LOW_IND(x,_DWORD))
#ifdef HIBYTE
#undef HIBYTE
#endif
#define HIBYTE(x)  BYTEn(x,HIGH_IND(x,_BYTE))
#ifdef HIWORD
#undef HIWORD
#endif
#define HIWORD(x)  WORDn(x,HIGH_IND(x,_WORD))
#define HIDWORD(x) DWORDn(x,HIGH_IND(x,_DWORD))
#define BYTE1(x)   BYTEn(x,  1)
#define BYTE2(x)   BYTEn(x,  2)
#define BYTE3(x)   BYTEn(x,  3)
#define BYTE4(x)   BYTEn(x,  4)
#define BYTE5(x)   BYTEn(x,  5)
#define BYTE6(x)   BYTEn(x,  6)
#define BYTE7(x)   BYTEn(x,  7)
#define BYTE8(x)   BYTEn(x,  8)
#define BYTE9(x)   BYTEn(x,  9)
#define BYTE10(x)  BYTEn(x, 10)
#define BYTE11(x)  BYTEn(x, 11)
#define BYTE12(x)  BYTEn(x, 12)
#define BYTE13(x)  BYTEn(x, 13)
#define BYTE14(x)  BYTEn(x, 14)
#define BYTE15(x)  BYTEn(x, 15)
#define WORD1(x)   WORDn(x,  1)
#define WORD2(x)   WORDn(x,  2)         // third word of the object, unsigned
#define WORD3(x)   WORDn(x,  3)
#define WORD4(x)   WORDn(x,  4)
#define WORD5(x)   WORDn(x,  5)
#define WORD6(x)   WORDn(x,  6)
#define WORD7(x)   WORDn(x,  7)


// now signed macros (the same but with sign extension)
#define SBYTEn(x, n)   (*((int8_t*)&(x)+n))
#define SWORDn(x, n)   (*((int16_t*)&(x)+n))
#define SDWORDn(x, n)  (*((int32_t*)&(x)+n))

#define SLOBYTE(x)  SBYTEn(x,LOW_IND(x,int8_t))
#define SLOWORD(x)  SWORDn(x,LOW_IND(x,int16_t))
#define SLODWORD(x) SDWORDn(x,LOW_IND(x,int32_t))
#define SHIBYTE(x)  SBYTEn(x,HIGH_IND(x,int8_t))
#define SHIWORD(x)  SWORDn(x,HIGH_IND(x,int16_t))
#define SHIDWORD(x) SDWORDn(x,HIGH_IND(x,int32_t))
#define SBYTE1(x)   SBYTEn(x,  1)
#define SBYTE2(x)   SBYTEn(x,  2)
#define SBYTE3(x)   SBYTEn(x,  3)
#define SBYTE4(x)   SBYTEn(x,  4)
#define SBYTE5(x)   SBYTEn(x,  5)
#define SBYTE6(x)   SBYTEn(x,  6)
#define SBYTE7(x)   SBYTEn(x,  7)
#define SBYTE8(x)   SBYTEn(x,  8)
#define SBYTE9(x)   SBYTEn(x,  9)
#define SBYTE10(x)  SBYTEn(x, 10)
#define SBYTE11(x)  SBYTEn(x, 11)
#define SBYTE12(x)  SBYTEn(x, 12)
#define SBYTE13(x)  SBYTEn(x, 13)
#define SBYTE14(x)  SBYTEn(x, 14)
#define SBYTE15(x)  SBYTEn(x, 15)
#define SWORD1(x)   SWORDn(x,  1)
#define SWORD2(x)   SWORDn(x,  2)
#define SWORD3(x)   SWORDn(x,  3)
#define SWORD4(x)   SWORDn(x,  4)
#define SWORD5(x)   SWORDn(x,  5)
#define SWORD6(x)   SWORDn(x,  6)
#define SWORD7(x)   SWORDn(x,  7)

#define __SPAIR16__(high, low)  (((int16_t)  (high) <<  8) | (uint8_t) (low))
#define __SPAIR32__(high, low)  (((int32_t)  (high) << 16) | (uint16_t)(low))
#define __SPAIR64__(high, low)  (((int64_t)  (high) << 32) | (uint32_t)(low))
#define __PAIR16__(high, low)   (((uint16_t) (high) <<  8) | (uint8_t) (low))
#define __PAIR32__(high, low)   (((uint32_t) (high) << 16) | (uint16_t)(low))
#define __PAIR64__(high, low)   (((uint64_t) (high) << 32) | (uint32_t)(low))

#ifndef CONTAINING_RECORD
#define CONTAINING_RECORD(address,type,field) ((type *)((char *)(address) - (uintptr_t)(&((type *)0)->field)))
#endif


typedef unsigned char byte;
typedef short unsigned int half;
typedef unsigned int word;
typedef struct {unsigned int xa[2];} twin;
typedef struct {unsigned int xa[4];} quad;
typedef struct {short unsigned int length; short unsigned int reserved; short unsigned int protocol; unsigned char source; unsigned char destination;} DECI2_HDR;
typedef struct {unsigned char type; unsigned char code; short unsigned int unused;} DCMP_HDR;
typedef struct {unsigned char result; unsigned char unused[3];} DCMP_CONNECT_DATA;
typedef struct {unsigned int param[4];} DCMP_BOOTP;
typedef struct {short unsigned int id; short unsigned int seq;} DCMP_ECHO_DATA;
typedef struct {short unsigned int proto;} DCMP_STATUS_DATA;
typedef struct {DECI2_HDR orig_hdr; unsigned char orig_data[16];} DCMP_ERROR_DATA;
typedef enum {DRFP_CODE_OPEN = 0, DRFP_CODE_OPENR = 1, DRFP_CODE_CLOSE = 2, DRFP_CODE_CLOSER = 3, DRFP_CODE_READ = 4, DRFP_CODE_READR = 5, DRFP_CODE_WRITE = 6, DRFP_CODE_WRITER = 7, DRFP_CODE_SEEK = 8, DRFP_CODE_SEEKR = 9, DRFP_CODE_IOCTL = 10, DRFP_CODE_IOCTLR = 11, DRFP_CODE_REMOVE = 12, DRFP_CODE_REMOVER = 13, DRFP_CODE_MKDIR = 14, DRFP_CODE_MKDIRR = 15, DRFP_CODE_RMDIR = 16, DRFP_CODE_RMDIRR = 17, DRFP_CODE_DOPEN = 18, DRFP_CODE_DOPENR = 19, DRFP_CODE_DCLOSE = 20, DRFP_CODE_DCLOSER = 21, DRFP_CODE_DREAD = 22, DRFP_CODE_DREADR = 23, DRFP_CODE_GETSTAT = 24, DRFP_CODE_GETSTATR = 25, DRFP_CODE_CHSTAT = 26, DRFP_CODE_CHSTATR = 27, DRFP_CODE_FORMAT = 28, DRFP_CODE_FORMATR = 29, DRFP_CODE_RENAME = 30, DRFP_CODE_RENAMER = 31, DRFP_CODE_CHDIR = 32, DRFP_CODE_CHDIRR = 33, DRFP_CODE_SYNC = 34, DRFP_CODE_SYNCR = 35, DRFP_CODE_MOUNT = 36, DRFP_CODE_MOUNTR = 37, DRFP_CODE_UMOUNT = 38, DRFP_CODE_UMOUNTR = 39, DRFP_CODE_SEEK64 = 40, DRFP_CODE_SEEK64R = 41, DRFP_CODE_DEVCTL = 42, DRFP_CODE_DEVCTLR = 43, DRFP_CODE_SYMLINK = 44, DRFP_CODE_SYMLINKR = 45, DRFP_CODE_READLINK = 46, DRFP_CODE_READLINKR = 47, DRFP_CODE_IOCTL2 = 48, DRFP_CODE_IOCTL2R = 49} DECIFILE_CMD;
struct sce_stat {unsigned int mode; unsigned int attr; unsigned int size; unsigned char ctime[8]; unsigned char atime[8]; unsigned char mtime[8]; unsigned int hisize; unsigned int private_[6];};
struct sce_dirent {struct sce_stat d_stat; unsigned char d_name[256];};
typedef struct {unsigned char code; unsigned char result;} NETMP_HDR;
typedef struct {short unsigned int reserved; unsigned char param[32];} NETMP_BOOTP;
typedef struct {unsigned char pri; unsigned char reserved; short unsigned int proto;} NETMP_PROTOS;
typedef struct {NETMP_PROTOS protos; unsigned int connect_time;} NETMP_STATUS_DATA;
typedef struct {unsigned char id; unsigned char group; unsigned char type; unsigned char code; unsigned char result; unsigned char count; short unsigned int unused;} DBGP_HDR;
typedef struct {unsigned int major_ver; unsigned int minor_ver; unsigned int target_id; unsigned int reserved1; unsigned int mem_align; unsigned int reserved2; unsigned int reg_size; unsigned int nreg; unsigned int nbrkpt; unsigned int ncont; unsigned int nstep; unsigned int nnext; struct __anon_struct_19 {unsigned int mem_limit_align; unsigned int mem_limit_size;} v1; struct __anon_struct_20 {unsigned int run_stop_state; unsigned int hdbg_area_addr; unsigned int hdbg_area_size;} v3;} DBGP_CONF_DATA;
typedef struct {unsigned char space; unsigned char align; short unsigned int reserved; unsigned int address; unsigned int length;} DBGP_MEM;
typedef struct {unsigned char kind; unsigned char number; short unsigned int reserved;} DBGP_REG;
typedef struct {unsigned int adr; unsigned int cnt;} DBGP_BRKPT_DATA;
typedef struct {unsigned int entry; unsigned int gp; unsigned int reserved1; unsigned int reserved2; unsigned int argc;} DBGP_EERUN;
typedef struct {unsigned int flag; unsigned int off; unsigned int cnt;} DBGP_XGKT_CTL;
typedef struct {unsigned int flag; unsigned int length; unsigned int offset; unsigned int sequence;} DBGP_XGKT_DATA;
typedef struct {short unsigned int reserved; short unsigned int sbp; short unsigned int sbw; short unsigned int spsm; short unsigned int x; short unsigned int y; short unsigned int w; short unsigned int h;} DBGP_RDIMG;
typedef struct {unsigned int length; unsigned int sequence; unsigned int reserved[2];} DBGP_RDIMG_DATA;
typedef struct {unsigned int type; unsigned int number; unsigned int mask; unsigned int func; unsigned int arg;} DBGP_EE_HANDLER_DATA;
typedef struct {unsigned int count;} DBGP_EE_THREADLIST_HDR;
typedef struct {unsigned int id; unsigned int priority; unsigned int status; unsigned int cause; unsigned int waitid; unsigned int wakeupcount; unsigned int counts; unsigned int pc; unsigned int sp; unsigned int func; unsigned int ra; unsigned int reserved[2];} DBGP_EE_THREADLIST_DATA;
typedef struct {unsigned int id;} DBGP_EE_THREADID_DATA;
typedef struct {unsigned int count;} DBGP_EE_THREADTCB_HDR;
typedef struct {unsigned int id; unsigned int currentpriority; unsigned int status; unsigned int cause; unsigned int waitid; unsigned int wakeupcount; unsigned int counts; unsigned int programcounter; unsigned int stackpointer; unsigned int func; unsigned int args; unsigned int argc; unsigned int stack; unsigned int stacksize; unsigned int endofheap; unsigned int option; unsigned int gpReg; unsigned int initpriority; unsigned int sa; unsigned int fcr31; quad gpr[32]; twin hi; twin lo; twin hi1; twin lo1; unsigned int fpr[32]; unsigned int facc; unsigned int reserved[3];} DBGP_EE_THREADTCB_DATA;
typedef struct {unsigned int id;} DBGP_EE_SEMAID_DATA;
typedef struct {unsigned int count;} DBGP_EE_SEMABLOCK_HDR;
typedef struct {unsigned int id; unsigned int count; unsigned int maxcount; unsigned int attr; unsigned int option; unsigned int numWaitThreads;} DBGP_EE_SEMABLOCK_DATA;
typedef struct {unsigned char ident[16]; short unsigned int type; short unsigned int machine; unsigned int version; unsigned int entry; unsigned int phoff; unsigned int shoff; unsigned int flags; short unsigned int ehsize; short unsigned int phentsize; short unsigned int phnum; short unsigned int shentsize; short unsigned int shnum; short unsigned int shstrndx;} DS_ELF_EHDR;
typedef struct {unsigned int type; unsigned int offset; unsigned int vaddr; unsigned int paddr; unsigned int filesz; unsigned int memsz; unsigned int flags; unsigned int align;} DS_ELF_PHDR;
typedef struct {unsigned int name; unsigned int type; unsigned int flags; unsigned int addr; unsigned int offset; unsigned int size; unsigned int link; unsigned int info; unsigned int addralign; unsigned int entsize;} DS_ELF_SHDR;
typedef struct {unsigned int gprmask; unsigned int cprmask[4]; unsigned int gp_value;} DS_ELF_REGINFO;
typedef struct {unsigned int name; unsigned int value; unsigned int size; unsigned char info; unsigned char other; short unsigned int shndx;} DS_ELF_SYMTAB;
typedef struct {unsigned int moduleinfo; unsigned int entry; unsigned int gp_value; unsigned int text_size; unsigned int data_size; unsigned int bss_size; short unsigned int moduleversion; char modulename[];} IOPMOD;
typedef struct {unsigned int moduleinfo; unsigned int entry; unsigned int gp_value; unsigned int text_size; unsigned int data_size; unsigned int bss_size; unsigned int erx_lib_addr; unsigned int erx_lib_size; unsigned int erx_stub_addr; unsigned int erx_stub_size; short unsigned int moduleversion; char modulename[];} EEMOD;
typedef struct {unsigned char cmd; unsigned char action; unsigned char result; unsigned char stamp; unsigned int module_id;} ILOADP_HDR;
typedef struct {short unsigned int version; short unsigned int flags; unsigned int mod_addr; unsigned int text_size; unsigned int data_size; unsigned int bss_size; unsigned int entry_address; unsigned int gp_value; unsigned char extnumwords; unsigned char exttype; short unsigned int reserved; unsigned int extword[];} ILOADP_MODINFO_DATA;
typedef struct {unsigned int ret_value;} ILOADP_REPORT_DATA;
typedef struct {unsigned char cmd; unsigned char action; unsigned char result; unsigned char stamp; unsigned int module_id;} ELOADP_HDR;
typedef struct {short unsigned int version; short unsigned int flags; unsigned int mod_addr; unsigned int text_size; unsigned int data_size; unsigned int bss_size; unsigned int entry_address; unsigned int gp_value; unsigned char extnumwords; unsigned char exttype; short unsigned int reserved; unsigned int extword[];} ELOADP_MODINFO_DATA;
typedef struct {unsigned int ret_value;} ELOADP_REPORT_DATA;
typedef unsigned int PAMP_HDR;
typedef struct {unsigned int a; unsigned int b; unsigned int c; unsigned int d;} TRIG_SELECT;
typedef struct {unsigned int address; unsigned int addressmask; unsigned int hidata; unsigned int lodata; unsigned int hidatamask; unsigned int lodatamask;} TRIG_REG;
typedef struct {unsigned char GINT; unsigned char GRDYGC; unsigned char GVLDGC; unsigned char GRDYCG; unsigned char GVLDCG; unsigned char GRW; unsigned char GACK; unsigned char GREQ; unsigned char GA; unsigned char VBLANK; unsigned int GDL; unsigned int GDH;} gifdata_t;
typedef struct {unsigned char Ap0a; unsigned char Ap0b; unsigned char Ap1a; unsigned char Ap1b; unsigned char Aja; unsigned char Ajb; unsigned char Atpce; unsigned char Atpc0; unsigned char Atpc1; unsigned char Atpc2; unsigned char Atpc3; unsigned char Atrig; unsigned char Ambst0; unsigned char Ambst1; unsigned char Ambst2; unsigned char Ambst3; unsigned char Ambst4; unsigned char Ambst5; unsigned char Ambsnd; unsigned char Av0a; unsigned char Av0b; unsigned char Av1a; unsigned char Av1b; unsigned char Av1xa; unsigned char Av1xb; unsigned char Agifdt0; unsigned char Agifdt1; unsigned char Apgn0; unsigned char Apgn1; unsigned char Apgn2; unsigned char Apgn3; unsigned char Apgn4; unsigned char Asrv; unsigned char Amrd0; unsigned char Amrd1; unsigned char Amwt0; unsigned char Amwt1; unsigned char Apms0; unsigned char Apms1; unsigned char Apwt0; unsigned char Apwt1; unsigned char Apwt2; unsigned char Apwt3; unsigned char Attr0; unsigned char Attr1; unsigned char Actr; unsigned char Ammd0; unsigned char Ammd1; unsigned char Ammd2; unsigned char Adisp; unsigned char Atrd0; unsigned char Atrd1; unsigned char Atrd2; unsigned char Atrd3; unsigned char Atrd4; unsigned char Amrd2; unsigned char Amrd3; unsigned char Amwt2; unsigned char Amwt3; unsigned char Apms2; unsigned char Apms3; unsigned char Apwt4; unsigned char Apwt5; unsigned char Apwt6; unsigned char Apwt7; unsigned char Attr2; unsigned char Attr3; unsigned char Areserve2; unsigned char Areserve3;} eegsdata_t;
typedef struct {unsigned int bid; unsigned int master; unsigned int eegs; unsigned int gif; unsigned int iop;} paver_t;
typedef struct {unsigned int addr; unsigned int amsk; unsigned int hidata; unsigned int lodata; unsigned int hidmsk; unsigned int lodmsk;} gifreg_t;
typedef struct {unsigned int addr; unsigned int spu2core0; unsigned int spu2core1; unsigned int cdrom; unsigned int dev9; unsigned int sif; unsigned int read; unsigned int write; unsigned int enable; unsigned int addrmask; unsigned int data; unsigned int datamask;} iopmem_t;
typedef struct {unsigned int MD; unsigned int AD; unsigned int MRAS; unsigned int MCAS; unsigned int RA; unsigned int _rsv0; unsigned int MWE; unsigned int _rsv1; unsigned int SMWR; unsigned int SMEMAC; unsigned int SSRST; unsigned int WAIT; unsigned int UBE; unsigned int SWR; unsigned int SRD; unsigned int RT; unsigned int DACKEX; unsigned int DACK9; unsigned int DACK8; unsigned int DACK5; unsigned int DACK4; unsigned int SIFTYPE; unsigned int INTEX; unsigned int INT9; unsigned int INT8; unsigned int INT5; unsigned int INT4; unsigned int EXTR; unsigned int CS9C; unsigned int CS9; unsigned int cs8; unsigned int cs5; unsigned int cs4; unsigned int cs1; unsigned int cs2; unsigned int IOIS16; unsigned int SSWR; unsigned int SSAC; unsigned int SRST; unsigned int SINT; unsigned int GINT; unsigned int DACK; unsigned int DREQ0; unsigned int DREQ1; unsigned int BGNT; unsigned int BREQ; unsigned int RDY; unsigned int WRAC; unsigned int RDAC; unsigned int BE0; unsigned int BE1; unsigned int BE2; unsigned int BE3; unsigned int _rsv2; unsigned int HBLK1; unsigned int VBLK1; unsigned int _rsv3; unsigned int _rsv4; unsigned int DMAPIO; unsigned int DIR; unsigned int SIFAC; unsigned int _rsv;} iopfpga_t;
typedef struct {unsigned int MD; unsigned int AD; unsigned int MCAS; unsigned int REFRESH; unsigned int MRAS; unsigned int _rsv0; unsigned int SMWR; unsigned int SMEMAC; unsigned int _rsv1; unsigned int DACK_INTERNAL; unsigned int DACK_DEV9; unsigned int DACK_SPU2_1; unsigned int DACK_CDROM; unsigned int DACK_SPU2_0; unsigned int _rsv2; unsigned int INTEX; unsigned int INT9; unsigned int INT8; unsigned int INT5; unsigned int INT4; unsigned int EXTR; unsigned int _rsv3; unsigned int CS9; unsigned int cs8; unsigned int cs5; unsigned int cs4; unsigned int _rsv4; unsigned int cs2; unsigned int _rsv5; unsigned int SSWR; unsigned int SSAC; unsigned int _rsv6; unsigned int HBLK1; unsigned int VBLK1; unsigned int _rsv7; unsigned int DMACH; unsigned int DMADIR; unsigned int _rsv8; unsigned int TRIG_B; unsigned int TRIG_A; unsigned int _rsv9; unsigned int MTRIG;} iopdata_t;
struct _dsp_buf {struct _dsp_buf *forw; struct _dsp_buf *back; char buf[];};
typedef struct _dsp_buf DSP_BUF;
typedef struct {DSP_BUF *head; DSP_BUF *tail;} DSP_QUE;
struct _ds_recv_func_desc;
struct _ds_desc {struct _ds_desc *forw; struct _ds_desc *back; int id; int type; int f_psnet; void *psnet_priv; int fd; DSP_QUE sque; DECI2_HDR rhdr; char *sptr; char *rptr; int slen; int rlen; DSP_BUF *sbuf; DSP_BUF *rbuf; int64_t sec; int64_t usec; int comport; char *msg; int tty_len; struct __anon_struct_58 {struct _ds_recv_func_desc *head; struct _ds_recv_func_desc *tail;} recv_func_list; int (*accept_func)(struct _ds_desc *desc); NETMP_PROTOS *protos; int nprotos;};
typedef struct _ds_desc DS_DESC;
typedef DSP_BUF * (DS_RECV_FUNC)(DS_DESC *desc, DSP_BUF *db);
struct _ds_recv_func_desc {struct _ds_recv_func_desc *forw; struct _ds_recv_func_desc *back; int proto; int type; int code; DS_RECV_FUNC *func;};
typedef struct _ds_recv_func_desc DS_RECV_FUNC_DESC;
struct _ds_hist {struct _ds_hist *forw; struct _ds_hist *back; int no; char buf[];};
typedef struct _ds_hist DS_HIST;
typedef struct {DS_HIST *head; DS_HIST *tail; DS_HIST *curr; int no; char buf[1024]; char *ptr; char yank[1024];} DS_HISTBUF;
struct _ds_option {struct _ds_option *forw; struct _ds_option *back; char *name; char *str; char *def_str; int type; int val; int def_val;};
typedef void DS_FILE;
struct _GIF_OPT {unsigned char *m_buff1; unsigned char *m_buff2; unsigned char *m_buff3; unsigned char *m_buff4; int m_size1; int m_size2; int m_size3; int m_size4; int m_count1; int m_count2; int m_count3; int m_count4; int m_error1; int m_error2; int m_error3; int m_error4; int m_flag; int m_complete; unsigned int m_sdram;};
struct _SMEM_OPT {int row_addr; int col_addr; int read_row_addr; int read_col_addr; int checkmode; int checkEnable; int checkdata; int m_complete;};
struct VERIFY_SSS {char *diagname; int dev; int bus_type; int type; int step; int status; int sta_int; int sta_ube; int sta_wait; int cnt_case[99]; int cnt; int cnt_rd; int cnt_wr; int cnt_dmar; int cnt_dmaw; int cnt_int; int cnt_ube; int cnt_wait; int flg_wr; int flg_rd; int flg_dmaw; int flg_dmar; int flg_int; int flg_ube; int flg_wait; unsigned int ref_ROW; unsigned int ref_COL; unsigned int ref_WR; unsigned int bef_ROW; unsigned int bef_COL; unsigned char COL_flag0[256]; unsigned char COL_flag1[4]; unsigned char ROW_flag[2]; unsigned int okCount; unsigned int ngCount;};
typedef struct VERIFY_SSS VERIFY_SSS;
typedef VERIFY_SSS *PVERIFY_SSS;
struct VERIFY_PFM {int status; long unsigned int cnt;};
typedef struct VERIFY_PFM VERIFY_PFM;
typedef VERIFY_PFM *PVERIFY_PFM;
typedef struct {char *name; char *sname; int value;} STRS;
typedef struct {char *def_fmt; char *ds_fmt; STRS *strs; int nent;} FMT_STRS;
typedef struct {DS_DESC *head; DS_DESC *tail;} DS_DESC_LIST;

struct log_data {struct log_data *forw; struct log_data *back; struct timeval tval; char *msg; int id; int nlen; int dlen;};
typedef struct log_data LOG_DATA;
struct mem {struct mem *forw; struct mem *back; char *file; char *func; int size; int magic;};
typedef struct mem MEM;
typedef struct {MEM *head; MEM *tail;} MEM_LIST;
typedef struct {char buf[256]; int put; int get; int len;} CQ;
typedef struct {short unsigned int mag; short unsigned int len; unsigned int ctg; short unsigned int pri; short unsigned int rep; unsigned char rtag; unsigned char atag; unsigned char acode; unsigned char crsv[5]; short unsigned int cid; short unsigned int seq; unsigned int req; unsigned int sum;} DS_DECI1;
typedef struct {int cid; int tseq; int gseq; char *snd_buf; char *snd_ptr; int snd_len; int snd_dslen; DS_DECI1 rcv_hdr; char *rcv_buf; char *rcv_ptr; int rcv_len; char *read_ptr; int read_len;} DS_PSNETD_PRIV;
struct cmd_entry {struct cmd_entry *forw; struct cmd_entry *back; char *name; char *arg; char *help; int (*func)(int ac, char **av); int f_enable;};
typedef struct cmd_entry CMD_ENTRY;
struct show_arg {struct show_arg *forw; struct show_arg *back; char *name; int (*func)(int ac, char **av);};
typedef struct show_arg SHOW_ARG;
struct help_arg {struct help_arg *forw; struct help_arg *back; char *name; int (*func)(char *name);};
typedef struct help_arg HELP_ARG;
struct alias {struct alias *forw; struct alias *back; int used; char *name; char *value;};
typedef struct alias ALIAS;
typedef struct _ds_option DS_OPTION;
typedef struct {unsigned int bits[64];} CLASS;
struct re {struct re *next; struct re *sub; int type; int val; CLASS *class_;};
typedef struct re RE;
struct rerp {struct rerp *forw; struct rerp *back; RE *re; char *re_str; char *rp;};
typedef struct rerp RERP;
struct lconv {char *decimal_point; char *thousands_sep; char *grouping; char *int_curr_symbol; char *currency_symbol; char *mon_decimal_point; char *mon_thousands_sep; char *mon_grouping; char *positive_sign; char *negative_sign; char int_frac_digits; char frac_digits; char p_cs_precedes; char p_sep_by_space; char n_cs_precedes; char n_sep_by_space; char p_sign_posn; char n_sign_posn;};
typedef struct {char *fmt; unsigned int mask; unsigned int code; unsigned int isa;} OPCODE;
typedef struct {unsigned int pc; unsigned int sp; unsigned int ra;} BT_REG;
typedef struct {int put; int get; int len; char buf[16384];} TTYQ;
typedef struct {int kind; int number; int size; char *name;} EE_REGS;
struct brkpt {struct brkpt *forw; struct brkpt *back; int flag; int no; int init_cnt; int curr_cnt; unsigned int adr; unsigned int ins;};
typedef struct brkpt BRKPT;
struct bt {struct bt *forw; struct bt *back; int no; unsigned int adr; unsigned int sp_lo; unsigned int sp_hi;};
typedef struct bt BT;
struct syms {struct syms *forw; struct syms *back; int id; int base; DS_ELF_SYMTAB *symtab; char *shstrtab; char *strtab; int shnum; int nsymtab; DS_ELF_SHDR *shdr;};
typedef struct syms SYMS;
struct mod_syms {struct mod_syms *forw; struct mod_syms *back; unsigned int adr; unsigned int siz; unsigned int id; char name[];};
typedef struct mod_syms MOD_SYMS;
struct symline {struct symline *forw; struct symline *back; unsigned int value;};
typedef struct symline SYMLINE;
struct imod;
typedef struct imod IMOD;
typedef struct {char *fmt; unsigned int mask; unsigned int code;} VOPCODE;
struct help_map {struct help_map *forw; struct help_map *back; int ej; int level; char *key; char **pl;};
typedef struct help_map HELP_MAP;
typedef struct {short unsigned int magic; short unsigned int vstamp; unsigned int ilineMax; unsigned int cbLine; unsigned int cblineOffset; unsigned int idnMax; unsigned int cbDnOffset; unsigned int ipdMax; unsigned int cbPdOffset; unsigned int isymMax; unsigned int cbSymOffset; unsigned int ioptMax; unsigned int cbOptOffset; unsigned int iauxMax; unsigned int cbAuxOffset; unsigned int issMax; unsigned int cbSsOffset; unsigned int issExtMax; unsigned int cbSsExtOffset; unsigned int ifdMax; unsigned int cbFdOffset; unsigned int crfd; unsigned int cbRfdOffset; unsigned int iextMax; unsigned int cbExtOffset;} SYM_HDR;
typedef struct {unsigned int iss; unsigned int value; unsigned int st_sc_index;} SYM;
typedef struct {short unsigned int reserved; short unsigned int ifd; SYM sym;} EXT_SYM;
typedef struct {unsigned int adr; unsigned int rss; unsigned int issBase; unsigned int cbSs; unsigned int isymBase; unsigned int csym; unsigned int ilineBase; unsigned int cline; unsigned int ioptBase; unsigned int copt; short unsigned int ipdFirst; short unsigned int cpd; unsigned int iauxBase; unsigned int caux; unsigned int rfdBase; unsigned int crfd; unsigned int misc; unsigned int cbLineOffset; unsigned int cbLine;} FDT;
typedef struct {unsigned int adr; unsigned int isym; unsigned int iline; unsigned int regmask; unsigned int regoffset; unsigned int iopt; unsigned int fregmask; unsigned int fregoffset; unsigned int frameoffset; short unsigned int framereg; short unsigned int pcreg; unsigned int lnLow; unsigned int lnHigh; unsigned int cbLineOffset;} PDT;
typedef struct {unsigned int sadr; unsigned int eadr;} ADRS;
struct mdebug {struct mdebug *forw; struct mdebug *back; char *path; int id; int base; SYM_HDR *shdr; unsigned char *lins; unsigned char *line; PDT *pdts; PDT *pdte; SYM *lsyms; SYM *lsyme; char *lstrs; char *lstre; EXT_SYM *esyms; EXT_SYM *esyme; char *estrs; char *estre; FDT *fdts; FDT *fdte; ADRS *fdt_adrs; ADRS *pdt_adrs;};
typedef struct mdebug MDEBUG;
struct handle {int fd; DIR *dir; int ino; char *name;};
struct seq_time {struct seq_time *forw; struct seq_time *back; int seq; int64_t s; int64_t u;};
typedef struct seq_time SEQ_TIME;
typedef struct {int off; int bit; char *name;} IOP_REGS;

struct st_proto {struct st_proto *forw; struct st_proto *back; DS_DESC *desc; short unsigned int proto; unsigned char nid;};
typedef struct st_proto ST_PROTO;
typedef struct {unsigned int bid; unsigned int master; unsigned int eegs; unsigned int gif; unsigned int iop;} PAMP_VERSION;
struct iop_data {unsigned int addr; unsigned int spu2core0; unsigned int spu2core1; unsigned int cdrom; unsigned int dev9; unsigned int sif; unsigned int read; unsigned int write; unsigned int enable; unsigned int addrmask; unsigned int data; unsigned int datamask;};
typedef struct iop_data TRIG_DATA;
struct id_mi_name {int id; ILOADP_MODINFO_DATA *mi; char name[100];};
typedef struct {char *str; unsigned int val;} trig_st;
typedef struct {char *str; unsigned int putvblank; unsigned int putreg; unsigned int putintc;} gif_st;
typedef struct {char *str; unsigned int putvblank; unsigned int putdata; unsigned int putintc;} iop_st;
typedef struct {int no; int qword;} BUSDATA;
typedef struct {int ddapixel; int pgn; int prim; int read; int read_bit; int sread; int strans; int strans_bit; int write; int write_bit; int miss; int miss_bit; int outpixel; int outpixel_bit; int ddasetup; int ddaidle; int ddawait; int clut; int disp;} gs_pa;

struct xrecv_kbd_dat_struct {unsigned int zero; unsigned __int8 str[3];};
struct xrecv_comport_dat_struct {unsigned int zero; unsigned __int8 str[256];};

struct dscons_send_char_struct {unsigned int zero; unsigned char str[1];};

#ifdef TARGET_EE
typedef quad regtype;
typedef struct {unsigned int cause; unsigned int epc; unsigned int status; unsigned int errorepc;} SCRS;
struct imod {struct imod *forw; struct imod *back; int flags; unsigned int id; ILOADP_MODINFO_DATA info; unsigned int erx_stub_addr; unsigned int erx_stub_size; unsigned int erx_lib_addr; unsigned int erx_lib_size; char *name; unsigned char status; unsigned int retval; int arg_siz; char args[160]; unsigned int vaddr;};
#else
typedef unsigned int regtype;
typedef struct {unsigned int cause; unsigned int epc;} SCRS;
struct imod {struct imod *forw; struct imod *back; int flags; unsigned int id; ILOADP_MODINFO_DATA info; char *name; unsigned char status; unsigned int retval; int arg_siz; char args[160]; unsigned int vaddr;};
#endif
