
#include "dsxdb_prototypes.h"

static const char *thread_state_str[] =
{
  "",
  "Run",
  "Ready",
  "",
  "Wait",
  "",
  "",
  "",
  "Suspend",
  "",
  "",
  "",
  "WaitSuspend",
  "",
  "",
  "",
  "Dormant"
};
static const char *register_state_str[] =
{
  "zero",
  "at",
  "v0",
  "v1",
  "a0",
  "a1",
  "a2",
  "a3",
  "t0",
  "t1",
  "t2",
  "t3",
  "t4",
  "t5",
  "t6",
  "t7",
  "s0",
  "s1",
  "s2",
  "s3",
  "s4",
  "s5",
  "s6",
  "s7",
  "t8",
  "t9",
  "k0",
  "k1",
  "gp",
  "sp",
  "fp",
  "ra"
};
static const char *intr_state_strs[] =
{
  "INTC_GS",
  "INTC_SBUS",
  "INTC_VBLANK_S",
  "INTC_VBLANK_E",
  "INTC_VIF0",
  "INTC_VIF1",
  "INTC_VU0",
  "INTC_VU1",
  "INTC_IPU",
  "INTC_TIM0",
  "INTC_TIM1",
  "INTC_11",
  "INTC_12",
  "INTC_13",
  "INTC_14",
  "INTC_15"
};
static const char *dmac_state_strs[] =
{
  "DMAC_VIF0",
  "DMAC_VIF1",
  "DMAC_GIF",
  "DMAC_FROM_IPU",
  "DMAC_TO_IPU",
  "DMAC_SIF0",
  "DMAC_SIF1",
  "DMAC_SIF2",
  "DMAC_FROM_SPR",
  "DMAC_TO_SPR",
  "DMAC_10",
  "DMAC_11",
  "DMAC_12",
  "DMAC_CIS",
  "DMAC_MEIS",
  "DMAC_BEIS"
};

static int mode_4 = 1;
static int code_5 = 63;
static int fmode_7 = 1;
static int bmode_8 = 0;
static int code_17 = 1;
static int sid_18 = 0;
static int mode_19 = 1;

static int isnumstr(char *str);

static int isnumstr(char *str)
{
  while ( *str > 47 && *str <= 57 )
    ++str;
  return *str == 0;
}

