#include "dsnet_prototypes.h"

char *ds_stamp_str[2] = { DSNET_STAMP_STRING, DSNET_VERSION_STRING };
char *ds_version_str = DSNET_VERSION_STRING;

static int escape = 29;
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I) || defined(DSNET_COMPILING_M)
static int f_ncmv = 1;
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I || DSNET_COMPILING_M */
static int dsm_waiting = -1;
static int to_sec = 120;
static int to_usec = 0;
static int bitmap = 33791;
static int f_nokbd = 0;
static int f_nostty = 0;
#ifdef DSNET_COMPILING_E
static int kbd_node = 69;
static int kbd_proto = 528;
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
static int kbd_node = 73;
static int kbd_proto = 272;
#endif /* DSNET_COMPILING_I */
#if defined(DSNET_COMPILING_D) || defined(DSNET_COMPILING_M)
static int kbd_node = 72;
#ifdef DSNET_COMPILING_D
static int kbd_proto = 1041;
#endif /* DSNET_COMPILING_D */
#ifdef DSNET_COMPILING_M
static int kbd_proto = 1040;
#endif /* DSNET_COMPILING_M */
#endif /* DSNET_COMPILING_D || DSNET_COMPILING_M */

static DS_DESC *target_desc;

#if defined(DSNET_COMPILING_D) || defined(DSNET_COMPILING_M)
static void send_char(int ch);
#endif /* DSNET_COMPILING_D || DSNET_COMPILING_M */
static DSP_BUF *recv_ttyp(DS_DESC *desc, DSP_BUF *db);
static void start_cons();
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I) || defined(DSNET_COMPILING_M)
static DSP_BUF *recv_netmp(DS_DESC *desc, DSP_BUF *db);
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I)
static int send_netmp_connect_request();
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I */
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I || DSNET_COMPILING_M */
static int usage(int f_true);

#if defined(DSNET_COMPILING_D) || defined(DSNET_COMPILING_M)
static void send_char(int ch)
{
  struct dscons_send_char_struct dat; // [esp+0h] [ebp-Ch] BYREF
  DSP_BUF *db; // [esp+8h] [ebp-4h]

  dat.zero = 0;
  dat.str[0] = ch;
#ifdef DSNET_COMPILING_D
  db = ds_alloc_buf(1041, (unsigned __int8)kbd_node, &dat, sizeof(dat));
#endif /* DSNET_COMPILING_D */
#ifdef DSNET_COMPILING_M
  db = ds_alloc_buf(1040, (unsigned __int8)kbd_node, &dat, sizeof(dat));
#endif /* DSNET_COMPILING_M */
  if ( db )
    ds_send_desc(target_desc, db);
}
#endif /* DSNET_COMPILING_D || DSNET_COMPILING_M */

static DSP_BUF *recv_ttyp(DS_DESC *desc, DSP_BUF *db)
{
  int len; // [esp+0h] [ebp-10h]
  int i; // [esp+4h] [ebp-Ch]
  int i_1; // [esp+4h] [ebp-Ch]
  char *v6; // [esp+8h] [ebp-8h]

  v6 = &db->buf[12];
  if ( !db )
    ds_exit(130);
  len = *(unsigned __int16 *)db->buf - 12;
  if ( desc->type == 1 )
  {
    for ( i = 0; len > i; ++i )
    {
      if ( escape == (unsigned __int8)v6[i] )
      {
        ds_printf("\n");
        ds_exit(0);
      }
    }
    if ( dsm_waiting >= 0 || f_nokbd )
    {
      return db;
    }
    else
    {
      db->buf[7] = kbd_node;
      *(_WORD *)&db->buf[4] = kbd_proto;
      return ds_send_desc(target_desc, db);
    }
  }
  else
  {
    if ( !*(_DWORD *)&db->buf[8] )
    {
      for ( i_1 = 0; len > i_1; ++i_1 )
        ds_printf("%c", (unsigned __int8)v6[i_1]);
    }
    return ds_free_buf(db);
  }
}

