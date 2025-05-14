
#include "dsxdb_prototypes.h"

char *ds_stamp_str[2] = { DSNET_STAMP_STRING, DSNET_VERSION_STRING };
char *ds_version_str = DSNET_VERSION_STRING;

static int f_nx = 0;
int dsc_connected = 0;
static int dsc_proto = 0;
static int resetted = 0;
static int force_reset_on_start = 0;
static int cmd_executing = 0;
static int expected_resetr = 0;
static int no_kbd = 0;
static int no_exit = 0;
static int f_batch = 0;
static int dsm_waiting = -1;
static int to_sec = -1;
static int to_usec = -1;
static int need_getconf = 0;
DBGP_CONF_DATA dbconf =
{
  .major_ver = 3,
  .minor_ver = 0,
  .target_id = TARGET_SDBGP,
  .reserved1 = 0,
  .mem_align = 1,
  .reserved2 = 0,
  .reg_size = TARGET_REG_SIZE,
  .nreg = 1,
  .nbrkpt = 0,
  .ncont = 1,
  .nstep = 0,
  .nnext = 0,
  .mem_limit_align = 1,
  .mem_limit_size = 512,
  .run_stop_state = 2,
  .hdbg_area_addr = 0,
  .hdbg_area_size = 0,
};
static int dbconf_is_valid = 0;
static int dbconf_max_mem_align = 0;
static int cur_state = 0;
static int cur_proto = -1;
static int cur_stype = -1;
static int cur_wtype = -1;
static void *cur_pointer = NULL;
static int cur_length = 0;
static int cur_padding = 0;
static int *cur_count_pointer = NULL;
static int cur_result = 0;
static int cur_stamp = 0;

#ifdef DSNET_COMPILING_E
static int cur_cpuid = 0;
#endif /* DSNET_COMPILING_E */

static int dcmp_waiting_status = 0;
TTYQ ttyq = { 0, 0, 0, "" };
static int prompt_len = 0;
static int input_line_erased = 0;

#ifdef DSNET_COMPILING_E
static void *xgkt_stream = NULL;
static void *rdimg_stream = NULL;
#endif /* DSNET_COMPILING_E */

static int dsip_stamp = 128;
static int argc = 0;
static char **argv = NULL;

static DS_OPTION *opt_target_name;
static DS_OPTION *opt_tty_mask;
static DS_OPTION *opt_atty_mask;
static DS_OPTION *opt_file_priority;
static DS_OPTION *opt_reset_on_start;
static DS_OPTION *opt_check_manager_version;
static DS_OPTION *opt_dr_default_format;
static DS_OPTION *opt_dr_default_di;
static DS_OPTION *opt_ex_default_dr;
#ifdef DSNET_COMPILING_E
static DS_OPTION *opt_dr0_default_di;
static DS_OPTION *opt_ex0_default_dr;
static DS_OPTION *opt_dr1_default_di;
static DS_OPTION *opt_ex1_default_dr;
#endif /* DSNET_COMPILING_E */
static DS_OPTION *opt_lstep_default_list;
static DS_OPTION *opt_lstep_stop_at_no_line;
static DS_OPTION *opt_source_directories;
static DS_OPTION *opt_target_exec_ctl_config;
static DS_OPTION *opt_target_exec_ctl_override;
static DS_OPTION *opt_initial_ebootp;
static DS_OPTION *opt_current_ebootp;
static DS_OPTION *opt_initial_ibootp;
static DS_OPTION *opt_current_ibootp;
static DS_OPTION *opt_automatic_prefix_breakpoint;
static DS_OPTION *opt_describe_ub_all;
#ifdef DSNET_COMPILING_E
static DS_OPTION *opt_xgkt_flag;
#endif /* DSNET_COMPILING_E */
static DS_OPTION *opt_di_address;
static DS_OPTION *opt_di_instruction_word;
static DS_OPTION *opt_di_branch_address;
static DS_OPTION *opt_di_macro;
static DS_OPTION *opt_help_lang;
static DS_OPTION *opt_help_pager;
static DS_OPTION *opt_iopconf;
static DS_OPTION *opt_iopmodules;
static DS_DESC *target_desc;
static DS_DESC *kbd_desc;

#ifdef DSNET_COMPILING_E
static int xgkt_off;
static int xgkt_eoff;
static int xgkt_seq;
static int xgkt_glen;
static int xgkt_vbs0;
static int xgkt_vif1;
static int rdimg_seq;
static int rdimg_len;

quad regbuf_vals[11][32];
unsigned int regbuf_mask[11];
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
unsigned int regbuf_vals[10][32];
unsigned int regbuf_mask[10];
#endif /* DSNET_COMPILING_I */

static void ntoh_word_copy(unsigned int *dest, unsigned int *src, int len);
static DSP_BUF *alloc_dbgp(int id, int group, int type, int code, int result, int count, void **pp, int len);
static void print_prompt();
static void erase_input_line();
static void redraw_input_line(int f_force);
static void display_ttyq();
static DSP_BUF *recv_ttyp(DS_DESC *desc, DSP_BUF *db);
static void flush_tty_buf();
static void abort_input(int code);
static void normal_input(int code);
static DSP_BUF *recv_kbd(DS_DESC *desc, DSP_BUF *db);
#ifdef DSNET_COMPILING_E
static int setup_xgkt(char *fname, unsigned int cnt, unsigned int off);
static DSP_BUF *recv_xgkt(DSP_BUF *db);
static DSP_BUF *recv_rdimg(DSP_BUF *db);
#endif /* DSNET_COMPILING_E */
static DSP_BUF *unexpected_reply(DSP_BUF *db, DBGP_HDR *ch);
static DSP_BUF *recv_dbgp(DS_DESC *desc, DSP_BUF *db);
static void print_state_errors(char *fmt);
static int send_and_wait(DSP_BUF *db, int stype, void *ptr, int len, int wresult);
static int get_conf(DBGP_CONF_DATA *conf);
static void send_dbconf();
static int rdwr_mem_align(int code, int align, int cpuid, int space, unsigned int adr, void *ptr, int len);
static int rdwr_mem(int code, unsigned int adr, void *ptr, int len);
static int load_mem(unsigned int adr, void *ptr, int len);
static int store_mem(unsigned int adr, void *ptr, int len);
#ifdef DSNET_COMPILING_E
static int load_quad_registers_id(int id, unsigned int *masks, quad *pv, int n);
static int store_quad_registers_id(int id, unsigned int *masks, quad *pv, int n);
static int access_quad_registers(int (*func)(), unsigned int *masks, quad *pv, int n);
#endif /* DSNET_COMPILING_E */
static DSP_BUF *recv_loadp(DS_DESC *desc, DSP_BUF *db);
static int get_and_check_config();
static void print_size(unsigned int size);
static void print_align_list(unsigned int align);
static int show_dbconf(int ac, char **av);
static int reset_cmd(int ac, char **av);
#ifdef DSNET_COMPILING_E
static int dbgctl(int id, int flag);
static int dbgctl_cmd(int ac, char **av);
static int xgkt_cmd(int ac, char **av);
static int storeimage_cmd(int ac, char **av);
#endif /* DSNET_COMPILING_E */
static int bpfunc_cmd(int ac, char **av);
static DSP_BUF *recv_dcmp(DS_DESC *desc, DSP_BUF *db);
static void batch(int ac, char **av);
static DSP_BUF *recv_netmp(DS_DESC *desc, DSP_BUF *db);
static int send_netmp_connect_request();
static DSP_BUF *recv_console(DS_DESC *desc, DSP_BUF *db);
static int start_console(DS_DESC *desc);
static void drfp_error(DSP_BUF *db);
static int usage(int f_true);
static void dup_and_exe(char *cmd);
static void set_options_to_default();

static void ntoh_word_copy(unsigned int *dest, unsigned int *src, int len)
{
  int i; // [esp+0h] [ebp-4h]
  signed int lena; // [esp+14h] [ebp+10h]

  lena = (unsigned int)len >> 2;
  for ( i = 0; lena > i; ++i )
    *dest++ = *src++;
}

static DSP_BUF *alloc_dbgp(int id, int group, int type, int code, int result, int count, void **pp, int len)
{
  DSP_BUF *db; // [esp+1Ch] [ebp-Ch]

  db = ds_alloc_buf(TARGET_SDBGP, TARGET_DID, 0, len + 8);

  if ( !db )
    return 0;

  db->buf[8] = id;
  db->buf[9] = group;
  db->buf[10] = type;
  db->buf[11] = code;
  db->buf[12] = result;
  db->buf[13] = count;
  *(_WORD *)&db->buf[14] = 0;

  if ( pp )
    *pp = &db->buf[16];

  return db;
}

static void print_prompt()
{
  char *str; // [esp+0h] [ebp-4h]

  if ( !f_batch || !no_kbd )
  {
    if ( dbconf.run_stop_state == 1 )
    {
      str = "R";
    }
    else if ( dbconf.run_stop_state == 2 )
    {
      str = "S";
    }
    else
    {
      str = "?";
    }
    ds_additional_prompt_str = str;
    prompt_len = ds_prompt();
  }
}

static void erase_input_line()
{
  int n; // [esp+0h] [ebp-Ch]
  int n_1; // [esp+0h] [ebp-Ch]
  int i; // [esp+4h] [ebp-8h]
  int i_1; // [esp+4h] [ebp-8h]

  input_line_erased = 0;
  n = prompt_len;
  if ( prompt_len > 0 )
  {
    n_1 = strlen(ds_histbuf.buf) + n;
    ds_printf("\r");
    for ( i = 0; n_1 > i; ++i )
      ds_printf("%c", 32);
    for ( i_1 = 0; n_1 > i_1; ++i_1 )
      ds_printf("%c", 8);
    prompt_len = 0;
    ++input_line_erased;
  }
}

static void redraw_input_line(int f_force)
{
  char *p; // [esp+0h] [ebp-8h]

  if ( f_force || input_line_erased )
  {
    print_prompt();
    p = ds_histbuf.ptr;
    ds_histbuf.ptr = ds_histbuf.buf;
    ds_editline(&ds_histbuf, 12, 0);
    while ( p > ds_histbuf.ptr )
      ds_editline(&ds_histbuf, 6, 0);
  }
}

static void display_ttyq()
{
  int ch; // [esp+0h] [ebp-4h]

  ch = 10;
  erase_input_line();
  while ( ttyq.len > 0 )
  {
    ch = ttyq.buf[ttyq.get & 0x3FFF];
    ++ttyq.get;
    --ttyq.len;
    ds_printf("%c", ch);
  }
  if ( ch != 10 )
    ds_printf("\n");
  redraw_input_line(0);
}

static DSP_BUF *recv_ttyp(DS_DESC *desc, DSP_BUF *db)
{
  char v3; // cl
  char *bp; // [esp+8h] [ebp-14h]
  int n; // [esp+14h] [ebp-8h]

  if ( !db )
    return 0;

  if ( (cur_state & 2) == 0 || cur_proto != TARGET_SDBGP || cur_stype != 20 )
  {
    n = *(unsigned __int16 *)db->buf - 12;
    if ( !*(_DWORD *)&db->buf[8] )
    {
      bp = &db->buf[12];
      while ( &db->buf[n + 12] > bp )
      {
        if ( ttyq.len > 0x3FFF )
          display_ttyq();
        v3 = *bp;
        ttyq.buf[ttyq.put & 0x3FFF] = *bp++;
        ++ttyq.put;
        ++ttyq.len;
        if ( v3 == 10 )
          display_ttyq();
      }
    }
  }
  return ds_free_buf(db);
}

static void flush_tty_buf()
{
  unsigned int wv; // [esp+0h] [ebp-8h] BYREF
  DSP_BUF *db; // [esp+4h] [ebp-4h]

  ttyq.len = 0;
  ttyq.get = 0;
  ttyq.put = 0;
  wv = 1;
  db = ds_alloc_buf(TARGET_TTYP, 72, &wv, 4);
  if ( db )
    ds_send_desc(target_desc, db);
}

static void abort_input(int code)
{
  DSP_BUF *db; // [esp+4h] [ebp-8h]

  if ( (_BYTE)code != 3 )
    goto LABEL_7;
  if ( (opt_tty_mask->val & 1) != 0 )
    flush_tty_buf();
  if ( (cur_state & 2) != 0 && (db = alloc_dbgp(DBGP_CPUID_CPU, DBGP_GROUP_SYSTEM, DBGP_TYPE_BREAK, 0, 0, 0, 0, 0)) != 0 )
  {
    ds_send_desc(target_desc, db);
    cur_proto = TARGET_SDBGP;
    cur_stype = 20;
    ds_printf("*** Sending Break ...\n");
    LOBYTE(cur_state) = cur_state | 0x40;
  }
  else
  {
LABEL_7:
    LOBYTE(cur_state) = cur_state | 0x40;
    cur_state &= 0xFFFFFFFC;
  }
}

static void normal_input(int code)
{
  struct {
    unsigned int zero;
    unsigned char code;
  } dat; // [esp+4h] [ebp-10h] BYREF
  DSP_BUF *p; // [esp+Ch] [ebp-8h]
  unsigned __int8 v3; // [esp+13h] [ebp-1h]

  v3 = code;
  if ( (cur_state & 2) != 0 && (cur_proto != TARGET_SDBGP|| cur_stype != 20) )
  {
    dat.zero = 0;
    dat.code = v3;
    p = ds_alloc_buf(TARGET_TTYP, TARGET_DID, &dat, 5);
    if ( p )
      ds_send_desc(target_desc, p);
  }
}

