
#include "dsxdb_prototypes.h"

extern DBGP_CONF_DATA dbconf; // defined in dbg.c
#ifdef DSNET_COMPILING_E
extern unsigned int current_entry_point; // defined in mem.c
extern unsigned int current_gp_value; // defined in mem.c
#endif /* DSNET_COMPILING_E */

#ifdef DSNET_COMPILING_E
static int run_argc = 0;
static char *run_args = NULL;
static int run_args_len = 0;
#endif /* DSNET_COMPILING_E */

static struct {BRKPT *head;BRKPT *tail;} bps;

static int __cdecl get_target_brkpt(int flag);
static int __cdecl put_target_brkpt(int flag);
static int __cdecl store_breakpoints(int flag);
static int __cdecl restore_breakpoints(int flag);
static void __cdecl print_brkpt(BRKPT *bp, char *msg);
static int __cdecl add_brkpt(int flag, unsigned int adr, unsigned int cnt);
static int __cdecl del_brkpt(int flag, unsigned int adr);
static void __cdecl automatic_prefix_breakpoint(char *dp, char *sp);
static int __cdecl targ_cont_cmd(int ac, char **av);
static int __cdecl targ_until_cmd(int ac, char **av);
static int __cdecl targ_step_cmd(int ac, char **av);
static int __cdecl targ_next_cmd(int ac, char **av);
static int __cdecl targ_lstep_cmd(int ac, char **av);
static int __cdecl targ_lnext_cmd(int ac, char **av);
static int __cdecl targ_luntil_cmd(int ac, char **av);
static unsigned int __cdecl get_pc(SCRS *scrs);
static int __cdecl is_breakpoint(int flag, SCRS *scrs);
static int __cdecl load_scrs(SCRS *scrs);
static int __cdecl do_cont(int flag, SCRS *scrs);
static int __cdecl host_do_step(int f_next, SCRS *scrs);
static int __cdecl do_lstep(int f_next, SCRS *scrs);
static int __cdecl host_cont_cmd(int ac, char **av);
static int __cdecl host_until_cmd(int ac, char **av);
static int __cdecl host_step_cmd(int ac, char **av);
static int __cdecl host_next_cmd(int ac, char **av);
static int __cdecl host_lstep_cmd(int ac, char **av);
static int __cdecl host_lnext_cmd(int ac, char **av);
static int __cdecl host_luntil_cmd(int ac, char **av);
static int __cdecl exitc(int r);

static int __cdecl get_target_brkpt(int flag)
{
  int nc; // [esp+0h] [ebp-14h] BYREF
  int i; // [esp+4h] [ebp-10h]
  int r; // [esp+8h] [ebp-Ch]
  BRKPT *bp; // [esp+Ch] [ebp-8h]
  DBGP_BRKPT_DATA *cbps; // [esp+10h] [ebp-4h]

  cbps = 0;
  r = -1;
  if ( !dbconf.nbrkpt )
    return ds_error("dbconf.nbrkpt is zero");
  cbps = (DBGP_BRKPT_DATA *)ds_alloc_mem_low("exe.c", "get_target_brkpt", sizeof(DBGP_BRKPT_DATA) * dbconf.nbrkpt);
  if ( !cbps )
    return -1;
  if ( get_brkpt(cbps, &nc) >= 0 )
  {
    for ( bp = bps.head; bp; bp = bp->forw )
    {
      if ( ((LOBYTE(bp->flag) ^ (unsigned __int8)flag) & 0x1C) == 0 && (bp->flag & 0x108) != 8 )
      {
        for ( i = 0; nc > i; ++i )
        {
          if ( bp->adr == cbps[i].adr )
            bp->curr_cnt = cbps[i].cnt;
        }
      }
    }
    r = 0;
  }
  if ( cbps )
    ds_free_mem_low(cbps, "exe.c", "get_target_brkpt");
  return r;
}

static int __cdecl put_target_brkpt(int flag)
{
  int init_cnt; // ecx
  int nr; // [esp+4h] [ebp-14h]
  int v4; // [esp+8h] [ebp-10h]
  int r; // [esp+Ch] [ebp-Ch]
  BRKPT *bp; // [esp+10h] [ebp-8h]
  BRKPT *bp_1; // [esp+10h] [ebp-8h]
  DBGP_BRKPT_DATA *rbps; // [esp+14h] [ebp-4h]

  r = -1;
  nr = 0;
  for ( bp = bps.head; bp; bp = bp->forw )
  {
    if ( ((LOBYTE(bp->flag) ^ (unsigned __int8)flag) & 0x1C) == 0 && (bp->flag & 0x108) != 8 )
    {
      if ( (bp->flag & 0x200) != 0 )
      {
        if ( !bp->init_cnt )
          continue;
      }
      else if ( !bp->curr_cnt )
      {
        continue;
      }
      ++nr;
    }
  }
  if ( nr > dbconf.nbrkpt )
    return ds_error("too many breakpoints (%d < %d)", dbconf.nbrkpt, nr);
  rbps = (DBGP_BRKPT_DATA *)ds_alloc_mem_low("exe.c", "put_target_brkpt", sizeof(DBGP_BRKPT_DATA) * nr);
  if ( !rbps )
    return -1;
  v4 = 0;
  bp_1 = bps.head;
  while ( 2 )
  {
    if ( bp_1 )
    {
      if ( ((LOBYTE(bp_1->flag) ^ (unsigned __int8)flag) & 0x1C) == 0 && (bp_1->flag & 0x108) != 8 )
      {
        if ( (bp_1->flag & 0x200) != 0 )
        {
          if ( bp_1->init_cnt )
          {
LABEL_23:
            rbps[v4].adr = bp_1->adr;
            if ( (bp_1->flag & 0x200) != 0 )
              init_cnt = bp_1->init_cnt;
            else
              init_cnt = bp_1->curr_cnt;
            rbps[v4++].cnt = init_cnt;
          }
        }
        else if ( bp_1->curr_cnt )
        {
          goto LABEL_23;
        }
      }
      bp_1 = bp_1->forw;
      continue;
    }
    break;
  }
  if ( put_brkpt(rbps, nr) >= 0 )
    r = 0;
  ds_free_mem_low(rbps, "exe.c", "put_target_brkpt");
  return r;
}

static int __cdecl store_breakpoints(int flag)
{
  int v2; // ecx
  int v3; // ecx
  unsigned int break_ins; // [esp+0h] [ebp-8h] BYREF
  BRKPT *bp; // [esp+4h] [ebp-4h]

  break_ins = 65549;
  if ( is_target_exec_ctl() )
    return put_target_brkpt(flag);
  for ( bp = bps.head; bp; bp = bp->forw )
  {
    if ( ((LOBYTE(bp->flag) ^ (unsigned __int8)flag) & 0x1C) == 0 && (bp->flag & 0x108) != 8 )
    {
      if ( ds_load_mem(bp->adr, &bp->ins, 4) )
        return -1;
      v2 = bp->flag;
      LOBYTE(v2) = v2 | 1;
      bp->flag = v2;
      if ( ds_store_mem(bp->adr, &break_ins, 4) )
        return -1;
      v3 = bp->flag;
      LOBYTE(v3) = v3 | 2;
      bp->flag = v3;
    }
  }
  return 0;
}

static int __cdecl restore_breakpoints(int flag)
{
  int v2; // ecx
  unsigned int ins; // [esp+4h] [ebp-8h] BYREF
  BRKPT *bp; // [esp+8h] [ebp-4h]

  if ( is_target_exec_ctl() )
    return get_target_brkpt(flag);
  for ( bp = bps.head; bp; bp = bp->forw )
  {
    if ( ((LOBYTE(bp->flag) ^ (unsigned __int8)flag) & 0x1C) == 0 && (bp->flag & 0x108) != 8 )
    {
      if ( (bp->flag & 2) != 0 )
      {
        if ( ds_load_mem(bp->adr, &ins, 4) )
          return -1;
        if ( ins == 65549 )
        {
          if ( (bp->flag & 1) != 0 )
          {
            if ( ds_store_mem(bp->adr, &bp->ins, 4) )
              return -1;
            v2 = bp->flag;
            LOBYTE(v2) = v2 & 0xFD;
            bp->flag = v2;
          }
          else
          {
            ds_printf("restore_breakpoints: VINS=0 ??\n");
          }
        }
        else
        {
          ds_printf("restore_breakpoints: not break_ins ??\n");
        }
      }
      else
      {
        ds_printf("restore_breakpoints: STORE=0 ??\n");
      }
    }
  }
  return 0;
}