static void start_cons()
{
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I)
  const char *v0; // eax
  const char *v1; // eax
  const char *v2; // eax
  const char *v3; // eax
  const char *v4; // [esp+0h] [ebp-Ch]
  int n; // [esp+4h] [ebp-8h]
  int n_1; // [esp+4h] [ebp-8h]
  int i_3; // [esp+8h] [ebp-4h]
  int i; // [esp+8h] [ebp-4h]
  int i_2; // [esp+8h] [ebp-4h]
  int j; // [esp+8h] [ebp-4h]

  v4 = ",";
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I */
  dsm_waiting = -1;
  to_usec = -1;
  to_sec = -1;
  ds_printf("target=%<SN>", kbd_node);
  ds_printf(" display=");
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I)
  i_3 = 0;
  n = 0;
  while ( i_3 <= 15 )
  {
    if ( ((bitmap >> i_3) & 1) != 0 )
      ++n;
    ++i_3;
  }
  if ( n <= 0 )
  {
    v4 = "";
  }
  else
  {
#ifdef DSNET_COMPILING_E
    ds_printf("E");
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
    ds_printf("I");
#endif /* DSNET_COMPILING_I */
    if ( n <= 1 )
      v0 = "";
    else
      v0 = "[";
    ds_printf("%s", v0);
    for ( i = 0; i <= 9; ++i )
    {
      if ( ((bitmap >> i) & 1) != 0 )
        ds_printf("%c", i + 48);
    }
    if ( (bitmap & 0x8000u) != 0 )
      ds_printf("K");
    if ( n <= 1 )
      v1 = "";
    else
      v1 = "]";
    ds_printf("%s", v1);
    ds_printf("TTYP");
  }
  n_1 = 0;
  for ( i_2 = 16; i_2 <= 31; ++i_2 )
  {
    if ( ((bitmap >> i_2) & 1) != 0 )
      ++n_1;
  }
  if ( n_1 > 0 )
  {
#ifdef DSNET_COMPILING_E
    ds_printf("%sI", v4);
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
    ds_printf("%sE", v4);
#endif /* DSNET_COMPILING_I */
    if ( n_1 <= 1 )
      v2 = "";
    else
      v2 = "[";
    ds_printf("%s", v2);
    for ( j = 16; j <= 25; ++j )
    {
      if ( ((bitmap >> j) & 1) != 0 )
        ds_printf("%c", j + 32);
    }
    if ( bitmap < 0 )
      ds_printf("K");
    if ( n_1 <= 1 )
      v3 = "";
    else
      v3 = "]";
    ds_printf("%s", v3);
    ds_printf("TTYP");
  }
  if ( !f_nostty )
    ds_printf(",STTYP");
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I */
#ifdef DSNET_COMPILING_D
  ds_printf("DTTYP");
#endif /* DSNET_COMPILING_D */
#ifdef DSNET_COMPILING_M
  ds_printf("NTTYP");
#endif /* DSNET_COMPILING_M */
  if ( f_nokbd )
    ds_printf(" no-keyboard");
  else
    ds_printf(" keyboard=%<SP>", kbd_proto);
  ds_printf(" escape=%C", escape);
  ds_printf("\n");
#if defined(DSNET_COMPILING_D) || defined(DSNET_COMPILING_M)
  send_char(12);
#endif /* DSNET_COMPILING_D || DSNET_COMPILING_M */
}

