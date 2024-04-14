#include "dsnet_prototypes.h"

typedef struct {long unsigned int Amd; unsigned int Ama; unsigned int Amras_x; unsigned int Amcas_x0; unsigned int Amcas_x1; unsigned int Amcas_x2; unsigned int Amcas_x3; unsigned int Arow_adrs; unsigned int Asmem_ref; unsigned int Amwe_x; unsigned int Abyte_acc; unsigned int Asmem_wr; unsigned int Asmem_acc; unsigned int Assrst_x; unsigned int Await_x; unsigned int Aube_x; unsigned int Aswr_x; unsigned int Asrd_x; unsigned int Art_x; unsigned int Adack_ex; unsigned int Adack9; unsigned int Adack8; unsigned int Adack5; unsigned int Adack4; unsigned int Adreq_ex; unsigned int Adreq9; unsigned int Adreq8; unsigned int Adreq5; unsigned int Adreq4; unsigned int Aint_ex; unsigned int Aint_x9; unsigned int Aint_x8; unsigned int Aint_x5; unsigned int Aint_x4; unsigned int Aextr_x; unsigned int Acs9c_x; unsigned int Acs_x9; unsigned int Acs_x8; unsigned int Acs_x5; unsigned int Acs_x4; unsigned int Acs_x1; unsigned int Acs_x2; unsigned int Aiois16_x; unsigned int Ass_wr; unsigned int Ass_acc; unsigned int Asrst_x; unsigned int Asint_x; unsigned int Agint_x; unsigned int Adack; unsigned int Adreq_x0; unsigned int Adreq_x1; unsigned int Abgnt_x; unsigned int Abreq_x; unsigned int Ardy_x; unsigned int Awrac_x; unsigned int Ardac_x; unsigned int Abe_x0; unsigned int Abe_x1; unsigned int Abe_x2; unsigned int Abe_x3; unsigned int Areserv0; unsigned int Ahblk1; unsigned int Avblk1; unsigned int Acycle_end; unsigned int Acycle_start; unsigned int Adma_pio0; unsigned int Adma_pio1; unsigned int Adirection; unsigned int Asif_acc; unsigned int Apage_end; unsigned int Apage_start; unsigned int Areserv1; unsigned int Aoffset; unsigned int AtriggerB; unsigned int AtriggerA; unsigned int Areserv2; unsigned int Amaster_trig;} MM;
typedef struct {unsigned int sdram_data1; unsigned int sdram_data2; unsigned int sdram_data3; unsigned int sdram_data4;} NN;

extern int SmemDataCheck(VERIFY_SSS *verify, MM *mm);
extern int SmemSequenceCheck(VERIFY_SSS *verify, MM *mm);
extern int SifSequenceCheck(VERIFY_SSS *verify, MM *mm);
extern int SsbusSequenceCheckExt(VERIFY_SSS *verify, MM *mm, int xr_w);
extern int SsbusSequenceCheck(VERIFY_SSS *verify, MM *mm);
extern int SmemCheckStrict(VERIFY_SSS *verify, MM *mm, MM *end);
extern int SifCheckStrict(VERIFY_SSS *verify, MM *mm, MM *end);
extern int SsbusCheckStrict(VERIFY_SSS *verify, MM *mm, MM *end);
extern int SMEM_SDRAMCheck(VERIFY_SSS *verify, NN *nn, NN *end);

int SmemDataCheck(VERIFY_SSS *verify, MM *mm)
{
  int i; // [esp+4h] [ebp-1Ch]
  unsigned int ref_DATA; // [esp+8h] [ebp-18h]
  unsigned int DATA; // [esp+Ch] [ebp-14h]
  unsigned int WR; // [esp+14h] [ebp-Ch]
  unsigned int COL; // [esp+18h] [ebp-8h]
  unsigned int ROW; // [esp+1Ch] [ebp-4h]

  WR = (mm->Ama & 0x40000000) != 0;
  DATA = mm->Amd;
  if ( (mm->Ama & 0x80000000) == 0 )
    return 0;
  if ( (mm->Ama & 0x40000000) != 0 )
  {
    ROW = HIWORD(mm->Ama) & 0x7FF;
    LOWORD(COL) = mm->Ama & 0x7FF;
  }
  else
  {
    ROW = verify->bef_ROW;
    COL = verify->bef_COL;
  }
  if ( verify->status )
  {
    if ( (COL & 1) != 0 )
      ref_DATA = -1431655766;
    else
      ref_DATA = 1431655765;
    if ( (_BYTE)COL || DATA != 1431655765 )
    {
      if ( verify->ref_WR == WR && verify->ref_ROW == ROW && verify->ref_COL == (COL & 0x300) && ref_DATA == DATA )
      {
        verify->COL_flag0[(unsigned __int8)COL] = 1;
        if ( (unsigned __int8)COL == 255 )
        {
          for ( i = 0; i <= 255; ++i )
          {
            if ( verify->COL_flag0[i] )
              ++verify->okCount;
            else
              ++verify->ngCount;
            verify->COL_flag0[i] = 0;
          }
          verify->COL_flag1[(unsigned __int16)(COL & 0x300) >> 8] = 1;
          if ( ROW == 682 )
            verify->ROW_flag[0] = 1;
          if ( ROW == 1365 )
            verify->ROW_flag[1] = 1;
        }
      }
    }
    else
    {
      verify->ref_COL = COL & 0x300;
      verify->COL_flag0[0] = 1;
      verify->ref_WR = WR;
      verify->ref_ROW = ROW;
    }
  }
  else if ( !(_BYTE)COL && DATA == 1431655765 )
  {
    verify->ref_COL = COL & 0x300;
    verify->COL_flag0[0] = 1;
    verify->ref_WR = WR;
    verify->ref_ROW = ROW;
    verify->status = 1;
  }
  return 0;
}

