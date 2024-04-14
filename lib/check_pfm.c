#include "dsnet_prototypes.h"

typedef eegsdata_t MM;
typedef struct {unsigned int sdram_data1; unsigned int sdram_data2; unsigned int sdram_data3; unsigned int sdram_data4;} NN;

extern int IsSend(eegsdata_t *mm, int type);
extern int IsPrim(eegsdata_t *mm);
extern int IsMEMIF(eegsdata_t *mm, int mmd);
extern int GetDDAValidPixel(eegsdata_t *mm);
extern int GetPgn(eegsdata_t *mm);
extern int IsDDA(eegsdata_t *mm, int pgn);
extern int GetReadDRAM(eegsdata_t *mm);
extern int GetReadDRAM_bit(eegsdata_t *mm);
extern int GetWriteDRAM(eegsdata_t *mm);
extern int GetWriteDRAM_bit(eegsdata_t *mm);
extern int GetPageMiss(eegsdata_t *mm);
extern int GetPageMiss_bit(eegsdata_t *mm);
extern int GetTransSRAM(eegsdata_t *mm);
extern int GetTransSRAM_bit(eegsdata_t *mm);
extern int GetReadSRAM(eegsdata_t *mm);
extern int IsTransClut(eegsdata_t *mm);
extern int IsCrtDisplay(eegsdata_t *mm);
extern int GetOutValidPixel(eegsdata_t *mm);
extern int GetOutValidPixel_bit(eegsdata_t *mm);
extern int IsAfterdsresetCheck(eegsdata_t *mm);
extern int IsAfterdsreset(eegsdata_t *mm, eegsdata_t *end, int *opt);
extern int IsAfterGSResetCheck(eegsdata_t *mm);
extern int IsAfterGSReset(eegsdata_t *mm, eegsdata_t *end, int *opt);
extern int IsPath(eegsdata_t *mm, int path);
extern void InitGS(gs_pa *gs);
extern void UpdateGS(eegsdata_t *mm, gs_pa *gs);
extern void print_gs(gs_pa *gs);
extern int SRV_CheckStrict(eegsdata_t *mm, eegsdata_t *end, int *opt);
extern int TRD_Check(eegsdata_t *mm, eegsdata_t *end, int *opt);
extern int TRD_CheckStrict(eegsdata_t *mm, eegsdata_t *end, int *opt);
extern int OUTPIXEL_Check(eegsdata_t *mm, eegsdata_t *end, int *opt);
extern int OUTPIXEL_CheckStrict(eegsdata_t *mm, eegsdata_t *end, int *opt);
extern int TTR_CheckStrict(eegsdata_t *mm, eegsdata_t *end, int *opt);
extern int CTR_CheckStrict(eegsdata_t *mm, eegsdata_t *end, int *opt);
extern int PMS_Check(eegsdata_t *mm, eegsdata_t *end, int *opt);
extern int PMS_CheckStrict(eegsdata_t *mm, eegsdata_t *end, int *opt);
extern int MRD_CheckStrict(eegsdata_t *mm, eegsdata_t *end, int *opt);
extern int MWT_CheckStrict(eegsdata_t *mm, eegsdata_t *end, int *opt);
extern int PWT_CheckStrict(eegsdata_t *mm, eegsdata_t *end, int *opt);
extern int PGN_Check(eegsdata_t *mm, eegsdata_t *end, int *opt);
extern int PGN_CheckStrict(eegsdata_t *mm, eegsdata_t *end, int *opt);
extern int DISP_Check(VERIFY_PFM *verify, eegsdata_t *mm, eegsdata_t *end, int *opt);
extern int PrintGS(eegsdata_t *mm, eegsdata_t *end, int *opt);
extern int IsHostLocal(eegsdata_t *mm, eegsdata_t *end, int *opt);
extern int IsLocalHost(eegsdata_t *mm, eegsdata_t *end, int *opt);
extern int IsLocalLocal(eegsdata_t *mm, eegsdata_t *end, int *opt);
extern int IsSprite(eegsdata_t *mm, eegsdata_t *end, int *opt);
extern int PFM_SDRAMCheck(NN *nn, NN *end, int opt);
extern int IsEEPerform(eegsdata_t *mm, eegsdata_t *end, int *opt);
extern int Bist_Calculation(int n, int cnt, int mode);

static int status_116 = 0;
static int status_120 = 0;
static gs_pa gs_120;
static gs_pa gs_121;
static int a01_128;
static int a02_129;
static int a03_130;
static int a04_131;
static int a05_132;
static int a06_133;
static int a07_134;
static int a08_135;
static int a09_136;
static int a10_137;
static int a11_138;
static int a12_139;
static int a13_140;
static int a14_141;
static int a15_142;
static int a16_143;
static int b01_144;
static int b02_145;
static int b03_146;
static int b04_147;
static int b05_148;
static int b06_149;
static int b07_150;
static int b08_151;
static int b09_152;
static int b10_153;
static int b11_154;
static int b12_155;
static int b13_156;
static int b14_157;
static int b15_158;
static int b16_159;
static int c01_160;
static int c02_161;
static int c03_162;
static int c04_163;
static int c05_164;
static int c06_165;
static int c07_166;
static int c08_167;
static int c09_168;
static int c10_169;
static int c11_170;
static int c12_171;
static int c13_172;
static int c14_173;
static int c15_174;
static int c16_175;
static int d01_176;
static int d02_177;
static int d03_178;
static int m_179;
static int m01_180;
static int m02_181;
static int m03_182;
static int m04_183;
static int m05_184;
static int m06_185;
static int m07_186;
static int m08_187;
static int m09_188;
static int m10_189;
static int m11_190;
static int m12_191;
static int m13_192;
static int m14_193;
static int m15_194;
static int m16_195;

int IsSend(eegsdata_t *mm, int type)
{
  return (mm->Ap1a & 4) == 0
      && type == ((32 * ((mm->Ap1a & 2) != 0)) | (16 * (mm->Ap1a & 1)) | (8 * (mm->Ap0b >> 7)) | (4
                                                                                                * ((mm->Ap0b & 0x40) != 0)) | (2 * ((mm->Ap0b & 0x20) != 0)) | ((mm->Ap0b & 0x10) != 0));
}

int IsPrim(eegsdata_t *mm)
{
  return mm->Aja & 1;
}

int IsMEMIF(eegsdata_t *mm, int mmd)
{
  return ((4 * (mm->Atpce & 1)) | (2 * (mm->Ajb >> 7)) | ((mm->Ajb & 0x40) != 0)) == mmd;
}

int GetDDAValidPixel(eegsdata_t *mm)
{
  int pgn; // [esp+4h] [ebp-4h]

  pgn = (16 * (mm->Ap1b >> 7)) | (8 * ((mm->Ap1b & 0x40) != 0)) | (4 * ((mm->Ap1b & 0x20) != 0)) | (2
                                                                                                  * ((mm->Ap1b & 0x10) != 0)) | ((mm->Ap1b & 8) != 0);
  if ( pgn <= 0 || pgn > 16 )
    return 0;
  else
    return (16 * (mm->Ap1b >> 7)) | (8 * ((mm->Ap1b & 0x40) != 0)) | (4 * ((mm->Ap1b & 0x20) != 0)) | (2 * ((mm->Ap1b & 0x10) != 0)) | ((mm->Ap1b & 8) != 0);
}