#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I) || defined(DSNET_COMPILING_M)
static DSP_BUF *recv_netmp(DS_DESC *desc, DSP_BUF *db)
{
  int v2; // eax
  int n; // [esp+0h] [ebp-Ch]
  NETMP_HDR *v5; // [esp+4h] [ebp-8h]

  v5 = (NETMP_HDR *)&db->buf[8];
  if ( !db )
    ds_exit(130);
  if ( dsm_waiting == v5->code )
  {
    if ( v5->code != 1 )
    {
      if ( v5->code == 5 )
      {
        if ( !f_ncmv )
        {
          v2 = strlen((const char *)ds_version_str);
          ds_send_netmp(target_desc, 10, 0, ds_version_str, v2);
          dsm_waiting = 11;
          return ds_free_buf(db);
        }
      }
      else
      {
        if ( v5->code != 11 )
          return ds_free_buf(db);
        n = *(unsigned __int16 *)db->buf - 10;
        if ( db->buf[9] )
          ds_exit(148);
        if ( n != strlen((const char *)ds_version_str) )
          ds_exit_invalid_version(&db->buf[10], n, (char *)ds_version_str);
        if ( ds_strncmp((char *)ds_version_str, &db->buf[10], n) )
          ds_exit_invalid_version(&db->buf[10], n, (char *)ds_version_str);
      }
      start_cons();
      return ds_free_buf(db);
    }
    if ( db->buf[9] )
      ds_exit(132);
    ds_send_netmp_default_msg(target_desc);
    dsm_waiting = 5;
  }
  else if ( dsm_waiting == 11 && v5->code == 10 && db->buf[9] )
  {
    ds_exit(148);
  }
  return ds_free_buf(db);
}

#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I)
static int send_netmp_connect_request()
{
  int i; // [esp+4h] [ebp-8Ch]
  NETMP_PROTOS *p; // [esp+8h] [ebp-88h]
  NETMP_PROTOS protos[33]; // [esp+Ch] [ebp-84h] BYREF

  p = protos;
  ds_bzero(protos, sizeof(protos));
  for ( i = 0; i <= 31; ++i )
  {
    if ( ((bitmap >> i) & 1) != 0 )
    {
      p->pri = 0x80;
      if ( i > 15 )
#ifdef DSNET_COMPILING_E
        p->proto = i + 256;
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
        p->proto = i + 512;
#endif /* DSNET_COMPILING_I */
      else
#ifdef DSNET_COMPILING_E
        p->proto = i + 528;
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
        p->proto = i + 272;
#endif /* DSNET_COMPILING_I */
      ++p;
    }
  }
  if ( !f_nostty )
  {
    p->pri = 0x80;
    p->proto = 1043;
    ++p;
  }
  return ds_send_netmp(target_desc, 0, 0, protos, (char *)p - (char *)protos);
}
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I */
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I || DSNET_COMPILING_M */

static int usage(int f_true)
{
  if ( !f_true )
    return 0;
  ds_printf("Usage: %s [<option>]... [ei0-9k]...\n", ds_program_name);
  ds_printf("  <option>:\n");
  ds_printf("    -d <host>[/<port_offset>]  set hostname and port offset of manager\n");
  ds_printf("    -d /<port_offset>          set port offset of manager\n");
  ds_printf("    -d <host>[:<port>]         set hostname and port number of manager\n");
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I) || defined(DSNET_COMPILING_M)
  ds_printf("    -ncmv                      do not check manager version\n");
  ds_printf("    -cmv                       check manager version\n");
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I || DSNET_COMPILING_M */
  ds_printf("    -ns                        do not display STTYP\n");
  ds_printf("    -e <escape>                set escape character\n");
  ds_printf("    -nokbd                     do not send input data to target\n");
  return ds_exit(129);
}

int main(int ac, char **av)
{
  int v2; // eax
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I)
  int v4; // [esp+4h] [ebp-18h]
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I */
  int r; // [esp+8h] [ebp-14h]
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I) || defined(DSNET_COMPILING_D)
  int r_1; // [esp+8h] [ebp-14h]
  int r_2; // [esp+8h] [ebp-14h]
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I || DSNET_COMPILING_D */
  char d; // [esp+Eh] [ebp-Eh]
  char c; // [esp+Fh] [ebp-Dh]
  char c_1; // [esp+Fh] [ebp-Dh]
  char c_2; // [esp+Fh] [ebp-Dh]
  char *p; // [esp+10h] [ebp-Ch]
  char *target_name; // [esp+14h] [ebp-8h]
  DS_DESC *desc; // [esp+18h] [ebp-4h]
  int argc; // [esp+24h] [ebp+8h]
  char **argv; // [esp+28h] [ebp+Ch]

