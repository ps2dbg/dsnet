
#include "dsxdb_prototypes.h"

extern int dsc_connected; // defined in dbg.c
static IOP_REGS iop_regs[] =
{
  { 1, 0, "lo" },
  { 1, 1, "hi" },
  { 2, 0, "zero" },
  { 2, 1, "at" },
  { 2, 2, "v0" },
  { 2, 3, "v1" },
  { 2, 4, "a0" },
  { 2, 5, "a1" },
  { 2, 6, "a2" },
  { 2, 7, "a3" },
  { 2, 8, "t0" },
  { 2, 9, "t1" },
  { 2, 10, "t2" },
  { 2, 11, "t3" },
  { 2, 12, "t4" },
  { 2, 13, "t5" },
  { 2, 14, "t6" },
  { 2, 15, "t7" },
  { 2, 16, "s0" },
  { 2, 17, "s1" },
  { 2, 18, "s2" },
  { 2, 19, "s3" },
  { 2, 20, "s4" },
  { 2, 21, "s5" },
  { 2, 22, "s6" },
  { 2, 23, "s7" },
  { 2, 24, "t8" },
  { 2, 25, "t9" },
  { 2, 26, "k0" },
  { 2, 27, "k1" },
  { 2, 28, "gp" },
  { 2, 29, "sp" },
  { 2, 30, "fp" },
  { 2, 31, "ra" },
  { 3, 3, "bpc" },
  { 3, 5, "bda" },
  { 3, 6, "tar" },
  { 3, 7, "dcic" },
  { 3, 8, "bada" },
  { 3, 9, "bdam" },
  { 3, 11, "bpcm" },
  { 3, 12, "sr" },
  { 3, 13, "cr" },
  { 3, 14, "epc" },
  { 3, 15, "prid" },
  { 6, 0, "vxy0" },
  { 6, 1, "vz0" },
  { 6, 2, "vxy1" },
  { 6, 3, "vz1" },
  { 6, 4, "vxy2" },
  { 6, 5, "vz2" },
  { 6, 6, "rgb" },
  { 6, 7, "otz" },
  { 6, 8, "ir0" },
  { 6, 9, "ir1" },
  { 6, 10, "ir2" },
  { 6, 11, "ir3" },
  { 6, 12, "sxy0" },
  { 6, 13, "sxy1" },
  { 6, 14, "sxy2" },
  { 6, 15, "sxy2p" },
  { 6, 16, "szx" },
  { 6, 17, "sz0" },
  { 6, 18, "sz1" },
  { 6, 19, "sz2" },
  { 6, 20, "rgb0" },
  { 6, 21, "rgb1" },
  { 6, 22, "rgb2" },
  { 6, 23, "res1" },
  { 6, 24, "mac0" },
  { 6, 25, "mac1" },
  { 6, 26, "mac2" },
  { 6, 27, "mac3" },
  { 6, 28, "irgb" },
  { 6, 29, "orgb" },
  { 6, 30, "data32" },
  { 6, 31, "lzc" },
  { 7, 0, "r11r12" },
  { 7, 1, "r13r21" },
  { 7, 2, "r22r23" },
  { 7, 3, "r31r32" },
  { 7, 4, "r33" },
  { 7, 5, "trx" },
  { 7, 6, "try" },
  { 7, 7, "trz" },
  { 7, 8, "l11l12" },
  { 7, 9, "l13l21" },
  { 7, 10, "l22l23" },
  { 7, 11, "l31l32" },
  { 7, 12, "l33" },
  { 7, 13, "rbk" },
  { 7, 14, "gbk" },
  { 7, 15, "bbk" },
  { 7, 16, "lr1lr2" },
  { 7, 17, "lr3lg1" },
  { 7, 18, "lg2lg3" },
  { 7, 19, "lb1lb2" },
  { 7, 20, "lb3" },
  { 7, 21, "rfc" },
  { 7, 22, "gfc" },
  { 7, 23, "bfc" },
  { 7, 24, "ofx" },
  { 7, 25, "ofy" },
  { 7, 26, "h" },
  { 7, 27, "dqa" },
  { 7, 28, "dqb" },
  { 7, 29, "1_3" },
  { 7, 30, "1_4" },
  { 7, 31, "flag0" },
  { 0, 0, NULL }
};

extern unsigned int regbuf_vals[10][32]; // defined in dbg.c
extern unsigned int regbuf_mask[10]; // defined in dbg.c

static void __cdecl print_symcr(unsigned int rcr);
static void __cdecl print_rcr(unsigned int rcr);
static void __cdecl print_symsr(unsigned int rsr);
static void __cdecl print_rsr(unsigned int rsr);
static void __cdecl print_dcic(unsigned int dcic);
static int __cdecl set_regmasks(unsigned int *masks, char *name);
static int __cdecl dr_default_setmask(unsigned int *masks, int off, int bit);
static int __cdecl dr_default(char *fmt, unsigned int *masks, unsigned int *vals);

