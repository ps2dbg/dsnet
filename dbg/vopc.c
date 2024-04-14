
#include "dsxdb_prototypes.h"

extern unsigned int dot; // defined in mem.c
static VOPCODE vopcodes_upper[] =
{
  { "ABS&dest    vft,vfs", 0x060007ff, 0x000001fd },
  { "ADD&dest    vfd,vfs,vft", 0x0600003f, 0x00000028 },
  { "ADDI&dest   vfd,vfs,I", 0x061f003f, 0x00000022 },
  { "ADDQ&dest   vfd,vfs,Q", 0x061f003f, 0x00000020 },
  { "ADD*&dest   vfd,vfs,vft", 0x0600003c, 0x00000000 },
  { "ADDA&dest   ACC,vft,vfs", 0x060007ff, 0x000002bc },
  { "ADDAI&dest  ACC,vfs,I", 0x061f07ff, 0x0000023e },
  { "ADDAQ&dest  ACC,vfs,Q", 0x061f07ff, 0x0000023c },
  { "ADDA*&dest  ACC,vfs,vft", 0x060007fc, 0x0000003c },
  { "CLIP*&dest  vfs,vft", 0x07e007ff, 0x01c001ff },
  { "FTOI0&dest  vft,vfs", 0x060007ff, 0x0000017c },
  { "FTOI4&dest  vft,vfs", 0x060007ff, 0x0000017d },
  { "FTOI12&dest vft,vfs", 0x060007ff, 0x0000017e },
  { "FTOI15&dest vft,vfs", 0x060007ff, 0x0000017f },
  { "ITOF0&dest  vft,vfs", 0x060007ff, 0x0000013c },
  { "ITOF4&dest  vft,vfs", 0x060007ff, 0x0000013d },
  { "ITOF12&dest vft,vfs", 0x060007ff, 0x0000013e },
  { "ITOF15&dest vft,vfs", 0x060007ff, 0x0000013f },
  { "MADD&dest   vfd,vfs,vft", 0x0600003f, 0x00000029 },
  { "MADDI&dest  vfd,vfs,I", 0x061f003f, 0x00000023 },
  { "MADDQ&dest  vfd,vfs,Q", 0x061f003f, 0x00000021 },
  { "MADD*&dest  vfd,vfs,vft", 0x0600003c, 0x00000008 },
  { "MADDA&dest  ACC,vfs,vft", 0x060007ff, 0x000002bd },
  { "MADDAI&dest ACC,vfs,I", 0x061f07ff, 0x0000023f },
  { "MADDAQ&dest ACC,vfs,Q", 0x061f07ff, 0x0000023d },
  { "MADDA*&dest ACC,vfs,vft", 0x060007fc, 0x000000bc },
  { "MAX&dest    vfd,vfs,vft", 0x0600003f, 0x0000002b },
  { "MAXI&dest   vfd,vfs,I", 0x061f003f, 0x0000001d },
  { "MAX*&dest   vfd,vfs,vft", 0x0600003c, 0x00000010 },
  { "MINI&dest   vfd,vfs,vft", 0x0600003f, 0x0000002f },
  { "MINII&dest  vfd,vfs,I", 0x061f003f, 0x0000001f },
  { "MINI*&dest  vfd,vfs,vft", 0x0600003c, 0x00000014 },
  { "MSUB&dest   vfd,vfs,vft", 0x0600003f, 0x0000002d },
  { "MSUBI&dest  vfd,vfs,I", 0x061f003f, 0x00000027 },
  { "MSUBQ&dest  vfd,vfs,Q", 0x061f003f, 0x00000025 },
  { "MSUB*&dest  vfd,vfs,vft", 0x0600003c, 0x0000000c },
  { "MSUBA&dest  ACC,vft,vfs", 0x060007ff, 0x000002fd },
  { "MSUBAI&dest ACC,vfs,I", 0x061f07ff, 0x0000027f },
  { "MSUBAQ&dest ACC,vfs,Q", 0x061f07ff, 0x0000027d },
  { "MSUBA*&dest ACC,vfs,vft", 0x060007fc, 0x000000fc },
  { "MUL&dest    vfd,vfs,vft", 0x0600003f, 0x0000002a },
  { "MULI&dest   vfd,vfs,I", 0x061f003f, 0x0000001e },
  { "MULQ&dest   vfd,vfs,Q", 0x061f003f, 0x0000001c },
  { "MUL*&dest   vfd,vfs,vft", 0x0600003c, 0x00000018 },
  { "MULA&dest   ACC,vfs,vft", 0x060007ff, 0x000002be },
  { "MULAI&dest  ACC,vfs,I", 0x061f07ff, 0x000001fe },
  { "MULAQ&dest  ACC,vfs,Q", 0x061f07ff, 0x000001fc },
  { "MULA*&dest  ACC,vfs,vft", 0x060007fc, 0x000001bc },
  { "NOP", 0x07ffffff, 0x000002ff },
  { "OPMULA&dest ACC,vfs,vft", 0x07e007ff, 0x01c002fe },
  { "OPMSUB&dest vfd,vfs,vft", 0x07e0003f, 0x01c0002e },
  { "SUB&dest    vfd,vfs,vft", 0x0600003f, 0x0000002c },
  { "SUBI&dest   vfd,vfs,I", 0x061f003f, 0x00000026 },
  { "SUBQ&dest   vfd,vfs,Q", 0x061f003f, 0x00000024 },
  { "SUB*&dest   vfd,vfs,vft", 0x0600003c, 0x00000004 },
  { "SUBA&dest   ACC,vfs,vft", 0x060007ff, 0x000002fc },
  { "SUBAI&dest  ACC,vfs,I", 0x061f07ff, 0x0000027e },
  { "SUBAQ&dest  ACC,vfs,Q", 0x061f07ff, 0x0000027c },
  { "SUBA*&dest  ACC,vfs,vft", 0x060007fc, 0x0000007c },
  { NULL, 0x00000000, 0x00000000 },
};
static VOPCODE vopcodes_lower[] =
{
  { "NOP", 0xffffffff, 0x8000033c },
  { "B          boff", 0xfffff800, 0x40000000 },
  { "BAL        vit,boff", 0xffe0f800, 0x42000000 },
  { "DIV        Q,vfs&fsf,vft&ftf", 0xfe0007ff, 0x800003bc },
  { "EATAN      P,vfs&fsf", 0xff9f07ff, 0x800007fd },
  { "EATANxy    P,vfs", 0xffff07ff, 0x8180077c },
  { "EATANxz    P,vfs", 0xffff07ff, 0x8140077d },
  { "EEXP       P,vfs&fsf", 0xff9f07ff, 0x800007fe },
  { "ELENG      P,vfs", 0xffff07ff, 0x81c0073e },
  { "ERCPR      P,vfs&fsf", 0xff9f07ff, 0x800007be },
  { "ERLENG     P,vfs", 0xffff07ff, 0x81c0073f },
  { "ERSADD     P,vfs", 0xffff07ff, 0x81c0073d },
  { "ERSQRT     P,vfs&fsf", 0xff9f07ff, 0x800007bd },
  { "ESADD      P,vfs", 0xffff07ff, 0x81c0073c },
  { "ESIN       P,vfs&fsf", 0xff9f07ff, 0x800007fc },
  { "ESQRT      P,vfs&fsf", 0xff9f07ff, 0x800007bc },
  { "ESUM       P,vfs", 0xffff07ff, 0x81e0077e },
  { "FCAND      VI01,climm", 0xff000000, 0x24000000 },
  { "FCEQ       VI01,climm", 0xff000000, 0x20000000 },
  { "FCGET      vit", 0xffe0ffff, 0x38000000 },
  { "FCOR       VI01,climm", 0xff000000, 0x26000000 },
  { "FCSET      climm", 0xff000000, 0x22000000 },
  { "FMAND      vit,vis", 0xffe007ff, 0x34000000 },
  { "FMEQ       vit,vis", 0xffe007ff, 0x30000000 },
  { "FMOR       vit,vis", 0xffe007ff, 0x36000000 },
  { "FSAND      vit,stimm", 0xffc0f800, 0x2c000000 },
  { "FSEQ       vit,stimm", 0xffc0f800, 0x28000000 },
  { "FSOR       vit,stimm", 0xffc0f800, 0x2e000000 },
  { "FSSET      stimm", 0xffdff83f, 0x2a000000 },
  { "IADD       vid,vis,vit", 0xffe0003f, 0x80000030 },
  { "IADDI      vit,vis,iaddi", 0xffe0003f, 0x80000032 },
  { "IADDIU     vit,vis,iaddiu", 0xfe000000, 0x10000000 },
  { "IAND       vid,vis,vit", 0xffe0003f, 0x80000034 },
  { "IBEQ       vit,vis,boff", 0xffe00000, 0x50000000 },
  { "IBGEZ      vis,boff", 0xffff0000, 0x5e000000 },
  { "IBGTZ      vis,boff", 0xffff0000, 0x5a000000 },
  { "IBLEZ      vis,boff", 0xffff0000, 0x5c000000 },
  { "IBLTZ      vis,boff", 0xffff0000, 0x58000000 },
  { "IBNE       vit,vis,boff", 0xffe00000, 0x52000000 },
  { "ILW&dest   vit,qoff(vis)", 0xfe000000, 0x08000000 },
  { "ILWR&dest  vit,(vis)", 0xfe0007ff, 0x800003fe },
  { "IOR        vid,vis,vit", 0xffe0003f, 0x80000035 },
  { "ISUB       vid,vis,vit", 0xffe0003f, 0x80000031 },
  { "ISUBIU     vit,vis,iaddiu", 0xfe000000, 0x12000000 },
  { "ISW        vit,qoff(vis)", 0xfe000000, 0x0a000000 },
  { "ISWR&dest  vit,(vis)", 0xfe0007ff, 0x800003ff },
  { "JALR       vit,vis", 0xffe007ff, 0x4a000000 },
  { "JR         vis", 0xffff07ff, 0x48000000 },
  { "LQ&dest    vft,qoff(vis)", 0xfe000000, 0x00000000 },
  { "LQD&dest   vft,(--vis)", 0xfe0007ff, 0x8000037e },
  { "LQI&dest   vft,(vis++)", 0xfe0007ff, 0x8000037c },
  { "MFIR&dest  vft,vis", 0xfe0007ff, 0x800003fd },
  { "MFP&dest   vft,P", 0xfe00ffff, 0x8000067c },
  { "MOVE&dest  vft,vfs", 0xfe0007ff, 0x8000033c },
  { "MR32&dest  vft,vfs", 0xfe0007ff, 0x8000033d },
  { "MTIR       vit,vfs&fsf", 0xff8007ff, 0x800003fc },
  { "RGET&dest  vft,R", 0xfe00ffff, 0x8000043d },
  { "RINIT      R,vfs&fsf", 0xff9f07ff, 0x8000043e },
  { "RNEXT&dest vft,R", 0xfe00ffff, 0x8000043c },
  { "RSQRT      Q,vfs&fsf,vft&ftf", 0xfe0007ff, 0x800003be },
  { "RXOR       R,vfs&fsf", 0xff9f07ff, 0x8000043f },
  { "SQ&dest    vfs,qoff(vit)", 0xfe000000, 0x02000000 },
  { "SQD&dest   vfs,(--vit)", 0xfe0007ff, 0x8000037f },
  { "SQI&dest   vfs,(vit++)", 0xfe0007ff, 0x8000037d },
  { "SQRT       Q,vft&ftf", 0xfe60ffff, 0x800003bd },
  { "WAITP", 0xffffffff, 0x800007bf },
  { "WAITQ", 0xffffffff, 0x800003bf },
  { "XGKICK     vis", 0xffff07ff, 0x800006fc },
  { "XITOP      vit", 0xffe0ffff, 0x800006bd },
  { "XTOP       vit", 0xffe0ffff, 0x800006bc },
  { "(null)", 0x00000000, 0x00000000 },
};
static const char *dest_name_0[] =
{
  "    ",
  "w   ",
  "z   ",
  "wz  ",
  "y   ",
  "wy  ",
  "yz  ",
  "wyz ",
  "x   ",
  "wx  ",
  "xz  ",
  "wxz ",
  "xy  ",
  "wxy ",
  "xyz ",
  "wxyz"
};
static const char *xyzw_name_0[] =
{ "x", "y", "z", "w" };
static int last_cpuid_4 = 0;
static int last_space_5 = 0;
static int last_cnt_6 = 20;