int GetPgn(eegsdata_t *mm)
{
  return (16 * (mm->Ap1b >> 7)) | (8 * ((mm->Ap1b & 0x40) != 0)) | (4 * ((mm->Ap1b & 0x20) != 0)) | (2 * ((mm->Ap1b & 0x10) != 0)) | ((mm->Ap1b & 8) != 0);
}

int IsDDA(eegsdata_t *mm, int pgn)
{
  return pgn == ((16 * (mm->Ap1b >> 7)) | (8 * ((mm->Ap1b & 0x40) != 0)) | (4 * ((mm->Ap1b & 0x20) != 0)) | (2 * ((mm->Ap1b & 0x10) != 0)) | ((mm->Ap1b & 8) != 0));
}

int GetReadDRAM(eegsdata_t *mm)
{
  return ((mm->Atpc0 ^ 1) & 1) + !(mm->Atpce >> 7) + (((mm->Aja >> 2) ^ 1) & 1) + (((mm->Aja >> 1) ^ 1) & 1);
}

int GetReadDRAM_bit(eegsdata_t *mm)
{
  int v1; // ecx
  int v2; // edx
  int result; // eax

  v1 = ((mm->Aja >> 1) ^ 1) & 1;
  if ( (mm->Aja & 4) == 0 )
    LOBYTE(v1) = v1 | 2;
  v2 = v1;
  if ( (mm->Atpce & 0x80) == 0 )
    LOBYTE(v2) = v1 | 4;
  result = v2;
  if ( (mm->Atpc0 & 1) == 0 )
    LOBYTE(result) = v2 | 8;
  return result;
}

int GetWriteDRAM(eegsdata_t *mm)
{
  return (((mm->Atpc0 >> 2) ^ 1) & 1)
       + (((mm->Atpc0 >> 1) ^ 1) & 1)
       + (((mm->Aja >> 4) ^ 1) & 1)
       + (((mm->Aja >> 3) ^ 1) & 1);
}

int GetWriteDRAM_bit(eegsdata_t *mm)
{
  int v1; // ecx
  int v2; // edx
  int result; // eax

  v1 = ((mm->Aja >> 3) ^ 1) & 1;
  if ( (mm->Aja & 0x10) == 0 )
    LOBYTE(v1) = v1 | 2;
  v2 = v1;
  if ( (mm->Atpc0 & 2) == 0 )
    LOBYTE(v2) = v1 | 4;
  result = v2;
  if ( (mm->Atpc0 & 4) == 0 )
    LOBYTE(result) = v2 | 8;
  return result;
}

int GetPageMiss(eegsdata_t *mm)
{
  return (((mm->Atpc0 >> 4) ^ 1) & 1)
       + (((mm->Atpc0 >> 3) ^ 1) & 1)
       + (((mm->Aja >> 6) ^ 1) & 1)
       + (((mm->Aja >> 5) ^ 1) & 1);
}

int GetPageMiss_bit(eegsdata_t *mm)
{
  int v1; // ecx
  int v2; // edx
  int result; // eax

  v1 = ((mm->Aja >> 5) ^ 1) & 1;
  if ( (mm->Aja & 0x40) == 0 )
    LOBYTE(v1) = v1 | 2;
  v2 = v1;
  if ( (mm->Atpc0 & 8) == 0 )
    LOBYTE(v2) = v1 | 4;
  result = v2;
  if ( (mm->Atpc0 & 0x10) == 0 )
    LOBYTE(result) = v2 | 8;
  return result;
}

int GetTransSRAM(eegsdata_t *mm)
{
  return (((mm->Atpc1 >> 2) ^ 1) & 1)
       + (((mm->Atpc1 >> 1) ^ 1) & 1)
       + (((mm->Ajb >> 4) ^ 1) & 1)
       + (((mm->Ajb >> 3) ^ 1) & 1);
}

int GetTransSRAM_bit(eegsdata_t *mm)
{
  int v1; // ecx
  int v2; // edx
  int result; // eax

  v1 = ((mm->Ajb >> 3) ^ 1) & 1;
  if ( (mm->Ajb & 0x10) == 0 )
    LOBYTE(v1) = v1 | 2;
  v2 = v1;
  if ( (mm->Atpc1 & 2) == 0 )
    LOBYTE(v2) = v1 | 4;
  result = v2;
  if ( (mm->Atpc1 & 4) == 0 )
    LOBYTE(result) = v2 | 8;
  return result;
}

int GetReadSRAM(eegsdata_t *mm)
{
  return 16 * ((mm->Atpce & 0x40) != 0)
       + 8 * ((mm->Atpce & 0x20) != 0)
       + 4 * ((mm->Atpce & 0x10) != 0)
       + 2 * ((mm->Atpce & 8) != 0)
       + ((mm->Atpce & 4) != 0);
}

int IsTransClut(eegsdata_t *mm)
{
  return (mm->Ajb & 0x20) != 0;
}

int IsCrtDisplay(eegsdata_t *mm)
{
  return (mm->Atpce & 2) != 0;
}

int GetOutValidPixel(eegsdata_t *mm)
{
  int i; // [esp+8h] [ebp-4h]

  i = 0;
  if ( (mm->Aja & 8) == 0 )
    i = 2 * (mm->Ajb & 1) + (mm->Aja >> 7) + 1;
  if ( (mm->Aja & 0x10) == 0 )
    i += 2 * ((mm->Ajb & 4) != 0) + ((mm->Ajb & 2) != 0) + 1;
  if ( (mm->Atpc0 & 2) == 0 )
    i += 2 * ((mm->Atpc0 & 0x40) != 0) + ((mm->Atpc0 & 0x20) != 0) + 1;
  if ( (mm->Atpc0 & 4) == 0 )
    i += 2 * (mm->Atpc1 & 1) + (mm->Atpc0 >> 7) + 1;
  return i;
}

int GetOutValidPixel_bit(eegsdata_t *mm)
{
  return ((mm->Atpc1 & 1) << 7) | (mm->Atpc0 >> 7 << 6) | (32 * ((mm->Atpc0 & 0x40) != 0)) | (16
                                                                                            * ((mm->Atpc0 & 0x20) != 0)) | (8 * ((mm->Ajb & 4) != 0)) | (4 * ((mm->Ajb & 2) != 0)) | (2 * (mm->Ajb & 1)) | (mm->Aja >> 7);
}

int IsAfterdsresetCheck(eegsdata_t *mm)
{
  int v1; // ebx

  v1 = 0;
  if ( IsDDA(mm, 17)
    && IsMEMIF(mm, 2)
    && !IsPrim(mm)
    && !GetWriteDRAM(mm)
    && !GetTransSRAM(mm)
    && !GetReadSRAM(mm)
    && !GetOutValidPixel(mm) )
  {
    return 1;
  }
  return v1;
}

int IsAfterdsreset(eegsdata_t *mm, eegsdata_t *end, int *opt)
{
  while ( end > mm )
  {
    if ( IsAfterdsresetCheck(mm) )
      return 0;
    mm = (eegsdata_t *)((char *)mm + 10);
  }
  return 1;
}