static DSP_BUF *recv_kbd(DS_DESC *desc, DSP_BUF *db)
{
  int v3; // eax
  int v4; // eax
  int len; // [esp+0h] [ebp-410h]
  unsigned __int8 *bp; // [esp+8h] [ebp-408h]
  unsigned __int8 buf[1024]; // [esp+Ch] [ebp-404h] BYREF
  DECI2_HDR *dh; // [esp+40Ch] [ebp-4h]

  if ( !db )
    return 0;
  dh = (DECI2_HDR *)db->buf;
  len = dh->length - 12;
  if ( len < 0 )
    return db;
  bp = (unsigned __int8 *)&db->buf[12];
  if ( (cur_state & 1) != 0 )
  {
    while ( 1 )
    {
      v4 = len--;
      if ( v4 <= 0 )
        break;
      if ( *bp == 3 || *bp == 7 )
      {
        abort_input(*bp);
      }
      else if ( (opt_tty_mask->val & 1) != 0 )
      {
        normal_input(*bp);
      }
      ++bp;
    }
  }
  else
  {
    while ( 1 )
    {
      v3 = len--;
      if ( v3 <= 0 )
        break;
      if ( cmd_executing )
      {
        if ( *bp == 3 || *bp == 7 )
        {
          LOBYTE(cur_state) = cur_state | 0x40;
          ++ds_abort_input;
        }
      }
      else
      {
        switch ( ds_editline(&ds_histbuf, (char)*bp, (char *)buf) )
        {
          case 3:
          case 7:
            ds_printf("\n");
            print_prompt();
            break;
          case 9:
            if ( !*ds_histbuf.ptr && ds_cmd_completion(&ds_histbuf, (char *)buf) < 0 )
            {
              ds_printf("\n");
              redraw_input_line(1);
            }
            break;
          case 10:
          case 13:
            ds_printf("\n");
            prompt_len = 0;
            cmd_executing = 1;
            ds_cmd_execute((char *)buf, 1);
            cmd_executing = 0;
            print_prompt();
            break;
          default:
            break;
        }
      }
      ++bp;
    }
  }
  return ds_free_buf(db);
}

#ifdef DSNET_COMPILING_E
static int setup_xgkt(char *fname, unsigned int cnt, unsigned int off)
{
  if ( xgkt_stream )
    return ds_error("XGKICK Trace - busy");
  xgkt_stream = ds_fopen(fname, "w");
  if ( !xgkt_stream )
    return -1;
  xgkt_off = off;
  xgkt_eoff = cnt + off;
  xgkt_seq = 0;
  xgkt_vif1 = 0;
  xgkt_vbs0 = 0;
  return 0;
}

static DSP_BUF *recv_xgkt(DSP_BUF *db)
{
  int flag; // [esp+0h] [ebp-14h]
  int len; // [esp+4h] [ebp-10h]
  DBGP_XGKT_DATA *xd; // [esp+8h] [ebp-Ch]

  if ( !xgkt_stream )
    return db;
  xd = (DBGP_XGKT_DATA *)&db->buf[16];
  len = *(unsigned __int16 *)db->buf - 32;
  if ( len < 0 )
    return db;
  if ( *(_DWORD *)&db->buf[24] != xgkt_off )
    return db;
  if ( *(_DWORD *)&db->buf[28] != xgkt_seq )
    return db;
  if ( !xgkt_seq )
    xgkt_glen = *(_DWORD *)&db->buf[20];
  flag = xd->flag;
  if ( (xd->flag & 1) != 0 )
    ++xgkt_vbs0;
  if ( (flag & 0x1000) != 0 )
    ++xgkt_vif1;
  if ( len > 0 )
    ds_fwrite(&db->buf[32], len, 1, xgkt_stream);
  ++xgkt_seq;
  xgkt_glen -= len;
  if ( xgkt_glen <= 0 )
  {
    xgkt_seq = 0;
    if ( ++xgkt_off >= xgkt_eoff )
    {
      ds_fclose(xgkt_stream);
      xgkt_stream = 0;
      erase_input_line();
      ds_printf("*** XGKICK Trace Completed");
      ds_printf(" (VU0_busy=%d VIF1_busy=%d)\n", xgkt_vbs0, xgkt_vif1);
      redraw_input_line(0);
    }
  }
  return db;
}

static DSP_BUF *recv_rdimg(DSP_BUF *db)
{
  int len; // [esp+0h] [ebp-10h]

  if ( !rdimg_stream )
    return db;
  len = *(unsigned __int16 *)db->buf - 32;
  if ( len < 0 )
    return db;
  if ( *(_DWORD *)&db->buf[20] != rdimg_seq )
    return db;
  if ( len > 0 )
  {
    ds_fwrite(&db->buf[32], len, 1, rdimg_stream);
    rdimg_len += len;
    ++rdimg_seq;
  }
  if ( (unsigned int)rdimg_len >= *(_DWORD *)&db->buf[16] )
  {
    ds_fclose(rdimg_stream);
    rdimg_stream = 0;
  }
  return db;
}
#endif /* DSNET_COMPILING_E */

static DSP_BUF *unexpected_reply(DSP_BUF *db, DBGP_HDR *ch)
{
  int v2; // eax
  unsigned int *wp; // [esp+4h] [ebp-10h]
  int n; // [esp+Ch] [ebp-8h]

  erase_input_line();
  if ( ch->type != 1 || ch->result || (unsigned int)*(unsigned __int16 *)db->buf - 16 <= 0x43 )
  {
    ds_printf("*** Unexpected reply -");
    if ( ch->id )
      ds_printf(" id=%<DI>", ch->id);
    ds_printf(" type=%<DT>", ch->type);
    if ( ch->code )
      ds_printf(" code=%B", ch->code);
    ds_printf(" result=%<DR>", ch->result);
    if ( ch->count )
      ds_printf(" count=%B", ch->count);
    if ( ch->unused )
      ds_printf(" unused=%W", ch->unused);
    ds_printf("\n");
    if ( ch->type == 21 && (ch->result == 36 || ch->result == 39) )
    {
      ds_printf("*** Target program stopped.");
      ds_printf(" Check the location by dr command.\n");
    }
  }
  else
  {
    memcpy(&dbconf, &ch[1], sizeof(dbconf));
    n = 17;
    for ( wp = (unsigned int *)&dbconf; ; ++wp )
    {
      v2 = n--;
      if ( v2 <= 0 )
        break;
      *wp = *wp;
    }
  }
  redraw_input_line(0);
  return db;
}

static DSP_BUF *recv_dbgp(DS_DESC *desc, DSP_BUF *db)
{
  DSP_BUF *result;
  regtype* rp;
  int len;

  /* For my sanity */
  struct {
    DECI2_HDR d2;
    DBGP_HDR dbg;
    char data[0];
  } *d;

  if ( !db )
  {
    cur_state &= ~(1 | 2);
    cur_state |= 0x80;
    return 0;
  }

  d = (void*)db->buf;

  len = d->d2.length - (sizeof(DECI2_HDR) + sizeof(DBGP_HDR));

  if ( len < 0 )
    return db;

  if ( d->dbg.type == DBGP_TYPE_BREAKR )
  {
    ds_bzero(regbuf_mask, sizeof(regbuf_mask));
    dbconf.run_stop_state = 2;
  }
  else if ( d->dbg.type == DBGP_TYPE_CONTINUER || d->dbg.type == DBGP_TYPE_RUNR )
  {
    dbconf.run_stop_state = 1;
  }

#ifdef DSNET_COMPILING_E
  if ( d->dbg.type == DBGP_TYPE_XGKTDATAR )
    return recv_xgkt(db);
#endif /* DSNET_COMPILING_E */

  if ( (cur_state & 1) == 0
    || cur_proto != TARGET_SDBGP
    || cur_wtype != d->dbg.type
    && (d->dbg.type != DBGP_TYPE_CONTINUER || cur_stype != DBGP_TYPE_CONTINUE )
    && (d->dbg.type != DBGP_TYPE_RUNR || cur_stype != DBGP_TYPE_RUN ) )
  {
    return unexpected_reply(db, &d->dbg);
  }

#ifdef DSNET_COMPILING_E
  cur_cpuid = d->dbg.id;
#endif /* DSNET_COMPILING_E */

  cur_result = d->dbg.result;
  if (cur_result != DBGP_RESULT_GOOD) {
    cur_state &= ~( 1 | 2 );
    cur_state |= STATE_OK;
    return ds_free_buf(db);
  }

  switch (d->dbg.group) {
    case DBGP_GROUP_THREAD:
      switch(d->dbg.type) {
        case DBGP_EE_THREAD_TYPE_THREADLISTR:
        case DBGP_EE_THREAD_TYPE_SEMABLOCKR:
        case DBGP_EE_THREAD_TYPE_HANDLERLISTR:
        case DBGP_EE_THREAD_TYPE_TCBR:
            memcpy(cur_pointer, &d->dbg, sizeof(DBGP_HDR));
            ntoh_word_copy(cur_pointer + sizeof(DBGP_HDR), (unsigned int*)d->data, len);
            break;
        default:
            break;
      }
      break;
    case DBGP_GROUP_MODULE:
      switch(d->dbg.type) {
        case DBGP_MODULE_TYPE_LISTR:
        case DBGP_MODULE_TYPE_MEMLISTR:
          iload_callback(0, d->dbg.type, d->data, len);
        case DBGP_MODULE_TYPE_INFOR:
          if (len < 0x23) {
            goto einval;
          }
          iload_callback(*(int*)&d->data[0], d->dbg.type, &d->data[4], len - 4);
          break;
        default:
          break;
      }
      break;
    case DBGP_GROUP_SYSTEM:
      switch(d->dbg.type) {
        case DBGP_TYPE_GETCONFR:
          if (len > cur_length) {
            len = cur_length;
          }
          memcpy(cur_pointer, d->data, len);
          // weird leftover non-op loop was here. byte swap for big-endian systems?
          break;
        case DBGP_TYPE_GETREGR: {
          regtype* dst;
          struct {
            DBGP_REG rhdr;
            regtype rdata;
          } *src;

          if (len != (TARGET_REG_BYTES + sizeof(DBGP_REG)) * cur_length) {
            goto einval;
          }

          src = (void *)d->data;
          dst = (void *)cur_pointer;

          for (int i = 0; i < cur_length; i++) {
            if (src->rhdr.kind < TARGET_REG_NUM_KIND &&
                src->rhdr.number < TARGET_REG_COUNT) {
              regbuf_vals[src->rhdr.kind][src->rhdr.number] = src->rdata;
              regbuf_mask[src->rhdr.kind] |= 1 << src->rhdr.number;

            }

            dst[i] = src[i].rdata;
          }
        } break;
        case DBGP_TYPE_PUTREGR: {
          struct {
            DBGP_REG rhdr;
            regtype rdata;
          } *src;

          if (len != (TARGET_REG_BYTES + sizeof(DBGP_REG)) * cur_length) {
            goto einval;
          }

          src = (void *)d->data;
          for (int i = 0; i < cur_length; i++) {
            if (src->rhdr.kind < TARGET_REG_NUM_KIND &&
                src->rhdr.number < TARGET_REG_COUNT) {
              regbuf_vals[src->rhdr.kind][src->rhdr.number] = src->rdata;
              regbuf_mask[src->rhdr.kind] |= 1 << src->rhdr.number;

            }
          }
        } break;
        case DBGP_TYPE_RDMEMR:
            if (len != cur_padding + cur_length + sizeof(DBGP_MEM)) {
              goto einval;
            }
            memcpy(cur_pointer, &d->data[sizeof(DBGP_MEM) + cur_padding], cur_length);
            break;
        case DBGP_TYPE_WRMEMR:
            if (len != sizeof(DBGP_MEM)) {
              goto einval;
            }
            break;
        case DBGP_TYPE_GETBRKPTR: {
            DBGP_BRKPT_DATA *src, *dst;
            if (len > sizeof(DBGP_BRKPT_DATA) * cur_length ||
                len != sizeof(DBGP_BRKPT_DATA) * d->dbg.count) {
              goto einval;
            }

            src = (DBGP_BRKPT_DATA *)d->data;
            dst = cur_pointer;
            for (int i = 0; i < d->dbg.count; i++) {
              dst[i] = src[i];
            }

            if (cur_count_pointer) {
              *cur_count_pointer = d->dbg.count;
            }
        } break;
        case DBGP_TYPE_PUTBRKPTR:
        case DBGP_TYPE_BREAKR:
            if (len != 0) {
              goto einval;
            }
            break;
        case DBGP_TYPE_CONTINUER:
        case DBGP_TYPE_RUNR:
            if (len != 0) {
              goto einval;
            }
            return ds_free_buf(db);
        break;
        case DBGP_TYPE_XGKTCTLR:
            if (len != sizeof(DBGP_XGKT_CTL)) {
              goto einval;
            }
            break;
        case DBGP_TYPE_DBGCTLR:
            if (len != 4) {
              goto einval;
            }
            break;
#ifdef DSNET_COMPILING_E
        case DBGP_TYPE_RDIMGR:
            db = recv_rdimg(db);
            if (rdimg_stream) {
              return ds_free_buf(db);
            }
            break;
#endif
        case DBGP_TYPE_SETBPFUNCR:
            if (len != 4) {
              goto einval;
            }
            break;
        default:
            return db;
            break;
      }
      break;
    default:
      break;
  }

  cur_state &= ~( 1 | 2 );
  cur_state |= STATE_OK;
  return ds_free_buf(db);

  einval:
  cur_state &= ~( 1 | 2 );
  cur_state |= STATE_INVALID_REPLY;
  return ds_free_buf(db);
}

static void print_state_errors(char *fmt)
{
  const char *v1; // [esp+0h] [ebp-4h]

  v1 = "";
  ds_printf("***");
  if ( (cur_state & 0x20) != 0 )
  {
    ds_printf("%s Timeout", "");
    v1 = ",";
  }
  if ( (cur_state & 0x40) != 0 )
  {
    ds_printf("%s Abort", v1);
    v1 = ",";
  }
  if ( (cur_state & 0x80u) != 0 )
  {
    ds_printf("%s Connection Closed", v1);
    v1 = ",";
  }
  if ( (cur_state & 0x100) != 0 )
  {
    ds_printf("%s System Error", v1);
    v1 = ",";
  }
  if ( (cur_state & 0x200) != 0 )
  {
    v1 = ",";
    ds_printf("%s Invalid Reply", ",");
  }
  if ( (cur_state & 0x400) != 0 )
  {
    ds_printf("%s Resetted", v1);
    v1 = ",";
  }
  if ( (cur_state & 0x10) != 0 )
  {
    ds_printf("%s", v1);
    v1 = ",";
#ifdef DSNET_COMPILING_E
    if ( cur_cpuid )
      ds_printf(" %<DI>", cur_cpuid);
#endif /* DSNET_COMPILING_E */
    ds_printf(fmt, cur_result);
  }
  if ( (cur_state & 0x800) != 0 )
    ds_printf("%s %<CC_ER_LONG>", v1, cur_result);
  ds_printf("\n");
}

