#include "dsnet_prototypes.h"

char *ds_stamp_str[2] = { DSNET_STAMP_STRING, DSNET_VERSION_STRING };
char *ds_version_str = DSNET_VERSION_STRING;

static char *device_name = "/dev/mrp0";
static DS_DESC *dev_desc = NULL;
static DS_DESC *iop_desc = NULL;
static DS_DESC *ee_desc = NULL;
static DS_DESC *comport_desc = NULL;
static DS_DESC *netdev_desc = NULL;
static int f_server = 1;
static int iconnect_status_sent = 0;
static int econnect_status_sent = 0;
static struct {ST_PROTO *head;ST_PROTO *tail;} st_proto_list = { NULL, NULL };

static DS_OPTION *opt_nci;
static DS_OPTION *opt_nce;
static DS_OPTION *opt_psnet;
static DS_OPTION *opt_iopconf;
static DS_OPTION *opt_iopmodules;
static DS_OPTION *opt_netdev;
static unsigned int param[4];

static void __cdecl drfp_error(DSP_BUF *db);
static void __cdecl clear_st_proto(int nid);
static void __cdecl add_st_proto(DS_DESC *desc, int proto, int nid);
static void __cdecl send_st_proto(int proto);
static void __cdecl tty_flush_check(DSP_BUF *db);
static DSP_BUF *__cdecl send_net(DS_DESC *desc, DSP_BUF *db, int proto);
static DSP_BUF *__cdecl recv_dcmp_connect(DS_DESC *desc, DSP_BUF *db);
static DSP_BUF *__cdecl recv_dcmp_error(DS_DESC *desc, DSP_BUF *db);
static DSP_BUF *__cdecl recv_target(DS_DESC *desc, DSP_BUF *db);
static DSP_BUF *__cdecl recv_netmp(DS_DESC *desc, DSP_BUF *rdb, DECI2_HDR *dh, int len);
static DSP_BUF *__cdecl recv_client(DS_DESC *desc, DSP_BUF *db);
static int __cdecl start_client(DS_DESC *desc);
static int __cdecl usage(int f_true);
static void __cdecl connect_dev(char *device_name, int f_first);
static void __cdecl idle_func();

static void __cdecl drfp_error(DSP_BUF *db)
{
  ds_send_net(1043, db);
}

static void __cdecl clear_st_proto(int nid)
{
  ST_PROTO *q; // [esp+Ch] [ebp-Ch]
  ST_PROTO *p; // [esp+10h] [ebp-8h]

  for ( p = st_proto_list.head; p; p = q )
  {
    q = p->forw;
    if ( !(_BYTE)nid || p->nid == (_BYTE)nid )
    {
      if ( p->forw )
        p->forw->back = p->back;
      else
        st_proto_list.tail = p->back;
      if ( p->back )
        p->back->forw = p->forw;
      else
        st_proto_list.head = p->forw;
      ds_free_mem_low(p, "dsnetm.c", "clear_st_proto");
    }
  }
}

static void __cdecl add_st_proto(DS_DESC *desc, int proto, int nid)
{
  ST_PROTO *tail; // edx
  ST_PROTO *p; // [esp+Ch] [ebp-8h]
  ST_PROTO *p_1; // [esp+Ch] [ebp-8h]

  for ( p = st_proto_list.head; p; p = p->forw )
  {
    if ( p->proto == (_WORD)proto && (_BYTE)nid == p->nid )
      return;
  }
  p_1 = (ST_PROTO *)ds_alloc_mem_low("dsnetm.c", "add_st_proto", sizeof(ST_PROTO) + 1);
  if ( p_1 )
  {
    p_1->desc = desc;
    p_1->proto = proto;
    p_1->nid = nid;
    tail = st_proto_list.tail;
    p_1->back = st_proto_list.tail;
    if ( tail )
      p_1->back->forw = p_1;
    else
      st_proto_list.head = p_1;
    p_1->forw = 0;
    st_proto_list.tail = p_1;
  }
}

static void __cdecl send_st_proto(int proto)
{
  ST_PROTO *q; // [esp+Ch] [ebp-Ch]
  ST_PROTO *p; // [esp+10h] [ebp-8h]

  for ( p = st_proto_list.head; p; p = q )
  {
    q = p->forw;
    if ( p->proto == (_WORD)proto )
    {
      ds_send_dcmp_status(p->desc, p->nid, 1, (unsigned __int16)proto);
      if ( p->forw )
        p->forw->back = p->back;
      else
        st_proto_list.tail = p->back;
      if ( p->back )
        p->back->forw = p->forw;
      else
        st_proto_list.head = p->forw;
      ds_free_mem_low(p, "dsnetm.c", "send_st_proto");
    }
  }
}

