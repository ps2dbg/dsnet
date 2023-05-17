#include "dsnet_prototypes.h"

char *ds_stamp_str[2] = { DSNET_STAMP_STRING, DSNET_VERSION_STRING };
char *ds_version_str = DSNET_VERSION_STRING;

static int f_verbose = 0;
static int f_ncmv = 1;
static int f_nttyp = 0;
static int f_nostty = 0;
static int f_ndrfp = 0;
static int f_wait = 1;
static int stamp = 240;
static int id = 0;
static int dsm_waiting = -1;
static int to_sec = 120;
static int to_usec = 0;
static int waiting_startr = 0;
static int waiting_status = 0;
static int proto_drfp = 292;

static DS_DESC *target_desc;
static int arg_siz;
static char args[160];
static DS_OPTION *opt_iopconf;
static DS_OPTION *opt_iopmodules;

static void __cdecl drfp_error(DSP_BUF *db)
{
  if ( target_desc )
    ds_send_desc(target_desc, db);
  else
    ds_free_buf(db);
}

static void __cdecl send_xstart()
{
  DSP_BUF *db; // [esp+8h] [ebp-4h]

  db = ds_alloc_buf(336, 73, 0, arg_siz + 8);
  if ( !db )
    ds_exit(135);
  db->buf[8] = 0;
  db->buf[9] = 15;
  db->buf[10] = 0;
  db->buf[11] = stamp;
  *(_DWORD *)&db->buf[12] = 0;
  memcpy(&db->buf[16], args, arg_siz);
  if ( f_verbose )
    ds_dump("*** send", (DECI2_HDR *)db->buf, *(unsigned __int16 *)db->buf);
  ds_send_desc(target_desc, db);
}

static DSP_BUF *__cdecl recv_xloadp(DS_DESC *desc, DSP_BUF *db)
{
  int r; // [esp+0h] [ebp-Ch]
  int r_1; // [esp+0h] [ebp-Ch]
  int r_2; // [esp+0h] [ebp-Ch]
  ILOADP_HDR *lp; // [esp+4h] [ebp-8h]

  lp = (ILOADP_HDR *)&db->buf[8];
  if ( !db )
    ds_exit(130);
  if ( f_verbose )
    ds_dump("*** recv", (DECI2_HDR *)db->buf, *(unsigned __int16 *)db->buf);
  if ( lp->cmd == 1 )
  {
    if ( stamp == (unsigned __int8)db->buf[11] )
    {
      r = (unsigned __int8)db->buf[10];
      if ( db->buf[10] )
      {
        ds_error("*** %<IR_N_LONG>", r);
        r_1 = r + 160;
        if ( r_1 <= 159 || r_1 > 223 )
          r_1 = 255;
        ds_exit(r_1);
      }
      id = *(_DWORD *)&db->buf[12];
      waiting_startr = 0;
      return ds_free_buf(db);
    }
    else
    {
      return db;
    }
  }
  else if ( lp->cmd == 16 )
  {
    if ( db->buf[10] == 3 || db->buf[10] == 4 )
    {
      r_2 = *(_DWORD *)&db->buf[16];
      if ( r_2 < 0 || r_2 > 127 )
        r_2 = 255;
      ds_exit(r_2);
    }
    return ds_free_buf(db);
  }
  else
  {
    return db;
  }
}

static DSP_BUF *__cdecl recv_dstp(DS_DESC *desc, DSP_BUF *db)
{
  int len; // [esp+0h] [ebp-10h]
  int i; // [esp+4h] [ebp-Ch]

  len = *(unsigned __int16 *)db->buf - 12;
  if ( !db )
    ds_exit(130);
  if ( !*(_DWORD *)&db->buf[8] )
  {
    for ( i = 0; len > i; ++i )
      ds_printf("%c", (unsigned __int8)db->buf[i + 12]);
  }
  return ds_free_buf(db);
}