int __cdecl store_user_breakpoints()
{
  return store_breakpoints(8);
}

int __cdecl restore_user_breakpoints()
{
  return restore_breakpoints(8);
}

int __cdecl eval_bp_reg(char *name, unsigned int *padr)
{
  unsigned int wv; // [esp+0h] [ebp-8h] BYREF
  BRKPT *bp; // [esp+4h] [ebp-4h]

  if ( ds_strncmp("$BP", name, 3) || ds_scan_digit_word(name + 3, &wv) )
    return ds_error("invalid breakpoint name - %s", name);
  if ( wv )
  {
    for ( bp = bps.head; bp; bp = bp->forw )
    {
      if ( bp->no == wv )
      {
        *padr = bp->adr;
        return 0;
      }
    }
  }
  return ds_error("breakpoint not found - %s", name);
}

static void __cdecl print_brkpt(BRKPT *bp, char *msg)
{
  const char *v2; // eax
  char buf[1024]; // [esp+0h] [ebp-400h] BYREF

  if ( (bp->flag & 8) != 0 )
  {
    ds_printf(" $BP%d=0x%W", bp->no, bp->adr);
    if ( address_to_symbol(buf, bp->adr, 0) )
      ds_printf(" <%s>", buf);
    ds_printf(" init=0x%x", bp->init_cnt);
    ds_printf(" curr=0x%x", bp->curr_cnt);
    if ( (bp->flag & 0x100) != 0 )
      v2 = "en";
    else
      v2 = "dis";
    ds_printf(" # %sabled", v2);
    if ( (bp->flag & 0x200) != 0 )
      ds_printf(", auto-init");
    if ( msg )
      ds_printf(" (%s)", msg);
    ds_printf("\n");
  }
}

static int __cdecl add_brkpt(int flag, unsigned int adr, unsigned int cnt)
{
  int v3; // ebx
  int v4; // esi
  int v5; // edx
  int v6; // edx
  int v8; // edx
  int v9; // ebx
  int v10; // edx
  BRKPT *tail; // edx
  int v12; // [esp+Ch] [ebp-8h]
  BRKPT *bp; // [esp+10h] [ebp-4h]
  BRKPT *bp_1; // [esp+10h] [ebp-4h]
  static int brkpt_no = 1;

  for ( bp = bps.head; bp; bp = bp->forw )
  {
    if ( ((LOBYTE(bp->flag) ^ (unsigned __int8)flag) & 0x1C) == 0 && bp->adr == adr )
    {
      bp->curr_cnt = cnt;
      bp->init_cnt = cnt;
      v3 = bp->flag;
      BYTE1(v3) &= ~2u;
      bp->flag = v3;
      v12 = flag & 0x200;
      v4 = bp->flag;
      if ( (flag & 8) != 0 )
      {
        v5 = bp->flag;
        BYTE1(v5) = BYTE1(v4) | 1;
        v6 = v12 | v5;
      }
      else
      {
        v6 = v4 | v12;
      }
      bp->flag = v6;
      print_brkpt(bp, 0);
      return 0;
    }
  }
  bp_1 = (BRKPT *)ds_alloc_mem_low("exe.c", "add_brkpt", sizeof(BRKPT));
  if ( !bp_1 )
    return -1;
  v8 = flag;
  if ( (flag & 8) != 0 )
    BYTE1(v8) = BYTE1(flag) | 1;
  bp_1->flag = v8;
  v9 = bp_1->flag;
  BYTE1(v9) &= ~2u;
  bp_1->flag = v9;
  bp_1->flag |= flag & 0x200;
  if ( (flag & 8) != 0 )
    v10 = brkpt_no++;
  else
    v10 = 0;
  bp_1->no = v10;
  bp_1->adr = adr;
  bp_1->curr_cnt = cnt;
  bp_1->init_cnt = cnt;
  bp_1->ins = 0;
  tail = bps.tail;
  bp_1->back = bps.tail;
  if ( tail )
    bp_1->back->forw = bp_1;
  else
    bps.head = bp_1;
  bp_1->forw = 0;
  bps.tail = bp_1;
  print_brkpt(bp_1, 0);
  return 0;
}

static int __cdecl del_brkpt(int flag, unsigned int adr)
{
  int r; // [esp+8h] [ebp-Ch]
  BRKPT *bq; // [esp+Ch] [ebp-8h]
  BRKPT *bp; // [esp+10h] [ebp-4h]

  r = 0;
  for ( bp = bps.head; bp; bp = bq )
  {
    bq = bp->forw;
    if ( ((LOBYTE(bp->flag) ^ (unsigned __int8)flag) & 0x1C) == 0 && ((flag & 0x80u) != 0 || bp->adr == adr) )
    {
      if ( bp->forw )
        bp->forw->back = bp->back;
      else
        bps.tail = bp->back;
      if ( bp->back )
        bp->back->forw = bp->forw;
      else
        bps.head = bp->forw;
      ds_free_mem_low(bp, "exe.c", "del_brkpt");
      ++r;
    }
  }
  return r;
}

int __cdecl remove_breakpoints()
{
  int flag; // ecx
  BRKPT *bp; // [esp+0h] [ebp-4h]

  for ( bp = bps.head; bp; bp = bp->forw )
  {
    if ( (bp->flag & 8) != 0 )
    {
      flag = bp->flag;
      BYTE1(flag) &= ~1u;
      bp->flag = flag;
    }
  }
  del_brkpt(132, 0);
  del_brkpt(144, 0);
  return 0;
}

int __cdecl bp_cmd(int ac, char **av)
{
  const char *v3; // eax
  char *v4; // eax
  char *p; // [esp+0h] [ebp-418h]
  char *p_1; // [esp+0h] [ebp-418h]
  char buf[1024]; // [esp+4h] [ebp-414h] BYREF
  int flag; // [esp+404h] [ebp-14h]
  int n; // [esp+408h] [ebp-10h]
  unsigned int cnt; // [esp+40Ch] [ebp-Ch] BYREF
  unsigned int adr; // [esp+410h] [ebp-8h] BYREF
  BRKPT *bp; // [esp+414h] [ebp-4h]
  int aca; // [esp+420h] [ebp+8h]
  char **ava; // [esp+424h] [ebp+Ch]

  n = 0;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  ava = av + 1;
  if ( aca > 0 && **ava == 45 )
  {
    if ( is_target_exec_ctl() )
      v3 = "[,<cnt>]";
    else
      v3 = "";
    return ds_error("Usage: bp [<adr>%s]...", v3);
  }
  else if ( aca > 0 )
  {
    while ( aca > 0 )
    {
      for ( p = strcpy(buf, *ava); *p && *p != 44; ++p )
        ;
      if ( *p == 44 )
      {
        *p = 0;
        v4 = p + 1;
      }
      else
      {
        v4 = 0;
      }
      p_1 = v4;
      if ( ds_eval_word(buf, &adr) )
        return -1;
      if ( (adr & 3) != 0 )
        ds_error("invalid alignment (0x%W)", adr);
      flag = 520;
      cnt = 1;
      if ( p_1 )
      {
        if ( ds_eval_word(p_1, &cnt) )
          return -1;
        flag &= ~0x200u;
      }
      if ( add_brkpt(flag, adr, cnt) < 0 )
        return -1;
      --aca;
      ++ava;
    }
    return 0;
  }
  else
  {
    for ( bp = bps.head; bp; bp = bp->forw )
    {
      print_brkpt(bp, 0);
      ++n;
    }
    if ( !n )
      ds_printf("no breakpoint\n");
    return 0;
  }
}

static void __cdecl automatic_prefix_breakpoint(char *dp, char *sp)
{
  int v2; // eax
  char *p; // [esp+4h] [ebp-4h]

  if ( is_automatic_prefix_breakpoint() )
  {
    for ( p = sp; *p; ++p )
    {
      v2 = 0;
      if ( *p > 47 && *p <= 57 )
        v2 = 1;
      if ( !v2 )
        goto LABEL_8;
    }
    strcpy(dp, "$BP");
    strcpy(dp + 3, sp);
  }
  else
  {
LABEL_8:
    strcpy(dp, sp);
  }
}