int IsAfterGSResetCheck(eegsdata_t *mm)
{
  int v1; // ebx

  v1 = 0;
  if ( IsDDA(mm, 17)
    && IsMEMIF(mm, 0)
    && !IsPrim(mm)
    && !GetReadDRAM(mm)
    && !GetWriteDRAM(mm)
    && !GetTransSRAM(mm)
    && !GetOutValidPixel(mm) )
  {
    return 1;
  }
  return v1;
}

int IsAfterGSReset(eegsdata_t *mm, eegsdata_t *end, int *opt)
{
  while ( end > mm )
  {
    if ( IsAfterGSResetCheck(mm) )
      return 0;
    mm = (eegsdata_t *)((char *)mm + 10);
  }
  return 1;
}

int IsPath(eegsdata_t *mm, int path)
{
  return path == ((2 * ((mm->Ap1b & 4) != 0)) | ((mm->Ap1b & 2) != 0));
}

void InitGS(gs_pa *gs)
{
  gs->ddapixel = 0;
  gs->pgn = 0;
  gs->prim = 0;
  gs->read = 0;
  gs->read_bit = 0;
  gs->sread = 0;
  gs->strans = 0;
  gs->strans_bit = 0;
  gs->write = 0;
  gs->write_bit = 0;
  gs->miss = 0;
  gs->miss_bit = 0;
  gs->outpixel = 0;
  gs->outpixel_bit = 0;
  gs->ddasetup = 0;
  gs->ddaidle = 0;
  gs->ddawait = 0;
  gs->clut = 0;
  gs->disp = 0;
}

void UpdateGS(eegsdata_t *mm, gs_pa *gs)
{
  gs->prim = IsPrim(mm);
  gs->ddapixel += GetDDAValidPixel(mm);
  gs->pgn = GetPgn(mm);
  gs->read += GetReadDRAM(mm);
  gs->write += GetWriteDRAM(mm);
  gs->miss += GetPageMiss(mm);
  gs->outpixel = GetOutValidPixel(mm);
  gs->strans = GetTransSRAM(mm);
  gs->sread = GetReadSRAM(mm);
  gs->clut = IsTransClut(mm);
  gs->disp = IsCrtDisplay(mm);
  gs->ddasetup = IsDDA(mm, 24);
  gs->ddaidle = IsDDA(mm, 17);
  gs->ddawait = IsDDA(mm, 18);
  gs->strans_bit = GetTransSRAM_bit(mm);
  gs->read_bit = GetReadDRAM_bit(mm);
  gs->write_bit = GetWriteDRAM_bit(mm);
  gs->miss_bit = GetPageMiss_bit(mm);
  gs->outpixel_bit = GetOutValidPixel_bit(mm);
}

void print_gs(gs_pa *gs)
{
  printf(
    "prim %d ddapixel %d ddasetup %d ddawait %d read %d write %d miss %d outpixel %d\n",
    gs->prim,
    gs->ddapixel,
    gs->ddasetup,
    gs->ddawait,
    gs->read,
    gs->write,
    gs->miss,
    gs->outpixel);
}

int SRV_CheckStrict(eegsdata_t *mm, eegsdata_t *end, int *opt)
{
  gs_pa gs; // [esp+4h] [ebp-54h] BYREF
  int test; // [esp+50h] [ebp-8h]
  int status; // [esp+54h] [ebp-4h]

  status = 0;
  test = 1;
  while ( end > mm )
  {
    if ( !status )
      status = IsPath(mm, 3) != 0;
    if ( status == 1 )
      UpdateGS(mm, &gs);
    else
      InitGS(&gs);
    if ( status == 1 )
    {
      if ( *opt == gs.prim )
      {
        test = 0;
      }
      else if ( gs.prim )
      {
        printf("    NG : %s %d\n", "SRV", gs.prim);
        return 1;
      }
    }
    mm = (eegsdata_t *)((char *)mm + 10);
  }
  if ( test )
    printf("    NG : %s %d was not appeared\n", "SRV", *opt);
  else
    printf("    OK : %s %d\n", "SRV", *opt);
  return test;
}

int TRD_Check(eegsdata_t *mm, eegsdata_t *end, int *opt)
{
  gs_pa gs; // [esp+4h] [ebp-54h] BYREF
  int test; // [esp+50h] [ebp-8h]
  int status; // [esp+54h] [ebp-4h]

  status = 0;
  test = 1;
  while ( end > mm )
  {
    if ( !status )
      status = IsPath(mm, 3) != 0;
    if ( status == 1 )
      UpdateGS(mm, &gs);
    else
      InitGS(&gs);
    if ( status == 1 && *opt == gs.sread )
    {
      test = 0;
      break;
    }
    mm = (eegsdata_t *)((char *)mm + 10);
  }
  if ( test )
    printf("    NG : %s %d was not appeared\n", "TRD", *opt);
  else
    printf("    OK : %s %d\n", "TRD", *opt);
  return test;
}

int TRD_CheckStrict(eegsdata_t *mm, eegsdata_t *end, int *opt)
{
  gs_pa pregs; // [esp+4h] [ebp-A0h] BYREF
  gs_pa gs; // [esp+50h] [ebp-54h] BYREF
  int test; // [esp+9Ch] [ebp-8h]
  int status; // [esp+A0h] [ebp-4h]

  status = 0;
  test = 1;
  InitGS(&pregs);
  while ( end > mm )
  {
    if ( !status )
      status = IsPath(mm, 3) != 0;
    if ( status == 1 )
      UpdateGS(mm, &gs);
    else
      InitGS(&gs);
    if ( status == 1 )
    {
      if ( *opt == 4 )
      {
        if ( *opt == gs.sread )
        {
          test = 0;
        }
        else if ( gs.sread )
        {
          printf("    NG : %s %d\n", "TRD", gs.sread);
          test = 1;
          break;
        }
      }
      else if ( (*opt & 1) != 0 )
      {
        if ( *opt == gs.sread || *opt == pregs.sread + gs.sread )
        {
          test = 0;
          break;
        }
      }
      else if ( *opt == gs.sread )
      {
        test = 0;
        break;
      }
    }
    UpdateGS(mm, &pregs);
    mm = (eegsdata_t *)((char *)mm + 10);
  }
  if ( test )
    printf("    NG : %s %d was not appeared\n", "TRD", *opt);
  else
    printf("    OK : %s %d\n", "TRD", *opt);
  return test;
}

int OUTPIXEL_Check(eegsdata_t *mm, eegsdata_t *end, int *opt)
{
  gs_pa gs; // [esp+4h] [ebp-54h] BYREF
  int test; // [esp+50h] [ebp-8h]
  int status; // [esp+54h] [ebp-4h]

  status = 0;
  test = 1;
  while ( end > mm )
  {
    if ( !status )
      status = IsPath(mm, 3) != 0;
    if ( status == 1 )
      UpdateGS(mm, &gs);
    else
      InitGS(&gs);
    if ( status == 1 )
    {
      if ( *opt == gs.outpixel )
      {
        test = 0;
      }
      else if ( opt[1] != gs.outpixel && gs.outpixel )
      {
        printf("    NG : %s %d\n", "outpixel", gs.outpixel);
        return 1;
      }
    }
    mm = (eegsdata_t *)((char *)mm + 10);
  }
  if ( test )
    printf("    NG : %s %d was not appeared\n", "outpixel", *opt);
  else
    printf("    OK : %s %d\n", "outpixel", *opt);
  return test;
}