static void __cdecl tty_flush_check(DSP_BUF *db)
{
  if ( *(_WORD *)db->buf == 12 && (unsigned __int8)*(_DWORD *)&db->buf[8] == 1 )
    ds_flush_desc_by_proto(*(unsigned __int16 *)&db->buf[4]);
}

static DSP_BUF *__cdecl send_net(DS_DESC *desc, DSP_BUF *db, int proto)
{
  if ( proto > 527 && proto <= 543 || proto > 271 && proto <= 287 )
    tty_flush_check(db);
  if ( !proto || ds_desc_by_proto(proto) )
    return ds_send_net(proto, db);
  add_st_proto(desc, (unsigned __int16)proto, (unsigned __int8)db->buf[6]);
  return ds_send_dcmp_error(desc, db, 1);
}

static DSP_BUF *__cdecl recv_dcmp_connect(DS_DESC *desc, DSP_BUF *db)
{
  int v3; // eax
  DCMP_CONNECT_DATA *connect_data; // [esp+0h] [ebp-Ch]

  connect_data = (DCMP_CONNECT_DATA *)&db->buf[12];
  if ( (db->buf[9] != 1 || connect_data->result != 2) && connect_data->result )
    return db;
  v3 = (unsigned __int8)db->buf[9];
  if ( v3 == 1 )
  {
LABEL_13:
    if ( db->buf[6] == 73 )
    {
      iop_desc = desc;
      if ( ++iconnect_status_sent == 1 )
        ds_send_dcmp_status_net(0, (unsigned __int8)db->buf[6]);
    }
    else if ( db->buf[6] == 69 )
    {
      ee_desc = desc;
      if ( ++econnect_status_sent == 1 )
        ds_send_dcmp_status_net(0, (unsigned __int8)db->buf[6]);
    }
    return ds_free_buf(db);
  }
  if ( (unsigned __int8)db->buf[9] <= 1u )
  {
    if ( db->buf[9] )
      return db;
    ds_send_dcmp_connect(desc, (unsigned __int8)db->buf[6], 0, 0);
    goto LABEL_13;
  }
  if ( v3 == 2 )
  {
    if ( db->buf[6] == 73 )
    {
      iop_desc = 0;
      iconnect_status_sent = 0;
      clear_st_proto(73);
    }
    else if ( db->buf[6] == 69 )
    {
      ee_desc = 0;
      econnect_status_sent = 0;
      clear_st_proto(69);
    }
  }
  else if ( v3 != 3 )
  {
    return db;
  }
  return ds_free_buf(db);
}

static DSP_BUF *__cdecl recv_dcmp_error(DS_DESC *desc, DSP_BUF *db)
{
  int proto; // [esp+0h] [ebp-14h]

  if ( *(_WORD *)db->buf != 36 )
    return db;
  if ( db->buf[18] != 72 )
    return db;
  proto = *(unsigned __int16 *)&db->buf[16];
  if ( !*(_WORD *)&db->buf[16] )
    return db;
  if ( !ds_desc_by_proto(proto) )
    return db;
  if ( db->buf[9] )
  {
    if ( db->buf[9] == 5 )
      return db;
  }
  else
  {
    db->buf[9] = 5;
  }
  return ds_send_net(proto, db);
}