int dt_cmd(int ac, char **av)
{
  int i_3; // [esp+0h] [ebp-28h]
  BT_REG br; // [esp+4h] [ebp-24h] BYREF
  DBGP_EE_THREADLIST_DATA *p_result; // [esp+10h] [ebp-18h]
  unsigned int pv; // [esp+14h] [ebp-14h] BYREF
  int cnt; // [esp+18h] [ebp-10h]
  DBGP_HDR *tcbhdr; // [esp+1Ch] [ebp-Ch]
  DBGP_HDR *listhdr; // [esp+20h] [ebp-8h]
  int r; // [esp+24h] [ebp-4h]
  int aca; // [esp+30h] [ebp+8h]
  char **ava; // [esp+34h] [ebp+Ch]
  char *c;

  static int tid = 0;

  r = 0;
  if ( !IsSupported(3, 10) )
    return ds_error("dt command is required DBGP version %d.%d later.", 3, 10);
  if ( ac )
  {
    cnt = 0;
    mode_4 = 1;
    code_5 = 0;
    bmode_8 = 0;
    if ( ac <= 1 )
    {
      mode_4 = 1;
      code_5 = 63;
    }
    else
    {
      aca = ac - 1;
      for ( ava = av + 1; aca > 0 && **ava == 45; ++ava )
      {
        for ( c = (*ava + 1); c; ++c )
        {
          switch ( *c )
          {
            case 'a':
              LOBYTE(code_5) = code_5 | 0x3F;
              ++cnt;
              break;
            case 'b':
              LOBYTE(mode_4) = mode_4 | 0x80;
              break;
            case 'c':
              LOBYTE(code_5) = code_5 | 0x80;
              break;
            case 'd':
              LOBYTE(code_5) = code_5 | 0x10;
              ++cnt;
              break;
            case 'e':
              LOBYTE(code_5) = code_5 | 1;
              ++cnt;
              break;
            case 'f':
              fmode_7 = 1;
              break;
            case 'h':
              fmode_7 = 0;
              break;
            case 'q':
              ++cnt;
              break;
            case 'r':
              LOBYTE(code_5) = code_5 | 2;
              ++cnt;
              break;
            case 's':
              LOBYTE(code_5) = code_5 | 8;
              ++cnt;
              break;
            case 'u':
              LOBYTE(code_5) = code_5 | 0x20;
              ++cnt;
              break;
            case 'v':
              LOBYTE(mode_4) = mode_4 | 3;
              break;
            case 'w':
              LOBYTE(code_5) = code_5 | 4;
              ++cnt;
              break;
            default:
              return ds_error("Usage: dt [-[abcdefhrqsuvw]*] [<tid>]");
          }
        }
        --aca;
      }
      if ( !cnt )
        LOBYTE(code_5) = code_5 | 0x3F;
      if ( aca > 0 )
      {
        unsigned int val;

        if ( !isnumstr(*ava) )
          return ds_error("Usage: dt [-[abcdefhrqsuvw]*] [<tid>]");
        if ( ds_scan_digit_word(*ava, &val) )
          return -1;
        if ( val > 0xFF )
          return ds_error("Thread id must be smaller than %d.", 256);
        tid = val;
        mode_4 &= 0xFFFFFFFC;
        LOBYTE(mode_4) = mode_4 | 2;
      }
    }
  }
  listhdr = (DBGP_HDR *)ds_alloc(13316 + sizeof(DBGP_HDR));
  if ( !listhdr )
    return -1;
  listhdr->code = code_5;
  r = get_thread_list(listhdr);
  if ( !r )
  {
    if ( (mode_4 & 2) != 0 )
    {
      cnt = (int)&listhdr[1];
      p_result = (DBGP_EE_THREADLIST_DATA *)&listhdr[1].result;
      tcbhdr = (DBGP_HDR *)ds_alloc(768 + sizeof(DBGP_HDR));
      if ( !tcbhdr )
      {
        ds_free(listhdr);
        return -1;
      }
      if ( (mode_4 & 1) != 0 )
      {
        for ( pv = 0; *(_DWORD *)cnt > pv; ++pv )
        {
          r = get_tcb(tcbhdr, p_result->id);
          if ( !r )
          {
            disp_tcb(tcbhdr, fmode_7);
            if ( (mode_4 & 0x80u) != 0 )
            {
              if ( tcbhdr->count )
              {
                br.pc = p_result->pc;
                br.sp = p_result->sp;
                br.ra = p_result->ra;
                disp_bt(&br, 0);
              }
            }
          }
          ++p_result;
        }
      }
      else
      {
        r = get_tcb(tcbhdr, tid);
        if ( !r )
        {
          disp_tcb(tcbhdr, fmode_7);
          if ( (mode_4 & 0x80u) != 0 )
          {
            if ( tcbhdr->count )
            {
              br.pc = *(_DWORD *)&tcbhdr[4].result;
              br.sp = *(_DWORD *)&tcbhdr[69].id;
              br.ra = *(_DWORD *)&tcbhdr[73].id;
              disp_bt(&br, 0);
            }
          }
        }
      }
      ds_free(tcbhdr);
    }
    else
    {
      p_result = (DBGP_EE_THREADLIST_DATA *)&listhdr[1];
      c = &listhdr[1].result;
      ds_printf("tid prio      pc       sp      status cause sid wakeup count function\n");
      for ( i_3 = 0; p_result->id > i_3; ++i_3 )
      {
        disp_thread_list((DBGP_EE_THREADLIST_DATA *)c);
        if ( (mode_4 & 0x80u) != 0 )
        {
          br.pc = *(_DWORD *)(c + 28);
          br.sp = *(_DWORD *)(c + 32);
          br.ra = *(_DWORD *)(c + 40);
          disp_bt(&br, 0);
        }
        c += 52;
      }
    }
  }
  ds_free(listhdr);
  return 0;
}

