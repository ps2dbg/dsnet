#include "dsnet_prototypes.h"

static char *sprint_hook = NULL;
static void *rec_fp = NULL;
DS_DESC *ds_disp_desc = NULL;
int ds_disp_proto = 1040;

static int doprnt(char *format, ...);
static int print_char_by_printable_char(int ch);
static int print_string_by_value(char *form, int sm, int value);
static int ds_vprintf(char *format, va_list ap);

static int doprnt(char *format, ...)
{
  int r; // [esp+0h] [ebp-8h]
  va_list ap; // [esp+14h] [ebp+Ch] BYREF

  va_start(ap, format);
  if ( sprint_hook )
  {
    r = vsprintf(sprint_hook, format, ap);
    if ( r > 0 )
      sprint_hook += r;
  }
  else
  {
    r = vfprintf(stdout, format, ap);
    if ( rec_fp )
      vfprintf((FILE *)rec_fp, format, ap);
  }
  va_end(ap);
  return r;
}

static int print_char_by_printable_char(int ch)
{
  if ( ch == 10 )
    return doprnt("\\n");
  if ( ch > 10 )
  {
    if ( ch == 27 )
      return doprnt("\\e");
    if ( ch > 27 )
    {
      if ( ch == 127 )
        return doprnt("\\?");
    }
    else if ( ch == 13 )
    {
      return doprnt("\\r");
    }
  }
  else
  {
    if ( ch == 8 )
      return doprnt("\\b");
    if ( ch > 8 )
      return doprnt("\\t");
    if ( !ch )
      return doprnt("\\0");
  }
  if ( (unsigned __int8)ch <= 0x1Fu )
    return doprnt("^%c", ch + 64);
  if ( (unsigned __int8)ch <= 0x7Eu )
    return doprnt("%c", ch);
  return doprnt("\\x%02x", ch & 0x7F);
}

static int print_string_by_value(char *form, int sm, int value)
{
  int ne; // [esp+0h] [ebp-18h]
  int r; // [esp+4h] [ebp-14h]
  char *df; // [esp+8h] [ebp-10h]
  char *cf; // [esp+Ch] [ebp-Ch]
  STRS *strs; // [esp+10h] [ebp-8h]
  FMT_STRS *fs; // [esp+14h] [ebp-4h]

  r = 0;
  for ( fs = fmt_strs; fs->ds_fmt; ++fs )
  {
    cf = form;
    for ( df = fs->ds_fmt; *df == *cf; ++df )
      ++cf;
    if ( *cf == 62 && !*df )
    {
      strs = fs->strs;
      for ( ne = fs->nent; ne > 0 && strs->value != value; --ne )
        ++strs;
      if ( ne <= 0 )
      {
        r = doprnt(fs->def_fmt, value);
      }
      else if ( sm == 104 && strs->sname )
      {
        r = doprnt("%s", strs->sname);
      }
      else
      {
        if ( sm == 108 )
          r = doprnt("%s_", fs->ds_fmt);
        r += doprnt("%s", strs->name);
      }
      break;
    }
  }
  if ( !fs->ds_fmt )
    r += doprnt("(""?""?""?""%s""?""?""?"")0x%x", form, value);
  return r;
}