static DSP_BUF *__cdecl recv_target(DS_DESC *desc, DSP_BUF *db)
{
  int v3; // eax
  int v4; // eax
  int r; // [esp+0h] [ebp-10h]
  DSP_BUF *dba; // [esp+1Ch] [ebp+Ch]

  if ( !db )
  {
    ee_desc = 0;
    iop_desc = 0;
    dev_desc = 0;
    econnect_status_sent = 0;
    iconnect_status_sent = 0;
    clear_st_proto(0);
    dba = ds_alloc_buf(1024, 72, 0, 3);
    if ( dba )
    {
      dba->buf[8] = 3;
      dba->buf[9] = 0;
      r = ds_reset_info(desc);
      if ( !ds_cmd_executing )
        ds_printf("\nReset-Info=0x%B", r);
      dba->buf[10] = r;
      ds_send_net(0, dba);
    }
    ds_recv_drfp(desc, 0);
    return 0;
  }
  v3 = *(unsigned __int16 *)&db->buf[4];
  if ( v3 == 290 )
    return ds_recv_drfp(desc, db);
  if ( *(unsigned __int16 *)&db->buf[4] > 0x122u )
  {
    if ( v3 == 1024 )
    {
      if ( (unsigned __int8)db->buf[8] <= 2u || db->buf[8] != 3 )
        return db;
      return ds_send_net(0, db);
    }
    if ( v3 == 1296 )
    {
      if ( !netdev_desc )
        return db;
      ds_send_desc(netdev_desc, db);
      return 0;
    }
    return send_net(desc, db, *(unsigned __int16 *)&db->buf[4]);
  }
  if ( v3 != 1 )
    return send_net(desc, db, *(unsigned __int16 *)&db->buf[4]);
  v4 = (unsigned __int8)db->buf[8];
  if ( v4 == 1 )
    return send_net(desc, db, *(unsigned __int16 *)&db->buf[4]);
  if ( (unsigned __int8)db->buf[8] <= 1u )
  {
    if ( !db->buf[8] )
      return recv_dcmp_connect(desc, db);
    return db;
  }
  if ( v4 != 2 )
  {
    if ( v4 == 3 )
      return recv_dcmp_error(desc, db);
    return db;
  }
  if ( db->buf[9] != 4 )
  {
    if ( !db->buf[9] )
      return db;
    return ds_send_net(0, db);
  }
  if ( !iop_desc && !opt_nci->val )
    ds_send_dcmp_connect(dev_desc, 73, 255, param);
  if ( !ee_desc && !opt_nce->val )
    ds_send_dcmp_connect(dev_desc, 69, 255, param);
  return db;
}

static DSP_BUF *__cdecl recv_netmp(DS_DESC *desc, DSP_BUF *rdb, DECI2_HDR *dh, int len)
{
  size_t v5; // eax
  int v6; // [esp-4h] [ebp-430h]
  char fname[1024]; // [esp+8h] [ebp-424h] BYREF
  DS_DESC *v8; // [esp+408h] [ebp-24h]
  char *msg; // [esp+40Ch] [ebp-20h]
  int v10; // [esp+410h] [ebp-1Ch]
  int nprotos; // [esp+414h] [ebp-18h]
  NETMP_PROTOS *protos1; // [esp+418h] [ebp-14h]
  NETMP_PROTOS *protos; // [esp+41Ch] [ebp-10h]
  NETMP_HDR *rp; // [esp+420h] [ebp-Ch]
  DSP_BUF *db; // [esp+424h] [ebp-8h]
  NETMP_HDR *nh; // [esp+428h] [ebp-4h]
  int n; // [esp+440h] [ebp+14h]

  nh = (NETMP_HDR *)&dh[1];
  n = len - 2;
  if ( n < 0 )
    return ds_send_dcmp_error(desc, rdb, 4);
  db = ds_alloc_buf(1024, 72, &dh[1], n + 2);
  if ( !db )
    return rdb;
  rp = (NETMP_HDR *)&db->buf[8];
  memcpy(&db->buf[8], nh, 2u);
  nprotos = (unsigned int)n >> 2;
  if ( !nh->code )
  {
    rp->code = 1;
    if ( desc->protos )
    {
      rp->result = 4;
    }
    else
    {
      protos = (NETMP_PROTOS *)ds_alloc_mem_low("dsnetm.c", "recv_netmp", n);
      if ( protos )
      {
        memcpy(protos, &rp[1], n);
        protos1 = protos;
        v10 = 0;
        while ( nprotos > v10 )
        {
          protos1->proto = protos1->proto;
          if ( ds_isbusy_desc(protos1->pri, protos1->proto) )
          {
            ds_free_mem_low(protos, "dsnetm.c", "recv_netmp");
            rp->result = 2;
            goto LABEL_54;
          }
          ++v10;
          ++protos1;
        }
        protos1 = protos;
        v10 = 0;
        while ( nprotos > v10 )
        {
          send_st_proto(protos1->proto);
          ++v10;
          ++protos1;
        }
        desc->protos = protos;
        desc->nprotos = nprotos;
        rp->result = 0;
        ds_disp_desc = ds_desc_by_proto(1040);
      }
      else
      {
        rp->result = 5;
      }
    }
LABEL_54:
    ds_send_desc(desc, db);
    return ds_free_buf(rdb);
  }
  if ( nh->code != 2 )
  {
    switch ( nh->code )
    {
      case 0xCu:
        rp->code = 13;
        if ( ds_poweroff(dev_desc) )
          rp->result = 1;
        else
          rp->result = 0;
        break;
      case 4u:
        rp->code = 5;
        msg = (char *)ds_alloc_mem_low("dsnetm.c", "recv_netmp", n + 1);
        if ( msg )
        {
          memcpy(msg, &rp[1], n);
          msg[n] = 0;
          ds_free_mem_low(desc->msg, "dsnetm.c", "recv_netmp");
          desc->msg = msg;
          rp->result = 0;
        }
        else
        {
          rp->result = 5;
        }
        break;
      case 6u:
        ds_free_buf(db);
        ds_send_netmp_status_reply(desc);
        return ds_free_buf(rdb);
      case 8u:
        rp->code = 9;
        v8 = ds_desc_by_proto(*(_WORD *)&rp[1]);
        if ( v8 )
        {
          ds_close_desc(v8);
          rp->result = 0;
        }
        else
        {
          rp->result = 6;
        }
        break;
      case 0xAu:
        ds_free_buf(db);
        v6 = strlen((const char *)ds_version_str);
        ds_send_netmp(desc, 11, 0, ds_version_str, v6);
        return ds_free_buf(rdb);
      case 0xEu:
        rp->code = 15;
        strcpy(fname, device_name);
        v5 = strlen(fname);
        strcpy(&fname[v5], "c");
        rp->result = 0;
        if ( comport_desc || (comport_desc = ds_open_comport(fname, recv_target)) != 0 )
        {
          desc->comport = 1;
          ++comport_desc->comport;
        }
        else
        {
          rp->result = 1;
        }
        break;
      default:
        rp->result = 1;
        break;
    }
    goto LABEL_54;
  }
  ds_reset(dev_desc, &nh[1], n);
  if ( n == 34 )
  {
    v8 = (DS_DESC *)&nh[2];
    if ( *(_DWORD *)&nh[2].code != -1 || v8->back != (struct _ds_desc *)-1 )
    {
      param[0] = (unsigned int)v8->forw;
      param[1] = (unsigned int)v8->back;
    }
    if ( v8->id != -1 || v8->type != -1 )
    {
      param[2] = v8->id;
      param[3] = v8->type;
    }
  }
  if ( !opt_nci->val )
    ds_send_dcmp_connect(dev_desc, 73, 255, param);
  if ( !opt_nce->val )
    ds_send_dcmp_connect(dev_desc, 69, 255, param);
  iop_desc = 0;
  ee_desc = 0;
  econnect_status_sent = 0;
  iconnect_status_sent = 0;
  clear_st_proto(0);
  ds_free_buf(rdb);
  return ds_free_buf(db);
}

