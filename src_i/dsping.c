#include "dsnet_prototypes.h"

char *ds_stamp_str[2] = { DSNET_STAMP_STRING, DSNET_VERSION_STRING };
char *ds_version_str = DSNET_VERSION_STRING;

static int f_ncmv = 1;
static int dsm_waiting = -1;
static struct {SEQ_TIME *head;SEQ_TIME *tail;} seq_time_list = { NULL, NULL };
static int pkt_count = 0;
static int pkt_size = 32;
static int pkt_timeout = 0;
static int send_count = 0;
static int recv_count = 0;
static int loss_count = 0;
static int64_t rt_min = INT64_MAX;
static int64_t rt_max = -1;
static int64_t rt_avg = 0;
static int rt_cnt = 0;

static int seq_4 = 0;

static DS_DESC *target_desc;
static unsigned __int8 *data_buf;

static void __cdecl free_seq_time(SEQ_TIME *p);
static void __cdecl send_echo();
static void __cdecl statics();
static void __cdecl check_timeout();
static DSP_BUF *__cdecl recv_echor(DSP_BUF *db, DECI2_HDR *dh, DCMP_HDR *ch);
static DSP_BUF *__cdecl recv_dcmp(DS_DESC *desc, DSP_BUF *db);
static DSP_BUF *__cdecl recv_netmp(DS_DESC *desc, DSP_BUF *db);
static DSP_BUF *__cdecl recv_kbd(DS_DESC *desc, DSP_BUF *db);
static int __cdecl usage(int f_true);

static void __cdecl free_seq_time(SEQ_TIME *p)
{
  if ( p->forw )
    p->forw->back = p->back;
  else
    seq_time_list.tail = p->back;
  if ( p->back )
    p->back->forw = p->forw;
  else
    seq_time_list.head = p->forw;
  ds_free_mem_low(p, "dsping.c", "free_seq_time");
}

static void __cdecl send_echo()
{
  SEQ_TIME *tail; // edx
  SEQ_TIME *v1; // [esp+8h] [ebp-14h]
  DSP_BUF *db; // [esp+18h] [ebp-4h]

  if ( pkt_count <= 0 || send_count < pkt_count )
  {
    v1 = (SEQ_TIME *)ds_alloc_mem_low("dsping.c", "send_echo", sizeof(SEQ_TIME));
    if ( !v1 )
      ds_exit(135);
    v1->seq = seq_4++;
    db = ds_alloc_buf(1, 73, 0, pkt_size + 8);
    if ( !db )
      ds_exit(135);
    db->buf[8] = 1;
    db->buf[9] = 0;
    *(_WORD *)&db->buf[10] = 0;
    *(_WORD *)&db->buf[12] = 73;
    *(_WORD *)&db->buf[14] = v1->seq;
    memcpy(&db->buf[16], data_buf, pkt_size);
    ds_send_desc(target_desc, db);
    ds_gettime(&v1->s, &v1->u);
    tail = seq_time_list.tail;
    v1->back = seq_time_list.tail;
    if ( tail )
      v1->back->forw = v1;
    else
      seq_time_list.head = v1;
    v1->forw = 0;
    seq_time_list.tail = v1;
    ++send_count;
  }
}

static void __cdecl statics()
{
  ds_printf("\n");
  ds_printf("%d packets transmitted,", send_count);
  ds_printf(" %d packets received,", recv_count);
  if ( send_count <= 0 )
    ds_printf(" %d%% packet loss\n", 0);
  else
    ds_printf(" %d%% packet loss\n", 100 * (send_count - recv_count) / send_count);
  if ( rt_cnt > 0 )
    rt_avg /= rt_cnt;
  if ( rt_min == INT64_MAX)
    rt_min = 0;
  if ( rt_max == -1 )
    rt_max = 0;
  ds_printf(
    "round-trip min/avg/max = %d.%03d/%d.%03d/%d.%03d ms\n",
    rt_min / 1000,
    rt_min % 1000,
    rt_avg / 1000,
    rt_avg % 1000,
    rt_max / 1000,
    rt_max % 1000);
  ds_exit(send_count > recv_count);
}

static void __cdecl check_timeout()
{
  int64_t v0; // [esp+4h] [ebp-14h]
  int64_t cu; // [esp+8h] [ebp-10h] BYREF
  int64_t psec; // [esp+Ch] [ebp-Ch] BYREF
  SEQ_TIME *q; // [esp+10h] [ebp-8h]
  SEQ_TIME *p; // [esp+14h] [ebp-4h]

  if ( pkt_timeout > 0 )
  {
    ds_gettime(&psec, &cu);
    for ( p = seq_time_list.head; p; p = q )
    {
      q = p->forw;
      v0 = psec - p->s;
      if ( cu - p->u < 0 )
        --v0;
      if ( v0 >= pkt_timeout )
      {
        free_seq_time(p);
        ++loss_count;
      }
    }
    if ( pkt_count > 0 && pkt_count <= loss_count + recv_count )
      statics();
  }
}