int __cdecl ub_cmd(int ac, char **av)
{
  int v3; // [esp+0h] [ebp-408h]
  char buf[1024]; // [esp+4h] [ebp-404h] BYREF
  unsigned int adr; // [esp+404h] [ebp-4h] BYREF
  int aca; // [esp+410h] [ebp+8h]
  char **ava; // [esp+414h] [ebp+Ch]

  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  ava = av + 1;
  if ( aca > 0 && **ava == 45 )
    return ds_error("Usage: ub [all|<adr>]...");
  if ( aca > 0 )
  {
    while ( aca > 0 )
    {
      if ( !strcmp("all", *ava) )
        goto LABEL_10;
      automatic_prefix_breakpoint(buf, *ava);
      if ( ds_eval_word(buf, &adr) )
        return -1;
      if ( del_brkpt(8, adr) != 1 )
        ds_printf("Warning: adr=0x%W is not breakpoint\n", adr);
      --aca;
      ++ava;
    }
    return 0;
  }
  else if ( is_describe_ub_all() )
  {
    return ds_error("\"ub all\" to remove all");
  }
  else
  {
LABEL_10:
    v3 = del_brkpt(136, 0);
    ds_printf("%d breakpoint(s) removed\n", v3);
    return 0;
  }
}

int __cdecl be_cmd(int ac, char **av)
{
  const char *v3; // eax
  int v4; // ecx
  int flag; // ecx
  const char *v6; // eax
  char *v7; // [esp-4h] [ebp-414h]
  char buf[1024]; // [esp+0h] [ebp-410h] BYREF
  int n; // [esp+400h] [ebp-10h]
  int f_bd; // [esp+404h] [ebp-Ch]
  unsigned int adr; // [esp+408h] [ebp-8h] BYREF
  BRKPT *bp; // [esp+40Ch] [ebp-4h]
  int aca; // [esp+418h] [ebp+8h]
  char **ava; // [esp+41Ch] [ebp+Ch]

  f_bd = 0;
  n = 0;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  v7 = *av;
  ava = av + 1;
  if ( !strcmp("bd", v7) )
    ++f_bd;
  if ( aca > 0 && **ava == 45 )
  {
    if ( f_bd )
      v3 = "bd";
    else
      v3 = "be";
    return ds_error("Usage: %s [<adr>]...", v3);
  }
  else
  {
    if ( aca > 0 )
    {
      while ( aca > 0 )
      {
        automatic_prefix_breakpoint(buf, *ava);
        if ( ds_eval_word(buf, &adr) )
          return -1;
        for ( bp = bps.head; bp; bp = bp->forw )
        {
          if ( (bp->flag & 8) != 0 && bp->adr == adr )
          {
            ++n;
            flag = bp->flag;
            if ( f_bd )
              BYTE1(flag) &= ~1u;
            else
              BYTE1(flag) |= 1u;
            bp->flag = flag;
            break;
          }
        }
        if ( !bp )
        {
          ds_printf("Warning: ");
          ds_printf("adr=0x%W is not breakpoint\n", adr);
        }
        --aca;
        ++ava;
      }
    }
    else
    {
      for ( bp = bps.head; bp; bp = bp->forw )
      {
        if ( (bp->flag & 8) != 0 )
        {
          ++n;
          v4 = bp->flag;
          if ( f_bd )
            BYTE1(v4) &= ~1u;
          else
            BYTE1(v4) |= 1u;
          bp->flag = v4;
        }
      }
    }
    if ( f_bd )
      v6 = "dis";
    else
      v6 = "en";
    ds_printf("%d breakpoint(s) %sabled\n", n, v6);
    return 0;
  }
}

int __cdecl bd_cmd(int ac, char **av)
{
  return be_cmd(ac, av);
}

int __cdecl break_cmd(int ac, char **av)
{
  int r; // [esp+0h] [ebp-4h]

  if ( ac <= 0 )
    return 0;
  if ( ac != 1 )
    return ds_error("Usage: break");
  r = break_and_wait_halt();
  if ( r == 34 || r == 33 || r == 36 || r == 39 )
    ex_default_dr();
  return exitc(r);
}

int __cdecl wait_cmd(int ac, char **av)
{
  int r; // [esp+0h] [ebp-4h]

  if ( ac <= 0 )
    return 0;
  if ( ac != 1 )
    return ds_error("Usage: wait");
  r = wait_halt();
  if ( r == 34 || r == 33 || r == 36 || r == 39 )
    ex_default_dr();
  return exitc(r);
}

static int __cdecl targ_cont_cmd(int ac, char **av)
{
  int r; // [esp+0h] [ebp-4h]
  char **ava; // [esp+10h] [ebp+Ch]

  if ( ac > 0 )
  {
    --ac;
    ava = av + 1;
    if ( ac > 0 && **ava == 45 )
      return ds_error("Usage: cont");
  }
  if ( ac > 0 )
    return ds_error("Usage: cont");
  if ( !dbconf.ncont )
    return ds_error("dbconf.ncont is zero");
  if ( store_breakpoints(8) < 0 )
    return -1;
  r = cont_and_wait_halt(0, 1);
  restore_breakpoints(8);
  if ( r == 34 || r == 33 || r == 36 || r == 39 )
    ex_default_dr();
  return exitc(r);
}

static int __cdecl targ_until_cmd(int ac, char **av)
{
  int i; // [esp+0h] [ebp-Ch]
  int r; // [esp+4h] [ebp-8h]
  unsigned int adr; // [esp+8h] [ebp-4h] BYREF
  int aca; // [esp+14h] [ebp+8h]
  char **ava; // [esp+18h] [ebp+Ch]

  r = 0;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  ava = av + 1;
  if ( aca > 0 && **ava == 45 )
    return ds_error("Usage: until <adr>...");
  if ( !dbconf.ncont )
    return ds_error("dbconf.ncont is zero");
  for ( i = 0; aca > i; ++i )
  {
    if ( ds_eval_word(ava[i], &adr) )
      return -1;
    if ( (adr & 3) != 0 )
      return ds_error("invalid align");
    del_brkpt(144, 0);
    if ( add_brkpt(16, adr, 1u) < 0 )
      return -1;
    if ( store_breakpoints(16) < 0 )
      return -1;
    r = cont_and_wait_halt(0, 1);
    if ( r != 34 )
      break;
  }
  if ( r == 34 || r == 33 || r == 36 || r == 39 )
    ex_default_dr();
  return exitc(r);
}

static int __cdecl targ_step_cmd(int ac, char **av)
{
  int v2; // eax
  int r; // [esp+0h] [ebp-Ch]
  int n; // [esp+4h] [ebp-8h]
  int cnt; // [esp+8h] [ebp-4h] BYREF
  static int last_cnt = 1;

  r = 0;
  if ( ac > 0 )
    v2 = 1;
  else
    v2 = last_cnt;
  cnt = v2;
  if ( ac > 0 )
  {
    --ac;
    ++av;
    if ( ac > 0 && **av == 45 )
      return ds_error("Usage: step [<cnt>]");
  }
  if ( ac > 1 )
    return ds_error("Usage: step [<cnt>]");
  if ( ac > 0 && ds_eval_word(*av, (unsigned int *)&cnt) )
    return -1;
  last_cnt = cnt;
  while ( cnt > 0 )
  {
    n = cnt;
    if ( dbconf.nstep < cnt )
    {
      n = dbconf.nstep;
      if ( !dbconf.nstep )
        return ds_error("dbconf.nstep is zero");
    }
    if ( store_breakpoints(8) < 0 )
      return -1;
    r = cont_and_wait_halt(1, (unsigned __int8)n);
    restore_breakpoints(8);
    if ( r != 35 )
      break;
    cnt -= n;
  }
  if ( r == 35 || r == 34 || r == 33 || r == 36 || r == 39 )
    ex_default_dr();
  return exitc(r);
}

