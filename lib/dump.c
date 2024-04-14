#include "dsnet_prototypes.h"

static int print_dcmp(unsigned __int8 *bp, int n);
static int print_ttyp(unsigned __int8 *bp, int n);
static int print_drfp(unsigned __int8 *bp, int n);
static int print_netmp(unsigned __int8 *bp, int n);
static int print_dbgp(unsigned __int8 *bp, int n, int f_ee);
static int print_iloadp(unsigned __int8 *bp, int n);
static int print_eloadp(unsigned __int8 *bp, int n);

static int print_dcmp(unsigned __int8 *bp, int n)
{
  int v3; // eax
  unsigned __int16 *v5; // [esp+20h] [ebp+8h]
  unsigned int na; // [esp+24h] [ebp+Ch]

  if ( (unsigned int)n <= 3 )
    return 0;
  na = n - 4;
  v5 = (unsigned __int16 *)(bp + 4);
  ds_printf(" type=%<CT>", *bp);
  if ( *((_WORD *)bp + 1) )
    ds_printf(" unused=%H", *((unsigned __int16 *)bp + 1));
  v3 = *bp;
  if ( v3 == 1 )
  {
    ds_printf(" code=%<CC_EC>", bp[1]);
    if ( na > 3 )
    {
      ds_printf(" id=%H seq=%H", *v5, v5[1]);
      return 8;
    }
    return 4;
  }
  if ( *bp > 1u )
  {
    if ( v3 == 2 )
    {
      ds_printf(" code=%<CC_ST>", bp[1]);
      if ( na > 1 )
      {
        if ( bp[1] != 4 && bp[1] )
          ds_printf(" proto=%<SP>", *v5);
        else
          ds_printf(" node=%<SN>", *v5);
        return 6;
      }
    }
    else if ( v3 == 3 )
    {
      ds_printf(" code=%<CC_ER>", bp[1]);
      if ( na > 0x17 )
      {
        ds_dump(" orig=", (DECI2_HDR *)v5, 24);
        return 28;
      }
    }
    return 4;
  }
  if ( *bp )
    return 4;
  ds_printf(" code=%<CC_CO>", bp[1]);
  if ( bp[1] > 3u )
    return 4;
  if ( *(_BYTE *)v5 || bp[1] == 1 || bp[1] == 3 )
    ds_printf(" result=%<CE_CO>", *(unsigned __int8 *)v5);
  if ( bp[1] || na <= 0xF )
    return 8;
  ds_printf(" %W_%W %W_%W", *((_DWORD *)v5 + 2), *((_DWORD *)v5 + 1), *((_DWORD *)v5 + 4), *((_DWORD *)v5 + 3));
  return 24;
}

static int print_ttyp(unsigned __int8 *bp, int n)
{
  int v3; // [esp-4h] [ebp-Ch]
  unsigned int wv; // [esp+0h] [ebp-8h]
  int i; // [esp+4h] [ebp-4h]
  unsigned __int8 *v6; // [esp+10h] [ebp+8h]

  if ( (unsigned int)n <= 3 )
    return 0;
  wv = *(_DWORD *)bp;
  ds_printf(" C=%x", *(_DWORD *)bp);
  v6 = bp + 4;
  if ( wv )
    return 4;
  ds_printf(" \"");
  for ( i = 0; i < (unsigned int)(n - 4); ++i )
  {
    v3 = *v6++;
    ds_printf("%C", v3);
  }
  ds_printf("\"");
  return n;
}

