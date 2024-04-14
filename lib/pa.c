#include "dsnet_prototypes.h"

static unsigned int *screg;
static unsigned int *prgn2;
static unsigned int *plcrs;

static int ispa_32 = 0;

static unsigned int sam_fd = 4294967295u;
static unsigned int mem_fd = 4294967295u;
static char *pmem = NULL;

static void wcopy(unsigned int *dst, unsigned int *src, unsigned int size);
static int IsLevel0(gifdata_t *p);
static void ConvertGIF(gifdata_t *p);
static int IsPA();
static int IsSupportDMA();
static int IsSupportEEGS();
static int IsSupportGIF();
static int IsSupportIOP();
static int IsNotEEGSSampling();
static int IsSampling();
static unsigned int GetPLXRegister(unsigned int sam_fd);
static void dma_copy(unsigned int baddr, unsigned int offset, unsigned int size);
static DSP_BUF *ds_send_pamp(
        DS_DESC *desc,
        DSP_BUF *rdb,
        DECI2_HDR *dh,
        unsigned int code,
        unsigned int result,
        void *ptr,
        int len);
static void copy_gif_record(unsigned __int8 *dst, unsigned int stop, unsigned int offset, unsigned int count);
static unsigned int GetIOPRas(unsigned int addr);
static unsigned int GetIOPCas(unsigned int addr);
static void SetIOPData(iopdata_t *d, iopfpga_t *s);
static void copy_iop_record(unsigned __int8 *dst, unsigned int stop, unsigned int offset, unsigned int count);
static void copy_rdmem(unsigned __int8 *dst, unsigned int stop, unsigned int offset, unsigned int count);
static void copy_eegs_record(unsigned __int8 *dst, unsigned int stop, unsigned int offset, unsigned int count);
static DSP_BUF *ds_send_pamp_gif_record(
        DS_DESC *desc,
        DSP_BUF *rdb,
        DECI2_HDR *dh,
        unsigned int code,
        unsigned int result,
        unsigned int stop,
        unsigned int offset,
        int count);
static DSP_BUF *ds_send_pamp_rdmem(
        DS_DESC *desc,
        DSP_BUF *rdb,
        DECI2_HDR *dh,
        unsigned int code,
        unsigned int result,
        unsigned int stop,
        unsigned int offset,
        int count);
static DSP_BUF *ds_send_pamp_iop_rdmem(
        DS_DESC *desc,
        DSP_BUF *rdb,
        DECI2_HDR *dh,
        unsigned int code,
        unsigned int result,
        unsigned int stop,
        unsigned int offset,
        int count);
static DSP_BUF *ds_send_pamp_iop_record(
        DS_DESC *desc,
        DSP_BUF *rdb,
        DECI2_HDR *dh,
        unsigned int code,
        unsigned int result,
        unsigned int stop,
        unsigned int offset,
        int count);
static DSP_BUF *ds_send_pamp_eegs_record(
        DS_DESC *desc,
        DSP_BUF *rdb,
        DECI2_HDR *dh,
        unsigned int code,
        unsigned int result,
        unsigned int stop,
        unsigned int offset,
        int count);
