#include "dsnet_prototypes.h"

char *ds_stamp_str[2] = { DSNET_STAMP_STRING, DSNET_VERSION_STRING };
char *ds_version_str = DSNET_VERSION_STRING;

static int f_verbose = 0;
static int f_long = 0;
static int f_ncmv = 1;
static int stamp = 241;
static int dsm_waiting = -1;
static int to_sec = 120;
static int to_usec = 0;
static int waiting_listr = 0;

static unsigned int *ids_6 = NULL;
static unsigned int *ide_7 = NULL;

static DS_DESC *target_desc;

static void __cdecl send_xload(int cmd, int action, int id, void *ptr, int len);
static void __cdecl show_info(ELOADP_HDR *lp);
static DSP_BUF *__cdecl recv_xloadp(DS_DESC *desc, DSP_BUF *db);
static DSP_BUF *__cdecl recv_dcmp(DS_DESC *desc, DSP_BUF *db);
static DSP_BUF *__cdecl recv_netmp(DS_DESC *desc, DSP_BUF *db);
static int __cdecl usage(int f_true);

static void __cdecl send_xload(int cmd, int action, int id, void *ptr, int len)
{
  DSP_BUF *db; // [esp+8h] [ebp-4h]

  db = ds_alloc_buf(592, 69, 0, len + 8);
  if ( !db )
    ds_exit(135);
  db->buf[8] = cmd;
  db->buf[9] = action;
  db->buf[10] = 0;
  db->buf[11] = stamp;
  *(_DWORD *)&db->buf[12] = id;
  if ( ptr && len > 0 )
    memcpy(&db->buf[16], ptr, len);
  if ( f_verbose )
    ds_dump("*** send", (DECI2_HDR *)db->buf, *(unsigned __int16 *)db->buf);
  ds_send_desc(target_desc, db);
}

static void __cdecl show_info(ELOADP_HDR *lp)
{
  const char *v1; // eax
  const char *v2; // eax
  const char *v3; // eax
  unsigned int v4; // [esp+0h] [ebp-34h]
  unsigned int lib_addr; // [esp+4h] [ebp-30h]
  int v6; // [esp+Ch] [ebp-28h]
  unsigned int gp_value; // [esp+10h] [ebp-24h]
  unsigned int entry_address; // [esp+14h] [ebp-20h]
  int v9; // [esp+18h] [ebp-1Ch]
  unsigned int v10; // [esp+1Ch] [ebp-18h]
  int v11; // [esp+20h] [ebp-14h]
  unsigned int module_id; // [esp+24h] [ebp-10h]
  unsigned __int16 flags; // [esp+28h] [ebp-Ch]
  __int16 v14; // [esp+2Ah] [ebp-Ah]

  v14 = *(_WORD *)&lp[1].cmd;
  flags = *(_WORD *)&lp[1].result;
  module_id = lp[1].module_id;
  v11 = *(_DWORD *)&lp[2].cmd;
  v10 = lp[2].module_id;
  v9 = *(_DWORD *)&lp[3].cmd;
  entry_address = lp[3].module_id;
  gp_value = *(_DWORD *)&lp[4].cmd;
  ds_printf(" %2x", lp->module_id);
  ds_printf(" %7x", module_id);
  ds_printf("-%7x", v9 + v10 + v11 + module_id - 1);
  ds_printf(" %5x", v9 + v10 + v11);
  ds_printf(" %5x", v11);
  ds_printf(" %5x", v10);
  ds_printf(" %5x", v9);
  ds_printf(" %2d.%-2d", HIBYTE(v14), (unsigned __int8)v14);
  ds_printf(" %S", (const wchar_t *)&lp[5] + LOBYTE(lp[4].module_id));
  ds_printf("\n");
  if ( f_long )
  {
    ds_printf("   ");
    ds_printf(" Status=%x", flags);
    ds_printf(" (");
    switch ( flags & 0xF )
    {
      case 1:
        ds_printf(" Loaded");
        break;
      case 2:
        ds_printf(" Starting");
        break;
      case 3:
        ds_printf(" Resident");
        break;
      case 4:
        ds_printf(" Stopping");
        break;
      case 5:
        ds_printf(" Self-Stopping");
        break;
      case 6:
        ds_printf(" Stopped");
        break;
      case 7:
        ds_printf(" Self-Stopped");
        break;
      default:
        ds_printf(" ???");
        break;
    }
    if ( (flags & 0x10) != 0 )
      v1 = " Removable";
    else
      v1 = "";
    ds_printf("%s", v1);
    if ( (flags & 0x20) != 0 )
      v2 = " No-Freemem-by-Unload";
    else
      v2 = "";
    ds_printf("%s", v2);
    if ( (flags & 0x40) != 0 )
      v3 = " Clearmem-by-Unload";
    else
      v3 = "";
    ds_printf("%s", v3);
    ds_printf(" )");
    if ( entry_address )
      ds_printf(" Entry=%x", entry_address);
    if ( gp_value )
      ds_printf(" GP=%x", gp_value);
    if ( BYTE1(lp[4].module_id) == 1 && LOBYTE(lp[4].module_id) > 4u )
    {
      v6 = *(_DWORD *)&lp[5].cmd;
      lib_addr = *(_DWORD *)&lp[6].cmd;
      v4 = lp[6].module_id;
      if ( v6 != -1 )
        ds_printf(" ErxStub=%x-%x", v6, lp[5].module_id + v6 - 1);
      if ( lib_addr != -1 )
        ds_printf(" ErxLib=%x-%x", lib_addr, v4 + v6 - 1);
    }
    ds_printf("\n");
  }
}