void disp_tcb(DBGP_HDR *hdr, int fmode)
{
  float fpr; // [esp+Ch] [ebp-4DCh] BYREF
  char buf[1024]; // [esp+10h] [ebp-4D8h] BYREF
  DBGP_EE_THREADTCB_DATA *tcb; // [esp+410h] [ebp-D8h]
  int i; // [esp+414h] [ebp-D4h]
  char *gpr[32]; // [esp+418h] [ebp-D0h] BYREF
  char *cause[3]; // [esp+498h] [ebp-50h]
  char *status[17]; // [esp+4A4h] [ebp-44h] BYREF

  qmemcpy(status, thread_state_str, sizeof(status));
  cause[0] = "";
  cause[1] = "Sleep";
  cause[2] = "Sema";
  qmemcpy(gpr, register_state_str, sizeof(gpr));
  if ( hdr->count )
  {
    tcb = (DBGP_EE_THREADTCB_DATA *)&hdr[1];
    ds_printf(
      "<<tid %3d  prio %3d count %d status [%s] ",
      *(_DWORD *)&hdr[1].id,
      *(_DWORD *)&hdr[1].result,
      *(_DWORD *)&hdr[4].id,
      status[*(_DWORD *)&hdr[2].id]);
    if ( (*(_DWORD *)&hdr[2].id & 4) != 0 )
    {
      if ( tcb->cause == 2 )
        ds_printf("cause [%s] sid %3d", cause[tcb->cause], tcb->waitid);
      else
        ds_printf("cause [%s]", cause[tcb->cause]);
    }
    ds_printf(" wakeup %d>>\n", tcb->wakeupcount);
    address_to_symbol(buf, tcb->programcounter, 0);
    ds_printf("pc    = %08x  ", tcb->programcounter);
    if ( buf[0] )
      ds_printf("<%s>", buf);
    ds_printf("\n");
    address_to_symbol(buf, tcb->func, 0);
    ds_printf("func  = %08x  ", tcb->func);
    if ( buf[0] )
      ds_printf("<%s>", buf);
    ds_printf("\n");
    ds_printf("args  = %08x  argc = %08x  end of heap = %08x\n", tcb->args, tcb->argc, tcb->endofheap);
    ds_printf("stack = %08x  stackpointer = %08x  stacksize = %08x\n", tcb->stack, tcb->stackpointer, tcb->stacksize);
    if ( fmode == 1 )
    {
      for ( i = 0; i <= 25; ++i )
      {
        memcpy(&fpr, &tcb->fpr[i], sizeof(fpr));
        ds_printf(
          "%4s($%02d) = %08x_%08x_%08x_%08x   $fpr%-2d = %+g\n",
          gpr[i],
          i,
          tcb->gpr[i].xa[3],
          tcb->gpr[i].xa[2],
          tcb->gpr[i].xa[1],
          tcb->gpr[i].xa[0],
          i,
          fpr);
      }
      for ( i = 26; i <= 29; ++i )
      {
        memcpy(&fpr, &tcb->fpr[i], sizeof(fpr));
        ds_printf(
          "%4s($%02d) = %08x_%08x_%08x_%08x   $fpr%-2d = %+g\n",
          gpr[i + 2],
          i + 2,
          tcb->gpr[i + 2].xa[3],
          tcb->gpr[i].xa[2],
          tcb->gpr[i + 2].xa[1],
          tcb->gpr[i + 2].xa[0],
          i,
          fpr);
      }
      memcpy(&fpr, &tcb->fpr[30], sizeof(fpr));
      ds_printf(
        "hi  = %08x_%08x  lo  = %08x_%08x  $fpr%-2d = %+g\n",
        tcb->hi.xa[1],
        tcb->hi.xa[0],
        tcb->lo.xa[1],
        tcb->lo.xa[0],
        30,
        fpr);
      memcpy(&fpr, &tcb->fpr[31], sizeof(fpr));
      ds_printf(
        "hi1 = %08x_%08x  lo1 = %08x_%08x  $fpr%-2d = %+g\n",
        tcb->hi1.xa[1],
        tcb->hi1.xa[0],
        tcb->lo1.xa[1],
        tcb->lo1.xa[0],
        31,
        fpr);
      memcpy(&fpr, &tcb->facc, sizeof(fpr));
      ds_printf("fcr31 = %08x  facc = %+g  sa = %08x\n", tcb->fcr31, fpr, tcb->sa);
    }
    else
    {
      for ( i = 0; i <= 25; ++i )
        ds_printf(
          "%4s($%02d) = %08x_%08x_%08x_%08x   $fpr%-2d = %08x\n",
          gpr[i],
          i,
          tcb->gpr[i].xa[3],
          tcb->gpr[i].xa[2],
          tcb->gpr[i].xa[1],
          tcb->gpr[i].xa[0],
          i,
          tcb->fpr[i]);
      for ( i = 26; i <= 29; ++i )
        ds_printf(
          "%4s($%02d) = %08x_%08x_%08x_%08x   $fpr%-2d = %08x\n",
          gpr[i + 2],
          i + 2,
          tcb->gpr[i + 2].xa[3],
          tcb->gpr[i].xa[2],
          tcb->gpr[i + 2].xa[1],
          tcb->gpr[i + 2].xa[0],
          i,
          tcb->fpr[i]);
      ds_printf(
        "hi  = %08x_%08x  lo  = %08x_%08x  $fpr%-2d = %08x\n",
        tcb->hi.xa[1],
        tcb->hi.xa[0],
        tcb->lo.xa[1],
        tcb->lo.xa[0],
        30,
        tcb->fpr[30]);
      ds_printf(
        "hi1 = %08x_%08x  lo1 = %08x_%08x  $fpr%-2d = %08x\n",
        tcb->hi1.xa[1],
        tcb->hi1.xa[0],
        tcb->lo1.xa[1],
        tcb->lo1.xa[0],
        31,
        tcb->fpr[31]);
      ds_printf("fcr31 = %08x  facc = %08x  sa = %08x\n", tcb->fcr31, tcb->facc, tcb->sa);
    }
  }
}