static void __cdecl print_symcr(unsigned int rcr)
{
  char *v1; // eax
  char *v2; // eax
  char *v3; // eax
  char *v4; // eax
  char *v5; // eax
  char *v6; // eax
  char *v7; // eax
  char *v8; // eax
  char *v9; // eax
  __int16 v10; // ax
  const char *v11; // eax
  const char *v12; // [esp-24h] [ebp-24h]
  const char *v13; // [esp-20h] [ebp-20h]
  const char *v14; // [esp-1Ch] [ebp-1Ch]
  const char *v15; // [esp-18h] [ebp-18h]
  const char *v16; // [esp-14h] [ebp-14h]
  const char *v17; // [esp-10h] [ebp-10h]
  const char *v18; // [esp-Ch] [ebp-Ch]
  const char *v19; // [esp-8h] [ebp-8h]
  const char *v20; // [esp-4h] [ebp-4h]

  if ( (rcr & 0x80000000) == 0 )
    ds_printf("");
  else
    ds_printf(" BD");
  if ( (rcr & 0x40000000) != 0 )
    ds_printf(" BT");
  else
    ds_printf("");
  ds_printf(" CE%d", (rcr & 0x30000000) >> 28);
  if ( (rcr & 0x100) != 0 )
    v1 = "0";
  else
    v1 = "";
  v20 = v1;
  if ( (rcr & 0x200) != 0 )
    v2 = "1";
  else
    v2 = "";
  v19 = v2;
  if ( (rcr & 0x300) != 0 )
    v3 = "S";
  else
    v3 = "";
  v18 = v3;
  if ( (rcr & 0x400) != 0 )
    v4 = "0";
  else
    v4 = "";
  v17 = v4;
  if ( (rcr & 0x800) != 0 )
    v5 = "1";
  else
    v5 = "";
  v16 = v5;
  if ( (rcr & 0x1000) != 0 )
    v6 = "2";
  else
    v6 = "";
  v15 = v6;
  if ( (rcr & 0x2000) != 0 )
    v7 = "3";
  else
    v7 = "";
  v14 = v7;
  if ( (rcr & 0x4000) != 0 )
    v8 = "4";
  else
    v8 = "";
  v13 = v8;
  if ( (rcr & 0x8000u) == 0 )
    v9 = "";
  else
    v9 = "5";
  v12 = v9;
  HIBYTE(v10) = BYTE1(rcr);
  LOBYTE(v10) = 0;
  if ( v10 )
    v11 = " IP";
  else
    v11 = "";
  ds_printf("%s%s%s%s%s%s%s%s%s%s", v11, v12, v13, v14, v15, v16, v17, v18, v19, v20);
  switch ( rcr & 0x3C )
  {
    case 0u:
      ds_printf(" External interrupt");
      break;
    case 0x10u:
      ds_printf(" Address error (load/fetch)");
      break;
    case 0x14u:
      ds_printf(" Address error (store)");
      break;
    case 0x18u:
      ds_printf(" Bus error (fetch)");
      break;
    case 0x1Cu:
      ds_printf(" Bus error (load/store)");
      break;
    case 0x20u:
      ds_printf(" Syscall");
      break;
    case 0x24u:
      ds_printf(" Breakpoint");
      break;
    case 0x28u:
      ds_printf(" Reserved instruction");
      break;
    case 0x2Cu:
      ds_printf(" Coprocessor unusable");
      break;
    case 0x30u:
      ds_printf(" Arithmetic overflow");
      break;
    default:
      ds_printf(" Exception code 0x%x", (unsigned __int8)(rcr & 0x3C) >> 2);
      break;
  }
  ds_printf(" ");
}

static void __cdecl print_rcr(unsigned int rcr)
{
  ds_printf("$cr=0x%W [", rcr);
  print_symcr(rcr);
  ds_printf("]");
}

static void __cdecl print_symsr(unsigned int rsr)
{
  char *v1; // eax
  char *v2; // eax
  char *v3; // eax
  char *v4; // eax
  const char *v5; // eax
  const char *v6; // eax
  const char *v7; // eax
  const char *v8; // eax
  const char *v9; // eax
  const char *v10; // eax
  char *v11; // eax
  char *v12; // eax
  char *v13; // eax
  char *v14; // eax
  char *v15; // eax
  char *v16; // eax
  char *v17; // eax
  char *v18; // eax
  char *v19; // eax
  __int16 v20; // ax
  const char *v21; // eax
  char *v22; // eax
  const char *v23; // eax
  char *v24; // eax
  const char *v25; // eax
  char *v26; // eax
  const char *v27; // eax
  const char *v28; // [esp-24h] [ebp-24h]
  const char *v29; // [esp-20h] [ebp-20h]
  const char *v30; // [esp-1Ch] [ebp-1Ch]
  const char *v31; // [esp-18h] [ebp-18h]
  const char *v32; // [esp-14h] [ebp-14h]
  const char *v33; // [esp-10h] [ebp-10h]
  const char *v34; // [esp-10h] [ebp-10h]
  const char *v35; // [esp-Ch] [ebp-Ch]
  const char *v36; // [esp-Ch] [ebp-Ch]
  const char *v37; // [esp-8h] [ebp-8h]
  const char *v38; // [esp-8h] [ebp-8h]
  const char *v39; // [esp-4h] [ebp-4h]
  const char *v40; // [esp-4h] [ebp-4h]
  const char *v41; // [esp-4h] [ebp-4h]
  const char *v42; // [esp-4h] [ebp-4h]
  const char *v43; // [esp-4h] [ebp-4h]

  if ( (rsr & 0x10000000) != 0 )
    v1 = "0";
  else
    v1 = "";
  v39 = v1;
  if ( (rsr & 0x20000000) != 0 )
    v2 = "1";
  else
    v2 = "";
  v37 = v2;
  if ( (rsr & 0x40000000) != 0 )
    v3 = "2";
  else
    v3 = "";
  v35 = v3;
  if ( (rsr & 0x80000000) == 0 )
    v4 = "";
  else
    v4 = "3";
  v33 = v4;
  if ( (rsr & 0xF0000000) != 0 )
    v5 = " Cu";
  else
    v5 = "";
  ds_printf("%s%s%s%s%s", v5, v33, v35, v37, v39);
  if ( (rsr & 0x400000) != 0 )
    v6 = " BEV";
  else
    v6 = "";
  ds_printf("%s", v6);
  if ( (rsr & 0x200000) != 0 )
    v7 = " TS";
  else
    v7 = "";
  ds_printf("%s", v7);
  if ( (rsr & 0x100000) != 0 )
    v8 = " PE";
  else
    v8 = "";
  ds_printf("%s", v8);
  if ( (rsr & 0x40000) != 0 )
    v9 = " PZ";
  else
    v9 = "";
  ds_printf("%s", v9);
  if ( (rsr & 0x10000) != 0 )
    v10 = " ISC";
  else
    v10 = "";
  ds_printf("%s", v10);
  if ( (rsr & 0x100) != 0 )
    v11 = "0";
  else
    v11 = "";
  v40 = v11;
  if ( (rsr & 0x200) != 0 )
    v12 = "1";
  else
    v12 = "";
  v38 = v12;
  if ( (rsr & 0x300) != 0 )
    v13 = "S";
  else
    v13 = "";
  v36 = v13;
  if ( (rsr & 0x400) != 0 )
    v14 = "0";
  else
    v14 = "";
  v34 = v14;
  if ( (rsr & 0x800) != 0 )
    v15 = "1";
  else
    v15 = "";
  v32 = v15;
  if ( (rsr & 0x1000) != 0 )
    v16 = "2";
  else
    v16 = "";
  v31 = v16;
  if ( (rsr & 0x2000) != 0 )
    v17 = "3";
  else
    v17 = "";
  v30 = v17;
  if ( (rsr & 0x4000) != 0 )
    v18 = "4";
  else
    v18 = "";
  v29 = v18;
  if ( (rsr & 0x8000u) == 0 )
    v19 = "";
  else
    v19 = "5";
  v28 = v19;
  HIBYTE(v20) = BYTE1(rsr);
  LOBYTE(v20) = 0;
  if ( v20 )
    v21 = " IM";
  else
    v21 = "";
  ds_printf("%s%s%s%s%s%s%s%s%s%s", v21, v28, v29, v30, v31, v32, v34, v36, v38, v40);
  if ( (rsr & 0x30) != 0 )
  {
    if ( (rsr & 0x10) != 0 )
      v22 = "IE";
    else
      v22 = "";
    v41 = v22;
    if ( (rsr & 0x20) != 0 )
      v23 = "KU";
    else
      v23 = "";
    ds_printf(" %s%so", v23, v41);
  }
  if ( (rsr & 0xC) != 0 )
  {
    if ( (rsr & 4) != 0 )
      v24 = "IE";
    else
      v24 = "";
    v42 = v24;
    if ( (rsr & 8) != 0 )
      v25 = "KU";
    else
      v25 = "";
    ds_printf(" %s%sp", v25, v42);
  }
  if ( (rsr & 3) != 0 )
  {
    if ( (rsr & 1) != 0 )
      v26 = "IE";
    else
      v26 = "";
    v43 = v26;
    if ( (rsr & 2) != 0 )
      v27 = "KU";
    else
      v27 = "";
    ds_printf(" %s%sc", v27, v43);
  }
  ds_printf(" ");
}