static int __cdecl targ_next_cmd(int ac, char **av)
{
  int v2; // eax
  int r; // [esp+0h] [ebp-Ch]
  int n; // [esp+4h] [ebp-8h]
  int cnt; // [esp+8h] [ebp-4h] BYREF
  static int last_cnt = 1;

  r = 0;
  if ( ac > 0 )
    v2 = 1;
  else
    v2 = last_cnt;
  cnt = v2;
  if ( ac > 0 )
  {
    --ac;
    ++av;
    if ( ac > 0 && **av == 45 )
      return ds_error("Usage: next [<cnt>]");
  }
  if ( ac > 1 )
    return ds_error("Usage: next [<cnt>]");
  if ( ac > 0 && ds_eval_word(*av, (unsigned int *)&cnt) )
    return -1;
  last_cnt = cnt;
  while ( cnt > 0 )
  {
    n = cnt;
    if ( dbconf.nnext < cnt )
    {
      n = dbconf.nnext;
      if ( !dbconf.nnext )
        return ds_error("dbconf.nnext is zero");
    }
    if ( store_breakpoints(8) < 0 )
      return -1;
    r = cont_and_wait_halt(2, (unsigned __int8)n);
    restore_breakpoints(8);
    if ( r != 35 )
      break;
    cnt -= n;
  }
  if ( r == 35 || r == 34 || r == 33 || r == 36 || r == 39 )
    ex_default_dr();
  return exitc(r);
}

static int __cdecl targ_lstep_cmd(int ac, char **av)
{
  int v3; // eax
  char *s2; // [esp+0h] [ebp-28h]
  char *file0; // [esp+4h] [ebp-24h]
  unsigned int v6; // [esp+8h] [ebp-20h] BYREF
  unsigned int loc; // [esp+Ch] [ebp-1Ch] BYREF
  int v8; // [esp+10h] [ebp-18h] BYREF
  int delta0; // [esp+14h] [ebp-14h] BYREF
  int line1; // [esp+18h] [ebp-10h] BYREF
  int line0; // [esp+1Ch] [ebp-Ch] BYREF
  int r; // [esp+20h] [ebp-8h]
  int cnt; // [esp+24h] [ebp-4h] BYREF
  static int last_cnt = 1;

  r = 0;
  if ( !dbconf.nstep )
    return ds_error("dbconf.nstep is zero");
  if ( ac > 0 )
    v3 = 1;
  else
    v3 = last_cnt;
  cnt = v3;
  if ( ac > 0 )
  {
    --ac;
    ++av;
    if ( ac > 0 && **av == 45 )
      return ds_error("Usage: lstep [<cnt>]");
  }
  if ( ac > 1 )
    return ds_error("Usage: lstep [<cnt>]");
  if ( ac > 0 && ds_eval_word(*av, (unsigned int *)&cnt) )
    return -1;
  if ( ds_eval_word("$_PC", &loc) )
    return -1;
  file0 = address_to_file_and_line(loc, &line0, 0, &delta0, 0);
  if ( !file0 || line0 <= 0 )
  {
    if ( is_lstep_stop_at_no_line() )
      return ds_error("*** No line for 0x%W", loc);
    file0 = "";
    line0 = 0;
  }
  last_cnt = cnt;
  while ( cnt > 0 )
  {
    do
    {
      while ( 1 )
      {
        if ( store_breakpoints(8) < 0 )
          return -1;
        r = cont_and_wait_halt(1, 1);
        restore_breakpoints(8);
        if ( r != 35 )
          goto LABEL_37;
        if ( ds_eval_word("$_PC", &v6) )
          return -1;
        s2 = address_to_file_and_line(v6, &line1, 0, &v8, 0);
        if ( s2 )
        {
          if ( line1 > 0 )
            break;
        }
        if ( is_lstep_stop_at_no_line() )
        {
          ds_error("*** No line for 0x%W", v6);
          ex_default_dr();
          return -1;
        }
        line1 = 0;
      }
    }
    while ( !strcmp(file0, s2) && line1 == line0 );
    file0 = s2;
    line0 = line1;
    delta0 = v8;
    r = 0;
    --cnt;
  }
LABEL_37:
  if ( r == 34 || r == 33 || r == 36 || r == 39 )
  {
    ex_default_dr();
  }
  else if ( !r )
  {
    lstep_default_list();
  }
  return exitc(r);
}

static int __cdecl targ_lnext_cmd(int ac, char **av)
{
  int v3; // eax
  char *s2; // [esp+0h] [ebp-28h]
  char *file0; // [esp+4h] [ebp-24h]
  unsigned int v6; // [esp+8h] [ebp-20h] BYREF
  unsigned int loc; // [esp+Ch] [ebp-1Ch] BYREF
  int v8; // [esp+10h] [ebp-18h] BYREF
  int delta0; // [esp+14h] [ebp-14h] BYREF
  int line1; // [esp+18h] [ebp-10h] BYREF
  int line0; // [esp+1Ch] [ebp-Ch] BYREF
  int r; // [esp+20h] [ebp-8h]
  int cnt; // [esp+24h] [ebp-4h] BYREF
  static int last_cnt = 1;

  r = 0;
  if ( !dbconf.nnext )
    return ds_error("dbconf.nnext is zero");
  if ( ac > 0 )
    v3 = 1;
  else
    v3 = last_cnt;
  cnt = v3;
  if ( ac > 0 )
  {
    --ac;
    ++av;
    if ( ac > 0 && **av == 45 )
      return ds_error("Usage: lnext [<cnt>]");
  }
  if ( ac > 1 )
    return ds_error("Usage: lnext [<cnt>]");
  if ( ac > 0 && ds_eval_word(*av, (unsigned int *)&cnt) )
    return -1;
  if ( ds_eval_word("$_PC", &loc) )
    return -1;
  file0 = address_to_file_and_line(loc, &line0, 0, &delta0, 0);
  if ( !file0 || line0 <= 0 )
  {
    if ( is_lstep_stop_at_no_line() )
      return ds_error("*** No line for 0x%W", loc);
    file0 = "";
    line0 = 0;
  }
  last_cnt = cnt;
  while ( cnt > 0 )
  {
    do
    {
      while ( 1 )
      {
        if ( store_breakpoints(8) < 0 )
          return -1;
        r = cont_and_wait_halt(2, 1);
        restore_breakpoints(8);
        if ( r != 35 )
          goto LABEL_37;
        if ( ds_eval_word("$_PC", &v6) )
          return -1;
        s2 = address_to_file_and_line(v6, &line1, 0, &v8, 0);
        if ( s2 )
        {
          if ( line1 > 0 )
            break;
        }
        if ( is_lstep_stop_at_no_line() )
        {
          ds_error("*** No line for 0x%W", v6);
          ex_default_dr();
          return -1;
        }
        line1 = 0;
      }
    }
    while ( !strcmp(file0, s2) && line1 == line0 );
    file0 = s2;
    line0 = line1;
    delta0 = v8;
    r = 0;
    --cnt;
  }
LABEL_37:
  if ( r == 34 || r == 33 || r == 36 || r == 39 )
  {
    ex_default_dr();
  }
  else if ( !r )
  {
    lstep_default_list();
  }
  return exitc(r);
}

static int __cdecl targ_luntil_cmd(int ac, char **av)
{
  char *file0; // [esp+0h] [ebp-18h]
  unsigned int addr; // [esp+4h] [ebp-14h] BYREF
  int delta0; // [esp+8h] [ebp-10h] BYREF
  int line0; // [esp+Ch] [ebp-Ch] BYREF
  int r; // [esp+10h] [ebp-8h]
  int line; // [esp+14h] [ebp-4h] BYREF

  r = 0;
  if ( ac > 0 )
  {
    --ac;
    ++av;
    if ( ac > 0 && **av == 45 )
      return ds_error("Usage: luntil <line>");
  }
  if ( ac != 1 )
    return ds_error("Usage: luntil <line>");
  if ( ds_eval_word("$_PC", &addr) )
    return -1;
  if ( ds_scan_digit_word(*av, (unsigned int *)&line) )
    return ds_error("Usage: luntil <line>");
  file0 = address_to_file_and_line(addr, &line0, 0, &delta0, 0);
  if ( !file0 || line0 <= 0 )
    return ds_error("*** No line for 0x%W", addr);
  addr = file_and_line_to_address(line, file0);
  if ( !addr )
    return ds_error("*** No address for line %d in %s", line, file0);
  r = -1;
  if ( addr )
  {
    del_brkpt(144, 0);
    if ( add_brkpt(16, addr, 1u) < 0 )
      return -1;
    if ( store_breakpoints(16) < 0 )
      return -1;
    r = cont_and_wait_halt(0, 1);
    if ( r == 34 || r == 33 || r == 36 || r == 39 || !r )
      ex_default_dr();
  }
  return exitc(r);
}

