#include "dsnet_prototypes.h"

static int char_value_map[256];

static int once_34 = 0;

static void __cdecl build_char_value_map();
static int __cdecl ds_scan_num_quad(char *s, quad *pv, int base);

int __cdecl ds_scan_proto(char *str, int *pv)
{
  if ( !strcmp("ANY", str) )
  {
    *pv = 0;
  }
  else if ( !strcmp("DCMP", str) )
  {
    *pv = 1;
  }
  else if ( !strcmp("ITTYP", str) )
  {
    *pv = 272;
  }
  else if ( !strcmp("I0TTYP", str) )
  {
    *pv = 272;
  }
  else if ( !strcmp("I1TTYP", str) )
  {
    *pv = 273;
  }
  else if ( !strcmp("I2TTYP", str) )
  {
    *pv = 274;
  }
  else if ( !strcmp("I3TTYP", str) )
  {
    *pv = 275;
  }
  else if ( !strcmp("I4TTYP", str) )
  {
    *pv = 276;
  }
  else if ( !strcmp("I5TTYP", str) )
  {
    *pv = 277;
  }
  else if ( !strcmp("I6TTYP", str) )
  {
    *pv = 278;
  }
  else if ( !strcmp("I7TTYP", str) )
  {
    *pv = 279;
  }
  else if ( !strcmp("I8TTYP", str) )
  {
    *pv = 280;
  }
  else if ( !strcmp("I9TTYP", str) )
  {
    *pv = 281;
  }
  else if ( !strcmp("IKTTYP", str) )
  {
    *pv = 287;
  }
  else if ( !strcmp("DRFP0", str) )
  {
    *pv = 288;
  }
  else if ( !strcmp("DRFP1", str) )
  {
    *pv = 289;
  }
  else if ( !strcmp("DRFP2", str) )
  {
    *pv = 290;
  }
  else if ( !strcmp("DRFP3", str) )
  {
    *pv = 291;
  }
  else if ( !strcmp("DRFP4", str) )
  {
    *pv = 292;
  }
  else if ( !strcmp("ISDBGP", str) )
  {
    *pv = 304;
  }
  else if ( !strcmp("ITDBGP", str) )
  {
    *pv = 320;
  }
  else if ( !strcmp("ILOADP", str) )
  {
    *pv = 336;
  }
  else if ( !strcmp("ETTYP", str) )
  {
    *pv = 528;
  }
  else if ( !strcmp("E0TTYP", str) )
  {
    *pv = 528;
  }
  else if ( !strcmp("E1TTYP", str) )
  {
    *pv = 529;
  }
  else if ( !strcmp("E2TTYP", str) )
  {
    *pv = 530;
  }
  else if ( !strcmp("E3TTYP", str) )
  {
    *pv = 531;
  }
  else if ( !strcmp("E4TTYP", str) )
  {
    *pv = 532;
  }
  else if ( !strcmp("E5TTYP", str) )
  {
    *pv = 533;
  }
  else if ( !strcmp("E6TTYP", str) )
  {
    *pv = 534;
  }
  else if ( !strcmp("E7TTYP", str) )
  {
    *pv = 535;
  }
  else if ( !strcmp("E8TTYP", str) )
  {
    *pv = 536;
  }
  else if ( !strcmp("E9TTYP", str) )
  {
    *pv = 537;
  }
  else if ( !strcmp("EKTTYP", str) )
  {
    *pv = 543;
  }
  else if ( !strcmp("ESDBGP", str) )
  {
    *pv = 560;
  }
  else if ( !strcmp("ETDBGP", str) )
  {
    *pv = 576;
  }
  else if ( !strcmp("ELOADP", str) )
  {
    *pv = 592;
  }
  else if ( !strcmp("NETMP", str) )
  {
    *pv = 1024;
  }
  else if ( !strcmp("NTTYP", str) )
  {
    *pv = 1040;
  }
  else if ( !strcmp("CTTYP", str) )
  {
    *pv = 1042;
  }
  else if ( !strcmp("STTYP", str) )
  {
    *pv = 1043;
  }
  else if ( !strcmp("PAMP", str) )
  {
    *pv = 1280;
  }
  else if ( ds_eval_word(str, (unsigned int *)pv) )
  {
    return -1;
  }
  return 0;
}