static int print_drfp(unsigned __int8 *bp, int n)
{
  int result; // eax
  int v3; // [esp-4h] [ebp-14h]
  int v4; // [esp-4h] [ebp-14h]
  int v5; // [esp-4h] [ebp-14h]
  int v6; // [esp-4h] [ebp-14h]
  int i; // [esp+0h] [ebp-10h]
  int r; // [esp+4h] [ebp-Ch]
  unsigned int req; // [esp+8h] [ebp-8h]
  unsigned __int8 *v10; // [esp+Ch] [ebp-4h]
  unsigned __int8 *v11; // [esp+Ch] [ebp-4h]
  unsigned __int8 *v12; // [esp+18h] [ebp+8h]
  int na; // [esp+1Ch] [ebp+Ch]
  int nb; // [esp+1Ch] [ebp+Ch]
  int nc; // [esp+1Ch] [ebp+Ch]
  int nd; // [esp+1Ch] [ebp+Ch]
  int ne; // [esp+1Ch] [ebp+Ch]
  int nf; // [esp+1Ch] [ebp+Ch]
  int ng; // [esp+1Ch] [ebp+Ch]
  int nh; // [esp+1Ch] [ebp+Ch]
  int ni; // [esp+1Ch] [ebp+Ch]

  na = n - 4;
  if ( na < 0 )
    return 0;
  req = *(_DWORD *)bp;
  ds_printf(" %<FC>", *(_DWORD *)bp);
  nb = na - 4;
  if ( nb < 0 )
    return 4;
  v10 = bp + 8;
  ds_printf(" seq=0x%x", *((_DWORD *)bp + 1));
  r = 8;
  switch ( req )
  {
    case 0u:
      nc = nb - 4;
      if ( nc >= 0 )
      {
        ds_printf(" %s=0x%x", "flag", *(_DWORD *)v10);
        ds_printf(" \"");
        v12 = bp + 12;
        for ( i = 0; nc > i; ++i )
        {
          v3 = *v12++;
          ds_printf("%C", v3);
        }
        ds_printf("\"");
        r = nc + 12;
        goto LABEL_45;
      }
      result = 8;
      break;
    case 1u:
      nd = nb - 4;
      if ( nd >= 0 )
      {
        v4 = *(_DWORD *)v10;
        v10 = bp + 12;
        ds_printf(" %s=0x%x", "r", v4);
        r = 12;
        if ( nd - 4 >= 0 )
          goto LABEL_16;
        result = 12;
      }
      else
      {
        result = 8;
      }
      break;
    case 2u:
      if ( nb - 4 >= 0 )
      {
LABEL_16:
        ds_printf(" %s=0x%x", "fd", *(_DWORD *)v10);
        r += 4;
        goto LABEL_45;
      }
      result = 8;
      break;
    case 3u:
      if ( nb - 4 >= 0 )
      {
        ds_printf(" %s=0x%x", "r", *(_DWORD *)v10);
        r = 12;
        goto LABEL_45;
      }
      result = 8;
      break;
    case 4u:
    case 6u:
      ne = nb - 4;
      if ( ne >= 0 )
      {
        v5 = *(_DWORD *)v10;
        v11 = bp + 12;
        ds_printf(" %s=0x%x", "fd", v5);
        if ( ne - 4 >= 0 )
          goto LABEL_27;
        result = 12;
      }
      else
      {
        result = 8;
      }
      break;
    case 5u:
    case 7u:
      nf = nb - 4;
      if ( nf >= 0 )
      {
        v6 = *(_DWORD *)v10;
        v11 = bp + 12;
        ds_printf(" %s=0x%x", "r", v6);
        if ( nf - 4 >= 0 )
        {
LABEL_27:
          ds_printf(" %s=0x%x", "len", *(_DWORD *)v11);
          r = 16;
          goto LABEL_45;
        }
        result = 12;
      }
      else
      {
        result = 8;
      }
      break;
    case 8u:
      ng = nb - 4;
      if ( ng >= 0 )
      {
        ds_printf(" %s=0x%x", "fd", *(_DWORD *)v10);
        nh = ng - 4;
        if ( nh >= 0 )
        {
          ds_printf(" %s=0x%x", "off", *((_DWORD *)bp + 3));
          if ( nh - 4 >= 0 )
          {
            ds_printf(" %s=0x%x", "base", *((_DWORD *)bp + 4));
            r = 20;
            goto LABEL_45;
          }
          result = 16;
        }
        else
        {
          result = 12;
        }
      }
      else
      {
        result = 8;
      }
      break;
    case 9u:
      ni = nb - 4;
      if ( ni >= 0 )
      {
        ds_printf(" %s=0x%x", "r", *(_DWORD *)v10);
        if ( ni - 4 >= 0 )
        {
          ds_printf(" %s=0x%x", "pos", *((_DWORD *)bp + 3));
          r = 16;
          goto LABEL_45;
        }
        result = 12;
      }
      else
      {
        result = 8;
      }
      break;
    default:
LABEL_45:
      result = r;
      break;
  }
  return result;
}

