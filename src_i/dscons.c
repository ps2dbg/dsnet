#include "dsnet_prototypes.h"

char *ds_stamp_str[2] = { DSNET_STAMP_STRING, DSNET_VERSION_STRING };
char *ds_version_str = DSNET_VERSION_STRING;

static int escape = 29;
static int f_ncmv = 1;
static int dsm_waiting = -1;
static int to_sec = 120;
static int to_usec = 0;
static int bitmap = 33791;
static int f_nokbd = 0;
static int f_nostty = 0;
static int kbd_node = 73;
static int kbd_proto = 272;

static DS_DESC *target_desc;

static DSP_BUF *__cdecl recv_ttyp(DS_DESC *desc, DSP_BUF *db);
static void __cdecl start_cons();
static DSP_BUF *__cdecl recv_netmp(DS_DESC *desc, DSP_BUF *db);
static int __cdecl send_netmp_connect_request();
static int __cdecl usage(int f_true);

static DSP_BUF *__cdecl recv_ttyp(DS_DESC *desc, DSP_BUF *db)
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

static void __cdecl start_cons()
{
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
  dsm_waiting = -1;
  to_usec = -1;
  to_sec = -1;
  ds_printf("target=%<SN>", kbd_node);
  ds_printf(" display=");
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
    ds_printf("I");
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
    ds_printf("%sE", v4);
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
  if ( f_nokbd )
    ds_printf(" no-keyboard");
  else
    ds_printf(" keyboard=%<SP>", kbd_proto);
  ds_printf(" escape=%C", escape);
  ds_printf("\n");
}

static DSP_BUF *__cdecl recv_netmp(DS_DESC *desc, DSP_BUF *db)
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

static int __cdecl send_netmp_connect_request()
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
        p->proto = i + 512;
      else
        p->proto = i + 272;
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

static int __cdecl usage(int f_true)
{
  if ( !f_true )
    return 0;
  ds_printf("Usage: %s [<option>]... [ei0-9k]...\n", ds_program_name);
  ds_printf("  <option>:\n");
  ds_printf("    -d <host>[/<port_offset>]  set hostname and port offset of manager\n");
  ds_printf("    -d /<port_offset>          set port offset of manager\n");
  ds_printf("    -d <host>[:<port>]         set hostname and port number of manager\n");
  ds_printf("    -ncmv                      do not check manager version\n");
  ds_printf("    -cmv                       check manager version\n");
  ds_printf("    -ns                        do not display STTYP\n");
  ds_printf("    -e <escape>                set escape character\n");
  ds_printf("    -nokbd                     do not send input data to target\n");
  return ds_exit(129);
}

int __cdecl main(int ac, char **av)
{
  int v2; // eax
  int v4; // [esp+4h] [ebp-18h]
  int r; // [esp+8h] [ebp-14h]
  int r_1; // [esp+8h] [ebp-14h]
  int r_2; // [esp+8h] [ebp-14h]
  char d; // [esp+Eh] [ebp-Eh]
  char c; // [esp+Fh] [ebp-Dh]
  char c_1; // [esp+Fh] [ebp-Dh]
  char c_2; // [esp+Fh] [ebp-Dh]
  char *p; // [esp+10h] [ebp-Ch]
  char *target_name; // [esp+14h] [ebp-8h]
  DS_DESC *desc; // [esp+18h] [ebp-4h]
  int argc; // [esp+24h] [ebp+8h]
  char **argv; // [esp+28h] [ebp+Ch]

  v4 = 1;
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
    else if ( !strcmp("-ncmv", *argv) )
    {
      f_ncmv = 1;
    }
    else if ( !strcmp("-cmv", *argv) )
    {
      f_ncmv = 0;
    }
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
        if ( c == 105 || c == 73 || c == 101 || c == 69 )
        {
          v4 = 1;
          if ( c == 101 || c == 69 )
            v4 = 0x10000;
        }
        else if ( c <= 47 || c > 57 )
        {
          if ( c == 107 || c == 75 )
          {
            if ( !bitmap && !f_nokbd )
            {
              if ( v4 == 1 )
              {
                kbd_node = 73;
                kbd_proto = 287;
              }
              else
              {
                kbd_node = 69;
                kbd_proto = 543;
              }
            }
            bitmap |= v4 << 15;
          }
          else
          {
            usage(1);
          }
        }
        else
        {
          if ( !bitmap && !f_nokbd )
          {
            if ( v4 == 1 )
            {
              kbd_node = 73;
              kbd_proto = c + 224;
            }
            else
            {
              kbd_node = 69;
              kbd_proto = c + 480;
            }
          }
          bitmap |= v4 << (c - 48);
        }
        if ( p[1] == 45 )
        {
          d = p[2];
          if ( d <= 47 || d > 57 )
          {
            if ( d == 107 || d == 75 )
            {
              for ( c_2 = c + 1; c_2 <= 57; ++c_2 )
                bitmap |= v4 << (c_2 - 48);
              bitmap |= v4 << 15;
            }
            else
            {
              usage(1);
            }
          }
          else
          {
            for ( c_1 = c + 1; d >= c_1; ++c_1 )
              bitmap |= v4 << (c_1 - 48);
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
  if ( !ds_add_recv_func(target_desc, 1024, -1, -1, recv_netmp) )
    ds_exit(135);
  for ( r = 0; r <= 15; ++r )
  {
    if ( ((bitmap >> r) & 1) != 0 && !ds_add_recv_func(target_desc, r + 272, -1, -1, recv_ttyp) )
      ds_exit(135);
  }
  for ( r_1 = 16; r_1 <= 31; ++r_1 )
  {
    if ( ((bitmap >> r_1) & 1) != 0 && !ds_add_recv_func(target_desc, r_1 + 512, -1, -1, recv_ttyp) )
      ds_exit(135);
  }
  if ( !f_nostty && !ds_add_recv_func(target_desc, 1043, -1, -1, recv_ttyp) )
    ds_exit(135);
  desc = ds_open_kbd(0, 0);
  if ( !desc )
    ds_exit(131);
  if ( !ds_add_recv_func(desc, 1040, -1, -1, recv_ttyp) )
    ds_exit(135);
  if ( send_netmp_connect_request() < 0 )
    ds_exit(135);
  dsm_waiting = 1;
  while ( 1 )
  {
    r_2 = ds_select_desc(to_sec, to_usec);
    if ( r_2 < 0 )
      return ds_exit(0);
    if ( !r_2 )
    {
      if ( dsm_waiting == 5 )
      {
LABEL_101:
        ds_exit(146);
LABEL_102:
        ds_exit(147);
      }
      else if ( dsm_waiting > 5 )
      {
        if ( dsm_waiting == 11 )
          goto LABEL_102;
      }
      else if ( dsm_waiting == 1 )
      {
        ds_exit(136);
        goto LABEL_101;
      }
    }
  }
}

