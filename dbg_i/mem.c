#include "dsidb_prototypes.h"

unsigned int dot = 1048576u;
unsigned int current_entry_point = 0u;
unsigned int current_gp_value = 0u;
static int last_sz_2 = -1;
static int last_cnt_3 = 256;
static int last_adr_8 = 1048576;
static int last_cnt_9 = -1;
static int last_sz_10 = -1;
static const char TARG_IDENT[16] =
{
  '\x7F',
  'E',
  'L',
  'F',
  '\x01',
  '\x01',
  '\x01',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0'
};

static void __cdecl show_statics(int tb, int mst, int mut, int tst, int tut);

int __cdecl dmem_cmd(int ac, char **av)
{
  int result; // eax
  int v3; // ebx
  int v4; // ebx
  int v5; // ebx
  int v6; // ebx
  const char *v7; // eax
  const char *v8; // eax
  const char *v9; // eax
  int v10; // eax
  const char *v11; // eax
  char *v12; // [esp+4h] [ebp-2Ch]
  float fv; // [esp+8h] [ebp-28h] BYREF
  unsigned int wv; // [esp+Ch] [ebp-24h] BYREF
  int v15; // [esp+10h] [ebp-20h]
  int sz; // [esp+14h] [ebp-1Ch]
  int j; // [esp+18h] [ebp-18h]
  int i; // [esp+1Ch] [ebp-14h]
  int cnt; // [esp+20h] [ebp-10h] BYREF
  unsigned __int8 align; // [esp+27h] [ebp-9h]
  unsigned __int8 *bp; // [esp+28h] [ebp-8h]
  unsigned __int8 *buf; // [esp+2Ch] [ebp-4h]
  int aca; // [esp+38h] [ebp+8h]
  char **ava; // [esp+3Ch] [ebp+Ch]

  cnt = 256;
  if ( ac <= 0 )
  {
    cnt = last_cnt_3;
    sz = last_sz_2;
LABEL_17:
    last_cnt_3 = cnt;
    last_sz_2 = sz;
    switch ( sz )
    {
      case 'd':
        v4 = cnt + 7;
        LOBYTE(v4) = (cnt + 7) & 0xF8;
        cnt = v4;
        break;
      case 'f':
      case 'w':
        v5 = cnt + 3;
        LOBYTE(v5) = (cnt + 3) & 0xFC;
        cnt = v5;
        break;
      case 'h':
        v6 = cnt + 1;
        LOBYTE(v6) = (cnt + 1) & 0xFE;
        cnt = v6;
        break;
      case 'q':
        v3 = cnt + 15;
        LOBYTE(v3) = (cnt + 15) & 0xF0;
        cnt = v3;
        break;
      default:
        break;
    }
    if ( cnt > 0 )
    {
      buf = (unsigned __int8 *)ds_alloc_mem_low("mem.c", "dmem_cmd", cnt);
      if ( buf )
      {
        align = 0;
        switch ( sz )
        {
          case 'b':
            align = 0;
            break;
          case 'd':
            align = 3;
            break;
          case 'f':
          case 'w':
            align = 2;
            break;
          case 'h':
            align = 1;
            break;
          case 'q':
            align = 4;
            break;
          default:
            break;
        }
        if ( (dot & ((1 << align) - 1)) != 0 )
        {
          return ds_error("address alignment error");
        }
        else if ( ds_rdwr_mem_align(8, align, 0, 0, dot, buf, cnt) )
        {
          ds_free_mem_low(buf, "mem.c", "dmem_cmd");
          return -1;
        }
        else
        {
          bp = buf;
          i = 0;
          while ( cnt > i )
          {
            v15 = cnt - i;
            if ( cnt - i > 16 )
              v15 = 16;
            ds_printf(" 0x%W: ", i + dot);
            switch ( sz )
            {
              case 'b':
                for ( j = 0; j <= 15 && v15 > j; ++j )
                {
                  if ( j == 7 )
                    v9 = "  ";
                  else
                    v9 = " ";
                  ds_printf("%B%s", bp[j], v9);
                }
                ds_printf(" ");
                while ( j <= 15 )
                {
                  ds_printf("   ");
                  ++j;
                }
                for ( j = 0; j <= 15 && v15 > j; ++j )
                {
                  if ( bp[j] <= 0x1Fu || bp[j] > 0x7Eu )
                    v10 = 46;
                  else
                    v10 = bp[j];
                  ds_printf("%c", v10);
                }
                break;
              case 'd':
                ds_printf("0x%T", *(_DWORD *)bp, *((_DWORD *)bp + 1));
                if ( (unsigned int)v15 > 8 )
                  ds_printf("  0x%T", *((_DWORD *)bp + 2), *((_DWORD *)bp + 3));
                break;
              case 'f':
                for ( j = 0; j <= 15 && v15 > j; j += 4 )
                {
                  wv = *(_DWORD *)&bp[j];
                  memcpy(&fv, &wv, sizeof(fv));
                  ds_printf("%+13g", fv);
                  if ( j + 4 <= 15 && v15 > j + 4 )
                  {
                    if ( j == 4 )
                      v11 = "  ";
                    else
                      v11 = " ";
                    ds_printf("%s", v11);
                  }
                }
                break;
              case 'h':
                for ( j = 0; j <= 15 && v15 > j; j += 2 )
                {
                  ds_printf("0x%H", *(unsigned __int16 *)&bp[j]);
                  if ( j + 2 <= 15 && v15 > j + 2 )
                  {
                    if ( j == 6 )
                      v8 = "  ";
                    else
                      v8 = " ";
                    ds_printf("%s", v8);
                  }
                }
                break;
              case 'q':
                ds_printf("0x%Q", *(_DWORD *)bp, *((_DWORD *)bp + 1), *((_DWORD *)bp + 2), *((_DWORD *)bp + 3));
                break;
              case 'w':
                for ( j = 0; j <= 15 && v15 > j; j += 4 )
                {
                  ds_printf("0x%W", *(_DWORD *)&bp[j]);
                  if ( j + 4 <= 15 && v15 > j + 4 )
                  {
                    if ( j == 4 )
                      v7 = "  ";
                    else
                      v7 = " ";
                    ds_printf("%s", v7);
                  }
                }
                break;
              default:
                break;
            }
            ds_printf("\n");
            i += 16;
            bp += 16;
          }
          ds_free_mem_low(buf, "mem.c", "dmem_cmd");
          dot += cnt;
          return 0;
        }
      }
      else
      {
        return -1;
      }
    }
    else
    {
      return 0;
    }
  }
  else
  {
    cnt = 256;
    sz = (*av)[1];
    switch ( sz )
    {
      case 'b':
      case 'd':
      case 'f':
      case 'h':
      case 'q':
      case 'w':
        aca = ac - 1;
        v12 = *av;
        ava = av + 1;
        if ( aca > 0 && **ava == 45 || aca > 2 )
        {
          result = ds_error("Usage: %s [<adr> [<cnt>]]", v12);
        }
        else if ( aca > 0 && ds_eval_word(*ava, &dot) )
        {
          result = -1;
        }
        else
        {
          if ( aca <= 1 || !ds_eval_word(ava[1], (unsigned int *)&cnt) )
            goto LABEL_17;
          result = -1;
        }
        break;
      default:
        result = ds_error("invalid calling name");
        break;
    }
  }
  return result;
}