int SmemSequenceCheck(VERIFY_SSS *verify, MM *mm)
{
  int type; // eax
  unsigned int ROW; // [esp+8h] [ebp-Ch]
  unsigned int COL; // [esp+Ch] [ebp-8h]
  unsigned int DATA; // [esp+10h] [ebp-4h]

  DATA = mm->Amd;
  if ( (mm->Ama & 0x40000000) != 0 )
  {
    ROW = HIWORD(mm->Ama) & 0x7FF;
    COL = mm->Ama & 0x7FF;
  }
  else
  {
    ROW = verify->bef_ROW;
    COL = verify->bef_COL;
  }
  type = verify->type;
  if ( type == 3 )
  {
    if ( (mm->Ama & 0x40000000) != 0 && !ROW && !COL && DATA == 19088743 )
      verify->status = 201;
  }
  else if ( type > 3 )
  {
    if ( type == 4 )
    {
      if ( (mm->Ama & 0x80000000) != 0 && (mm->Ama & 0x40000000) == 0 && ROW == 80 && verify->ref_COL == COL )
      {
        verify->status = 200;
        ++verify->ref_COL;
      }
      if ( verify->ref_COL == 32 )
        verify->status = 201;
    }
    else if ( type == 5 )
    {
      if ( (mm->Ama & 0x80000000) != 0
        && (mm->Ama & 0x40000000) != 0
        && ROW == 172
        && verify->ref_COL == COL
        && DATA == LOBYTE(verify->ref_COL) )
      {
        verify->status = 200;
        ++verify->ref_COL;
      }
      if ( verify->ref_COL == 512 )
        verify->status = 201;
    }
  }
  else if ( type == 2 && (mm->Ama & 0x40000000) == 0 && !ROW && !COL && !DATA )
  {
    verify->status = 201;
  }
  return verify->status;
}

int SifSequenceCheck(VERIFY_SSS *verify, MM *mm)
{
  int status; // eax
  int v3; // eax
  unsigned int BE; // [esp+0h] [ebp-40h]
  unsigned int ref_RDAC; // [esp+8h] [ebp-38h]
  unsigned int ref_WRAC; // [esp+Ch] [ebp-34h]

  ref_WRAC = 0;
  ref_RDAC = 0;
  BE = (8 * ((mm->Amcas_x0 & 0x4000) != 0)) | (4 * ((mm->Amcas_x0 & 0x2000) != 0)) | ((mm->Amcas_x0 & 0x800) != 0) | (2 * ((mm->Amcas_x0 & 0x1000) != 0));
  switch ( verify->type )
  {
    case 4:
      ref_WRAC = 0;
      ref_RDAC = 1;
      break;
    case 5:
      ref_WRAC = 1;
      ref_RDAC = 0;
      break;
    default:
      break;
  }
  if ( verify->type == 1 || verify->type == 2 || verify->type == 3 )
  {
    status = verify->status;
    if ( status == 102 )
    {
      if ( SLOBYTE(mm->Amcas_x0) >= 0
        && (mm->Amcas_x0 & 0x40) == 0
        && (mm->Amcas_x0 & 0x200) == 0
        && (mm->Amcas_x0 & 0x100) != 0 )
      {
        verify->status = 103;
      }
    }
    else if ( status > 102 )
    {
      if ( status == 103 )
      {
        if ( (mm->Amcas_x0 & 0x40) == 0 && (mm->Amcas_x0 & 0x200) != 0 && (mm->Amcas_x0 & 0x100) == 0 )
          verify->status = 104;
      }
      else if ( status == 104
             && SLOBYTE(mm->Amcas_x0) < 0
             && (mm->Amcas_x0 & 0x40) != 0
             && (mm->Amcas_x0 & 0x200) != 0
             && (mm->Amcas_x0 & 0x100) != 0 )
      {
        verify->status = 105;
      }
    }
    else if ( status )
    {
      if ( status == 101
        && SLOBYTE(mm->Amcas_x0) >= 0
        && (mm->Amcas_x0 & 0x40) == 0
        && (mm->Amcas_x0 & 0x200) != 0
        && (mm->Amcas_x0 & 0x100) != 0 )
      {
        verify->status = 102;
      }
    }
    else if ( SLOBYTE(mm->Amcas_x0) >= 0
           && (mm->Amcas_x0 & 0x40) != 0
           && (mm->Amcas_x0 & 0x200) != 0
           && (mm->Amcas_x0 & 0x100) != 0 )
    {
      verify->status = 101;
    }
  }
  else if ( verify->type == 4 || verify->type == 5 )
  {
    v3 = verify->status;
    if ( v3 == 122 )
    {
      if ( (mm->Amcas_x0 & 8) != 0
        && ref_WRAC == ((mm->Amcas_x0 & 0x200) != 0)
        && ref_RDAC == ((mm->Amcas_x0 & 0x400) != 0)
        && (mm->Amcas_x0 & 0x100) == 0
        && !BE )
      {
        verify->status = 123;
      }
    }
    else if ( v3 > 122 )
    {
      if ( v3 == 123 )
      {
        if ( (mm->Amcas_x0 & 8) == 0
          && (mm->Amcas_x0 & 0x200) != 0
          && (mm->Amcas_x0 & 0x400) != 0
          && (mm->Amcas_x0 & 0x100) != 0
          && !BE )
        {
          verify->status = 124;
        }
      }
      else if ( v3 == 124
             && (mm->Amcas_x0 & 8) == 0
             && (mm->Amcas_x0 & 0x200) != 0
             && (mm->Amcas_x0 & 0x400) != 0
             && (mm->Amcas_x0 & 0x100) != 0
             && BE == 15 )
      {
        verify->status = 125;
      }
    }
    else if ( !v3
           && (mm->Amcas_x0 & 8) != 0
           && (mm->Amcas_x0 & 0x200) != 0
           && (mm->Amcas_x0 & 0x400) != 0
           && (mm->Amcas_x0 & 0x100) != 0
           && BE == 15 )
    {
      verify->status = 122;
    }
  }
  return verify->status;
}