#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I)
  v4 = 1;
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I */
  ds_program_name = ds_basename(*av);
  ds_printf("%s (%s)\n", ds_program_name, ds_stamp_str[0]);
  target_name = ds_getenv("DSNETM");
  argc = ac - 1;
  for ( argv = av + 1; argc > 0 && **argv == 45; ++argv )
  {
    if ( !strcmp("-d", *argv) )
    {
      usage(--argc <= 0);
      target_name = *++argv;
    }
    else if ( !strcmp("-e", *argv) )
    {
      usage(--argc <= 0);
      v2 = ds_scan_esc(*++argv, &escape);
      usage(v2);
    }
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I) || defined(DSNET_COMPILING_M)
    else if ( !strcmp("-ncmv", *argv) )
    {
      f_ncmv = 1;
    }
    else if ( !strcmp("-cmv", *argv) )
    {
      f_ncmv = 0;
    }
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I || DSNET_COMPILING_M */
    else if ( !strcmp("-ns", *argv) )
    {
      ++f_nostty;
    }
    else if ( !strcmp("-nokbd", *argv) )
    {
      ++f_nokbd;
    }
    else
    {
      usage(1);
    }
    --argc;
  }
  if ( argc > 0 )
  {
    bitmap = 0;
    while ( argc > 0 )
    {
      for ( p = *argv; ; ++p )
      {
        c = *p;
        if ( !*p )
          break;
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I)
        if ( c == 105 || c == 73 || c == 101 || c == 69 )
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I */
#if defined(DSNET_COMPILING_D) || defined(DSNET_COMPILING_M)
        if ( c != 105 && c != 73 && c != 101 && c != 69 )
#endif /* DSNET_COMPILING_D || DSNET_COMPILING_M */
        {
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I)
          v4 = 1;
#ifdef DSNET_COMPILING_E
          if ( c == 105 || c == 73 )
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
          if ( c == 101 || c == 69 )
#endif /* DSNET_COMPILING_I */
            v4 = 0x10000;
        }
        else if ( c <= 47 || c > 57 )
        {
          if ( c == 107 || c == 75 )
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I */
#if defined(DSNET_COMPILING_D) || defined(DSNET_COMPILING_M)
          if ( c <= 47 || c > 57 )
#endif /* DSNET_COMPILING_D || DSNET_COMPILING_M */
          {
#if defined(DSNET_COMPILING_D) || defined(DSNET_COMPILING_M)
            if ( c == 107 || c == 75 )
            {
              if ( !bitmap && !f_nokbd )
              {
                kbd_node = 72;
#ifdef DSNET_COMPILING_D
                kbd_proto = 1056;
#endif /* DSNET_COMPILING_D */
#ifdef DSNET_COMPILING_M
                kbd_proto = 1055;
#endif /* DSNET_COMPILING_M */
              }
              bitmap |= 0x8000u;
            }
            else
            {
              usage(1);
            }
          }
          else
          {
#endif /* DSNET_COMPILING_D || DSNET_COMPILING_M */
            if ( !bitmap && !f_nokbd )
            {
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I)
              if ( v4 == 1 )
#ifdef DSNET_COMPILING_E
              {
                kbd_node = 69;
                kbd_proto = 543;
              }
              else
#endif /* DSNET_COMPILING_E */
              {
                kbd_node = 73;
                kbd_proto = 287;
              }
#ifdef DSNET_COMPILING_I
              else
              {
                kbd_node = 69;
                kbd_proto = 543;
              }
#endif /* DSNET_COMPILING_I */
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I */
#if defined(DSNET_COMPILING_D) || defined(DSNET_COMPILING_M)
              kbd_node = 72;
#ifdef DSNET_COMPILING_D
              kbd_proto = c + 993;
#endif /* DSNET_COMPILING_D */
#ifdef DSNET_COMPILING_M
              kbd_proto = c + 992;
#endif /* DSNET_COMPILING_M */
#endif /* DSNET_COMPILING_D || DSNET_COMPILING_M */
            }
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I)
            bitmap |= v4 << 15;
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I */
#if defined(DSNET_COMPILING_D) || defined(DSNET_COMPILING_M)
            bitmap |= 1 << (c - 48);
#endif /* DSNET_COMPILING_D || DSNET_COMPILING_M */
          }
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I)
          else
          {
            usage(1);
          }
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I */
        }
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I)
        else
        {
          if ( !bitmap && !f_nokbd )
          {
            if ( v4 == 1 )
#ifdef DSNET_COMPILING_E
            {
              kbd_node = 69;
              kbd_proto = c + 480;
            }
            else
#endif /* DSNET_COMPILING_E */
            {
              kbd_node = 73;
              kbd_proto = c + 224;
            }
#ifdef DSNET_COMPILING_I
            else
            {
              kbd_node = 69;
              kbd_proto = c + 480;
            }
#endif /* DSNET_COMPILING_I */
          }
          bitmap |= v4 << (c - 48);
        }
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I */
        if ( p[1] == 45 )
        {
          d = p[2];
          if ( d <= 47 || d > 57 )
          {
            if ( d == 107 || d == 75 )
            {
              for ( c_2 = c + 1; c_2 <= 57; ++c_2 )
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I)
                bitmap |= v4 << (c_2 - 48);
              bitmap |= v4 << 15;
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I */
#if defined(DSNET_COMPILING_D) || defined(DSNET_COMPILING_M)
                bitmap |= 1 << (c_2 - 48);
              bitmap |= 0x8000u;
#endif /* DSNET_COMPILING_D || DSNET_COMPILING_M */
            }
            else
            {
              usage(1);
            }
          }
          else
          {
            for ( c_1 = c + 1; d >= c_1; ++c_1 )
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I)
              bitmap |= v4 << (c_1 - 48);
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I */
#if defined(DSNET_COMPILING_D) || defined(DSNET_COMPILING_M)
              bitmap |= 1 << (c_1 - 48);