static int send_and_wait(DSP_BUF *db, int stype, void *ptr, int len, int wresult)
{
  unsigned __int8 wtype; // [esp+7h] [ebp-Dh]
  int sec; // [esp+8h] [ebp-Ch]
  int r; // [esp+Ch] [ebp-8h]

  sec = 120;
  cur_state = 1;
  cur_proto = TARGET_SDBGP;
  if ( !db )
  {
    cur_stype = -1;
LABEL_9:
    wtype = 21;
    LOBYTE(cur_state) = cur_state | 2;
    sec = -1;
    ds_bzero(regbuf_mask, sizeof(regbuf_mask));
    goto LABEL_11;
  }
  cur_stype = (unsigned __int8)stype;
  if ( (unsigned __int8)stype == 22 )
    goto LABEL_8;
  if ( (unsigned __int8)stype <= 0x16u )
  {
    if ( (unsigned __int8)stype != 20 )
      goto LABEL_10;
LABEL_8:
    dbconf.run_stop_state = -1;
    goto LABEL_9;
  }
  if ( (unsigned __int8)stype == 24 )
    goto LABEL_8;
LABEL_10:
  wtype = stype + 1;
LABEL_11:
  cur_wtype = wtype;
  cur_pointer = ptr;
  cur_length = len;
  if ( db )
    ds_send_desc(target_desc, db);
  do
    r = ds_select_desc(sec, 0);
  while ( r > 0 && (cur_state & 1) != 0 );
  cur_state &= 0xFFFFFFFC;
  if ( r )
  {
    if ( r < 0 )
      cur_state |= 0x100u;
  }
  else
  {
    LOBYTE(cur_state) = cur_state | 0x20;
  }
#ifdef DSNET_COMPILING_E
  if ( (cur_state & 0xFF0) == 16 && (!cur_cpuid || !cur_result) && (wresult == cur_result || cur_result == 35) )
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  if ( (cur_state & 0xFF0) == 16 && (wresult == cur_result || cur_result == 35) )
#endif /* DSNET_COMPILING_I */
    return cur_result;
  print_state_errors(" %<DR_LONG>");
#ifdef DSNET_COMPILING_E
  if ( cur_cpuid )
  {
    if ( (cur_state & 0xFF0) == 16 && cur_result == 36 )
    {
      if ( cur_cpuid == 1 )
      {
        ex0_default_dr();
      }
      else if ( cur_cpuid == 2 )
      {
        ex1_default_dr();
      }
    }
    return -1;
  }
  else if ( (cur_state & 0x10) != 0 )
  {
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  if ( (cur_state & 0x10) != 0 )
#endif /* DSNET_COMPILING_I */
    return cur_result;
#ifdef DSNET_COMPILING_E
  }
#endif /* DSNET_COMPILING_E */
  else
#ifdef DSNET_COMPILING_E
  {
#endif /* DSNET_COMPILING_E */
    return -1;
#ifdef DSNET_COMPILING_E
  }
#endif /* DSNET_COMPILING_E */
}

static int get_conf(DBGP_CONF_DATA *conf)
{
  int r; // [esp+0h] [ebp-Ch]
  DBGP_CONF_DATA *p; // [esp+4h] [ebp-8h] BYREF
  DSP_BUF *db; // [esp+8h] [ebp-4h]

LABEL_1:
  db = alloc_dbgp(DBGP_CPUID_CPU, DBGP_GROUP_SYSTEM, DBGP_TYPE_GETCONF, 0, 0, 0, (void **)&p, sizeof(DBGP_CONF_DATA));
  if ( !db )
    return -1;
  ds_bzero(p, sizeof(DBGP_CONF_DATA));
  r = send_and_wait(db, DBGP_TYPE_GETCONF, conf, sizeof(DBGP_CONF_DATA), 0);
  if ( !r )
    return 0;
  if ( (cur_state & 0xFF0) == 2048 && dcmp_waiting_status )
  {
    while ( 1 )
    {
      r = ds_select_desc(-1, -1);
      if ( r <= 0 )
        break;
      if ( !dcmp_waiting_status )
      {
        ds_printf("*** Retry - GETCONF\n");
        goto LABEL_1;
      }
    }
  }
  return r;
}

static void send_dbconf()
{
  DBGP_CONF_DATA *p; // [esp+0h] [ebp-8h] BYREF
  DSP_BUF *db; // [esp+4h] [ebp-4h]

  db = alloc_dbgp(DBGP_CPUID_CPU, DBGP_GROUP_SYSTEM, DBGP_TYPE_GETCONF, 0, 0, 0, (void **)&p, sizeof(DBGP_CONF_DATA));
  if ( db )
  {
    ds_bzero(p, sizeof(DBGP_CONF_DATA));
    ds_send_desc(target_desc, db);
  }
}

static int rdwr_mem_align(int code, int align, int cpuid, int space, unsigned int adr, void *ptr, int len)
{
  int v8; // eax
  int nw; // [esp+14h] [ebp-28h]
  int n; // [esp+18h] [ebp-24h]
  int n_1; // [esp+18h] [ebp-24h]
  int amsk; // [esp+1Ch] [ebp-20h]
  int asiz; // [esp+20h] [ebp-1Ch]
  int npad; // [esp+24h] [ebp-18h]
  DBGP_MEM *mh; // [esp+2Ch] [ebp-10h] BYREF
  DSP_BUF *db; // [esp+30h] [ebp-Ch]
  unsigned __int8 v17; // [esp+37h] [ebp-5h]
  unsigned __int16 v18; // [esp+38h] [ebp-4h]
  unsigned __int8 v19; // [esp+3Ah] [ebp-2h]
  unsigned __int8 v20; // [esp+3Bh] [ebp-1h]

  v20 = code;
  v19 = align;
  v18 = cpuid;
  v17 = space;
  while ( v19 && (dbconf.mem_align & (1 << v19)) == 0 )
    --v19;
  asiz = 1 << v19;
  amsk = (1 << v19) - 1;
  npad = (~amsk & (amsk + 28)) - 28;
  while ( len > 0 )
  {
    n = len;
    if ( (unsigned int)len > 0xFFE3 )
      n = 65507;
    n_1 = ~amsk & (amsk + n - npad);
    if ( (unsigned int)(npad + n_1) > 0xFFE3 )
      n_1 -= asiz;
    if ( (dbconf.mem_limit_align & (1 << v19)) != 0
      && dbconf.mem_limit_size < n_1 + (~amsk & (unsigned int)(amsk + 28)) )
    {
      n_1 = dbconf.mem_limit_size - (~amsk & (amsk + 28));
      if ( n_1 <= 0 )
        return ds_error("invalid mem_limit");
    }
    if ( v20 == 10 )
      v8 = n_1 + npad;
    else
      v8 = 0;
    nw = v8;
    db = alloc_dbgp(v18, DBGP_GROUP_SYSTEM, v20, 0, 0, 1, (void **)&mh, v8 + 12);
    if ( !db )
      return -1;
    mh->space = v17;
    mh->align = v19;
    mh->reserved = 0;
    mh->address = adr;
    mh->length = n_1;
    if ( nw <= 0 )
    {
      cur_padding = (~amsk & (amsk + 28)) - 28;
      if ( send_and_wait(db, v20, ptr, n_1, 0) )
        return -1;
    }
    else
    {
      if ( npad > 0 )
        ds_bzero(&mh[1], npad);
      memcpy(&mh[1].space + npad, ptr, n_1);
      if ( send_and_wait(db, v20, 0, 0, 0) )
        return -1;
    }
    adr += n_1;
    ptr = (char *)ptr + n_1;
    len -= n_1;
  }
  return 0;
}

static int rdwr_mem(int code, unsigned int adr, void *ptr, int len)
{
  int n; // [esp+8h] [ebp-14h]
  int n_1; // [esp+8h] [ebp-14h]
  int off; // [esp+Ch] [ebp-10h]
  int amsk; // [esp+10h] [ebp-Ch]
  int asiz; // [esp+14h] [ebp-8h]

  asiz = 1 << dbconf_max_mem_align;
  amsk = (1 << dbconf_max_mem_align) - 1;
  off = amsk & adr;
  if ( (int)(amsk & adr) > 0 )
  {
    n = asiz - off;
    if ( len < asiz - off )
      n = len;
    if ( rdwr_mem_align((unsigned __int8)code, 0, 0, 0, adr, ptr, n) < 0 )
      return -1;
    adr += n;
    ptr = (char *)ptr + n;
    len -= n;
  }
  n_1 = len & ~amsk;
  if ( n_1 > 0 )
  {
    if ( rdwr_mem_align((unsigned __int8)code, (unsigned __int8)dbconf_max_mem_align, 0, 0, adr, ptr, n_1) < 0 )
      return -1;
    adr += n_1;
    ptr = (char *)ptr + n_1;
    len -= n_1;
  }
  if ( len <= 0 || rdwr_mem_align((unsigned __int8)code, 0, 0, 0, adr, ptr, len) >= 0 )
    return 0;
  else
    return -1;
}

static int load_mem(unsigned int adr, void *ptr, int len)
{
  return rdwr_mem(8, adr, ptr, len);
}

static int store_mem(unsigned int adr, void *ptr, int len)
{
  return rdwr_mem(10, adr, ptr, len);
}

int get_handlerlist(DBGP_HDR *phdr)
{
  DSP_BUF *db; // [esp+0h] [ebp-8h]

  db = alloc_dbgp(DBGP_CPUID_CPU, DBGP_GROUP_THREAD, DBGP_EE_THREAD_TYPE_HANDLERLIST, phdr->code, 0, 0, 0, 0);
  if ( !db )
    return -1;
  if ( send_and_wait(db, DBGP_EE_THREAD_TYPE_HANDLERLIST, phdr, 0, 0) )
    return -1;
  return 0;
}

int get_tcb(DBGP_HDR *phdr, int tid)
{
  int *ptid; // [esp+0h] [ebp-Ch] BYREF
  DSP_BUF *db; // [esp+4h] [ebp-8h]
  int id; // [esp+8h] [ebp-4h]

  id = 0;
  db = alloc_dbgp(DBGP_CPUID_CPU, DBGP_GROUP_THREAD, DBGP_EE_THREAD_TYPE_TCB, phdr->code, 0, 0, (void **)&ptid, 4);
  if ( !db )
    return -1;
  *ptid = tid;
  if ( send_and_wait(db, DBGP_EE_THREAD_TYPE_TCB, phdr, 0, 0) )
    return -1;
  else
    return 0;
}

int get_thread_list(DBGP_HDR *phdr)
{
  DSP_BUF *db; // [esp+0h] [ebp-8h]

  db = alloc_dbgp(DBGP_CPUID_CPU, DBGP_GROUP_THREAD, DBGP_EE_THREAD_TYPE_THREADLIST, phdr->code, 0, 0, 0, 0);
  if ( !db )
    return -1;
  if ( send_and_wait(db, DBGP_EE_THREAD_TYPE_THREADLIST, phdr, 0, 0) )
    return -1;
  return 0;
}

int get_semablock(DBGP_HDR *phdr, int sid)
{
  int *psid; // [esp+0h] [ebp-Ch] BYREF
  DSP_BUF *db; // [esp+4h] [ebp-8h]
  int id; // [esp+8h] [ebp-4h]

  id = 0;
  db = alloc_dbgp(DBGP_CPUID_CPU, DBGP_GROUP_THREAD, DBGP_EE_THREAD_TYPE_SEMABLOCK, phdr->code, 0, 0, (void **)&psid, 4);
  if ( !db )
    return -1;
  *psid = sid;
  if ( send_and_wait(db, DBGP_EE_THREAD_TYPE_SEMABLOCK, phdr, 0, 0) )
    return -1;
  else
    return 0;
}

int load_word_registers(unsigned int *masks, unsigned int *pv, int n)
{
  int nr; // [esp+Ch] [ebp-20h]
  int k; // [esp+10h] [ebp-1Ch]
  int j; // [esp+14h] [ebp-18h]
  int i; // [esp+18h] [ebp-14h]
  unsigned int *dp; // [esp+1Ch] [ebp-10h]
  DBGP_REG *rh; // [esp+20h] [ebp-Ch] BYREF
  DSP_BUF *db; // [esp+24h] [ebp-8h]
  int id; // [esp+28h] [ebp-4h]

  id = 0;
  j = 0;
  i = 0;
  while ( 1 )
  {
    if ( n <= 0 )
      return 0;
    nr = n;
    if ( dbconf.nreg < n )
      nr = dbconf.nreg;
    db = alloc_dbgp(id, DBGP_GROUP_SYSTEM, DBGP_TYPE_GETREG, 0, 0, (unsigned __int8)nr, (void **)&rh, 8 * nr);
    if ( !db )
      return -1;
    if ( i > 9 )
      break;
    for ( k = 0; nr > k; ++k )
    {
      while ( !masks[i] )
      {
        j = 0;
        if ( ++i > 9 )
          return ds_error("internal error %s:%s", "dbg.c", "load_word_registers");
      }
      while ( (masks[i] & (1 << j)) == 0 )
      {
        if ( ++j > 31 )
        {
          j = 0;
          if ( ++i > 9 )
            return ds_error("internal error %s:%s", "dbg.c", "load_word_registers");
        }
      }
      rh->kind = i;
      rh->number = j;
      rh->reserved = 0;
      dp = (unsigned int *)++rh;
      ds_bzero(rh, sizeof(DBGP_REG));
      rh = (DBGP_REG *)(dp + 1);
      if ( ++j > 31 )
      {
        j = 0;
        ++i;
      }
    }
    if ( send_and_wait(db, DBGP_TYPE_GETREG, pv, nr, 0) )
      return -1;
    pv += nr;
    n -= nr;
  }
  return ds_error("internal error %s:%s", "dbg.c", "load_word_registers");
}