int __cdecl ds_scan_nid(char *str, int *pv)
{
  if ( !strcmp("EE", str) || !strcmp("E", str) )
  {
    *pv = 69;
  }
  else if ( !strcmp("HOST", str) || !strcmp("H", str) )
  {
    *pv = 72;
  }
  else if ( !strcmp("IOP", str) || !strcmp("I", str) )
  {
    *pv = 73;
  }
  else if ( ds_eval_word(str, (unsigned int *)pv) )
  {
    return -1;
  }
  return 0;
}

int __cdecl ds_scan_string(char *buf, char *str)
{
  int ch; // [esp+4h] [ebp-14h]
  int i_3; // [esp+8h] [ebp-10h]
  int i; // [esp+8h] [ebp-10h]
  int quote; // [esp+Ch] [ebp-Ch]
  char *sp; // [esp+10h] [ebp-8h]
  char *bp; // [esp+14h] [ebp-4h]

  bp = buf;
  quote = *str;
  sp = str + 1;
  if ( quote != 34 && quote != 39 )
    return ds_error("invalid string (start) - %s", str);
  while ( *sp && quote != *sp )
  {
    ch = *sp++;
    if ( ch == 92 )
    {
      if ( !*sp )
        return ds_error("invalid string (escape) - %s", str);
      if ( *sp <= 47 || *sp > 55 )
      {
        if ( *sp == 120 || *sp == 88 )
        {
          if ( (isxdigit(*++sp)) == 0 )
            return ds_error("invalid string (hex) - %s", str);
          ch = 0;
          for ( i = 0; i <= 1 && (isxdigit(*sp)) != 0; ++i )
          {
            if ( (isdigit(*sp)) != 0 )
            {
              ch = *sp++ + 16 * ch - 48;
            }
            else
            {
              if ( (islower(*sp)) != 0 )
                ch = *sp + 16 * ch - 87;
              else
                ch = *sp + 16 * ch - 55;
              ++sp;
            }
          }
        }
        else
        {
          ch = *sp++;
          switch ( ch )
          {
            case 'a':
              LOBYTE(ch) = 7;
              break;
            case 'b':
              LOBYTE(ch) = 8;
              break;
            case 'f':
              LOBYTE(ch) = 12;
              break;
            case 'n':
              LOBYTE(ch) = 10;
              break;
            case 'r':
              LOBYTE(ch) = 13;
              break;
            case 't':
              LOBYTE(ch) = 9;
              break;
            case 'v':
              LOBYTE(ch) = 11;
              break;
            default:
              break;
          }
        }
      }
      else
      {
        LOBYTE(ch) = 0;
        for ( i_3 = 0; i_3 <= 2 && *sp > 47 && *sp <= 55; ++i_3 )
          LOBYTE(ch) = *sp++ + 8 * ch - 48;
      }
    }
    *bp++ = ch;
  }
  if ( quote == *sp && !sp[1] )
    return bp - buf;
  else
    return ds_error("invalid string (end) - %s", str);
}