int SsbusSequenceCheckExt(VERIFY_SSS *verify, MM *mm, int xr_w)
{
  int status; // eax
  unsigned int SRD; // [esp+8h] [ebp-10h]
  unsigned int CS; // [esp+10h] [ebp-8h]

  LOBYTE(CS) = ((mm->Amras_x & 0x400000) != 0) & (BYTE2(mm->Amras_x) >> 7) & HIBYTE(mm->Amras_x) & 1 & ((mm->Amras_x & 0x2000000) != 0 && (mm->Amras_x & 0x4000000) != 0 && (mm->Amras_x & 0x10000000) != 0 && (mm->Amras_x & 0x8000000) != 0);
  LOBYTE(SRD) = (mm->Amras_x & 0x10) != 0;
  status = verify->status;
  if ( status == 301 )
  {
    if ( xr_w )
    {
      if ( (mm->Amras_x & 8) == 0
        && (mm->Amras_x & 0x200000) == 0
        && ((unsigned __int8)SRD & (unsigned __int8)CS & ((mm->Amras_x & 0x20) != 0)) != 0 )
      {
        verify->status = 302;
      }
    }
    else if ( (mm->Amras_x & 0x10) == 0
           && (mm->Amras_x & 0x20) == 0
           && (mm->Amras_x & 0x200000) == 0
           && ((unsigned __int8)CS & ((mm->Amras_x & 8) != 0)) != 0 )
    {
      verify->status = 302;
    }
  }
  else if ( status > 301 )
  {
    if ( status == 302
      && (((mm->Amras_x & 8) != 0) & (unsigned __int8)SRD & ((mm->Amras_x & 0x20) != 0) & (unsigned __int8)CS & ((mm->Amras_x & 0x200000) != 0)) != 0 )
    {
      verify->status = 303;
    }
  }
  else if ( !status
         && (mm->Amras_x & 0x200000) == 0
         && (((mm->Amras_x & 0x400000) != 0) & (BYTE2(mm->Amras_x) >> 7) & HIBYTE(mm->Amras_x) & 1 & ((mm->Amras_x & 0x2000000) != 0 && (mm->Amras_x & 0x4000000) != 0 && (mm->Amras_x & 0x10000000) != 0 && (mm->Amras_x & 0x8000000) != 0)) != 0 )
  {
    verify->status = 301;
  }
  return verify->status;
}