static unsigned int __cdecl get_pc(SCRS *scrs)
{
  unsigned int result; // eax

#ifdef DSNET_COMPILING_E
  if ( (scrs->status & 4) != 0 )
  {
    result = scrs->errorepc;
    if ( (scrs->cause & 0x40000000) != 0 )
      result += 4;
  }
  else
  {
    result = scrs->epc;
    if ( (scrs->cause & 0x80000000) != 0 )
      result += 4;
  }
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  result = scrs->epc;
  if ( (scrs->cause & 0x80000000) != 0 )
    result += 4;
#endif /* DSNET_COMPILING_I */
  return result;
}

static int __cdecl is_breakpoint(int flag, SCRS *scrs)
{
  unsigned int adr; // [esp+0h] [ebp-8h]
  BRKPT *p; // [esp+4h] [ebp-4h]

  adr = get_pc(scrs);
  for ( p = bps.head; p; p = p->forw )
  {
    if ( ((LOBYTE(p->flag) ^ (unsigned __int8)flag) & 0x1C) == 0 && (p->flag & 0x108) != 8 && p->adr == adr )
      return 1;
  }
  return 0;
}

static int __cdecl load_scrs(SCRS *scrs)
{
#ifdef DSNET_COMPILING_E
  quad pv[4]; // [esp+0h] [ebp-6Ch] BYREF
  unsigned int masks[11]; // [esp+40h] [ebp-2Ch] BYREF
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  unsigned int vals[2]; // [esp+0h] [ebp-30h] BYREF
  unsigned int masks[10]; // [esp+8h] [ebp-28h] BYREF
#endif /* DSNET_COMPILING_I */

  ds_bzero(masks, sizeof(masks));
#ifdef DSNET_COMPILING_E
  masks[2] = 1073770496;
  if ( load_quad_registers(masks, pv, 4) )
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  masks[3] = 24576;
  if ( load_word_registers(masks, vals, 2) )
#endif /* DSNET_COMPILING_I */
    return -1;
#ifdef DSNET_COMPILING_E
  scrs->status = ds_cqw(pv[0]);
  scrs->cause = ds_cqw(pv[1]);
  scrs->epc = ds_cqw(pv[2]);
  scrs->errorepc = ds_cqw(pv[3]);
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  *scrs = *(SCRS *)vals;
#endif /* DSNET_COMPILING_I */
  return 0;
}

static int __cdecl do_cont(int flag, SCRS *scrs)
{
  int r; // [esp+0h] [ebp-4h]

  if ( store_breakpoints(flag) < 0 )
    return -1;
  r = cont_and_wait_halt(0, 1);
  restore_breakpoints(flag);
  del_brkpt(132, 0);
  del_brkpt(144, 0);
  if ( r != 36 )
    return r;
  if ( load_scrs(scrs) < 0 )
    return -2;
  if ( (scrs->cause & 0x3C) == 36 )
    return 36;
  return -2;
}

static int __cdecl host_do_step(int f_next, SCRS *scrs)
{
#ifdef DSNET_COMPILING_E
  unsigned int errorepc; // eax
  signed int cause; // ebx
  int v5; // eax
  unsigned int rpc; // [esp+4h] [ebp-Ch]
  unsigned int tadr; // [esp+8h] [ebp-8h] BYREF
  unsigned int ins; // [esp+Ch] [ebp-4h] BYREF
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  int v3; // eax
  unsigned int rpc; // [esp+0h] [ebp-Ch]
  unsigned int tadr; // [esp+4h] [ebp-8h] BYREF
  unsigned int ins; // [esp+8h] [ebp-4h] BYREF
#endif /* DSNET_COMPILING_I */

#ifdef DSNET_COMPILING_E
  if ( (scrs->status & 4) != 0 )
    errorepc = scrs->errorepc;
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  rpc = scrs->epc;
  if ( (scrs->cause & 0x80000000) == 0 )
  {
    if ( add_brkpt(4, rpc + 4, 1u) < 0 )
      return -1;
  }
#endif /* DSNET_COMPILING_I */
  else
#ifdef DSNET_COMPILING_E
    errorepc = scrs->epc;
  rpc = errorepc;
  if ( ds_load_mem(errorepc, &ins, 4) )
    return -1;
  cause = scrs->cause;
  if ( (scrs->status & 4) == 0 )
#endif /* DSNET_COMPILING_E */
  {
#ifdef DSNET_COMPILING_E
    if ( cause >= 0 )
      goto LABEL_32;
LABEL_10:
    v5 = is_branch_instruction(rpc, ins, &tadr);
    if ( v5 == 1 )
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
    if ( ds_load_mem(rpc, &ins, 4) )
      return -1;
    v3 = is_branch_instruction(rpc, ins, &tadr);
    if ( v3 == 1 )
#endif /* DSNET_COMPILING_I */
    {
      if ( f_next && ((ins & 0xFC1FFFFF) == 63497 || (ins & 0xFC000000) == 201326592) )
      {
        if ( add_brkpt(4, rpc + 8, 1u) < 0 )
          return -1;
      }
      else if ( add_brkpt(4, tadr, 1u) < 0 )
      {
        return -1;
      }
    }
#ifdef DSNET_COMPILING_E
    else if ( v5 > 1 )
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
    else if ( v3 > 1 )
#endif /* DSNET_COMPILING_I */
    {
#ifdef DSNET_COMPILING_E
      if ( v5 == 2 )
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
      if ( v3 == 2 )
#endif /* DSNET_COMPILING_I */
      {
        if ( tadr != rpc && add_brkpt(4, tadr, 1u) < 0 )
          return -1;
        if ( add_brkpt(4, rpc + 8, 1u) < 0 )
          return -1;
      }
    }
#ifdef DSNET_COMPILING_E
    else if ( !v5 )
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
    else if ( !v3 )
#endif /* DSNET_COMPILING_I */
    {
      return ds_error("host_do_step: BD=1 and branch=0");
    }
#ifdef DSNET_COMPILING_E
    return do_cont(4, scrs);
#endif /* DSNET_COMPILING_E */
  }
#ifdef DSNET_COMPILING_E
  if ( (cause & 0x40000000) != 0 )
    goto LABEL_10;
LABEL_32:
  if ( add_brkpt(4, rpc + 4, 1u) < 0 )
    return -1;
  if ( (ins & 0xFC0E0000) == 67239936
    || (ins & 0xF8000000) == 1342177280
    || (ins & 0xF81F0000) == 1476395008
    || (ins & 0xF3FE0000) == 1090650112 )
  {
    goto LABEL_10;
  }
#endif /* DSNET_COMPILING_E */
  return do_cont(4, scrs);
}

void __cdecl display_current_informations(int result)
{
  if ( result == 34 || result == 33 || result == 36 || result == 39 || result == -2 )
    ex_default_dr();
}

static int __cdecl do_lstep(int f_next, SCRS *scrs)
{
  int delta1; // [esp+0h] [ebp-24h] BYREF
  int delta0; // [esp+4h] [ebp-20h] BYREF
  int line1; // [esp+8h] [ebp-1Ch] BYREF
  int pline; // [esp+Ch] [ebp-18h] BYREF
  int r; // [esp+10h] [ebp-14h]
  char *file1; // [esp+14h] [ebp-10h]
  char *file0; // [esp+18h] [ebp-Ch]
  unsigned int adr1; // [esp+1Ch] [ebp-8h]
  unsigned int adr0; // [esp+20h] [ebp-4h]

  adr0 = get_pc(scrs);
  file0 = address_to_file_and_line(adr0, &pline, 0, &delta0, 0);
  if ( file0 && pline > 0 )
  {
    while ( 1 )
    {
      while ( 1 )
      {
LABEL_6:
        r = host_do_step(f_next, scrs);
        if ( r != 36 )
          return r;
        adr1 = get_pc(scrs);
        file1 = address_to_file_and_line(adr1, &line1, 0, &delta1, 0);
        if ( file1 )
        {
          if ( line1 > 0 )
            break;
        }
        if ( is_lstep_stop_at_no_line() )
        {
          ds_error("*** No line for 0x%W", adr1);
          return -2;
        }
        file1 = "";
        line1 = 0;
      }
      if ( strcmp(file0, file1) || line1 != pline )
        break;
      file0 = file1;
      pline = line1;
      delta0 = delta1;
    }
    return 0;
  }
  else
  {
    if ( !is_lstep_stop_at_no_line() )
    {
      file0 = "";
      pline = 0;
      goto LABEL_6;
    }
    return ds_error("*** No line for 0x%W", adr0);
  }
}