char *__cdecl ds_decode_args(int ac, char **av)
{
  int quote; // [esp+8h] [ebp-24h]
  int ch; // [esp+Ch] [ebp-20h]
  int ch_1; // [esp+Ch] [ebp-20h]
  int n; // [esp+10h] [ebp-1Ch]
  int i; // [esp+14h] [ebp-18h]
  int j_1; // [esp+14h] [ebp-18h]
  int i_3; // [esp+18h] [ebp-14h]
  int i_1; // [esp+18h] [ebp-14h]
  char *v11; // [esp+1Ch] [ebp-10h]
  char *sp; // [esp+20h] [ebp-Ch]
  char *bp; // [esp+24h] [ebp-8h]
  char *buf; // [esp+28h] [ebp-4h]

  i_3 = 0;
  n = 0;
  while ( ac > i_3 )
    n += strlen(av[i_3++]) + 1;
  buf = (char *)ds_alloc(n);
  if ( !buf )
    return 0;
  i_1 = 0;
  bp = buf;
  while ( ac > i_1 )
  {
    sp = av[i_1];
    v11 = sp;
    av[i_1] = bp;
    quote = -1;
    while ( 1 )
    {
      ch = *sp++;
      if ( !ch )
        break;
      if ( quote >= 0 && quote == ch )
      {
        quote = -1;
      }
      else if ( quote < 0 && (ch == 34 || ch == 39) )
      {
        quote = ch;
      }
      else if ( ch == 92 )
      {
        if ( !*sp )
        {
          ds_error("invalid escape - %s", v11);
          return (char *)ds_free(buf);
        }
        if ( *sp <= 47 || *sp > 55 )
        {
          if ( *sp == 120 || *sp == 88 )
          {
            if ( (isxdigit(*++sp)) == 0 )
            {
              ds_error("invalid hex - %s", v11);
              return (char *)ds_free(buf);
            }
            ch_1 = 0;
            for ( j_1 = 0; j_1 <= 1 && (isxdigit(*sp)) != 0; ++j_1 )
            {
              if ( (isdigit(*sp)) != 0 )
              {
                ch_1 = *sp++ + 16 * ch_1 - 48;
              }
              else
              {
                if ( (islower(*sp)) != 0 )
                  ch_1 = *sp + 16 * ch_1 - 87;
                else
                  ch_1 = *sp + 16 * ch_1 - 55;
                ++sp;
              }
            }
          }
          else
          {
            ch_1 = *sp++;
            switch ( ch_1 )
            {
              case 'a':
                LOBYTE(ch_1) = 7;
                break;
              case 'b':
                LOBYTE(ch_1) = 8;
                break;
              case 'f':
                LOBYTE(ch_1) = 12;
                break;
              case 'n':
                LOBYTE(ch_1) = 10;
                break;
              case 'r':
                LOBYTE(ch_1) = 13;
                break;
              case 't':
                LOBYTE(ch_1) = 9;
                break;
              case 'v':
                LOBYTE(ch_1) = 11;
                break;
              default:
                break;
            }
          }
        }
        else
        {
          LOBYTE(ch_1) = 0;
          for ( i = 0; i <= 2 && *sp > 47 && *sp <= 55; ++i )
            LOBYTE(ch_1) = *sp++ + 8 * ch_1 - 48;
        }
        *bp++ = ch_1;
      }
      else
      {
        *bp++ = ch;
      }
    }
    if ( quote >= 0 )
    {
      ds_error("invalid quote - %s", v11);
      return (char *)ds_free(buf);
    }
    *bp++ = 0;
    ++i_1;
  }
  return buf;
}

static void __cdecl build_char_value_map()
{
  int i; // [esp+0h] [ebp-4h]
  int i_1; // [esp+0h] [ebp-4h]
  int i_2; // [esp+0h] [ebp-4h]
  int i_3; // [esp+0h] [ebp-4h]

  if ( ++once_34 == 1 )
  {
    for ( i = 0; i <= 255; ++i )
      char_value_map[i] = -1;
    for ( i_1 = 48; i_1 <= 57; ++i_1 )
      char_value_map[i_1] = i_1 - 48;
    for ( i_2 = 65; i_2 <= 70; ++i_2 )
      char_value_map[i_2] = i_2 - 55;
    for ( i_3 = 97; i_3 <= 102; ++i_3 )
      char_value_map[i_3] = i_3 - 87;
  }
}

int __cdecl ds_scan_digit_word(char *s, unsigned int *pv)
{
  int v3; // eax
  int v; // [esp+0h] [ebp-Ch]
  int value; // [esp+4h] [ebp-8h]

  value = 0;
  build_char_value_map();
  if ( (isdigit(*s)) == 0 )
    return -1;
  if ( *s == 48 )
  {
    if ( *++s == 120 || *s == 88 )
    {
      ++s;
      v3 = 16;
    }
    else
    {
      v3 = 8;
    }
  }
  else
  {
    v3 = 10;
  }
  if ( v3 == 16 && !*s )
    return -1;
  while ( *s )
  {
    v = char_value_map[(unsigned __int8)*s];
    if ( v < 0 )
      return -1;
    if ( v >= v3 )
      return -1;
    value = v3 * value + v;
    ++s;
  }
  if ( pv )
    *pv = value;
  return 0;
}