int SsbusSequenceCheck(VERIFY_SSS *verify, MM *mm)
{
  int dev; // eax
  int sta_int; // eax
  int sta_ube; // eax
  int sta_wait; // eax
  unsigned int INT; // [esp+8h] [ebp-30h]
  unsigned int DACK; // [esp+Ch] [ebp-2Ch]
  unsigned int DREQ; // [esp+10h] [ebp-28h]
  unsigned int WAIT; // [esp+14h] [ebp-24h]
  unsigned int UBE; // [esp+18h] [ebp-20h]
  unsigned int CS; // [esp+28h] [ebp-10h]
  unsigned int XCS; // [esp+2Ch] [ebp-Ch]
  int v14; // [esp+34h] [ebp-4h]

  v14 = 0;
  UBE = (mm->Amras_x & 4) != 0;
  WAIT = (mm->Amras_x & 2) != 0;
  dev = verify->dev;
  if ( dev == 4 )
  {
    DREQ = 0;
    DACK = (mm->Amras_x & 0x400) != 0;
    XCS = (mm->Amras_x & 0x4000000) != 0;
    LOBYTE(CS) = ((mm->Amras_x & 0x200000) != 0) & ((mm->Amras_x & 0x400000) != 0) & (BYTE2(mm->Amras_x) >> 7) & HIBYTE(mm->Amras_x) & 1 & ((mm->Amras_x & 0x2000000) != 0 && (mm->Amras_x & 0x10000000) != 0 && (mm->Amras_x & 0x8000000) != 0);
    INT = (mm->Amras_x & 0x100000) != 0;
  }
  else if ( dev > 4 )
  {
    if ( dev == 8 )
    {
      DREQ = 0;
      DACK = BYTE1(mm->Amras_x) & 1;
      XCS = HIBYTE(mm->Amras_x) & 1;
      LOBYTE(CS) = ((mm->Amras_x & 0x200000) != 0) & ((mm->Amras_x & 0x400000) != 0) & (BYTE2(mm->Amras_x) >> 7) & ((mm->Amras_x & 0x2000000) != 0 && (mm->Amras_x & 0x4000000) != 0 && (mm->Amras_x & 0x10000000) != 0 && (mm->Amras_x & 0x8000000) != 0);
      INT = (mm->Amras_x & 0x40000) != 0;
    }
    else if ( dev > 8 )
    {
      if ( dev == 9 )
      {
        DREQ = 0;
        DACK = LOBYTE(mm->Amras_x) >> 7;
        XCS = BYTE2(mm->Amras_x) >> 7;
        LOBYTE(CS) = ((mm->Amras_x & 0x200000) != 0) & ((mm->Amras_x & 0x400000) != 0) & HIBYTE(mm->Amras_x) & 1 & ((mm->Amras_x & 0x2000000) != 0 && (mm->Amras_x & 0x4000000) != 0 && (mm->Amras_x & 0x10000000) != 0 && (mm->Amras_x & 0x8000000) != 0);
        INT = (mm->Amras_x & 0x20000) != 0;
      }
      else if ( dev == 90 )
      {
        DREQ = 0;
        DACK = 0;
        XCS = (mm->Amras_x & 0x400000) != 0;
        LOBYTE(CS) = ((mm->Amras_x & 0x200000) != 0) & (BYTE2(mm->Amras_x) >> 7) & HIBYTE(mm->Amras_x) & 1 & ((mm->Amras_x & 0x2000000) != 0 && (mm->Amras_x & 0x4000000) != 0 && (mm->Amras_x & 0x10000000) != 0 && (mm->Amras_x & 0x8000000) != 0);
        INT = 0;
      }
    }
    else if ( dev == 5 )
    {
      DREQ = 0;
      DACK = (mm->Amras_x & 0x200) != 0;
      XCS = (mm->Amras_x & 0x2000000) != 0;
      LOBYTE(CS) = ((mm->Amras_x & 0x200000) != 0) & ((mm->Amras_x & 0x400000) != 0) & (BYTE2(mm->Amras_x) >> 7) & HIBYTE(mm->Amras_x) & 1 & ((mm->Amras_x & 0x4000000) != 0 && (mm->Amras_x & 0x10000000) != 0 && (mm->Amras_x & 0x8000000) != 0);
      INT = (mm->Amras_x & 0x80000) != 0;
    }
  }
  else if ( dev == 1 )
  {
    DREQ = 0;
    DACK = 0;
    XCS = (mm->Amras_x & 0x8000000) != 0;
    LOBYTE(CS) = ((mm->Amras_x & 0x200000) != 0) & ((mm->Amras_x & 0x400000) != 0) & (BYTE2(mm->Amras_x) >> 7) & HIBYTE(mm->Amras_x) & 1 & ((mm->Amras_x & 0x2000000) != 0 && (mm->Amras_x & 0x4000000) != 0 && (mm->Amras_x & 0x10000000) != 0);
    INT = 0;
  }
  else if ( dev > 1 )
  {
    if ( dev == 2 )
    {
      DREQ = 0;
      DACK = 0;
      XCS = (mm->Amras_x & 0x10000000) != 0;
      LOBYTE(CS) = ((mm->Amras_x & 0x200000) != 0) & ((mm->Amras_x & 0x400000) != 0) & (BYTE2(mm->Amras_x) >> 7) & HIBYTE(mm->Amras_x) & 1 & ((mm->Amras_x & 0x2000000) != 0 && (mm->Amras_x & 0x4000000) != 0 && (mm->Amras_x & 0x8000000) != 0);
      INT = 0;
    }
  }
  else if ( !dev )
  {
    DREQ = 0;
    DACK = (mm->Amras_x & 0x40) != 0;
    XCS = (mm->Amras_x & 0x200000) != 0;
    LOBYTE(CS) = ((mm->Amras_x & 0x400000) != 0) & (BYTE2(mm->Amras_x) >> 7) & HIBYTE(mm->Amras_x) & 1 & ((mm->Amras_x & 0x2000000) != 0 && (mm->Amras_x & 0x4000000) != 0 && (mm->Amras_x & 0x10000000) != 0 && (mm->Amras_x & 0x8000000) != 0);
    INT = BYTE2(mm->Amras_x) & 1;
  }
  switch ( verify->status )
  {
    case 0:
      if ( (((mm->Amras_x & 0x10) != 0) & ((mm->Amras_x & 0x20) != 0) & (unsigned __int8)CS & (unsigned __int8)XCS & (DACK == 0 && DREQ == 0) & ((mm->Amras_x & 8) != 0)) == 0 )
      {
        verify->status = 90;
        verify->cnt = 1;
      }
      ++verify->cnt_case[0];
      break;
    case 1:
      if ( !XCS && (DACK & 1) != 0 )
      {
        verify->status = 3;
        verify->cnt_case[3] = 1;
      }
      else if ( XCS == 0 && DACK == 0 )
      {
        verify->status = 22;
        verify->cnt_case[22] = 1;
      }
      ++verify->cnt_case[1];
      break;
    case 3:
      if ( (mm->Amras_x & 8) != 0 )
      {
        if ( (mm->Amras_x & 0x10) == 0 && (mm->Amras_x & 0x20) == 0 )
        {
          verify->status = 15;
          verify->cnt_case[15] = 1;
        }
        else if ( verify->cnt_case[3] <= 19 )
        {
          if ( (((mm->Amras_x & 0x10) != 0) & ((mm->Amras_x & 0x20) != 0) & (unsigned __int8)CS & (unsigned __int8)XCS & (DACK == 0 && DREQ == 0) & ((mm->Amras_x & 8) != 0)) != 0 )
            v14 = 1;
        }
        else
        {
          v14 = 1;
        }
      }
      else
      {
        verify->status = 5;
        verify->cnt_case[5] = 1;
      }
      ++verify->cnt_case[3];
      break;
    case 5:
      if ( (mm->Amras_x & 8) != 0 )
      {
        verify->status = 6;
        verify->cnt_case[6] = 1;
      }
      else if ( (((mm->Amras_x & 0x10) != 0) & ((mm->Amras_x & 0x20) != 0) & (unsigned __int8)CS & (unsigned __int8)XCS & (DACK == 0 && DREQ == 0) & ((mm->Amras_x & 8) != 0)) != 0 )
      {
        v14 = 1;
      }
      ++verify->cnt_case[5];
      break;
    case 6:
      if ( XCS )
      {
        verify->status = 7;
        verify->cnt_case[7] = 1;
      }
      else if ( (mm->Amras_x & 8) != 0 )
      {
        if ( (((mm->Amras_x & 0x10) != 0) & ((mm->Amras_x & 0x20) != 0) & (unsigned __int8)CS & (unsigned __int8)XCS & (DACK == 0 && DREQ == 0) & ((mm->Amras_x & 8) != 0)) != 0 )
          v14 = 1;
      }
      else
      {
        verify->status = 5;
        verify->cnt_case[5] = 1;
      }
      ++verify->cnt_case[6];
      break;
    case 7:
      if ( (((mm->Amras_x & 0x10) != 0) & ((mm->Amras_x & 0x20) != 0) & (unsigned __int8)CS & (unsigned __int8)XCS & (DACK == 0 && DREQ == 0) & ((mm->Amras_x & 8) != 0)) != 0 )
      {
        verify->status = 1;
        verify->cnt_case[1] = 1;
        ++verify->cnt_dmaw;
      }
      else if ( verify->cnt_case[7] > 1 )
      {
        v14 = 1;
      }
      ++verify->cnt_case[7];
      break;
    case 0xF:
      if ( (mm->Amras_x & 0x10) != 0 )
      {
        verify->status = 16;
        verify->cnt_case[16] = 1;
      }
      else if ( (((mm->Amras_x & 0x10) != 0) & ((mm->Amras_x & 0x20) != 0) & (unsigned __int8)CS & (unsigned __int8)XCS & (DACK == 0 && DREQ == 0) & ((mm->Amras_x & 8) != 0)) != 0 )
      {
        v14 = 1;
      }
      ++verify->cnt_case[15];
      break;
    case 0x10:
      if ( !DACK && (XCS & 1) != 0 )
      {
        verify->status = 17;
        verify->cnt_case[17] = 1;
      }
      else if ( (mm->Amras_x & 0x10) != 0 )
      {
        if ( (((mm->Amras_x & 0x10) != 0) & ((mm->Amras_x & 0x20) != 0) & (unsigned __int8)CS & (unsigned __int8)XCS & (DACK == 0 && DREQ == 0) & ((mm->Amras_x & 8) != 0)) != 0 )
          v14 = 1;
      }
      else
      {
        verify->status = 15;
        verify->cnt_case[15] = 1;
      }
      ++verify->cnt_case[16];
      break;
    case 0x11:
      if ( (((mm->Amras_x & 0x10) != 0) & ((mm->Amras_x & 0x20) != 0) & (unsigned __int8)CS & (unsigned __int8)XCS & (DACK == 0 && DREQ == 0) & ((mm->Amras_x & 8) != 0)) != 0 )
      {
        verify->status = 1;
        verify->cnt_case[1] = 1;
        ++verify->cnt_dmar;
      }
      else if ( verify->cnt_case[17] > 0 )
      {
        v14 = 1;
      }
      ++verify->cnt_case[17];
      break;
    case 0x16:
      if ( (mm->Amras_x & 8) != 0 )
      {
        if ( (mm->Amras_x & 0x20) != 0 )
        {
          if ( verify->cnt_case[22] <= 1 )
          {
            if ( (((mm->Amras_x & 0x10) != 0) & ((mm->Amras_x & 0x20) != 0) & (unsigned __int8)CS & (unsigned __int8)XCS & (DACK == 0 && DREQ == 0) & ((mm->Amras_x & 8) != 0)) != 0 )
            {
              verify->status = 1;
              verify->cnt_case[1] = 1;
            }
          }
          else
          {
            v14 = 1;
          }
        }
        else
        {
          verify->status = 33;
          verify->cnt_case[33] = 1;
        }
      }
      else
      {
        verify->status = 23;
        verify->cnt_case[23] = 1;
      }
      ++verify->cnt_case[22];
      break;
    case 0x17:
      if ( (mm->Amras_x & 8) != 0 )
      {
        verify->status = 24;
        verify->cnt_case[24] = 1;
      }
      else if ( (((mm->Amras_x & 0x10) != 0) & ((mm->Amras_x & 0x20) != 0) & (unsigned __int8)CS & (unsigned __int8)XCS & (DACK == 0 && DREQ == 0) & ((mm->Amras_x & 8) != 0)) != 0 )
      {
        v14 = 1;
      }
      ++verify->cnt_case[23];
      break;
    case 0x18:
      if ( (((mm->Amras_x & 0x10) != 0) & ((mm->Amras_x & 0x20) != 0) & (unsigned __int8)CS & (unsigned __int8)XCS & (DACK == 0 && DREQ == 0) & ((mm->Amras_x & 8) != 0)) != 0 )
      {
        verify->status = 1;
        verify->cnt_case[1] = 1;
        ++verify->cnt_wr;
      }
      else if ( (mm->Amras_x & 8) == 0 )
      {
        verify->status = 23;
        verify->cnt_case[23] = 1;
      }
      ++verify->cnt_case[24];
      break;
    case 0x21:
      if ( (mm->Amras_x & 0x10) != 0 )
      {
        if ( (((mm->Amras_x & 0x10) != 0) & ((mm->Amras_x & 0x20) != 0) & (unsigned __int8)CS & (unsigned __int8)XCS & (DACK == 0 && DREQ == 0) & ((mm->Amras_x & 8) != 0)) != 0 )
          v14 = 1;
      }
      else
      {
        verify->status = 34;
        verify->cnt_case[34] = 1;
      }
      ++verify->cnt_case[33];
      break;
    case 0x22:
      if ( (mm->Amras_x & 0x10) != 0 )
      {
        verify->status = 35;
        verify->cnt_case[35] = 1;
      }
      else if ( (((mm->Amras_x & 0x10) != 0) & ((mm->Amras_x & 0x20) != 0) & (unsigned __int8)CS & (unsigned __int8)XCS & (DACK == 0 && DREQ == 0) & ((mm->Amras_x & 8) != 0)) != 0 )
      {
        v14 = 1;
      }
      ++verify->cnt_case[34];
      break;
    case 0x23:
      if ( XCS )
      {
        verify->status = 36;
        verify->cnt_case[36] = 1;
      }
      else if ( (mm->Amras_x & 0x10) != 0 )
      {
        if ( (((mm->Amras_x & 0x10) != 0) & ((mm->Amras_x & 0x20) != 0) & (unsigned __int8)CS & (unsigned __int8)XCS & (DACK == 0 && DREQ == 0) & ((mm->Amras_x & 8) != 0)) != 0 )
          v14 = 1;
      }
      else
      {
        verify->status = 34;
        verify->cnt_case[34] = 1;
      }
      ++verify->cnt_case[35];
      break;
    case 0x24:
      if ( (mm->Amras_x & 0x10) != 0 )
      {
        verify->status = 1;
        verify->cnt_case[1] = 1;
        ++verify->cnt_rd;
      }
      else if ( verify->cnt_case[36] > 0 )
      {
        v14 = 1;
      }
      ++verify->cnt_case[36];
      break;
    case 0x5A:
      if ( (((mm->Amras_x & 0x10) != 0) & ((mm->Amras_x & 0x20) != 0) & (unsigned __int8)CS & (unsigned __int8)XCS & (DACK == 0 && DREQ == 0) & ((mm->Amras_x & 8) != 0)) != 0 )
      {
        verify->status = 1;
        verify->cnt_case[1] = 1;
      }
      ++verify->cnt_case[91];
      break;
    default:
      break;
  }
  sta_int = verify->sta_int;
  if ( sta_int )
  {
    if ( sta_int == 1 && !INT )
    {
      verify->sta_int = 0;
      ++verify->cnt_int;
    }
  }
  else if ( INT )
  {
    verify->sta_int = 1;
  }
  sta_ube = verify->sta_ube;
  if ( sta_ube )
  {
    if ( sta_ube == 1 && !UBE )
    {
      verify->sta_ube = 0;
      ++verify->cnt_ube;
    }
  }
  else if ( UBE )
  {
    verify->sta_ube = 1;
  }
  sta_wait = verify->sta_wait;
  if ( sta_wait )
  {
    if ( sta_wait == 1 && !WAIT )
    {
      verify->sta_wait = 0;
      ++verify->cnt_wait;
    }
  }
  else if ( WAIT )
  {
    verify->sta_wait = 1;
  }
  if ( v14 == 1 )
  {
    printf("\n");
    printf("find illegal sequence\n");
    printf("=== NG === SSBUS %s (dev %d)\n", verify->diagname, verify->dev);
  }
  return v14;
}