static DSP_BUF *__cdecl recv_client(DS_DESC *desc, DSP_BUF *db)
{
  DS_DESC *v2; // eax
  int proto; // [esp+0h] [ebp-Ch]
  int len; // [esp+4h] [ebp-8h]
  DECI2_HDR *dh; // [esp+8h] [ebp-4h]

  dh = (DECI2_HDR *)db->buf;
  if ( db )
  {
    len = dh->length - 8;
    if ( len < 0 )
      return ds_send_dcmp_error(desc, db, 4);
    proto = *(unsigned __int16 *)&db->buf[4];
    if ( db->buf[7] == 73 )
    {
      if ( proto == 1042 )
      {
        if ( comport_desc )
        {
          ds_send_desc(comport_desc, db);
          return 0;
        }
      }
      else if ( iop_desc )
      {
        ds_send_desc(iop_desc, db);
        return 0;
      }
    }
    else
    {
      if ( db->buf[7] != 69 )
      {
        switch ( proto )
        {
          case 1024:
            return recv_netmp(desc, db, dh, len);
          case 1040:
            return ds_cmd_input(desc, db);
          case 1280:
            return ds_recv_pamp(desc, db);
        }
        return send_net(desc, db, proto);
      }
      if ( ee_desc )
      {
        ds_send_desc(ee_desc, db);
        return 0;
      }
    }
    return ds_send_dcmp_error(desc, db, 5);
  }
  ds_disp_desc = ds_desc_by_proto(1040);
  if ( desc->comport && comport_desc )
  {
    v2 = comport_desc;
    --comport_desc->comport;
    if ( v2->comport <= 0 )
      comport_desc = ds_close_desc(comport_desc);
  }
  return 0;
}

static int __cdecl start_client(DS_DESC *desc)
{
  if ( ds_add_recv_func(desc, -1, -1, -1, recv_client) )
    return 0;
  else
    return -1;
}

static int __cdecl usage(int f_true)
{
  if ( !f_true )
    return 0;
  ds_printf("Usage: %s [<options>]...\n", ds_program_name);
  ds_printf("  <options>:\n");
  ds_printf("    -d <device>      set device name to <device>\n");
  ds_printf("    -p <port>        set listen port to <port>\n");
  ds_printf("    -nci             do not send connect to IOP\n");
  ds_printf("    -nce             do not send connect to EE\n");
  ds_printf("    -i               interactive mode\n");
  ds_printf("    -psnet           psnet client mode\n");
  ds_printf("    -netdev <netdev> set net-device to <netdev>\n");
  return ds_exit(129);
}