// local variable allocation has failed, the output may be wrong!
int __cdecl smem_cmd(int ac, char **av)
{
  int result; // eax
  twin *p_tv; // [esp-14h] [ebp-464h]
  quad v4; // [esp-10h] [ebp-460h] OVERLAPPED
  quad y; // [esp+8h] [ebp-448h] BYREF
  void *ptr; // [esp+18h] [ebp-438h]
  float fv; // [esp+1Ch] [ebp-434h] BYREF
  quad s; // [esp+20h] [ebp-430h] BYREF
  twin tv; // [esp+30h] [ebp-420h] BYREF
  unsigned int wv; // [esp+38h] [ebp-418h] BYREF
  unsigned __int16 hv; // [esp+3Ch] [ebp-414h] BYREF
  unsigned __int8 bv; // [esp+3Eh] [ebp-412h] BYREF
  unsigned __int8 align; // [esp+3Fh] [ebp-411h]
  int i; // [esp+40h] [ebp-410h]
  int n; // [esp+44h] [ebp-40Ch]
  int sz; // [esp+48h] [ebp-408h]
  char *av0; // [esp+4Ch] [ebp-404h]
  char buf[1024]; // [esp+50h] [ebp-400h] BYREF
  int aca; // [esp+458h] [ebp+8h]
  int acb; // [esp+458h] [ebp+8h]
  char **ava; // [esp+45Ch] [ebp+Ch]
  char **avb; // [esp+45Ch] [ebp+Ch]

  if ( ac <= 0 )
    return 0;
  if ( **av != 115 )
    return ds_error("invalid calling name");
  sz = (*av)[1];
  switch ( sz )
  {
    case 'b':
    case 'd':
    case 'f':
    case 'h':
    case 'q':
    case 'w':
      aca = ac - 1;
      av0 = *av;
      ava = av + 1;
      if ( aca > 0 && **ava == 45 || aca <= 1 )
        return ds_error("Usage: %s <adr> <val>...", av0);
      if ( ds_eval_word(*ava, &dot) )
        return -1;
      acb = aca - 1;
      avb = ava + 1;
      break;
    default:
      return ds_error("invalid calling name");
  }
  while ( 1 )
  {
    if ( acb <= 0 )
      return 0;
    if ( **avb == 39 )
      break;
    switch ( sz )
    {
      case 'q':
        if ( ds_eval_quad(*avb, &s) )
          return -1;
LABEL_26:
        ptr = &s;
        n = 16;
        break;
      case 'd':
        if ( ds_eval_quad(*avb, &s) )
          return -1;
LABEL_30:
        v4 = s;
        p_tv = &tv;
        ds_cqt(*(quad *)((char *)&v4 - 4));
        ptr = &tv;
        n = 8;
        break;
      case 'w':
        if ( ds_eval_quad(*avb, &s) )
          return -1;
LABEL_34:
        wv = ds_cqw(s);
        ptr = &wv;
        n = 4;
        break;
      case 'h':
        if ( ds_eval_quad(*avb, &s) )
          return -1;
LABEL_38:
        hv = ds_cqw(s);
        ptr = &hv;
        n = 2;
        break;
      case 'b':
        if ( **avb == 34 )
        {
          n = ds_scan_string(buf, *avb);
          if ( n < 0 )
            return -1;
          if ( n > 0 )
            ptr = buf;
        }
        else
        {
          if ( ds_eval_quad(*avb, &s) )
            return -1;
LABEL_48:
          bv = ds_cqw(s);
          ptr = &bv;
          n = 1;
        }
        break;
      case 'f':
        if ( ds_eval_float(*avb, &fv) )
          return -1;
        memcpy(&wv, &fv, sizeof(wv));
        ptr = &wv;
        n = 4;
        break;
    }
LABEL_53:
    align = 0;
    switch ( sz )
    {
      case 'b':
        align = 0;
        break;
      case 'd':
        align = 3;
        break;
      case 'f':
      case 'w':
        align = 2;
        break;
      case 'h':
        align = 1;
        break;
      case 'q':
        align = 4;
        break;
      default:
        break;
    }
    if ( (dot & ((1 << align) - 1)) != 0 )
      return ds_error("address alignment error");
    if ( ds_rdwr_mem_align(10, align, 0, 0, dot, ptr, n) )
      return -1;
    --acb;
    ++avb;
    dot += n;
  }
  n = ds_scan_string(buf, *avb);
  if ( n < 0 )
    return -1;
  ds_bzero(&s, sizeof(s));
  for ( i = 0; n > i; ++i )
  {
    ds_cwq(&y, 8u);
    ds_qshl(&s, s, y);
    ds_cwq(&y, (unsigned __int8)buf[i]);
    ds_qor(&s, s, y);
  }
  switch ( sz )
  {
    case 'b':
      goto LABEL_48;
    case 'd':
      goto LABEL_30;
    case 'f':
      result = ds_error("invalid argument");
      break;
    case 'h':
      goto LABEL_38;
    case 'q':
      goto LABEL_26;
    case 'w':
      goto LABEL_34;
    default:
      goto LABEL_53;
  }
  return result;
}