static int print_netmp(unsigned __int8 *bp, int n)
{
  int v3; // [esp-4h] [ebp-20h]
  char *msg; // [esp+8h] [ebp-14h]
  int i; // [esp+Ch] [ebp-10h]
  int i_1; // [esp+Ch] [ebp-10h]
  int nprotos; // [esp+10h] [ebp-Ch]
  NETMP_PROTOS *protos; // [esp+14h] [ebp-8h]

  if ( (unsigned int)n <= 1 )
    return 0;
  ds_printf(" code=%<MC> result=%<ME>", *bp, bp[1]);
  switch ( *bp )
  {
    case 0u:
    case 1u:
      nprotos = (unsigned int)(n - 2) >> 2;
      protos = (NETMP_PROTOS *)&CONTAINING_RECORD(bp, NETMP_PROTOS, pri)->proto;
      for ( i = 0; nprotos > i; ++i )
      {
        ds_printf(" pri=%B,proto=%<SP>", protos->pri, protos->proto);
        ++protos;
      }
      return 4 * nprotos + 2;
    case 2u:
      if ( (unsigned int)n > 0x23 )
      {
        ds_printf(" %W_%W %W_%W", *((_DWORD *)bp + 2), *((_DWORD *)bp + 1), *((_DWORD *)bp + 4), *((_DWORD *)bp + 3));
        return 36;
      }
      return 2;
    case 3u:
      if ( (unsigned int)n > 2 )
      {
        ds_printf(" %B", bp[2]);
        return 3;
      }
      return 2;
  }
  if ( *bp != 4 && *bp != 5 && *bp != 10 && *bp != 11 )
  {
    if ( (*bp == 8 || *bp == 9) && (unsigned int)n > 3 )
    {
      ds_printf(" %<SP>", *((unsigned __int16 *)bp + 1));
      return 4;
    }
    return 2;
  }
  msg = (char *)(bp + 2);
  ds_printf(" \"");
  for ( i_1 = 0; i_1 < (unsigned int)(n - 2); ++i_1 )
  {
    v3 = *msg++;
    ds_printf("%C", v3);
  }
  ds_printf("\"");
  return n;
}