static void __cdecl print_rsr(unsigned int rsr)
{
  ds_printf("$sr=0x%W [", rsr);
  print_symsr(rsr);
  ds_printf("]");
}

static void __cdecl print_dcic(unsigned int dcic)
{
  const char *v1; // eax
  const char *v2; // eax
  const char *v3; // eax
  const char *v4; // eax
  const char *v5; // eax
  const char *v6; // eax
  const char *v7; // eax
  const char *v8; // eax
  const char *v9; // eax
  const char *v10; // eax
  const char *v11; // eax
  const char *v12; // eax
  const char *v13; // eax
  const char *v14; // eax
  const char *v15; // eax

  ds_printf("$dcic=0x%W [", dcic);
  if ( (dcic & 0x80000000) == 0 )
    v1 = "";
  else
    v1 = " TR";
  ds_printf("%s", v1);
  if ( (dcic & 0x40000000) != 0 )
    v2 = " UD";
  else
    v2 = "";
  ds_printf("%s", v2);
  if ( (dcic & 0x20000000) != 0 )
    v3 = " KD";
  else
    v3 = "";
  ds_printf("%s", v3);
  if ( (dcic & 0x10000000) != 0 )
    v4 = " TE";
  else
    v4 = "";
  ds_printf("%s", v4);
  if ( (dcic & 0x8000000) != 0 )
    v5 = " DW";
  else
    v5 = "";
  ds_printf("%s", v5);
  if ( (dcic & 0x4000000) != 0 )
    v6 = " DR";
  else
    v6 = "";
  ds_printf("%s", v6);
  if ( (dcic & 0x2000000) != 0 )
    v7 = " DAE";
  else
    v7 = "";
  ds_printf("%s", v7);
  if ( (dcic & 0x1000000) != 0 )
    v8 = " PCE";
  else
    v8 = "";
  ds_printf("%s", v8);
  if ( (dcic & 0x800000) != 0 )
    v9 = " DE";
  else
    v9 = "";
  ds_printf("%s", v9);
  if ( (dcic & 0x20) != 0 )
    v10 = " T";
  else
    v10 = "";
  ds_printf("%s", v10);
  if ( (dcic & 0x10) != 0 )
    v11 = " W";
  else
    v11 = "";
  ds_printf("%s", v11);
  if ( (dcic & 8) != 0 )
    v12 = " R";
  else
    v12 = "";
  ds_printf("%s", v12);
  if ( (dcic & 4) != 0 )
    v13 = " DA";
  else
    v13 = "";
  ds_printf("%s", v13);
  if ( (dcic & 2) != 0 )
    v14 = " PC";
  else
    v14 = "";
  ds_printf("%s", v14);
  if ( (dcic & 1) != 0 )
    v15 = " DB";
  else
    v15 = "";
  ds_printf("%s", v15);
  ds_printf(" ]");
}

static int __cdecl set_regmasks(unsigned int *masks, char *name)
{
  int v3; // eax
  int rf; // [esp+18h] [ebp-10h]
  int n; // [esp+1Ch] [ebp-Ch] BYREF
  IOP_REGS *reg; // [esp+20h] [ebp-8h]
  char *p; // [esp+24h] [ebp-4h]

  p = name;
  n = 0;
  rf = 0;
  if ( !strcmp("all", name) )
  {
    rf = 206;
  }
  else if ( !strcmp("hl", name) )
  {
    rf = 2;
  }
  else if ( !strcmp("gpr", name) )
  {
    rf = 4;
  }
  else if ( !strcmp("scc", name)
         || !strcmp("scr", name)
         || !strcmp("c0r", name)
         || !strcmp("cop0", name)
         || !strcmp("cp0", name) )
  {
    rf = 8;
  }
  else if ( !strcmp("c2r", name) )
  {
    rf = 64;
  }
  else if ( !strcmp("c2c", name) )
  {
    rf = 128;
  }
  else if ( !strcmp("gte", name) || !strcmp("cop2", name) || !strcmp("cp2", name) )
  {
    rf = 192;
  }
  if ( rf )
  {
    for ( reg = iop_regs; reg->name; ++reg )
    {
      if ( ((rf >> reg->off) & 1) != 0 )
      {
        masks[reg->off] |= 1 << reg->bit;
        ++n;
      }
    }
    return n;
  }
  else
  {
    if ( *p == 36 )
      ++p;
    if ( *p == 95 )
      ++p;
    if ( !strcmp("cause", p) )
      p = "cr";
    for ( reg = iop_regs; reg->name; ++reg )
    {
      if ( !strcmp(p, reg->name) )
      {
        masks[reg->off] |= 1 << reg->bit;
        return 1;
      }
    }
    v3 = 0;
    if ( *p > 47 && *p <= 57 )
      v3 = 1;
    if ( v3 && !ds_scan_digit_word(p, (unsigned int *)&n) )
    {
      for ( reg = iop_regs; reg->name; ++reg )
      {
        if ( reg->off == 2 && n == reg->bit )
        {
          masks[reg->off] |= 1 << reg->bit;
          return 1;
        }
      }
    }
    return ds_error("invalid register name - %s", name);
  }
}