static DSP_BUF *__cdecl recv_xloadp(DS_DESC *desc, DSP_BUF *db)
{
  int len; // [esp+4h] [ebp-14h]
  int r; // [esp+8h] [ebp-10h]
  int r_1; // [esp+8h] [ebp-10h]
  unsigned int id; // [esp+Ch] [ebp-Ch]
  unsigned int id_1; // [esp+Ch] [ebp-Ch]
  ELOADP_HDR *lp; // [esp+10h] [ebp-8h]
  DECI2_HDR *dh; // [esp+14h] [ebp-4h]

  dh = (DECI2_HDR *)db->buf;
  lp = (ELOADP_HDR *)&db->buf[8];
  if ( !db )
    ds_exit(130);
  if ( f_verbose )
    ds_dump("*** recv", dh, dh->length);
  len = dh->length - 16;
  if ( stamp != (unsigned __int8)db->buf[11] )
    return db;
  if ( lp->cmd == 5 )
  {
    if ( stamp != (unsigned __int8)db->buf[11] )
      return db;
    r = (unsigned __int8)db->buf[10];
    if ( db->buf[10] )
    {
      ds_error("*** %<IR_N_LONG>", r);
      r_1 = r + 160;
      if ( r_1 <= 159 || r_1 > 223 )
        r_1 = 255;
      ds_exit(r_1);
    }
    ids_6 = (unsigned int *)&db->buf[16];
    ide_7 = (unsigned int *)&db->buf[4 * ((unsigned int)len >> 2) + 16];
    while ( ide_7 > ids_6 )
    {
      id = *ids_6++;
      if ( id )
      {
        send_xload(6, 0, id, 0, 0);
        waiting_listr = 0;
        return 0;
      }
    }
    goto LABEL_29;
  }
  if ( lp->cmd == 7 )
  {
    if ( !db->buf[10] && (unsigned int)len > 0x20 )
      show_info(lp);
    while ( ide_7 > ids_6 )
    {
      id_1 = *ids_6++;
      if ( id_1 )
      {
        send_xload(6, 0, id_1, 0, 0);
        return ds_free_buf(db);
      }
    }
LABEL_29:
    ds_exit(0);
  }
  return db;
}