int SmemCheckStrict(VERIFY_SSS *verify, MM *mm, MM *end)
{
  while ( end > mm )
  {
    SmemDataCheck(verify, mm);
    verify->bef_ROW = HIWORD(mm->Ama) & 0x7FF;
    verify->bef_COL = mm->Ama & 0x7FF;
    ++verify->cnt;
    mm = (MM *)((char *)mm + 16);
  }
  return !verify->ngCount
      && verify->ROW_flag[0]
      && verify->ROW_flag[1]
      && verify->COL_flag1[0]
      && verify->COL_flag1[1]
      && verify->COL_flag1[2]
      && verify->COL_flag1[3];
}

int SifCheckStrict(VERIFY_SSS *verify, MM *mm, MM *end)
{
  int step; // eax

  while ( end > mm )
  {
    step = verify->step;
    if ( step == 31 )
    {
      verify->status = 0;
      if ( SmemSequenceCheck(verify, mm) == 201 )
      {
        verify->step = 40;
        verify->status = 0;
        verify->type = 4;
      }
    }
    else if ( step > 31 )
    {
      if ( step == 50 )
      {
        if ( SmemSequenceCheck(verify, mm) == 201 )
        {
          verify->step = 51;
          verify->status = 0;
        }
      }
      else if ( step > 50 )
      {
        if ( step == 51 )
        {
          if ( SifSequenceCheck(verify, mm) == 125 )
            verify->step = 999;
        }
        else if ( step == 999 )
        {
          return 1;
        }
      }
      else if ( step == 40 )
      {
        if ( SifSequenceCheck(verify, mm) == 125 )
        {
          verify->step = 41;
          verify->status = 0;
          verify->ref_COL = 16;
        }
      }
      else if ( step == 41 && SmemSequenceCheck(verify, mm) == 201 )
      {
        verify->step = 50;
        verify->status = 0;
        verify->ref_COL = 256;
        verify->type = 5;
      }
    }
    else if ( step == 20 )
    {
      if ( SmemSequenceCheck(verify, mm) == 201 )
      {
        verify->step = 21;
        verify->status = 0;
      }
    }
    else if ( step > 20 )
    {
      if ( step == 21 )
      {
        if ( SifSequenceCheck(verify, mm) == 105 )
        {
          verify->step = 30;
          verify->status = 0;
          verify->type = 3;
        }
      }
      else if ( step == 30 && SifSequenceCheck(verify, mm) == 105 )
      {
        verify->step = 31;
        verify->status = 0;
      }
    }
    else if ( step == 10 )
    {
      if ( SifSequenceCheck(verify, mm) == 105 )
      {
        verify->step = 11;
        verify->status = 0;
      }
    }
    else if ( step == 11 && SsbusSequenceCheckExt(verify, mm, 1) == 303 )
    {
      verify->step = 20;
      verify->status = 0;
      verify->type = 2;
    }
    verify->bef_ROW = HIWORD(mm->Ama) & 0x7FF;
    verify->bef_COL = mm->Ama & 0x7FF;
    ++verify->cnt;
    mm = (MM *)((char *)mm + 16);
  }
  return 0;
}