int __cdecl check_reserved_name(char *name)
{
  IOP_REGS *reg; // [esp+0h] [ebp-4h]

  if ( !ds_strncmp("BP", name, 2) )
    return 1;
  if ( !ds_strncmp("BT", name, 2) )
    return 1;
  if ( !ds_strncmp("SFA", name, 3) )
    return 1;
  if ( !ds_strncmp("SFS", name, 3) )
    return 1;
  if ( !strcmp("REBOOTCOUNT", name) )
    return 1;
  if ( !strcmp("cause", name) )
    return 1;
  for ( reg = iop_regs; reg->name; ++reg )
  {
    if ( !strcmp(name, reg->name) )
      return 1;
  }
  return 0;
}

int __cdecl load_word_reg(char *name, unsigned int *pv)
{
  unsigned int v3; // edx
  int j; // [esp+8h] [ebp-38h]
  int i; // [esp+Ch] [ebp-34h]
  unsigned int vals[2]; // [esp+10h] [ebp-30h] BYREF
  unsigned int masks[10]; // [esp+18h] [ebp-28h] BYREF

  ds_bzero(masks, sizeof(masks));
  if ( !ds_strncmp("$BP", name, 3) )
    return eval_bp_reg(name, pv);
  if ( !ds_strncmp("$BT", name, 3) )
    return eval_bt_reg(name, pv);
  if ( !ds_strncmp("$SFA", name, 4) )
    return eval_sfa_reg(name, pv);
  if ( !ds_strncmp("$SFS", name, 4) )
    return eval_sfs_reg(name, pv);
  if ( !strcmp("$REBOOTCOUNT", name) )
  {
    *pv = dsc_connected;
    return 0;
  }
  if ( !strcmp("$PC", name) || !strcmp("$_PC", name) )
  {
    masks[3] = 24576;
    if ( name[1] == 95 && masks[3] == (masks[3] & regbuf_mask[3]) )
    {
      *(_QWORD *)vals = *(_QWORD *)&regbuf_vals[3][13];
    }
    else if ( load_word_registers(masks, vals, 2) )
    {
      return -1;
    }
    v3 = vals[1];
    if ( (vals[0] & 0x80000000) != 0 )
      v3 = vals[1] + 4;
    *pv = v3;
    return 0;
  }
  else if ( set_regmasks(masks, name) == 1 )
  {
    if ( *name == 36 && name[1] == 95 )
    {
      for ( i = 0; i <= 9; ++i )
      {
        if ( masks[i] )
        {
          if ( (masks[i] & regbuf_mask[i]) != 0 )
          {
            for ( j = 0; j <= 31; ++j )
            {
              if ( (masks[i] & (1 << j)) != 0 )
              {
                *pv = regbuf_vals[i][j];
                return 0;
              }
            }
          }
          return load_word_registers(masks, pv, 1);
        }
      }
    }
    return load_word_registers(masks, pv, 1);
  }
  else
  {
    return -1;
  }
}

int __cdecl store_word_reg(char *name, unsigned int val)
{
  int n; // [esp+0h] [ebp-530h]
  int i; // [esp+4h] [ebp-52Ch]
  unsigned int vals[320]; // [esp+8h] [ebp-528h] BYREF
  unsigned int masks[10]; // [esp+508h] [ebp-28h] BYREF

  ds_bzero(masks, sizeof(masks));
  n = set_regmasks(masks, name);
  if ( n <= 0 )
    return -1;
  for ( i = 0; n > i; ++i )
    vals[i] = val;
  return store_word_registers(masks, vals, n);
}

int __cdecl load_quad_reg(char *name, quad *pv)
{
  quad v3; // [esp+0h] [ebp-14h] BYREF
  unsigned int wv; // [esp+10h] [ebp-4h] BYREF

  if ( load_word_reg(name, &wv) )
    return -1;
  ds_cwq(&v3, wv);
  *pv = v3;
  return 0;
}

int __cdecl store_quad_reg(char *name, quad val)
{
  unsigned int v3; // [esp-4h] [ebp-4h]

  v3 = ds_cqw(val);
  return store_word_reg(name, v3);
}

static int __cdecl dr_default_setmask(unsigned int *masks, int off, int bit)
{
  if ( (masks[off] & (1 << bit)) != 0 )
    return 0;
  masks[off] |= 1 << bit;
  return 1;
}