static int print_dbgp(unsigned __int8 *bp, int n, int f_ee)
{
  int result; // eax
  char *v4; // eax
  int v5; // eax
  int argc; // [esp+4h] [ebp-28h]
  DBGP_XGKT_DATA *xd; // [esp+4h] [ebp-28h]
  char *p; // [esp+8h] [ebp-24h]
  char *v9; // [esp+Ch] [ebp-20h]
  int r; // [esp+14h] [ebp-18h]
  int r_1; // [esp+14h] [ebp-18h]
  int r_2; // [esp+14h] [ebp-18h]
  int r_3; // [esp+14h] [ebp-18h]
  int r_4; // [esp+14h] [ebp-18h]
  unsigned int *wp; // [esp+1Ch] [ebp-10h]
  unsigned int *wp_1; // [esp+1Ch] [ebp-10h]
  DBGP_REG *rh; // [esp+20h] [ebp-Ch]
  int na; // [esp+38h] [ebp+Ch]
  int nb; // [esp+38h] [ebp+Ch]
  int nc; // [esp+38h] [ebp+Ch]
  int nd; // [esp+38h] [ebp+Ch]

  na = n - 8;
  if ( na < 0 )
    return 0;
  if ( *bp )
    ds_printf(" id=%<DI>", *bp);
  ds_printf(" type=%<DT>", bp[2]);
  if ( bp[2] > 0x17u || bp[2] < 0x15u )
  {
    if ( bp[3] )
      ds_printf(" code=%B", bp[3]);
  }
  else
  {
    ds_printf(" code=%<DC_C>", bp[3]);
  }
  ds_printf(" result=%<DR>", bp[4]);
  if ( bp[5] )
    ds_printf(" count=%B", bp[5]);
  if ( *((_WORD *)bp + 3) )
    ds_printf(" unused=%H", *((unsigned __int16 *)bp + 3));
  switch ( bp[2] )
  {
    case 0u:
    case 1u:
      wp = (unsigned int *)(bp + 8);
      for ( r = 0; na > r; r += 4 )
        ds_printf(" %x", *wp++);
      result = 4 * (r - 4) + 8;
      break;
    case 4u:
    case 5u:
    case 6u:
    case 7u:
      r_1 = 8;
      for ( rh = (DBGP_REG *)(bp + 8); ; rh += 2 )
      {
        while ( 1 )
        {
          nb = na - 4;
          if ( nb < 0 )
            return r_1;
          r_2 = r_1 + 4;
          ds_printf(" %d,%d", rh->kind, rh->number);
          if ( rh->reserved )
            ds_printf(" reserved=%H", rh->reserved);
          if ( !f_ee )
            break;
          na = nb - 16;
          if ( na < 0 )
            return r_2;
          r_1 = r_2 + 16;
          ds_printf(" %Q", *(_DWORD *)&rh[1], *(_DWORD *)&rh[2], *(_DWORD *)&rh[3], *(_DWORD *)&rh[4]);
          rh += 5;
        }
        na = nb - 4;
        if ( na < 0 )
          break;
        r_1 = r_2 + 4;
        ds_printf(" %W", *(_DWORD *)&rh[1]);
      }
      result = r_2;
      break;
    case 8u:
    case 9u:
    case 0xAu:
    case 0xBu:
      if ( na - 12 >= 0 )
      {
        if ( bp[8] )
          ds_printf(" space=%<DS>", bp[8]);
        ds_printf(" align=%<DA>", bp[9]);
        if ( *((_WORD *)bp + 5) )
          ds_printf(" reserved=%H", *((unsigned __int16 *)bp + 5));
        ds_printf(" adr=%W", *((_DWORD *)bp + 3));
        ds_printf(" len=%W", *((_DWORD *)bp + 4));
        result = 20;
      }
      else
      {
        result = 8;
      }
      break;
    case 0x10u:
    case 0x11u:
    case 0x12u:
    case 0x13u:
      xd = (DBGP_XGKT_DATA *)(bp + 8);
      r_4 = (unsigned int)na >> 3;
      while ( 1 )
      {
        v5 = r_4--;
        if ( v5 <= 0 )
          break;
        ds_printf(" %W,%d", xd->flag, xd->length);
        xd = (DBGP_XGKT_DATA *)((char *)xd + 8);
      }
      result = na * ((unsigned int)na >> 3) + 8;
      break;
    case 0x18u:
      nc = na - 20;
      if ( nc >= 0 )
      {
        argc = *((_DWORD *)bp + 6);
        ds_printf(" entry=%W gp=%W argc=%d (", *((_DWORD *)bp + 2), *((_DWORD *)bp + 3), argc);
        wp_1 = (unsigned int *)(bp + 28);
        v9 = (char *)&bp[4 * *((_DWORD *)bp + 6) + 28];
        nd = nc - 4 * *((_DWORD *)bp + 6);
        for ( r_3 = 0; argc > r_3; ++r_3 )
        {
          ds_printf("\"");
          for ( p = v9; *p; ++p )
          {
            if ( *p == 34 )
              ds_printf("\\\"");
            else
              ds_printf("%C", *p);
          }
          if ( r_3 >= argc - 1 )
            v4 = "";
          else
            v4 = " ";
          ds_printf("\"%s", v4);
          v9 += *wp_1++;
        }
        ds_printf(")");
        result = nd + 28 + 4 * *((_DWORD *)bp + 6);
      }
      else
      {
        result = 8;
      }
      break;
    case 0x20u:
    case 0x21u:
      if ( (unsigned int)na <= 0xB )
        goto LABEL_63;
      ds_printf(" flag=0x%x", *((_DWORD *)bp + 2));
      ds_printf(" off=0x%x", *((_DWORD *)bp + 3));
      ds_printf(" cnt=0x%x", *((_DWORD *)bp + 4));
      result = 20;
      break;
    case 0x23u:
      if ( (unsigned int)na <= 0xF )
        goto LABEL_63;
      ds_printf(" flag=0x%x", *((_DWORD *)bp + 2));
      ds_printf(" len=0x%x", *((_DWORD *)bp + 3));
      ds_printf(" off=0x%x", *((_DWORD *)bp + 4));
      ds_printf(" seq=0x%x", *((_DWORD *)bp + 5));
      result = 24;
      break;
    case 0x24u:
    case 0x25u:
      if ( (unsigned int)na <= 3 )
        goto LABEL_63;
      ds_printf(" flag=0x%x", *((_DWORD *)bp + 2));
      result = 12;
      break;
    default:
LABEL_63:
      result = 8;
      break;
  }
  return result;
}