int OUTPIXEL_CheckStrict(eegsdata_t *mm, eegsdata_t *end, int *opt)
{
  gs_pa gs; // [esp+4h] [ebp-54h] BYREF
  int test; // [esp+50h] [ebp-8h]
  int status; // [esp+54h] [ebp-4h]

  status = 0;
  test = 1;
  while ( end > mm )
  {
    if ( !status )
      status = IsPath(mm, 3) != 0;
    if ( status == 1 )
      UpdateGS(mm, &gs);
    else
      InitGS(&gs);
    if ( status == 1 )
    {
      if ( *opt == 11 )
      {
        if ( *opt == gs.outpixel )
        {
          test = 0;
        }
        else if ( gs.outpixel && gs.outpixel != 3 )
        {
          goto LABEL_26;
        }
      }
      else if ( *opt == 13 || *opt == 15 )
      {
        if ( *opt == gs.outpixel )
        {
          test = 0;
        }
        else if ( gs.outpixel && gs.outpixel != 6 )
        {
LABEL_26:
          printf("    NG : %s %d\n", "outpixel", gs.outpixel);
          return 1;
        }
      }
      else if ( *opt == gs.outpixel )
      {
        test = 0;
      }
      else if ( gs.outpixel )
      {
        goto LABEL_26;
      }
    }
    mm = (eegsdata_t *)((char *)mm + 10);
  }
  if ( test )
    printf("    NG : %s %d was not appeared\n", "outpixel", *opt);
  else
    printf("    OK : %s %d\n", "outpixel", *opt);
  return test;
}

int TTR_CheckStrict(eegsdata_t *mm, eegsdata_t *end, int *opt)
{
  gs_pa gs; // [esp+4h] [ebp-54h] BYREF
  int test; // [esp+50h] [ebp-8h]
  int status; // [esp+54h] [ebp-4h]

  status = 0;
  test = 1;
  while ( end > mm )
  {
    if ( !status )
      status = IsPath(mm, 3) != 0;
    if ( status == 1 )
      UpdateGS(mm, &gs);
    else
      InitGS(&gs);
    if ( status == 1 )
    {
      if ( *opt == gs.strans_bit )
      {
        test = 0;
      }
      else if ( gs.strans_bit )
      {
        printf("    NG : %s %d\n", "TTR", gs.strans_bit);
        return 1;
      }
    }
    mm = (eegsdata_t *)((char *)mm + 10);
  }
  if ( test )
    printf("    NG : %s %d was not appeared\n", "TTR", *opt);
  else
    printf("    OK : %s %d\n", "TTR", *opt);
  return test;
}

int CTR_CheckStrict(eegsdata_t *mm, eegsdata_t *end, int *opt)
{
  gs_pa gs; // [esp+4h] [ebp-54h] BYREF
  int test; // [esp+50h] [ebp-8h]
  int status; // [esp+54h] [ebp-4h]

  status = 0;
  test = 1;
  while ( end > mm )
  {
    if ( !status )
      status = IsPath(mm, 3) != 0;
    if ( status == 1 )
      UpdateGS(mm, &gs);
    else
      InitGS(&gs);
    if ( status == 1 )
    {
      if ( *opt == gs.clut )
      {
        test = 0;
      }
      else if ( gs.clut )
      {
        printf("    NG : %s %d\n", "CTR", gs.clut);
        return 1;
      }
    }
    mm = (eegsdata_t *)((char *)mm + 10);
  }
  if ( test )
    printf("    NG : %s %d was not appeared\n", "CTR", *opt);
  else
    printf("    OK : %s %d\n", "CTR", *opt);
  return test;
}

int PMS_Check(eegsdata_t *mm, eegsdata_t *end, int *opt)
{
  gs_pa gs; // [esp+4h] [ebp-54h] BYREF
  int test; // [esp+50h] [ebp-8h]
  int status; // [esp+54h] [ebp-4h]

  status = 0;
  test = 1;
  while ( end > mm )
  {
    if ( !status )
      status = IsPath(mm, 3) != 0;
    if ( status == 1 )
      UpdateGS(mm, &gs);
    else
      InitGS(&gs);
    if ( status == 1 && *opt == gs.miss_bit )
      test = 0;
    mm = (eegsdata_t *)((char *)mm + 10);
  }
  if ( test )
    printf("    NG : %s %d was not appeared\n", "PMS", *opt);
  else
    printf("    OK : %s %d\n", "PMS", *opt);
  return test;
}

int PMS_CheckStrict(eegsdata_t *mm, eegsdata_t *end, int *opt)
{
  gs_pa gs; // [esp+4h] [ebp-54h] BYREF
  int test; // [esp+50h] [ebp-8h]
  int status; // [esp+54h] [ebp-4h]

  status = 0;
  test = 1;
  while ( end > mm )
  {
    if ( !status )
      status = IsPath(mm, 3) != 0;
    if ( status == 1 )
      UpdateGS(mm, &gs);
    else
      InitGS(&gs);
    if ( status == 1 )
    {
      if ( *opt == 5 || *opt == 10 )
      {
        if ( *opt == gs.miss_bit )
        {
          test = 0;
          printf("    OK : %s %d\n", "PMS", *opt);
          return 0;
        }
        if ( gs.miss_bit )
          test = 1;
      }
      else if ( *opt == gs.miss_bit )
      {
        test = 0;
      }
      else if ( gs.miss_bit )
      {
        printf("    NG : %s %d\n", "PMS", gs.miss_bit);
        return 1;
      }
    }
    mm = (eegsdata_t *)((char *)mm + 10);
  }
  if ( test )
    printf("    NG : %s %d was not appeared\n", "PMS", *opt);
  else
    printf("    OK : %s %d\n", "PMS", *opt);
  return test;
}

int MRD_CheckStrict(eegsdata_t *mm, eegsdata_t *end, int *opt)
{
  gs_pa gs; // [esp+4h] [ebp-54h] BYREF
  int test; // [esp+50h] [ebp-8h]
  int status; // [esp+54h] [ebp-4h]

  status = 0;
  test = 1;
  while ( end > mm )
  {
    if ( !status )
      status = IsPath(mm, 3) != 0;
    if ( status == 1 )
      UpdateGS(mm, &gs);
    else
      InitGS(&gs);
    if ( status == 1 )
    {
      if ( *opt == gs.read_bit )
      {
        test = 0;
      }
      else if ( gs.read_bit )
      {
        printf("    NG : %s %d\n", "MRD", gs.read_bit);
        return 1;
      }
    }
    mm = (eegsdata_t *)((char *)mm + 10);
  }
  if ( test )
    printf("    NG : %s %d was not appeared\n", "MRD", *opt);
  else
    printf("    OK : %s %d\n", "MRD", *opt);
  return test;
}

int MWT_CheckStrict(eegsdata_t *mm, eegsdata_t *end, int *opt)
{
  gs_pa gs; // [esp+4h] [ebp-54h] BYREF
  int test; // [esp+50h] [ebp-8h]
  int status; // [esp+54h] [ebp-4h]

  status = 0;
  test = 1;
  while ( end > mm )
  {
    if ( !status )
      status = IsPath(mm, 3) != 0;
    if ( status == 1 )
      UpdateGS(mm, &gs);
    else
      InitGS(&gs);
    if ( status == 1 )
    {
      if ( *opt == gs.write_bit )
      {
        test = 0;
      }
      else if ( gs.write_bit )
      {
        printf("    NG : %s %d\n", "MWT", gs.write_bit);
        return 1;
      }
    }
    mm = (eegsdata_t *)((char *)mm + 10);
  }
  if ( test )
    printf("    NG : %s %d was not appeared\n", "MWT", *opt);
  else
    printf("    OK : %s %d\n", "MWT", *opt);
  return test;
}

