#include "dsnet_prototypes.h"

char *ds_stamp_str[2] = { DSNET_STAMP_STRING, DSNET_VERSION_STRING };
char *ds_version_str = DSNET_VERSION_STRING;

static int escape = 29;
static int f_ncmv = 1;
static int dsm_waiting = -1;
static int to_sec = 120;
static int to_usec = 0;

static DS_DESC *target_desc;

static DSP_BUF *__cdecl recv_ttyp(DS_DESC *desc, DSP_BUF *db);
static void __cdecl start_cons();
static DSP_BUF *__cdecl recv_netmp(DS_DESC *desc, DSP_BUF *db);
static int __cdecl usage(int f_true);

static DSP_BUF *__cdecl recv_ttyp(DS_DESC *desc, DSP_BUF *db)
{
  int len; // [esp+0h] [ebp-10h]
  int i; // [esp+4h] [ebp-Ch]
  int i_1; // [esp+4h] [ebp-Ch]
  DSP_BUF **v6; // [esp+8h] [ebp-8h]

  v6 = (DSP_BUF **)&db->buf[12];
  if ( !db )
    ds_exit(130);
  len = *(unsigned __int16 *)db->buf - 12;
  if ( desc->type == 1 )
  {
    for ( i = 0; len > i; ++i )
    {
      if ( escape == *((unsigned __int8 *)v6 + i) )
      {
        ds_printf("\n");
        ds_exit(0);
      }
    }
    if ( dsm_waiting >= 0 )
    {
      return db;
    }
    else
    {
      db->buf[7] = 73;
      *(_WORD *)&db->buf[4] = 1042;
      return ds_send_desc(target_desc, db);
    }
  }
  else
  {
    if ( !*(_DWORD *)&db->buf[8] )
    {
      for ( i_1 = 0; len > i_1; ++i_1 )
        ds_printf("%c", *((unsigned __int8 *)v6 + i_1));
    }
    return ds_free_buf(db);
  }
}

static void __cdecl start_cons()
{
  DS_DESC *desc; // [esp+0h] [ebp-4h]

  desc = ds_open_kbd(0, 0);
  if ( !desc )
    ds_exit(131);
  if ( !ds_add_recv_func(desc, 1040, -1, -1, recv_ttyp) )
    ds_exit(135);
  dsm_waiting = -1;
  to_usec = -1;
  to_sec = -1;
  ds_printf("target=%<SN>", 73);
  ds_printf(" display=%<SP>", 1042);
  ds_printf(" keyboard=%<SP>", 1042);
  ds_printf(" escape=%C", escape);
  ds_printf("\n");
}

static DSP_BUF *__cdecl recv_netmp(DS_DESC *desc, DSP_BUF *db)
{
  int v2; // eax
  int n; // [esp+0h] [ebp-Ch]
  NETMP_HDR *mh; // [esp+4h] [ebp-8h]

  mh = (NETMP_HDR *)&db->buf[8];
  if ( !db )
    ds_exit(130);
  if ( dsm_waiting != mh->code )
  {
    if ( dsm_waiting == 11 && mh->code == 10 && db->buf[9] )
      ds_exit(148);
    return ds_free_buf(db);
  }
  switch ( mh->code )
  {
    case 1u:
      if ( db->buf[9] )
        ds_exit(132);
      ds_send_netmp_default_msg(target_desc);
      dsm_waiting = 5;
      return ds_free_buf(db);
    case 5u:
      if ( !f_ncmv )
      {
        v2 = strlen(ds_version_str);
        ds_send_netmp(target_desc, 10, 0, ds_version_str, v2);
        dsm_waiting = 11;
        return ds_free_buf(db);
      }
LABEL_23:
      ds_send_netmp(target_desc, 14, 0, 0, 0);
      dsm_waiting = 15;
      return ds_free_buf(db);
    case 0xBu:
      n = *(unsigned __int16 *)db->buf - 10;
      if ( db->buf[9] )
        ds_exit(148);
      if ( n != strlen(ds_version_str) )
        ds_exit_invalid_version(&db->buf[10], n, ds_version_str);
      if ( ds_strncmp(ds_version_str, &db->buf[10], n) )
        ds_exit_invalid_version(&db->buf[10], n, ds_version_str);
      goto LABEL_23;
    case 0xFu:
      if ( db->buf[9] )
        ds_exit(131);
      start_cons();
      break;
  }
  return ds_free_buf(db);
}

static int __cdecl usage(int f_true)
{
  if ( !f_true )
    return 0;
  ds_printf("Usage: %s [<option>]... [<device>]", ds_program_name);
  ds_printf("  <option>:\n");
  ds_printf("    -d <host>[/<port_offset>]  set hostname and port offset of manager\n");
  ds_printf("    -d /<port_offset>          set port offset of manager\n");
  ds_printf("    -d <host>[:<port>]         set hostname and port number of manager\n");
  ds_printf("    -ncmv                      do not check manager version\n");
  ds_printf("    -cmv                       check manager version\n");
  ds_printf("   -e <escape>                 specify escape character\n");
  return ds_exit(129);
}

int __cdecl main(int ac, char **av)
{
  int v2; // eax
  int r; // [esp+0h] [ebp-Ch]
  char *device_name; // [esp+4h] [ebp-8h]
  char *target_name; // [esp+8h] [ebp-4h]
  int argc; // [esp+14h] [ebp+8h]
  char **argv; // [esp+18h] [ebp+Ch]

  target_name = 0;
  device_name = "/dev/mrp0c";
  ds_program_name = ds_basename(*av);
  ds_printf("%s (%s)\n", ds_program_name, ds_stamp_str[0]);
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
    else if ( !strcmp("-e", *argv) )
    {
      usage(--argc <= 0);
      v2 = ds_scan_esc(*++argv, &escape);
      usage(v2);
    }
    else
    {
      usage(1);
    }
    --argc;
  }
  if ( argc > 0 )
  {
    usage(target_name != 0);
    --argc;
    device_name = *argv;
  }
  usage(argc > 0);
  if ( !target_name )
  {
    ds_comp_main(device_name, escape);
    return ds_exit(0);
  }
  target_desc = ds_connect_net(target_name, 0);
  if ( !target_desc )
    ds_exit(132);
  if ( !ds_add_recv_func(target_desc, 1024, -1, -1, recv_netmp) )
    ds_exit(135);
  if ( !ds_add_recv_func(target_desc, 1042, -1, -1, recv_ttyp) )
    ds_exit(135);
  if ( ds_send_netmp1(target_desc, 0, 0, 128, 1042) < 0 )
    ds_exit(135);
  dsm_waiting = 1;
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
        if ( dsm_waiting == 11 )
          goto LABEL_38;
      }
      else if ( dsm_waiting == 1 )
      {
        ds_exit(136);
        goto LABEL_37;
      }
    }
  }
}