static int __cdecl host_cont_cmd(int ac, char **av)
{
  int v2; // eax
#ifdef DSNET_COMPILING_E
  int r; // [esp+4h] [ebp-20h]
  SCRS scrs_area; // [esp+Ch] [ebp-18h] BYREF
  int i; // [esp+1Ch] [ebp-8h]
  int cnt; // [esp+20h] [ebp-4h] BYREF
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  int r; // [esp+0h] [ebp-18h]
  SCRS scrs_area; // [esp+8h] [ebp-10h] BYREF
  int i; // [esp+10h] [ebp-8h]
  int cnt; // [esp+14h] [ebp-4h] BYREF
#endif /* DSNET_COMPILING_I */
  static int last_cnt = 1;

  cnt = 1;
  r = 0;
  if ( ac > 0 )
    v2 = 1;
  else
    v2 = last_cnt;
  cnt = v2;
  if ( ac > 0 )
  {
    --ac;
    ++av;
    if ( ac > 0 && **av == 45 )
      return ds_error("Usage: cont [<cnt>]");
  }
  if ( ac > 1 )
    return ds_error("Usage: cont [<cnt>]");
#ifdef DSNET_COMPILING_E
  if ( ac > 0 )
  {
    if ( ds_eval_word(*av, (unsigned int *)&cnt) )
      return -1;
    if ( cnt <= 0 )
      return 0;
  }
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  if ( ac <= 0 )
    goto LABEL_15;
  if ( ds_eval_word(*av, (unsigned int *)&cnt) )
    return -1;
  if ( cnt <= 0 )
    return 0;
LABEL_15:
#endif /* DSNET_COMPILING_I */
  last_cnt = cnt;
  if ( load_scrs(&scrs_area) < 0 )
    return -1;
#ifdef DSNET_COMPILING_E
  i = 0;
LABEL_18:
  if ( cnt > i )
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  for ( i = 0; cnt > i; ++i )
#endif /* DSNET_COMPILING_I */
  {
#ifdef DSNET_COMPILING_E
    do
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
    while ( (scrs_area.cause & 0x80000000) != 0 || is_breakpoint(8, &scrs_area) )
#endif /* DSNET_COMPILING_I */
    {
#ifdef DSNET_COMPILING_E
      if ( (scrs_area.status & 4) != 0 )
      {
        if ( (scrs_area.cause & 0x40000000) != 0 )
          goto LABEL_24;
      }
      else if ( (scrs_area.cause & 0x80000000) != 0 )
      {
        goto LABEL_24;
      }
      if ( !is_breakpoint(8, &scrs_area) )
      {
        r = do_cont(8, &scrs_area);
        if ( r != 36 )
          break;
        ++i;
        goto LABEL_18;
      }
LABEL_24:
#endif /* DSNET_COMPILING_E */
      r = host_do_step(0, &scrs_area);
#ifdef DSNET_COMPILING_I
      if ( r != 36 || is_breakpoint(8, &scrs_area) )
        goto LABEL_26;
#endif /* DSNET_COMPILING_I */
    }
#ifdef DSNET_COMPILING_E
    while ( r == 36 && !is_breakpoint(8, &scrs_area) );
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
    r = do_cont(8, &scrs_area);
    if ( r != 36 )
      break;
#endif /* DSNET_COMPILING_I */
  }
#ifdef DSNET_COMPILING_I
LABEL_26:
#endif /* DSNET_COMPILING_I */
  display_current_informations(r);
  return exitc(r);
}

static int __cdecl host_until_cmd(int ac, char **av)
{
#ifdef DSNET_COMPILING_E
  signed int cause; // ebx
  int i; // [esp+8h] [ebp-1ACh]
  int i_1; // [esp+8h] [ebp-1ACh]
  int r; // [esp+Ch] [ebp-1A8h]
  unsigned int adrs[100]; // [esp+10h] [ebp-1A4h] BYREF
  SCRS *scrs; // [esp+1A0h] [ebp-14h]
  SCRS scrs_area; // [esp+1A4h] [ebp-10h] BYREF
  int aca; // [esp+1BCh] [ebp+8h]
  char **ava; // [esp+1C0h] [ebp+Ch]
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  int i; // [esp+4h] [ebp-1A4h]
  int i_1; // [esp+4h] [ebp-1A4h]
  int r; // [esp+8h] [ebp-1A0h]
  unsigned int adrs[100]; // [esp+Ch] [ebp-19Ch] BYREF
  SCRS *scrs; // [esp+19Ch] [ebp-Ch]
  SCRS scrs_area; // [esp+1A0h] [ebp-8h] BYREF
  int aca; // [esp+1B0h] [ebp+8h]
  char **ava; // [esp+1B4h] [ebp+Ch]
#endif /* DSNET_COMPILING_I */

  scrs = &scrs_area;
  r = 0;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  ava = av + 1;
  if ( aca > 0 && **ava == 45 || aca <= 0 || aca > 100 )
    return ds_error("Usage: until <adr>...");
  if ( load_scrs(scrs) < 0 )
    return -1;
  for ( i = 0; aca > i; ++i )
  {
    if ( ds_eval_word(ava[i], &adrs[i]) )
      return -1;
    if ( (adrs[i] & 3) != 0 )
      return ds_error("invalid align");
  }
  for ( i_1 = 0; aca > i_1; ++i_1 )
  {
#ifdef DSNET_COMPILING_E
    if ( adrs[i_1] == get_pc(scrs) )
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
    if ( adrs[i_1] != get_pc(scrs) )
      goto LABEL_36;
    if ( aca == 1 || i_1 < aca - 1 )
#endif /* DSNET_COMPILING_I */
    {
#ifdef DSNET_COMPILING_E
      if ( aca == 1 || i_1 < aca - 1 )
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
      r = host_do_step(0, scrs);
      if ( r != 36 )
        break;
    }
    if ( aca == 1 )
    {
LABEL_36:
      if ( (scrs->cause & 0x80000000) == 0 )
        goto LABEL_37;
      r = host_do_step(0, scrs);
      if ( r != 36 )
        break;
      if ( adrs[i_1] != get_pc(scrs) )
#endif /* DSNET_COMPILING_I */
      {
#ifdef DSNET_COMPILING_E
        r = host_do_step(0, scrs);
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
LABEL_37:
        if ( add_brkpt(16, adrs[i_1], 1u) < 0 )
          return -1;
        r = do_cont(16, scrs);
#endif /* DSNET_COMPILING_I */
        if ( r != 36 )
          break;
      }
#ifdef DSNET_COMPILING_E
      if ( aca != 1 )
        continue;
#endif /* DSNET_COMPILING_E */
    }
#ifdef DSNET_COMPILING_E
    cause = scrs->cause;
    if ( (scrs->status & 4) != 0 )
    {
      if ( (cause & 0x40000000) != 0 )
        goto LABEL_30;
    }
    else if ( cause < 0 )
    {
LABEL_30:
      r = host_do_step(0, scrs);
      if ( r != 36 )
        break;
      if ( adrs[i_1] == get_pc(scrs) )
        continue;
    }
    if ( add_brkpt(16, adrs[i_1], 1u) < 0 )
      return -1;
    r = do_cont(16, scrs);
    if ( r != 36 )
      break;
#endif /* DSNET_COMPILING_E */
  }
  display_current_informations(r);
  return exitc(r);
}