static DSP_BUF *__cdecl recv_dcmp(DS_DESC *desc, DSP_BUF *db)
{
  int v2; // eax
  int v3; // eax
  int last_status; // [esp+4h] [ebp-10h]
  DCMP_HDR *ch; // [esp+Ch] [ebp-8h]

  ch = (DCMP_HDR *)&db->buf[8];
  if ( !db )
    ds_exit(130);
  if ( ch->type == 3 )
  {
    if ( *(_WORD *)&db->buf[16] == 336 && !db->buf[20] )
    {
      v2 = (unsigned __int8)db->buf[9];
      if ( v2 == 2 )
      {
        LOBYTE(waiting_status) = waiting_status | 4;
      }
      else if ( (unsigned __int8)db->buf[9] > 2u )
      {
        if ( v2 == 3 )
        {
          LOBYTE(waiting_status) = waiting_status | 8;
        }
        else if ( v2 == 5 )
        {
          LOBYTE(waiting_status) = waiting_status | 1;
        }
      }
      else if ( v2 == 1 )
      {
        LOBYTE(waiting_status) = waiting_status | 2;
      }
    }
  }
  else
  {
    if ( ch->type != 2 )
      return ds_free_buf(db);
    last_status = waiting_status;
    v3 = (unsigned __int8)db->buf[9];
    if ( v3 == 1 )
    {
      waiting_status &= ~2u;
    }
    else if ( (unsigned __int8)db->buf[9] > 1u )
    {
      if ( v3 == 2 )
      {
        waiting_status &= ~4u;
      }
      else
      {
        if ( v3 != 3 )
          return ds_free_buf(db);
        waiting_status &= ~8u;
      }
    }
    else
    {
      if ( db->buf[9] || *(_WORD *)&db->buf[12] != 73 )
        return ds_free_buf(db);
      waiting_status &= ~1u;
    }
    if ( waiting_startr && last_status && !waiting_status )
      send_xstart();
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
      waiting_startr = 1;
      send_xstart();
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

static void __cdecl set_proto(char *prog)
{
  char c; // [esp+3h] [ebp-1h]

  if ( !ds_strncmp(prog, "host", 4) )
  {
    c = prog[4];
    if ( c > 47 && c <= 52 && prog[5] == 58 )
      proto_drfp = c + 240;
  }
}

static void __cdecl set_args(int ac, char **av)
{
  int v2; // eax
  int i; // [esp+0h] [ebp-Ch]
  int f_dev; // [esp+4h] [ebp-8h]
  char *p; // [esp+8h] [ebp-4h]
  char *p_1; // [esp+8h] [ebp-4h]

  f_dev = 0;
  for ( p = *av; *p; ++p )
  {
    if ( *p == 58 )
      ++f_dev;
  }
  p_1 = args;
  if ( !f_dev )
  {
    strcpy(args, "host4:");
    p_1 = &args[6];
  }
  for ( i = 0; ac > i; ++i )
  {
    if ( (unsigned int)(&p_1[strlen(av[i])] - ((char *)&arg_siz + 3)) > 0xA0 )
    {
      v2 = ds_error("** too long arguments");
      ds_exit(v2);
    }
    strcpy(p_1, av[i]);
    p_1 += strlen(av[i]) + 1;
  }
  arg_siz = p_1 - args;
}

static int __cdecl usage(int f_true)
{
  if ( !f_true )
    return 0;
  ds_printf("%s (%s)\n", ds_program_name, ds_stamp_str[0]);
  ds_printf("Usage: %s [<option>]... <fname> [<arg>]...]\n", ds_program_name);
  ds_printf("  <option>:\n");
  ds_printf("    -d <host>[/<port_offset>]  set hostname and port offset of manager\n");
  ds_printf("    -d /<port_offset>          set port offset of manager\n");
  ds_printf("    -d <host>[:<port>]         set hostname and port number of manager\n");
  ds_printf("    -ncmv                      do not check manager version\n");
  ds_printf("    -cmv                       check manager version\n");
  ds_printf("    -v                         display send/recv packets\n");
  ds_printf("    -nt                        do not handle I[0-9K]TTYP\n");
  ds_printf("    -ns                        do not handle STTYP\n");
  ds_printf("    -nf                        do not handle DRFP4\n");
  ds_printf("    -iopconf <IOPCONF>         set IOPCONF variable\n");
  ds_printf("    -iopmodules <IOPMODULES>   set IOPMODULES variable\n");
  ds_printf("    -w                         wait ELOADP is enabled [default]\n");
  ds_printf("    -nw                        no wait ELOADP is enabled\n");
  return ds_exit(129);
}

int __cdecl main(int ac, char **av)
{
  NETMP_PROTOS *_p; // [esp+4h] [ebp-54h]
  NETMP_PROTOS *_p_3; // [esp+4h] [ebp-54h]
  NETMP_PROTOS *_p_4; // [esp+4h] [ebp-54h]
  NETMP_PROTOS *_p_5; // [esp+4h] [ebp-54h]
  int i; // [esp+8h] [ebp-50h]
  int r; // [esp+Ch] [ebp-4Ch]
  NETMP_PROTOS *p; // [esp+10h] [ebp-48h]
  NETMP_PROTOS protos[14]; // [esp+14h] [ebp-44h] BYREF
  char *iopmodules; // [esp+4Ch] [ebp-Ch]
  char *iopconf; // [esp+50h] [ebp-8h]
  char *target_name; // [esp+54h] [ebp-4h]
  int argc; // [esp+60h] [ebp+8h]
  char **argv; // [esp+64h] [ebp+Ch]

  iopconf = 0;
  iopmodules = 0;
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
    else if ( !strcmp("-ncmv", *argv) )
    {
      f_ncmv = 1;
    }
    else if ( !strcmp("-cmv", *argv) )
    {
      f_ncmv = 0;
    }
    else if ( !strcmp("-nt", *argv) )
    {
      ++f_nttyp;
    }
    else if ( !strcmp("-ns", *argv) )
    {
      ++f_nostty;
    }
    else if ( !strcmp("-nf", *argv) )
    {
      ++f_ndrfp;
    }
    else if ( !strcmp("-w", *argv) )
    {
      f_wait = 1;
    }
    else if ( !strcmp("-nw", *argv) )
    {
      f_wait = 0;
    }
    else if ( !strcmp("-iopconf", *argv) )
    {
      usage(--argc <= 0);
      iopconf = *++argv;
    }
    else if ( !strcmp("-iopmodules", *argv) )
    {
      usage(--argc <= 0);
      iopmodules = *++argv;
    }
    else
    {
      usage(1);
    }
    --argc;
  }
  if ( !argc )
    usage(1);
  if ( !iopconf )
    iopconf = "/usr/local/sce/iop/modules";
  if ( !iopmodules )
    iopmodules = "/usr/local/sce/iop/modules";
  if ( f_verbose )
  {
    ds_printf("%s (%s)\n", ds_program_name, ds_stamp_str[0]);
    ds_printf("IOPCONF=\"%S\", IOPMODULES=\"%S\"\n", (const wchar_t *)iopconf, (const wchar_t *)iopmodules);
  }
  opt_iopconf = ds_set_option("IOPCONF", 3, iopconf, 0, 1);
  opt_iopmodules = ds_set_option("IOPMODULES", 3, iopmodules, 0, 1);
  set_proto(*argv);
  set_args(argc, argv);
  target_desc = ds_connect_net(target_name, 0);
  if ( !target_desc )
    ds_exit(132);
  if ( !ds_add_recv_func(target_desc, 1, -1, -1, recv_dcmp) )
    ds_exit(135);
  if ( !ds_add_recv_func(target_desc, 1024, -1, -1, recv_netmp) )
    ds_exit(135);
  ds_bzero(protos, sizeof(protos));
  if ( !ds_add_recv_func(target_desc, 336, -1, -1, recv_xloadp) )
    ds_exit(135);
  p = &protos[1];
  protos[0].pri = -32;
  protos[0].proto = 336;
  if ( !f_nttyp )
  {
    for ( i = 0; i <= 9; ++i )
    {
      if ( !ds_add_recv_func(target_desc, i + 272, -1, -1, recv_dstp) )
        ds_exit(135);
      _p = p++;
      _p->pri = -32;
      _p->proto = i + 272;
    }
    if ( !ds_add_recv_func(target_desc, 287, -1, -1, recv_dstp) )
      ds_exit(135);
    _p_3 = p++;
    _p_3->pri = -32;
    _p_3->proto = 287;
  }
  if ( !f_nostty )
  {
    if ( !ds_add_recv_func(target_desc, 1043, -1, -1, recv_dstp) )
      ds_exit(135);
    _p_4 = p++;
    _p_4->pri = -32;
    _p_4->proto = 1043;
  }
  if ( !f_ndrfp )
  {
    if ( !ds_add_recv_func(target_desc, proto_drfp, -1, -1, ds_recv_drfp) )
      ds_exit(135);
    _p_5 = p++;
    _p_5->pri = -32;
    _p_5->proto = proto_drfp;
    ds_drfp_err_func = (void (__cdecl *)())drfp_error;
  }
  if ( ds_send_netmp(target_desc, 0, 0, protos, (char *)p - (char *)protos) < 0 )
    ds_exit(135);
  dsm_waiting = 1;
  while ( 1 )
  {
    r = ds_select_desc(to_sec, to_usec);
    if ( r < 0 )
      return ds_exit(0);
    if ( r )
      goto LABEL_76;
    if ( dsm_waiting == 5 )
      goto LABEL_74;
    if ( dsm_waiting > 5 )
    {
      if ( dsm_waiting == 11 )
        goto LABEL_75;
    }
    else if ( dsm_waiting == 1 )
    {
      ds_exit(136);
LABEL_74:
      ds_exit(146);
LABEL_75:
      ds_exit(147);
    }
LABEL_76:
    if ( !f_wait && (waiting_status & 2) != 0 )
      ds_exit(151);
  }
}

