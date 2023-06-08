
#include "dsxdb_prototypes.h"

extern unsigned int dot; // defined in mem.c
static VOPCODE vopcodes_upper[] =
{
  { "ABS&dest    vft,vfs", 100665343u, 509u },
  { "ADD&dest    vfd,vfs,vft", 100663359u, 40u },
  { "ADDI&dest   vfd,vfs,I", 102694975u, 34u },
  { "ADDQ&dest   vfd,vfs,Q", 102694975u, 32u },
  { "ADD*&dest   vfd,vfs,vft", 100663356u, 0u },
  { "ADDA&dest   ACC,vft,vfs", 100665343u, 700u },
  { "ADDAI&dest  ACC,vfs,I", 102696959u, 574u },
  { "ADDAQ&dest  ACC,vfs,Q", 102696959u, 572u },
  { "ADDA*&dest  ACC,vfs,vft", 100665340u, 60u },
  { "CLIP*&dest  vfs,vft", 132122623u, 29360639u },
  { "FTOI0&dest  vft,vfs", 100665343u, 380u },
  { "FTOI4&dest  vft,vfs", 100665343u, 381u },
  { "FTOI12&dest vft,vfs", 100665343u, 382u },
  { "FTOI15&dest vft,vfs", 100665343u, 383u },
  { "ITOF0&dest  vft,vfs", 100665343u, 316u },
  { "ITOF4&dest  vft,vfs", 100665343u, 317u },
  { "ITOF12&dest vft,vfs", 100665343u, 318u },
  { "ITOF15&dest vft,vfs", 100665343u, 319u },
  { "MADD&dest   vfd,vfs,vft", 100663359u, 41u },
  { "MADDI&dest  vfd,vfs,I", 102694975u, 35u },
  { "MADDQ&dest  vfd,vfs,Q", 102694975u, 33u },
  { "MADD*&dest  vfd,vfs,vft", 100663356u, 8u },
  { "MADDA&dest  ACC,vfs,vft", 100665343u, 701u },
  { "MADDAI&dest ACC,vfs,I", 102696959u, 575u },
  { "MADDAQ&dest ACC,vfs,Q", 102696959u, 573u },
  { "MADDA*&dest ACC,vfs,vft", 100665340u, 188u },
  { "MAX&dest    vfd,vfs,vft", 100663359u, 43u },
  { "MAXI&dest   vfd,vfs,I", 102694975u, 29u },
  { "MAX*&dest   vfd,vfs,vft", 100663356u, 16u },
  { "MINI&dest   vfd,vfs,vft", 100663359u, 47u },
  { "MINII&dest  vfd,vfs,I", 102694975u, 31u },
  { "MINI*&dest  vfd,vfs,vft", 100663356u, 20u },
  { "MSUB&dest   vfd,vfs,vft", 100663359u, 45u },
  { "MSUBI&dest  vfd,vfs,I", 102694975u, 39u },
  { "MSUBQ&dest  vfd,vfs,Q", 102694975u, 37u },
  { "MSUB*&dest  vfd,vfs,vft", 100663356u, 12u },
  { "MSUBA&dest  ACC,vft,vfs", 100665343u, 765u },
  { "MSUBAI&dest ACC,vfs,I", 102696959u, 639u },
  { "MSUBAQ&dest ACC,vfs,Q", 102696959u, 637u },
  { "MSUBA*&dest ACC,vfs,vft", 100665340u, 252u },
  { "MUL&dest    vfd,vfs,vft", 100663359u, 42u },
  { "MULI&dest   vfd,vfs,I", 102694975u, 30u },
  { "MULQ&dest   vfd,vfs,Q", 102694975u, 28u },
  { "MUL*&dest   vfd,vfs,vft", 100663356u, 24u },
  { "MULA&dest   ACC,vfs,vft", 100665343u, 702u },
  { "MULAI&dest  ACC,vfs,I", 102696959u, 510u },
  { "MULAQ&dest  ACC,vfs,Q", 102696959u, 508u },
  { "MULA*&dest  ACC,vfs,vft", 100665340u, 444u },
  { "NOP", 134217727u, 767u },
  { "OPMULA&dest ACC,vfs,vft", 132122623u, 29360894u },
  { "OPMSUB&dest vfd,vfs,vft", 132120639u, 29360174u },
  { "SUB&dest    vfd,vfs,vft", 100663359u, 44u },
  { "SUBI&dest   vfd,vfs,I", 102694975u, 38u },
  { "SUBQ&dest   vfd,vfs,Q", 102694975u, 36u },
  { "SUB*&dest   vfd,vfs,vft", 100663356u, 4u },
  { "SUBA&dest   ACC,vfs,vft", 100665343u, 764u },
  { "SUBAI&dest  ACC,vfs,I", 102696959u, 638u },
  { "SUBAQ&dest  ACC,vfs,Q", 102696959u, 636u },
  { "SUBA*&dest  ACC,vfs,vft", 100665340u, 124u },
  { NULL, 0u, 0u }
};
static VOPCODE vopcodes_lower[] =
{
  { "NOP", 4294967295u, 2147484476u },
  { "B          boff", 4294965248u, 1073741824u },
  { "BAL        vit,boff", 4292933632u, 1107296256u },
  { "DIV        Q,vfs&fsf,vft&ftf", 4261414911u, 2147484604u },
  { "EATAN      P,vfs&fsf", 4288612351u, 2147485693u },
  { "EATANxy    P,vfs", 4294903807u, 2172651388u },
  { "EATANxz    P,vfs", 4294903807u, 2168457085u },
  { "EEXP       P,vfs&fsf", 4288612351u, 2147485694u },
  { "ELENG      P,vfs", 4294903807u, 2176845630u },
  { "ERCPR      P,vfs&fsf", 4288612351u, 2147485630u },
  { "ERLENG     P,vfs", 4294903807u, 2176845631u },
  { "ERSADD     P,vfs", 4294903807u, 2176845629u },
  { "ERSQRT     P,vfs&fsf", 4288612351u, 2147485629u },
  { "ESADD      P,vfs", 4294903807u, 2176845628u },
  { "ESIN       P,vfs&fsf", 4288612351u, 2147485692u },
  { "ESQRT      P,vfs&fsf", 4288612351u, 2147485628u },
  { "ESUM       P,vfs", 4294903807u, 2178942846u },
  { "FCAND      VI01,climm", 4278190080u, 603979776u },
  { "FCEQ       VI01,climm", 4278190080u, 536870912u },
  { "FCGET      vit", 4292935679u, 939524096u },
  { "FCOR       VI01,climm", 4278190080u, 637534208u },
  { "FCSET      climm", 4278190080u, 570425344u },
  { "FMAND      vit,vis", 4292872191u, 872415232u },
  { "FMEQ       vit,vis", 4292872191u, 805306368u },
  { "FMOR       vit,vis", 4292872191u, 905969664u },
  { "FSAND      vit,stimm", 4290836480u, 738197504u },
  { "FSEQ       vit,stimm", 4290836480u, 671088640u },
  { "FSOR       vit,stimm", 4290836480u, 771751936u },
  { "FSSET      stimm", 4292868159u, 704643072u },
  { "IADD       vid,vis,vit", 4292870207u, 2147483696u },
  { "IADDI      vit,vis,iaddi", 4292870207u, 2147483698u },
  { "IADDIU     vit,vis,iaddiu", 4261412864u, 268435456u },
  { "IAND       vid,vis,vit", 4292870207u, 2147483700u },
  { "IBEQ       vit,vis,boff", 4292870144u, 1342177280u },
  { "IBGEZ      vis,boff", 4294901760u, 1577058304u },
  { "IBGTZ      vis,boff", 4294901760u, 1509949440u },
  { "IBLEZ      vis,boff", 4294901760u, 1543503872u },
  { "IBLTZ      vis,boff", 4294901760u, 1476395008u },
  { "IBNE       vit,vis,boff", 4292870144u, 1375731712u },
  { "ILW&dest   vit,qoff(vis)", 4261412864u, 134217728u },
  { "ILWR&dest  vit,(vis)", 4261414911u, 2147484670u },
  { "IOR        vid,vis,vit", 4292870207u, 2147483701u },
  { "ISUB       vid,vis,vit", 4292870207u, 2147483697u },
  { "ISUBIU     vit,vis,iaddiu", 4261412864u, 301989888u },
  { "ISW        vit,qoff(vis)", 4261412864u, 167772160u },
  { "ISWR&dest  vit,(vis)", 4261414911u, 2147484671u },
  { "JALR       vit,vis", 4292872191u, 1241513984u },
  { "JR         vis", 4294903807u, 1207959552u },
  { "LQ&dest    vft,qoff(vis)", 4261412864u, 0u },
  { "LQD&dest   vft,(--vis)", 4261414911u, 2147484542u },
  { "LQI&dest   vft,(vis++)", 4261414911u, 2147484540u },
  { "MFIR&dest  vft,vis", 4261414911u, 2147484669u },
  { "MFP&dest   vft,P", 4261478399u, 2147485308u },
  { "MOVE&dest  vft,vfs", 4261414911u, 2147484476u },
  { "MR32&dest  vft,vfs", 4261414911u, 2147484477u },
  { "MTIR       vit,vfs&fsf", 4286580735u, 2147484668u },
  { "RGET&dest  vft,R", 4261478399u, 2147484733u },
  { "RINIT      R,vfs&fsf", 4288612351u, 2147484734u },
  { "RNEXT&dest vft,R", 4261478399u, 2147484732u },
  { "RSQRT      Q,vfs&fsf,vft&ftf", 4261414911u, 2147484606u },
  { "RXOR       R,vfs&fsf", 4288612351u, 2147484735u },
  { "SQ&dest    vfs,qoff(vit)", 4261412864u, 33554432u },
  { "SQD&dest   vfs,(--vit)", 4261414911u, 2147484543u },
  { "SQI&dest   vfs,(vit++)", 4261414911u, 2147484541u },
  { "SQRT       Q,vft&ftf", 4267769855u, 2147484605u },
  { "WAITP", 4294967295u, 2147485631u },
  { "WAITQ", 4294967295u, 2147484607u },
  { "XGKICK     vis", 4294903807u, 2147485436u },
  { "XITOP      vit", 4292935679u, 2147485373u },
  { "XTOP       vit", 4292935679u, 2147485372u },
  { NULL, 0u, 0u }
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

