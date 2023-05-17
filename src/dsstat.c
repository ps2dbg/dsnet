#include "dsnet_prototypes.h"

char *ds_stamp_str[2] = { "Version " DSNET_VERSION_STRING " Wed Jun  4 13:15:57 JST 2003", DSNET_VERSION_STRING };
char *ds_version_str = DSNET_VERSION_STRING;

static int f_ncmv = 1;
static int dsm_waiting = -1;
static int to_sec = 120;
static int to_usec = 0;

static DS_DESC *target_desc;

static void __cdecl show_status(NETMP_STATUS_DATA *status, int len);
static DSP_BUF *__cdecl recv_netmp(DS_DESC *desc, DSP_BUF *db);
static int __cdecl usage(int f_true);

static void __cdecl show_status(NETMP_STATUS_DATA *status, int len)
{
  size_t v2; // eax
  signed int connect_time; // [esp+10h] [ebp-1010h]
  int v4; // [esp+14h] [ebp-100Ch]
  int n; // [esp+18h] [ebp-1008h]
  int n_1; // [esp+18h] [ebp-1008h]
  NETMP_STATUS_DATA *v7; // [esp+1Ch] [ebp-1004h]
  char tim[1024]; // [esp+20h] [ebp-1000h] BYREF
  char day[1024]; // [esp+420h] [ebp-C00h] BYREF
  char pri[1024]; // [esp+820h] [ebp-800h] BYREF
  char proto[1024]; // [esp+C20h] [ebp-400h] BYREF

  ds_printf("# Protocol  Prio         Time  User@Host,Prog\n");
  while ( len > 0 )
  {
    ds_sprintf(proto, "%<SP>", status->protos.proto);
    ds_sprintf(pri, "0x%02x", status->protos.pri);
    connect_time = status->connect_time;
    v4 = connect_time / 86400;
    if ( connect_time / 86400 <= 0 )
      ds_sprintf(day, "    ", v4);
    else
      ds_sprintf(day, "%3d+", v4);
    ds_sprintf(tim, "%02d:%02d:%02d", connect_time / 3600 % 24, connect_time / 60 % 60, connect_time % 60);
    v7 = status + 1;
    v2 = strlen((const char *)&status[1]);
    n = v2 + 1;
    if ( (((_BYTE)v2 + 1) & 3) != 0 )
      n = v2 + 5 - (((_BYTE)v2 + 1) & 3);
    n_1 = n + 8;
    status = (NETMP_STATUS_DATA *)((char *)status + n_1);
    ds_printf("  %8s  %4s %s%s  %s\n", proto, pri, day, tim, (const char *)v7);
    len -= n_1;
  }
}

static DSP_BUF *__cdecl recv_netmp(DS_DESC *desc, DSP_BUF *db)
{
  int v2; // eax
  int n; // [esp+0h] [ebp-Ch]
  NETMP_HDR *v5; // [esp+4h] [ebp-8h]
  DECI2_HDR *dh; // [esp+8h] [ebp-4h]

  dh = (DECI2_HDR *)db->buf;
  v5 = (NETMP_HDR *)&db->buf[8];
  if ( !db )
    ds_exit(130);
  if ( v5->code == 3 )
  {
    ds_exit(145);
    return ds_free_buf(db);
  }
  if ( dsm_waiting != v5->code )
  {
    if ( dsm_waiting == 11 && v5->code == 10 && db->buf[9] )
      ds_exit(148);
    return ds_free_buf(db);
  }
  switch ( v5->code )
  {
    case 5u:
      if ( !f_ncmv )
      {
        v2 = strlen((const char *)ds_version_str);
        ds_send_netmp(target_desc, 10, 0, ds_version_str, v2);
        dsm_waiting = 11;
        return ds_free_buf(db);
      }
      goto LABEL_21;
    case 0xBu:
      n = dh->length - 10;
      if ( db->buf[9] )
        ds_exit(148);
      if ( n != strlen((const char *)ds_version_str) )
        ds_exit_invalid_version(&db->buf[10], n, (char *)ds_version_str);
      if ( ds_strncmp((char *)ds_version_str, &db->buf[10], n) )
        ds_exit_invalid_version(&db->buf[10], n, (char *)ds_version_str);
LABEL_21:
      if ( ds_send_netmp(target_desc, 6, 0, 0, 0) < 0 )
        ds_exit(135);
      dsm_waiting = 7;
      return ds_free_buf(db);
    case 7u:
      if ( db->buf[9] )
        ds_exit(143);
      show_status((NETMP_STATUS_DATA *)&db->buf[12], dh->length - 12);
      ds_exit(0);
      break;
  }
  return ds_free_buf(db);
}

static int __cdecl usage(int f_true)
{
  if ( !f_true )
    return 0;
  ds_printf("Usage: %s [<option>]...\n", ds_program_name);
  ds_printf("  <option>:\n");
  ds_printf("    -d <host>[/<port_offset>]  set hostname and port offset of manager\n");
  ds_printf("    -d /<port_offset>          set port offset of manager\n");
  ds_printf("    -d <host>[:<port>]         set hostname and port number of manager\n");
  ds_printf("    -ncmv                      do not check manager version\n");
  ds_printf("    -cmv                       check manager version\n");
  return ds_exit(129);
}

int __cdecl main(int ac, char **av)
{
  int r; // [esp+0h] [ebp-8h]
  char *target_name; // [esp+4h] [ebp-4h]
  int argc; // [esp+10h] [ebp+8h]
  char **argv; // [esp+14h] [ebp+Ch]

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
    else if ( !strcmp("-ncmv", *argv) )
    {
      f_ncmv = 1;
    }
    else if ( !strcmp("-cmv", *argv) )
    {
      f_ncmv = 0;
    }
    else
    {
      usage(1);
    }
    --argc;
  }
  if ( argc )
    usage(1);
  target_desc = ds_connect_net(target_name, 0);
  if ( !target_desc )
    ds_exit(132);
  if ( !ds_add_recv_func(target_desc, 1024, -1, -1, recv_netmp) )
    ds_exit(135);
  if ( f_ncmv )
  {
    if ( ds_send_netmp(target_desc, 6, 0, 0, 0) < 0 )
      ds_exit(135);
    dsm_waiting = 7;
  }
  else
  {
    if ( ds_send_netmp_default_msg(target_desc) < 0 )
      ds_exit(135);
    dsm_waiting = 5;
  }
  while ( 1 )
  {
    r = ds_select_desc(to_sec, to_usec);
    if ( r < 0 )
      return ds_exit(0);
    if ( !r )
    {
      if ( dsm_waiting == 5 )
      {
LABEL_37:
        ds_exit(146);
LABEL_38:
        ds_exit(147);
      }
      else if ( dsm_waiting > 5 )
      {
        if ( dsm_waiting == 7 )
        {
          ds_exit(141);
LABEL_36:
          ds_exit(136);
          goto LABEL_37;
        }
        if ( dsm_waiting == 11 )
          goto LABEL_38;
      }
      else if ( dsm_waiting == 1 )
      {
        goto LABEL_36;
      }
    }
  }
}