static char *__cdecl vdisasm(char *buf, int cpuid, int space, unsigned int adr, unsigned int upper, unsigned int lower);

static char *__cdecl vdisasm(char *buf, int cpuid, int space, unsigned int adr, unsigned int upper, unsigned int lower)
{
  const char *v6; // eax
  VOPCODE *v7; // eax
  unsigned int v8; // eax
  int v9; // eax
  int v10; // eax
  int v11; // eax
  int v12; // eax
  int v13; // eax
  int v14; // eax
  int v15; // eax
  int v16; // eax
  int v17; // eax
  int v18; // eax
  int v19; // eax
  int v20; // eax
  int v21; // eax
  int v22; // eax
  int v23; // eax
  int v24; // eax
  int v25; // eax
  int v26; // eax
  int v27; // eax
  int v28; // eax
  unsigned int v29; // eax
  int v30; // eax
  int v31; // eax
  char *v32; // eax
  char *v33; // eax
  char *v34; // eax
  const char *v35; // eax
  int v36; // eax
  int v37; // eax
  const char *v39; // [esp-Ch] [ebp-444h]
  const char *v40; // [esp-8h] [ebp-440h]
  const char *v41; // [esp-4h] [ebp-43Ch]
  unsigned int inst; // [esp+10h] [ebp-428h]
  int low; // [esp+14h] [ebp-424h]
  int v44; // [esp+18h] [ebp-420h]
  char ch; // [esp+23h] [ebp-415h]
  char *dp; // [esp+24h] [ebp-414h]
  char *sp; // [esp+28h] [ebp-410h]
  char opr[1024]; // [esp+2Ch] [ebp-40Ch] BYREF
  char *bp; // [esp+42Ch] [ebp-Ch]
  VOPCODE *vopcode; // [esp+430h] [ebp-8h]
  unsigned __int8 v51; // [esp+435h] [ebp-3h]
  unsigned __int16 v52; // [esp+436h] [ebp-2h]

  v52 = cpuid;
  v51 = space;
  bp = buf;
  low = 0;
  if ( (_BYTE)space )
  {
    if ( (unsigned __int8)space == 2 )
    {
      if ( v52 == 1 )
        v6 = "uMem0";
      else
        v6 = "uMem1";
      bp += ds_sprintf(bp, "%s", v6);
      bp += ds_sprintf(bp, ",0x%03x:", adr >> 3);
    }
  }
  else
  {
    bp += ds_sprintf(bp, "0x%08x:", adr);
  }
  while ( 1 )
  {
    v7 = low ? vopcodes_lower : vopcodes_upper;
    vopcode = v7;
    v8 = low ? lower : upper;
    inst = v8;
    v9 = ds_sprintf(bp, " ");
    bp += v9;
    if ( low && (upper & 0x80000000) != 0 )
    {
      v10 = ds_sprintf(bp, "loi        %g", *(float *)&inst);
      bp += v10;
    }
    else
    {
      while ( vopcode->fmt && vopcode->code != (vopcode->mask & inst) )
        ++vopcode;
      if ( vopcode->fmt )
      {
        sp = vopcode->fmt;
        while ( *sp )
        {
          v12 = 0;
          if ( *sp > 96 && *sp <= 122 )
            v12 = 1;
          if ( v12 || *sp == 42 || *sp == 38 )
          {
            opr[0] = *sp++;
            dp = &opr[1];
            opr[1] = 0;
            while ( 1 )
            {
              v14 = 0;
              if ( *sp > 96 && *sp <= 122 )
                v14 = 1;
              if ( !v14 )
                break;
              *dp = *sp++;
              *++dp = 0;
            }
            if ( !strcmp("&dest", opr) )
            {
              v15 = ds_sprintf(bp, ".%s", dest_name_0[(inst >> 21) & 0xF]);
              bp += v15;
            }
            else if ( !strcmp("*", opr) )
            {
              v16 = ds_sprintf(bp, "%s", xyzw_name_0[inst & 3]);
              bp += v16;
            }
            else if ( !strcmp("vft", opr) )
            {
              v17 = ds_sprintf(bp, "vf%02d", BYTE2(inst) & 0x1F);
              bp += v17;
            }
            else if ( !strcmp("vfs", opr) )
            {
              v18 = ds_sprintf(bp, "vf%02d", (unsigned __int16)inst >> 11);
              bp += v18;
            }
            else if ( !strcmp("vfd", opr) )
            {
              v19 = ds_sprintf(bp, "vf%02d", (inst >> 6) & 0x1F);
              bp += v19;
            }
            else if ( !strcmp("vit", opr) )
            {
              v20 = ds_sprintf(bp, "vi%02d", BYTE2(inst) & 0x1F);
              bp += v20;
            }
            else if ( !strcmp("vis", opr) )
            {
              v21 = ds_sprintf(bp, "vi%02d", (unsigned __int16)inst >> 11);
              bp += v21;
            }
            else if ( !strcmp("vid", opr) )
            {
              v22 = ds_sprintf(bp, "vi%02d", (inst >> 6) & 0x1F);
              bp += v22;
            }
            else if ( !strcmp("&ftf", opr) )
            {
              v23 = ds_sprintf(bp, "%s", xyzw_name_0[(inst >> 23) & 3]);
              bp += v23;
            }
            else if ( !strcmp("&fsf", opr) )
            {
              v24 = ds_sprintf(bp, "%s", xyzw_name_0[(inst >> 21) & 3]);
              bp += v24;
            }
            else if ( !strcmp("boff", opr) )
            {
              v44 = 8 * ((inst & 0x7FF) + 1) + adr;
              if ( v51 == 2 )
                v44 >>= 3;
              v25 = ds_sprintf(bp, "0x%x", v44);
              bp += v25;
            }
            else if ( !strcmp("qoff", opr) )
            {
              v26 = ds_sprintf(bp, "%d", inst & 0x7FF);
              bp += v26;
            }
            else if ( !strcmp("climm", opr) )
            {
              v27 = ds_sprintf(bp, "0x%x", inst & 0xFFFFFF);
              bp += v27;
            }
            else if ( !strcmp("stimm", opr) )
            {
              v28 = ds_sprintf(bp, "0x%x", inst & 0x7FF | (((inst & 0x200000) != 0) << 11));
              bp += v28;
            }
            else if ( !strcmp("iaddi", opr) )
            {
              if ( ((inst >> 6) & 0x10) != 0 )
                v29 = (inst >> 6) & 0xF | 0xFFFFFFF0;
              else
                v29 = (inst >> 6) & 0x1F;
              v30 = ds_sprintf(bp, "%d", v29);
              bp += v30;
            }
            else
            {
              if ( !strcmp("iaddiu", opr) )
                v31 = ds_sprintf(bp, "%d", inst & 0x7FF | (((inst >> 21) & 0xF) << 11));
              else
                v31 = ds_sprintf(bp, "??%s??", opr);
              bp += v31;
            }
          }
          else
          {
            ch = *sp++;
            v13 = 0;
            if ( ch > 64 && ch <= 90 )
              v13 = 1;
            if ( v13 )
              ch += 32;
            *bp++ = ch;
            *bp = 0;
          }
        }
      }
      else
      {
        v11 = ds_sprintf(bp, ".word 0x%W", inst);
        bp += v11;
      }
    }
    if ( ++low != 1 )
      break;
    if ( (upper & 0x78000000) != 0 )
    {
      if ( (upper & 0x8000000) != 0 )
        v32 = "t";
      else
        v32 = "";
      v41 = v32;
      if ( (upper & 0x10000000) != 0 )
        v33 = "d";
      else
        v33 = "";
      v40 = v33;
      if ( (upper & 0x20000000) != 0 )
        v34 = "m";
      else
        v34 = "";
      v39 = v34;
      if ( (upper & 0x40000000) != 0 )
        v35 = "e";
      else
        v35 = "";
      v36 = ds_sprintf(bp, "[%s%s%s%s]", v35, v39, v40, v41);
      bp += v36;
    }
    if ( 44 - (bp - buf) > 0 )
    {
      v37 = ds_sprintf(bp, "%*s", 44 - (bp - buf), "");
      bp += v37;
    }
  }
  return buf;
}