static int print_iloadp(unsigned __int8 *bp, int n)
{
  int result; // eax
  char *v3; // eax
  char *v4; // eax
  char *v5; // eax
  char *v6; // eax
  int v7; // eax
  int v8; // eax
  char *v9; // [esp-Ch] [ebp-24h]
  char *v10; // [esp-8h] [ebp-20h]
  char *v11; // [esp-4h] [ebp-1Ch]
  int l; // [esp+0h] [ebp-18h]
  int i; // [esp+4h] [ebp-14h]
  int i_1; // [esp+4h] [ebp-14h]
  int i_2; // [esp+4h] [ebp-14h]
  char *cp; // [esp+8h] [ebp-10h]
  unsigned int *wp; // [esp+Ch] [ebp-Ch]
  unsigned int na; // [esp+24h] [ebp+Ch]

  wp = (unsigned int *)(bp + 8);
  na = n - 8;
  if ( (na & 0x80000000) != 0 )
    return 0;
  ds_printf(" cmd=%<IC>", *bp);
  if ( *bp <= 1u )
  {
    if ( (bp[1] & 8) != 0 )
      v3 = " WATCH";
    else
      v3 = "";
    v11 = v3;
    if ( (bp[1] & 4) != 0 )
      v4 = " INFO";
    else
      v4 = "";
    v10 = v4;
    if ( (bp[1] & 2) != 0 )
      v5 = " START";
    else
      v5 = "";
    v9 = v5;
    if ( (bp[1] & 1) != 0 )
      v6 = " LOAD";
    else
      v6 = "";
    ds_printf(" action=(%s%s%s%s )", v6, v9, v10, v11);
  }
  else if ( *bp > 9u || *bp < 8u )
  {
    if ( bp[1] )
      ds_printf(" action=%B", bp[1]);
  }
  else
  {
    ds_printf(" action=");
    if ( bp[1] )
    {
      if ( bp[1] == 1 )
        ds_printf("START");
      else
        ds_printf("%B", bp[1]);
    }
    else
    {
      ds_printf("END");
    }
  }
  if ( bp[2] )
  {
    if ( *bp == 16 )
      ds_printf(" result=%<IR_R>", bp[2]);
    else
      ds_printf(" result=%<IR_N>", bp[2]);
  }
  ds_printf(" stamp=%B", bp[3]);
  if ( *((_DWORD *)bp + 1) )
    ds_printf(" module_id=%W", *((_DWORD *)bp + 1));
  switch ( *bp )
  {
    case 0u:
      cp = (char *)(bp + 8);
      i = 0;
      while ( (int)na > i )
      {
        ds_printf(" \"%S\"", (const wchar_t *)cp);
        l = strlen(cp) + 1;
        i += l;
        cp += l;
      }
      result = na + 8;
      break;
    case 1u:
    case 7u:
      if ( na > 0x1F )
      {
        ds_printf(" version=%H", *((unsigned __int16 *)bp + 4));
        ds_printf(" flags=%H", *((unsigned __int16 *)bp + 5));
        ds_printf(" maddr=%W", *((_DWORD *)bp + 3));
        ds_printf(" tsize=%W", *((_DWORD *)bp + 4));
        ds_printf(" dsize=%W", *((_DWORD *)bp + 5));
        ds_printf(" bsize=%W", *((_DWORD *)bp + 6));
        ds_printf(" name=\"%S\"", (const wchar_t *)bp + 10);
        result = na + 8;
      }
      else
      {
        result = 8;
      }
      break;
    case 5u:
      i_1 = na >> 2;
      while ( 1 )
      {
        v7 = i_1--;
        if ( v7 <= 0 )
          break;
        ds_printf(" %W", *wp++);
      }
      result = 4 * (na >> 2) + 8;
      break;
    case 0xBu:
      i_2 = na >> 2;
      while ( 1 )
      {
        v8 = i_2--;
        if ( v8 <= 0 )
          break;
        ds_printf(" %W", *wp++);
      }
      result = 4 * (na >> 2) + 8;
      break;
    case 0x10u:
      if ( na > 3 )
      {
        ds_printf(" ret=%W", *wp);
        result = 12;
      }
      else
      {
        result = 8;
      }
      break;
    default:
      result = 8;
      break;
  }
  return result;
}