int store_word_registers(unsigned int *masks, unsigned int *pv, int n)
{
  int nr; // [esp+Ch] [ebp-20h]
  int k; // [esp+10h] [ebp-1Ch]
  int j; // [esp+14h] [ebp-18h]
  int i; // [esp+18h] [ebp-14h]
  unsigned int *dp; // [esp+1Ch] [ebp-10h]
  DBGP_REG *rh; // [esp+20h] [ebp-Ch] BYREF
  DSP_BUF *db; // [esp+24h] [ebp-8h]
  int id; // [esp+28h] [ebp-4h]

  id = 0;
  j = 0;
  i = 0;
  while ( 1 )
  {
    if ( n <= 0 )
      return 0;
    nr = n;
    if ( dbconf.nreg < n )
      nr = dbconf.nreg;
    db = alloc_dbgp(id, DBGP_GROUP_SYSTEM, DBGP_TYPE_PUTREG, 0, 0, (unsigned __int8)nr, (void **)&rh, 8 * nr);
    if ( !db )
      return -1;
    if ( i > 9 )
      break;
    for ( k = 0; nr > k; ++k )
    {
      while ( !masks[i] )
      {
        j = 0;
        if ( ++i > 9 )
          return ds_error("internal error %s:%s", "dbg.c", "store_word_registers");
      }
      while ( (masks[i] & (1 << j)) == 0 )
      {
        if ( ++j > 31 )
        {
          j = 0;
          if ( ++i > 9 )
            return ds_error("internal error %s:%s", "dbg.c", "store_word_registers");
        }
      }
      rh->kind = i;
      rh->number = j;
      rh->reserved = 0;
      dp = (unsigned int *)&rh[1];
      *dp = *pv++;
      rh = (DBGP_REG *)(dp + 1);
      if ( ++j > 31 )
      {
        j = 0;
        ++i;
      }
    }
    if ( send_and_wait(db, DBGP_TYPE_PUTREG, 0, nr, 0) )
      return -1;
    n -= nr;
  }
  return ds_error("internal error %s:%s", "dbg.c", "store_word_registers");
}

static int load_quad_registers_id(int id, unsigned int *masks, quad *pv, int n)
{
  int nr; // [esp+Ch] [ebp-1Ch]
  int k; // [esp+10h] [ebp-18h]
  int j; // [esp+14h] [ebp-14h]
  int i; // [esp+18h] [ebp-10h]
  quad *dp; // [esp+1Ch] [ebp-Ch]
  DBGP_REG *rh; // [esp+20h] [ebp-8h] BYREF
  DSP_BUF *db; // [esp+24h] [ebp-4h]

  j = 0;
  i = 0;
  while ( 1 )
  {
    if ( n <= 0 )
      return 0;
    nr = n;
    if ( dbconf.nreg < n )
      nr = dbconf.nreg;
    db = alloc_dbgp(id, DBGP_GROUP_SYSTEM, DBGP_TYPE_GETREG, 0, 0, (unsigned __int8)nr, (void **)&rh, 20 * nr);
    if ( !db )
      return -1;
    if ( i > 10 )
      break;
    for ( k = 0; nr > k; ++k )
    {
      while ( !masks[i] )
      {
        j = 0;
        if ( ++i > 10 )
          return ds_error("internal error %s:%s", "dbg.c", "load_quad_registers_id");
      }
      while ( (masks[i] & (1 << j)) == 0 )
      {
        if ( ++j > 31 )
        {
          j = 0;
          if ( ++i > 10 )
            return ds_error("internal error %s:%s", "dbg.c", "load_quad_registers_id");
        }
      }
      rh->kind = i;
      rh->number = j;
      rh->reserved = 0;
      dp = (quad *)++rh;
      ds_bzero(rh, sizeof(DBGP_REG));
      rh = (DBGP_REG *)&dp[1];
      if ( ++j > 31 )
      {
        j = 0;
        ++i;
      }
    }
    if ( send_and_wait(db, DBGP_TYPE_GETREG, pv, nr, 0) )
      return -1;
    pv += nr;
    n -= nr;
  }
  return ds_error("internal error %s:%s", "dbg.c", "load_quad_registers_id");
}

static int store_quad_registers_id(int id, unsigned int *masks, quad *pv, int n)
{
  DBGP_REG *v5; // eax
  int nr; // [esp+Ch] [ebp-1Ch]
  int k; // [esp+10h] [ebp-18h]
  int j; // [esp+14h] [ebp-14h]
  int i; // [esp+18h] [ebp-10h]
  quad *dp; // [esp+1Ch] [ebp-Ch]
  DBGP_REG *rh; // [esp+20h] [ebp-8h] BYREF
  DSP_BUF *db; // [esp+24h] [ebp-4h]

  j = 0;
  i = 0;
  while ( 1 )
  {
    if ( n <= 0 )
      return 0;
    nr = n;
    if ( dbconf.nreg < n )
      nr = dbconf.nreg;
    db = alloc_dbgp(id, DBGP_GROUP_SYSTEM, DBGP_TYPE_PUTREG, 0, 0, (unsigned __int8)nr, (void **)&rh, 20 * nr);
    if ( !db )
      return -1;
    if ( i > 10 )
      break;
    for ( k = 0; nr > k; ++k )
    {
      while ( !masks[i] )
      {
        j = 0;
        if ( ++i > 10 )
          return ds_error("internal error %s:%s", "dbg.c", "store_quad_registers_id");
      }
      while ( (masks[i] & (1 << j)) == 0 )
      {
        if ( ++j > 31 )
        {
          j = 0;
          if ( ++i > 10 )
            return ds_error("internal error %s:%s", "dbg.c", "store_quad_registers_id");
        }
      }
      rh->kind = i;
      rh->number = j;
      rh->reserved = 0;
      dp = (quad *)&rh[1];
      v5 = rh + 1;
      ((unsigned int *)(&rh[1]))[0] = pv->xa[0];
      ((unsigned int *)(&v5[1]))[0] = pv->xa[1];
      ((unsigned int *)(&v5[2]))[0] = pv->xa[2];
      ((unsigned int *)(&v5[3]))[0] = pv->xa[3];
      ++pv;
      rh = (DBGP_REG *)&dp[1];
      if ( ++j > 31 )
      {
        j = 0;
        ++i;
      }
    }
    if ( send_and_wait(db, DBGP_TYPE_PUTREG, 0, nr, 0) )
      return -1;
    n -= nr;
  }
  return ds_error("internal error %s:%s", "dbg.c", "store_quad_registers_id");
}

static int access_quad_registers(int (*func)(), unsigned int *masks, quad *pv, int n)
{
  int nvu1; // [esp+Ch] [ebp-40h]
  int nvu0; // [esp+10h] [ebp-3Ch]
  int ncpu; // [esp+14h] [ebp-38h]
  int j; // [esp+18h] [ebp-34h]
  int i; // [esp+1Ch] [ebp-30h]
  unsigned int rmasks[11]; // [esp+20h] [ebp-2Ch] BYREF

  nvu1 = 0;
  nvu0 = 0;
  ncpu = 0;
  for ( i = 0; i <= 10; ++i )
  {
    if ( masks[i] )
    {
      for ( j = 0; j <= 31; ++j )
      {
        if ( (masks[i] & (1 << j)) != 0 )
        {
          if ( i < 7 )
          {
            ++ncpu;
          }
          else if ( i <= 8 )
          {
            ++nvu0;
          }
          else
          {
            ++nvu1;
          }
        }
      }
    }
  }
  if ( ncpu )
  {
    memcpy(rmasks, masks, sizeof(rmasks));
    memset(&rmasks[7], 0, 16);
    if ( ((int (*)(_DWORD, unsigned int *, quad *, int))func)(0, rmasks, pv, ncpu) )
      return -1;
    pv += ncpu;
  }
  if ( nvu0 )
  {
    ds_bzero(rmasks, sizeof(rmasks));
    rmasks[7] = masks[7];
    rmasks[8] = masks[8];
    if ( ((int (*)(int, unsigned int *, quad *, int))func)(1, rmasks, pv, nvu0) )
      return -1;
    pv += nvu0;
  }
  if ( nvu1
    && (ds_bzero(rmasks, sizeof(rmasks)),
        rmasks[9] = masks[9],
        rmasks[10] = masks[10],
        ((int (*)(int, unsigned int *, quad *, int))func)(2, rmasks, pv, nvu1)) )
  {
    return -1;
  }
  else
  {
    return 0;
  }
}

int load_quad_registers(unsigned int *masks, quad *pv, int n)
{
  return access_quad_registers((int (*)())load_quad_registers_id, masks, pv, n);
}

int store_quad_registers(unsigned int *masks, quad *pv, int n)
{
  return access_quad_registers((int (*)())store_quad_registers_id, masks, pv, n);
}

int cont_and_wait_halt(int code, int cnt)
{
  DSP_BUF *db; // [esp+8h] [ebp-8h]

  db = alloc_dbgp(DBGP_CPUID_CPU, DBGP_GROUP_SYSTEM, DBGP_TYPE_CONTINUE, code, 0, cnt, 0, 0);
  if ( !db )
    return -1;

  return send_and_wait(db, DBGP_TYPE_CONTINUE, 0, 0, DBGP_RESULT_EXCEPTION);
}

int run_and_wait_halt(unsigned int entry_point, unsigned int gp_value, int argc, char *args, int args_len)
{
  DBGP_EERUN *er; // [esp+0h] [ebp-8h] BYREF
  DSP_BUF *db; // [esp+4h] [ebp-4h]

  db = alloc_dbgp(DBGP_CPUID_CPU, DBGP_GROUP_SYSTEM, DBGP_TYPE_RUN, 0, 0, 0, (void **)&er, args_len + sizeof(DBGP_EERUN));
  if ( !db )
    return -1;
  ds_bzero(er, sizeof(DBGP_EERUN));
  er->entry = entry_point;
  er->gp = gp_value;
  er->argc = argc;
  if ( args )
  {
    if ( args_len > 0 )
      memcpy(&er[1], args, args_len);
  }
  return send_and_wait(db, DBGP_TYPE_RUN, 0, 0, DBGP_RESULT_EXCEPTION);
}

int break_and_wait_halt()
{
  DSP_BUF *db; // [esp+0h] [ebp-4h]

  db = alloc_dbgp(DBGP_CPUID_CPU, DBGP_GROUP_SYSTEM, DBGP_TYPE_BREAK, 0, 0, 0, 0, 0);
  if ( !db )
    return -1;

  return send_and_wait(db, DBGP_TYPE_BREAK, 0, 0, 0);
}

int wait_halt()
{
  return send_and_wait(0, 255, 0, 0, 0);
}

int get_brkpt(DBGP_BRKPT_DATA *bps, int *pn)
{
  DSP_BUF *db; // [esp+0h] [ebp-4h]

  db = alloc_dbgp(DBGP_CPUID_CPU, DBGP_GROUP_SYSTEM, DBGP_TYPE_GETBRKPT, 0, 0, 0, 0, 0);
  if ( !db )
    return -1;
  cur_count_pointer = pn;
  if ( send_and_wait(db, DBGP_TYPE_GETBRKPT, bps, dbconf.nbrkpt, 0) )
    return -1;
  else
    return 0;
}

int put_brkpt(DBGP_BRKPT_DATA *bps, int n)
{
  int i; // [esp+0h] [ebp-Ch]
  DBGP_BRKPT_DATA *dp; // [esp+4h] [ebp-8h] BYREF
  DSP_BUF *db; // [esp+8h] [ebp-4h]

  db = alloc_dbgp(DBGP_CPUID_CPU, DBGP_GROUP_SYSTEM, DBGP_TYPE_PUTBRKPT, 0, 0, (unsigned __int8)n, (void **)&dp, 8 * n);
  if ( !db )
    return -1;
  for ( i = 0; n > i; ++i )
  {
    dp->adr = bps->adr;
    dp->cnt = bps->cnt;
    ++dp;
    ++bps;
  }
  cur_count_pointer = 0;
  if ( send_and_wait(db, DBGP_TYPE_PUTBRKPT, 0, n, 0) )
    return -1;
  else
    return 0;
}

int is_target_is_running()
{
  if ( dbconf.run_stop_state == 1 )
    return 1;
  if ( dbconf.run_stop_state == 2 )
    return 0;
  return -1;
}