int __cdecl inp_cmd(int ac, char **av)
{
  int result; // eax
  const char *v3; // eax
  const char *v4; // eax
  const char *v5; // eax
  int v6; // eax
  char *v7; // [esp+0h] [ebp-28h]
  int n_3; // [esp+4h] [ebp-24h]
  int v9; // [esp+8h] [ebp-20h]
  int j_3; // [esp+Ch] [ebp-1Ch]
  int j_1; // [esp+Ch] [ebp-1Ch]
  int j; // [esp+Ch] [ebp-1Ch]
  int k; // [esp+Ch] [ebp-1Ch]
  int i; // [esp+10h] [ebp-18h]
  int cnt; // [esp+14h] [ebp-14h] BYREF
  int adr; // [esp+18h] [ebp-10h] BYREF
  unsigned __int8 align; // [esp+1Fh] [ebp-9h]
  unsigned __int8 *bp; // [esp+20h] [ebp-8h]
  unsigned __int8 *buf; // [esp+24h] [ebp-4h]
  int aca; // [esp+30h] [ebp+8h]
  char **ava; // [esp+34h] [ebp+Ch]

  if ( ac <= 0 )
  {
    adr = last_adr_8;
    cnt = last_cnt_9;
    v9 = last_sz_10;
LABEL_18:
    last_adr_8 = adr;
    last_cnt_9 = cnt;
    last_sz_10 = v9;
    if ( v9 == 104 )
    {
      cnt *= 2;
    }
    else if ( v9 > 104 )
    {
      if ( v9 == 113 )
      {
        cnt *= 16;
      }
      else if ( v9 == 119 )
      {
        cnt *= 4;
      }
    }
    else if ( v9 == 100 )
    {
      cnt *= 8;
    }
    if ( cnt > 0 )
    {
      buf = (unsigned __int8 *)ds_alloc_mem_low("mem.c", "inp_cmd", cnt);
      if ( buf )
      {
        align = 0;
        switch ( v9 )
        {
          case 'b':
            align = 0;
            break;
          case 'd':
            align = 3;
            break;
          case 'h':
            align = 1;
            break;
          case 'q':
            align = 4;
            break;
          case 'w':
            align = 2;
            break;
          default:
            break;
        }
        if ( (adr & ((1 << align) - 1)) != 0 )
        {
          return ds_error("address alignment error");
        }
        else if ( ds_rdwr_mem_align(8, align, 0, 0, adr, buf, cnt) )
        {
          ds_free_mem_low(buf, "mem.c", "inp_cmd");
          return -1;
        }
        else
        {
          bp = buf;
          for ( i = 0; cnt > i; i += 16 )
          {
            n_3 = cnt - i;
            if ( cnt - i > 16 )
              n_3 = 16;
            ds_printf(" 0x%W: ", i + adr);
            switch ( v9 )
            {
              case 'b':
                for ( j = 0; j <= 15 && n_3 > j; ++j )
                {
                  if ( j == 7 )
                    v5 = "  ";
                  else
                    v5 = " ";
                  ds_printf("%B%s", bp[j], v5);
                }
                ds_printf(" ");
                while ( j <= 15 )
                {
                  ds_printf("   ");
                  ++j;
                }
                for ( k = 0; k <= 15 && n_3 > k; ++k )
                {
                  if ( bp[k] <= 0x1Fu || bp[k] > 0x7Eu )
                    v6 = 46;
                  else
                    v6 = bp[k];
                  ds_printf("%c", v6);
                }
                break;
              case 'd':
                ds_printf("0x%T", *(_DWORD *)bp, *((_DWORD *)bp + 1));
                if ( (unsigned int)n_3 > 8 )
                  ds_printf("  0x%T", *((_DWORD *)bp + 2), *((_DWORD *)bp + 3));
                break;
              case 'h':
                for ( j_1 = 0; j_1 <= 15 && n_3 > j_1; j_1 += 2 )
                {
                  ds_printf("0x%H", *(unsigned __int16 *)&bp[j_1]);
                  if ( j_1 + 2 <= 15 && n_3 > j_1 + 2 )
                  {
                    if ( j_1 == 6 )
                      v4 = "  ";
                    else
                      v4 = " ";
                    ds_printf("%s", v4);
                  }
                }
                break;
              case 'q':
                ds_printf("0x%Q", *(_DWORD *)bp, *((_DWORD *)bp + 1), *((_DWORD *)bp + 2), *((_DWORD *)bp + 3));
                break;
              case 'w':
                for ( j_3 = 0; j_3 <= 15 && n_3 > j_3; j_3 += 4 )
                {
                  ds_printf("0x%W", *(_DWORD *)&bp[j_3]);
                  if ( j_3 + 4 <= 15 && n_3 > j_3 + 4 )
                  {
                    if ( j_3 == 4 )
                      v3 = "  ";
                    else
                      v3 = " ";
                    ds_printf("%s", v3);
                  }
                }
                break;
              default:
                break;
            }
            ds_printf("\n");
            bp += 16;
          }
          ds_free_mem_low(buf, "mem.c", "inp_cmd");
          return 0;
        }
      }
      else
      {
        return -1;
      }
    }
    else
    {
      return 0;
    }
  }
  else
  {
    v9 = (*av)[1];
    switch ( (*av)[1] )
    {
      case 'b':
      case 'd':
      case 'h':
      case 'q':
      case 'w':
        aca = ac - 1;
        v7 = *av;
        ava = av + 1;
        if ( aca > 0 && **ava == 45 || aca > 2 )
          return ds_error("Usage: %s [<adr> [<cnt>]]", v7);
        adr = last_adr_8;
        cnt = last_cnt_9;
        if ( aca <= 0 )
          goto LABEL_13;
        if ( ds_eval_word(*ava, (unsigned int *)&adr) )
        {
          result = -1;
        }
        else
        {
          cnt = 1;
LABEL_13:
          if ( aca <= 1 || !ds_eval_word(ava[1], (unsigned int *)&cnt) )
            goto LABEL_18;
          result = -1;
        }
        break;
      default:
        return ds_error("invalid calling name");
    }
  }
  return result;
}