static int __cdecl host_step_cmd(int ac, char **av)
{
  int v2; // eax
#ifdef DSNET_COMPILING_E
  int r; // [esp+0h] [ebp-24h]
  SCRS scrs_area; // [esp+8h] [ebp-1Ch] BYREF
  int i; // [esp+18h] [ebp-Ch]
  int cnt; // [esp+1Ch] [ebp-8h] BYREF
  int f_next; // [esp+20h] [ebp-4h]
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  int r; // [esp+0h] [ebp-1Ch]
  SCRS scrs_area; // [esp+8h] [ebp-14h] BYREF
  int i; // [esp+10h] [ebp-Ch]
  int cnt; // [esp+14h] [ebp-8h] BYREF
  int f_next; // [esp+18h] [ebp-4h]
#endif /* DSNET_COMPILING_I */
  static int last_cnt = 1;

  f_next = 0;
  cnt = 1;
  r = 0;
  if ( ac > 0 )
    v2 = 1;
  else
    v2 = last_cnt;
  cnt = v2;
  if ( ac > 0 )
  {
    --ac;
    ++av;
    if ( ac > 0 && **av == 45 )
      return ds_error("Usage: step [<cnt>]");
  }
  if ( ac > 1 )
    return ds_error("Usage: step [<cnt>]");
  if ( ac <= 0 )
    goto LABEL_15;
  if ( ds_eval_word(*av, (unsigned int *)&cnt) )
    return -1;
  if ( cnt <= 0 )
    return 0;
LABEL_15:
  last_cnt = cnt;
  if ( load_scrs(&scrs_area) < 0 )
    return -1;
  for ( i = 0; cnt > i; ++i )
  {
    r = host_do_step(f_next, &scrs_area);
    if ( r != 36 )
      break;
  }
  display_current_informations(r);
  return exitc(r);
}

static int __cdecl host_next_cmd(int ac, char **av)
{
  int v2; // eax
#ifdef DSNET_COMPILING_E
  int r; // [esp+0h] [ebp-24h]
  SCRS scrs_area; // [esp+8h] [ebp-1Ch] BYREF
  int i; // [esp+18h] [ebp-Ch]
  int cnt; // [esp+1Ch] [ebp-8h] BYREF
  int f_next; // [esp+20h] [ebp-4h]
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  int r; // [esp+0h] [ebp-1Ch]
  SCRS scrs_area; // [esp+8h] [ebp-14h] BYREF
  int i; // [esp+10h] [ebp-Ch]
  int cnt; // [esp+14h] [ebp-8h] BYREF
  int f_next; // [esp+18h] [ebp-4h]
#endif /* DSNET_COMPILING_I */
  static int last_cnt = 1;

  f_next = 1;
  cnt = 1;
  r = 0;
  if ( ac > 0 )
    v2 = 1;
  else
    v2 = last_cnt;
  cnt = v2;
  if ( ac > 0 )
  {
    --ac;
    ++av;
    if ( ac > 0 && **av == 45 )
      return ds_error("Usage: next [<cnt>]");
  }
  if ( ac > 1 )
    return ds_error("Usage: next [<cnt>]");
  if ( ac <= 0 )
    goto LABEL_15;
  if ( ds_eval_word(*av, (unsigned int *)&cnt) )
    return -1;
  if ( cnt <= 0 )
    return 0;
LABEL_15:
  last_cnt = cnt;
  if ( load_scrs(&scrs_area) < 0 )
    return -1;
  for ( i = 0; cnt > i; ++i )
  {
    r = host_do_step(f_next, &scrs_area);
    if ( r != 36 )
      break;
  }
  display_current_informations(r);
  return exitc(r);
}

static int __cdecl host_lstep_cmd(int ac, char **av)
{
  int v2; // eax
#ifdef DSNET_COMPILING_E
  int r; // [esp+0h] [ebp-24h]
  SCRS scrs_area; // [esp+8h] [ebp-1Ch] BYREF
  int i; // [esp+18h] [ebp-Ch]
  int cnt; // [esp+1Ch] [ebp-8h] BYREF
  int f_next; // [esp+20h] [ebp-4h]
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  int r; // [esp+0h] [ebp-1Ch]
  SCRS scrs_area; // [esp+8h] [ebp-14h] BYREF
  int i; // [esp+10h] [ebp-Ch]
  int cnt; // [esp+14h] [ebp-8h] BYREF
  int f_next; // [esp+18h] [ebp-4h]
#endif /* DSNET_COMPILING_I */
  static int last_cnt = 1;

  f_next = 0;
  cnt = 1;
  r = 0;
  if ( ac > 0 )
    v2 = 1;
  else
    v2 = last_cnt;
  cnt = v2;
  if ( ac > 0 )
  {
    --ac;
    ++av;
    if ( ac > 0 && **av == 45 )
      return ds_error("Usage: lstep [<cnt>]");
  }
  if ( ac > 1 )
    return ds_error("Usage: lstep [<cnt>]");
  if ( ac <= 0 )
    goto LABEL_15;
  if ( ds_eval_word(*av, (unsigned int *)&cnt) )
    return -1;
  if ( cnt <= 0 )
    return 0;
LABEL_15:
  last_cnt = cnt;
  if ( load_scrs(&scrs_area) < 0 )
    return -1;
  for ( i = 0; cnt > i; ++i )
  {
    r = do_lstep(f_next, &scrs_area);
    if ( r != 36 )
      break;
  }
  if ( r )
    display_current_informations(r);
  else
    lstep_default_list();
  return exitc(r);
}

static int __cdecl host_lnext_cmd(int ac, char **av)
{
  int v2; // eax
#ifdef DSNET_COMPILING_E
  int r; // [esp+0h] [ebp-24h]
  SCRS scrs_area; // [esp+8h] [ebp-1Ch] BYREF
  int i; // [esp+18h] [ebp-Ch]
  int cnt; // [esp+1Ch] [ebp-8h] BYREF
  int f_next; // [esp+20h] [ebp-4h]
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  int r; // [esp+0h] [ebp-1Ch]
  SCRS scrs_area; // [esp+8h] [ebp-14h] BYREF
  int i; // [esp+10h] [ebp-Ch]
  int cnt; // [esp+14h] [ebp-8h] BYREF
  int f_next; // [esp+18h] [ebp-4h]
#endif /* DSNET_COMPILING_I */
  static int last_cnt = 1;

  f_next = 1;
  cnt = 1;
  r = 0;
  if ( ac > 0 )
    v2 = 1;
  else
    v2 = last_cnt;
  cnt = v2;
  if ( ac > 0 )
  {
    --ac;
    ++av;
    if ( ac > 0 && **av == 45 )
      return ds_error("Usage: lnext [<cnt>]");
  }
  if ( ac > 1 )
    return ds_error("Usage: lnext [<cnt>]");
  if ( ac <= 0 )
    goto LABEL_15;
  if ( ds_eval_word(*av, (unsigned int *)&cnt) )
    return -1;
  if ( cnt <= 0 )
    return 0;
LABEL_15:
  last_cnt = cnt;
  if ( load_scrs(&scrs_area) < 0 )
    return -1;
  for ( i = 0; cnt > i; ++i )
  {
    r = do_lstep(f_next, &scrs_area);
    if ( r != 36 )
      break;
  }
  if ( r )
    display_current_informations(r);
  else
    lstep_default_list();
  return exitc(r);
}