static DSP_BUF *recv_loadp(DS_DESC *desc, DSP_BUF *db)
{
  DSP_BUF *result; // eax
  unsigned int vals[2]; // [esp+8h] [ebp-2Ch] BYREF
  int i; // [esp+10h] [ebp-24h]
  ILOADP_MODINFO_DATA *mi; // [esp+14h] [ebp-20h]
  unsigned int *wpe; // [esp+18h] [ebp-1Ch]
  unsigned int *wp; // [esp+1Ch] [ebp-18h]
  unsigned int id; // [esp+20h] [ebp-14h]
  int r; // [esp+24h] [ebp-10h]
  int len; // [esp+28h] [ebp-Ch]
  ILOADP_HDR *ip; // [esp+2Ch] [ebp-8h]
  DECI2_HDR *dh; // [esp+30h] [ebp-4h]

  dh = (DECI2_HDR *)db->buf;
  ip = (ILOADP_HDR *)&db->buf[8];
  if ( db )
  {
    len = dh->length - 16;
    if ( len >= 0 )
    {
      id = ip->module_id;
      if ( ip->cmd == 16 )
      {
        if ( len == 4 )
        {
          vals[0] = ip->result;
          vals[1] = *(_DWORD *)&ip[1].cmd;
          r = iload_callback(id, ip->cmd, vals, 8);
          if ( r && (cur_state & 1) != 0 && cur_proto == TARGET_SDBGP && cur_wtype == 21 )
          {
            cur_result = r;
            cur_state &= 0xFFFFFFFC;
            LOBYTE(cur_state) = cur_state | 0x10;
          }
          return ds_free_buf(db);
        }
        else
        {
          return db;
        }
      }
      else if ( (cur_state & 1) != 0 )
      {
        if ( cur_proto == TARGET_LOADP && cur_wtype == ip->cmd )
        {
          if ( cur_stamp == ip->stamp )
          {
            cur_result = ip->result;
            if ( cur_result )
            {
LABEL_39:
              cur_state &= ~1u;
              LOBYTE(cur_state) = cur_state | 0x10;
              return ds_free_buf(db);
            }
            else
            {
              switch ( ip->cmd )
              {
                case 1u:
                case 7u:
                  if ( !len || (unsigned int)len > 0x20 )
                  {
                    mi = (ILOADP_MODINFO_DATA *)&ip[1];
                    *(_WORD *)&ip[1].cmd = *(_WORD *)&ip[1].cmd;
                    mi->flags = mi->flags;
                    mi->mod_addr = mi->mod_addr;
                    mi->text_size = mi->text_size;
                    mi->data_size = mi->data_size;
                    mi->bss_size = mi->bss_size;
                    mi->entry_address = mi->entry_address;
                    mi->gp_value = mi->gp_value;
                    for ( i = 0; i < mi->extnumwords; ++i )
                      mi->extword[i] = mi->extword[i];
                    goto LABEL_38;
                  }
                  result = db;
                  break;
                case 3u:
                case 9u:
                  if ( !len )
                    goto LABEL_38;
                  result = db;
                  break;
                case 5u:
                case 0xBu:
                  wp = (unsigned int *)&ip[1].cmd;
                  wpe = (unsigned int *)(&ip[1].cmd + 4 * ((unsigned int)len >> 2));
                  while ( wpe > wp )
                  {
                    *wp = *wp;
                    ++wp;
                  }
LABEL_38:
                  iload_callback(id, ip->cmd, &ip[1], len);
                  goto LABEL_39;
                default:
                  result = db;
                  break;
              }
            }
          }
          else
          {
            return db;
          }
        }
        else
        {
          return db;
        }
      }
      else
      {
        return db;
      }
    }
    else
    {
      return db;
    }
  }
  else
  {
    cur_state &= 0xFFFFFFFC;
    LOBYTE(cur_state) = cur_state | 0x80;
    return 0;
  }
  return result;
}

int send_iload_and_wait(int cmd, int action, unsigned int id, void *ptr, int len, int nodisp)
{
  unsigned int *wp; // [esp+8h] [ebp-20h] BYREF
  int n; // [esp+Ch] [ebp-1Ch]
  int r; // [esp+10h] [ebp-18h]
  ILOADP_HDR *ip; // [esp+14h] [ebp-14h]
  DECI2_HDR *dh; // [esp+18h] [ebp-10h]
  DSP_BUF *db2; // [esp+1Ch] [ebp-Ch]
  DSP_BUF *db; // [esp+20h] [ebp-8h]
  unsigned __int8 v14; // [esp+26h] [ebp-2h]
  unsigned __int8 v15; // [esp+27h] [ebp-1h]

  v15 = cmd;
  v14 = action;
  n = 0;
  if ( (unsigned __int8)cmd != 6 )
  {
    if ( (unsigned __int8)cmd > 6u )
    {
      if ( (unsigned __int8)cmd != 10 )
        goto LABEL_10;
      goto LABEL_8;
    }
    if ( (unsigned __int8)cmd != 4 )
      goto LABEL_10;
  }
  if ( is_target_is_running() )
    goto LABEL_10;
LABEL_8:
  if ( !IsSupported(TARGET_VERSION_MAJOR, TARGET_VERSION_MINOR) )
    return ds_error("LOADP extension is required DBGP version %d.%d or later.", TARGET_VERSION_MAJOR, TARGET_VERSION_MINOR);
LABEL_10:
  if ( is_target_is_running() )
  {
    if ( !v15 )
      ds_bzero(regbuf_mask, sizeof(regbuf_mask));
    db = ds_alloc_buf(TARGET_LOADP, TARGET_DID, 0, len + 8);
    if ( db )
    {
      dh = (DECI2_HDR *)db->buf;
      ip = (ILOADP_HDR *)&db->buf[8];
      db->buf[8] = v15;
      ip->action = v14;
      ip->result = 0;
      cur_stamp = dsip_stamp;
      ip->stamp = dsip_stamp++;
      if ( dsip_stamp > 239 )
        dsip_stamp = 128;
      ip->module_id = id;
      if ( ptr && len > 0 )
        memcpy(&ip[1], ptr, len);
      while ( 1 )
      {
        cur_state = 1;
        cur_proto = TARGET_LOADP;
        cur_stype = v15;
        cur_wtype = v15 + 1;
        dsc_proto = 0;
        db2 = ds_dup_buf(db);
        ds_send_desc(target_desc, db);
        do
          r = ds_select_desc(120, 0);
        while ( r > 0 && (cur_state & 1) != 0 );
        cur_state &= ~1u;
        if ( (cur_state & 0x800) == 0 || cur_result != 1 )
          break;
        while ( 1 )
        {
          r = ds_select_desc(-1, -1);
          if ( r <= 0 || dsc_proto )
            break;
          if ( (cur_state & 0x440) != 0 )
            goto LABEL_46;
        }
        if ( r <= 0 )
          break;
        db = db2;
      }
LABEL_46:
      ds_free_buf(db2);
      if ( !r )
        LOBYTE(cur_state) = cur_state | 0x20;
      if ( r < 0 )
        cur_state |= 0x100u;
      if ( (cur_state & 0xFF0) != 16 || cur_result )
      {
        if ( (cur_state & 0xFF0) != 16 || nodisp != cur_result )
          print_state_errors(" %<IR_N_LONG>");
        if ( (cur_state & 0xFF0) == 16 )
          return cur_result;
        else
          return -1;
      }
      else
      {
        return 0;
      }
    }
    else
    {
      return -1;
    }
  }
  else
  {
    if ( v15 == 6 )
    {
      v15 = 66;
      n += 4;
    }
    else if ( v15 > 6u )
    {
      if ( v15 != 10 )
        goto LABEL_20;
      v15 = 68;
    }
    else
    {
      if ( v15 != 4 )
      {
LABEL_20:
        ds_printf("*** LOCKED\n");
        return -1;
      }
      v15 = 64;
    }
    db = alloc_dbgp(DBGP_CPUID_CPU, DBGP_GROUP_MODULE, v15, 0, 0, 0, (void **)&wp, n);
    if ( db )
    {
      if ( v15 == 66 )
        *wp = id;
      return send_and_wait(db, v15, ptr, len, 0);
    }
    else
    {
      return -1;
    }
  }
}

static int get_and_check_config()
{
  int n; // [esp+4h] [ebp-4h]

  if ( get_conf(&dbconf) )
  {
    ds_printf("can not get GETCONF - may be reset is needed\n");
    return -1;
  }
  else if ( dbconf.target_id == TARGET_SDBGP)
  {
    if ( (dbconf.mem_align & 1) != 0 )
    {
      for ( n = 10; n >= 0 && (dbconf.mem_align & (1 << n)) == 0; --n )
        ;
      dbconf_max_mem_align = n;
      if ( dbconf.reg_size == TARGET_REG_SIZE )
      {
        if ( dbconf.nbrkpt <= 0xFF )
        {
          if ( dbconf.nreg && dbconf.nreg <= 0xFF )
          {
            if ( dbconf.ncont <= 0xFF )
            {
              if ( dbconf.nstep <= 0xFF )
              {
                if ( dbconf.nnext <= 0xFF )
                {
                  dbconf_is_valid = 1;
                  opt_target_exec_ctl_config->val = dbconf.nstep != 0;
                  return 0;
                }
                else
                {
                  return ds_error("invalid nnext");
                }
              }
              else
              {
                return ds_error("invalid nstep");
              }
            }
            else
            {
              return ds_error("invalid ncont");
            }
          }
          else
          {
            return ds_error("invalid nreg");
          }
        }
        else
        {
          return ds_error("invalid nbrkpt");
        }
      }
      else
      {
        return ds_error("invalid reg_size");
      }
    }
    else
    {
      return ds_error("invalid mem_align");
    }
  }
  else
  {
    return ds_error("invalid target_id");
  }
}

static void print_size(unsigned int size)
{
  const char *v1; // eax
  int v2; // [esp+0h] [ebp-4h]

  v2 = 0;
  if ( size <= 0x3FF || (ds_printf("%dKB", size >> 10), v2 = 1, (size &= 0x3FFu) != 0) )
  {
    if ( v2 )
      v1 = "+";
    else
      v1 = "";
    ds_printf("%s%d", v1, size);
  }
}

static void print_align_list(unsigned int align)
{
  const char *v1; // eax
  int following; // [esp+0h] [ebp-8h]
  int i; // [esp+4h] [ebp-4h]

  following = 0;
  for ( i = 31; i >= 0; --i )
  {
    if ( (align & (1 << i)) != 0 )
    {
      if ( ++following == 1 )
        v1 = "";
      else
        v1 = " ";
      ds_printf("%s", v1);
      switch ( i )
      {
        case 0:
          ds_printf("BYTE");
          break;
        case 1:
          ds_printf("HALF");
          break;
        case 2:
          ds_printf("WORD");
          break;
        case 3:
          ds_printf("TWIN");
          break;
        case 4:
          ds_printf("QUAD");
          break;
        case 10:
          ds_printf("1KB");
          break;
        default:
          print_size(1 << i);
          break;
      }
    }
  }
}

static int show_dbconf(int ac, char **av)
{
  const char *v3; // eax
  unsigned int run_stop_state; // eax
  char **ava; // [esp+10h] [ebp+Ch]

  if ( ac <= 0 )
    return 0;
  ava = av + 1;
  if ( ac - 1 > 0 && **ava == 45 )
    return ds_error("Usage: show dbconf");
  if ( !dbconf_is_valid )
    return ds_error("dbconf is invalid, now");
  ds_printf("major_ver       = 0x%08x # %d", dbconf.major_ver, dbconf.major_ver);
  ds_printf("\nminor_ver       = 0x%08x # %d", dbconf.minor_ver, dbconf.minor_ver);
  ds_printf("\ntarget_id       = 0x%08x # %<SP>", dbconf.target_id, dbconf.target_id);
  ds_printf("\nreserved1       = 0x%08x # %d", dbconf.reserved1, dbconf.reserved1);
  ds_printf("\nmem_align       = 0x%08x # ", dbconf.mem_align);
  print_align_list(dbconf.mem_align);
  ds_printf("\nreserved2       = 0x%08x # %d", dbconf.reserved2, dbconf.reserved2);
  if ( dbconf.reg_size )
    v3 = "s";
  else
    v3 = "";
  ds_printf("\nreg_size        = 0x%08x # %dbit%s", dbconf.reg_size, 1 << dbconf.reg_size, v3);
  ds_printf("\nnreg            = 0x%08x # %d", dbconf.nreg, dbconf.nreg);
  ds_printf("\nnbrkpt          = 0x%08x # %d", dbconf.nbrkpt, dbconf.nbrkpt);
  ds_printf("\nncont           = 0x%08x # %d", dbconf.ncont, dbconf.ncont);
  ds_printf("\nnstep           = 0x%08x # %d", dbconf.nstep, dbconf.nstep);
  ds_printf("\nnnext           = 0x%08x # %d", dbconf.nnext, dbconf.nnext);
  ds_printf("\nmem_limit_align = 0x%08x # ", dbconf.mem_limit_align);
  print_align_list(dbconf.mem_align);
  ds_printf("\nmem_limit_size  = 0x%08x # ", dbconf.mem_limit_size);
  print_size(dbconf.mem_limit_size);
  ds_printf("\nrun_stop_state  = 0x%08x # ", dbconf.run_stop_state);
  run_stop_state = dbconf.run_stop_state;
  if ( run_stop_state == 1 )
  {
    ds_printf("RUNNING");
  }
  else if ( run_stop_state == 2 )
  {
    ds_printf("STOPPED");
  }
  else
  {
    ds_printf("???????");
  }
  ds_printf("\nhdbg_area_addr  = 0x%08x #", dbconf.hdbg_area_addr);
  ds_printf("\nhdbg_area_size  = 0x%08x # ", dbconf.hdbg_area_size);
  print_size(dbconf.hdbg_area_size);
  ds_printf("\n");
  return 0;
}

int is_target_exec_ctl()
{
  if ( opt_target_exec_ctl_override->val < 0 )
    return opt_target_exec_ctl_config->val;
  else
    return opt_target_exec_ctl_override->val;
}

int is_automatic_prefix_breakpoint()
{
  return opt_automatic_prefix_breakpoint->val;
}

int is_describe_ub_all()
{
  return opt_describe_ub_all->val;
}

int is_lstep_stop_at_no_line()
{
  return opt_lstep_stop_at_no_line->val;
}

void di_format(int *f_adr, int *f_iw, int *f_ba, int *f_ma)
{
  if ( f_adr )
    *f_adr = opt_di_address->val;
  if ( f_iw )
    *f_iw = opt_di_instruction_word->val;
  if ( f_ba )
    *f_ba = opt_di_branch_address->val;
  if ( f_ma )
    *f_ma = opt_di_macro->val;
}

char *dr_format_str()
{
  return opt_dr_default_format->str;
}

char *source_directories_str()
{
  return opt_source_directories->str;
}

int get_help_lang()
{
  char *lang; // [esp+0h] [ebp-4h]

  lang = opt_help_lang->str;
  if ( !strcmp("euc", lang) || !strcmp("EUC", lang) )
    return 1;
  if ( !strcmp("jis", lang) || !strcmp("JIS", lang) )
    return 2;
  if ( !strcmp("sjis", lang) || !strcmp("SJIS", lang) )
    return 3;
  return 0;
}

char *get_help_pager()
{
  return opt_help_pager->str;
}