// local variable allocation has failed, the output may be wrong!
int __cdecl out_cmd(int ac, char **av)
{
  twin *p_tv; // [esp-14h] [ebp-464h]
  quad v4; // [esp-10h] [ebp-460h] OVERLAPPED
  quad y; // [esp+8h] [ebp-448h] BYREF
  void *ptr; // [esp+18h] [ebp-438h]
  quad s; // [esp+1Ch] [ebp-434h] BYREF
  twin tv; // [esp+2Ch] [ebp-424h] BYREF
  unsigned int wv; // [esp+34h] [ebp-41Ch] BYREF
  unsigned __int16 hv; // [esp+38h] [ebp-418h] BYREF
  unsigned __int8 bv; // [esp+3Ah] [ebp-416h] BYREF
  unsigned __int8 align; // [esp+3Bh] [ebp-415h]
  int i; // [esp+3Ch] [ebp-414h]
  int n; // [esp+40h] [ebp-410h]
  int sz; // [esp+44h] [ebp-40Ch]
  int adr; // [esp+48h] [ebp-408h] BYREF
  char *av0; // [esp+4Ch] [ebp-404h]
  char buf[1024]; // [esp+50h] [ebp-400h] BYREF
  int aca; // [esp+458h] [ebp+8h]
  int acb; // [esp+458h] [ebp+8h]
  char **ava; // [esp+45Ch] [ebp+Ch]
  char **avb; // [esp+45Ch] [ebp+Ch]

  if ( ac <= 0 )
    return 0;
  if ( **av != 111 )
    return ds_error("invalid calling name");
  sz = (*av)[1];
  switch ( sz )
  {
    case 'b':
    case 'd':
    case 'h':
    case 'q':
    case 'w':
      aca = ac - 1;
      av0 = *av;
      ava = av + 1;
      if ( aca > 0 && **ava == 45 || aca <= 1 )
        return ds_error("Usage: %s <adr> <val>...", av0);
      if ( ds_eval_word(*ava, (unsigned int *)&adr) )
        return -1;
      acb = aca - 1;
      avb = ava + 1;
      break;
    default:
      return ds_error("invalid calling name");
  }
  while ( acb > 0 )
  {
    if ( **avb == 39 )
    {
      n = ds_scan_string(buf, *avb);
      if ( n < 0 )
        return -1;
      ds_bzero(&s, sizeof(s));
      for ( i = 0; n > i; ++i )
      {
        ds_cwq(&y, 8u);
        ds_qshl(&s, s, y);
        ds_cwq(&y, (unsigned __int8)buf[i]);
        ds_qor(&s, s, y);
      }
      switch ( sz )
      {
        case 'b':
          goto LABEL_47;
        case 'd':
          goto LABEL_29;
        case 'h':
          goto LABEL_37;
        case 'q':
          goto LABEL_25;
        case 'w':
          goto LABEL_33;
        default:
          break;
      }
    }
    else
    {
      switch ( sz )
      {
        case 'q':
          if ( ds_eval_quad(*avb, &s) )
            return -1;
LABEL_25:
          ptr = &s;
          n = 16;
          break;
        case 'd':
          if ( ds_eval_quad(*avb, &s) )
            return -1;
LABEL_29:
          v4 = s;
          p_tv = &tv;
          ds_cqt(*(quad *)((char *)&v4 - 4));
          ptr = &tv;
          n = 8;
          break;
        case 'w':
          if ( ds_eval_quad(*avb, &s) )
            return -1;
LABEL_33:
          wv = ds_cqw(s);
          ptr = &wv;
          n = 4;
          break;
        case 'h':
          if ( ds_eval_quad(*avb, &s) )
            return -1;
LABEL_37:
          hv = ds_cqw(s);
          ptr = &hv;
          n = 2;
          break;
        case 'b':
          if ( **avb == 34 )
          {
            n = ds_scan_string(buf, *avb);
            if ( n < 0 )
              return -1;
            if ( n > 0 )
              ptr = buf;
          }
          else
          {
            if ( ds_eval_quad(*avb, &s) )
              return -1;
LABEL_47:
            bv = ds_cqw(s);
            ptr = &bv;
            n = 1;
          }
          break;
      }
    }
    align = 0;
    switch ( sz )
    {
      case 'b':
        align = 0;
        break;
      case 'd':
        align = 3;
        break;
      case 'h':
        align = 1;
        break;
      case 'q':
        align = 4;
        break;
      case 'w':
        align = 2;
        break;
      default:
        break;
    }
    if ( (adr & ((1 << align) - 1)) != 0 )
      return ds_error("address alignment error");
    if ( ds_rdwr_mem_align(10, align, 0, 0, adr, ptr, n) )
      return -1;
    --acb;
    ++avb;
    adr += n;
  }
  return 0;
}