static int ds_vprintf(char *format, va_list ap)
{
  char v2; // cl
  int v3; // eax
  char v4; // cl
  int v5; // eax
  int v7; // [esp-4h] [ebp-2058h]
  void *v8; // [esp+1Ch] [ebp-2038h]
  char *v9; // [esp+20h] [ebp-2034h]
  char *sval; // [esp+20h] [ebp-2034h]
  double v11; // [esp+24h] [ebp-2030h]
  int v13; // [esp+2Ch] [ebp-2028h]
  int v14; // [esp+30h] [ebp-2024h]
  int ival; // [esp+30h] [ebp-2024h]
  int v16; // [esp+38h] [ebp-201Ch]
  int astn; // [esp+3Ch] [ebp-2018h]
  int sm; // [esp+40h] [ebp-2014h]
  int r; // [esp+48h] [ebp-200Ch]
  char fmt[8192]; // [esp+4Ch] [ebp-2008h] BYREF
  char *q; // [esp+204Ch] [ebp-8h] BYREF
  char *p; // [esp+2050h] [ebp-4h]

  r = 0;
  p = format;
  while ( *p )
  {
    if ( *p == 37 )
    {
      q = fmt;
      if ( fmt < &fmt[8192] )
        *q++ = *p++;
      while ( *p == 35 || *p == 48 || *p == 45 || *p == 43 )
      {
        if ( q < &fmt[8192] )
          *q++ = *p++;
      }
      v16 = 0;
      astn = 0;
      if ( *p == 42 )
      {
        if ( q < &fmt[8192] )
          *q++ = *p++;
        astn = 1;
        v16 = va_arg(ap, _DWORD);
      }
      else
      {
        while ( (isdigit(*p)) != 0 )
        {
          if ( q < &fmt[8192] )
            *q++ = *p++;
        }
      }
      if ( *p == 46 )
      {
        if ( q < &fmt[8192] )
          *q++ = *p++;
        if ( *p == 42 )
        {
          if ( q < &fmt[8192] )
            *q++ = *p++;
          ++astn;
          v16 = va_arg(ap, _DWORD);
        }
        else
        {
          while ( (isdigit(*p)) != 0 )
          {
            if ( q < &fmt[8192] )
              *q++ = *p++;
          }
        }
      }
      sm = 0;
      while ( *p == 104 || *p == 108 || *p == 76 )
      {
        if ( q >= &fmt[8192] )
        {
          v3 = *p;
        }
        else
        {
          v2 = *p;
          *q = *p;
          v3 = v2;
          ++p;
          ++q;
        }
        sm = v3;
      }
      if ( q >= &fmt[8192] )
      {
        v5 = *p;
      }
      else
      {
        v4 = *p;
        *q = *p;
        v5 = v4;
        ++p;
        ++q;
      }
      *q = 0;
      switch ( v5 )
      {
        case '%':
          r += doprnt("%%");
          break;
        case '<':
          r += print_string_by_value(p, sm, va_arg(ap, int));
          while ( *p && *p != 62 )
            ++p;
          if ( *p == 62 )
            ++p;
          break;
        case 'B':
          r += doprnt("%02x", (unsigned __int8)va_arg(ap, _DWORD));
          break;
        case 'C':
          r += print_char_by_printable_char(va_arg(ap, int));
          break;
        case 'D':
        case 'O':
        case 'U':
          v13 = va_arg(ap, _DWORD);
          if ( astn == 1 )
          {
            r += doprnt(fmt, v16, v13);
          }
          else if ( astn > 1 )
          {
            if ( astn == 2 )
              r += doprnt(fmt, v16, 0, v13);
          }
          else if ( !astn )
          {
            r += doprnt(fmt, v13);
          }
          break;
        case 'E':
        case 'e':
        case 'f':
        case 'g':
          v11 = va_arg(ap, double);
          if ( astn == 1 )
          {
            r += doprnt(fmt, v16, v11);
          }
          else if ( astn > 1 )
          {
            if ( astn == 2 )
              r += doprnt(fmt, v16, 0, v11);
          }
          else if ( !astn )
          {
            r += doprnt(fmt, v11);
          }
          break;
        case 'H':
          r += doprnt("%04x", (unsigned __int16)va_arg(ap, _DWORD));
          break;
        case 'Q':
          r += doprnt(
                 "%08x_%08x_%08x_%08x",
                 va_arg(ap, _DWORD),
                 va_arg(ap, _DWORD),
                 va_arg(ap, _DWORD),
                 va_arg(ap, _DWORD));
          break;
        case 'S':
          sval = (char *)va_arg(ap, char *);
          while ( 1 )
          {
            ival = *sval++;
            if ( !ival )
              break;
            r += print_char_by_printable_char(ival);
          }
          break;
        case 'T':
          r += doprnt("%08x_%08x", va_arg(ap, _DWORD), va_arg(ap, _DWORD));
          break;
        case 'W':
          r += doprnt("%08x", va_arg(ap, _DWORD));
          break;
        case 'X':
        case 'c':
        case 'd':
        case 'i':
        case 'o':
        case 'u':
        case 'x':
          v14 = va_arg(ap, _DWORD);
          if ( astn == 1 )
          {
            r += doprnt(fmt, v16, v14);
          }
          else if ( astn > 1 )
          {
            if ( astn == 2 )
              r += doprnt(fmt, v16, 0, v14);
          }
          else if ( !astn )
          {
            r += doprnt(fmt, v14);
          }
          break;
        case 'p':
          v8 = va_arg(ap, void *);
          if ( astn == 1 )
          {
            r += doprnt(fmt, v16, v8);
          }
          else if ( astn > 1 )
          {
            if ( astn == 2 )
              r += doprnt(fmt, v16, 0, v8);
          }
          else if ( !astn )
          {
            r += doprnt(fmt, v8);
          }
          break;
        case 's':
          v9 = va_arg(ap, char *);
          if ( astn == 1 )
          {
            r += doprnt(fmt, v16, v9);
          }
          else if ( astn > 1 )
          {
            if ( astn == 2 )
              r += doprnt(fmt, v16, 0, v9);
          }
          else if ( !astn )
          {
            r += doprnt(fmt, v9);
          }
          break;
        default:
          r += doprnt("\nds_printf - invalid format \"%s\"\n", fmt);
          break;
      }
    }
    else
    {
      v7 = *p++;
      r += doprnt("%c", v7);
    }
  }
  return r;
}