int PWT_CheckStrict(eegsdata_t *mm, eegsdata_t *end, int *opt)
{
  gs_pa gs; // [esp+8h] [ebp-5Ch] BYREF
  int test; // [esp+54h] [ebp-10h]
  int status; // [esp+58h] [ebp-Ch]
  int cnt; // [esp+5Ch] [ebp-8h]
  int PWT; // [esp+60h] [ebp-4h]

  PWT = 0;
  cnt = 0;
  status = 0;
  test = 1;
  while ( end > mm )
  {
    PWT = ((mm->Atpc1 & 1) << 7) | (mm->Atpc0 >> 7 << 6) | (32 * ((mm->Atpc0 & 0x40) != 0)) | (16
                                                                                             * ((mm->Atpc0 & 0x20) != 0)) | (8 * ((mm->Ajb & 4) != 0)) | (4 * ((mm->Ajb & 2) != 0)) | (2 * (mm->Ajb & 1)) | (mm->Aja >> 7);
    if ( !status )
      status = IsPath(mm, 3) != 0;
    if ( status == 1 )
      UpdateGS(mm, &gs);
    else
      InitGS(&gs);
    ++cnt;
    if ( status == 1 )
    {
      if ( *opt == gs.outpixel_bit )
      {
        test = 0;
      }
      else if ( gs.outpixel_bit )
      {
        printf("    NG : %s %d\n", "PWT", gs.outpixel_bit);
        return 1;
      }
    }
    mm = (eegsdata_t *)((char *)mm + 10);
  }
  if ( test )
    printf("    NG : %s %d was not appeared\n", "PWT", *opt);
  else
    printf("    OK : %s %d\n", "PWT", *opt);
  return test;
}

int PGN_Check(eegsdata_t *mm, eegsdata_t *end, int *opt)
{
  gs_pa gs; // [esp+4h] [ebp-54h] BYREF
  int test; // [esp+50h] [ebp-8h]
  int status; // [esp+54h] [ebp-4h]

  status = 0;
  test = 1;
  while ( end > mm )
  {
    if ( !status )
      status = IsPath(mm, 3) != 0;
    if ( status == 1 )
      UpdateGS(mm, &gs);
    else
      InitGS(&gs);
    if ( status == 1 && *opt == gs.pgn )
    {
      test = 0;
      break;
    }
    mm = (eegsdata_t *)((char *)mm + 10);
  }
  if ( test )
    printf("    NG : %s %d was not appeared\n", "PGN", *opt);
  else
    printf("    OK : %s %d\n", "PGN", *opt);
  return test;
}

int PGN_CheckStrict(eegsdata_t *mm, eegsdata_t *end, int *opt)
{
  gs_pa gs; // [esp+4h] [ebp-54h] BYREF
  int test; // [esp+50h] [ebp-8h]
  int status; // [esp+54h] [ebp-4h]

  status = 0;
  test = 1;
  while ( end > mm )
  {
    if ( !status )
      status = IsPath(mm, 3) != 0;
    if ( status == 1 )
      UpdateGS(mm, &gs);
    else
      InitGS(&gs);
    if ( status == 1 )
    {
      if ( *opt == 18 )
      {
        if ( *opt == gs.pgn )
        {
          test = 0;
        }
        else if ( gs.pgn != 17 && gs.pgn != 8 )
        {
LABEL_14:
          printf("    NG : %s %d\n", "PGN", gs.pgn);
          return 1;
        }
      }
      else if ( *opt == 31 )
      {
        if ( *opt == gs.pgn )
        {
          printf("    OK : %s %d\n", "PGN", *opt);
          return 0;
        }
        if ( gs.pgn != 17 )
          test = 1;
      }
      else if ( *opt == gs.pgn )
      {
        test = 0;
      }
      else if ( gs.pgn != 17 )
      {
        goto LABEL_14;
      }
    }
    mm = (eegsdata_t *)((char *)mm + 10);
  }
  if ( test )
    printf("    NG : %s %d was not appeared\n", "PGN", *opt);
  else
    printf("    OK : %s %d\n", "PGN", *opt);
  return test;
}

int DISP_Check(VERIFY_PFM *verify, eegsdata_t *mm, eegsdata_t *end, int *opt)
{
  int status; // eax
  gs_pa gs; // [esp+0h] [ebp-4Ch] BYREF

  while ( end > mm )
  {
    status = verify->status;
    if ( verify->status == 1 )
    {
      UpdateGS(mm, &gs);
      if ( *opt == gs.disp )
        verify->status = 2;
    }
    else if ( status > 1 )
    {
      if ( status == 2 )
      {
        m_result = 0;
        return 0;
      }
    }
    else if ( !status )
    {
      InitGS(&gs);
      if ( IsPath(mm, 3) )
        verify->status = 1;
    }
    mm = (eegsdata_t *)((char *)mm + 10);
  }
  return 0;
}

int PrintGS(eegsdata_t *mm, eegsdata_t *end, int *opt)
{
  gs_pa gs; // [esp+0h] [ebp-50h] BYREF
  int status; // [esp+4Ch] [ebp-4h]

  status = 0;
  while ( end > mm )
  {
    if ( !status )
      status = IsPath(mm, 3) != 0;
    if ( status == 1 )
      UpdateGS(mm, &gs);
    else
      InitGS(&gs);
    if ( status == 1 )
    {
      printf("SRV %d PGN %d PMS %d MRD %d MWT %d ", gs.prim, gs.pgn, gs.miss, gs.read_bit, gs.write_bit);
      printf("TTR %d PWT %d CTR %d TRD %d DISP %d\n", gs.strans_bit, gs.outpixel_bit, gs.clut, gs.sread, gs.disp);
    }
    mm = (eegsdata_t *)((char *)mm + 10);
  }
  return 1;
}

int IsHostLocal(eegsdata_t *mm, eegsdata_t *end, int *opt)
{
  gs_pa gs; // [esp+0h] [ebp-50h] BYREF
  int status; // [esp+4Ch] [ebp-4h]

  status = 0;
  while ( end > mm )
  {
    if ( status )
    {
      if ( status == 1 )
        UpdateGS(mm, &gs);
    }
    else
    {
      InitGS(&gs);
      status = 1;
    }
    if ( IsSend(mm, 41) )
      return 0;
    mm = (eegsdata_t *)((char *)mm + 10);
  }
  return 1;
}

int IsLocalHost(eegsdata_t *mm, eegsdata_t *end, int *opt)
{
  gs_pa gs; // [esp+0h] [ebp-50h] BYREF
  int status; // [esp+4Ch] [ebp-4h]

  status = 0;
  while ( end > mm )
  {
    if ( status )
    {
      if ( status == 1 )
        UpdateGS(mm, &gs);
    }
    else
    {
      InitGS(&gs);
      status = 1;
    }
    if ( IsSend(mm, 39) )
      return 0;
    mm = (eegsdata_t *)((char *)mm + 10);
  }
  return 1;
}

