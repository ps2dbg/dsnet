#include "dsnet_prototypes.h"

char *ds_stamp_str[2] = { "Version " DSNET_VERSION_STRING " Wed Jun  4 13:15:57 JST 2003", DSNET_VERSION_STRING };
char *ds_version_str = DSNET_VERSION_STRING;

static int f_ncmv = 29;
static int dsm_waiting = -1;
static int to_sec = 120;
static int to_usec = 0;

static DS_DESC *target_desc;
static unsigned __int16 proto;

static DSP_BUF *__cdecl recv_netmp(DS_DESC *desc, DSP_BUF *db);
static int __cdecl usage(int f_true);

static DSP_BUF *__cdecl recv_netmp(DS_DESC *desc, DSP_BUF *db)
{
  int v2; // eax
  int n; // [esp+0h] [ebp-Ch]
  NETMP_HDR *v5; // [esp+4h] [ebp-8h]

  v5 = (NETMP_HDR *)&db->buf[8];
  if ( !db )
    ds_exit(130);
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
        v2 = strlen(ds_version_str);
        ds_send_netmp(target_desc, 10, 0, ds_version_str, v2);
        dsm_waiting = 11;
        return ds_free_buf(db);
      }
      goto LABEL_19;
    case 0xBu:
      n = *(unsigned __int16 *)db->buf - 10;
      if ( db->buf[9] )
        ds_exit(148);
      if ( n != strlen(ds_version_str) )
        ds_exit_invalid_version(&db->buf[10], n, ds_version_str);
      if ( ds_strncmp(ds_version_str, &db->buf[10], n) )
        ds_exit_invalid_version(&db->buf[10], n, ds_version_str);
LABEL_19:
      if ( ds_send_netmp(target_desc, 8, 0, &proto, 2) < 0 )
        ds_exit(135);
      dsm_waiting = 9;
      return ds_free_buf(db);
    case 9u:
      if ( db->buf[9] )
        ds_exit(144);
      ds_exit(0);
      break;
  }
  return ds_free_buf(db);
}

static int __cdecl usage(int f_true)
{
  if ( !f_true )
    return 0;
  ds_printf("Usage: %s [<option>]... <proto>\n", ds_program_name);
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
  int val; // [esp+0h] [ebp-Ch] BYREF
  int r; // [esp+4h] [ebp-8h]
  char *target_name; // [esp+8h] [ebp-4h]
  int argc; // [esp+14h] [ebp+8h]
  char **argv; // [esp+18h] [ebp+Ch]

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
  if ( argc != 1 )
    usage(1);
  if ( ds_scan_proto(*argv, &val) )
    usage(1);
  proto = val;
  target_desc = ds_connect_net(target_name, 0);
  if ( !target_desc )
    ds_exit(132);
  if ( !ds_add_recv_func(target_desc, 1024, -1, -1, recv_netmp) )
    ds_exit(135);
  if ( f_ncmv )
  {
    if ( ds_send_netmp(target_desc, 8, 0, &proto, 2) < 0 )
      ds_exit(135);
    dsm_waiting = 9;
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
      if ( dsm_waiting == 9 )
      {
        ds_exit(142);
        goto LABEL_37;
      }
      if ( dsm_waiting > 9 )
      {
        if ( dsm_waiting == 11 )
          goto LABEL_38;
      }
      else if ( dsm_waiting == 5 )
      {
LABEL_37:
        ds_exit(146);
LABEL_38:
        ds_exit(147);
      }
    }
  }
}