int SsbusCheckStrict(VERIFY_SSS *verify, MM *mm, MM *end)
{
  while ( end > mm )
  {
    if ( SsbusSequenceCheck(verify, mm) )
      return 1;
    ++verify->cnt;
    mm = (MM *)((char *)mm + 16);
  }
  return 0;
}

int SMEM_SDRAMCheck(VERIFY_SSS *verify, NN *nn, NN *end)
{
  int dev; // eax
  int status; // [esp+0h] [ebp-4h]

  status = 0;
  if ( verify->dev == 1 && m_init )
  {
    status = m_sdram & 1;
    m_init = 0;
  }
  else if ( verify->dev == 2 && m_init )
  {
    m_sdram = (unsigned __int16)m_sdram;
    if ( (unsigned __int16)m_sdram == 0xFFFF )
      m_sdram = 0;
    else
      m_sdram = (m_sdram | (m_sdram << 16)) + 65537;
    m_init = 0;
  }
  else if ( verify->dev == 3 && m_init )
  {
    m_sdram = (unsigned __int8)m_sdram;
    if ( (unsigned __int8)m_sdram == 255 )
      m_sdram = 0;
    else
      m_sdram = (m_sdram | (m_sdram << 8) | (m_sdram << 16) | (m_sdram << 24)) + 16843009;
    m_init = 0;
  }
  while ( end > nn )
  {
    dev = verify->dev;
    if ( dev == 1 )
    {
      if ( (status & 1) != 0 )
      {
        if ( nn->sdram_data1 != -1431677611 )
        {
          if ( verify->ngCount <= 9 )
            printf("sdram_data1 = %x %d\n", nn->sdram_data1, status);
          ++verify->ngCount;
        }
        if ( nn->sdram_data2 != -1431677611 )
        {
          if ( verify->ngCount <= 9 )
            printf("sdram_data2 = %x %d\n", nn->sdram_data2, status);
          ++verify->ngCount;
        }
        if ( nn->sdram_data3 != -1431677611 )
        {
          if ( verify->ngCount <= 9 )
            printf("sdram_data3 = %x %d\n", nn->sdram_data3, status);
          ++verify->ngCount;
        }
        if ( nn->sdram_data4 != -1431677611 )
        {
          if ( verify->ngCount <= 9 )
            printf("sdram_data4 = %x %d\n", nn->sdram_data4, status);
          ++verify->ngCount;
        }
      }
      else
      {
        if ( nn->sdram_data1 != 1431677610 )
        {
          if ( verify->ngCount <= 9 )
            printf("sdram_data1 = %x %d\n", nn->sdram_data1, status);
          ++verify->ngCount;
        }
        if ( nn->sdram_data2 != 1431677610 )
        {
          if ( verify->ngCount <= 9 )
            printf("sdram_data2 = %x %d\n", nn->sdram_data2, status);
          ++verify->ngCount;
        }
        if ( nn->sdram_data3 != 1431677610 )
        {
          if ( verify->ngCount <= 9 )
            printf("sdram_data3 = %x %d\n", nn->sdram_data3, status);
          ++verify->ngCount;
        }
        if ( nn->sdram_data4 != 1431677610 )
        {
          if ( verify->ngCount <= 9 )
            printf("sdram_data4 = %x %d\n", nn->sdram_data4, status);
          ++verify->ngCount;
        }
      }
      ++status;
    }
    else if ( dev > 1 )
    {
      if ( dev == 2 )
      {
        if ( m_sdram != nn->sdram_data1 )
        {
          if ( verify->ngCount <= 9 )
            printf("sdram_data1 = %x %x\n", nn->sdram_data1, m_sdram);
          ++verify->ngCount;
        }
        if ( m_sdram != nn->sdram_data2 )
        {
          if ( verify->ngCount <= 9 )
            printf("sdram_data2 = %x %x\n", nn->sdram_data2, m_sdram);
          ++verify->ngCount;
        }
        if ( m_sdram != nn->sdram_data3 )
        {
          if ( verify->ngCount <= 9 )
            printf("sdram_data3 = %x %x\n", nn->sdram_data3, m_sdram);
          ++verify->ngCount;
        }
        if ( m_sdram != nn->sdram_data4 )
        {
          if ( verify->ngCount <= 9 )
            printf("sdram_data4 = %x %x\n", nn->sdram_data4, m_sdram);
          ++verify->ngCount;
        }
        if ( m_sdram == -1 )
          m_sdram = 0;
        else
          m_sdram += 65537;
      }
      else if ( dev == 3 )
      {
        if ( m_sdram != nn->sdram_data1 )
        {
          if ( verify->ngCount <= 9 )
            printf("sdram_data1 = %x %x\n", nn->sdram_data1, m_sdram);
          ++verify->ngCount;
        }
        if ( m_sdram != nn->sdram_data2 )
        {
          if ( verify->ngCount <= 9 )
            printf("sdram_data2 = %x %x\n", nn->sdram_data2, m_sdram);
          ++verify->ngCount;
        }
        if ( m_sdram != nn->sdram_data3 )
        {
          if ( verify->ngCount <= 9 )
            printf("sdram_data3 = %x %x\n", nn->sdram_data3, m_sdram);
          ++verify->ngCount;
        }
        if ( m_sdram != nn->sdram_data4 )
        {
          if ( verify->ngCount <= 9 )
            printf("sdram_data4 = %x %x\n", nn->sdram_data4, m_sdram);
          ++verify->ngCount;
        }
        if ( m_sdram == -1 )
          m_sdram = 0;
        else
          m_sdram += 16843009;
      }
    }
    else if ( !dev )
    {
      if ( nn->sdram_data1 != -1 )
      {
        if ( verify->ngCount <= 9 )
          printf("sdram_data1 = %x %d\n", nn->sdram_data1, status);
        ++verify->ngCount;
      }
      if ( nn->sdram_data2 != -1 )
      {
        if ( verify->ngCount <= 9 )
          printf("sdram_data2 = %x %d\n", nn->sdram_data2, status);
        ++verify->ngCount;
      }
      if ( nn->sdram_data3 != -1 )
      {
        if ( verify->ngCount <= 9 )
          printf("sdram_data3 = %x %d\n", nn->sdram_data3, status);
        ++verify->ngCount;
      }
      if ( nn->sdram_data4 != -1 )
      {
        if ( verify->ngCount <= 9 )
          printf("sdram_data4 = %x %d\n", nn->sdram_data4, status);
        ++verify->ngCount;
      }
    }
    ++nn;
  }
  return 0;
}