int IsLocalLocal(eegsdata_t *mm, eegsdata_t *end, int *opt)
{
  while ( end > mm )
  {
    if ( status_116 == 1 )
    {
      if ( IsDDA(mm, 24) )
        status_116 = 2;
    }
    else if ( status_116 > 1 )
    {
      if ( status_116 == 2 )
        UpdateGS(mm, &gs_120);
    }
    else if ( !status_116 )
    {
      InitGS(&gs_120);
      if ( IsSend(mm, 41) )
        status_116 = 1;
    }
    if ( IsMEMIF(mm, 4) )
      return 0;
    mm = (eegsdata_t *)((char *)mm + 10);
  }
  return 1;
}

int IsSprite(eegsdata_t *mm, eegsdata_t *end, int *opt)
{
  while ( end > mm )
  {
    if ( status_120 == 1 )
    {
      UpdateGS(mm, &gs_121);
      if ( gs_121.prim )
        status_120 = 2;
    }
    else if ( status_120 > 1 )
    {
      if ( status_120 == 2 )
      {
        UpdateGS(mm, &gs_121);
        if ( gs_121.ddapixel == 1024
          && gs_121.read > 255
          && gs_121.write == 256
          && gs_121.miss > 7
          && gs_121.outpixel > 15 )
        {
          return 0;
        }
      }
    }
    else if ( !status_120 )
    {
      InitGS(&gs_121);
      if ( IsPath(mm, 3) )
        status_120 = 1;
    }
    mm = (eegsdata_t *)((char *)mm + 10);
  }
  return 1;
}

int PFM_SDRAMCheck(NN *nn, NN *end, int opt)
{
  int error; // [esp+0h] [ebp-Ch]
  int test; // [esp+4h] [ebp-8h]
  int status; // [esp+8h] [ebp-4h]

  status = 0;
  test = 1;
  error = 0;
  if ( opt == 2 && m_init )
  {
    m_sdram = (unsigned __int16)m_sdram;
    if ( (unsigned __int16)m_sdram == 0xFFFF )
      m_sdram = 0;
    else
      m_sdram = (m_sdram | (m_sdram << 16)) + 65537;
    m_init = 0;
  }
  else if ( opt == 3 && m_init )
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
    if ( opt == 1 )
    {
      if ( (status & 1) != 0 )
      {
        if ( nn->sdram_data1 != -1431677611 )
        {
          if ( error <= 9 )
            printf("sdram_data1 = %x %d\n", nn->sdram_data1, status);
          ++error;
        }
        if ( nn->sdram_data2 != -1431677611 )
        {
          if ( error <= 9 )
            printf("sdram_data2 = %x %d\n", nn->sdram_data2, status);
          ++error;
        }
        if ( nn->sdram_data3 != -1431677611 )
        {
          if ( error <= 9 )
            printf("sdram_data3 = %x %d\n", nn->sdram_data3, status);
          ++error;
        }
        if ( nn->sdram_data4 != -1431677611 )
        {
          if ( error <= 9 )
            printf("sdram_data4 = %x %d\n", nn->sdram_data4, status);
          ++error;
        }
      }
      else
      {
        if ( nn->sdram_data1 != 1431677610 )
        {
          if ( error <= 9 )
            printf("sdram_data1 = %x %d\n", nn->sdram_data1, status);
          ++error;
        }
        if ( nn->sdram_data2 != 1431677610 )
        {
          if ( error <= 9 )
            printf("sdram_data2 = %x %d\n", nn->sdram_data2, status);
          ++error;
        }
        if ( nn->sdram_data3 != 1431677610 )
        {
          if ( error <= 9 )
            printf("sdram_data3 = %x %d\n", nn->sdram_data3, status);
          ++error;
        }
        if ( nn->sdram_data4 != 1431677610 )
        {
          if ( error <= 9 )
            printf("sdram_data4 = %x %d\n", nn->sdram_data4, status);
          ++error;
        }
      }
      ++status;
    }
    else if ( opt > 1 )
    {
      if ( opt == 2 )
      {
        if ( m_sdram != nn->sdram_data1 )
        {
          if ( error <= 9 )
            printf("sdram_data1 = %x %x\n", nn->sdram_data1, m_sdram);
          ++error;
        }
        if ( m_sdram != nn->sdram_data2 )
        {
          if ( error <= 9 )
            printf("sdram_data2 = %x %x\n", nn->sdram_data2, m_sdram);
          ++error;
        }
        if ( m_sdram != nn->sdram_data3 )
        {
          if ( error <= 9 )
            printf("sdram_data3 = %x %x\n", nn->sdram_data3, m_sdram);
          ++error;
        }
        if ( m_sdram != nn->sdram_data4 )
        {
          if ( error <= 9 )
            printf("sdram_data4 = %x %x\n", nn->sdram_data4, m_sdram);
          ++error;
        }
        if ( m_sdram == -1 )
          m_sdram = 0;
        else
          m_sdram += 65537;
      }
      else if ( opt == 3 )
      {
        if ( m_sdram != nn->sdram_data1 )
        {
          if ( error <= 9 )
            printf("sdram_data1 = %x %x\n", nn->sdram_data1, m_sdram);
          ++error;
        }
        if ( m_sdram != nn->sdram_data2 )
        {
          if ( error <= 9 )
            printf("sdram_data2 = %x %x\n", nn->sdram_data2, m_sdram);
          ++error;
        }
        if ( m_sdram != nn->sdram_data3 )
        {
          if ( error <= 9 )
            printf("sdram_data3 = %x %x\n", nn->sdram_data3, m_sdram);
          ++error;
        }
        if ( m_sdram != nn->sdram_data4 )
        {
          if ( error <= 9 )
            printf("sdram_data4 = %x %x\n", nn->sdram_data4, m_sdram);
          ++error;
        }
        if ( m_sdram == -1 )
          m_sdram = 0;
        else
          m_sdram += 16843009;
      }
    }
    else if ( !opt )
    {
      if ( nn->sdram_data1 != -1 )
      {
        if ( error <= 9 )
          printf("sdram_data1 = %x %d\n", nn->sdram_data1, status);
        ++error;
      }
      if ( nn->sdram_data2 != -1 )
      {
        if ( error <= 9 )
          printf("sdram_data2 = %x %d\n", nn->sdram_data2, status);
        ++error;
      }
      if ( nn->sdram_data3 != -1 )
      {
        if ( error <= 9 )
          printf("sdram_data3 = %x %d\n", nn->sdram_data3, status);
        ++error;
      }
      if ( nn->sdram_data4 != -1 )
      {
        if ( error <= 9 )
          printf("sdram_data4 = %x %d\n", nn->sdram_data4, status);
        ++error;
      }
    }
    ++nn;
  }
  if ( !error )
    return 0;
  return test;
}