static DSP_BUF *recv_pamp_notpa(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len);
static DSP_BUF *recv_pamp_getversion(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len);
static DSP_BUF *recv_pamp_writereg(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len);
static DSP_BUF *recv_pamp_readreg(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len);
static DSP_BUF *recv_pamp_eegs_record(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len);
static DSP_BUF *recv_pamp_eegs_rdmem(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len);
static DSP_BUF *recv_pamp_gif_record(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len);
static DSP_BUF *recv_pamp_gif_rdmem(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len);
static DSP_BUF *recv_pamp_iop_rdmem(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len);
static DSP_BUF *recv_pamp_iop_record(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len);
static DSP_BUF *recv_pamp_reset(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len);
static DSP_BUF *recv_pamp_master_trigger_a_getselect(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_master_trigger_b_getselect(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_master_trigger_c_getselect(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_master_trigger_d_getselect(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_master_trigger_a_putselect(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_master_trigger_b_putselect(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_master_trigger_c_putselect(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_master_trigger_d_putselect(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_master_getstatus(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len);
static DSP_BUF *recv_pamp_master_trigger_gettrigpos(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_master_trigger_puttrigpos(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_master_sampling_start(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_master_sampling_stop(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len);
static // local variable allocation has failed, the output may be wrong!
DSP_BUF *recv_pamp_iop_trigger_a_getdata(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static // local variable allocation has failed, the output may be wrong!
DSP_BUF *recv_pamp_iop_trigger_b_getdata(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_iop_trigger_a_putdata(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_iop_trigger_b_putdata(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_iop_trigger_getstatus(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_iop_trigger_a_getintc(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_iop_trigger_a_putintc(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_iop_trigger_b_getintc(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_iop_trigger_b_putintc(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_gif_trigger_a_putreg(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len);
static DSP_BUF *recv_pamp_gif_trigger_a_getreg(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len);
static DSP_BUF *recv_pamp_gif_trigger_a_putvblanking(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_gif_trigger_a_getintc(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_gif_trigger_b_getintc(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_gif_trigger_a_putintc(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_gif_trigger_b_putintc(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_gif_trigger_a_getvblanking(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_iop_trigger_a_putvblanking(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_iop_trigger_a_getvblanking(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_iop_trigger_b_putvblanking(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_iop_trigger_b_getvblanking(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_gif_trigger_b_putreg(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len);
static DSP_BUF *recv_pamp_gif_trigger_b_getreg(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len);
static DSP_BUF *recv_pamp_gif_trigger_b_getvblanking(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_gif_trigger_b_putvblanking(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);
static DSP_BUF *recv_pamp_gif_trigger_getstatus(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len);

static void wcopy(unsigned int *dst, unsigned int *src, unsigned int size)
{
  unsigned int *end; // [esp+0h] [ebp-4h]

  end = &src[size >> 2];
  while ( end > src )
    *dst++ = *src++;
}

static int IsLevel0(gifdata_t *p)
{
  int result; // eax

  result = 0;
  if ( (p->GRDYGC & 0x7F) == 1
    || (p->GRDYGC & 0x7F) == 3
    || (p->GRDYGC & 0x7F) == 4
    || (p->GRDYGC & 0x7F) == 5
    || (p->GRDYGC & 0x7F) == 6
    || (p->GRDYGC & 0x7F) == 79 )
  {
    return 1;
  }
  return result;
}

static void ConvertGIF(gifdata_t *p)
{
  char v1; // dl
  char v2; // dl
  char v3; // dl
  char v4; // dl

  if ( (p->GINT & 0x40) != 0 )
  {
    if ( (p->GINT & 0x20) != 0 )
    {
      v1 = 0;
      if ( (p->GINT & 0x10) != 0 && (p->GINT & 4) == 0 && (p->GINT & 2) != 0 )
        v1 = 1;
      p->GINT &= ~0x80u;
      p->GINT |= v1 << 7;
    }
    else
    {
      v2 = 0;
      if ( (p->GINT & 0x10) == 0 && (p->GINT & 4) != 0 && (p->GINT & 8) != 0 )
        v2 = 1;
      p->GINT &= ~0x80u;
      p->GINT |= v2 << 7;
    }
  }
  else if ( (p->GINT & 0x20) != 0 )
  {
    if ( IsLevel0(p) )
    {
      p->GINT &= ~0x80u;
      p->GRDYGC |= 0x7Fu;
      *(_DWORD *)&p->GVLDGC = -1;
      *(_DWORD *)&p->GACK = -1;
    }
    else
    {
      v3 = 0;
      if ( (p->GINT & 0x10) == 0 && (p->GINT & 2) != 0 && (p->GINT & 4) != 0 && (p->GINT & 8) != 0 )
        v3 = 1;
      p->GINT &= ~0x80u;
      p->GINT |= v3 << 7;
    }
  }
  else if ( IsLevel0(p) )
  {
    p->GINT &= ~0x80u;
    p->GRDYGC |= 0x7Fu;
    *(_DWORD *)&p->GVLDGC = -1;
    *(_DWORD *)&p->GACK = -1;
  }
  else
  {
    v4 = 0;
    if ( (p->GINT & 0x10) != 0 && (p->GINT & 2) != 0 && (p->GINT & 4) == 0 && (p->GINT & 8) != 0 )
      v4 = 1;
    p->GINT &= ~0x80u;
    p->GINT |= v4 << 7;
  }
  p->GINT &= ~0x10u;
  p->GINT &= ~2u;
  p->GINT &= ~4u;
  p->GINT &= ~8u;
}

static int IsPA()
{
  int v0; // ebx
  struct stat stbuf; // [esp+4h] [ebp-58h] BYREF

  if ( !ispa_32 )
  {
    v0 = 0;
    if ( stat("/dev/sam0", &stbuf) >= 0 && (stbuf.st_mode & 0xF000) == 0x2000 )
      v0 = 1;
    ispa_32 = v0;
  }
  return ispa_32;
}

static int IsSupportDMA()
{
  return IsPA() && screg[128] > 0x2093010;
}

static int IsSupportEEGS()
{
  return 1;
}

static int IsSupportGIF()
{
  return 1;
}

static int IsSupportIOP()
{
  return 1;
}

static int IsNotEEGSSampling()
{
  return IsSupportEEGS() && !screg[16423];
}

static int IsSampling()
{
  if ( IsSupportEEGS() && (screg[0x4000] & 0x80000000) != 0 )
    return 1;
  if ( IsSupportGIF() && (screg[0x8000] & 0x80000000) != 0 )
    return 1;
  return IsSupportIOP() && (screg[49152] & 0x80000000) != 0;
}

static unsigned int GetPLXRegister(unsigned int sam_fd)
{
  void *p; // [esp+0h] [ebp-8h]
  void *p_1; // [esp+0h] [ebp-8h]
  void *p_2; // [esp+0h] [ebp-8h]
  unsigned int base; // [esp+4h] [ebp-4h] BYREF

  ds_ioctl(sam_fd, 0x53414D05u, &base);
#ifndef _WIN32
  p = mmap(0, 0x1000u, 3, 1, sam_fd, base);
#else
  p = (void *)-1;
#endif
  if ( p != (void *)-1 )
  {
    plcrs = (unsigned int *)((char *)p + (base & 0xFFF));
    *(_DWORD *)((char *)p + (base & 0xFFF) + 4) = 268435457;
    plcrs[61] = 1;
    ds_ioctl(sam_fd, 0x53414D06u, &base);
#ifndef _WIN32
    p_1 = mmap(0, 0x100000u, 3, 1, sam_fd, base);
#else
    p_1 = (void *)-1;
#endif
    if ( p_1 != (void *)-1 )
    {
      screg = (unsigned int *)((char *)p_1 + (base & 0xFFF));
      ds_ioctl(sam_fd, 0x53414D07u, &base);
#ifndef _WIN32
      p_2 = mmap(0, 0x10000000u, 3, 1, sam_fd, base);
#else
      p_2 = (void *)-1;
#endif
      if ( p_2 != (void *)-1 )
      {
        prgn2 = (unsigned int *)((char *)p_2 + (base & 0xFFF));
        mem_fd = open("/dev/mem", 0);
#ifndef _WIN32
        pmem = (char *)mmap(0, 0x200000u, 1, 2, mem_fd, 209715200);
#else
        pmem = (void *)-1;
#endif
        if ( pmem != (char *)-1 )
          return 0;
      }
    }
  }
  ds_printf("mmap() error\n");
  return 5;
}

unsigned int ds_open_sam()
{
  if ( !IsPA() )
    return 6;
  sam_fd = open("/dev/sam0", 2);
  if ( !GetPLXRegister(sam_fd) )
    return 0;
  close(sam_fd);
  close(mem_fd);
  mem_fd = -1;
  sam_fd = -1;
  return 2;
}

void set_dma(unsigned int phyAdr, unsigned int adrs, unsigned int bytes)
{
  unsigned int v3; // edx
  unsigned int v4; // edx

  plcrs[42] = 1;
  plcrs[32] = 451;
  plcrs[33] = phyAdr;
  plcrs[34] = adrs | 0x10000000;
  plcrs[35] = bytes;
  plcrs[36] = 8;
  v3 = plcrs[42];
  LOBYTE(v3) = v3 | 8;
  plcrs[42] = v3;
  v4 = plcrs[42];
  LOBYTE(v4) = v4 | 2;
  plcrs[42] = v4;
}

static void dma_copy(unsigned int baddr, unsigned int offset, unsigned int size)
{
  unsigned int end; // [esp+4h] [ebp-4h]

  end = size + offset;
  screg[16] = 0;
  while ( end > offset )
  {
    set_dma(baddr, offset, 0x1000u);
    while ( (plcrs[42] & 0x10) == 0 )
      ;
    offset += 4096;
    baddr += 4096;
  }
}

unsigned int ds_close_sam(unsigned int sam_fd)
{
  close(sam_fd);
  close(mem_fd);
  mem_fd = -1;
  return 0;
}

static DSP_BUF *ds_send_pamp(
        DS_DESC *desc,
        DSP_BUF *rdb,
        DECI2_HDR *dh,
        unsigned int code,
        unsigned int result,
        void *ptr,
        int len)
{
  DSP_BUF *db; // [esp+0h] [ebp-8h]

  db = ds_alloc_buf(1280, 72, 0, len + 8);
  if ( db )
  {
    *(_DWORD *)&db->buf[8] = code;
    *(_DWORD *)&db->buf[12] = result;
    if ( ptr )
    {
      if ( len > 0 )
        memcpy(&db->buf[16], ptr, len);
    }
    ds_send_desc(desc, db);
    return ds_free_buf(rdb);
  }
  else
  {
    ds_error("no memory");
    return rdb;
  }
}

static void copy_gif_record(unsigned __int8 *dst, unsigned int stop, unsigned int offset, unsigned int count)
{
  unsigned int v4; // eax
  int v5; // ebx
  void *dest; // [esp+Ch] [ebp-A8h]
  void *desta; // [esp+Ch] [ebp-A8h]
  void *destb; // [esp+Ch] [ebp-A8h]
  unsigned int stopaddr; // [esp+18h] [ebp-9Ch]
  unsigned __int8 *end; // [esp+20h] [ebp-94h]
  unsigned __int8 *s; // [esp+24h] [ebp-90h]
  unsigned __int8 *s_1; // [esp+24h] [ebp-90h]
  unsigned __int8 *s_2; // [esp+24h] [ebp-90h]
  unsigned int buf[32]; // [esp+28h] [ebp-8Ch] BYREF
  int mid; // [esp+A8h] [ebp-Ch]
  int post; // [esp+ACh] [ebp-8h]
  int pre; // [esp+B0h] [ebp-4h]
  unsigned int counta; // [esp+C8h] [ebp+14h]

  pre = (3 - offset % 3) % 3;
  post = (offset + count) % 3;
  mid = (count - pre - post) / 3;
  s = (unsigned __int8 *)&prgn2[(stop & 0xFFFFFFE0) >> 2];
  end = (unsigned __int8 *)(prgn2 + 0x4000000);
  if ( IsSupportDMA() )
  {
    v4 = stop;
    LOBYTE(v4) = stop & 0xE0;
    stopaddr = 32 * (offset / 3) + v4;
    v5 = mid;
    if ( pre )
      v5 = mid + 1;
    if ( post )
      dest = (void *)(32 * (v5 + 1));
    else
      dest = (void *)(32 * v5);
    if ( stopaddr > 0xFFFFFFF )
      stopaddr -= 0x10000000;
    if ( (unsigned int)dest + stopaddr <= 0x10000000 )
    {
      dma_copy(0xC800000u, stopaddr, (unsigned int)dest);
    }
    else
    {
      dma_copy(0xC800000u, stopaddr, 0x10000000 - stopaddr);
      dma_copy(478150656 - stopaddr, 0, (unsigned int)dest + stopaddr - 0x10000000);
    }
    s_1 = (unsigned __int8 *)pmem;
    if ( pre == 2 )
    {
      memcpy(dst, pmem + 26, 5u);
      memcpy(dst + 5, s_1, 5u);
      ConvertGIF((gifdata_t *)dst);
      memcpy(dst + 10, s_1 + 16, 0xAu);
      ConvertGIF((gifdata_t *)(dst + 10));
      dst += 20;
      s_1 += 32;
    }
    if ( pre == 1 )
    {
      memcpy(dst, s_1 + 16, 0xAu);
      ConvertGIF((gifdata_t *)dst);
      dst += 10;
      s_1 += 32;
    }
    while ( 1 )
    {
      desta = (void *)mid--;
      if ( (int)desta <= 0 )
        break;
      memcpy(dst, s_1 + 5, 0xAu);
      ConvertGIF((gifdata_t *)dst);
      memcpy(dst + 10, s_1 + 26, 5u);
      memcpy(dst + 15, s_1, 5u);
      ConvertGIF((gifdata_t *)(dst + 10));
      memcpy(dst + 20, s_1 + 16, 0xAu);
      ConvertGIF((gifdata_t *)dst + 1);
      dst += 30;
      s_1 += 32;
    }
    if ( post == 1 )
    {
      memcpy(dst, s_1 + 5, 0xAu);
      ConvertGIF((gifdata_t *)dst);
    }
    if ( post == 2 )
    {
      memcpy(dst, s_1 + 5, 0xAu);
      ConvertGIF((gifdata_t *)dst);
      memcpy(dst + 10, s_1 + 26, 5u);
      memcpy(dst + 15, s_1, 5u);
      ConvertGIF((gifdata_t *)(dst + 10));
    }
  }
  else
  {
    s_2 = &s[32 * (offset / 3)];
    if ( s_2 >= end )
      s_2 -= 0x10000000;
    if ( pre == 2 )
    {
      wcopy(buf, (unsigned int *)s_2, 0x20u);
      memcpy(dst, (char *)&buf[6] + 2, 5u);
      memcpy(dst + 5, buf, 5u);
      ConvertGIF((gifdata_t *)dst);
      counta = count - 1;
      if ( !counta )
        return;
      memcpy(dst + 10, &buf[4], 0xAu);
      ConvertGIF((gifdata_t *)(dst + 10));
      count = counta - 1;
      if ( !count )
        return;
      dst += 20;
      s_2 += 32;
      if ( s_2 >= end )
        s_2 = (unsigned __int8 *)prgn2;
    }
    if ( pre == 1 )
    {
      wcopy(buf, (unsigned int *)s_2, 0x20u);
      memcpy(dst, &buf[4], 0xAu);
      ConvertGIF((gifdata_t *)dst);
      if ( count == 1 )
        return;
      dst += 10;
      s_2 += 32;
      if ( s_2 >= end )
        s_2 = (unsigned __int8 *)prgn2;
    }
    while ( 1 )
    {
      destb = (void *)mid--;
      if ( (int)destb <= 0 )
        break;
      wcopy(buf, (unsigned int *)s_2, 0x20u);
      memcpy(dst, (char *)&buf[1] + 1, 0xAu);
      ConvertGIF((gifdata_t *)dst);
      memcpy(dst + 10, (char *)&buf[6] + 2, 5u);
      memcpy(dst + 15, buf, 5u);
      ConvertGIF((gifdata_t *)(dst + 10));
      memcpy(dst + 20, &buf[4], 0xAu);
      ConvertGIF((gifdata_t *)dst + 1);
      dst += 30;
      s_2 += 32;
      if ( s_2 >= end )
        s_2 = (unsigned __int8 *)prgn2;
    }
    if ( post == 1 )
    {
      wcopy(buf, (unsigned int *)s_2, 0x20u);
      memcpy(dst, (char *)&buf[1] + 1, 0xAu);
      ConvertGIF((gifdata_t *)dst);
    }
    if ( post == 2 )
    {
      wcopy(buf, (unsigned int *)s_2, 0x20u);
      memcpy(dst, (char *)&buf[1] + 1, 0xAu);
      ConvertGIF((gifdata_t *)dst);
      memcpy(dst + 10, (char *)&buf[6] + 2, 5u);
      memcpy(dst + 15, buf, 5u);
      ConvertGIF((gifdata_t *)(dst + 10));
    }
  }
}

unsigned int GetIOPAddr(unsigned int ras, unsigned int cas)
{
  return (4 * ((2 * (_WORD)cas) & 0x200 | (cas >> 1) & 0x100 | (unsigned __int8)cas)) | ((ras & 0x400 | ((_WORD)ras << 9) & 0x200 | (ras >> 1) & 0x1FF) << 12);
}

static unsigned int GetIOPRas(unsigned int addr)
{
  return ((addr & 0x200000) != 0) | (addr >> 12) & 0x400 | (addr >> 11) & 0x3FE;
}

static unsigned int GetIOPCas(unsigned int addr)
{
  return (addr >> 1) & 0x200 | (addr >> 3) & 0x100 | (unsigned __int8)(addr >> 2);
}

static void SetIOPData(iopdata_t *d, iopfpga_t *s)
{
  __int16 v2; // dx
  int v3; // ecx
  unsigned int IOPAddr; // eax
  char v5; // cl

  v2 = ((BYTE1(s->AD) >> 4) & 0xF) << 7;
  HIWORD(d->AD) &= 0xF87Fu;
  HIWORD(d->AD) |= v2;
  v3 = (unsigned __int8)(16 * ((s->AD & 0x800) != 0));
  HIBYTE(d->AD) &= ~0x10u;
  HIBYTE(d->AD) |= v3;
  IOPAddr = GetIOPAddr(HIWORD(s->AD) & 0x7FF, s->AD & 0x7FF);
  d->AD &= 0xFF800000;
  d->AD |= IOPAddr & 0x7FFFFF;
  if ( (BYTE1(s->MRAS) & 0xF8) != 112 || (s->MCAS & 0x300000) != 0 )
  {
    v5 = 16 * ((BYTE2(s->MCAS) >> 4) & 3);
    BYTE2(d->REFRESH) &= 0xCFu;
    BYTE2(d->REFRESH) |= v5;
  }
  else
  {
    BYTE2(d->REFRESH) &= 0xCFu;
    BYTE2(d->REFRESH) |= 0x20u;
  }
  HIBYTE(d->AD) &= ~0x20u;
  LOBYTE(d->MCAS) &= 0xC0u;
  BYTE1(d->MCAS) &= 7u;
  BYTE2(d->MCAS) &= ~0x40u;
  HIBYTE(d->MCAS) &= ~8u;
  HIBYTE(d->MCAS) &= ~0x20u;
  LOWORD(d->REFRESH) = 0;
  BYTE2(d->REFRESH) &= 0xF3u;
  HIWORD(d->REFRESH) &= 0xF07Fu;
  HIBYTE(d->REFRESH) &= ~0x40u;
}

void ConvertIOP(unsigned __int8 *dst, unsigned __int8 *src, int linesize)
{
  char v3; // cl
  char v4; // cl
  iopfpga_t *v7; // [esp+4h] [ebp-4h]
  int linesizea; // [esp+18h] [ebp+10h]

  for ( linesizea = linesize - 16; linesizea > 0; linesizea -= 32 )
  {
    if ( (src[5] & 0xF0) == 0xF0 )
    {
      v7 = (iopfpga_t *)(src + 16);
      *(_DWORD *)dst = v7->MD;
      *((_DWORD *)dst + 1) = v7->AD;
      *((_DWORD *)dst + 2) = v7->MRAS;
      *((_DWORD *)dst + 3) = v7->MCAS;
      if ( (BYTE1(v7->AD) & 0xF0) != 0xF0 && (v7->AD & 0x40000000) == 0 )
      {
        *(_DWORD *)dst = v7->RA;
        v4 = HIBYTE(v7->_rsv0) >> 7 << 7;
        dst[7] &= ~0x80u;
        dst[7] |= v4;
      }
      SetIOPData((iopdata_t *)dst, v7);
    }
    else
    {
      *(_DWORD *)dst = *(_DWORD *)src;
      *((_DWORD *)dst + 1) = *((_DWORD *)src + 1);
      *((_DWORD *)dst + 2) = *((_DWORD *)src + 2);
      *((_DWORD *)dst + 3) = *((_DWORD *)src + 3);
      if ( (src[7] & 0x40) == 0 )
      {
        *(_DWORD *)dst = *((_DWORD *)src + 4);
        v3 = src[23] >> 7 << 7;
        dst[7] &= ~0x80u;
        dst[7] |= v3;
      }
      SetIOPData((iopdata_t *)dst, (iopfpga_t *)src);
      v7 = (iopfpga_t *)(src + 16);
    }
    dst += 16;
    src = (unsigned __int8 *)&v7->RA;
  }
}

static void copy_iop_record(unsigned __int8 *dst, unsigned int stop, unsigned int offset, unsigned int count)
{
  unsigned int v4; // eax
  unsigned int linesize; // [esp+8h] [ebp-8h]
  unsigned int stopaddr; // [esp+Ch] [ebp-4h]

  if ( IsSupportDMA() )
  {
    v4 = stop;
    LOBYTE(v4) = stop & 0xF0;
    stopaddr = 32 * offset + v4;
    linesize = 16 * (2 * count + 1);
    if ( stopaddr > 0xFFFFFFF )
      stopaddr -= 0x10000000;
    if ( linesize + stopaddr <= 0x10000000 )
    {
      dma_copy(0xC800000u, stopaddr, linesize);
    }
    else
    {
      dma_copy(0xC800000u, stopaddr, 0x10000000 - stopaddr);
      dma_copy(478150656 - stopaddr, 0, linesize + stopaddr - 0x10000000);
    }
    ConvertIOP(dst, (unsigned __int8 *)pmem, linesize);
  }
}

static void copy_rdmem(unsigned __int8 *dst, unsigned int stop, unsigned int offset, unsigned int count)
{
  unsigned int stopaddr; // [esp+8h] [ebp-4h]

  if ( IsSupportDMA() )
  {
    stopaddr = offset + stop;
    if ( offset + stop > 0xFFFFFFF )
      stopaddr -= 0x10000000;
    if ( count + stopaddr <= 0x10000000 )
    {
      dma_copy(0xC800000u, stopaddr, count);
    }
    else
    {
      dma_copy(0xC800000u, stopaddr, 0x10000000 - stopaddr);
      dma_copy(478150656 - stopaddr, 0, count + stopaddr - 0x10000000);
    }
    memcpy(dst, pmem, count);
  }
}

static void copy_eegs_record(unsigned __int8 *dst, unsigned int stop, unsigned int offset, unsigned int count)
{
  unsigned int v4; // eax
  int v5; // ebx
  void *dest; // [esp+Ch] [ebp-A8h]
  void *desta; // [esp+Ch] [ebp-A8h]
  void *destb; // [esp+Ch] [ebp-A8h]
  unsigned int stopaddr; // [esp+18h] [ebp-9Ch]
  unsigned __int8 *end; // [esp+20h] [ebp-94h]
  unsigned __int8 *s; // [esp+24h] [ebp-90h]
  unsigned __int8 *s_1; // [esp+24h] [ebp-90h]
  unsigned __int8 *s_2; // [esp+24h] [ebp-90h]
  unsigned int buf[32]; // [esp+28h] [ebp-8Ch] BYREF
  int mid; // [esp+A8h] [ebp-Ch]
  int post; // [esp+ACh] [ebp-8h]
  int pre; // [esp+B0h] [ebp-4h]
  unsigned int counta; // [esp+C8h] [ebp+14h]

  pre = (3 - offset % 3) % 3;
  post = (offset + count) % 3;
  mid = (count - pre - post) / 3;
  s = (unsigned __int8 *)&prgn2[(stop & 0xFFFFFFE0) >> 2];
  end = (unsigned __int8 *)(prgn2 + 0x4000000);
  if ( IsSupportDMA() )
  {
    v4 = stop;
    LOBYTE(v4) = stop & 0xE0;
    stopaddr = 32 * (offset / 3) + v4;
    v5 = mid;
    if ( pre )
      v5 = mid + 1;
    if ( post )
      dest = (void *)(32 * (v5 + 1));
    else
      dest = (void *)(32 * v5);
    if ( stopaddr > 0xFFFFFFF )
      stopaddr -= 0x10000000;
    if ( (unsigned int)dest + stopaddr <= 0x10000000 )
    {
      dma_copy(0xC800000u, stopaddr, (unsigned int)dest);
    }
    else
    {
      dma_copy(0xC800000u, stopaddr, 0x10000000 - stopaddr);
      dma_copy(478150656 - stopaddr, 0, (unsigned int)dest + stopaddr - 0x10000000);
    }
    s_1 = (unsigned __int8 *)pmem;
    if ( pre == 2 )
    {
      memcpy(dst, pmem + 26, 5u);
      memcpy(dst + 5, s_1, 5u);
      memcpy(dst + 10, s_1 + 16, 0xAu);
      dst += 20;
      s_1 += 32;
    }
    if ( pre == 1 )
    {
      memcpy(dst, s_1 + 16, 0xAu);
      dst += 10;
      s_1 += 32;
    }
    while ( 1 )
    {
      desta = (void *)mid--;
      if ( (int)desta <= 0 )
        break;
      memcpy(dst, s_1 + 5, 0xAu);
      memcpy(dst + 10, s_1 + 26, 5u);
      memcpy(dst + 15, s_1, 5u);
      memcpy(dst + 20, s_1 + 16, 0xAu);
      dst += 30;
      s_1 += 32;
    }
    if ( post == 1 )
      memcpy(dst, s_1 + 5, 0xAu);
    if ( post == 2 )
    {
      memcpy(dst, s_1 + 5, 0xAu);
      memcpy(dst + 10, s_1 + 26, 5u);
      memcpy(dst + 15, s_1, 5u);
    }
  }
  else
  {
    s_2 = &s[32 * (offset / 3)];
    if ( s_2 >= end )
      s_2 -= 0x10000000;
    if ( pre == 2 )
    {
      wcopy(buf, (unsigned int *)s_2, 0x20u);
      memcpy(dst, (char *)&buf[6] + 2, 5u);
      memcpy(dst + 5, buf, 5u);
      counta = count - 1;
      if ( !counta )
        return;
      memcpy(dst + 10, &buf[4], 0xAu);
      count = counta - 1;
      if ( !count )
        return;
      dst += 20;
      s_2 += 32;
      if ( s_2 >= end )
        s_2 = (unsigned __int8 *)prgn2;
    }
    if ( pre == 1 )
    {
      wcopy(buf, (unsigned int *)s_2, 0x20u);
      memcpy(dst, &buf[4], 0xAu);
      if ( count == 1 )
        return;
      dst += 10;
      s_2 += 32;
      if ( s_2 >= end )
        s_2 = (unsigned __int8 *)prgn2;
    }
    while ( 1 )
    {
      destb = (void *)mid--;
      if ( (int)destb <= 0 )
        break;
      wcopy(buf, (unsigned int *)s_2, 0x20u);
      memcpy(dst, (char *)&buf[1] + 1, 0xAu);
      memcpy(dst + 10, (char *)&buf[6] + 2, 5u);
      memcpy(dst + 15, buf, 5u);
      memcpy(dst + 20, &buf[4], 0xAu);
      dst += 30;
      s_2 += 32;
      if ( s_2 >= end )
        s_2 = (unsigned __int8 *)prgn2;
    }
    if ( post == 1 )
    {
      wcopy(buf, (unsigned int *)s_2, 0x20u);
      memcpy(dst, (char *)&buf[1] + 1, 0xAu);
    }
    if ( post == 2 )
    {
      wcopy(buf, (unsigned int *)s_2, 0x20u);
      memcpy(dst, (char *)&buf[1] + 1, 0xAu);
      memcpy(dst + 10, (char *)&buf[6] + 2, 5u);
      memcpy(dst + 15, buf, 5u);
    }
  }
}

static DSP_BUF *ds_send_pamp_gif_record(
        DS_DESC *desc,
        DSP_BUF *rdb,
        DECI2_HDR *dh,
        unsigned int code,
        unsigned int result,
        unsigned int stop,
        unsigned int offset,
        int count)
{
  DSP_BUF *db; // [esp+0h] [ebp-8h]

  db = ds_alloc_buf(1280, 72, 0, 10 * count + 8);
  if ( db )
  {
    *(_DWORD *)&db->buf[8] = code;
    *(_DWORD *)&db->buf[12] = result;
    if ( count > 0 )
      copy_gif_record((unsigned __int8 *)&db->buf[16], stop, offset, count);
    ds_send_desc(desc, db);
    return ds_free_buf(rdb);
  }
  else
  {
    ds_error("no memory");
    return rdb;
  }
}

static DSP_BUF *ds_send_pamp_rdmem(
        DS_DESC *desc,
        DSP_BUF *rdb,
        DECI2_HDR *dh,
        unsigned int code,
        unsigned int result,
        unsigned int stop,
        unsigned int offset,
        int count)
{
  DSP_BUF *db; // [esp+0h] [ebp-8h]

  db = ds_alloc_buf(1280, 72, 0, count + 8);
  if ( db )
  {
    *(_DWORD *)&db->buf[8] = code;
    *(_DWORD *)&db->buf[12] = result;
    if ( count > 0 )
      copy_rdmem((unsigned __int8 *)&db->buf[16], stop, offset, count);
    ds_send_desc(desc, db);
    return ds_free_buf(rdb);
  }
  else
  {
    ds_error("no memory");
    return rdb;
  }
}

static DSP_BUF *ds_send_pamp_iop_rdmem(
        DS_DESC *desc,
        DSP_BUF *rdb,
        DECI2_HDR *dh,
        unsigned int code,
        unsigned int result,
        unsigned int stop,
        unsigned int offset,
        int count)
{
  DSP_BUF *db; // [esp+0h] [ebp-8h]

  db = ds_alloc_buf(1280, 72, 0, count + 8);
  if ( db )
  {
    *(_DWORD *)&db->buf[8] = code;
    *(_DWORD *)&db->buf[12] = result;
    if ( count > 0 )
      copy_rdmem((unsigned __int8 *)&db->buf[16], stop, offset, count);
    ds_send_desc(desc, db);
    return ds_free_buf(rdb);
  }
  else
  {
    ds_error("no memory");
    return rdb;
  }
}

static DSP_BUF *ds_send_pamp_iop_record(
        DS_DESC *desc,
        DSP_BUF *rdb,
        DECI2_HDR *dh,
        unsigned int code,
        unsigned int result,
        unsigned int stop,
        unsigned int offset,
        int count)
{
  DSP_BUF *db; // [esp+0h] [ebp-8h]

  db = ds_alloc_buf(1280, 72, 0, 16 * count + 8);
  if ( db )
  {
    *(_DWORD *)&db->buf[8] = code;
    *(_DWORD *)&db->buf[12] = result;
    if ( count > 0 )
      copy_iop_record((unsigned __int8 *)&db->buf[16], stop, offset, count);
    ds_send_desc(desc, db);
    return ds_free_buf(rdb);
  }
  else
  {
    ds_error("no memory");
    return rdb;
  }
}

static DSP_BUF *ds_send_pamp_eegs_record(
        DS_DESC *desc,
        DSP_BUF *rdb,
        DECI2_HDR *dh,
        unsigned int code,
        unsigned int result,
        unsigned int stop,
        unsigned int offset,
        int count)
{
  DSP_BUF *db; // [esp+0h] [ebp-8h]

  db = ds_alloc_buf(1280, 72, 0, 10 * count + 8);
  if ( db )
  {
    *(_DWORD *)&db->buf[8] = code;
    *(_DWORD *)&db->buf[12] = result;
    if ( count > 0 )
      copy_eegs_record((unsigned __int8 *)&db->buf[16], stop, offset + 2, count);
    ds_send_desc(desc, db);
    return ds_free_buf(rdb);
  }
  else
  {
    ds_error("no memory");
    return rdb;
  }
}

static DSP_BUF *recv_pamp_notpa(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len)
{
  return ds_send_pamp(desc, db, dh, *(_DWORD *)&dh[1].length + 1, 6u, 0, 0);
}

static DSP_BUF *recv_pamp_getversion(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len)
{
  PAMP_VERSION ver; // [esp+4h] [ebp-14h] BYREF

  ver.bid = *screg;
  ver.master = screg[128];
  if ( IsSupportEEGS() )
    ver.eegs = screg[16512];
  else
    ver.eegs = 0;
  if ( IsSupportGIF() )
    ver.gif = screg[32896];
  else
    ver.gif = 0;
  if ( IsSupportIOP() )
    ver.iop = screg[49280];
  else
    ver.iop = 0;
  return ds_send_pamp(desc, db, dh, 1u, 0, &ver, 20);
}

static DSP_BUF *recv_pamp_writereg(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len)
{
  unsigned int no; // [esp+8h] [ebp-Ch]
  unsigned int kind; // [esp+Ch] [ebp-8h]

  kind = *(_DWORD *)&dh[1].length & 0xF0000000;
  no = *(_DWORD *)&dh[1].length & 0xFFFFFFF;
  if ( kind == 0x40000000 )
  {
    plcrs[no >> 2] = *(_DWORD *)&dh[1].protocol;
  }
  else if ( kind == 1610612736 )
  {
    screg[no >> 2] = *(_DWORD *)&dh[1].protocol;
  }
  return ds_send_pamp(desc, db, dh, *(_DWORD *)&dh[1].length + 1, 0, 0, 0);
}

static DSP_BUF *recv_pamp_readreg(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len)
{
  unsigned int no; // [esp+0h] [ebp-14h]
  unsigned int kind; // [esp+4h] [ebp-10h]
  unsigned int val; // [esp+10h] [ebp-4h] BYREF

  kind = *(_DWORD *)&dh[1].length & 0xF0000000;
  no = *(_DWORD *)&dh[1].length & 0xFFFFFFF;
  if ( kind == 0x40000000 )
  {
    val = plcrs[no >> 2];
  }
  else if ( kind == 1610612736 )
  {
    val = screg[no >> 2];
  }
  return ds_send_pamp(desc, db, dh, *(_DWORD *)&dh[1].length + 1, 0, &val, 4);
}

static DSP_BUF *recv_pamp_eegs_record(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len)
{
  unsigned int size; // [esp+0h] [ebp-10h]
  unsigned int offset; // [esp+4h] [ebp-Ch]
  unsigned int *ph; // [esp+8h] [ebp-8h]

  ph = (unsigned int *)&dh[1].length;
  offset = *(_DWORD *)&dh[1].protocol;
  size = *(_DWORD *)&dh[2].length;
  if ( IsSampling() )
    return ds_send_pamp(desc, db, dh, *ph + 1, 2u, 0, 0);
  if ( IsNotEEGSSampling() || size + offset > 0x1800000 )
    return ds_send_pamp(desc, db, dh, *ph + 1, 1u, 0, 0);
  return ds_send_pamp_eegs_record(desc, db, dh, *ph + 1, 0, screg[32769], offset, size);
}

static DSP_BUF *recv_pamp_eegs_rdmem(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len)
{
  unsigned int size; // [esp+0h] [ebp-10h]
  unsigned int offset; // [esp+4h] [ebp-Ch]
  unsigned int *ph; // [esp+8h] [ebp-8h]

  ph = (unsigned int *)&dh[1].length;
  offset = *(_DWORD *)&dh[1].protocol;
  size = *(_DWORD *)&dh[2].length;
  if ( IsSampling() )
    return ds_send_pamp(desc, db, dh, *ph + 1, 2u, 0, 0);
  if ( IsNotEEGSSampling() || size + offset > 0x10000000 )
    return ds_send_pamp(desc, db, dh, *ph + 1, 1u, 0, 0);
  return ds_send_pamp_rdmem(desc, db, dh, *ph + 1, 0, screg[32769], offset, size);
}

static DSP_BUF *recv_pamp_gif_record(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len)
{
  unsigned int size; // [esp+0h] [ebp-10h]
  unsigned int offset; // [esp+4h] [ebp-Ch]
  unsigned int *ph; // [esp+8h] [ebp-8h]

  ph = (unsigned int *)&dh[1].length;
  offset = *(_DWORD *)&dh[1].protocol;
  size = *(_DWORD *)&dh[2].length;
  if ( IsSampling() )
    return ds_send_pamp(desc, db, dh, *ph + 1, 2u, 0, 0);
  if ( size + offset <= 0x1800000 )
    return ds_send_pamp_gif_record(desc, db, dh, *ph + 1, 0, screg[32769], offset, size);
  return ds_send_pamp(desc, db, dh, *ph + 1, 1u, 0, 0);
}

static DSP_BUF *recv_pamp_gif_rdmem(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len)
{
  unsigned int size; // [esp+0h] [ebp-10h]
  unsigned int offset; // [esp+4h] [ebp-Ch]
  unsigned int *ph; // [esp+8h] [ebp-8h]

  ph = (unsigned int *)&dh[1].length;
  offset = *(_DWORD *)&dh[1].protocol;
  size = *(_DWORD *)&dh[2].length;
  if ( IsSampling() )
    return ds_send_pamp(desc, db, dh, *ph + 1, 2u, 0, 0);
  if ( size + offset <= 0x10000000 )
    return ds_send_pamp_rdmem(desc, db, dh, *ph + 1, 0, screg[32769], offset, size);
  return ds_send_pamp(desc, db, dh, *ph + 1, 1u, 0, 0);
}

static DSP_BUF *recv_pamp_iop_rdmem(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len)
{
  unsigned int size; // [esp+0h] [ebp-10h]
  unsigned int offset; // [esp+4h] [ebp-Ch]
  unsigned int *ph; // [esp+8h] [ebp-8h]

  ph = (unsigned int *)&dh[1].length;
  offset = *(_DWORD *)&dh[1].protocol;
  size = *(_DWORD *)&dh[2].length;
  if ( IsSampling() )
    return ds_send_pamp(desc, db, dh, *ph + 1, 2u, 0, 0);
  if ( size + offset <= 0x10000000 )
    return ds_send_pamp_iop_rdmem(desc, db, dh, *ph + 1, 0, screg[49153], offset, size);
  return ds_send_pamp(desc, db, dh, *ph + 1, 1u, 0, 0);
}

static DSP_BUF *recv_pamp_iop_record(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len)
{
  unsigned int size; // [esp+0h] [ebp-10h]
  unsigned int offset; // [esp+4h] [ebp-Ch]
  unsigned int *ph; // [esp+8h] [ebp-8h]

  ph = (unsigned int *)&dh[1].length;
  offset = *(_DWORD *)&dh[1].protocol;
  size = *(_DWORD *)&dh[2].length;
  if ( IsSampling() )
    return ds_send_pamp(desc, db, dh, *ph + 1, 2u, 0, 0);
  if ( size + offset <= 0x600000 )
    return ds_send_pamp_iop_record(desc, db, dh, *ph + 1, 0, screg[49153], offset, size);
  return ds_send_pamp(desc, db, dh, *ph + 1, 1u, 0, 0);
}

static DSP_BUF *recv_pamp_reset(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len)
{
  screg[1] = 0x80000000;
  screg[1] = 0;
  screg[9] = 0;
  screg[3] = 0;
  screg[4] = 0;
  screg[5] = 0;
  screg[6] = 0;
  screg[7] = 0;
  screg[8] = 0x80000000;
  screg[8] = 0;
  screg[12] = 0;
  ds_ioctl(sam_fd, 0x53414D01u, 30);
  while ( (screg[16422] & 0x80000000) == 0 )
    ;
  while ( (screg[32806] & 0x80000000) == 0 )
    ;
  while ( (screg[49190] & 0x80000000) == 0 )
    ;
  IsSupportIOP();
  return ds_send_pamp(desc, db, dh, 3u, 0, 0, 0);
}

static DSP_BUF *recv_pamp_master_trigger_a_getselect(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  unsigned int reg; // [esp+0h] [ebp-8h] BYREF
  unsigned int v7; // [esp+4h] [ebp-4h]

  v7 = 0;
  reg = screg[4];
  return ds_send_pamp(desc, db, dh, 0x11u, 0, &reg, 4);
}

static DSP_BUF *recv_pamp_master_trigger_b_getselect(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  unsigned int reg; // [esp+0h] [ebp-8h] BYREF
  unsigned int v7; // [esp+4h] [ebp-4h]

  v7 = 0;
  reg = screg[5];
  return ds_send_pamp(desc, db, dh, 0x15u, 0, &reg, 4);
}

static DSP_BUF *recv_pamp_master_trigger_c_getselect(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  unsigned int reg; // [esp+0h] [ebp-8h] BYREF
  unsigned int v7; // [esp+4h] [ebp-4h]

  v7 = 0;
  reg = screg[6];
  return ds_send_pamp(desc, db, dh, 0x19u, 0, &reg, 4);
}

static DSP_BUF *recv_pamp_master_trigger_d_getselect(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  unsigned int reg; // [esp+0h] [ebp-8h] BYREF
  unsigned int v7; // [esp+4h] [ebp-4h]

  v7 = 0;
  reg = screg[7];
  return ds_send_pamp(desc, db, dh, 0x1Du, 0, &reg, 4);
}

static DSP_BUF *recv_pamp_master_trigger_a_putselect(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  if ( IsSampling() )
    return ds_send_pamp(desc, db, dh, *(_DWORD *)&dh[1].length + 1, 2u, 0, 0);
  screg[4] = args[1];
  return ds_send_pamp(desc, db, dh, 0x13u, 0, 0, 0);
}

static DSP_BUF *recv_pamp_master_trigger_b_putselect(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  if ( IsSampling() )
    return ds_send_pamp(desc, db, dh, *(_DWORD *)&dh[1].length + 1, 2u, 0, 0);
  screg[5] = args[1];
  return ds_send_pamp(desc, db, dh, 0x17u, 0, 0, 0);
}

static DSP_BUF *recv_pamp_master_trigger_c_putselect(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  if ( IsSampling() )
    return ds_send_pamp(desc, db, dh, *(_DWORD *)&dh[1].length + 1, 2u, 0, 0);
  screg[6] = args[1];
  return ds_send_pamp(desc, db, dh, 0x1Bu, 0, 0, 0);
}

static DSP_BUF *recv_pamp_master_trigger_d_putselect(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  if ( IsSampling() )
    return ds_send_pamp(desc, db, dh, *(_DWORD *)&dh[1].length + 1, 2u, 0, 0);
  screg[7] = args[1];
  return ds_send_pamp(desc, db, dh, 0x1Fu, 0, 0, 0);
}

static DSP_BUF *recv_pamp_master_getstatus(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len)
{
  unsigned int status; // [esp+4h] [ebp-8h] BYREF
  unsigned int v7; // [esp+8h] [ebp-4h]

  v7 = -1;
  status = 0;
  status = (IsSampling() << 31) | screg[8] & 0x7FFFFFFF;
  v7 = 0;
  return ds_send_pamp(desc, db, dh, 9u, 0, &status, 4);
}

static DSP_BUF *recv_pamp_master_trigger_gettrigpos(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  unsigned int status; // [esp+0h] [ebp-8h] BYREF
  unsigned int v7; // [esp+4h] [ebp-4h]

  v7 = -1;
  status = 0;
  status = screg[0x4000] & 0xFFFFFFF;
  if ( status )
  {
    status = ((status + 384) & 0xFFFFFFF) >> 18;
    if ( status )
      status = 1024 - status;
  }
  else
  {
    status = 1024;
  }
  v7 = 0;
  return ds_send_pamp(desc, db, dh, 0x21u, 0, &status, 4);
}

static DSP_BUF *recv_pamp_master_trigger_puttrigpos(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  unsigned int pos; // [esp+8h] [ebp-Ch]
  unsigned int pos_1; // [esp+8h] [ebp-Ch]
  unsigned int pos_2; // [esp+8h] [ebp-Ch]
  unsigned int v9; // [esp+10h] [ebp-4h]

  pos = args[1];
  if ( IsSampling() )
    return ds_send_pamp(desc, db, dh, *(_DWORD *)&dh[1].length + 1, 2u, 0, 0);
  if ( pos <= 0x400 )
  {
    pos_1 = 1024 - pos;
    if ( pos_1 )
      pos_2 = (pos_1 << 18) - 512;
    else
      pos_2 = 0;
    screg[0x4000] = pos_2 + (screg[0x4000] & 0xE0000000 | 0x10000000);
    screg[0x8000] = pos_2 + (screg[0x8000] & 0xE0000000 | 0x10000000);
    v9 = 0;
    if ( IsSupportIOP() )
      screg[49152] = 3 * (pos_2 >> 2) + (screg[49152] & 0xE0000000 | 0x10000000) + 0x4000000;
  }
  else
  {
    v9 = 1;
  }
  return ds_send_pamp(desc, db, dh, 0x23u, v9, 0, 0);
}

static DSP_BUF *recv_pamp_master_sampling_start(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  unsigned int trigselect; // [esp+0h] [ebp-Ch]
  unsigned int reg; // [esp+4h] [ebp-8h]
  unsigned int reg_1; // [esp+4h] [ebp-8h]
  unsigned int reg_2; // [esp+4h] [ebp-8h]
  unsigned int v10; // [esp+8h] [ebp-4h]

  if ( IsSampling() )
  {
    v10 = 2;
  }
  else
  {
    trigselect = 0;
    screg[8] = 0x80000000;
    if ( (screg[4] & 0x80000000) != 0 )
      trigselect = screg[4];
    if ( (screg[5] & 0x80000000) != 0 )
      trigselect |= screg[5];
    if ( (screg[6] & 0x80000000) != 0 )
      trigselect |= screg[6];
    if ( (screg[7] & 0x80000000) != 0 )
      trigselect |= screg[7];
    reg = screg[0x4000] & 0xFFFFFFF;
    if ( reg )
      reg |= 0x10000000u;
    if ( (trigselect & 1) != 0 )
      reg |= 0x40000000u;
    screg[0x4000] = reg;
    reg_1 = screg[0x8000] & 0xFFFFFFF;
    if ( reg_1 )
      reg_1 |= 0x10000000u;
    if ( (trigselect & 2) != 0 )
      reg_1 |= 0x40000000u;
    if ( (trigselect & 4) != 0 )
      reg_1 |= 0x20000000u;
    screg[0x8000] = reg_1;
    reg_2 = screg[49152] & 0xFFFFFFF;
    if ( reg_2 )
      reg_2 |= 0x10000000u;
    if ( (trigselect & 8) != 0 )
      reg_2 |= 0x40000000u;
    if ( (trigselect & 0x10) != 0 )
      reg_2 |= 0x20000000u;
    screg[49152] = reg_2;
    screg[9] = 7;
    v10 = 0;
  }
  return ds_send_pamp(desc, db, dh, 5u, v10, 0, 0);
}

static DSP_BUF *recv_pamp_master_sampling_stop(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len)
{
  unsigned int v6; // [esp+0h] [ebp-4h]

  if ( IsSampling() )
  {
    screg[9] = 0;
    v6 = 0;
  }
  else
  {
    v6 = 1;
  }
  return ds_send_pamp(desc, db, dh, 7u, v6, 0, 0);
}

// local variable allocation has failed, the output may be wrong!
static DSP_BUF *recv_pamp_iop_trigger_a_getdata(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  int ras; // [esp+4h] [ebp-1Ch]
  int cas; // [esp+8h] [ebp-18h]
  TRIG_DATA data; // [esp+Ch] [ebp-14h] OVERLAPPED BYREF

  LOBYTE(data.spu2core0) &= ~1u;
  HIBYTE(data.addr) &= 0xC1u;
  ras = screg[49164];
  cas = screg[49166];
  HIBYTE(data.addr) = ((unsigned __int8)(ras >> 31) << 7) | (((ras >> 30) & 1) << 6) & 0x7F | HIBYTE(data.addr) & 0x3F;
  data.addr = GetIOPAddr(ras, cas) & 0x1FFFFFF | data.addr & 0xFE000000;
  data.spu2core1 = GetIOPAddr(screg[49165], screg[49167]);
  data.cdrom = screg[49168];
  data.dev9 = screg[49169];
  if ( (screg[49156] & 0x10) != 0 )
  {
    LOBYTE(data.spu2core0) |= 1u;
    if ( (screg[49157] & 4) != 0 )
    {
      LOBYTE(data.spu2core0) |= 1u;
      if ( (screg[49162] & 8) != 0 )
        HIBYTE(data.addr) |= 8u;
      else
        HIBYTE(data.addr) &= ~8u;
      if ( (screg[49162] & 4) != 0 )
        HIBYTE(data.addr) |= 2u;
      else
        HIBYTE(data.addr) &= ~2u;
      if ( (screg[49162] & 0x10) != 0 )
        HIBYTE(data.addr) |= 4u;
      else
        HIBYTE(data.addr) &= ~4u;
      if ( (screg[49162] & 0x20) != 0 )
        HIBYTE(data.addr) |= 0x10u;
      else
        HIBYTE(data.addr) &= ~0x10u;
    }
    else if ( (screg[49157] & 1) != 0 )
    {
      HIBYTE(data.addr) |= 0x20u;
    }
  }
  return ds_send_pamp(desc, db, dh, 0x30011u, 0, &data, 20);
}

// local variable allocation has failed, the output may be wrong!
static DSP_BUF *recv_pamp_iop_trigger_b_getdata(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  int ras; // [esp+4h] [ebp-1Ch]
  int cas; // [esp+8h] [ebp-18h]
  TRIG_DATA data; // [esp+Ch] [ebp-14h] OVERLAPPED BYREF

  LOBYTE(data.spu2core0) &= ~1u;
  HIBYTE(data.addr) &= 0xC1u;
  ras = screg[49176];
  cas = screg[49178];
  HIBYTE(data.addr) = ((unsigned __int8)(ras >> 31) << 7) | (((ras >> 30) & 1) << 6) & 0x7F | HIBYTE(data.addr) & 0x3F;
  data.addr = GetIOPAddr(ras, cas) & 0x1FFFFFF | data.addr & 0xFE000000;
  data.spu2core1 = GetIOPAddr(screg[49177], screg[49179]);
  data.cdrom = screg[49180];
  data.dev9 = screg[49181];
  if ( (screg[49158] & 0x10) != 0 )
  {
    LOBYTE(data.spu2core0) |= 1u;
    if ( (screg[49159] & 4) != 0 )
    {
      LOBYTE(data.spu2core0) |= 1u;
      if ( (screg[49174] & 8) != 0 )
        HIBYTE(data.addr) |= 8u;
      else
        HIBYTE(data.addr) &= ~8u;
      if ( (screg[49174] & 4) != 0 )
        HIBYTE(data.addr) |= 2u;
      else
        HIBYTE(data.addr) &= ~2u;
      if ( (screg[49174] & 0x10) != 0 )
        HIBYTE(data.addr) |= 4u;
      else
        HIBYTE(data.addr) &= ~4u;
      if ( (screg[49174] & 0x20) != 0 )
        HIBYTE(data.addr) |= 0x10u;
      else
        HIBYTE(data.addr) &= ~0x10u;
    }
    else if ( (screg[49159] & 1) != 0 )
    {
      HIBYTE(data.addr) |= 0x20u;
    }
  }
  return ds_send_pamp(desc, db, dh, 0x30021u, 0, &data, 20);
}

static DSP_BUF *recv_pamp_iop_trigger_a_putdata(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  unsigned int v6; // edx
  unsigned int v7; // edx
  unsigned int v8; // edx
  unsigned int v9; // edx
  unsigned int v10; // edx
  unsigned int v11; // edx
  unsigned int v12; // edx
  unsigned int addr; // [esp+8h] [ebp-10h]
  unsigned int addr_1; // [esp+8h] [ebp-10h]
  unsigned int *ph; // [esp+Ch] [ebp-Ch]
  iopmem_t *pmem; // [esp+14h] [ebp-4h]

  pmem = (iopmem_t *)&CONTAINING_RECORD(args, iopmem_t, addr)->spu2core0;
  ph = (unsigned int *)&dh[1].length;
  if ( IsSampling() )
    return ds_send_pamp(desc, db, dh, *ph + 1, 2u, 0, 0);
  if ( ((((pmem->addr & 0x8000000) != 0) | ((pmem->addr & 0x4000000) != 0) | ((pmem->addr & 0x2000000) != 0) | HIBYTE(pmem->addr) & 1) & ((pmem->addr & 0x10000000) != 0)) != 0 )
    return ds_send_pamp(desc, db, dh, *ph + 1, 1u, 0, 0);
  if ( *((char *)args + 7) < 0 )
  {
    v6 = screg[49156];
    LOBYTE(v6) = 0;
    screg[49156] = v6;
    screg[49157] = 0;
    screg[49156] |= 0x80000010;
    if ( (*((_BYTE *)args + 7) & 0x10) != 0 )
    {
      screg[49157] |= 0x80000001;
      screg[49160] = 54;
    }
    else if ( ((*((_BYTE *)args + 7) & 8) != 0) | ((*((_BYTE *)args + 7) & 4) != 0) | ((*((_BYTE *)args + 7) & 2) != 0) | *((_BYTE *)args + 7) & 1 )
    {
      screg[49157] |= 0x80000004;
      v7 = screg[49162];
      LOBYTE(v7) = 0;
      screg[49162] = v7;
      v8 = screg[49162];
      BYTE1(v8) |= 0xDu;
      screg[49162] = v8;
      if ( (*((_BYTE *)args + 7) & 1) != 0 )
      {
        v9 = screg[49162];
        LOBYTE(v9) = v9 | 4;
        screg[49162] = v9;
      }
      if ( (*((_BYTE *)args + 7) & 4) != 0 )
      {
        v10 = screg[49162];
        LOBYTE(v10) = v10 | 8;
        screg[49162] = v10;
      }
      if ( (*((_BYTE *)args + 7) & 2) != 0 )
      {
        v11 = screg[49162];
        LOBYTE(v11) = v11 | 0x10;
        screg[49162] = v11;
      }
      if ( (*((_BYTE *)args + 7) & 8) != 0 )
      {
        v12 = screg[49162];
        LOBYTE(v12) = v12 | 0x20;
        screg[49162] = v12;
      }
    }
  }
  addr = pmem->addr & 0xFFFFFF;
  screg[49164] = (((*((_BYTE *)args + 7) & 0x20) != 0) << 30) | (((*((_BYTE *)args + 7) & 0x40) != 0) << 31) | GetIOPRas(addr);
  screg[49166] = GetIOPCas(addr);
  addr_1 = args[2];
  screg[49165] = GetIOPRas(addr_1);
  screg[49167] = GetIOPCas(addr_1);
  screg[49168] = args[3];
  screg[49169] = args[4];
  return ds_send_pamp(desc, db, dh, 0x30013u, 0, 0, 0);
}

static DSP_BUF *recv_pamp_iop_trigger_b_putdata(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  unsigned int v6; // edx
  unsigned int v7; // edx
  unsigned int v8; // edx
  unsigned int v9; // edx
  unsigned int v10; // edx
  unsigned int v11; // edx
  unsigned int v12; // edx
  unsigned int v13; // edx
  unsigned int addr; // [esp+8h] [ebp-10h]
  unsigned int addr_1; // [esp+8h] [ebp-10h]
  unsigned int *ph; // [esp+Ch] [ebp-Ch]
  iopmem_t *pmem; // [esp+14h] [ebp-4h]

  pmem = (iopmem_t *)&CONTAINING_RECORD(args, iopmem_t, addr)->spu2core0;
  ph = (unsigned int *)&dh[1].length;
  if ( IsSampling() )
    return ds_send_pamp(desc, db, dh, *ph + 1, 2u, 0, 0);
  if ( ((((pmem->addr & 0x8000000) != 0) | ((pmem->addr & 0x4000000) != 0) | ((pmem->addr & 0x2000000) != 0) | HIBYTE(pmem->addr) & 1) & ((pmem->addr & 0x10000000) != 0)) != 0 )
    return ds_send_pamp(desc, db, dh, *ph + 1, 1u, 0, 0);
  v6 = screg[49158];
  LOBYTE(v6) = 0;
  screg[49158] = v6;
  v7 = screg[49159];
  LOBYTE(v7) = 0;
  screg[49159] = v7;
  if ( *((char *)args + 7) < 0 )
  {
    screg[49158] |= 0x80000010;
    if ( (*((_BYTE *)args + 7) & 0x10) != 0 )
    {
      screg[49159] |= 0x80000001;
      screg[49172] = 54;
    }
    else if ( ((*((_BYTE *)args + 7) & 8) != 0) | ((*((_BYTE *)args + 7) & 4) != 0) | ((*((_BYTE *)args + 7) & 2) != 0) | *((_BYTE *)args + 7) & 1 )
    {
      screg[49159] |= 0x80000004;
      v8 = screg[49174];
      LOBYTE(v8) = 0;
      screg[49174] = v8;
      v9 = screg[49174];
      BYTE1(v9) |= 0xDu;
      screg[49174] = v9;
      if ( (*((_BYTE *)args + 7) & 1) != 0 )
      {
        v10 = screg[49174];
        LOBYTE(v10) = v10 | 4;
        screg[49174] = v10;
      }
      if ( (*((_BYTE *)args + 7) & 4) != 0 )
      {
        v11 = screg[49174];
        LOBYTE(v11) = v11 | 8;
        screg[49174] = v11;
      }
      if ( (*((_BYTE *)args + 7) & 2) != 0 )
      {
        v12 = screg[49174];
        LOBYTE(v12) = v12 | 0x10;
        screg[49174] = v12;
      }
      if ( (*((_BYTE *)args + 7) & 8) != 0 )
      {
        v13 = screg[49174];
        LOBYTE(v13) = v13 | 0x20;
        screg[49174] = v13;
      }
    }
  }
  addr = pmem->addr & 0xFFFFFF;
  screg[49176] = (((*((_BYTE *)args + 7) & 0x20) != 0) << 30) | (((*((_BYTE *)args + 7) & 0x40) != 0) << 31) | GetIOPRas(addr);
  screg[49178] = GetIOPCas(addr);
  addr_1 = args[2];
  screg[49177] = GetIOPRas(addr_1);
  screg[49179] = GetIOPCas(addr_1);
  screg[49180] = args[3];
  screg[49181] = args[4];
  return ds_send_pamp(desc, db, dh, 0x30023u, 0, 0, 0);
}

static DSP_BUF *recv_pamp_iop_trigger_getstatus(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  unsigned int intc; // [esp+0h] [ebp-8h] BYREF
  unsigned int v7; // [esp+4h] [ebp-4h]

  v7 = 0;
  intc = screg[49163];
  return ds_send_pamp(desc, db, dh, 0x30019u, 0, &intc, 4);
}

static DSP_BUF *recv_pamp_iop_trigger_a_getintc(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  unsigned int reg; // [esp+0h] [ebp-Ch]
  unsigned int intc; // [esp+4h] [ebp-8h] BYREF
  unsigned int v8; // [esp+8h] [ebp-4h]

  v8 = 0;
  intc = screg[49163];
  reg = screg[49156];
  if ( (reg & 0x80000000) != 0 && (reg & 8) != 0 )
    intc |= 0x80000000;
  return ds_send_pamp(desc, db, dh, 0x30019u, v8, &intc, 4);
}

static DSP_BUF *recv_pamp_iop_trigger_a_putintc(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  unsigned int reg; // [esp+0h] [ebp-10h]
  unsigned int intc; // [esp+4h] [ebp-Ch]

  if ( IsSampling() )
    return ds_send_pamp(desc, db, dh, *(_DWORD *)&dh[1].length + 1, 2u, 0, 0);
  intc = args[1];
  screg[49163] = intc & 1;
  if ( (intc & 0x80000000) == 0 )
  {
    reg = screg[49156];
    if ( (reg & 8) != 0 )
    {
      reg &= ~8u;
      if ( (reg & 0xFFFFFFF) == 0 )
        reg &= ~0x80000000;
    }
    screg[49156] = reg;
  }
  else
  {
    screg[49156] = screg[49156] & 0x7FFFFF00 | 0x80000008;
    screg[49157] = 0;
  }
  return ds_send_pamp(desc, db, dh, 0x30017u, 0, 0, 0);
}

static DSP_BUF *recv_pamp_iop_trigger_b_getintc(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  unsigned int reg; // [esp+0h] [ebp-Ch]
  unsigned int intc; // [esp+4h] [ebp-8h] BYREF
  unsigned int v8; // [esp+8h] [ebp-4h]

  v8 = 0;
  intc = screg[49175];
  reg = screg[49158];
  if ( (reg & 0x80000000) != 0 && (reg & 8) != 0 )
    intc |= 0x80000000;
  return ds_send_pamp(desc, db, dh, 0x30025u, v8, &intc, 4);
}

static DSP_BUF *recv_pamp_iop_trigger_b_putintc(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  unsigned int reg; // [esp+0h] [ebp-10h]
  unsigned int intc; // [esp+4h] [ebp-Ch]

  if ( IsSampling() )
    return ds_send_pamp(desc, db, dh, *(_DWORD *)&dh[1].length + 1, 2u, 0, 0);
  intc = args[1];
  screg[49158] = intc & 1;
  if ( (intc & 0x80000000) == 0 )
  {
    reg = screg[49158];
    if ( (reg & 8) != 0 )
    {
      reg &= ~8u;
      if ( (reg & 0xFFFFFFF) == 0 )
        reg &= ~0x80000000;
    }
    screg[49158] = reg;
  }
  else
  {
    screg[49158] = screg[49158] & 0x7FFFFF00 | 0x80000008;
    screg[49159] = 0;
  }
  return ds_send_pamp(desc, db, dh, 0x30027u, 0, 0, 0);
}

static DSP_BUF *recv_pamp_gif_trigger_a_putreg(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len)
{
  if ( IsSampling() )
    return ds_send_pamp(desc, db, dh, *(_DWORD *)&dh[1].length + 1, 2u, 0, 0);
  screg[32772] = args[1];
  screg[32773] = args[2];
  screg[32774] = args[4];
  screg[32775] = args[6];
  screg[32776] = args[3];
  screg[32777] = args[5];
  return ds_send_pamp(desc, db, dh, 0x20013u, 0, 0, 0);
}

static DSP_BUF *recv_pamp_gif_trigger_a_getreg(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len)
{
  TRIG_REG reg; // [esp+4h] [ebp-18h] BYREF

  reg.address = screg[32772];
  reg.addressmask = screg[32773];
  reg.hidata = screg[32776];
  reg.hidatamask = screg[32777];
  reg.lodata = screg[32774];
  reg.lodatamask = screg[32775];
  return ds_send_pamp(desc, db, dh, 0x20011u, 0, &reg, 24);
}

static DSP_BUF *recv_pamp_gif_trigger_a_putvblanking(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  if ( IsSampling() )
    return ds_send_pamp(desc, db, dh, *(_DWORD *)&dh[1].length + 1, 2u, 0, 0);
  screg[32778] = args[1];
  return ds_send_pamp(desc, db, dh, 0x20017u, 0, 0, 0);
}

static DSP_BUF *recv_pamp_gif_trigger_a_getintc(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  unsigned int gint; // [esp+0h] [ebp-8h] BYREF
  unsigned int v7; // [esp+4h] [ebp-4h]

  v7 = 0;
  gint = screg[32779];
  if ( (gint & 1) != 0 )
    gint |= 0x80000000;
  return ds_send_pamp(desc, db, dh, 0x20019u, v7, &gint, 4);
}

static DSP_BUF *recv_pamp_gif_trigger_b_getintc(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  unsigned int gint; // [esp+0h] [ebp-8h] BYREF
  unsigned int v7; // [esp+4h] [ebp-4h]

  v7 = 0;
  gint = screg[32787];
  if ( (gint & 1) != 0 )
    gint |= 0x80000000;
  return ds_send_pamp(desc, db, dh, 0x20029u, v7, &gint, 4);
}

static DSP_BUF *recv_pamp_gif_trigger_a_putintc(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  if ( IsSampling() )
    return ds_send_pamp(desc, db, dh, *(_DWORD *)&dh[1].length + 1, 2u, 0, 0);
  if ( (args[1] & 0x80000000) != 0 )
    screg[32779] = args[1] & 1;
  return ds_send_pamp(desc, db, dh, 0x2001Bu, 0, 0, 0);
}

static DSP_BUF *recv_pamp_gif_trigger_b_putintc(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  if ( IsSampling() )
    return ds_send_pamp(desc, db, dh, *(_DWORD *)&dh[1].length + 1, 2u, 0, 0);
  if ( (args[1] & 0x80000000) != 0 )
    screg[32787] = args[1] & 1;
  return ds_send_pamp(desc, db, dh, 0x2002Bu, 0, 0, 0);
}

static DSP_BUF *recv_pamp_gif_trigger_a_getvblanking(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  unsigned int vblank; // [esp+0h] [ebp-8h] BYREF
  unsigned int v7; // [esp+4h] [ebp-4h]

  v7 = 0;
  vblank = screg[32778];
  return ds_send_pamp(desc, db, dh, 0x20015u, 0, &vblank, 4);
}

static DSP_BUF *recv_pamp_iop_trigger_a_putvblanking(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  unsigned int reg; // [esp+0h] [ebp-Ch]
  unsigned int reg_1; // [esp+0h] [ebp-Ch]

  if ( IsSampling() )
    return ds_send_pamp(desc, db, dh, *(_DWORD *)&dh[1].length + 1, 2u, 0, 0);
  reg = args[1];
  screg[49170] = reg;
  if ( (reg & 0x80000000) == 0 )
  {
    reg_1 = screg[49156] & 0xFFFFFEFF;
    if ( (screg[49156] & 0xFFFFEFF) == 0 )
      reg_1 = screg[49156] & 0x7FFFFEFF;
    screg[49156] = reg_1;
  }
  else
  {
    screg[49156] |= 0x80000100;
  }
  return ds_send_pamp(desc, db, dh, 0x3001Bu, 0, 0, 0);
}

static DSP_BUF *recv_pamp_iop_trigger_a_getvblanking(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  unsigned int reg; // [esp+0h] [ebp-Ch]
  unsigned int vblank; // [esp+4h] [ebp-8h] BYREF
  unsigned int v8; // [esp+8h] [ebp-4h]

  v8 = 0;
  vblank = screg[49170];
  reg = screg[49156];
  if ( (reg & 0x80000000) != 0 && (reg & 0x100) != 0 )
    vblank |= 0x80000000;
  return ds_send_pamp(desc, db, dh, 0x30019u, v8, &vblank, 4);
}

static DSP_BUF *recv_pamp_iop_trigger_b_putvblanking(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  if ( IsSampling() )
    return ds_send_pamp(desc, db, dh, *(_DWORD *)&dh[1].length + 1, 2u, 0, 0);
  screg[49182] = args[1];
  return ds_send_pamp(desc, db, dh, 0x3002Bu, 0, 0, 0);
}

static DSP_BUF *recv_pamp_iop_trigger_b_getvblanking(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  unsigned int reg; // [esp+0h] [ebp-Ch]
  unsigned int vblank; // [esp+4h] [ebp-8h] BYREF
  unsigned int v8; // [esp+8h] [ebp-4h]

  v8 = 0;
  vblank = screg[49182];
  reg = screg[49158];
  if ( (reg & 0x80000000) != 0 && (reg & 0x100) != 0 )
    vblank |= 0x80000000;
  return ds_send_pamp(desc, db, dh, 0x30029u, v8, &vblank, 4);
}

static DSP_BUF *recv_pamp_gif_trigger_b_putreg(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len)
{
  if ( IsSampling() )
    return ds_send_pamp(desc, db, dh, *(_DWORD *)&dh[1].length + 1, 2u, 0, 0);
  screg[32780] = args[1];
  screg[32781] = args[2];
  screg[32782] = args[4];
  screg[32783] = args[6];
  screg[32784] = args[3];
  screg[32785] = args[5];
  return ds_send_pamp(desc, db, dh, 0x20023u, 0, 0, 0);
}

static DSP_BUF *recv_pamp_gif_trigger_b_getreg(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int len)
{
  TRIG_REG reg; // [esp+4h] [ebp-18h] BYREF

  reg.address = screg[32780];
  reg.addressmask = screg[32781];
  reg.hidata = screg[32784];
  reg.hidatamask = screg[32785];
  reg.lodata = screg[32782];
  reg.lodatamask = screg[32783];
  return ds_send_pamp(desc, db, dh, 0x20021u, 0, &reg, 24);
}

static DSP_BUF *recv_pamp_gif_trigger_b_getvblanking(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  unsigned int vb; // [esp+0h] [ebp-8h] BYREF
  unsigned int v7; // [esp+4h] [ebp-4h]

  v7 = 0;
  vb = screg[32786];
  return ds_send_pamp(desc, db, dh, 0x20025u, 0, &vb, 4);
}

static DSP_BUF *recv_pamp_gif_trigger_b_putvblanking(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  if ( IsSampling() )
    return ds_send_pamp(desc, db, dh, *(_DWORD *)&dh[1].length + 1, 2u, 0, 0);
  screg[32786] = args[1];
  return ds_send_pamp(desc, db, dh, 0x20027u, 0, 0, 0);
}

static DSP_BUF *recv_pamp_gif_trigger_getstatus(
        DS_DESC *desc,
        DSP_BUF *db,
        DECI2_HDR *dh,
        unsigned int *args,
        int len)
{
  unsigned int status; // [esp+0h] [ebp-8h] BYREF
  unsigned int v7; // [esp+4h] [ebp-4h]

  v7 = 0;
  status = screg[32770];
  return ds_send_pamp(desc, db, dh, 0x20003u, 0, &status, 4);
}

DSP_BUF *ds_recv_pamp(DS_DESC *desc, DSP_BUF *db)
{
  unsigned int code; // [esp+0h] [ebp-10h]
  unsigned int *args; // [esp+4h] [ebp-Ch]
  int len; // [esp+8h] [ebp-8h]
  DECI2_HDR *dh; // [esp+Ch] [ebp-4h]

  dh = (DECI2_HDR *)db->buf;
  if ( !db )
    return 0;
  args = (unsigned int *)&db->buf[8];
  len = dh->length - 12;
  code = *(_DWORD *)&db->buf[8];
  if ( !IsPA() )
    return recv_pamp_notpa(desc, db, dh, args, len);
  if ( code == 131094 )
    return recv_pamp_gif_trigger_a_putvblanking(desc, db, dh, args, len);
  if ( code > 0x20016 )
  {
    if ( code == 196624 )
      return recv_pamp_iop_trigger_a_getdata(desc, db, dh, args, len);
    if ( code > 0x30010 )
    {
      if ( code == 196640 )
        return recv_pamp_iop_trigger_b_getdata(desc, db, dh, args, len);
      if ( code > 0x30020 )
      {
        if ( code == 196646 )
          return recv_pamp_iop_trigger_b_putintc(desc, db, dh, args, len);
        if ( code > 0x30026 )
        {
          if ( code == 196648 )
            return recv_pamp_iop_trigger_b_getvblanking(desc, db, dh, args, len);
          if ( code == 196650 )
            return recv_pamp_iop_trigger_b_putvblanking(desc, db, dh, args, len);
        }
        else
        {
          if ( code == 196642 )
            return recv_pamp_iop_trigger_b_putdata(desc, db, dh, args, len);
          if ( code == 196644 )
            return recv_pamp_iop_trigger_b_getintc(desc, db, dh, args, len);
        }
      }
      else
      {
        if ( code == 196630 )
          return recv_pamp_iop_trigger_a_putintc(desc, db, dh, args, len);
        if ( code > 0x30016 )
        {
          if ( code == 196632 )
            return recv_pamp_iop_trigger_a_getvblanking(desc, db, dh, args, len);
          if ( code == 196634 )
            return recv_pamp_iop_trigger_a_putvblanking(desc, db, dh, args, len);
        }
        else
        {
          if ( code == 196626 )
            return recv_pamp_iop_trigger_a_putdata(desc, db, dh, args, len);
          if ( code == 196628 )
            return recv_pamp_iop_trigger_a_getintc(desc, db, dh, args, len);
        }
      }
    }
    else
    {
      if ( code == 131108 )
        return recv_pamp_gif_trigger_b_getvblanking(desc, db, dh, args, len);
      if ( code > 0x20024 )
      {
        if ( code == 131114 )
          return recv_pamp_gif_trigger_b_putintc(desc, db, dh, args, len);
        if ( code > 0x2002A )
        {
          if ( code == 196608 )
          {
            screg[3] = -2147483644;
            return recv_pamp_iop_record(desc, db, dh, args, len);
          }
          if ( code == 196610 )
            return recv_pamp_iop_trigger_getstatus(desc, db, dh, args, len);
        }
        else
        {
          if ( code == 131110 )
            return recv_pamp_gif_trigger_b_putvblanking(desc, db, dh, args, len);
          if ( code == 131112 )
            return recv_pamp_gif_trigger_b_getintc(desc, db, dh, args, len);
        }
      }
      else
      {
        if ( code == 131098 )
          return recv_pamp_gif_trigger_a_putintc(desc, db, dh, args, len);
        if ( code > 0x2001A )
        {
          if ( code == 131104 )
            return recv_pamp_gif_trigger_b_getreg(desc, db, dh, args, len);
          if ( code == 131106 )
            return recv_pamp_gif_trigger_b_putreg(desc, db, dh, args, len);
        }
        else if ( code == 131096 )
        {
          return recv_pamp_gif_trigger_a_getintc(desc, db, dh, args, len);
        }
      }
    }
    goto LABEL_123;
  }
  if ( code == 26 )
    return recv_pamp_master_trigger_c_putselect(desc, db, dh, args, len);
  if ( code > 0x1A )
  {
    if ( code == 0x10000 )
    {
      screg[3] = -2147483647;
      return recv_pamp_eegs_record(desc, db, dh, args, len);
    }
    if ( code > 0x10000 )
    {
      if ( code == 131088 )
        return recv_pamp_gif_trigger_a_getreg(desc, db, dh, args, len);
      if ( code > 0x20010 )
      {
        if ( code == 131090 )
          return recv_pamp_gif_trigger_a_putreg(desc, db, dh, args, len);
        if ( code == 131092 )
          return recv_pamp_gif_trigger_a_getvblanking(desc, db, dh, args, len);
      }
      else
      {
        if ( code == 0x20000 )
        {
          screg[3] = -2147483646;
          return recv_pamp_gif_record(desc, db, dh, args, len);
        }
        if ( code == 131074 )
          return recv_pamp_gif_trigger_getstatus(desc, db, dh, args, len);
      }
    }
    else
    {
      if ( code == 30 )
        return recv_pamp_master_trigger_d_putselect(desc, db, dh, args, len);
      if ( code > 0x1E )
      {
        if ( code == 32 )
          return recv_pamp_master_trigger_gettrigpos(desc, db, dh, args, len);
        if ( code == 34 )
          return recv_pamp_master_trigger_puttrigpos(desc, db, dh, args, len);
      }
      else if ( code == 28 )
      {
        return recv_pamp_master_trigger_d_getselect(desc, db, dh, args, len);
      }
    }
    goto LABEL_123;
  }
  if ( code == 8 )
    return recv_pamp_master_getstatus(desc, db, dh, args, len);
  if ( code > 8 )
  {
    if ( code == 20 )
      return recv_pamp_master_trigger_b_getselect(desc, db, dh, args, len);
    if ( code > 0x14 )
    {
      if ( code == 22 )
        return recv_pamp_master_trigger_b_putselect(desc, db, dh, args, len);
      if ( code == 24 )
        return recv_pamp_master_trigger_c_getselect(desc, db, dh, args, len);
    }
    else
    {
      if ( code == 16 )
        return recv_pamp_master_trigger_a_getselect(desc, db, dh, args, len);
      if ( code == 18 )
        return recv_pamp_master_trigger_a_putselect(desc, db, dh, args, len);
    }
    goto LABEL_123;
  }
  if ( code == 2 )
    return recv_pamp_reset(desc, db, dh, args, len);
  if ( code > 2 )
  {
    if ( code == 4 )
      return recv_pamp_master_sampling_start(desc, db, dh, args, len);
    if ( code == 6 )
      return recv_pamp_master_sampling_stop(desc, db, dh, args, len);
LABEL_123:
    if ( (code & 0xF0000000) == 1610612736 || (code & 0xF0000000) == 0x40000000 )
    {
      if ( (code & 3) == 0 )
        db = recv_pamp_readreg(desc, db, dh, args, len);
      if ( (code & 3) == 2 )
        db = recv_pamp_writereg(desc, db, dh, args, len);
    }
    if ( (code & 0xF0000000) == 0x10000000 )
    {
      screg[3] = -2147483647;
      db = recv_pamp_eegs_rdmem(desc, db, dh, args, len);
    }
    if ( (code & 0xF0000000) == 0x20000000 )
    {
      screg[3] = -2147483646;
      db = recv_pamp_gif_rdmem(desc, db, dh, args, len);
    }
    if ( (code & 0xF0000000) == 805306368 )
    {
      screg[3] = -2147483644;
      return recv_pamp_iop_rdmem(desc, db, dh, args, len);
    }
    return db;
  }
  if ( code )
    goto LABEL_123;
  return recv_pamp_getversion(desc, db, dh, args, len);
}