static int __cdecl ds_scan_num_quad(char *s, quad *pv, int base)
{
  quad v4; // [esp-10h] [ebp-80h]
  quad v5; // [esp-10h] [ebp-80h]
  quad v6; // [esp+8h] [ebp-68h] BYREF
  quad v7; // [esp+18h] [ebp-58h] BYREF
  quad v8; // [esp+28h] [ebp-48h] BYREF
  int us; // [esp+38h] [ebp-38h]
  int v; // [esp+3Ch] [ebp-34h]
  quad cv; // [esp+40h] [ebp-30h] BYREF
  quad qv; // [esp+50h] [ebp-20h] BYREF
  quad qbase; // [esp+60h] [ebp-10h] BYREF

  us = 0;
  ds_cwq(&qbase, base);
  ds_bzero(&qv, sizeof(quad));
  ds_bzero(&cv, sizeof(quad));
  while ( *s )
  {
    if ( *s == 95 )
    {
      if ( ++us > 3 )
        return -1;
      v4 = cv;
      ds_cwq(&v7, 0x20u);
      ds_qshl(&v8, qv, v7);
      ds_qor(&qv, v8, v4);
      ds_bzero(&cv, sizeof(quad));
    }
    else
    {
      v = char_value_map[(unsigned __int8)*s];
      if ( v < 0 )
        return -1;
      if ( v >= base )
        return -1;
      ds_cwq(&v7, v);
      ds_qmul(&v8, cv, qbase);
      ds_qadd(&cv, v8, v7);
    }
    ++s;
  }
  if ( pv )
  {
    v5 = cv;
    ds_cwq(&v6, 0x20u);
    ds_qshl(&v8, qv, v6);
    ds_qor(&v7, v8, v5);
    *pv = v7;
  }
  return 0;
}

int __cdecl ds_scan_digit_quad(char *s, quad *pv)
{
  int v3; // eax

  build_char_value_map();
  if ( (isdigit(*s)) == 0 )
    return -1;
  if ( *s == 48 )
  {
    if ( *++s == 120 || *s == 88 )
    {
      ++s;
      v3 = 16;
    }
    else
    {
      v3 = 8;
    }
  }
  else
  {
    v3 = 10;
  }
  if ( v3 != 16 || *s )
    return ds_scan_num_quad(s, pv, v3);
  else
    return -1;
}

int __cdecl ds_scan_hex_quad(char *str, quad *pv)
{
  char *s; // [esp+0h] [ebp-4h]

  s = str;
  build_char_value_map();
  if ( (isxdigit(*str)) == 0 )
    return ds_error("invalid hex - %s", str);
  if ( *str == 48 && (str[1] == 120 || str[1] == 88) )
  {
    s = str + 2;
    if ( !str[2] )
      return -1;
  }
  if ( ds_scan_num_quad(s, pv, 16) < 0 )
    return ds_error("invalid hex - %s", str);
  return 0;
}

int __cdecl ds_scan_hex_word(char *str, unsigned int *pv)
{
  int v; // [esp+0h] [ebp-Ch]
  int value; // [esp+4h] [ebp-8h]
  char *s; // [esp+8h] [ebp-4h]

  s = str;
  value = 0;
  build_char_value_map();
  if ( (isxdigit(*str)) == 0 )
    return ds_error("invalid hex - %s", str);
  if ( *str == 48 && (str[1] == 120 || str[1] == 88) )
  {
    s = str + 2;
    if ( !str[2] )
      return -1;
  }
  while ( *s )
  {
    v = char_value_map[(unsigned __int8)*s];
    if ( v < 0 )
      return ds_error("invalid hex - %s", str);
    value = v | (16 * value);
    ++s;
  }
  if ( pv )
    *pv = value;
  return 0;
}