static int print_eloadp(unsigned __int8 *bp, int n)
{
  int result; // eax
  char *v3; // eax
  char *v4; // eax
  char *v5; // eax
  char *v6; // eax
  int v7; // eax
  char *v8; // [esp-Ch] [ebp-24h]
  char *v9; // [esp-8h] [ebp-20h]
  char *v10; // [esp-4h] [ebp-1Ch]
  int l; // [esp+0h] [ebp-18h]
  int i; // [esp+4h] [ebp-14h]
  int i_1; // [esp+4h] [ebp-14h]
  char *cp; // [esp+8h] [ebp-10h]
  unsigned int *wp; // [esp+Ch] [ebp-Ch]
  unsigned int na; // [esp+24h] [ebp+Ch]

  wp = (unsigned int *)(bp + 8);
  na = n - 8;
  if ( (na & 0x80000000) != 0 )
    return 0;
  ds_printf(" cmd=%<EC>", *bp);
  if ( *bp <= 1u )
  {
    if ( (bp[1] & 8) != 0 )
      v3 = " WATCH";
    else
      v3 = "";
    v10 = v3;
    if ( (bp[1] & 4) != 0 )
      v4 = " INFO";
    else
      v4 = "";
    v9 = v4;
    if ( (bp[1] & 2) != 0 )
      v5 = " START";
    else
      v5 = "";
    v8 = v5;
    if ( (bp[1] & 1) != 0 )
      v6 = " LOAD";
    else
      v6 = "";
    ds_printf(" action=(%s%s%s%s )", v6, v8, v9, v10);
  }
  else if ( *bp > 9u || *bp < 8u )
  {
    if ( bp[1] )
      ds_printf(" action=%B", bp[1]);
  }
  else
  {
    ds_printf(" action=");
    if ( bp[1] )
    {
      if ( bp[1] == 1 )
        ds_printf("START");
      else
        ds_printf("%B", bp[1]);
    }
    else
    {
      ds_printf("END");
    }
  }
  if ( bp[2] )
  {
    if ( *bp == 16 )
      ds_printf(" result=%<ER_R>", bp[2]);
    else
      ds_printf(" result=%<ER_N>", bp[2]);
  }
  ds_printf(" stamp=%B", bp[3]);
  if ( *((_DWORD *)bp + 1) )
    ds_printf(" module_id=%W", *((_DWORD *)bp + 1));
  switch ( *bp )
  {
    case 0u:
      cp = (char *)(bp + 8);
      i = 0;
      while ( (int)na > i )
      {
        ds_printf(" \"%S\"", (const wchar_t *)cp);
        l = strlen(cp) + 1;
        i += l;
        cp += l;
      }
      result = na + 8;
      break;
    case 1u:
    case 7u:
      if ( na > 0x1F )
      {
        ds_printf(" version=%H", *((unsigned __int16 *)bp + 4));
        ds_printf(" flags=%H", *((unsigned __int16 *)bp + 5));
        ds_printf(" maddr=%W", *((_DWORD *)bp + 3));
        ds_printf(" tsize=%W", *((_DWORD *)bp + 4));
        ds_printf(" dsize=%W", *((_DWORD *)bp + 5));
        ds_printf(" bsize=%W", *((_DWORD *)bp + 6));
        ds_printf(" name=\"%S\"", (const wchar_t *)bp + 10);
        result = na + 8;
      }
      else
      {
        result = 8;
      }
      break;
    case 5u:
      i_1 = na >> 2;
      while ( 1 )
      {
        v7 = i_1--;
        if ( v7 <= 0 )
          break;
        ds_printf(" %W", *wp++);
      }
      result = 4 * (na >> 2) + 8;
      break;
    case 0x10u:
      if ( na > 3 )
      {
        ds_printf(" ret=%W", *wp);
        result = 12;
      }
      else
      {
        result = 8;
      }
      break;
    default:
      result = 8;
      break;
  }
  return result;
}