int __cdecl vdi_cmd(int ac, char **av)
{
  const char *v3; // eax
  char *v4; // eax
  int v5; // [esp+0h] [ebp-10414h]
  unsigned int lower; // [esp+4h] [ebp-10410h]
  unsigned int upper; // [esp+8h] [ebp-1040Ch]
  int space; // [esp+Ch] [ebp-10408h]
  int v9; // [esp+10h] [ebp-10404h]
  int f_mark; // [esp+14h] [ebp-10400h]
  int mark; // [esp+18h] [ebp-103FCh] BYREF
  int len; // [esp+1Ch] [ebp-103F8h]
  int cnt; // [esp+20h] [ebp-103F4h] BYREF
  int i; // [esp+24h] [ebp-103F0h]
  int adr; // [esp+28h] [ebp-103ECh] BYREF
  unsigned __int8 *dp; // [esp+2Ch] [ebp-103E8h]
  unsigned __int8 dat[65507]; // [esp+30h] [ebp-103E4h] BYREF
  unsigned __int8 buf[1024]; // [esp+10014h] [ebp-400h] BYREF

  mark = 0;
  f_mark = 0;
  if ( ac <= 0 )
  {
    v9 = last_cpuid_4;
    space = last_space_5;
    cnt = last_cnt_6;
  }
  else
  {
    v9 = 0;
    space = 0;
    cnt = 20;
  }
  if ( ac > 0 )
  {
    --ac;
    ++av;
  }
  while ( ac > 0 && **av == 45 )
  {
    if ( !strcmp("-m", *av) )
    {
      ++f_mark;
      if ( ac <= 1 )
        return ds_error("Usage: vdi [-<cpuid>] [-m <mark>] [<adr> [<cnt>]]");
      if ( ds_eval_word(av[1], (unsigned int *)&mark) )
        return -1;
      --ac;
      ++av;
    }
    else if ( !strcmp("-cpu", *av) )
    {
      v9 = 0;
      space = 0;
    }
    else if ( !strcmp("-vu0", *av) )
    {
      v9 = 1;
      space = 2;
    }
    else
    {
      if ( strcmp("-vu1", *av) )
        return ds_error("Usage: vdi [-<cpuid>] [-m <mark>] [<adr> [<cnt>]]");
      v9 = 2;
      space = 2;
    }
    --ac;
    ++av;
  }
  if ( ac > 2 )
    return ds_error("Usage: vdi [-<cpuid>] [-m <mark>] [<adr> [<cnt>]]");
  if ( ac <= 0 )
  {
    if ( space == 2 )
      adr = dot >> 3;
    else
      adr = dot;
  }
  else if ( ds_eval_word(*av, (unsigned int *)&adr) )
  {
    return -1;
  }
  if ( ac > 1 && ds_eval_word(av[1], (unsigned int *)&cnt) )
    return -1;
  if ( space == 2 )
  {
    v5 = cnt + adr;
    if ( adr < 0 )
      adr = 0;
    if ( v9 == 1 )
    {
      if ( v5 > 512 )
        v5 = 512;
    }
    else if ( v5 > 2048 )
    {
      v5 = 2048;
    }
    cnt = v5 - adr;
    adr *= 8;
    mark *= 8;
  }
  last_cpuid_4 = v9;
  last_space_5 = space;
  last_cnt_6 = cnt;
  cnt *= 8;
  if ( cnt <= 0 )
    return 0;
  i = 0;
  len = 0;
  while ( cnt > i )
  {
    if ( len <= 0 )
    {
      len = cnt - i;
      if ( (unsigned int)(cnt - i) > 0xFFE3 )
        len = 65507;
      dp = dat;
      if ( ds_rdwr_mem_align(8, 3, (unsigned __int16)v9, (unsigned __int8)space, i + adr, dat, len) )
        return -1;
    }
    lower = *(_DWORD *)dp;
    upper = *((_DWORD *)dp + 1);
    if ( f_mark && mark == i + adr )
      v3 = "->";
    else
      v3 = "  ";
    ds_printf("%s", v3);
    v4 = vdisasm((char *)buf, (unsigned __int16)v9, (unsigned __int8)space, i + adr, upper, lower);
    ds_printf("%s\n", v4);
    i += 8;
    dp += 8;
    len -= 8;
  }
  dot = cnt + adr;
  return 0;
}