int __cdecl ds_scan_symbol_or_hex_quad(char *str, quad *pq)
{
  quad v3; // [esp+0h] [ebp-18h] BYREF
  char *p; // [esp+10h] [ebp-8h]
  unsigned int wv; // [esp+14h] [ebp-4h] BYREF

  if ( *str == 46 && (isdigit(str[1])) != 0 )
  {
    if ( ds_scan_digit_quad(str + 1, pq) )
      return ds_error("invalid digit - %s", str + 1);
    else
      return 0;
  }
  if ( ds_opt_hex_radix && !ds_opt_hex_radix->val )
  {
    if ( (isdigit(*str)) != 0 )
    {
      if ( ds_scan_digit_quad(str, pq) )
        return ds_error("invalid digit - %s", str);
      else
        return 0;
    }
    goto LABEL_12;
  }
  if ( (isdigit(*str)) != 0 )
    return ds_scan_hex_quad(str, pq);
  for ( p = str; *p && (isxdigit(*p)) != 0; ++p )
    ;
  if ( *p )
  {
LABEL_12:
    if ( ds_symbol_to_value(str, &wv) )
      return ds_error("symbol not found - %s", str);
    ds_cwq(&v3, wv);
    *pq = v3;
    return 0;
  }
  if ( ds_symbol_to_value(str, &wv) )
    return ds_scan_hex_quad(str, pq);
  ds_printf("use '0x%s' for hex, or ':%s' for symbol\n", str, str);
  return -1;
}

int __cdecl ds_scan_esc(char *p, int *pv)
{
  if ( p && *p )
  {
    if ( *p == 94 )
    {
      if ( !p[1] || p[2] )
        return -1;
      *pv = p[1] & 0x1F;
    }
    else
    {
      if ( p[1] )
        return -1;
      *pv = *p;
    }
  }
  else
  {
    *pv = 0;
  }
  return 0;
}

char *__cdecl ds_scan_args(char *s, int *pac, char **av, int max_args)
{
  int quote; // [esp+4h] [ebp-Ch]
  int par; // [esp+8h] [ebp-8h]
  int ac; // [esp+Ch] [ebp-4h]
  char *sa; // [esp+18h] [ebp+8h]

  while ( *s && (isspace(*s)) != 0 )
    ++s;
  for ( ac = 0; ; ++ac )
  {
    if ( !*s || max_args <= ac )
      goto LABEL_64;
    if ( !ac && *s == 33 )
    {
      *av = "!";
      ac = 1;
      for ( sa = s + 1; *sa && (isspace(*sa)) != 0; ++sa )
        ;
      if ( *sa )
      {
        av[1] = sa;
        ac = 2;
      }
      s = "";
      goto LABEL_64;
    }
    if ( *s == 59 )
      break;
    if ( *s == 35 )
    {
      *s = 0;
      goto LABEL_64;
    }
    av[ac] = s;
    par = 0;
    quote = -1;
    while ( *s && (par > 0 || quote >= 0 || *s != 59 && *s != 35 && (isspace(*s)) == 0) )
    {
      if ( *s == 92 )
      {
        if ( s[1] )
          ++s;
      }
      else if ( quote < 0 )
      {
        if ( *s == 34 || *s == 39 )
        {
          quote = *s;
        }
        else if ( *s == 40 )
        {
          ++par;
        }
        else if ( *s == 41 && --par < 0 )
        {
          goto LABEL_49;
        }
      }
      else if ( quote == *s )
      {
        quote = -1;
      }
      ++s;
    }
    if ( quote >= 0 )
    {
      ds_error("unterminated '%c'", quote);
LABEL_65:
      *pac = -1;
      return "";
    }
    if ( par > 0 )
    {
LABEL_49:
      ds_error("unblaced '('");
      goto LABEL_65;
    }
    if ( (isspace(*s)) != 0 )
    {
      *s++ = 0;
      while ( (isspace(*s)) != 0 )
        ++s;
    }
    if ( *s == 59 )
    {
      *s++ = 0;
      ++ac;
      goto LABEL_56;
    }
    if ( *s == 35 )
      *s = 0;
  }
  ++s;
LABEL_56:
  while ( *s && ((isspace(*s)) != 0 || *s == 59) )
    ++s;
LABEL_64:
  av[ac] = 0;
  *pac = ac;
  return s;
}