int ds_printf(char *fmt, ...)
{
  DSP_BUF *db; // [esp+4h] [ebp-Ch]
  int r; // [esp+8h] [ebp-8h]
  va_list va; // [esp+1Ch] [ebp+Ch] BYREF

  va_start(va, fmt);
  sprint_hook = 0;
  r = ds_vprintf(fmt, va);
  fflush(stdout);
  if ( rec_fp )
    fflush((FILE *)rec_fp);
  if ( r > 0 )
  {
    if ( ds_disp_desc )
    {
      db = ds_alloc_buf((unsigned __int16)ds_disp_proto, 72, 0, r + 5);
      if ( db )
      {
        *(_DWORD *)&db->buf[8] = 0;
        *(_WORD *)db->buf = r + 12;
        sprint_hook = &db->buf[12];
        ds_vprintf(fmt, va);
        ds_send_desc(ds_disp_desc, db);
      }
    }
  }
  va_end(va);
  return r;
}

int ds_sprintf(char *buf, char *fmt, ...)
{
  va_list va; // [esp+18h] [ebp+10h] BYREF
  int ret;

  va_start(va, fmt);
  sprint_hook = buf;
  ret = ds_vprintf(fmt, va);
  va_end(va);
  return ret;
}

int ds_error(char *fmt, ...)
{
  char *v2; // eax
  char *estr; // [esp+0h] [ebp-408h]
  char buf[1024]; // [esp+4h] [ebp-404h] BYREF
  va_list ap; // [esp+404h] [ebp-4h]
  va_list va; // [esp+414h] [ebp+Ch] BYREF

  va_start(va, fmt);
  va_copy(ap, va);
  va_end(va);
  if ( *fmt == 33 )
  {
    ++fmt;
    v2 = strerror(errno);
  }
  else
  {
    v2 = 0;
  }
  estr = v2;
  sprint_hook = buf;
  ds_vprintf(fmt, ap);
  va_end(ap);
  if ( estr )
  {
    strcat(buf, " - ");
    strcat(buf, estr);
  }
  strcat(buf, "\n");
  ds_printf("%s", buf);
  return -1;
}