int check_sss(VERIFY_SSS *verify, char *p, int size, char **av)
{
  MM *end; // [esp+8h] [ebp-Ch]
  int qword; // [esp+10h] [ebp-4h]

  qword = 1;
  end = (MM *)&p[size];
  switch ( verify->bus_type )
  {
    case 1:
      return SsbusCheckStrict(verify, (MM *)p, end);
    case 2:
      return SifCheckStrict(verify, (MM *)p, end);
    case 3:
      return SmemCheckStrict(verify, (MM *)p, end);
    case 4:
      verify->dev -= 30;
      return SMEM_SDRAMCheck(verify, (NN *)p, (NN *)&p[size]);
  }
  return qword;
}

int SetBusType(VERIFY_SSS *verify, char **av)
{
  int bus_type; // eax
  int i; // [esp+0h] [ebp-4h]

  verify->diagname = av[1];
  verify->dev = atoi(av[2]);
  if ( verify->dev >= 0 && verify->dev <= 9 || verify->dev == 90 )
  {
    verify->bus_type = 1;
  }
  else if ( verify->dev == 10 )
  {
    verify->bus_type = 2;
  }
  else if ( verify->dev == 20 )
  {
    verify->bus_type = 3;
  }
  else
  {
    if ( verify->dev <= 29 || verify->dev > 33 )
    {
      printf("illegal check verify->device No.: %d\n", verify->dev);
      printf("   0-9, 90 : SSBUS\n");
      printf("   10      : SIF\n");
      printf("   20      : SMEM\n");
      printf("   30-33   : SDRAM\n");
      return -1;
    }
    verify->bus_type = 4;
  }
  for ( i = 5; av[i]; ++i )
  {
    bus_type = verify->bus_type;
    if ( bus_type != 2 && bus_type <= 2 && bus_type == 1 )
    {
      if ( !strcmp(av[i], "WR") )
      {
        verify->flg_wr = 1;
      }
      else if ( !strcmp(av[i], "RD") )
      {
        verify->flg_rd = 1;
      }
      else if ( !strcmp(av[i], "DMAW") )
      {
        verify->flg_dmaw = 1;
      }
      else if ( !strcmp(av[i], "DMAR") )
      {
        verify->flg_dmar = 1;
      }
      else if ( !strcmp(av[i], "INT") )
      {
        verify->flg_int = 1;
      }
      else if ( !strcmp(av[i], "UBE") )
      {
        verify->flg_ube = 1;
      }
      else if ( !strcmp(av[i], "WAIT") )
      {
        verify->flg_wait = 1;
      }
      else
      {
        if ( strcmp(av[i], "ALL") )
        {
          printf("illegal option : %s\n\n", av[i]);
          printf("   WR   : check SSBUS WRITE     sequence\n");
          printf("   RD   : check SSBUS READ      sequence\n");
          printf("   DMAW : check SSBUS DMA-WRITE sequence\n");
          printf("   DMAR : check SSBUS DMA-READ  sequence\n");
          printf("   INT  : check SSBUS INT       sequence\n");
          printf("   UBE  : check SSBUS UBE       sequence\n");
          printf("   WAIT : check SSBUS WAIT      sequence\n");
          printf("   ALL  : check SSBUS ALL       sequence\n");
          return -2;
        }
        verify->flg_wr = 1;
        verify->flg_rd = 1;
        verify->flg_dmaw = 1;
        verify->flg_dmar = 1;
        verify->flg_int = 1;
        verify->flg_ube = 1;
        verify->flg_wait = 1;
      }
    }
  }
  return 0;
}