static int reset_cmd(int ac, char **av)
{
  unsigned int wv; // [esp+0h] [ebp-48h] BYREF
  quad pv; // [esp+4h] [ebp-44h] BYREF
  char *istr; // [esp+14h] [ebp-34h]
  char *estr; // [esp+18h] [ebp-30h]
  NETMP_BOOTP mb; // [esp+1Ch] [ebp-2Ch] BYREF
  int ccon; // [esp+40h] [ebp-8h]
  int crst; // [esp+44h] [ebp-4h]
  int aca; // [esp+50h] [ebp+8h]
  int acb; // [esp+50h] [ebp+8h]
  char **ava; // [esp+54h] [ebp+Ch]
  char **avb; // [esp+54h] [ebp+Ch]

  crst = resetted;
  ccon = dsc_connected;
  if ( ac <= 0 )
    return 0;
  ava = av + 1;
  aca = ac - 1;
  if ( aca > 0 && !strcmp("-i", *ava) )
  {
    ds_set_option("current_ebootp", 3, opt_initial_ebootp->str, 0, 0);
    ds_set_option("current_ibootp", 3, opt_initial_ibootp->str, 0, 0);
    ++ava;
    --aca;
  }
  if ( aca > 0 && !strcmp("-help", *ava) )
    return ds_error("Usage: reset [-i | <ebootp> [<ibootp>]]");
  estr = opt_current_ebootp->str;
  istr = opt_current_ibootp->str;
  ds_bzero(&mb, sizeof(mb));
  if ( aca > 0 )
  {
    estr = *ava;
    ds_set_option("current_ebootp", 3, estr, 0, 0);
    avb = ava + 1;
    acb = aca - 1;
    if ( acb > 0 )
    {
      istr = *avb;
      ds_set_option("current_ibootp", 3, istr, 0, 0);
      if ( acb - 1 > 0 )
        return ds_error("Usage: reset [-i | <ebootp> [<ibootp>]]");
    }
  }
  if ( ds_eval_quad(estr, &pv) )
    return ds_error("Usage: reset [-i | <ebootp> [<ibootp>]]");
  wv = pv.xa[0];
  memcpy(mb.param, &wv, 4u);
  wv = pv.xa[1];
  memcpy(&mb.param[4], &wv, 4u);
  if ( ds_eval_quad(istr, &pv) )
    return ds_error("Usage: reset [-i | <ebootp> [<ibootp>]]");
  wv = pv.xa[0];
  memcpy(&mb.param[8], &wv, 4u);
  wv = pv.xa[1];
  memcpy(&mb.param[12], &wv, 4u);
  flush_tty_buf();
  remove_breakpoints();
  clear_symbol();
  clear_mdebug();
  clear_mod_list();
#ifdef DSNET_COMPILING_E
  if ( xgkt_stream )
    ds_fclose(xgkt_stream);
  xgkt_stream = 0;
  if ( rdimg_stream )
    ds_fclose(rdimg_stream);
  rdimg_stream = 0;
#endif /* DSNET_COMPILING_E */
  ds_bzero(regbuf_mask, sizeof(regbuf_mask));
  if ( ds_reset(target_desc, &mb, 34) < 0 )
    return -1;
  expected_resetr = 1;
  while ( ds_select_desc(-1, -1) > 0 && (resetted <= crst || dsc_connected <= ccon) )
    ;
  expected_resetr = 0;
  get_and_check_config();
  return 0;
}

#ifdef DSNET_COMPILING_E
static int dbgctl(int id, int flag)
{
  unsigned int *wp; // [esp+0h] [ebp-8h] BYREF
  DSP_BUF *db; // [esp+4h] [ebp-4h]

  db = alloc_dbgp(id, DBGP_GROUP_SYSTEM, DBGP_TYPE_DBGCTL, 0, 0, 0, (void **)&wp, 4);
  if ( !db )
    return -1;
  *wp = flag;
  return send_and_wait(db, DBGP_TYPE_DBGCTL, 0, 0, 0);
}

static int dbgctl_cmd(int ac, char **av)
{
  int flag; // [esp+0h] [ebp-8h]
  int id; // [esp+4h] [ebp-4h]

  if ( ac <= 0 )
    return 0;
  if ( ac != 3 )
    return ds_error("Usage: dbgctl {vu0|vu1} {on|off}");
  if ( !strcmp("vu0", av[1]) )
  {
    id = 1;
  }
  else
  {
    if ( strcmp("vu1", av[1]) )
      return ds_error("Usage: dbgctl {vu0|vu1} {on|off}");
    id = 2;
  }
  if ( !strcmp("on", av[2]) )
  {
    flag = 1;
    goto LABEL_15;
  }
  if ( strcmp("off", av[2]) )
    return ds_error("Usage: dbgctl {vu0|vu1} {on|off}");
  flag = 0;
LABEL_15:
  if ( id == 2 && !flag )
  {
    if ( xgkt_stream )
      ds_fclose(xgkt_stream);
    xgkt_stream = 0;
  }
  return dbgctl(id, flag);
}

static int xgkt_cmd(int ac, char **av)
{
  DBGP_XGKT_CTL *xc; // [esp+0h] [ebp-14h] BYREF
  DSP_BUF *db; // [esp+4h] [ebp-10h]
  unsigned int off; // [esp+8h] [ebp-Ch] BYREF
  unsigned int cnt; // [esp+Ch] [ebp-8h] BYREF
  char *fname; // [esp+10h] [ebp-4h]
  int aca; // [esp+1Ch] [ebp+8h]
  char **ava; // [esp+20h] [ebp+Ch]

  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  ava = av + 1;
  if ( aca > 0 && **ava == 45 || aca != 2 && aca != 3 )
    return ds_error("Usage: xgkt <fname> <cnt> [<off>]");
  fname = *ava;
  if ( ds_eval_word(ava[1], &cnt) )
    return -1;
  if ( !cnt )
    return 0;
  off = 0;
  if ( aca > 2 && ds_eval_word(ava[2], &off) )
    return -1;
  if ( dbgctl(2, 1) )
    return -1;
  if ( setup_xgkt(fname, cnt, off) )
    return -1;
  db = alloc_dbgp(DBGP_CPUID_VU1, DBGP_GROUP_SYSTEM, DBGP_TYPE_XGKTCTL, 0, 0, 0, (void **)&xc, 12);
  if ( !db )
    return -1;
  xc->flag = opt_xgkt_flag->val;
  xc->off = off;
  xc->cnt = cnt;
  if ( send_and_wait(db, DBGP_TYPE_XGKTCTL, 0, 0, 0) )
    return -1;
  ds_printf("*** XGKICK Trace Started ");
  ds_printf(" (flag=0x%x fname=\"%s\" cnt=0x%x off=0x%x)\n", opt_xgkt_flag->val, fname, cnt, off);
  return 0;
}

static int storeimage_cmd(int ac, char **av)
{
  DBGP_RDIMG *pp; // [esp+0h] [ebp-2Ch] BYREF
  DSP_BUF *db; // [esp+4h] [ebp-28h]
  int h; // [esp+8h] [ebp-24h] BYREF
  int w; // [esp+Ch] [ebp-20h] BYREF
  int y; // [esp+10h] [ebp-1Ch] BYREF
  int x; // [esp+14h] [ebp-18h] BYREF
  int spsm; // [esp+18h] [ebp-14h] BYREF
  int sbw; // [esp+1Ch] [ebp-10h] BYREF
  int sbp; // [esp+20h] [ebp-Ch] BYREF
  int r; // [esp+24h] [ebp-8h]
  char *fname; // [esp+28h] [ebp-4h]
  int aca; // [esp+34h] [ebp+8h]
  char **ava; // [esp+38h] [ebp+Ch]

  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  ava = av + 1;
  if ( aca > 0 && **ava == 45 || aca != 8 )
    return ds_error("Usage: storeimage <fname> <bp> <bw> <psm> <x> <y> <w> <h>");
  fname = *ava;
  if ( ds_eval_word(ava[1], (unsigned int *)&sbp) )
    return -1;
  if ( ds_eval_word(ava[2], (unsigned int *)&sbw) )
    return -1;
  if ( ds_eval_word(ava[3], (unsigned int *)&spsm) )
    return -1;
  if ( ds_eval_word(ava[4], (unsigned int *)&x) )
    return -1;
  if ( ds_eval_word(ava[5], (unsigned int *)&y) )
    return -1;
  if ( ds_eval_word(ava[6], (unsigned int *)&w) )
    return -1;
  if ( ds_eval_word(ava[7], (unsigned int *)&h) )
    return -1;
  if ( rdimg_stream )
    return ds_error("storeimage - busy");
  rdimg_stream = ds_fopen(fname, "w");
  if ( !rdimg_stream )
    return -1;
  rdimg_seq = 0;
  rdimg_len = 0;
  db = alloc_dbgp(DBGP_CPUID_CPU, DBGP_GROUP_SYSTEM, DBGP_TYPE_RDIMG, 0, 0, 0, (void **)&pp, 16);
  if ( !db )
    return -1;
  pp->sbp = sbp;
  pp->sbw = sbw;
  pp->spsm = spsm;
  pp->x = x;
  pp->y = y;
  pp->w = w;
  pp->h = h;
  r = send_and_wait(db, DBGP_TYPE_RDIMG, 0, 0, 0);
  if ( rdimg_stream )
    ds_fclose(rdimg_stream);
  rdimg_stream = 0;
  return r;
}
#endif /* DSNET_COMPILING_E */

static int bpfunc_cmd(int ac, char **av)
{
  DSP_BUF *db; // [esp+0h] [ebp-Ch]
  unsigned int *wp; // [esp+4h] [ebp-8h] BYREF
  unsigned int adr; // [esp+8h] [ebp-4h] BYREF
  int aca; // [esp+14h] [ebp+8h]
  char **ava; // [esp+18h] [ebp+Ch]

  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  ava = av + 1;
  if ( aca > 0 && **ava == 45 || aca != 1 )
    return ds_error("Usage: bpfunc <adr>");
  if ( ds_eval_word(*ava, &adr) )
    return -1;
  db = alloc_dbgp(DBGP_CPUID_CPU, DBGP_GROUP_SYSTEM, DBGP_TYPE_SETBPFUNC, 0, 0, 0, (void **)&wp, 4);
  if ( !db )
    return -1;
  *wp = adr;
  if ( send_and_wait(db, DBGP_TYPE_SETBPFUNC, 0, 0, 0) )
    return -1;
  else
    return 0;
}

static DSP_BUF *recv_dcmp(DS_DESC *desc, DSP_BUF *db)
{
  int v2; // eax
  DCMP_ERROR_DATA *ed; // [esp+8h] [ebp-Ch]
  DCMP_HDR *ch; // [esp+Ch] [ebp-8h]

  ch = (DCMP_HDR *)&db->buf[8];
  if ( !db )
    ds_exit(130);
  if ( ch->type == 2 )
  {
    ed = (DCMP_ERROR_DATA *)&db->buf[12];
    if ( (unsigned __int8)db->buf[9] <= 3u )
      dcmp_waiting_status &= ~(1 << db->buf[9]);
    if ( db->buf[9] )
    {
      if ( db->buf[9] == 1 && cur_proto == ed->orig_hdr.length )
        ++dsc_proto;
    }
    // clearly wrong struct access
    else if ( ed->orig_hdr.length == TARGET_DID )
    {
      ++dsc_connected;
      if ( need_getconf )
      {
        send_dbconf();
        need_getconf = 0;
      }
    }
  }
  else if ( ch->type == 3 )
  {
    v2 = (unsigned __int8)db->buf[9];
    if ( v2 == 2 )
    {
      LOBYTE(dcmp_waiting_status) = dcmp_waiting_status | 4;
    }
    else if ( (unsigned __int8)db->buf[9] > 2u )
    {
      if ( v2 == 3 )
      {
        LOBYTE(dcmp_waiting_status) = dcmp_waiting_status | 8;
      }
      else if ( v2 == 5 )
      {
        LOBYTE(dcmp_waiting_status) = dcmp_waiting_status | 1;
      }
    }
    else if ( v2 == 1 )
    {
      LOBYTE(dcmp_waiting_status) = dcmp_waiting_status | 2;
    }
    if ( (cur_state & 1) != 0 && cur_proto == *(unsigned __int16 *)&db->buf[16] )
    {
      if ( cur_proto == TARGET_SDBGP)
      {
        if ( cur_stype != (unsigned __int8)db->buf[22] )
          return ds_free_buf(db);
      }
      else if ( cur_proto == TARGET_LOADP && cur_stype != (unsigned __int8)db->buf[20] )
      {
        return ds_free_buf(db);
      }
      cur_state &= 0xFFFFFFFC;
      cur_state |= 0x800u;
      cur_result = (unsigned __int8)db->buf[9];
    }
  }
  return ds_free_buf(db);
}

static void batch(int ac, char **av)
{
  int r; // [esp+4h] [ebp-40Ch]
  char *v3; // [esp+8h] [ebp-408h]
  char *p; // [esp+Ch] [ebp-404h]
  char buf[1024]; // [esp+10h] [ebp-400h] BYREF

  v3 = buf;
  while ( ac > 0 )
  {
    for ( p = *av; *p && (unsigned int)(v3 - buf) <= 0x3FE; ++p )
      *v3++ = *p;
    if ( ac > 1 && (unsigned int)(v3 - buf) <= 0x3FE )
      *v3++ = 32;
    --ac;
    ++av;
  }
  *v3 = 0;
  r = ds_cmd_execute(buf, 0);
  if ( !no_exit )
    ds_exit(r);
  ++f_batch;
}

