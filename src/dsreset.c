#include "dsnet_prototypes.h"

char *ds_stamp_str[2] = { DSNET_STAMP_STRING, DSNET_VERSION_STRING };
char *ds_version_str = DSNET_VERSION_STRING;

static int f_ncmv = 1;
static int dsm_waiting = -1;
static int to_sec = 120;
static int to_usec = 0;
static int f_poweroff = 0;

static DS_DESC *target_desc;
static NETMP_BOOTP mb;

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
  if ( v5->code == 3 || v5->code == 13 )
  {
    ds_exit(0);
  }
  else if ( dsm_waiting == v5->code )
  {
    if ( v5->code == 1 )
    {
      if ( db->buf[9] )
        ds_exit(132);
      ds_send_netmp_default_msg(target_desc);
      dsm_waiting = 5;
    }
    else
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
      if ( f_poweroff )
      {
        dsm_waiting = 13;
        if ( ds_poweroff(target_desc) < 0 )
          ds_exit(149);
      }
      else
      {
        dsm_waiting = 3;
        if ( ds_reset(target_desc, &mb, 34) < 0 )
          ds_exit(134);
      }
    }
  }
  else if ( dsm_waiting == 11 && v5->code == 10 && db->buf[9] )
  {
    ds_exit(148);
  }
  return ds_free_buf(db);
}

static int __cdecl usage(int f_true)
{
  if ( !f_true )
    return 0;
  ds_printf("Usage: %s [<option>]... [<ebootp> [<ibootp>]]\n", ds_program_name);
  ds_printf("  <option>:\n");
  ds_printf("    -d <host>[/<port_offset>]  set hostname and port offset of manager\n");
  ds_printf("    -d /<port_offset>          set port offset of manager\n");
  ds_printf("    -d <host>[:<port>]         set hostname and port number of manager\n");
  ds_printf("    -ncmv                      do not check manager version\n");
  ds_printf("    -cmv                       check manager version\n");
  ds_printf("    -poweroff                  power off (only B3 system)\n");
  return ds_exit(129);
}

int __cdecl main(int ac, char **av)
{
  unsigned int wv; // [esp+0h] [ebp-1Ch] BYREF
  quad qv; // [esp+4h] [ebp-18h] BYREF
  int r; // [esp+14h] [ebp-8h]
  char *target_name; // [esp+18h] [ebp-4h]
  int argc; // [esp+24h] [ebp+8h]
  int argca; // [esp+24h] [ebp+8h]
  char **argv; // [esp+28h] [ebp+Ch]
  char **argva; // [esp+28h] [ebp+Ch]

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
    else if ( !strcmp("-poweroff", *argv) )
    {
      ++f_poweroff;
    }
    else
    {
      usage(1);
    }
    --argc;
  }
  for ( r = 0; (unsigned int)r <= 0xF; ++r )
    mb.param[r] = -1;
  if ( argc > 0 )
  {
    if ( ds_eval_quad(*argv, &qv) )
      usage(1);
    wv = qv.xa[0];
    memcpy(mb.param, &wv, 4u);
    wv = qv.xa[1];
    memcpy(&mb.param[4], &wv, 4u);
    argva = argv + 1;
    argca = argc - 1;
    if ( argca > 0 )
    {
      if ( ds_eval_quad(*argva, &qv) )
        usage(1);
      wv = qv.xa[0];
      memcpy(&mb.param[8], &wv, 4u);
      wv = qv.xa[1];
      memcpy(&mb.param[12], &wv, 4u);
      usage(argca - 1 > 0);
    }
  }
  target_desc = ds_connect_net(target_name, 0);
  if ( !target_desc )
    ds_exit(132);
  if ( !ds_add_recv_func(target_desc, 1024, -1, -1, recv_netmp) )
    ds_exit(135);
  if ( ds_send_netmp(target_desc, 0, 0, 0, 0) < 0 )
    ds_exit(135);
  dsm_waiting = 1;
  while ( 1 )
  {
    r = ds_select_desc(to_sec, to_usec);
    if ( r < 0 )
      return ds_exit(0);
    if ( !r )
    {
      switch ( dsm_waiting )
      {
        case 1:
          goto LABEL_35;
        case 3:
          ds_exit(137);
LABEL_35:
          ds_exit(136);
          goto LABEL_36;
        case 5:
LABEL_36:
          ds_exit(146);
          goto LABEL_37;
        case 11:
LABEL_37:
          ds_exit(147);
          goto LABEL_38;
        case 13:
LABEL_38:
          ds_exit(150);
          break;
        default:
          continue;
      }
    }
  }
}