static DSP_BUF *__cdecl recv_echor(DSP_BUF *db, DECI2_HDR *dh, DCMP_HDR *ch)
{
  SEQ_TIME *p; // [esp+4h] [ebp-20h]
  int64_t s; // [esp+Ch] [ebp-18h]
  int64_t cu; // [esp+10h] [ebp-14h] BYREF
  int64_t cs; // [esp+14h] [ebp-10h] BYREF
  int seq; // [esp+18h] [ebp-Ch]
  int len; // [esp+1Ch] [ebp-8h]
  DCMP_ECHO_DATA *ed; // [esp+20h] [ebp-4h]

  ed = (DCMP_ECHO_DATA *)&ch[1];
  len = dh->length;
  ds_gettime(&cs, &cu);
  if ( len != pkt_size + 16 )
    return db;
  seq = ed->seq;
  for ( p = seq_time_list.head; p && p->seq != seq; p = p->forw )
    ;
  if ( p )
  {
    if ( pkt_timeout <= 0 )
      goto LABEL_12;
    s = cs - p->s;
    if ( cu - p->u < 0 )
      --s;
    if ( s < pkt_timeout )
    {
LABEL_12:
      cu += 1000000 * (cs - p->s) - p->u;
      ds_printf("%d bytes from %<SN>: seq=%d time=%d.%03d ms\n", len, dh->source, seq, cu / 1000, cu % 1000);
      rt_avg += cu;
      if ( rt_min > cu )
        rt_min = cu;
      if ( cu > rt_max )
        rt_max = cu;
      ++rt_cnt;
      free_seq_time(p);
      ++recv_count;
      if ( pkt_count > 0 && pkt_count <= loss_count + recv_count )
        statics();
    }
  }
  return ds_free_buf(db);
}

static DSP_BUF *__cdecl recv_dcmp(DS_DESC *desc, DSP_BUF *db)
{
  DCMP_HDR *ch; // [esp+0h] [ebp-8h]

  ch = (DCMP_HDR *)&db->buf[8];
  if ( !db )
    ds_exit(130);
  if ( ch->type == 1 && db->buf[9] == 1 )
    return recv_echor(db, (DECI2_HDR *)db->buf, ch);
  return db;
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

static DSP_BUF *__cdecl recv_kbd(DS_DESC *desc, DSP_BUF *db)
{
  int v3; // eax
  unsigned __int8 *bp; // [esp+0h] [ebp-Ch]
  int len; // [esp+4h] [ebp-8h]

  if ( !db )
    return 0;
  len = *(unsigned __int16 *)db->buf - 12;
  if ( len < 0 )
    return db;
  for ( bp = (unsigned __int8 *)&db->buf[12]; ; ++bp )
  {
    v3 = len--;
    if ( v3 <= 0 )
      break;
    if ( *bp == 3 || *bp == 7 )
      statics();
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
  ds_printf("    -c <count>                 ping packet counts\n");
  ds_printf("    -s <size>                  ping packet size\n");
  ds_printf("    -t <sec>                   set timeout\n");
  return ds_exit(129);
}

int __cdecl main(int ac, char **av)
{
  int v2; // eax
  int v3; // eax
  int v4; // eax
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
    else if ( !strcmp("-c", *argv) )
    {
      usage(--argc <= 0);
      v2 = ds_scan_digit_word(*++argv, (unsigned int *)&pkt_count);
      usage(v2);
    }
    else if ( !strcmp("-s", *argv) )
    {
      usage(--argc <= 0);
      v3 = ds_scan_digit_word(*++argv, (unsigned int *)&pkt_size);
      usage(v3);
    }
    else if ( !strcmp("-t", *argv) )
    {
      usage(--argc <= 0);
      v4 = ds_scan_digit_word(*++argv, (unsigned int *)&pkt_timeout);
      usage(v4);
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
  usage(argc > 0);
  if ( (unsigned int)pkt_size <= 0xF || pkt_size > 0xFFFF )
  {
    ds_printf("min size = %d, max size = %d\n", 16, 0xFFFF);
    return ds_exit(129);
  }
  pkt_size -= 16;
  target_desc = ds_connect_net(target_name, 0);
  if ( !target_desc )
    ds_exit(132);
  if ( !ds_add_recv_func(target_desc, 1024, -1, -1, recv_netmp) )
    ds_exit(135);
  if ( !ds_add_recv_func(target_desc, 1, -1, -1, recv_dcmp) )
    ds_exit(135);
  if ( ds_send_netmp1(target_desc, 0, 0, 128, 1) < 0 )
    ds_exit(135);
  dsm_waiting = 1;
  data_buf = (unsigned __int8 *)ds_alloc_mem_low("dsping.c", "main", pkt_size);
  if ( !data_buf )
    ds_exit(135);
  ds_bzero(data_buf, pkt_size);
  if ( !ds_open_kbd(recv_kbd, 0) )
    ds_exit(131);
  while ( 1 )
  {
    r = ds_select_desc(1, 0);
    if ( r < 0 )
      return ds_exit(0);
    if ( r )
    {
      if ( dsm_waiting < 0 && !send_count )
        send_echo();
    }
    else
    {
      if ( dsm_waiting == 5 )
        goto LABEL_43;
      if ( dsm_waiting > 5 )
      {
        if ( dsm_waiting == 11 )
          goto LABEL_44;
      }
      else if ( dsm_waiting == 1 )
      {
        ds_exit(136);
LABEL_43:
        ds_exit(146);
LABEL_44:
        ds_exit(147);
      }
      check_timeout();
      send_echo();
    }
  }
}