static int __cdecl host_luntil_cmd(int ac, char **av)
{
  int v2; // eax
  unsigned int pc; // eax
  unsigned int v5; // eax
#ifdef DSNET_COMPILING_E
  int r; // [esp+4h] [ebp-34h]
  char *file0; // [esp+8h] [ebp-30h]
  SCRS v8; // [esp+10h] [ebp-28h] BYREF
  unsigned int delta0; // [esp+20h] [ebp-18h] BYREF
  unsigned int line0; // [esp+24h] [ebp-14h] BYREF
  unsigned int line; // [esp+28h] [ebp-10h] BYREF
  unsigned int addr; // [esp+2Ch] [ebp-Ch]
  int i; // [esp+30h] [ebp-8h]
  int cnt; // [esp+34h] [ebp-4h]
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  int v6; // [esp+0h] [ebp-2Ch]
  char *s2; // [esp+4h] [ebp-28h]
  SCRS v8; // [esp+Ch] [ebp-20h] BYREF
  unsigned int delta0; // [esp+14h] [ebp-18h] BYREF
  unsigned int line0; // [esp+18h] [ebp-14h] BYREF
  unsigned int line; // [esp+1Ch] [ebp-10h] BYREF
  unsigned int addr; // [esp+20h] [ebp-Ch]
  int i; // [esp+24h] [ebp-8h]
  int cnt; // [esp+28h] [ebp-4h]
#endif /* DSNET_COMPILING_I */
  static int last_cnt = 1;

  cnt = 1;
#ifdef DSNET_COMPILING_E
  r = 0;
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  v6 = 0;
#endif /* DSNET_COMPILING_I */
  if ( ac > 0 )
    v2 = 1;
  else
    v2 = last_cnt;
  cnt = v2;
  if ( ac > 0 )
  {
    --ac;
    ++av;
    if ( ac > 0 && **av == 45 )
      return ds_error("Usage: luntil <line>");
  }
  if ( ac != 1 )
    return ds_error("Usage: luntil <line>");
  last_cnt = cnt;
  if ( load_scrs(&v8) < 0 )
    return -1;
  addr = get_pc(&v8);
  if ( ds_scan_digit_word(*av, &line) )
    return ds_error("Usage: luntil <line>");
#ifdef DSNET_COMPILING_E
  file0 = address_to_file_and_line(addr, (int *)&line0, 0, (int *)&delta0, 0);
  if ( !file0 || !line0 )
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  s2 = address_to_file_and_line(addr, (int *)&line0, 0, (int *)&delta0, 0);
  if ( !s2 || !line0 )
#endif /* DSNET_COMPILING_I */
    return ds_error("*** No line for 0x%W", addr);
#ifdef DSNET_COMPILING_E
  addr = file_and_line_to_address(line, file0);
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  addr = file_and_line_to_address(line, s2);
#endif /* DSNET_COMPILING_I */
  if ( !addr )
#ifdef DSNET_COMPILING_E
    return ds_error("*** No address for line %d in %s", line, file0);
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
    return ds_error("*** No address for line %d in %s", line, s2);
#endif /* DSNET_COMPILING_I */
  for ( i = 0; i < 1; ++i )
  {
    pc = get_pc(&v8);
    if ( addr == pc )
    {
#ifdef DSNET_COMPILING_E
      r = host_do_step(0, &v8);
      if ( r != 36 )
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
      v6 = host_do_step(0, &v8);
      if ( v6 != 36 )
#endif /* DSNET_COMPILING_I */
        break;
    }
#ifdef DSNET_COMPILING_E
    if ( (v8.status & 4) != 0 )
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
    if ( (v8.cause & 0x80000000) != 0 )
#endif /* DSNET_COMPILING_I */
    {
#ifdef DSNET_COMPILING_E
      if ( (v8.cause & 0x40000000) == 0 )
        goto LABEL_28;
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
      v6 = host_do_step(0, &v8);
      if ( v6 != 36 )
        break;
      v5 = get_pc(&v8);
      if ( addr == v5 )
        break;
#endif /* DSNET_COMPILING_I */
    }
#ifdef DSNET_COMPILING_E
    else if ( (v8.cause & 0x80000000) == 0 )
    {
      goto LABEL_28;
    }
    r = host_do_step(0, &v8);
    if ( r != 36 )
      break;
    v5 = get_pc(&v8);
    if ( addr == v5 )
      break;
LABEL_28:
#endif /* DSNET_COMPILING_E */
    if ( add_brkpt(16, addr, 1u) < 0 )
      return -1;
#ifdef DSNET_COMPILING_E
    r = do_cont(16, &v8);
    if ( r != 36 )
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
    v6 = do_cont(16, &v8);
    if ( v6 != 36 )
#endif /* DSNET_COMPILING_I */
      break;
  }
#ifdef DSNET_COMPILING_E
  if ( r )
    display_current_informations(r);
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  if ( v6 )
    display_current_informations(v6);
#endif /* DSNET_COMPILING_I */
  else
    lstep_default_list();
#ifdef DSNET_COMPILING_E
  return exitc(r);
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  return exitc(v6);
#endif /* DSNET_COMPILING_I */
}

#ifdef DSNET_COMPILING_E
int __cdecl set_runarg(int ac, char **av)
{
  int v3; // ebx
  char *v4; // ebx
  char *v5; // ebx
  char *buf; // [esp+4h] [ebp-Ch]
  char *bp; // [esp+8h] [ebp-8h]
  int i_3; // [esp+Ch] [ebp-4h]
  int i; // [esp+Ch] [ebp-4h]
  int i_2; // [esp+Ch] [ebp-4h]

  if ( run_args )
    run_args = (char *)ds_free_mem_low(run_args, "exe.c", "set_runarg");
  run_argc = 0;
  buf = ds_decode_args(ac, av);
  if ( !buf )
    return -1;
  i_3 = 0;
  run_args_len = 0;
  while ( ac > i_3 )
  {
    v3 = run_args_len + 1;
    run_args_len = strlen(av[i_3++]) + v3;
  }
  run_args_len += sizeof(_DWORD) * ac;
  run_args = (char *)ds_alloc_mem_low("exe.c", "set_runarg", run_args_len);
  bp = run_args;
  if ( !run_args )
    return -1;
  for ( i = 0; ac > i; ++i )
  {
    v4 = bp;
    bp += 4;
    *(_DWORD *)v4 = strlen(av[i]) + 1;
  }
  for ( i_2 = 0; ac > i_2; ++i_2 )
  {
    v5 = strcpy(bp, av[i_2]);
    bp = &v5[strlen(av[i_2]) + 1];
  }
  run_argc = ac;
  ds_free_mem_low(buf, "exe.c", "set_runarg");
  return 0;
}

int __cdecl run_cmd(int ac, char **av)
{
  quad pv; // [esp+0h] [ebp-18h] BYREF
  int r; // [esp+10h] [ebp-8h]
  unsigned int entry; // [esp+14h] [ebp-4h]

  if ( ac <= 0 )
    return 0;
  if ( ac > 1 )
  {
    if ( *av[1] == 45 )
      return ds_error("Usage: run [<fname> [<args>...]]");
    if ( pload_cmd(ac, av) )
      return -1;
  }
  entry = current_entry_point;
  if ( !current_entry_point )
    return ds_error("current entry point is not set (pload)");
  current_entry_point = 0;
  if ( store_breakpoints(8) < 0 )
    return -1;
  r = run_and_wait_halt(entry, current_gp_value, run_argc, run_args, run_args_len);
  restore_breakpoints(8);
  display_current_informations(r);
  if ( r == 37 )
  {
    if ( load_quad_reg("$a0", &pv) )
      return -1;
    r = ds_cqw(pv);
    ds_printf("*** retval=0x%W\n", r);
    if ( r < 0 || r > 127 )
      return 255;
  }
  else if ( r )
  {
    r += 160;
    if ( r <= 159 || r > 223 )
      return 255;
  }
  return r;
}
#endif /* DSNET_COMPILING_E */

int __cdecl cont_cmd(int ac, char **av)
{
  if ( is_target_exec_ctl() )
    return targ_cont_cmd(ac, av);
  else
    return host_cont_cmd(ac, av);
}

int __cdecl until_cmd(int ac, char **av)
{
  if ( is_target_exec_ctl() )
    return targ_until_cmd(ac, av);
  else
    return host_until_cmd(ac, av);
}

int __cdecl step_cmd(int ac, char **av)
{
  if ( is_target_exec_ctl() )
    return targ_step_cmd(ac, av);
  else
    return host_step_cmd(ac, av);
}

int __cdecl next_cmd(int ac, char **av)
{
  if ( is_target_exec_ctl() )
    return targ_next_cmd(ac, av);
  else
    return host_next_cmd(ac, av);
}

int __cdecl lstep_cmd(int ac, char **av)
{
  if ( is_target_exec_ctl() )
    return targ_lstep_cmd(ac, av);
  else
    return host_lstep_cmd(ac, av);
}

int __cdecl lnext_cmd(int ac, char **av)
{
  if ( is_target_exec_ctl() )
    return targ_lnext_cmd(ac, av);
  else
    return host_lnext_cmd(ac, av);
}

int __cdecl luntil_cmd(int ac, char **av)
{
  if ( is_target_exec_ctl() )
    return targ_luntil_cmd(ac, av);
  else
    return host_luntil_cmd(ac, av);
}

static int __cdecl exitc(int r)
{
  int v1; // edx

  v1 = r;
  if ( r >= 0 )
  {
    v1 = r + 160;
    if ( r + 160 > 223 )
      return 255;
  }
  return v1;
}