#endif /* DSNET_COMPILING_D || DSNET_COMPILING_M */
          }
          p += 2;
        }
      }
      --argc;
      ++argv;
    }
  }
  target_desc = ds_connect_net(target_name, 0);
  if ( !target_desc )
    ds_exit(132);
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I) || defined(DSNET_COMPILING_M)
  if ( !ds_add_recv_func(target_desc, 1024, -1, -1, recv_netmp) )
    ds_exit(135);
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I || DSNET_COMPILING_M */
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I) || defined(DSNET_COMPILING_D)
  for ( r = 0; r <= 15; ++r )
  {
#ifdef DSNET_COMPILING_E
    if ( ((bitmap >> r) & 1) != 0 && !ds_add_recv_func(target_desc, r + 528, -1, -1, recv_ttyp) )
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
    if ( ((bitmap >> r) & 1) != 0 && !ds_add_recv_func(target_desc, r + 272, -1, -1, recv_ttyp) )
#endif /* DSNET_COMPILING_I */
#ifdef DSNET_COMPILING_D
    if ( ((bitmap >> r) & 1) != 0 && !ds_add_recv_func(target_desc, r + 1041, -1, -1, recv_ttyp) )
#endif /* DSNET_COMPILING_D */
      ds_exit(135);
  }
  for ( r_1 = 16; r_1 <= 31; ++r_1 )
  {
#ifdef DSNET_COMPILING_E
    if ( ((bitmap >> r_1) & 1) != 0 && !ds_add_recv_func(target_desc, r_1 + 256, -1, -1, recv_ttyp) )
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
    if ( ((bitmap >> r_1) & 1) != 0 && !ds_add_recv_func(target_desc, r_1 + 512, -1, -1, recv_ttyp) )
#endif /* DSNET_COMPILING_I */
#ifdef DSNET_COMPILING_D
    if ( ((bitmap >> r_1) & 1) != 0 && !ds_add_recv_func(target_desc, r_1 + 1025, -1, -1, recv_ttyp) )
#endif /* DSNET_COMPILING_D */
      ds_exit(135);
  }
  if ( !f_nostty && !ds_add_recv_func(target_desc, 1043, -1, -1, recv_ttyp) )
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I || DSNET_COMPILING_D */
#ifdef DSNET_COMPILING_M
  if ( !ds_add_recv_func(target_desc, 1040, -1, -1, recv_ttyp) )