void ds_dump(char *str, DECI2_HDR *dh, int n)
{
  int protocol; // eax
  unsigned __int8 *v4; // [esp+0h] [ebp-Ch]
  unsigned __int8 *bp; // [esp+0h] [ebp-Ch]
  int r; // [esp+4h] [ebp-8h]
  int i; // [esp+8h] [ebp-4h]
  int na; // [esp+1Ch] [ebp+10h]

  v4 = (unsigned __int8 *)dh;
  if ( str )
    ds_printf("%s", str);
  if ( (unsigned int)n > 7 )
  {
    if ( dh->reserved )
      ds_printf(" reserved=%H", dh->reserved);
    ds_printf(" %<SP>,%h<SN>%h<SN>", dh->protocol, dh->source, dh->destination);
    na = n - 8;
    bp = (unsigned __int8 *)&dh[1];
    r = 0;
    protocol = dh->protocol;
    if ( protocol == 336 )
    {
      r = print_iloadp(bp, na);
      goto LABEL_44;
    }
    if ( dh->protocol <= 0x150u )
    {
      if ( protocol != 287 )
      {
        if ( dh->protocol > 0x11Fu )
        {
          if ( protocol == 304 )
            goto LABEL_39;
          if ( dh->protocol <= 0x130u )
          {
            if ( dh->protocol <= 0x124u )
              r = print_drfp(bp, na);
            goto LABEL_44;
          }
          if ( protocol == 320 )
LABEL_39:
            r = print_dbgp(bp, na, 0);
        }
        else
        {
          if ( protocol != 1 )
          {
            if ( !dh->protocol || dh->protocol > 0x119u || dh->protocol < 0x110u )
              goto LABEL_44;
            goto LABEL_38;
          }
          r = print_dcmp(bp, na);
        }
LABEL_44:
        n = na - r;
        v4 = &bp[r];
        goto LABEL_45;
      }
      goto LABEL_38;
    }
    if ( protocol != 576 )
    {
      if ( dh->protocol > 0x240u )
      {
        if ( protocol == 1024 )
        {
          r = print_netmp(bp, na);
          goto LABEL_44;
        }
        if ( dh->protocol <= 0x400u )
        {
          if ( protocol == 592 )
            r = print_eloadp(bp, na);
          goto LABEL_44;
        }
        if ( dh->protocol <= 0x413u && dh->protocol >= 0x410u )
          goto LABEL_38;
        goto LABEL_44;
      }
      if ( protocol == 543 )
      {
LABEL_38:
        r = print_ttyp(bp, na);
        goto LABEL_44;
      }
      if ( dh->protocol <= 0x21Fu )
      {
        if ( dh->protocol > 0x219u || dh->protocol < 0x210u )
          goto LABEL_44;
        goto LABEL_38;
      }
      if ( protocol != 560 )
        goto LABEL_44;
    }
    r = print_dbgp(bp, na, 1);
    goto LABEL_44;
  }
LABEL_45:
  if ( n > 0 )
  {
    for ( i = 0; n > i && i <= 15; ++i )
      ds_printf(" %02x", v4[i]);
    if ( n > 15 )
      ds_printf(" ...");
  }
  ds_printf("\n");
}

