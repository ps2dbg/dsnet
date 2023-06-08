#include "dsnet_prototypes.h"

char *ds_stamp_str[2] = { DSNET_STAMP_STRING, DSNET_VERSION_STRING };
char *ds_version_str = DSNET_VERSION_STRING;

static int f_server = 1;
static int f_ncmv = 1;
static int dsm_waiting = -1;
static int to_sec = 120;
static int to_usec = 0;
static int child_status = -1;
static int idle_sec = 1;
static char *cmds = NULL;

static DS_DESC *target_desc;
static DS_OPTION *opt_iopconf;
static DS_OPTION *opt_iopmodules;

static void __cdecl drfp_error(DSP_BUF *db);
static DSP_BUF *__cdecl recv_netmp(DS_DESC *desc, DSP_BUF *db);
static int __cdecl usage(int f_true);

static void __cdecl drfp_error(DSP_BUF *db)
{
  if ( target_desc )
    ds_send_desc(target_desc, db);
  else
    ds_free_buf(db);
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
    ds_printf("Restarting file service\n");
    ds_recv_drfp(desc, 0);
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
      dsm_waiting = -1;
      to_usec = -1;
      to_sec = -1;
      ds_printf("Starting file service");
      if ( cmds )
      {
        ds_printf(" with command mode (idle=%d [sec])\n", idle_sec);
        ds_cmd_execution_for_filesv(cmds, &child_status);
        to_sec = idle_sec;
        to_usec = 0;
      }
      else
      {
        ds_printf("\n");
        if ( f_server )
          ds_fork();
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
  ds_printf("Usage: %s [<option>]...\n", ds_program_name);
  ds_printf("  <option>:\n");
  ds_printf("    -d <host>[/<port_offset>]  set hostname and port offset of manager\n");
  ds_printf("    -d /<port_offset>          set port offset of manager\n");
  ds_printf("    -d <host>[:<port>]         set hostname and port number of manager\n");
  ds_printf("    -ncmv                      do not check manager version\n");
  ds_printf("    -cmv                       check manager version\n");
  ds_printf("    -i                         interactive mode\n");
  ds_printf("    -c \"<cmds>\"                command mode (<cmds> is shell command)\n");
  ds_printf("    -t <sec>                   set max idle time for command mode\n");
  ds_printf("    -iopconf <IOPCONF>         set IOPCONF variable\n");
  ds_printf("    -iopmodules <IOPMODULES>   set IOPMODULES variable\n");
  return ds_exit(129);
}

int __cdecl main(int ac, char **av)
{
  int v2; // eax
  int r; // [esp+0h] [ebp-10h]
  char *iopmodules; // [esp+4h] [ebp-Ch]
  char *iopconf; // [esp+8h] [ebp-8h]
  char *target_name; // [esp+Ch] [ebp-4h]
  int argc; // [esp+18h] [ebp+8h]
  char **argv; // [esp+1Ch] [ebp+Ch]

  iopconf = 0;
  iopmodules = 0;
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
    else if ( !strcmp("-i", *argv) )
    {
      f_server = 0;
    }
    else if ( !strcmp("-c", *argv) )
    {
      usage(--argc <= 0);
      cmds = *++argv;
    }
    else if ( !strcmp("-t", *argv) )
    {
      usage(--argc <= 0);
      v2 = ds_scan_digit_word(*++argv, (unsigned int *)&idle_sec);
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
  usage(argc > 0);
  if ( !iopconf )
    iopconf = "/usr/local/sce/iop/modules";
  if ( !iopmodules )
    iopmodules = "/usr/local/sce/iop/modules";
  ds_printf("IOPCONF=\"%S\", IOPMODULES=\"%S\"\n", (const wchar_t *)iopconf, (const wchar_t *)iopmodules);
  opt_iopconf = ds_set_option("IOPCONF", 3, iopconf, 0, 1);
  opt_iopmodules = ds_set_option("IOPMODULES", 3, iopmodules, 0, 1);
  target_desc = ds_connect_net(target_name, 0);
  if ( !target_desc )
    ds_exit(132);
  if ( !ds_add_recv_func(target_desc, 1024, -1, -1, recv_netmp) )
    ds_exit(135);
#ifdef DSNET_COMPILING_E
  if ( !ds_add_recv_func(target_desc, 288, -1, -1, ds_recv_drfp) )
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  if ( !ds_add_recv_func(target_desc, 289, -1, -1, ds_recv_drfp) )
#endif /* DSNET_COMPILING_I */
    ds_exit(135);
#ifdef DSNET_COMPILING_E
  if ( ds_send_netmp1(target_desc, 0, 0, 240, 288) < 0 )
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  if ( ds_send_netmp1(target_desc, 0, 0, 240, 289) < 0 )
#endif /* DSNET_COMPILING_I */
    ds_exit(135);
  dsm_waiting = 1;
  ds_drfp_err_func = (void (__cdecl *)())drfp_error;
  while ( 1 )
  {
    r = ds_select_desc(to_sec, to_usec);
    if ( r < 0 )
      return ds_exit(0);
    if ( r )
      goto LABEL_47;
    if ( dsm_waiting == 5 )
      goto LABEL_45;
    if ( dsm_waiting > 5 )
    {
      if ( dsm_waiting == 11 )
        goto LABEL_46;
    }
    else if ( dsm_waiting == 1 )
    {
      ds_exit(136);
LABEL_45:
      ds_exit(146);
LABEL_46:
      ds_exit(147);
    }
LABEL_47:
    if ( cmds && ds_drfp_opened_files <= 0 && child_status >= 0 && !r )
    {
      if ( child_status >= 0 && child_status <= 127 )
        ds_exit(child_status);
      ds_exit(255);
    }
  }
}