#endif /* DSNET_COMPILING_M */
    ds_exit(135);
  desc = ds_open_kbd(0, 0);
  if ( !desc )
    ds_exit(131);
  if ( !ds_add_recv_func(desc, 1040, -1, -1, recv_ttyp) )
    ds_exit(135);
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I)
  if ( send_netmp_connect_request() < 0 )
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I */
#ifdef DSNET_COMPILING_M
  if ( ds_send_netmp1(target_desc, 0, 0, 128, 1040) < 0 )
#endif /* DSNET_COMPILING_M */
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I) || defined(DSNET_COMPILING_M)
    ds_exit(135);
  dsm_waiting = 1;
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I || DSNET_COMPILING_M */
#ifdef DSNET_COMPILING_D
  start_cons();
#endif /* DSNET_COMPILING_D */
  while ( 1 )
  {
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I) || defined(DSNET_COMPILING_D)
    r_2 = ds_select_desc(to_sec, to_usec);
    if ( r_2 < 0 )
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I || DSNET_COMPILING_D */
#ifdef DSNET_COMPILING_M
    r = ds_select_desc(to_sec, to_usec);
    if ( r < 0 )
#endif /* DSNET_COMPILING_M */
      return ds_exit(0);
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I) || defined(DSNET_COMPILING_D)
    if ( !r_2 )
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I || DSNET_COMPILING_D */
#ifdef DSNET_COMPILING_M
    if ( !r )
#endif /* DSNET_COMPILING_M */
    {
      if ( dsm_waiting == 5 )
      {
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I)
LABEL_101:
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I */
#ifdef DSNET_COMPILING_D
LABEL_86:
#endif /* DSNET_COMPILING_D */
#ifdef DSNET_COMPILING_M
LABEL_81:
#endif /* DSNET_COMPILING_M */
        ds_exit(146);
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I)
LABEL_102:
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I */
#ifdef DSNET_COMPILING_D
LABEL_87:
#endif /* DSNET_COMPILING_D */
#ifdef DSNET_COMPILING_M
LABEL_82:
#endif /* DSNET_COMPILING_M */
        ds_exit(147);
      }
      else if ( dsm_waiting > 5 )
      {
        if ( dsm_waiting == 11 )
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I)
          goto LABEL_102;
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I */
#ifdef DSNET_COMPILING_D
          goto LABEL_87;
#endif /* DSNET_COMPILING_D */
#ifdef DSNET_COMPILING_M
          goto LABEL_82;
#endif /* DSNET_COMPILING_M */
      }
      else if ( dsm_waiting == 1 )
      {
        ds_exit(136);
#if defined(DSNET_COMPILING_E) || defined(DSNET_COMPILING_I)
        goto LABEL_101;
#endif /* DSNET_COMPILING_E || DSNET_COMPILING_I */
#ifdef DSNET_COMPILING_D
        goto LABEL_86;
#endif /* DSNET_COMPILING_D */
#ifdef DSNET_COMPILING_M
        goto LABEL_81;
#endif /* DSNET_COMPILING_M */
      }
    }
  }
}