static int __cdecl dr_default(char *fmt, unsigned int *masks, unsigned int *vals)
{
  char *v3; // eax
  int v4; // eax
  int v5; // eax
  int v6; // eax
  unsigned int v8; // eax
  int v9; // eax
  int v10; // eax
  int v11; // eax
  int v12; // [esp-4h] [ebp-2Ch]
  char _c_4; // [esp+12h] [ebp-16h]
  char _c_5; // [esp+12h] [ebp-16h]
  char _c; // [esp+13h] [ebp-15h]
  char _c_3; // [esp+13h] [ebp-15h]
  int ch; // [esp+14h] [ebp-14h]
  int nreg; // [esp+18h] [ebp-10h]
  int nreg_1; // [esp+18h] [ebp-10h]
  int n; // [esp+1Ch] [ebp-Ch]
  int n_1; // [esp+1Ch] [ebp-Ch]
  IOP_REGS *reg; // [esp+20h] [ebp-8h]
  IOP_REGS *reg_1; // [esp+20h] [ebp-8h]
  char *p; // [esp+24h] [ebp-4h]
  char *p_1; // [esp+24h] [ebp-4h]

  nreg = 0;
  p = fmt;
  while ( *p )
  {
    v3 = p++;
    if ( *v3 == 37 && *p != 37 )
    {
      n = 2;
      if ( ds_strncmp("PC", p, 2) )
      {
        n = 5;
        if ( ds_strncmp("symcr", p, 5) )
        {
          n = 5;
          if ( ds_strncmp("symsr", p, 5) )
          {
            for ( reg = iop_regs; reg->name; ++reg )
            {
              n = strlen(reg->name);
              if ( !ds_strncmp(p, reg->name, n) )
              {
                _c = p[n];
                v4 = 0;
                if ( _c > 47 && _c <= 57 )
                  v4 = 1;
                if ( !v4 )
                {
                  nreg += dr_default_setmask(masks, reg->off, reg->bit);
                  break;
                }
              }
            }
            if ( !reg->name )
              n = 0;
          }
          else
          {
            nreg += dr_default_setmask(masks, 3, 12);
          }
        }
        else
        {
          nreg += dr_default_setmask(masks, 3, 13);
        }
      }
      else
      {
        nreg_1 = dr_default_setmask(masks, 3, 13) + nreg;
        nreg = dr_default_setmask(masks, 3, 14) + nreg_1;
      }
      if ( n <= 0 )
      {
        while ( 1 )
        {
          if ( *p != 95 )
          {
            _c_3 = *p;
            v5 = 0;
            if ( *p > 96 && _c_3 <= 122 || _c_3 > 64 && _c_3 <= 90 )
              v5 = 1;
            if ( !v5 )
            {
              v6 = 0;
              if ( *p > 47 && *p <= 57 )
                v6 = 1;
              if ( !v6 )
                break;
            }
          }
          ++p;
        }
      }
      else
      {
        p += n;
      }
    }
  }
  if ( load_word_registers(masks, vals, nreg) )
    return -1;
  p_1 = fmt;
  while ( *p_1 )
  {
    ch = *p_1++;
    if ( ch == 37 )
    {
      if ( *p_1 == 37 )
      {
        ds_printf("%c", 37);
      }
      else
      {
        n_1 = 2;
        if ( ds_strncmp("PC", p_1, 2) )
        {
          n_1 = 5;
          if ( ds_strncmp("symcr", p_1, 5) )
          {
            n_1 = 5;
            if ( ds_strncmp("symsr", p_1, 5) )
            {
              for ( reg_1 = iop_regs; reg_1->name; ++reg_1 )
              {
                n_1 = strlen(reg_1->name);
                if ( !ds_strncmp(p_1, reg_1->name, n_1) )
                {
                  _c_4 = p_1[n_1];
                  v9 = 0;
                  if ( _c_4 > 47 && _c_4 <= 57 )
                    v9 = 1;
                  if ( !v9 )
                  {
                    ds_printf("%W", regbuf_vals[reg_1->off][reg_1->bit]);
                    break;
                  }
                }
              }
              if ( !reg_1->name )
                n_1 = 0;
            }
            else
            {
              print_symsr(regbuf_vals[3][12]);
            }
          }
          else
          {
            print_symcr(regbuf_vals[3][13]);
          }
        }
        else
        {
          v8 = regbuf_vals[3][14];
          if ( (regbuf_vals[3][13] & 0x80000000) != 0 )
            v8 = regbuf_vals[3][14] + 4;
          ds_printf("%W", v8);
        }
        if ( n_1 <= 0 )
        {
          ds_printf("%%");
          while ( 1 )
          {
            if ( *p_1 != 95 )
            {
              _c_5 = *p_1;
              v10 = 0;
              if ( *p_1 > 96 && _c_5 <= 122 || _c_5 > 64 && _c_5 <= 90 )
                v10 = 1;
              if ( !v10 )
              {
                v11 = 0;
                if ( *p_1 > 47 && *p_1 <= 57 )
                  v11 = 1;
                if ( !v11 )
                  break;
              }
            }
            v12 = *p_1++;
            ds_printf("%c", v12);
          }
        }
        else
        {
          p_1 += n_1;
        }
      }
    }
    else
    {
      ds_printf("%c", ch);
    }
  }
  return 0;
}