static void __cdecl show_statics(int tb, int mst, int mut, int tst, int tut)
{
  long double v5; // fst7
  int v6; // [esp+Ch] [ebp-1Ch]
  int v7; // [esp+Ch] [ebp-1Ch]
  int v8; // [esp+18h] [ebp-10h]
  int v9; // [esp+1Ch] [ebp-Ch]
  int v10; // [esp+24h] [ebp-4h]

  v9 = (tut + 500) / 1000 + 1000 * tst;
  if ( v9 <= 0 )
    v6 = 0;
  else
    v6 = (int)((long double)tb * 1000.0 / (long double)v9);
  v8 = v6;
  v10 = (mut + 500) / 1000 + 1000 * mst;
  if ( v10 <= 0 )
    v7 = 0;
  else
    v7 = (int)((long double)tb * 1000.0 / (long double)v10);
  ds_printf("%d bytes %d.%03d sec %.1f KB/S", tb, v9 / 1000, v9 % 1000, (double)((long double)v8 / 1024.0));
  if ( v9 <= 0 )
    v5 = 0.0;
  else
    v5 = (long double)v10 * 100.0 / (long double)v9;
  ds_printf(
    " (%d.%03d sec %.1f KB/S %.1f %%)\n",
    v10 / 1000,
    v10 % 1000,
    (double)((long double)v7 / 1024.0),
    (double)v5);
}