void disp_thread_list_all(DBGP_HDR *hdr)
{
  ;
}

void disp_thread_list(DBGP_EE_THREADLIST_DATA *pdata)
{
  char buf[1024]; // [esp+8h] [ebp-454h] BYREF
  int j; // [esp+408h] [ebp-54h]
  char *cause[3]; // [esp+40Ch] [ebp-50h]
  char *status[17]; // [esp+418h] [ebp-44h] BYREF

  qmemcpy(status, thread_state_str, sizeof(status));
  cause[0] = "";
  cause[1] = "Sleep";
  cause[2] = "Sema";
  ds_printf("%3d %3d %08x %08x ", pdata->id, pdata->priority, pdata->pc, pdata->sp);
  if ( (pdata->status & 4) != 0 )
  {
    if ( pdata->cause == 2 )
      ds_printf("%11s %5s %3d ", status[pdata->status], cause[pdata->cause], pdata->waitid);
    else
      ds_printf("%11s %5s     ", status[pdata->status], cause[pdata->cause]);
  }
  else
  {
    ds_printf("%11s           ", status[pdata->status]);
  }
  address_to_symbol(buf, pdata->func, 0);
  ds_printf("%2d %9d ", pdata->wakeupcount, pdata->counts);
  for ( j = strlen(buf); j > 0 && buf[j - 1] != 44; --j )
    ;
  if ( buf[0] )
    ds_printf("<%s>", &buf[j]);
  ds_printf("\n");
}