int __cdecl dreg_cmd(int ac, char **av)
{
  int v3; // eax
  int v4; // eax
  int v5; // [esp-4h] [ebp-568h]
  char *fmt; // [esp+4h] [ebp-560h]
  int v7; // [esp+Ch] [ebp-558h]
  unsigned int rcr; // [esp+10h] [ebp-554h]
  unsigned int rsr; // [esp+14h] [ebp-550h]
  unsigned int v10; // [esp+18h] [ebp-54Ch]
  IOP_REGS *reg; // [esp+24h] [ebp-540h]
  IOP_REGS *reg_1; // [esp+24h] [ebp-540h]
  int last_off; // [esp+28h] [ebp-53Ch]
  int i; // [esp+2Ch] [ebp-538h]
  int n; // [esp+30h] [ebp-534h]
  int n_1; // [esp+30h] [ebp-534h]
  int r; // [esp+34h] [ebp-530h]
  unsigned int *pv; // [esp+38h] [ebp-52Ch]
  unsigned int vals[320]; // [esp+3Ch] [ebp-528h] BYREF
  unsigned int masks[10]; // [esp+53Ch] [ebp-28h] BYREF
  int aca; // [esp+56Ch] [ebp+8h]
  char **ava; // [esp+570h] [ebp+Ch]

  pv = vals;
  last_off = -1;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  ava = av + 1;
  if ( aca > 0 && **ava == 45 )
    return ds_error("Usage: dr [<reg>]...");
  ds_bzero(masks, sizeof(masks));
  if ( aca > 0 )
  {
    n = 0;
    while ( aca > 0 )
    {
      r = set_regmasks(masks, *ava);
      if ( r <= 0 )
        return -1;
      n += r;
      --aca;
      ++ava;
    }
    if ( load_word_registers(masks, vals, n) )
      return -1;
    if ( n <= 1 )
    {
      for ( reg_1 = iop_regs; reg_1->name && (masks[reg_1->off] & (1 << reg_1->bit)) == 0; ++reg_1 )
        ;
      if ( !reg_1->name )
        return -1;
      if ( reg_1->off == 3 && reg_1->bit == 13 )
      {
        print_rcr(vals[0]);
      }
      else if ( reg_1->off == 3 && reg_1->bit == 12 )
      {
        print_rsr(vals[0]);
      }
      else if ( reg_1->off == 3 && reg_1->bit == 7 )
      {
        print_dcic(vals[0]);
      }
      else if ( reg_1->off == 2 )
      {
        ds_printf("$%s($%d)=0x%W", reg_1->name, reg_1->bit, vals[0]);
      }
      else
      {
        ds_printf("$%s=0x%W", reg_1->name, vals[0]);
      }
      ds_printf("\n");
    }
    else
    {
      LOBYTE(i) = 0;
      for ( reg = iop_regs; reg->name; ++reg )
      {
        if ( (masks[reg->off] & (1 << reg->bit)) != 0 )
        {
          if ( last_off >= 0 && reg->off != last_off && (i & 3) != 0 )
          {
            ds_printf("\n");
            LOBYTE(i) = 0;
          }
          n_1 = 6 - strlen(reg->name);
          while ( 1 )
          {
            v3 = n_1--;
            if ( v3 <= 0 )
              break;
            ds_printf(" ");
          }
          v5 = *pv++;
          ds_printf(" $%s=0x%W", reg->name, v5);
          v4 = i & 3;
          LOBYTE(i) = i + 1;
          if ( v4 == 3 )
            ds_printf("\n");
          last_off = reg->off;
        }
      }
      if ( (i & 3) != 0 )
        ds_printf("\n");
    }
    return 0;
  }
  else
  {
    fmt = dr_format_str();
    if ( fmt && *fmt )
    {
      if ( dr_default(fmt, masks, vals) )
      {
        return -1;
      }
      else
      {
        dr_default_di();
        return 0;
      }
    }
    else
    {
      masks[1] = 3;
      masks[2] = -2;
      masks[3] = 28928;
      if ( load_word_registers(masks, vals, 37) )
      {
        return -1;
      }
      else
      {
        v10 = vals[33];
        rsr = vals[34];
        rcr = vals[35];
        v7 = vals[36];
        ds_printf(
          " at=%W  v0-1=%W,%W  a0-3=%W,%W,%W,%W\n",
          vals[2],
          vals[3],
          vals[4],
          vals[5],
          vals[6],
          vals[7],
          vals[8]);
        ds_printf(
          " t0-7=%W,%W,%W,%W, %W,%W,%W,%W\n",
          vals[9],
          vals[10],
          vals[11],
          vals[12],
          vals[13],
          vals[14],
          vals[15],
          vals[16]);
        ds_printf(
          " s0-7=%W,%W,%W,%W, %W,%W,%W,%W\n",
          vals[17],
          vals[18],
          vals[19],
          vals[20],
          vals[21],
          vals[22],
          vals[23],
          vals[24]);
        ds_printf(
          " t8=%W t9=%W   k0=%W k1=%W   gp=%W sp=%W\n",
          vals[25],
          vals[26],
          vals[27],
          vals[28],
          vals[29],
          vals[30]);
        ds_printf(
          " fp=%W ra=%W   lo=%W hi=%W   PC=%W bada=%W\n",
          vals[31],
          vals[32],
          vals[0],
          vals[1],
          v7 + (v7 < 0 ? 4 : 0),
          v10);
        ds_printf(" ");
        print_rcr(rcr);
        ds_printf("\n");
        ds_printf(" ");
        print_rsr(rsr);
        ds_printf("\n");
        dr_default_di();
        return 0;
      }
    }
  }
}

int __cdecl sreg_cmd(int ac, char **av)
{
  int f_force; // [esp+0h] [ebp-8h]
  unsigned int wv; // [esp+4h] [ebp-4h] BYREF
  int aca; // [esp+10h] [ebp+8h]
  char **ava; // [esp+14h] [ebp+Ch]

  f_force = 0;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  for ( ava = av + 1; aca > 0 && **ava == 45; ++ava )
  {
    if ( strcmp("-f", *ava) )
      return ds_error("Usage: sr [-f] [<reg> <val>]...");
    ++f_force;
    --aca;
  }
  if ( aca <= 0 || (aca & 1) != 0 )
    return ds_error("Usage: sr [-f] [<reg> <val>]...");
  if ( !f_force && is_target_is_running() )
    return ds_error("target is not stopped state");
  while ( aca > 0 )
  {
    if ( ds_eval_word(ava[1], &wv) )
      return -1;
    if ( store_word_reg(*ava, wv) )
      return -1;
    aca -= 2;
    ava += 2;
  }
  return 0;
}