int __cdecl bload_cmd(int ac, char **av)
{
  int64_t tu0; // [esp+0h] [ebp-44h] BYREF
  int64_t psec; // [esp+4h] [ebp-40h] BYREF
  int tut; // [esp+8h] [ebp-3Ch]
  int tst; // [esp+Ch] [ebp-38h]
  int64_t mu0; // [esp+10h] [ebp-34h] BYREF
  int64_t v8; // [esp+14h] [ebp-30h] BYREF
  int mut; // [esp+18h] [ebp-2Ch]
  int mst; // [esp+1Ch] [ebp-28h]
  int64_t u; // [esp+20h] [ebp-24h] BYREF
  int64_t s; // [esp+24h] [ebp-20h] BYREF
  int tb; // [esp+28h] [ebp-1Ch]
  int len; // [esp+2Ch] [ebp-18h]
  int r; // [esp+30h] [ebp-14h]
  void *stream; // [esp+34h] [ebp-10h]
  unsigned __int8 *buf; // [esp+38h] [ebp-Ch]
  char *path; // [esp+3Ch] [ebp-8h]
  unsigned int adr; // [esp+40h] [ebp-4h] BYREF
  int aca; // [esp+4Ch] [ebp+8h]
  char **ava; // [esp+50h] [ebp+Ch]

  buf = 0;
  stream = 0;
  r = -1;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  ava = av + 1;
  if ( aca > 0 && **ava == 45 || aca != 2 )
    return ds_error("Usage: bload <fname> <adr>");
  path = *ava;
  if ( ds_eval_word(ava[1], &adr) )
    return -1;
  buf = (unsigned __int8 *)ds_alloc_mem_low("mem.c", "bload_cmd", 0x100000);
  if ( !buf )
    return -1;
  stream = ds_fopen(path, "r");
  if ( stream )
  {
    mut = 0;
    mst = 0;
    tut = 0;
    tst = 0;
    tb = 0;
    if ( ds_gettime(&psec, &tu0) >= 0 )
    {
      while ( 1 )
      {
        len = ds_fread(buf, 1, 0x100000, stream, 1);
        if ( len <= 0 )
          break;
        if ( ds_gettime(&v8, &mu0) < 0 || ds_store_mem(adr, buf, len) || ds_gettime(&s, &u) < 0 )
          goto LABEL_25;
        mst += s - v8;
        mut += u - mu0;
        if ( mut < 0 )
        {
          --mst;
          mut += 1000000;
        }
        adr += len;
        tb += len;
      }
      if ( ds_gettime(&s, &u) >= 0 )
      {
        tst += s - psec;
        tut += u - tu0;
        if ( tut < 0 )
        {
          --tst;
          tut += 1000000;
        }
        show_statics(tb, mst, mut, tst, tut);
        r = 0;
      }
    }
  }
LABEL_25:
  ds_free_mem_low(buf, "mem.c", "bload_cmd");
  ds_fclose(stream);
  return r;
}