int ds_cmd(int ac, char **av)
{
  int j_3; // [esp+0h] [ebp-20h]
  unsigned int pv; // [esp+4h] [ebp-1Ch] BYREF
  DBGP_HDR *hdr; // [esp+8h] [ebp-18h]
  DBGP_EE_THREADID_DATA *tid; // [esp+Ch] [ebp-14h]
  DBGP_EE_SEMABLOCK_DATA *data; // [esp+10h] [ebp-10h]
  DBGP_EE_SEMABLOCK_HDR *header; // [esp+14h] [ebp-Ch]
  int r; // [esp+18h] [ebp-8h]
  int i_3; // [esp+1Ch] [ebp-4h]
  int aca; // [esp+28h] [ebp+8h]
  char **ava; // [esp+2Ch] [ebp+Ch]

  r = 0;
  if ( !IsSupported(3, 10) )
    return ds_error("ds command is required DBGP version %d.%d later.", 3, 10);
  if ( ac )
  {
    mode_19 = 1;
    code_17 = 1;
    aca = ac - 1;
    for ( ava = av + 1; aca > 0 && **ava == 45; ++ava )
    {
      for ( pv = (unsigned int)(*ava + 1); *(_BYTE *)pv; ++pv )
      {
        if ( *(_BYTE *)pv != 118 )
          return ds_error("Usage: ds [-v] [<sid>]");
        mode_19 = 2;
        code_17 = 1;
        sid_18 = 0;
      }
      --aca;
    }
    if ( aca > 0 )
    {
      if ( ds_scan_digit_word(*ava, &pv) )
        return -1;
      if ( pv > 0xFF )
        return ds_error("Sema id must be smaller than %d.", 256);
      sid_18 = pv;
      mode_19 = 2;
      code_17 = 0;
    }
  }
  hdr = (DBGP_HDR *)ds_alloc(7172 + sizeof(DBGP_HDR));
  if ( !hdr )
    return -1;
  hdr->code = code_17;
  r = get_semablock(hdr, sid_18);
  if ( !r && hdr->count )
  {
    header = (DBGP_EE_SEMABLOCK_HDR *)&hdr[1];
    data = (DBGP_EE_SEMABLOCK_DATA *)&hdr[1].result;
    for ( i_3 = 0; header->count > i_3; ++i_3 )
    {
      ds_printf(
        "sid %3d count %3d maxcount %3d attr %08x option %08x numwait %3d\n",
        data->id,
        data->count,
        data->maxcount,
        data->attr,
        data->option,
        data->numWaitThreads);
      pv = data->numWaitThreads;
      tid = (DBGP_EE_THREADID_DATA *)++data;
      for ( j_3 = 0; (int)pv > j_3; ++j_3 )
      {
        if ( mode_19 != 1 )
          ds_printf("tid %3d\n", tid->id);
        ++tid;
      }
      data = (DBGP_EE_SEMABLOCK_DATA *)tid;
    }
  }
  ds_free(hdr);
  return 0;
}

int intr_cmd(int ac, char **av)
{
  int enable; // [esp+10h] [ebp-494h]
  char *dmac[16]; // [esp+14h] [ebp-490h] BYREF
  char *intc[16]; // [esp+54h] [ebp-450h] BYREF
  char buf[1024]; // [esp+94h] [ebp-410h] BYREF
  DBGP_HDR *hdr; // [esp+494h] [ebp-10h]
  DBGP_EE_HANDLER_DATA *pdata; // [esp+498h] [ebp-Ch]
  int r; // [esp+49Ch] [ebp-8h]
  int i; // [esp+4A0h] [ebp-4h]

  r = 0;
  qmemcpy(intc, intr_state_strs, sizeof(intc));
  qmemcpy(dmac, dmac_state_strs, sizeof(dmac));
  if ( !IsSupported(3, 10) )
    return ds_error("intr command is required DBGP version %d.%d later.", 3, 10);
  if ( ac > 1 )
    return ds_error("Usage: intr");
  hdr = (DBGP_HDR *)ds_alloc(5120 + sizeof(DBGP_HDR));
  if ( !hdr )
    return -1;
  r = get_handlerlist(hdr);
  if ( !r )
  {
    pdata = (DBGP_EE_HANDLER_DATA *)&hdr[1];
    ds_printf("handler list\n");
    for ( i = 0; i < hdr->count; ++i )
    {
      enable = pdata->mask & (1 << pdata->number);
      address_to_symbol(buf, pdata->func, 0);
      if ( pdata->type )
        ds_printf("%-13s ", dmac[pdata->number]);
      else
        ds_printf("%-13s ", intc[pdata->number]);
      if ( enable )
        ds_printf("ON ");
      else
        ds_printf("OFF");
      ds_printf(" func = %08x ", pdata->func);
      if ( buf[0] )
        ds_printf("<%s>", buf);
      ds_printf(" arg = %08x\n", pdata->arg);
      ++pdata;
    }
  }
  ds_free(hdr);
  return 0;
}