int __cdecl hbp_cmd(int ac, char **av)
{
  char *v3; // eax
  char *v4; // eax
  char *v5; // eax
  char *v6; // eax
  char *v7; // eax
  char *v8; // eax
  char *v9; // [esp-Ch] [ebp-C64h]
  char *v10; // [esp-Ch] [ebp-C64h]
  int f_debug; // [esp+4h] [ebp-C54h]
  int hub; // [esp+8h] [ebp-C50h]
  unsigned int msk; // [esp+Ch] [ebp-C4Ch] BYREF
  unsigned int adr; // [esp+10h] [ebp-C48h] BYREF
  char *q; // [esp+14h] [ebp-C44h]
  char *p; // [esp+18h] [ebp-C40h]
  char msks[1024]; // [esp+1Ch] [ebp-C3Ch] BYREF
  char adrs[1024]; // [esp+41Ch] [ebp-83Ch] BYREF
  char type[1024]; // [esp+81Ch] [ebp-43Ch] BYREF
  unsigned int regs[5]; // [esp+C1Ch] [ebp-3Ch] BYREF
  unsigned int masks[10]; // [esp+C30h] [ebp-28h] BYREF
  int aca; // [esp+C60h] [ebp+8h]
  char **ava; // [esp+C64h] [ebp+Ch]

  hub = 0;
  f_debug = 0;
  if ( ac <= 0 )
    return 0;
  if ( !strcmp("hub", *av) )
    hub = 1;
  aca = ac - 1;
  for ( ava = av + 1; aca > 0 && **ava == 45; ++ava )
  {
    if ( strcmp("-debug", *ava) )
    {
LABEL_111:
      if ( hub )
        return ds_error("Usage: hub [{pc|da|dr|dw}]...");
      else
        return ds_error("Usage: hbp [{pc|da|dr|dw}[ku]*[:<adr>[,<msk>]]]...");
    }
    ++f_debug;
    --aca;
  }
  ds_bzero(masks, sizeof(masks));
  masks[3] = 2728;
  if ( load_word_registers(masks, regs, 5) )
    return -1;
  if ( f_debug )
  {
    ds_printf("load:\n");
    print_dcic(regs[2]);
    ds_printf("\n");
    ds_printf("$bpc =0x%W $bpcm=0x%W\n", regs[0], regs[4]);
    ds_printf("$bda =0x%W $bdam=0x%W\n", regs[1], regs[3]);
  }
  if ( aca )
  {
    regs[2] |= 0x80800000;
    while ( aca > 0 )
    {
      p = *ava;
      q = type;
      while ( *p && *p != 58 )
        *q++ = *p++;
      *q = 0;
      v7 = p;
      if ( *p == 58 )
        v7 = p + 1;
      p = v7;
      q = adrs;
      while ( *p && *p != 44 )
        *q++ = *p++;
      *q = 0;
      v8 = p;
      if ( *p == 44 )
        v8 = p + 1;
      p = v8;
      q = msks;
      while ( *p )
        *q++ = *p++;
      *q = 0;
      if ( hub )
      {
        if ( !ds_strncmp("pc", type, 2) )
        {
          regs[2] &= ~0x1000000u;
          goto LABEL_106;
        }
        if ( type[0] == 100 )
        {
          regs[2] &= ~0x2000000u;
          goto LABEL_106;
        }
        goto LABEL_111;
      }
      if ( ds_strncmp("pc", type, 2) )
      {
        if ( type[0] != 100 || !type[1] )
          goto LABEL_111;
        regs[2] |= 0xE000000u;
        switch ( type[1] )
        {
          case 'r':
            regs[2] &= ~0x8000000u;
            break;
          case 'w':
            regs[2] &= ~0x4000000u;
            break;
          case 'a':
            break;
          default:
            goto LABEL_111;
        }
        adr = regs[1];
        msk = regs[3];
        if ( adrs[0] )
        {
          if ( ds_eval_word(adrs, &adr) )
            return -1;
          msk = -1;
          if ( msks[0] )
          {
            if ( ds_eval_word(msks, &msk) )
              return -1;
          }
        }
        regs[1] = adr;
        regs[3] = msk;
      }
      else
      {
        regs[2] |= 0x1000000u;
        adr = regs[0];
        msk = regs[4];
        if ( adrs[0] )
        {
          if ( ds_eval_word(adrs, &adr) )
            return -1;
          msk = -1;
          if ( msks[0] )
          {
            if ( ds_eval_word(msks, &msk) )
              return -1;
          }
        }
        regs[0] = adr;
        regs[4] = msk;
      }
      p = &type[2];
      if ( type[2] )
      {
        regs[2] &= 0x9FFFFFFF;
        while ( *p )
        {
          if ( *p == 117 )
          {
            regs[2] |= 0x40000000u;
          }
          else
          {
            if ( *p != 107 )
              goto LABEL_111;
            regs[2] |= 0x20000000u;
          }
          ++p;
        }
      }
      --aca;
      ++ava;
    }
    goto LABEL_106;
  }
  if ( hub )
  {
    regs[2] &= 0xFCFFFFFF;
LABEL_106:
    if ( f_debug )
    {
      ds_printf("store regs:\n");
      print_dcic(regs[2]);
      ds_printf("\n");
      ds_printf("$bpc =0x%W $bpcm=0x%W\n", regs[0], regs[4]);
      ds_printf("$bda =0x%W $bdam=0x%W\n", regs[1], regs[3]);
    }
    if ( store_word_registers(masks, regs, 5) )
      return -1;
    else
      return 0;
  }
  if ( (regs[2] & 0x80000000) == 0 )
    ds_printf("Trap Disabled\n");
  if ( (regs[2] & 0x800000) == 0 )
    ds_printf("Debug Disabled\n");
  if ( (regs[2] & 0x3000000) == 0 )
    return 0;
  if ( (regs[2] & 0x1000000) != 0 )
  {
    if ( (regs[2] & 0x20000000) != 0 )
      v3 = "k";
    else
      v3 = "";
    v9 = v3;
    if ( (regs[2] & 0x40000000) != 0 )
      v4 = "u";
    else
      v4 = "";
    ds_printf(" pc%s%s:%W,%W", v4, v9, regs[0], regs[4]);
  }
  if ( (regs[2] & 0x2000000) != 0 )
  {
    ds_printf(" d");
    switch ( regs[2] & 0xC000000 )
    {
      case 0xC000000u:
        ds_printf("a");
        break;
      case 0x4000000u:
        ds_printf("r");
        break;
      case 0x8000000u:
        ds_printf("w");
        break;
    }
    if ( (regs[2] & 0x20000000) != 0 )
      v5 = "k";
    else
      v5 = "";
    v10 = v5;
    if ( (regs[2] & 0x40000000) != 0 )
      v6 = "u";
    else
      v6 = "";
    ds_printf("%s%s:%W,%W", v6, v10, regs[1], regs[3]);
  }
  ds_printf("\n");
  return 0;
}