int __cdecl bsave_cmd(int ac, char **av)
{
  int v3; // eax
  int64_t tu0; // [esp+0h] [ebp-48h] BYREF
  int64_t psec; // [esp+4h] [ebp-44h] BYREF
  int tut; // [esp+8h] [ebp-40h]
  int tst; // [esp+Ch] [ebp-3Ch]
  int64_t mu0; // [esp+10h] [ebp-38h] BYREF
  int64_t v9; // [esp+14h] [ebp-34h] BYREF
  int mut; // [esp+18h] [ebp-30h]
  int mst; // [esp+1Ch] [ebp-2Ch]
  int64_t u; // [esp+20h] [ebp-28h] BYREF
  int64_t s; // [esp+24h] [ebp-24h] BYREF
  int tb; // [esp+28h] [ebp-20h]
  int cnt; // [esp+2Ch] [ebp-1Ch] BYREF
  int n; // [esp+30h] [ebp-18h]
  int r; // [esp+34h] [ebp-14h]
  void *stream; // [esp+38h] [ebp-10h]
  unsigned __int8 *buf; // [esp+3Ch] [ebp-Ch]
  char *path; // [esp+40h] [ebp-8h]
  unsigned int adr; // [esp+44h] [ebp-4h] BYREF
  int aca; // [esp+50h] [ebp+8h]
  char **ava; // [esp+54h] [ebp+Ch]

  buf = 0;
  stream = 0;
  r = -1;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  ava = av + 1;
  if ( aca > 0 && **ava == 45 || aca != 3 )
    return ds_error("Usage: bsave <fname> <adr> <cnt>");
  path = *ava;
  if ( ds_eval_word(ava[1], &adr) || ds_eval_word(ava[2], (unsigned int *)&cnt) )
    return -1;
  buf = (unsigned __int8 *)ds_alloc_mem_low("mem.c", "bsave_cmd", 0x100000);
  if ( !buf )
    return -1;
  stream = ds_fopen(path, "w");
  if ( stream )
  {
    mut = 0;
    mst = 0;
    tut = 0;
    tst = 0;
    tb = 0;
    if ( ds_gettime(&psec, &tu0) >= 0 )
    {
      while ( cnt > 0 )
      {
        n = cnt;
        if ( cnt > 0x100000 )
          n = 0x100000;
        if ( ds_gettime(&v9, &mu0) < 0 || ds_load_mem(adr, buf, n) || ds_gettime(&s, &u) < 0 )
          goto LABEL_29;
        mst += s - v9;
        mut += u - mu0;
        if ( mut < 0 )
        {
          --mst;
          mut += 1000000;
        }
        v3 = ds_fwrite(buf, 1, n, stream);
        if ( n != v3 )
          goto LABEL_29;
        adr += n;
        cnt -= n;
        tb += n;
      }
      if ( ds_gettime(&s, &u) >= 0 )
      {
        tst += s - psec;
        tut += u - tu0;
        if ( tut < 0 )
        {
          --tst;
          tut += 1000000;
        }
        show_statics(tb, mst, mut, tst, tut);
        r = 0;
      }
    }
  }
LABEL_29:
  ds_free_mem_low(buf, "mem.c", "bsave_cmd");
  ds_fclose(stream);
  return r;
}