int result_sss(VERIFY_SSS *verify)
{
  int bus_type; // eax
  int cnt_ng; // [esp+0h] [ebp-4h]

  cnt_ng = 0;
  bus_type = verify->bus_type;
  if ( bus_type == 2 )
    return verify->step != 999;
  if ( bus_type > 2 )
  {
    if ( bus_type != 3 )
    {
      if ( bus_type == 4 )
        return verify->ngCount != 0;
      return 1;
    }
    if ( !verify->ngCount
      && verify->ROW_flag[0]
      && verify->ROW_flag[1]
      && verify->COL_flag1[0]
      && verify->COL_flag1[1]
      && verify->COL_flag1[2]
      && verify->COL_flag1[3] )
    {
      return 0;
    }
    else
    {
      printf("\n");
      if ( verify->ngCount )
        printf("NG_COUNT     = %d\n", verify->ngCount);
      if ( !verify->ROW_flag[0] )
        printf("verify->ROW_flag[0]  = %d\n", verify->ROW_flag[0]);
      if ( !verify->ROW_flag[1] )
        printf("verify->ROW_flag[1]  = %d\n", verify->ROW_flag[1]);
      if ( !verify->COL_flag1[0] )
        printf("verify->COL_flag1[0] = %d\n", verify->COL_flag1[0]);
      if ( !verify->COL_flag1[1] )
        printf("verify->COL_flag1[1] = %d\n", verify->COL_flag1[1]);
      if ( !verify->COL_flag1[2] )
        printf("verify->COL_flag1[2] = %d\n", verify->COL_flag1[2]);
      if ( !verify->COL_flag1[3] )
        printf("verify->COL_flag1[3] = %d\n", verify->COL_flag1[3]);
      return 1;
    }
  }
  else
  {
    if ( bus_type != 1 )
      return 1;
    if ( verify->status )
    {
      if ( verify->flg_wr && !verify->cnt_wr )
      {
        printf("can not find   WRITE   sequence\n");
        cnt_ng = 1;
      }
      if ( verify->flg_rd && !verify->cnt_rd )
      {
        printf("can not find   READ    sequence\n");
        ++cnt_ng;
      }
      if ( verify->flg_dmaw && !verify->cnt_dmaw )
      {
        printf("can not find DMA-WRITE sequence\n");
        ++cnt_ng;
      }
      if ( verify->flg_dmar && !verify->cnt_dmar )
      {
        printf("can not find DMA-READ  sequence\n");
        ++cnt_ng;
      }
      if ( verify->flg_int && !verify->cnt_int )
      {
        printf("can not find   INT     sequence\n");
        ++cnt_ng;
      }
      if ( verify->flg_ube && !verify->cnt_ube )
      {
        printf("can not find   UBE     sequence\n");
        ++cnt_ng;
      }
      if ( verify->flg_wait )
      {
        if ( !verify->cnt_wait )
        {
          printf("can not find   WAIT    sequence\n");
          ++cnt_ng;
        }
      }
    }
    else
    {
      printf("can not find DEFAULT condition\n");
      cnt_ng = 1;
    }
    return cnt_ng != 0;
  }
}