static DSP_BUF *__cdecl recv_dcmp(DS_DESC *desc, DSP_BUF *db)
{
  DCMP_HDR *ch; // [esp+8h] [ebp-8h]

  ch = (DCMP_HDR *)&db->buf[8];
  if ( !db )
    ds_exit(130);
  if ( ch->type == 3 )
  {
    if ( db->buf[9] == 1 && *(_WORD *)&db->buf[16] == 592 )
      ds_exit(151);
  }
  else if ( ch->type == 2 && db->buf[9] == 1 && *(_WORD *)&db->buf[12] == 592 && waiting_listr )
  {
    send_xload(4, 0, 0, 0, 0);
  }
  return ds_free_buf(db);
}

static DSP_BUF *__cdecl recv_netmp(DS_DESC *desc, DSP_BUF *db)
{
  int v2; // eax
  int n; // [esp+0h] [ebp-Ch]
  NETMP_HDR *v5; // [esp+4h] [ebp-8h]

  v5 = (NETMP_HDR *)&db->buf[8];
  if ( !db )
    ds_exit(130);
  if ( v5->code == 3 )
  {
    ds_exit(145);
  }
  else if ( dsm_waiting == v5->code )
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
      dsm_waiting = -1;
      to_usec = -1;
      to_sec = -1;
      waiting_listr = 1;
      ds_printf(" Id   Begin     End  Size (Text  Data   Bss) Ver  Name\n");
      send_xload(4, 0, 0, 0, 0);
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

static int __cdecl usage(int f_true)
{
  if ( !f_true )
    return 0;
  ds_printf("%s (%s)\n", ds_program_name, ds_stamp_str[0]);
  ds_printf("Usage: %s [<option>]...\n", ds_program_name);
  ds_printf("  <option>:\n");
  ds_printf("    -d <host>[/<port_offset>]  set hostname and port offset of manager\n");
  ds_printf("    -d /<port_offset>          set port offset of manager\n");
  ds_printf("    -d <host>[:<port>]         set hostname and port number of manager\n");
  ds_printf("    -ncmv                      do not check manager version\n");
  ds_printf("    -cmv                       check manager version\n");
  ds_printf("    -v                         display send/recv packets\n");
  ds_printf("    -l                         long format\n");
  return ds_exit(129);
}

int __cdecl main(int ac, char **av)
{
  int r; // [esp+0h] [ebp-Ch]
  NETMP_PROTOS protos; // [esp+4h] [ebp-8h] BYREF
  char *target_name; // [esp+8h] [ebp-4h]
  int argc; // [esp+14h] [ebp+8h]
  char **argv; // [esp+18h] [ebp+Ch]

  ds_program_name = ds_basename(*av);
  target_name = ds_getenv("DSNETM");
  argc = ac - 1;
  for ( argv = av + 1; argc > 0 && **argv == 45; ++argv )
  {
    if ( !strcmp("-d", *argv) )
    {
      usage(--argc <= 0);
      target_name = *++argv;
    }
    else if ( !strcmp("-v", *argv) )
    {
      ++f_verbose;
    }
    else if ( !strcmp("-l", *argv) )
    {
      ++f_long;
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
  if ( f_verbose )
    ds_printf("%s (%s)\n", ds_program_name, ds_stamp_str[0]);
  target_desc = ds_connect_net(target_name, 0);
  if ( !target_desc )
    ds_exit(132);
  if ( !ds_add_recv_func(target_desc, 1, -1, -1, recv_dcmp) )
    ds_exit(135);
  if ( !ds_add_recv_func(target_desc, 1024, -1, -1, recv_netmp) )
    ds_exit(135);
  if ( !ds_add_recv_func(target_desc, 592, -1, -1, recv_xloadp) )
    ds_exit(135);
  protos.pri = -31;
  protos.reserved = 0;
  protos.proto = 592;
  if ( ds_send_netmp(target_desc, 0, 0, &protos, 4) < 0 )
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
LABEL_41:
        ds_exit(146);
LABEL_42:
        ds_exit(147);
      }
      else if ( dsm_waiting > 5 )
      {
        if ( dsm_waiting == 11 )
          goto LABEL_42;
      }
      else if ( dsm_waiting == 1 )
      {
        ds_exit(136);
        goto LABEL_41;
      }
    }
  }
}