static DSP_BUF *recv_netmp(DS_DESC *desc, DSP_BUF *db)
{
  int v2; // eax
  int n; // [esp+0h] [ebp-Ch]
  NETMP_HDR *v5; // [esp+4h] [ebp-8h]

  v5 = (NETMP_HDR *)&db->buf[8];
  if ( !db )
    ds_exit(130);
  if ( v5->code == 3 )
  {
    ++resetted;
    remove_breakpoints();
    clear_symbol();
    clear_mdebug();
    clear_mod_list();
#ifdef DSNET_COMPILING_E
    if ( xgkt_stream )
      ds_fclose(xgkt_stream);
    xgkt_stream = 0;
    if ( rdimg_stream )
      ds_fclose(rdimg_stream);
    rdimg_stream = 0;
#endif /* DSNET_COMPILING_E */
    ds_recv_drfp(desc, 0);
    dbconf.run_stop_state = 0;
    if ( (cur_state & 1) != 0 )
    {
      cur_state &= 0xFFFFFFFC;
    }
    else if ( !expected_resetr && !cmd_executing )
    {
      erase_input_line();
      ds_printf("*** Resetted\n");
      redraw_input_line(0);
    }
    cur_state |= 0x400u;
    if ( !expected_resetr )
      ++need_getconf;
    dcmp_waiting_status = 0;
  }
  else if ( dsm_waiting == v5->code )
  {
    if ( v5->code == 1 )
    {
      if ( db->buf[9] )
        ds_exit(132);
      if ( !argc || !no_kbd )
      {
        kbd_desc = ds_open_kbd(recv_kbd, 0);
        if ( !kbd_desc )
          ds_exit(131);
      }
      ds_send_netmp_default_msg(target_desc);
      dsm_waiting = 5;
    }
    else
    {
      if ( v5->code == 5 )
      {
        if ( opt_check_manager_version->val )
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
      if ( argc <= 0 && opt_reset_on_start->val || force_reset_on_start )
      {
        ds_printf("*** Resetting...\n");
        ds_cmd_execute("reset", 0);
      }
      if ( dbconf_is_valid || !get_and_check_config() )
      {
        if ( !f_nx )
          ds_read_startup_file();
        if ( argc > 0 )
          batch(argc, argv);
        ds_printf("*** DBGP Version %d.%02d\n", dbconf.major_ver, dbconf.minor_ver);
      }
      else if ( !f_nx )
      {
        ds_printf("** startup file execution is skipped\n");
      }
      if ( no_kbd )
      {
        if ( f_batch )
          ds_printf("(type CTRL-C to finish %s)\n\n", ds_program_name);
      }
      else
      {
        ds_printf("(type `help' for getting help, or `help help' for getting help of help)\n\n");
      }
      print_prompt();
    }
  }
  else if ( dsm_waiting == 11 && v5->code == 10 && db->buf[9] )
  {
    ds_exit(148);
  }
  return ds_free_buf(db);
}

static int send_netmp_connect_request()
{
  NETMP_PROTOS *_p; // [esp+4h] [ebp-78h]
  NETMP_PROTOS *_p_3; // [esp+4h] [ebp-78h]
  NETMP_PROTOS *_p_4; // [esp+4h] [ebp-78h]
  NETMP_PROTOS *_p_5; // [esp+4h] [ebp-78h]
  NETMP_PROTOS *_p_6; // [esp+4h] [ebp-78h]
  NETMP_PROTOS *_p_7; // [esp+4h] [ebp-78h]
  int i; // [esp+8h] [ebp-74h]
  int i_1; // [esp+8h] [ebp-74h]
  int pri; // [esp+Ch] [ebp-70h]
  NETMP_PROTOS *p; // [esp+10h] [ebp-6Ch]
  NETMP_PROTOS protos[26]; // [esp+14h] [ebp-68h] BYREF

  ds_bzero(protos, sizeof(protos));
  p = &protos[1];

  protos[0].pri = TARGET_PRI ;
  protos[0].proto = TARGET_SDBGP;
  for ( i = 0; i <= 9; ++i )
  {
    if ( ((opt_tty_mask->val >> i) & 1) != 0 )
    {
      _p = p++;
      _p->pri = TARGET_PRI;
      _p->proto = TARGET_TTYP + i;
    }
  }
  if ( SLOWORD(opt_tty_mask->val) < 0 )
  {
    _p_3 = p++;
    _p_3->pri = TARGET_PRI;
    _p_3->proto = TARGET_KTTYP;
  }
  for ( i_1 = 0; i_1 <= 9; ++i_1 )
  {
    if ( ((opt_atty_mask->val >> i_1) & 1) != 0 )
    {
      _p_4 = p++;
      // optionally register other CPU tty
      _p_4->pri = TARGET_PRI;
      _p_4->proto = TARGET_OTHER_TTYP + i_1;
    }
  }
  if ( SLOWORD(opt_atty_mask->val) < 0 )
  {
    _p_5 = p++;
    _p_5->pri = TARGET_PRI;
#ifdef DSNET_COMPILING_E
    _p_5->proto = PROTO_IKTTYP;
#elif DSNET_COMPILING_I
    _p_5->proto = PROTO_EKTTYP;
#endif /* DSNET_COMPILING_I */
  }
  if ( (opt_tty_mask->val & 0x10000) != 0 )
  {
    _p_6 = p++;
    _p_6->pri = TARGET_PRI;
    _p_6->proto = PROTO_STTYP;
  }
  pri = opt_file_priority->val;
  if ( pri >= 0 )
  {
    _p_7 = p++;
    _p_7->pri = pri;
    _p_7->proto = TARGET_DRFP;
  }
  p->pri = TARGET_PRI;
  p->proto = TARGET_LOADP;

  if ( ds_send_netmp(target_desc, 0, 0, protos, (char *)&p[1] - (char *)protos) < 0 )
    ds_exit(135);

  return 0;
}

static DSP_BUF *recv_console(DS_DESC *desc, DSP_BUF *db)
{
  if ( db )
  {
    if ( *(unsigned __int16 *)db->buf - 8 >= 0 )
    {
      if ( db->buf[7] == 72 )
      {
        if ( *(_WORD *)&db->buf[4] == 1041 )
          return recv_kbd(desc, db);
        else
          return ds_send_dcmp_error(desc, db, 1);
      }
      else
      {
        return ds_send_dcmp_error(desc, db, 0);
      }
    }
    else
    {
      return ds_send_dcmp_error(desc, db, 4);
    }
  }
  else
  {
    erase_input_line();
    ds_printf("*** Console - disconnected\n");
    redraw_input_line(0);
    ds_disp_desc = 0;
    return 0;
  }
}

static int start_console(DS_DESC *desc)
{
  if ( !ds_add_recv_func(desc, -1, -1, -1, recv_console) )
    return -1;
  erase_input_line();
  ds_printf("*** Console - connected (%s)\n", (const char *)&desc[1]);
  redraw_input_line(0);
  ds_disp_desc = desc;
  return 0;
}

static void drfp_error(DSP_BUF *db)
{
  if ( target_desc )
    ds_send_desc(target_desc, db);
  else
    ds_free_buf(db);
}

static int usage(int f_true)
{
  if ( !f_true )
    return 0;
  ds_printf("%s (%s)\n", ds_program_name, ds_stamp_str[0]);
  ds_printf("Usage: %s [<options>]... [<cmd> [<arg>]...]\n", ds_program_name);
  ds_printf("  <options>:\n");
  ds_printf("    -d <host>[/<port_offset>]  set hostname and port offset of manager\n");
  ds_printf("    -d /<port_offset>          set port offset of manager\n");
  ds_printf("    -d <host>[:<port>]         set hostname and port number of manager\n");
  ds_printf("    -ncmv                      do not check manager version\n");
  ds_printf("    -cmv                       check manager version\n");
  ds_printf("    -r                         reset on start\n");
  ds_printf("    -nr                        no reset on start\n");
  ds_printf("    -nx                        do not execute startup file\n");
  ds_printf("    -tec                       target executing control\n");
  ds_printf("    -ntec                      no target executing control\n");
  ds_printf("    -ebootp <ebootp>           initial EE boot parameter\n");
  ds_printf("    -ibootp <ibootp>           initial IOP boot parameter\n");
  ds_printf("    -nokbd                     do not use keyboard when batch\n");
  ds_printf("    -noexit                    do not exit when batch\n");
  ds_printf("    -p <port>                  set console port for listen\n");
  return ds_exit(129);
}

static void dup_and_exe(char *cmd)
{
  size_t v1; // eax
  char *v2; // eax
  char *buf; // [esp+0h] [ebp-4h]

  if ( cmd && *cmd )
  {
    v1 = strlen(cmd);
    buf = (char *)ds_alloc(v1 + 1);
    if ( buf )
    {
      v2 = strcpy(buf, cmd);
      ds_cmd_execute(v2, 0);
      ds_free(buf);
    }
  }
}

void dr_default_di()
{
  dup_and_exe(opt_dr_default_di->str);
}

void ex_default_dr()
{
  dup_and_exe(opt_ex_default_dr->str);
}

#ifdef DSNET_COMPILING_E
void dr0_default_di()
{
  dup_and_exe(opt_dr0_default_di->str);
}

void ex0_default_dr()
{
  dup_and_exe(opt_ex0_default_dr->str);
}

void dr1_default_di()
{
  dup_and_exe(opt_dr1_default_di->str);
}

void ex1_default_dr()
{
  dup_and_exe(opt_ex1_default_dr->str);
}
#endif /* DSNET_COMPILING_E */

void lstep_default_list()
{
  dup_and_exe(opt_lstep_default_list->str);
}

static void set_options_to_default()
{
  char *langenv; // [esp+0h] [ebp-10h]
  char *target_name; // [esp+Ch] [ebp-4h]

  langenv = ds_getenv("LANG");
  target_name = ds_getenv("DSNETM");
  if ( !target_name )
    target_name = (char *)"";
  opt_target_name = ds_set_option("target_name", 3, target_name, 0, 1);
  opt_tty_mask = ds_set_option("tty_mask", 2, 0, 99327, 1);
  opt_atty_mask = ds_set_option("atty_mask", 2, 0, 0, 1);
#ifdef DSNET_COMPILING_E
  opt_file_priority = ds_set_option("file_priority", 2, 0, 208, 1);
#elif DSNET_COMPILING_I
  opt_file_priority = ds_set_option("file_priority", 2, 0, 192, 1);
#endif /* DSNET_COMPILING_I */
  opt_reset_on_start = ds_set_option("reset_on_start", 1, 0, 1, 1);
  opt_check_manager_version = ds_set_option("check_manager_version", 1, 0, 0, 1);
  opt_target_exec_ctl_config = ds_set_option("target_exec_ctl_config", 2, 0, -1, 1);
  opt_target_exec_ctl_override = ds_set_option("target_exec_ctl_override", 2, 0, -1, 1);
  opt_dr_default_format = ds_set_option("dr_default_format", 3, (char *)"", 0, 1);
  opt_dr_default_di = ds_set_option("dr_default_di", 3, "\\di -m $_PC $_PC-8 7", 0, 1);
  opt_ex_default_dr = ds_set_option("ex_default_dr", 3, "\\dr", 0, 1);
#ifdef DSNET_COMPILING_E
  opt_dr0_default_di = ds_set_option("dr0_default_di", 3, "\\vdi -vu0 -m $_vu0vi26 $_vu0vi26-3 7", 0, 1);
  opt_ex0_default_dr = ds_set_option("ex0_default_dr", 3, "\\dr -vu0", 0, 1);
  opt_dr1_default_di = ds_set_option("dr1_default_di", 3, "\\vdi -vu1 -m $_vu1vi26 $_vu1vi26-3 7", 0, 1);
  opt_ex1_default_dr = ds_set_option("ex1_default_dr", 3, "\\dr -vu1", 0, 1);
#endif /* DSNET_COMPILING_E */
  opt_lstep_default_list = ds_set_option("lstep_default_list", 3, "\\list", 0, 1);
  opt_lstep_stop_at_no_line = ds_set_option("lstep_stop_at_no_line", 2, 0, 1, 1);
  opt_source_directories = ds_set_option("source_directories", 3, (char *)"", 0, 1);
  opt_initial_ebootp = ds_set_option("initial_ebootp", 3, "-1", 0, 1);
  opt_current_ebootp = ds_set_option("current_ebootp", 3, "-1", 0, 1);
  opt_initial_ibootp = ds_set_option("initial_ibootp", 3, "-1", 0, 1);
  opt_current_ibootp = ds_set_option("current_ibootp", 3, "-1", 0, 1);
  opt_automatic_prefix_breakpoint = ds_set_option("automatic_prefix_breakpoint", 1, 0, 0, 1);
  opt_describe_ub_all = ds_set_option("describe_ub_all", 1, 0, 0, 1);
#ifdef DSNET_COMPILING_E
  opt_xgkt_flag = ds_set_option("xgkt_flag", 2, 0, 0, 1);
#endif /* DSNET_COMPILING_E */
  opt_di_address = ds_set_option("di_address", 2, 0, 11, 1);
  opt_di_instruction_word = ds_set_option("di_instruction_word", 2, 0, 0, 1);
  opt_di_branch_address = ds_set_option("di_branch_address", 2, 0, 0, 1);
  opt_di_macro = ds_set_option("di_macro", 2, 0, 1, 1);

  if (langenv)  {
    if (!strcmp("ja_JP.eucJP", langenv) || !strcmp("ja_JP.EUC", langenv)) {
      opt_help_lang = ds_set_option("help_lang", 3, "euc", 0, 1);
    } else if (!strcmp("ja_JP.jisJP", langenv) || !strcmp("ja_JP.JIS", langenv)) {
      opt_help_lang = ds_set_option("help_lang", 3, "jis", 0, 1);
    } else if (!strcmp("ja_JP.sjisJP", langenv) || !strcmp("ja_JP.SJIS", langenv)) {
      opt_help_lang = ds_set_option("help_lang", 3, "sjis", 0, 1);
    } else {
      opt_help_lang = ds_set_option("help_lang", 3, "eng", 0, 1);
    }
  } else {
    opt_help_lang = ds_set_option("help_lang", 3, "eng", 0, 1);
  }

  opt_help_pager = ds_set_option("help_pager", 3, (char *)"", 0, 1);
  ds_opt_standard_init();
  opt_iopconf = ds_set_option("IOPCONF", 3, "/usr/local/sce/iop/modules", 0, 1);
  opt_iopmodules = ds_set_option("IOPMODULES", 3, "/usr/local/sce/iop/modules", 0, 1);
}

int main(int ac, char **av)
{
  int type; // ebx
  int v3; // ebx
  int v4; // ebx
  int v5; // ebx
  int v6; // ebx
  int v7; // ebx
  char *port_name; // [esp+4h] [ebp-Ch]
  int i; // [esp+8h] [ebp-8h]
  int r; // [esp+Ch] [ebp-4h]
  int argc_; // [esp+18h] [ebp+8h]
  char **argv_; // [esp+1Ch] [ebp+Ch]

  port_name = 0;
  ds_program_name = ds_basename(*av);
  set_options_to_default();
  ds_read_option_file();
  argc_ = ac - 1;
  for ( argv_ = av + 1; argc_ > 0 && **argv_ == 45; ++argv_ )
  {
    if ( !strcmp("-d", *argv_) )
    {
      usage(--argc_ <= 0);
      ds_set_option("target_name", 3, *++argv_, 0, 0);
    }
    else if ( !strcmp("-r", *argv_) )
    {
      ++force_reset_on_start;
      ds_set_option("reset_on_start", 1, 0, 1, 0);
    }
    else if ( !strcmp("-nr", *argv_) )
    {
      ds_set_option("reset_on_start", 1, 0, 0, 0);
    }
    else if ( !strcmp("-nx", *argv_) )
    {
      ++f_nx;
    }
    else if ( !strcmp("-tec", *argv_) )
    {
      opt_target_exec_ctl_override->val = 1;
    }
    else if ( !strcmp("-ntec", *argv_) )
    {
      opt_target_exec_ctl_override->val = 0;
    }
    else if ( !strcmp("-ebootp", *argv_) )
    {
      usage(--argc_ <= 0);
      ds_set_option("initial_ebootp", 3, *++argv_, 0, 0);
    }
    else if ( !strcmp("-ibootp", *argv_) )
    {
      usage(--argc_ <= 0);
      ds_set_option("initial_ibootp", 3, *++argv_, 0, 0);
    }
    else if ( !strcmp("-nokbd", *argv_) )
    {
      ++no_kbd;
    }
    else if ( !strcmp("-noexit", *argv_) )
    {
      ++no_exit;
    }
    else if ( !strcmp("-ncmv", *argv_) )
    {
      ds_set_option("check_manager_version", 1, 0, 0, 0);
    }
    else if ( !strcmp("-cmv", *argv_) )
    {
      ds_set_option("check_manager_version", 1, 0, 1, 0);
    }
    else if ( !strcmp("-p", *argv_) )
    {
      usage(--argc_ <= 0);
      port_name = *++argv_;
    }
    else
    {
      usage(1);
    }
    --argc_;
  }
  argc = argc_;
  argv = argv_;
  if ( !argc_ )
    ds_printf("%s (%s)\n", ds_program_name, ds_stamp_str[0]);
  type = opt_target_name->type;
  LOBYTE(type) = type | 0x10;
  opt_target_name->type = type;
  v3 = opt_tty_mask->type;
  LOBYTE(v3) = v3 | 0x10;
  opt_tty_mask->type = v3;
  v4 = opt_atty_mask->type;
  LOBYTE(v4) = v4 | 0x10;
  opt_atty_mask->type = v4;
  v5 = opt_file_priority->type;
  LOBYTE(v5) = v5 | 0x10;
  opt_file_priority->type = v5;
  v6 = opt_reset_on_start->type;
  LOBYTE(v6) = v6 | 0x10;
  opt_reset_on_start->type = v6;
  v7 = opt_check_manager_version->type;
  LOBYTE(v7) = v7 | 0x10;
  opt_check_manager_version->type = v7;
  ds_set_option("current_ebootp", 3, opt_initial_ebootp->str, 0, 0);
  ds_set_option("current_ibootp", 3, opt_initial_ibootp->str, 0, 0);
  target_desc = ds_connect_net(opt_target_name->str, 0);
  if ( !target_desc )
    ds_exit(132);
  if ( !ds_add_recv_func(target_desc, 1, -1, -1, recv_dcmp) )
    ds_exit(135);
  if ( !ds_add_recv_func(target_desc, 1024, -1, -1, recv_netmp) )
    ds_exit(135);
  if ( !ds_add_recv_func(target_desc, TARGET_SDBGP, -1, -1, recv_dbgp) )
    ds_exit(135);
  for ( i = 0; i <= 15; ++i )
  {
    if ( i <= 9 || i > 14 )
    {
      if ( ((opt_tty_mask->val >> i) & 1) != 0 && !ds_add_recv_func(target_desc, TARGET_TTYP + i, -1, -1, recv_ttyp) )
        ds_exit(135);
      if ( ((opt_atty_mask->val >> i) & 1) != 0 && !ds_add_recv_func(target_desc, TARGET_OTHER_TTYP +  i, -1, -1, recv_ttyp) )
        ds_exit(135);
    }
  }
  if ( (opt_tty_mask->val & 0x10000) != 0 && !ds_add_recv_func(target_desc, 1043, -1, -1, recv_ttyp) )
    ds_exit(135);
  if ( opt_file_priority->val >= 0 && !ds_add_recv_func(target_desc, TARGET_DRFP, -1, -1, ds_recv_drfp) )
    ds_exit(135);
  if ( !ds_add_recv_func(target_desc, TARGET_LOADP, -1, -1, recv_loadp) )
    ds_exit(135);

#ifdef DSNET_COMPILING_E
  ds_cmd_install("dt", "[-[acdefhrqsuvw]*] [<tid>]", "display thread", dt_cmd);
  ds_cmd_install("ds", "[-v] [<sid>]", "display semaphore", ds_cmd);
  ds_cmd_install("intr", (char *)"", "display handler", intr_cmd);
#endif /* DSNET_COMPILING_E */
  ds_cmd_install("dq", "[<adr> [<cnt>]]", "display memory", dmem_cmd);
  ds_cmd_install("dd", "[<adr> [<cnt>]]", "display memory", dmem_cmd);
  ds_cmd_install("dw", "[<adr> [<cnt>]]", "display memory", dmem_cmd);
  ds_cmd_install("dh", "[<adr> [<cnt>]]", "display memory", dmem_cmd);
  ds_cmd_install("db", "[<adr> [<cnt>]]", "display memory", dmem_cmd);
  ds_cmd_install("df", "[<adr> [<cnt>]]", "display memory", dmem_cmd);
  ds_cmd_install("sq", "<adr> <val>...", "store memory", smem_cmd);
  ds_cmd_install("sd", "<adr> <val>...", "store memory", smem_cmd);
  ds_cmd_install("sw", "<adr> <val>...", "store memory", smem_cmd);
  ds_cmd_install("sh", "<adr> <val>...", "store memory", smem_cmd);
  ds_cmd_install("sb", "<adr> <val>...", "store memory", smem_cmd);
  ds_cmd_install("sf", "<adr> <val>...", "store memory", smem_cmd);
  ds_cmd_install("iq", "[<adr> [<cnt>]]", "input from device", inp_cmd);
  ds_cmd_install("id", "[<adr> [<cnt>]]", "input from device", inp_cmd);
  ds_cmd_install("iw", "[<adr> [<cnt>]]", "input from device", inp_cmd);
  ds_cmd_install("ih", "[<adr> [<cnt>]]", "input from device", inp_cmd);
  ds_cmd_install("ib", "[<adr> [<cnt>]]", "input from device", inp_cmd);
  ds_cmd_install("oq", "<adr> <val>...", "output to device", out_cmd);
  ds_cmd_install("od", "<adr> <val>...", "output to device", out_cmd);
  ds_cmd_install("ow", "<adr> <val>...", "output to device", out_cmd);
  ds_cmd_install("oh", "<adr> <val>...", "output to device", out_cmd);
  ds_cmd_install("ob", "<adr> <val>...", "output to device", out_cmd);
  ds_cmd_install("bload", "<fname> <adr>", "binary load", bload_cmd);
  ds_cmd_install("bsave", "<fname> <adr> <cnt>", "binary save", bsave_cmd);
#ifdef DSNET_COMPILING_E
  ds_cmd_install("dr", "[-<cpuid>] [-[hfxw]*] [<reg>]...", "display register(s)", dreg_cmd);
#elif DSNET_COMPILING_I
  ds_cmd_install("dr", "[<reg>]...", "display register(s)", dreg_cmd);
#endif /* DSNET_COMPILING_I */
  ds_cmd_install("sr", "[-f] [<reg> <val>]...", "set register(s)", sreg_cmd);
  ds_cmd_install("rload", "<fname>", "load register(s)", rload_cmd);
  ds_cmd_install("rsave", "<fname> [<reg>]...", "store register(s)", rsave_cmd);
#ifdef DSNET_COMPILING_E
  ds_cmd_install("pload", "<fname> [<args>]...", "program load", pload_cmd);
#endif /* DSNET_COMPILING_E */
  ds_cmd_install("mload", "<fname> [<args>]...", "module load", mload_cmd);
  ds_cmd_install("mstart", "[-d] [<fname> [<args>]...]", "module start", mstart_cmd);
  ds_cmd_install("mremove", "<id>...", "module remove", mremove_cmd);
  ds_cmd_install("mlist", "[-m] [-l]", "module list", mlist_cmd);
  ds_cmd_install("memlist", "[-s] [-f] [-a <adr>] [-r <range>]", "memory list", memlist_cmd);
  ds_cmd_install("sload", "[-id <id>] [-b <base>] [<fname>]", "symbol load", pload_cmd);
  ds_cmd_install("di", "[-m <mark>] [<adr> [<cnt>]]", "disassemble", di_cmd);
#ifdef DSNET_COMPILING_E
  ds_cmd_install("vdi", "[-<cpuid>] [-m <mark>] [<adr> [<cnt>]]", "VU disassemble", vdi_cmd);
#endif /* DSNET_COMPILING_E */
  ds_cmd_install("as", "<adr> <inst>", "assemble", as_cmd);
  ds_cmd_install("list", "[<adr> [<cnt> [<back>]]]", "list source line", list_cmd);
  ds_cmd_install("bt", "[<cnt>]", "backtrace", bt_cmd);
  ds_cmd_install("bp", "[<adr>[,<cnt>]]...", "set breakpoint", bp_cmd);
  ds_cmd_install("ub", "[all|<adr>]...", "remove breakpoint", ub_cmd);
  ds_cmd_install("be", "[<adr>]...", "enable breakpoint", be_cmd);
  ds_cmd_install("bd", "[<adr>]...", "disable breakpoint", bd_cmd);
#ifdef DSNET_COMPILING_E
  ds_cmd_install("hbp", "[pc|da|dr|dw][uskx]*[:<adr>[,<msk>]]...", "set hardware breakpoint", hbp_cmd);
#elif DSNET_COMPILING_I
  ds_cmd_install("hbp", "[pc|da|dr|dw][ku]*[:<adr>[,<msk>]]...", "set hardware breakpoint", hbp_cmd);
#endif /* DSNET_COMPILING_I */
  ds_cmd_install("hub", "[pc|da|dr|dw]...", "remove hardware breakpoint", hbp_cmd);
#ifdef DSNET_COMPILING_E
  ds_cmd_install("run", "[<fname> [<args>]...]", "run program", run_cmd);
#endif /* DSNET_COMPILING_E */
  ds_cmd_install("cont", "[<cnt>]", "continue program", cont_cmd);
  ds_cmd_install("step", "[<cnt>]", "step instruction", step_cmd);
  ds_cmd_install("next", "[<cnt>]", "next instruction", next_cmd);
  ds_cmd_install("lstep", "[<cnt>]", "step source line", lstep_cmd);
  ds_cmd_install("lnext", "[<cnt>]", "next source line", lnext_cmd);
  ds_cmd_install("luntil", "<line>", "continue to specified source line with temp-bp", luntil_cmd);
  ds_cmd_install("until", "<adr>...", "continue to specified address with temp-bp", until_cmd);
  ds_cmd_install("break", (char *)"", "break program", break_cmd);
  ds_cmd_install("wait", (char *)"", "wait program", wait_cmd);
  ds_cmd_install("reset", "[-i] [<ebootp> [<ibootp>]]", "reset target", reset_cmd);
#ifdef DSNET_COMPILING_E
  ds_cmd_install("xgkt", "<fname> <cnt> [<off>]", "start XGKICK trace", xgkt_cmd);
  ds_cmd_install("dbgctl", "{vu0|vu1} {on|off}", "debug control", dbgctl_cmd);
  ds_cmd_install("storeimage", "<fname> <bp> <bw> <psm> <x> <y> <w> <h>", "store GS image", storeimage_cmd);
#endif /* DSNET_COMPILING_E */
  ds_cmd_install("bpfunc", "<adr>", "set break point function", bpfunc_cmd);
  ds_add_help(0, dbg_help);
  ds_help_completion_func = dbg_help_completion;
  ds_add_show_arg("dbconf", show_dbconf);
  ds_add_show_arg("symbol", show_symbol);
  ds_add_show_arg("mdebug", show_mdebug);
  ds_cmd_standard_install(1);
  ds_rdwr_mem_align_func = rdwr_mem_align;
  ds_load_mem_func = load_mem;
  ds_store_mem_func = store_mem;
  ds_load_quad_reg_func = load_quad_reg;
  ds_store_quad_reg_func = store_quad_reg;
  ds_symbol_to_value_func = symbol_to_value;
  ds_symbol_completion_func = symbol_completion;
  ds_check_reserved_name_func = check_reserved_name;
  if ( port_name )
  {
    if ( !ds_listen_net(port_name, start_console) )
      ds_exit(133);
    ds_disp_proto = 1041;
  }
  send_netmp_connect_request();
  dsm_waiting = 1;
  to_sec = 120;
  to_usec = 0;
  ds_drfp_err_func = drfp_error;
  while ( 1 )
  {
    r = ds_select_desc(to_sec, to_usec);
    if ( r < 0 )
      return ds_exit(0);
    if ( !r )
    {
      if ( dsm_waiting == 5 )
      {
LABEL_76:
        ds_exit(146);
LABEL_77:
        ds_exit(147);
      }
      else if ( dsm_waiting > 5 )
      {
        if ( dsm_waiting == 11 )
          goto LABEL_77;
      }
      else if ( dsm_waiting == 1 )
      {
        ds_exit(136);
        goto LABEL_76;
      }
    }
  }
}

int IsSupported(int MajorVersion, int MinorVersion)
{
  return MajorVersion == dbconf.major_ver && MinorVersion <= dbconf.minor_ver || MajorVersion < dbconf.major_ver;
}