int IsEEPerform(eegsdata_t *mm, eegsdata_t *end, int *opt)
{
  unsigned int v4; // [esp+10h] [ebp-1Ch]
  unsigned int bist_ans; // [esp+18h] [ebp-14h]
  unsigned int bist_cnt; // [esp+1Ch] [ebp-10h]
  unsigned int bist_on; // [esp+20h] [ebp-Ch]
  unsigned int cnt; // [esp+24h] [ebp-8h]
  int status; // [esp+28h] [ebp-4h]

  status = 0;
  cnt = 0;
  bist_on = 0;
  bist_cnt = 0;
  bist_ans = 0;
  while ( end > mm )
  {
    v4 = (4 * ((mm->Ap1a & 4) != 0)) | (2 * ((mm->Ap1a & 2) != 0)) | mm->Ap1a & 1;
    if ( status == 1 )
    {
      if ( ((32 * ((mm->Ap1a & 2) != 0)) | (16 * (mm->Ap1a & 1)) | (8 * (mm->Ap0b >> 7)) | (4 * ((mm->Ap0b & 0x40) != 0)) | (2 * ((mm->Ap0b & 0x20) != 0)) | ((mm->Ap0b & 0x10) != 0)) == 63
        && (mm->Ap1a & 4) == 0 )
      {
        ++bist_on;
      }
      if ( bist_on == 2 )
      {
        status = 2;
      }
      else if ( bist_on )
      {
        Bist_Calculation(
          (mm->Ap0b >> 7 << 15) | (((mm->Ap0b & 0x40) != 0) << 14) | (((mm->Ap0b & 0x20) != 0) << 13) | (((mm->Ap0b & 0x10) != 0) << 12) | (((mm->Ap0b & 8) != 0) << 11) | (((mm->Ap0b & 4) != 0) << 10) | (((mm->Ap0b & 2) != 0) << 9) | ((mm->Ap0b & 1) << 8) | (mm->Ap0a >> 7 << 7) | (((mm->Ap0a & 0x40) != 0) << 6) | (32 * ((mm->Ap0a & 0x20) != 0)) | (16 * ((mm->Ap0a & 0x10) != 0)) | (8 * ((mm->Ap0a & 8) != 0)) | (4 * ((mm->Ap0a & 4) != 0)) | (2 * ((mm->Ap0a & 2) != 0)) | mm->Ap0a & 1,
          bist_cnt,
          1);
        bist_ans = Bist_Calculation(v4, bist_cnt++, 1);
      }
    }
    else if ( status > 1 )
    {
      if ( bist_ans == 313 )
        return 0;
      status = 1;
      bist_on = 0;
      bist_cnt = 0;
      bist_ans = 0;
      Bist_Calculation(0, 0, 0);
    }
    else if ( (mm->Ap0b & 8) == 0 )
    {
      status = 1;
      bist_on = 0;
      bist_cnt = 0;
      bist_ans = 0;
      Bist_Calculation(0, 0, 0);
    }
    ++cnt;
    mm = (eegsdata_t *)((char *)mm + 10);
  }
  return 1;
}

int Bist_Calculation(int n, int cnt, int mode)
{
  if ( mode )
  {
    if ( cnt > 0 )
      d01_176 = ((unsigned __int8)c12_171 ^ (unsigned __int8)c16_175) & 1;
    else
      d01_176 = 1;
    if ( cnt > 0 )
      d02_177 = ((unsigned __int8)c03_162 ^ (unsigned __int8)d01_176) & 1;
    else
      d02_177 = 1;
    if ( cnt > 0 )
      d03_178 = ((unsigned __int8)c01_160 ^ (unsigned __int8)d02_177) & 1;
    else
      d03_178 = 1;
    if ( cnt > 0 )
      b16_159 = c15_174 & 1;
    else
      b16_159 = 0;
    a16_143 = (n >> 15) & 1;
    c16_175 = ((unsigned __int8)b16_159 ^ (n >> 15) & 1) & 1;
    if ( cnt > 0 )
      b15_158 = c14_173 & 1;
    else
      b15_158 = 0;
    a15_142 = (n >> 14) & 1;
    c15_174 = ((unsigned __int8)b15_158 ^ (n >> 14) & 1) & 1;
    if ( cnt > 0 )
      b14_157 = c13_172 & 1;
    else
      b14_157 = 0;
    a14_141 = (n >> 13) & 1;
    c14_173 = ((unsigned __int8)b14_157 ^ (n >> 13) & 1) & 1;
    if ( cnt > 0 )
      b13_156 = c12_171 & 1;
    else
      b13_156 = 0;
    a13_140 = (n >> 12) & 1;
    c13_172 = ((unsigned __int8)b13_156 ^ (n >> 12) & 1) & 1;
    if ( cnt > 0 )
      b12_155 = c11_170 & 1;
    else
      b12_155 = 0;
    a12_139 = (n >> 11) & 1;
    c12_171 = ((unsigned __int8)b12_155 ^ (n >> 11) & 1) & 1;
    if ( cnt > 0 )
      b11_154 = c10_169 & 1;
    else
      b11_154 = 0;
    a11_138 = (n >> 10) & 1;
    c11_170 = ((unsigned __int8)b11_154 ^ (n >> 10) & 1) & 1;
    if ( cnt > 0 )
      b10_153 = c09_168 & 1;
    else
      b10_153 = 0;
    a10_137 = (n >> 9) & 1;
    c10_169 = ((unsigned __int8)b10_153 ^ (n >> 9) & 1) & 1;
    if ( cnt > 0 )
      b09_152 = c08_167 & 1;
    else
      b09_152 = 0;
    a09_136 = (n >> 8) & 1;
    c09_168 = ((unsigned __int8)b09_152 ^ BYTE1(n) & 1) & 1;
    if ( cnt > 0 )
      b08_151 = c07_166 & 1;
    else
      b08_151 = 0;
    a08_135 = (n >> 7) & 1;
    c08_167 = ((unsigned __int8)b08_151 ^ (n >> 7) & 1) & 1;
    if ( cnt > 0 )
      b07_150 = c06_165 & 1;
    else
      b07_150 = 0;
    a07_134 = (n >> 6) & 1;
    c07_166 = ((unsigned __int8)b07_150 ^ (n >> 6) & 1) & 1;
    if ( cnt > 0 )
      b06_149 = c05_164 & 1;
    else
      b06_149 = 0;
    a06_133 = (n >> 5) & 1;
    c06_165 = ((unsigned __int8)b06_149 ^ (n >> 5) & 1) & 1;
    if ( cnt > 0 )
      b05_148 = c04_163 & 1;
    else
      b05_148 = 0;
    a05_132 = (n >> 4) & 1;
    c05_164 = ((unsigned __int8)b05_148 ^ (n >> 4) & 1) & 1;
    if ( cnt > 0 )
      b04_147 = c03_162 & 1;
    else
      b04_147 = 0;
    a04_131 = (n >> 3) & 1;
    c04_163 = ((unsigned __int8)b04_147 ^ (n >> 3) & 1) & 1;
    if ( cnt > 0 )
      b03_146 = c02_161 & 1;
    else
      b03_146 = 0;
    a03_130 = (n >> 2) & 1;
    c03_162 = ((unsigned __int8)b03_146 ^ (n >> 2) & 1) & 1;
    if ( cnt > 0 )
      b02_145 = c01_160 & 1;
    else
      b02_145 = 0;
    a02_129 = (n >> 1) & 1;
    c02_161 = ((unsigned __int8)b02_145 ^ (n >> 1) & 1) & 1;
    if ( cnt > 0 )
      b01_144 = d03_178 & 1;
    else
      b01_144 = 0;
    a01_128 = n & 1;
    c01_160 = ((unsigned __int8)b01_144 ^ n & 1) & 1;
    m16_195 = c16_175 << 15;
    m15_194 = c15_174 << 14;
    m14_193 = c14_173 << 13;
    m13_192 = c13_172 << 12;
    m12_191 = c12_171 << 11;
    m11_190 = c11_170 << 10;
    m10_189 = c10_169 << 9;
    m09_188 = c09_168 << 8;
    m08_187 = c08_167 << 7;
    m07_186 = c07_166 << 6;
    m06_185 = 32 * c06_165;
    m05_184 = 16 * c05_164;
    m04_183 = 8 * c04_163;
    m03_182 = 4 * c03_162;
    m02_181 = 2 * c02_161;
    m01_180 = c01_160;
    m_179 = 2 * c02_161
          + 4 * c03_162
          + 8 * c04_163
          + 16 * c05_164
          + 32 * c06_165
          + (c07_166 << 6)
          + (c08_167 << 7)
          + (c09_168 << 8)
          + (c10_169 << 9)
          + (c11_170 << 10)
          + (c12_171 << 11)
          + (c13_172 << 12)
          + (c14_173 << 13)
          + (c15_174 << 14)
          + (c16_175 << 15)
          + c01_160;
    return m_179;
  }
  else
  {
    a08_135 = 0;
    a07_134 = 0;
    a06_133 = 0;
    a05_132 = 0;
    a04_131 = 0;
    a03_130 = 0;
    a02_129 = 0;
    a01_128 = 0;
    a16_143 = 0;
    a15_142 = 0;
    a14_141 = 0;
    a13_140 = 0;
    a12_139 = 0;
    a11_138 = 0;
    a10_137 = 0;
    a09_136 = 0;
    b08_151 = 0;
    b07_150 = 0;
    b06_149 = 0;
    b05_148 = 0;
    b04_147 = 0;
    b03_146 = 0;
    b02_145 = 0;
    b01_144 = 0;
    b16_159 = 0;
    b15_158 = 0;
    b14_157 = 0;
    b13_156 = 0;
    b12_155 = 0;
    b11_154 = 0;
    b10_153 = 0;
    b09_152 = 0;
    c08_167 = 0;
    c07_166 = 0;
    c06_165 = 0;
    c05_164 = 0;
    c04_163 = 0;
    c03_162 = 0;
    c02_161 = 0;
    c01_160 = 0;
    c16_175 = 0;
    c15_174 = 0;
    c14_173 = 0;
    c13_172 = 0;
    c12_171 = 0;
    c11_170 = 0;
    c10_169 = 0;
    c09_168 = 0;
    m_179 = 0;
    d03_178 = 0;
    d02_177 = 0;
    d01_176 = 0;
    m08_187 = 0;
    m07_186 = 0;
    m06_185 = 0;
    m05_184 = 0;
    m04_183 = 0;
    m03_182 = 0;
    m02_181 = 0;
    m01_180 = 0;
    m16_195 = 0;
    m15_194 = 0;
    m14_193 = 0;
    m13_192 = 0;
    m12_191 = 0;
    m11_190 = 0;
    m10_189 = 0;
    m09_188 = 0;
    return 0;
  }
}