int __cdecl rload_cmd(int ac, char **av)
{
  int v3; // eax
  char *v4; // eax
  char *v5; // eax
  char *v6; // eax
  char _c; // [esp+Bh] [ebp-127Dh]
  int n_3; // [esp+Ch] [ebp-127Ch]
  int n_1; // [esp+Ch] [ebp-127Ch]
  int siz; // [esp+10h] [ebp-1278h] BYREF
  int r; // [esp+14h] [ebp-1274h]
  IOP_REGS *reg; // [esp+18h] [ebp-1270h]
  unsigned int val; // [esp+1Ch] [ebp-126Ch] BYREF
  unsigned int tvals[320]; // [esp+20h] [ebp-1268h] BYREF
  unsigned int avals[320]; // [esp+520h] [ebp-D68h]
  unsigned int tmasks[10]; // [esp+A20h] [ebp-868h] BYREF
  unsigned int amasks[10]; // [esp+A48h] [ebp-840h] BYREF
  void *stream; // [esp+A70h] [ebp-818h]
  char vstr[1024]; // [esp+A74h] [ebp-814h] BYREF
  char rstr[1024]; // [esp+E74h] [ebp-414h] BYREF
  char *q; // [esp+1274h] [ebp-14h]
  char *pe; // [esp+1278h] [ebp-10h]
  char *p; // [esp+127Ch] [ebp-Ch]
  char *buf; // [esp+1280h] [ebp-8h]
  char *fname; // [esp+1284h] [ebp-4h]
  int aca; // [esp+1290h] [ebp+8h]
  char **ava; // [esp+1294h] [ebp+Ch]

  r = -1;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  ava = av + 1;
  if ( aca > 0 && **ava == 45 )
    return ds_error("Usage: rload <fname>");
  fname = *ava;
  if ( aca != 1 )
    return ds_error("Usage: rload <fname>");
  if ( ds_fsize(fname, &siz) < 0 )
    return -1;
  stream = ds_fopen(fname, "r");
  if ( !stream )
    return -1;
  buf = (char *)ds_fload(stream, 0, 0, 1, siz);
  ds_fclose(stream);
  if ( !buf )
    return -1;
  ds_bzero(amasks, sizeof(amasks));
  p = buf;
  pe = &buf[siz];
  while ( pe > p )
  {
    while ( pe > p )
    {
      _c = *p;
      v3 = 0;
      if ( *p == 32 || _c == 12 || _c == 10 || _c == 13 || _c == 9 || _c == 11 )
        v3 = 1;
      if ( !v3 )
        break;
      ++p;
    }
    if ( *p == 10 || *p == 35 || *p == 59 || *p == 47 && p[1] == 47 )
    {
      while ( pe > p && *p != 10 )
        ++p;
      v4 = p;
      if ( *p == 10 )
        v4 = p + 1;
      p = v4;
    }
    else
    {
      q = rstr;
      while ( pe > p && *p != 10 && *p != 61 )
        *q++ = *p++;
      *q = 0;
      v5 = p;
      if ( pe > p && *p == 61 )
        v5 = p + 1;
      p = v5;
      q = vstr;
      while ( *p != 10 )
        *q++ = *p++;
      *q = 0;
      v6 = p;
      if ( pe > p && *p == 10 )
        v6 = p + 1;
      p = v6;
      ds_bzero(tmasks, sizeof(tmasks));
      if ( set_regmasks(tmasks, rstr) <= 0 || ds_eval_word(vstr, &val) )
        goto LABEL_72;
      for ( reg = iop_regs; reg->name; ++reg )
      {
        if ( (tmasks[reg->off] & (1 << reg->bit)) != 0 )
          avals[32 * reg->off + reg->bit] = val;
      }
      for ( n_3 = 0; n_3 <= 9; ++n_3 )
        amasks[n_3] |= tmasks[n_3];
    }
  }
  n_1 = 0;
  for ( reg = iop_regs; reg->name; ++reg )
  {
    if ( (amasks[reg->off] & (1 << reg->bit)) != 0 )
      tvals[n_1++] = avals[32 * reg->off + reg->bit];
  }
  if ( !store_word_registers(amasks, tvals, n_1) )
    r = 0;
LABEL_72:
  ds_free_mem_low(buf, "ireg.c", "rload_cmd");
  return r;
}

int __cdecl rsave_cmd(int ac, char **av)
{
  int v3; // eax
  int word_registers; // eax
  int v5; // eax
  unsigned int v6; // [esp-4h] [ebp-944h]
  void *v7; // [esp-4h] [ebp-944h]
  int n; // [esp+4h] [ebp-93Ch]
  int r; // [esp+8h] [ebp-938h]
  int r_1; // [esp+8h] [ebp-938h]
  IOP_REGS *reg; // [esp+Ch] [ebp-934h]
  unsigned int vals[320]; // [esp+10h] [ebp-930h] BYREF
  unsigned int masks[10]; // [esp+510h] [ebp-430h] BYREF
  void *stream; // [esp+538h] [ebp-408h]
  char buf[1024]; // [esp+53Ch] [ebp-404h] BYREF
  char *fname; // [esp+93Ch] [ebp-4h]
  int aca; // [esp+948h] [ebp+8h]
  int acb; // [esp+948h] [ebp+8h]
  char **ava; // [esp+94Ch] [ebp+Ch]
  char **avb; // [esp+94Ch] [ebp+Ch]

  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  ava = av + 1;
  if ( aca > 0 && **ava == 45 )
    return ds_error("Usage: rsave <fname> [<reg>]...");
  fname = *ava;
  avb = ava + 1;
  v3 = aca;
  acb = aca - 1;
  if ( v3 <= 0 )
    return ds_error("Usage: rsave <fname> [<reg>]...");
  ds_bzero(masks, sizeof(masks));
  if ( acb > 0 )
  {
    n = 0;
    while ( acb > 0 )
    {
      r = set_regmasks(masks, *avb);
      if ( r <= 0 )
        return -1;
      --acb;
      ++avb;
      n += r;
    }
    word_registers = load_word_registers(masks, vals, n);
  }
  else
  {
    masks[1] = 3;
    masks[2] = -2;
    masks[3] = 28672;
    word_registers = load_word_registers(masks, vals, 36);
  }
  if ( word_registers )
    return -1;
  stream = ds_fopen(fname, "w");
  if ( !stream )
    return -1;
  r_1 = 0;
  for ( reg = iop_regs; reg->name; ++reg )
  {
    if ( (masks[reg->off] & (1 << reg->bit)) != 0 )
    {
      v6 = vals[r_1++];
      ds_sprintf(buf, "$%s=0x%W\n", reg->name, v6);
      v7 = stream;
      v5 = strlen(buf);
      if ( ds_fwrite(buf, v5, 1, v7) != 1 )
        break;
    }
  }
  ds_fclose(stream);
  return 0;
}