int __cdecl pload_cmd(int ac, char **av)
{
  unsigned int type; // eax
  int v4; // [esp+4h] [ebp-43Ch] BYREF
  int id; // [esp+8h] [ebp-438h] BYREF
  int sload; // [esp+Ch] [ebp-434h]
  int symndx; // [esp+10h] [ebp-430h]
  int r; // [esp+14h] [ebp-42Ch]
  unsigned __int8 *v9; // [esp+18h] [ebp-428h]
  unsigned __int8 *buf; // [esp+1Ch] [ebp-424h]
  DS_ELF_REGINFO *ri; // [esp+20h] [ebp-420h]
  DS_ELF_SHDR *sh; // [esp+24h] [ebp-41Ch]
  DS_ELF_SHDR *shdr; // [esp+28h] [ebp-418h]
  DS_ELF_PHDR *ph; // [esp+2Ch] [ebp-414h]
  DS_ELF_PHDR *phdr; // [esp+30h] [ebp-410h]
  DS_ELF_EHDR *ehdr; // [esp+34h] [ebp-40Ch]
  void *stream; // [esp+38h] [ebp-408h]
  char *av0; // [esp+3Ch] [ebp-404h]
  char path[1024]; // [esp+40h] [ebp-400h] BYREF
  int aca; // [esp+448h] [ebp+8h]
  char **ava; // [esp+44Ch] [ebp+Ch]

  stream = 0;
  ehdr = 0;
  phdr = 0;
  shdr = 0;
  ri = 0;
  buf = 0;
  r = -1;
  id = 0;
  v4 = 0;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  av0 = *av;
  ava = av + 1;
  sload = strcmp("sload", av0) == 0;
  if ( aca > 1 && !strcmp("-id", *ava) )
  {
    ds_scan_hex_word(ava[1], (unsigned int *)&id);
    aca -= 2;
    ava += 2;
  }
  if ( aca > 1 && !strcmp("-b", *ava) )
  {
    ds_scan_hex_word(ava[1], (unsigned int *)&v4);
    aca -= 2;
    ava += 2;
  }
  if ( aca > 0 && **ava == 45 )
    goto LABEL_11;
  if ( aca > 0 )
    strcpy(path, *ava);
  if ( sload )
  {
    if ( !aca )
    {
      clear_symbol();
      clear_mdebug();
      return 0;
    }
    if ( aca > 1 )
      goto LABEL_11;
LABEL_22:
    stream = ds_fopen(path, "r");
    if ( !stream )
      goto LABEL_96;
    ehdr = (DS_ELF_EHDR *)ds_fload(stream, 0, 0, 52, 1);
    if ( !ehdr )
      goto LABEL_96;
    v9 = (unsigned __int8 *)ehdr;
    if ( ehdr->ident[0] == 98 && v9[1] == 1 || *v9 == 1 && v9[1] == 98 )
    {
      if ( !sload )
        ds_printf("ECOFF is not supported\n");
      goto LABEL_96;
    }
    for ( symndx = 0; (unsigned int)symndx <= 0xF; ++symndx )
    {
      if ( ehdr->ident[symndx] != TARG_IDENT[symndx] )
      {
        ds_printf("ident error\n");
        goto LABEL_96;
      }
    }
    if ( ehdr->machine == 8 && (ehdr->type == 2 || ehdr->type == 0xFF80 || ehdr->type == 0xFF81) )
    {
      if ( ehdr->ehsize == 52 && ehdr->phentsize == 32 )
      {
        if ( ehdr->phnum )
        {
          phdr = (DS_ELF_PHDR *)ds_fload(stream, 0, ehdr->phoff, 32, ehdr->phnum);
          if ( phdr )
          {
            if ( ehdr->type == 2 )
            {
              if ( v4 )
              {
                ds_printf("-b option for not relocatable ??\n");
                goto LABEL_96;
              }
              v4 = -1;
            }
            if ( sload )
            {
              ph = phdr;
              symndx = 0;
              while ( symndx < ehdr->phnum )
              {
                if ( ph->type == 1 || ph->filesz )
                  mod_set_vaddr(id, ph->vaddr);
                ++symndx;
                ++ph;
              }
            }
            else
            {
              ph = phdr;
              symndx = 0;
              while ( symndx < ehdr->phnum )
              {
                if ( ph->type == 1 || ph->filesz )
                {
                  ds_printf("Loading program (address=0x%W size=0x%W) ...\n", ph->vaddr, ph->filesz);
                  buf = (unsigned __int8 *)ds_fload(stream, 0, ph->offset, 1, ph->filesz);
                  if ( !buf || ds_store_mem(ph->vaddr, buf, ph->filesz) )
                    goto LABEL_96;
                  buf = (unsigned __int8 *)ds_free_mem_low(buf, "mem.c", "pload_cmd");
                }
                ++symndx;
                ++ph;
              }
            }
            if ( ehdr->shnum )
            {
              shdr = (DS_ELF_SHDR *)ds_fload(stream, 0, ehdr->shoff, 40, ehdr->shnum);
              if ( shdr )
              {
                sh = shdr;
                symndx = 0;
                while ( symndx < ehdr->shnum )
                {
                  ++symndx;
                  ++sh;
                }
                sh = shdr;
                symndx = 0;
                while ( symndx < ehdr->shnum )
                {
                  type = sh->type;
                  if ( type == 1879048197 )
                  {
                    load_mdebug(stream, ehdr, shdr, symndx, id, v4, path);
                  }
                  else if ( type > 0x70000005 )
                  {
                    if ( type == 1879048198 && sh->size == 24 )
                    {
                      ri = (DS_ELF_REGINFO *)ds_free_mem_low(ri, "mem.c", "pload_cmd");
                      ri = (DS_ELF_REGINFO *)ds_fload(stream, 0, sh->offset, 24, 1);
                      if ( !ri )
                        goto LABEL_96;
                    }
                  }
                  else if ( type == 2 && sh->link && sh->link < ehdr->shnum && sh->entsize == 16 )
                  {
                    load_symbol(stream, ehdr, shdr, symndx, sh->link, id, v4);
                  }
                  ++symndx;
                  ++sh;
                }
                if ( !sload )
                {
                  if ( !ri )
                  {
                    ds_printf("REGINFO not found\n");
                    goto LABEL_96;
                  }
                  ds_printf("Entry address = 0x%W\n", ehdr->entry);
                  ds_printf("GP value      = 0x%W\n", ri->gp_value);
                  current_entry_point = ehdr->entry;
                  current_gp_value = ri->gp_value;
                }
                r = 0;
              }
            }
            else
            {
              ds_printf("section header not found\n");
            }
          }
        }
        else
        {
          ds_printf("program header not found\n");
        }
      }
      else
      {
        ds_printf("invalid header\n");
      }
    }
    else
    {
      ds_printf("arch error (type=0x%x, machine=0x%x)\n", ehdr->type, ehdr->machine);
    }
LABEL_96:
    ds_free_mem_low(buf, "mem.c", "pload_cmd");
    ds_free_mem_low(ri, "mem.c", "pload_cmd");
    ds_free_mem_low(shdr, "mem.c", "pload_cmd");
    ds_free_mem_low(phdr, "mem.c", "pload_cmd");
    ds_free_mem_low(ehdr, "mem.c", "pload_cmd");
    ds_fclose(stream);
    return r;
  }
  if ( aca > 0 )
    goto LABEL_22;
LABEL_11:
  if ( sload )
    return ds_error("Usage: sload [-id <id>] [-b <base>] [<fname>]");
  else
    return ds_error("Usage: pload <fname> [<args>]...");
}