static void __cdecl connect_dev(char *device_name, int f_first)
{
  if ( !ds_cmd_executing )
  {
    if ( !f_first )
      ds_printf("\n");
    ds_printf("\rConnecting to %s ...\n", device_name);
  }
  if ( !f_server )
    ds_resume_kbd();
  dev_desc = ds_connect_net(device_name, recv_target);
  if ( !dev_desc )
    ds_exit(132);
  if ( opt_psnet->val )
    dev_desc->f_psnet = 1;
  if ( !opt_nci->val && ds_send_dcmp_connect(dev_desc, 73, 255, param) < 0 )
    ds_exit(135);
  if ( !opt_nce->val && ds_send_dcmp_connect(dev_desc, 69, 255, param) < 0 )
    ds_exit(135);
  if ( !f_server )
  {
    ds_raw_kbd();
    if ( !ds_cmd_executing )
      ds_prompt();
  }
}

static void __cdecl idle_func()
{
  if ( !dev_desc )
    connect_dev(device_name, 0);
}

int __cdecl main(int ac, char **av)
{
  int type; // ecx
  int v3; // ecx
  char *port_name; // [esp+0h] [ebp-4h]
  int argc; // [esp+Ch] [ebp+8h]
  char **argv; // [esp+10h] [ebp+Ch]

  port_name = 0;
  ds_program_name = ds_basename(*av);
  ds_printf("%s (%s)\n", ds_program_name, ds_stamp_str[0]);
  opt_psnet = ds_set_option("psnet", 1, 0, 0, 1);
  opt_netdev = ds_set_option("netdev", 3, "", 0, 1);
  opt_nci = ds_set_option("nci", 1, 0, 0, 1);
  opt_nce = ds_set_option("nce", 1, 0, 0, 1);
  ds_opt_standard_init();
  opt_iopconf = ds_set_option("IOPCONF", 3, "/usr/local/sce/iop/modules", 0, 1);
  opt_iopmodules = ds_set_option("IOPMODULES", 3, "/usr/local/sce/iop/modules", 0, 1);
  ds_read_option_file();
  argc = ac - 1;
  for ( argv = av + 1; argc > 0 && **argv == 45; ++argv )
  {
    if ( !strcmp("-d", *argv) )
    {
      usage(--argc <= 0);
      device_name = *++argv;
    }
    else if ( !strcmp("-p", *argv) )
    {
      usage(--argc <= 0);
      port_name = *++argv;
    }
    else if ( !strcmp("-i", *argv) )
    {
      f_server = 0;
    }
    else if ( !strcmp("-nci", *argv) )
    {
      opt_nci->val = 1;
    }
    else if ( !strcmp("-nce", *argv) )
    {
      opt_nce->val = 1;
    }
    else if ( !strcmp("-psnet", *argv) )
    {
      opt_psnet->val = 1;
    }
    else if ( !strcmp("-netdev", *argv) )
    {
      usage(--argc <= 0);
      ds_set_option("netdev", 3, *++argv, 0, 0);
    }
    else
    {
      usage(1);
    }
    --argc;
  }
  usage(argc > 0);
  type = opt_psnet->type;
  LOBYTE(type) = type | 0x10;
  opt_psnet->type = type;
  v3 = opt_netdev->type;
  LOBYTE(v3) = v3 | 0x10;
  opt_netdev->type = v3;
  if ( opt_nci->val )
    ds_printf("disable send connect message to IOP\n");
  if ( opt_nce->val )
    ds_printf("disable send connect message to EE\n");
  if ( opt_psnet->val )
    ds_printf("enable psnet mode\n");
  if ( !ds_listen_net(port_name, start_client) )
    ds_exit(133);
  if ( !f_server && !ds_open_kbd(ds_cmd_input, 0) )
    ds_exit(131);
  ds_cmd_standard_install(f_server == 0);
  connect_dev(device_name, 1);
  if ( dev_desc )
    ds_reset_info(dev_desc);
  if ( *opt_netdev->str )
    netdev_desc = ds_open_netdev(opt_netdev->str, recv_client);
  ds_read_startup_file();
  ds_open_sam();
  if ( f_server )
  {
    ds_fork();
    ds_cmd_control("quit", 0);
    ds_cmd_xxxable_install();
  }
  ds_idle_func = idle_func;
  ds_drfp_err_func = drfp_error;
  while ( ds_select_desc(1, 0) >= 0 )
    idle_func();
  return ds_exit(0);
}