int check_pfm(VERIFY_PFM *verify, char *p, int mode, int size, char *checkStr)
{
  int opt[4]; // [esp+0h] [ebp-24h] BYREF
  NN *n_end; // [esp+10h] [ebp-14h]
  NN *nn; // [esp+14h] [ebp-10h]
  eegsdata_t *end; // [esp+18h] [ebp-Ch]
  eegsdata_t *mm; // [esp+1Ch] [ebp-8h]
  int qword; // [esp+20h] [ebp-4h]

  qword = 1;
  mm = (eegsdata_t *)p;
  end = (eegsdata_t *)&p[size];
  nn = (NN *)p;
  n_end = (NN *)&p[size];
  opt[0] = mode;
  memset(&opt[1], 0, 12);
  if ( !strcmp(checkStr, "SRV_CheckStrict") )
    return SRV_CheckStrict(mm, end, opt);
  if ( !strcmp(checkStr, "TRD_Check") )
    return TRD_Check(mm, end, opt);
  if ( !strcmp(checkStr, "TRD_CheckStrict") )
    return TRD_CheckStrict(mm, end, opt);
  if ( !strcmp(checkStr, "OUTPIXEL_Check") )
    return OUTPIXEL_Check(mm, end, opt);
  if ( !strcmp(checkStr, "OUTPIXEL_CheckStrict") )
    return OUTPIXEL_CheckStrict(mm, end, opt);
  if ( !strcmp(checkStr, "TTR_CheckStrict") )
    return TTR_CheckStrict(mm, end, opt);
  if ( !strcmp(checkStr, "CTR_CheckStrict") )
    return CTR_CheckStrict(mm, end, opt);
  if ( !strcmp(checkStr, "DISP_Check") )
    return DISP_Check(verify, mm, end, opt);
  if ( !strcmp(checkStr, "PMS_Check") )
    return PMS_Check(mm, end, opt);
  if ( !strcmp(checkStr, "PMS_CheckStrict") )
    return PMS_CheckStrict(mm, end, opt);
  if ( !strcmp(checkStr, "MRD_CheckStrict") )
    return MRD_CheckStrict(mm, end, opt);
  if ( !strcmp(checkStr, "MWT_CheckStrict") )
    return MWT_CheckStrict(mm, end, opt);
  if ( !strcmp(checkStr, "PWT_CheckStrict") )
    return PWT_CheckStrict(mm, end, opt);
  if ( !strcmp(checkStr, "PGN_Check") )
    return PGN_Check(mm, end, opt);
  if ( !strcmp(checkStr, "PGN_CheckStrict") )
    return PGN_CheckStrict(mm, end, opt);
  if ( !strcmp(checkStr, "IsAfterdsreset") )
    return IsAfterdsreset(mm, end, opt);
  if ( !strcmp(checkStr, "IsAfterGSReset") )
    return IsAfterGSReset(mm, end, opt);
  if ( !strcmp(checkStr, "IsHostLocal") )
    return IsHostLocal(mm, end, opt);
  if ( !strcmp(checkStr, "IsLocalHost") )
    return IsLocalHost(mm, end, opt);
  if ( !strcmp(checkStr, "IsLocalLocal") )
    return IsLocalLocal(mm, end, opt);
  if ( !strcmp(checkStr, "IsSprite") )
    return IsSprite(mm, end, opt);
  if ( !strcmp(checkStr, "PFM_SDRAMCheck") )
    return PFM_SDRAMCheck(nn, n_end, opt[0]);
  if ( !strcmp(checkStr, "IsEEPerform") )
    return IsEEPerform(mm, end, opt);
  return PrintGS(mm, end, opt);
}

int reset_pfm(VERIFY_PFM *verify, char *checkStr)
{
  if ( !strcmp(checkStr, "DISP_Check") )
  {
    m_result = 1;
    verify->status = 0;
    verify->cnt = 0;
  }
  return 0;
}

int result_pfm(VERIFY_PFM *verify, int mode, char *checkStr)
{
  if ( m_result )
  {
    printf("    NG : DISP %d was not appeared\n", mode);
    return 1;
  }
  else
  {
    printf("    OK : DISP %d\n", mode);
    return 0;
  }
}