int ds_printf_cmd(int ac, char **av)
{
  int result; // eax
  char v3; // cl
  char v4; // al
  char *v5; // eax
  char *v6; // eax
  quad v7; // [esp-14h] [ebp-A48h]
  char *v8; // [esp-8h] [ebp-A3Ch]
  char *v9; // [esp-8h] [ebp-A3Ch]
  char *v10; // [esp-8h] [ebp-A3Ch]
  char *v11; // [esp-8h] [ebp-A3Ch]
  int v12; // [esp-4h] [ebp-A38h]
  _DWORD x[2]; // [esp+4h] [ebp-A30h] BYREF
  int x_8; // [esp+Ch] [ebp-A28h]
  quad qv; // [esp+10h] [ebp-A24h] BYREF
  unsigned int wv; // [esp+20h] [ebp-A14h] BYREF
  char str[513]; // [esp+24h] [ebp-A10h] BYREF
  char ch; // [esp+22Bh] [ebp-809h]
  char *q; // [esp+22Ch] [ebp-808h]
  char *p; // [esp+230h] [ebp-804h]
  char fmt[1024]; // [esp+234h] [ebp-800h] BYREF
  char format[1024]; // [esp+634h] [ebp-400h] BYREF
  int aca; // [esp+A3Ch] [ebp+8h]
  int acb; // [esp+A3Ch] [ebp+8h]
  char **ava; // [esp+A40h] [ebp+Ch]
  char **avb; // [esp+A40h] [ebp+Ch]

  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  ava = av + 1;
  if ( aca > 0 && **ava == 45 || aca <= 0 )
    return ds_error("Usage: printf \"<fmt>\" [<args>]...");
  x_8 = ds_scan_string(format, *ava);
  if ( x_8 < 0 )
    return -1;
  format[x_8] = 0;
  acb = aca - 1;
  avb = ava + 1;
  p = format;
  while ( 1 )
  {
LABEL_11:
    if ( !*p )
    {
      if ( acb <= 0 )
        return 0;
      else
        return ds_printf("\ntoo many args\n");
    }
    if ( *p == 37 )
      break;
    v12 = *p++;
    ds_printf("%c", v12);
  }
  q = fmt;
  if ( fmt < format )
    *q++ = *p++;
  while ( *p == 35 || *p == 46 || *p == 43 || *p == 45 || (isdigit(*p)) != 0 )
  {
    if ( q < format )
      *q++ = *p++;
  }
  if ( (*p == 104 || *p == 108 || *p == 76) && q < format )
    *q++ = *p++;
  if ( q >= format )
  {
    v4 = *p;
  }
  else
  {
    v3 = *p;
    *q = *p;
    v4 = v3;
    ++p;
    ++q;
  }
  ch = v4;
  *q = 0;
  switch ( ch )
  {
    case '%':
    case 'B':
    case 'C':
    case 'D':
    case 'H':
    case 'O':
    case 'S':
    case 'U':
    case 'W':
    case 'X':
    case 'c':
    case 'd':
    case 'i':
    case 'o':
    case 'p':
    case 's':
    case 'u':
    case 'x':
      if ( --acb < 0 )
        goto LABEL_64;
      v8 = *avb++;
      if ( ds_eval_word(v8, &wv) )
      {
        result = -1;
      }
      else
      {
        if ( ch != 115 )
        {
          ds_printf(fmt, wv);
          goto LABEL_11;
        }
        while ( 1 )
        {
          str[512] = 0;
          if ( ds_load_mem(wv, str, 512) )
            break;
          ds_printf("%s", str);
          wv += 512;
          if ( strlen(str) != 512 )
            goto LABEL_11;
        }
        result = -1;
      }
      break;
    case '<':
      if ( --acb < 0 )
        goto LABEL_64;
      v10 = *avb++;
      if ( !ds_eval_word(v10, &wv) )
      {
        while ( *p && *p != 62 )
        {
          if ( q < format )
            *q++ = *p++;
        }
        if ( *p == 62 && q < format )
          *q++ = *p++;
        *q = 0;
        ds_printf(fmt, wv);
        goto LABEL_11;
      }
      result = -1;
      break;
    case 'E':
    case 'e':
    case 'f':
    case 'g':
    case 'n':
      result = ds_printf("format '%c' is not supported\n", ch);
      break;
    case 'Q':
    case 'T':
      if ( --acb < 0 )
        goto LABEL_64;
      v9 = *avb++;
      if ( !ds_eval_quad(v9, &qv) )
      {
        if ( ch == 84 )
        {
          *(_QWORD *)&v7.xa[2] = *(_QWORD *)&qv.xa[1];
          v7.xa[1] = qv.xa[0];
          v7.xa[0] = (unsigned int)x;
          ds_cqt(v7);
          ds_printf(fmt, x[0], x[1]);
        }
        else
        {
          ds_printf(fmt, qv.xa[0], qv.xa[1], qv.xa[2], qv.xa[3]);
        }
        goto LABEL_11;
      }
      result = -1;
      break;
    case '{':
      if ( --acb >= 0 )
      {
        v11 = *avb++;
        if ( ds_eval_word(v11, &wv) )
        {
          result = -1;
        }
        else
        {
          for ( x_8 = 0; wv > x_8; ++x_8 )
          {
            while ( *p && *p != 125 && *p != 124 )
            {
              v5 = p++;
              if ( *v5 == 92 && *p )
                ++p;
            }
            if ( *p == 124 )
            {
              ++p;
            }
            else if ( !*p || *p == 125 )
            {
              break;
            }
          }
          if ( *p )
          {
            if ( *p == 125 )
            {
              ds_printf("%d", wv);
            }
            else
            {
              q = fmt;
              while ( *p && *p != 125 && *p != 124 )
              {
                if ( *p == 92 && p[1] )
                  ++p;
                if ( q < format )
                  *q++ = *p++;
              }
              *q = 0;
              ds_printf(fmt, wv);
            }
            while ( *p && *p != 125 )
            {
              v6 = p++;
              if ( *v6 == 92 && *p )
                ++p;
            }
            if ( *p )
            {
              ++p;
              goto LABEL_11;
            }
          }
          result = ds_printf("\ninvalid format\n");
        }
      }
      else
      {
LABEL_64:
        result = ds_printf("\ntoo short args\n");
      }
      break;
    default:
      result = ds_printf("\ninvalid format (%s)\n", fmt);
      break;
  }
  return result;
}

int ds_record_cmd(int ac, char **av)
{
  char *v3; // eax
  int f_append; // [esp+0h] [ebp-4h]
  int aca; // [esp+Ch] [ebp+8h]
  char **ava; // [esp+10h] [ebp+Ch]

  f_append = 0;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  for ( ava = av + 1; aca > 0 && **ava == 45; ++ava )
  {
    if ( strcmp("-a", *ava) )
      return ds_error("Usage: record [[-a] <fname>]");
    ++f_append;
    --aca;
  }
  if ( !aca && f_append || aca > 1 )
    return ds_error("Usage: record [[-a] <fname>]");
  if ( rec_fp )
  {
    ds_fclose(rec_fp);
    rec_fp = 0;
  }
  if ( !aca )
    return 0;
  if ( f_append )
    v3 = "a";
  else
    v3 = "w";
  rec_fp = ds_fopen(*ava, v3);
  if ( rec_fp )
    return 0;
  else
    return -1;
}

