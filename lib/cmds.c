#include "dsnet_prototypes.h"

// I'm just going to put these here...
struct _GIF_OPT gif;
struct _GIF_OPT *pgif;
struct _SMEM_OPT smem;
struct _SMEM_OPT *psmem;
int m_result;
unsigned int m_sdram;
unsigned int m_init;

DS_HISTBUF ds_histbuf;
static struct __anon_struct_135 {CMD_ENTRY *head; CMD_ENTRY *tail;} cmd_list;
static struct __anon_struct_136 {DS_OPTION *head; DS_OPTION *tail;} ds_option_list;
static struct __anon_struct_137 {ALIAS *head; ALIAS *tail;} aliases;

char path_46[1025] = {0};
char show_arg_77[1024] = {0};
static int (__cdecl *last_func_88)() = &ds_help_cmd;

static int cmd_max_na = 0;
DS_OPTION *ds_opt_hex_radix = NULL;
DS_OPTION *ds_opt_log_total_size = NULL;
DS_OPTION *ds_opt_log_packet_size = NULL;
DS_OPTION *ds_opt_histfile = NULL;
DS_OPTION *ds_opt_histfilesize = NULL;
DS_OPTION *ds_opt_histsize = NULL;
DS_OPTION *ds_opt_tty_max_size = NULL;
int ds_abort_input = 0;
int ds_cmd_executing = 0;

static struct __anon_struct_131 {SHOW_ARG *head; SHOW_ARG *tail;} show_arg_list = { NULL, NULL };
static struct __anon_struct_133 {HELP_ARG *head; HELP_ARG *tail;} help_arg_list = { NULL, NULL };

static void __cdecl show_option(char *name, int f_force);
static int __cdecl is_reserved_name(char *name);
static int __cdecl read_option(char *fname);
static int __cdecl scan_digit_with_dot(char *str, int *psec, int *pusec);
static int __cdecl ds_status_cmd(int ac, char **av);
static int __cdecl ds_history_cmd(int ac, char **av);
static int __cdecl ds_enable_cmd(int ac, char **av);
static int __cdecl ds_disable_cmd(int ac, char **av);
static ALIAS *__cdecl alias(char *buf, int nbuf, int ac, char **av);
static int __cdecl ds_cmd_execute_internal(char *s, int f_repeat);
static int __cdecl ds_path_completion(DS_HISTBUF *hb, char *path);
static int __cdecl ds_symbol_completion(DS_HISTBUF *hb, char *name);
static int __cdecl ds_get_last_keyword(char *buf, char *str);
static int __cdecl ds_set_completion(DS_HISTBUF *hb, char *str);
static int __cdecl ds_help_completion(DS_HISTBUF *hb, char *str);
static int __cdecl ds_path_or_symbol_completion(DS_HISTBUF *hb, char *str);

int __cdecl ds_shell_cmd(int ac, char **av)
{
  char *v4; // eax
  char *v5; // eax
  char *v7; // eax
  int v8; // eax
  int v9; // eax
  int n; // [esp+0h] [ebp-428h]
  int i; // [esp+4h] [ebp-424h]
  sig_t sigint; // [esp+8h] [ebp-420h]
  int r; // [esp+Ch] [ebp-41Ch]
  int r_1; // [esp+Ch] [ebp-41Ch]
  int fd; // [esp+10h] [ebp-418h]
  int raw; // [esp+14h] [ebp-414h]
  int status; // [esp+18h] [ebp-410h] BYREF
  int pid; // [esp+1Ch] [ebp-40Ch]
  char *bp; // [esp+20h] [ebp-408h]
  char buf[1024]; // [esp+24h] [ebp-404h] BYREF
  char *shell; // [esp+424h] [ebp-4h]

  if ( ac <= 0 )
    return 0;
  if ( ac > 1 && !strcmp("-help", av[1]) )
    return ds_error("Usage: ! [<cmd>] [<args>]...]");
  if ( ac > 2 )
  {
    bp = buf;
    for ( i = 1; ac > i; ++i )
    {
      n = strlen(av[i]);
      memcpy(bp, av[i], n);
      bp += n;
      *bp++ = 32;
    }
    *bp = 0;
    av[1] = buf;
  }
  shell = getenv("SHELL");
  if ( !shell )
    shell = "/bin/sh";
  raw = ds_resume_kbd();
  pid = fork();
  if ( pid >= 0 )
  {
    if ( !pid )
    {
      for ( fd = 0; fd <= 255; ++fd )
      {
        if ( fd > 2 )
          close(fd);
      }
      if ( ac <= 1 )
        v5 = shell;
      else
        v5 = av[1];
      execlp(shell, shell, "-c", v5, 0);
      v7 = strerror(errno);
      printf("execlp - %s\n", v7);
      exit(1);
    }
    sigint = signal(SIGINT, SIG_IGN);
    r_1 = wait(&status);
    signal(SIGINT, sigint);
    if ( r_1 < 0 )
      goto LABEL_30;
    v8 = 0;
    if ( (unsigned __int8)status != 127 && (status & 0x7F) != 0 )
      v8 = 1;
    if ( v8 )
LABEL_30:
      v9 = -1;
    else
      v9 = 0;
    r = v9;
  }
  else
  {
    v4 = strerror(errno);
    r = ds_error("fork - %s", v4);
  }
  if ( raw )
    ds_raw_kbd();
  return r;
}

char *__cdecl ds_ref_option_str(char *name)
{
  DS_OPTION *p; // [esp+0h] [ebp-4h]

  for ( p = ds_option_list.head; p; p = p->forw )
  {
    if ( (p->type & 3) == 3 && !strcmp(name, p->name) )
      return p->str;
  }
  return 0;
}

int __cdecl ds_ref_user_variable(char *name, unsigned int *wp)
{
  DS_OPTION *p; // [esp+0h] [ebp-4h]

  for ( p = ds_option_list.head; p; p = p->forw )
  {
    if ( !strcmp(name, p->name) && (p->type & 0x23) == 34 )
    {
      *wp = p->val;
      return 1;
    }
  }
  return 0;
}

DS_OPTION *__cdecl ds_set_option(char *name, int type, char *str, int val, int f_def)
{
  size_t v5; // eax
  char *v6; // eax
  DS_OPTION *tail; // edx
  size_t v8; // eax
  char *v9; // eax
  size_t v10; // eax
  char *v11; // eax
  DS_OPTION *q; // [esp+Ch] [ebp-8h]
  DS_OPTION *p; // [esp+10h] [ebp-4h]

  for ( p = ds_option_list.head; p; p = q )
  {
    q = p->forw;
    if ( !strcmp(name, p->name) )
      break;
  }
  if ( !p )
  {
    p = (DS_OPTION *)ds_alloc_mem_low("cmds.c", "ds_set_option", sizeof(DS_OPTION));
    if ( !p )
      ds_exit(135);
    ds_bzero(p, sizeof(DS_OPTION));
    v5 = strlen(name);
    v6 = (char *)ds_alloc_mem_low("cmds.c", "ds_set_option", v5 + 1);
    p->name = v6;
    if ( !v6 )
      ds_exit(135);
    strcpy(p->name, name);
    p->type = type;
    tail = ds_option_list.tail;
    p->back = ds_option_list.tail;
    if ( tail )
      p->back->forw = p;
    else
      ds_option_list.head = p;
    p->forw = 0;
    ds_option_list.tail = p;
  }
  if ( (p->type & 3) != 0 )
  {
    if ( (p->type & 3) == 3 )
    {
      p->str = (char *)ds_free_mem_low(p->str, "cmds.c", "ds_set_option");
      if ( str )
      {
        v8 = strlen(str);
        v9 = (char *)ds_alloc_mem_low("cmds.c", "ds_set_option", v8 + 1);
        p->str = v9;
        if ( !v9 )
          ds_exit(135);
        strcpy(p->str, str);
      }
      else
      {
        p->str = 0;
      }
      if ( f_def )
      {
        p->def_str = (char *)ds_free_mem_low(p->def_str, "cmds.c", "ds_set_option");
        if ( str )
        {
          v10 = strlen(str);
          v11 = (char *)ds_alloc_mem_low("cmds.c", "ds_set_option", v10 + 1);
          p->def_str = v11;
          if ( !v11 )
            ds_exit(135);
          strcpy(p->def_str, str);
        }
        else
        {
          p->str = 0;
        }
      }
    }
    else
    {
      p->val = val;
      if ( f_def )
        p->def_val = val;
    }
    return p;
  }
  else
  {
    if ( p->forw )
      p->forw->back = p->back;
    else
      ds_option_list.tail = p->back;
    if ( p->back )
      p->back->forw = p->forw;
    else
      ds_option_list.head = p->forw;
    ds_free_mem_low(p, "cmds.c", "ds_set_option");
    return 0;
  }
}

static void __cdecl show_option(char *name, int f_force)
{
  int v2; // eax
  char *v3; // eax
  char *bp; // [esp+0h] [ebp-Ch]
  char *v5; // [esp+4h] [ebp-8h]
  DS_OPTION *p; // [esp+8h] [ebp-4h]

  for ( p = ds_option_list.head; p; p = p->forw )
  {
    if ( !name || !strcmp(name, p->name) )
    {
      if ( (p->type & 0x10) != 0 )
      {
        v5 = "";
      }
      else if ( (p->type & 0x20) != 0 )
      {
        v5 = "  ";
      }
      else
      {
        v5 = " ";
      }
      v2 = p->type & 3;
      if ( v2 == 2 )
      {
        if ( f_force || p->def_val != p->val || (p->type & 0x20) != 0 )
          ds_printf("%s%s=0x%x\n", v5, p->name, p->val);
      }
      else if ( (p->type & 3) == 3 )
      {
        if ( v2 == 3 && (f_force || strcmp(p->str, p->def_str)) )
        {
          ds_printf("%s%s=\"", v5, p->name);
          for ( bp = p->str; *bp; ++bp )
          {
            if ( *bp == 10 )
              ds_printf("\\n\\\n");
            else
              ds_printf("%c", *bp);
          }
          ds_printf("\"\n");
        }
      }
      else if ( v2 == 1 && (f_force || p->def_val != p->val) )
      {
        if ( p->val )
          v3 = "";
        else
          v3 = "no";
        ds_printf("%s%s%s\n", v5, v3, p->name);
      }
    }
  }
}

static int __cdecl is_reserved_name(char *name)
{
  if ( (isalpha(*name)) == 0 )
    return 1;
  if ( ds_check_reserved_name_func )
    return ds_check_reserved_name_func(name);
  return 0;
}

int __cdecl ds_set_cmd(int ac, char **av)
{
  char *v3; // eax
  int v4; // eax
  int n; // [esp+Ch] [ebp-818h]
  int val; // [esp+10h] [ebp-814h] BYREF
  int f_all; // [esp+14h] [ebp-810h]
  char str[1024]; // [esp+18h] [ebp-80Ch] BYREF
  char name[1024]; // [esp+418h] [ebp-40Ch] BYREF
  char *bq; // [esp+818h] [ebp-Ch]
  char *bp; // [esp+81Ch] [ebp-8h]
  DS_OPTION *p; // [esp+820h] [ebp-4h]
  int aca; // [esp+82Ch] [ebp+8h]
  char **ava; // [esp+830h] [ebp+Ch]

  f_all = 0;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  ava = av + 1;
  if ( aca > 0 && **ava == 45 )
    return ds_error("Usage: set [all] [<var>[=<val>]]");
  if ( aca )
  {
    while ( aca > 0 )
    {
      if ( !strcmp("all", *ava) )
      {
        ++f_all;
      }
      else
      {
        bp = *ava;
        bq = name;
        while ( *bp && *bp != 61 )
          *bq++ = *bp++;
        *bq = 0;
        for ( p = ds_option_list.head;
              p && strcmp(name, p->name) && ((p->type & 3) != 1 || strncmp("no", name, 2u) || strcmp(&name[2], p->name));
              p = p->forw )
        {
          ;
        }
        if ( !p )
        {
          if ( *bp != 61 )
            return ds_error("set: no %s option: 'set all' gives all option values.", name);
          if ( is_reserved_name(name) )
            return ds_error("set: %s - reserved name", name);
          p = ds_set_option(name, 2, 0, 0, 0);
          p->type |= 0x20u;
        }
        v3 = bp++;
        if ( *v3 == 61 )
        {
          v4 = p->type & 3;
          if ( v4 == 2 )
          {
            if ( (p->type & 0x10) != 0 )
              return ds_error("set: %s option is locked", name);
            if ( (p->type & 0x20) != 0 && (*bp == 32 || !*bp) )
            {
              if ( p->forw )
                p->forw->back = p->back;
              else
                ds_option_list.tail = p->back;
              if ( p->back )
                p->back->forw = p->forw;
              else
                ds_option_list.head = p->forw;
              ds_free_mem_low(p, "cmds.c", "ds_set_cmd");
            }
            else
            {
              if ( ds_eval_word(bp, (unsigned int *)&val) )
                return -1;
              ds_set_option(name, 2, 0, val, 0);
            }
          }
          else if ( (p->type & 3) == 3 )
          {
            if ( v4 == 3 )
            {
              if ( (p->type & 0x10) != 0 )
                return ds_error("set: %s option is locked", name);
              n = ds_scan_string(str, bp);
              if ( n < 0 )
                return -1;
              str[n] = 0;
              ds_set_option(name, 3, str, 0, 0);
            }
          }
          else if ( v4 == 1 )
          {
            return ds_error("set: [no]%s option doesn't take a value", p->name);
          }
        }
        else if ( (p->type & 3) == 1 )
        {
          if ( (p->type & 0x10) != 0 )
            return ds_error("set: %s option is locked", name);
          if ( !strncmp("no", name, 2u) )
            ds_set_option(&name[2], 1, 0, 0, 0);
          else
            ds_set_option(name, 1, 0, 1, 0);
        }
        else if ( (p->type & 3) != 0 )
        {
          show_option(name, 1);
        }
      }
      --aca;
      ++ava;
    }
    if ( f_all )
      show_option(0, 1);
    return 0;
  }
  else
  {
    show_option(0, 0);
    return 0;
  }
}

static int __cdecl read_option(char *fname)
{
  char *v2; // eax
  char *av[101]; // [esp+4h] [ebp-1C4h] BYREF
  int cl; // [esp+198h] [ebp-30h]
  int pac; // [esp+19Ch] [ebp-2Ch] BYREF
  int size; // [esp+1A0h] [ebp-28h] BYREF
  char *sp; // [esp+1A4h] [ebp-24h]
  char *dp; // [esp+1A8h] [ebp-20h]
  char *dest; // [esp+1ACh] [ebp-1Ch]
  char *lp; // [esp+1B0h] [ebp-18h]
  char *pe; // [esp+1B4h] [ebp-14h]
  char *q; // [esp+1B8h] [ebp-10h]
  char *p; // [esp+1BCh] [ebp-Ch]
  char *buf; // [esp+1C0h] [ebp-8h]
  void *stream; // [esp+1C4h] [ebp-4h]

  buf = 0;
  stream = ds_fopen(fname, "r");
  if ( !stream )
    return -1;
  if ( ds_fsize(fname, &size) < 0 )
    return -1;
  buf = (char *)ds_fload(stream, 0, 0, 1, size);
  ds_fclose(stream);
  if ( !buf )
    return -1;
  p = buf;
  pe = &buf[size];
  while ( pe > p )
  {
    while ( pe > p && *p != 10 && (isspace(*p)) != 0 )
      ++p;
    cl = 0;
    for ( q = p; pe > q && *q != 10; ++q )
    {
      if ( *q == 92 && q[1] == 10 )
      {
        ++cl;
        ++q;
      }
    }
    v2 = q++;
    if ( p != v2 )
    {
      dest = (char *)ds_alloc_mem_low("cmds.c", "read_option", q - p);
      if ( !dest )
        break;
      if ( cl )
      {
        dp = dest;
        for ( sp = p; sp < q - 1; ++sp )
        {
          if ( *sp == 92 && sp[1] == 10 )
            ++sp;
          else
            *dp++ = *sp;
        }
        *dp = 0;
      }
      else
      {
        memcpy(dest, p, q - p - 1);
        dest[q - p - 1] = 0;
      }
      lp = dest;
      while ( *lp )
      {
        lp = ds_scan_args(lp, &pac, av, 100);
        if ( pac > 0 && !strcmp("option", av[0]) )
          ds_set_cmd(pac, av);
      }
      ds_free_mem_low(dest, "cmds.c", "read_option");
    }
    p = q;
  }
  ds_free_mem_low(buf, "cmds.c", "read_option");
  return 0;
}

int __cdecl ds_read_option_file()
{
  char cwd[1025]; // [esp+0h] [ebp-80Ch] BYREF
  char fname[1025]; // [esp+404h] [ebp-408h] BYREF
  char *home; // [esp+808h] [ebp-4h]

  if ( !*ds_program_name )
    return ds_error("ds_read_option_file: ds_program_name[0] == '\\0'");
  home = getenv("HOME");
  if ( !home )
    goto LABEL_11;
  sprintf(fname, "%s/.%src", home, ds_program_name);
  if ( access(fname, 4) )
    goto LABEL_11;
  if ( read_option(fname) < 0 )
    return -1;
  if ( !getcwd(cwd, 0x3FFu) )
    return ds_error("!getcwd");
  if ( !strcmp(home, cwd) )
    return 0;
LABEL_11:
  sprintf(fname, "./.%src", ds_program_name);
  if ( access(fname, 4) )
    return 0;
  else
    return read_option(fname);
}

int __cdecl ds_read_startup_file()
{
  char line[1125]; // [esp+0h] [ebp-C74h] BYREF
  char cwd[1025]; // [esp+468h] [ebp-80Ch] BYREF
  char fname[1025]; // [esp+86Ch] [ebp-408h] BYREF
  char *home; // [esp+C70h] [ebp-4h]

  if ( !*ds_program_name )
    return ds_error("ds_read_statup_file: ds_program_name[0] == '\\0'");
  home = getenv("HOME");
  if ( !home )
    goto LABEL_9;
  sprintf(fname, "%s/.%src", home, ds_program_name);
  if ( access(fname, 4) )
    goto LABEL_9;
  sprintf(line, "source %s", fname);
  ds_cmd_execute(line, 0);
  if ( !getcwd(cwd, 0x3FFu) )
    return ds_error("!getcwd");
  if ( !strcmp(home, cwd) )
    return 0;
LABEL_9:
  sprintf(fname, "./.%src", ds_program_name);
  if ( !access(fname, 4) )
  {
    sprintf(line, "source %s", fname);
    ds_cmd_execute(line, 0);
  }
  return 0;
}

int __cdecl ds_source_cmd(int ac, char **av)
{
  char *v3; // eax
  int cl; // [esp+4h] [ebp-2Ch]
  int r; // [esp+8h] [ebp-28h]
  int size; // [esp+Ch] [ebp-24h] BYREF
  char *sp; // [esp+10h] [ebp-20h]
  char *v8; // [esp+14h] [ebp-1Ch]
  char *pe; // [esp+18h] [ebp-18h]
  char *q; // [esp+1Ch] [ebp-14h]
  char *p; // [esp+20h] [ebp-10h]
  char *line; // [esp+24h] [ebp-Ch]
  char *buf; // [esp+28h] [ebp-8h]
  void *stream; // [esp+2Ch] [ebp-4h]
  int aca; // [esp+38h] [ebp+8h]
  char **ava; // [esp+3Ch] [ebp+Ch]

  stream = 0;
  buf = 0;
  if ( ac <= 0 && !path_46[0] )
    return 0;
  if ( ac > 0 )
  {
    aca = ac - 1;
    ava = av + 1;
    if ( aca > 0 && **ava == 45 )
      return ds_error("Usage: source <fname>");
    if ( aca > 0 )
      strncpy(path_46, *ava, 0x400u);
  }
  stream = ds_fopen(path_46, "r");
  if ( !stream )
    return -1;
  if ( ds_fsize(path_46, &size) < 0 )
    return -1;
  buf = (char *)ds_fload(stream, 0, 0, 1, size);
  ds_fclose(stream);
  if ( !buf )
    return -1;
  p = buf;
  pe = &buf[size];
  while ( 1 )
  {
    r = 0;
    if ( pe <= p )
      break;
    while ( pe > p && *p != 10 && (isspace(*p)) != 0 )
      ++p;
    cl = 0;
    for ( q = p; pe > q && *q != 10; ++q )
    {
      if ( *q == 92 && q[1] == 10 )
      {
        ++cl;
        ++q;
      }
    }
    v3 = q++;
    if ( p != v3 )
    {
      if ( strncmp("option ", p, 7u) )
      {
        r = -1;
        line = (char *)ds_alloc_mem_low("cmds.c", "ds_source_cmd", q - p);
        if ( !line )
          break;
        if ( cl )
        {
          v8 = line;
          for ( sp = p; sp < q - 1; ++sp )
          {
            if ( *sp == 92 && sp[1] == 10 )
              ++sp;
            else
              *v8++ = *sp;
          }
          *v8 = 0;
        }
        else
        {
          memcpy(line, p, q - p - 1);
          line[q - p - 1] = 0;
        }
        for ( p = q; pe > p && (isspace(*p)) == 0; ++p )
          ;
        if ( p >= pe )
        {
          buf = (char *)ds_free_mem_low(buf, "cmds.c", "ds_source_cmd");
          q = p;
        }
        r = ds_cmd_execute(line, 0);
        ds_free_mem_low(line, "cmds.c", "ds_source_cmd");
        if ( r )
          break;
      }
    }
    p = q;
  }
  ds_free_mem_low(buf, "cmds.c", "ds_source_cmd");
  return r;
}

int __cdecl ds_cd_cmd(int ac, char **av)
{
  char *v3; // eax
  char path[1025]; // [esp+0h] [ebp-408h] BYREF
  char *dir; // [esp+404h] [ebp-4h]
  int aca; // [esp+410h] [ebp+8h]
  char **ava; // [esp+414h] [ebp+Ch]

  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  ava = av + 1;
  if ( aca > 0 && **ava == 45 || aca > 1 )
    return ds_error("Usage: cd [<dir>]");
  if ( aca <= 0 )
    v3 = "~";
  else
    v3 = *ava;
  dir = v3;
  if ( !ds_tilde_expand(path, v3) )
    strcpy(path, dir);
  if ( chdir(path) >= 0 )
    return 0;
  else
    return ds_error("!chdir(%s)", path);
}

static int __cdecl scan_digit_with_dot(char *str, int *psec, int *pusec)
{
  int base; // [esp+4h] [ebp-10h]
  int usec; // [esp+8h] [ebp-Ch]
  int sec; // [esp+Ch] [ebp-8h]
  char *s; // [esp+10h] [ebp-4h]

  s = str;
  sec = 0;
  usec = 0;
  base = 100000;
  if ( (isdigit(*str)) == 0 )
    return ds_error("invalid digit - %s", str);
  while ( (isdigit(*s)) != 0 )
    sec = *s++ + 10 * sec - 48;
  if ( *s == 46 )
  {
    ++s;
    while ( (isdigit(*s)) != 0 )
    {
      usec += base * (*s++ - 48);
      base /= 10;
    }
  }
  if ( *s )
    return ds_error("invalid digit - %s", str);
  *psec = sec;
  *pusec = usec;
  return 0;
}

int __cdecl ds_if_cmd(int ac, char **av)
{
  char *v3; // eax
  char v4; // dl
  char *v5; // [esp-4h] [ebp-20h]
  int n; // [esp+4h] [ebp-18h]
  int n_1; // [esp+4h] [ebp-18h]
  int v8; // [esp+8h] [ebp-14h]
  int v9; // [esp+Ch] [ebp-10h]
  char *s; // [esp+10h] [ebp-Ch]
  char *str; // [esp+14h] [ebp-8h]
  unsigned int wv; // [esp+18h] [ebp-4h] BYREF
  int aca; // [esp+24h] [ebp+8h]
  char **ava; // [esp+28h] [ebp+Ch]

  if ( ac <= 0 )
    return 0;
  if ( ac <= 1 )
    return ds_error("Usage: if <expr> <cmd> [elif <expr> <cmd>]... [else <cmd>]");
  if ( ds_eval_word(av[1], &wv) )
    return -1;
  aca = ac - 2;
  ava = av + 2;
  while ( !wv && aca > 0 )
  {
    if ( !strcmp("else", *ava) )
    {
      wv = 1;
    }
    else if ( !strcmp("elif", *ava) )
    {
      if ( aca <= 1 )
        return ds_error("Usage: if <expr> <cmd> [elif <expr> <cmd>]... [else <cmd>]");
      --aca;
      if ( ds_eval_word(*++ava, &wv) )
        return -1;
    }
    --aca;
    ++ava;
  }
  v8 = 0;
  n = 0;
  while ( aca > v8 && strcmp("elif", ava[v8]) && strcmp("else", ava[v8]) )
  {
    v5 = ava[v8++];
    n += strlen(v5) + 1;
  }
  if ( n <= 0 )
    return 0;
  str = (char *)ds_alloc_mem_low("cmds.c", "ds_if_cmd", n);
  if ( !str )
    return -1;
  n_1 = 0;
  s = str;
  while ( v8 > n_1 )
  {
    strcpy(s, ava[n_1]);
    v3 = &s[strlen(ava[n_1])];
    s = v3 + 1;
    if ( n_1 >= v8 - 1 )
      v4 = 0;
    else
      v4 = 32;
    *v3 = v4;
    ++n_1;
  }
  v9 = ds_cmd_execute(str, 0);
  ds_free_mem_low(str, "cmds.c", "ds_if_cmd");
  return v9;
}

int __cdecl ds_repeat_cmd(int ac, char **av)
{
  int v3; // ebx
  char *v4; // eax
  char v5; // dl
  unsigned int wv; // [esp+4h] [ebp-3Ch] BYREF
  int f_i; // [esp+8h] [ebp-38h]
  char *cond; // [esp+Ch] [ebp-34h]
  char *dest; // [esp+10h] [ebp-30h]
  char *src; // [esp+14h] [ebp-2Ch]
  int v11; // [esp+18h] [ebp-28h]
  int pusec; // [esp+1Ch] [ebp-24h] BYREF
  int psec; // [esp+20h] [ebp-20h] BYREF
  int pv; // [esp+24h] [ebp-1Ch] BYREF
  int n; // [esp+28h] [ebp-18h]
  int tus; // [esp+2Ch] [ebp-14h]
  int ts; // [esp+30h] [ebp-10h]
  int k; // [esp+34h] [ebp-Ch]
  int j; // [esp+38h] [ebp-8h]
  int i; // [esp+3Ch] [ebp-4h]
  int aca; // [esp+48h] [ebp+8h]
  int acb; // [esp+48h] [ebp+8h]
  char **ava; // [esp+4Ch] [ebp+Ch]
  char **avb; // [esp+4Ch] [ebp+Ch]

  pv = -1;
  psec = 1;
  pusec = 0;
  v11 = 0;
  cond = 0;
  f_i = 0;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  for ( ava = av + 1; aca > 0 && **ava == 45; ava = avb + 1 )
  {
    if ( !strcmp("-c", *ava) )
    {
      avb = ava + 1;
      acb = aca - 1;
      if ( acb <= 0 )
        return ds_error("Usage: repeat [-c <cnt>] [-i <sec>] [-while <expr>] <cmd>...");
      if ( ds_eval_word(*avb, (unsigned int *)&pv) )
        return -1;
    }
    else if ( !strcmp("-i", *ava) )
    {
      avb = ava + 1;
      acb = aca - 1;
      if ( acb <= 0 )
        return ds_error("Usage: repeat [-c <cnt>] [-i <sec>] [-while <expr>] <cmd>...");
      if ( scan_digit_with_dot(*avb, &psec, &pusec) )
        return -1;
      ++f_i;
    }
    else
    {
      if ( strcmp("-while", *ava) )
        return ds_error("Usage: repeat [-c <cnt>] [-i <sec>] [-while <expr>] <cmd>...");
      avb = ava + 1;
      acb = aca - 1;
      if ( acb <= 0 )
        return ds_error("Usage: repeat [-c <cnt>] [-i <sec>] [-while <expr>] <cmd>...");
      cond = *avb;
      if ( !f_i )
      {
        f_i = 1;
        pusec = 0;
        psec = 0;
      }
    }
    aca = acb - 1;
  }
  if ( aca <= 0 )
    return ds_error("Usage: repeat [-c <cnt>] [-i <sec>] [-while <expr>] <cmd>...");
  if ( !f_i )
  {
    psec = 1;
    pusec = 0;
  }
  n = 0;
  for ( i = 0; aca > i; ++i )
  {
    v3 = n + 1;
    n = strlen(ava[i]) + v3;
  }
  src = (char *)ds_alloc_mem_low("cmds.c", "ds_repeat_cmd", n);
  if ( !src )
    return -1;
  i = 0;
  dest = src;
  while ( aca > i )
  {
    strcpy(dest, ava[i]);
    v4 = &dest[strlen(ava[i])];
    dest = v4 + 1;
    if ( i >= aca - 1 )
      v5 = 0;
    else
      v5 = 32;
    *v4 = v5;
    ++i;
  }
  i = 0;
  while ( pv < 0 || pv > i )
  {
    if ( cond )
    {
      if ( ds_eval_word(cond, &wv) )
      {
        v11 = -1;
        break;
      }
      if ( !wv )
      {
        v11 = 0;
        break;
      }
    }
    dest = (char *)ds_alloc_mem_low("cmds.c", "ds_repeat_cmd", n);
    if ( !dest )
    {
      v11 = -1;
      break;
    }
    strcpy(dest, src);
    v11 = ds_cmd_execute(dest, 0);
    ds_free_mem_low(dest, "cmds.c", "ds_repeat_cmd");
    if ( v11 < 0 )
      break;
    if ( pv < 0 || i < pv - 1 )
    {
      ds_abort_input = 0;
      j = psec;
      k = pusec;
      while ( 1 )
      {
        if ( psec || pusec )
        {
          if ( j <= 0 )
          {
            ts = 0;
            tus = k;
            k = 0;
          }
          else
          {
            --j;
            ts = 1;
            tus = 0;
          }
        }
        else
        {
          tus = 0;
          ts = 0;
        }
        v11 = ds_select_desc(ts, tus);
        if ( v11 < 0 || ds_abort_input )
          break;
        if ( !v11 && ds_idle_func )
          ds_idle_func();
        if ( j <= 0 && k <= 0 )
        {
          v11 = 0;
          break;
        }
      }
      if ( ds_abort_input )
      {
        v11 = -1;
        break;
      }
    }
    if ( pv >= 0 )
      ++i;
    v11 = 0;
  }
  ds_free_mem_low(src, "cmds.c", "ds_repeat_cmd");
  return v11;
}

static int __cdecl ds_status_cmd(int ac, char **av)
{
  int f_mem; // [esp+0h] [ebp-8h]
  int f_desc; // [esp+4h] [ebp-4h]
  int aca; // [esp+10h] [ebp+8h]
  char **ava; // [esp+14h] [ebp+Ch]

  f_desc = 0;
  f_mem = 0;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  for ( ava = av + 1; aca > 0 && **ava == 45; ++ava )
  {
    if ( !strcmp("-m", *ava) )
    {
      ++f_mem;
    }
    else
    {
      if ( strcmp("-d", *ava) )
        return ds_error("Usage: show status [-m] [-d]");
      ++f_desc;
    }
    --aca;
  }
  if ( !f_desc && !f_mem )
  {
    f_desc = 1;
    f_mem = 1;
  }
  if ( f_desc )
    ds_status_desc();
  if ( f_mem )
    ds_status_mem();
  return 0;
}

static int __cdecl ds_history_cmd(int ac, char **av)
{
  DS_HIST *hp; // [esp+0h] [ebp-4h]
  char **ava; // [esp+10h] [ebp+Ch]

  if ( ac <= 0 )
    return 0;
  ava = av + 1;
  if ( ac - 1 > 0 && **ava == 45 )
    return ds_error("Usage: show history");
  for ( hp = ds_histbuf.head; hp; hp = hp->forw )
    ds_printf(" %4d %s\n", hp->no, hp->buf);
  return 0;
}

int __cdecl ds_add_show_arg(char *name, int (__cdecl *func)(int ac, char **av))
{
  SHOW_ARG *tail; // edx
  SHOW_ARG *sa; // [esp+4h] [ebp-4h]

  sa = (SHOW_ARG *)ds_alloc_mem_low("cmds.c", "ds_add_show_arg", sizeof(SHOW_ARG));
  if ( !sa )
    return -1;
  sa->name = name;
  sa->func = func;
  tail = show_arg_list.tail;
  sa->back = show_arg_list.tail;
  if ( tail )
    sa->back->forw = sa;
  else
    show_arg_list.head = sa;
  sa->forw = 0;
  show_arg_list.tail = sa;
  return 0;
}

int __cdecl ds_show_cmd(int ac, char **av)
{
  SHOW_ARG *sa; // [esp+4h] [ebp-Ch]
  SHOW_ARG *sa_1; // [esp+4h] [ebp-Ch]
  char *tmp[2]; // [esp+8h] [ebp-8h] BYREF

  if ( ac <= 0 )
    return 0;
  if ( ac > 1 && !strcmp("log", av[1]) )
    return ds_log_cmd(ac - 1, av + 1);
  if ( ac > 1 && !strcmp("status", av[1]) )
    return ds_status_cmd(ac - 1, av + 1);
  if ( ac > 1 && !strcmp("history", av[1]) )
    return ds_history_cmd(ac - 1, av + 1);
  if ( ac > 1 )
  {
    for ( sa = show_arg_list.head; sa; sa = sa->forw )
    {
      if ( !strcmp(sa->name, av[1]) )
        return sa->func(ac - 1, av + 1);
    }
  }
  tmp[0] = "";
  tmp[1] = "-help";
  ds_log_cmd(2, tmp);
  ds_status_cmd(2, tmp);
  ds_history_cmd(2, tmp);
  for ( sa_1 = show_arg_list.head; sa_1; sa_1 = sa_1->forw )
    sa_1->func(2, tmp);
  return -1;
}

int __cdecl ds_quit_cmd(int ac, char **av)
{
  if ( ac <= 0 )
    return 0;
  if ( ac == 1 )
    return ds_exit(0);
  return ds_error("Usage: quit");
}

int __cdecl ds_add_help(char *name, int (__cdecl *func)(char *name))
{
  HELP_ARG *tail; // edx
  HELP_ARG *ha; // [esp+4h] [ebp-4h]

  ha = (HELP_ARG *)ds_alloc_mem_low("cmds.c", "ds_add_help", sizeof(HELP_ARG));
  if ( !ha )
    return -1;
  ha->name = name;
  ha->func = func;
  tail = help_arg_list.tail;
  ha->back = help_arg_list.tail;
  if ( tail )
    ha->back->forw = ha;
  else
    help_arg_list.head = ha;
  ha->forw = 0;
  help_arg_list.tail = ha;
  return 0;
}

int __cdecl ds_help_cmd(int ac, char **av)
{
  size_t v3; // [esp-Ch] [ebp-30h]
  char *arg; // [esp-8h] [ebp-2Ch]
  char *help; // [esp-4h] [ebp-28h]
  HELP_ARG *ha; // [esp+8h] [ebp-1Ch]
  HELP_ARG *ha_1; // [esp+8h] [ebp-1Ch]
  HELP_ARG *ha_2; // [esp+8h] [ebp-1Ch]
  int na; // [esp+Ch] [ebp-18h]
  int n; // [esp+10h] [ebp-14h]
  int i_3; // [esp+14h] [ebp-10h]
  int f_all; // [esp+18h] [ebp-Ch]
  CMD_ENTRY *q; // [esp+1Ch] [ebp-8h]
  CMD_ENTRY *p; // [esp+20h] [ebp-4h]
  CMD_ENTRY *p_1; // [esp+20h] [ebp-4h]
  int aca; // [esp+2Ch] [ebp+8h]
  char **ava; // [esp+30h] [ebp+Ch]

  f_all = 0;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  ava = av + 1;
  if ( aca > 0 && **ava == 45 )
    return ds_error("Usage: help [<cmd>]...");
  if ( aca <= 0 || aca == 1 && !strcmp("internal-summary", *ava) )
  {
    if ( aca <= 0 )
    {
      for ( ha = help_arg_list.head; ha; ha = ha->forw )
      {
        if ( !ha->name && ha->func("") )
          return 0;
      }
    }
    for ( p = cmd_list.head; p; p = p->forw )
    {
      if ( p->f_enable )
      {
        n = 0;
        for ( q = p->forw; q && q->f_enable && !strcmp(p->arg, q->arg) && !strcmp(p->help, q->help); q = q->forw )
          ++n;
        if ( n )
        {
          ds_printf("%s", p->name);
          na = cmd_max_na - strlen(p->name) - 1;
          for ( i_3 = 0; n > i_3 && p; ++i_3 )
          {
            ds_printf("/%s", p->forw->name);
            na = na - 1 - strlen(p->forw->name);
            if ( na < 0 )
              na = 0;
            if ( i_3 == n - 1 )
              ds_printf(" %-*s  %s", na, p->forw->arg, p->forw->help);
            p = p->forw;
          }
          ds_printf("\n");
        }
        else
        {
          help = p->help;
          arg = p->arg;
          v3 = cmd_max_na - strlen(p->name) - 1;
          ds_printf("%s %-*s  %s\n", p->name, v3, arg, help);
        }
      }
    }
  }
  else
  {
LABEL_40:
    if ( aca > 0 )
    {
      if ( !strcmp("all", *ava) )
        ++f_all;
      for ( ha_1 = help_arg_list.head; ha_1; ha_1 = ha_1->forw )
      {
        if ( !ha_1->name && ha_1->func(*ava) )
        {
          if ( aca > 1 )
            ds_printf("\n");
LABEL_65:
          --aca;
          ++ava;
          goto LABEL_40;
        }
      }
      for ( p_1 = cmd_list.head; p_1; p_1 = p_1->forw )
      {
        if ( f_all || !strcmp(*ava, p_1->name) )
          ds_printf("%s %s    %s\n", p_1->name, p_1->arg, p_1->help);
      }
      for ( ha_2 = help_arg_list.head; ha_2; ha_2 = ha_2->forw )
      {
        if ( ha_2->name && (f_all || !strcmp(*ava, ha_2->name)) )
          ha_2->func(*ava);
      }
      if ( !f_all )
        goto LABEL_65;
    }
  }
  return 0;
}

void __cdecl ds_cmd_install(char *name, char *arg, char *help, int (__cdecl *func)(int ac, char **av))
{
  CMD_ENTRY *tail; // edx
  size_t v5; // esi
  int na; // [esp+8h] [ebp-8h]
  CMD_ENTRY *p; // [esp+Ch] [ebp-4h]

  p = (CMD_ENTRY *)ds_alloc_mem_low("cmds.c", "ds_cmd_install", sizeof(CMD_ENTRY));
  if ( !p )
    ds_exit(135);
  p->name = name;
  p->arg = arg;
  p->help = help;
  p->func = func;
  p->f_enable = 1;
  tail = cmd_list.tail;
  p->back = cmd_list.tail;
  if ( tail )
    p->back->forw = p;
  else
    cmd_list.head = p;
  p->forw = 0;
  cmd_list.tail = p;
  v5 = strlen(p->name);
  na = strlen(p->arg) + v5 + 1;
  if ( na > cmd_max_na )
    cmd_max_na = na;
}

void __cdecl ds_cmd_control(char *name, int f_enable)
{
  CMD_ENTRY *p; // [esp+0h] [ebp-4h]

  for ( p = cmd_list.head; p; p = p->forw )
  {
    if ( !strcmp(name, p->name) )
      p->f_enable = f_enable;
  }
}

void __cdecl ds_opt_standard_init()
{
  char tmp[1024]; // [esp+0h] [ebp-400h] BYREF

  memset(tmp, 0, sizeof(tmp));

  ds_opt_hex_radix = ds_set_option("hex_radix", 1, 0, 1, 1);
  ds_opt_log_total_size = ds_set_option("log_total_size", 2, 0, 0x20000, 1);
  ds_opt_log_packet_size = ds_set_option("log_packet_size", 2, 0, 128, 1);
  ds_sprintf(tmp, "~/.%s_history", ds_program_name);
  ds_opt_histfile = ds_set_option("histfile", 3, tmp, 0, 1);
  ds_opt_histfilesize = ds_set_option("histfilesize", 2, 0, 256, 1);
  ds_opt_histsize = ds_set_option("histsize", 2, 0, 256, 1);
  ds_opt_tty_max_size = ds_set_option("tty_max_size", 2, 0, 0x800000, 1);
}

void __cdecl ds_cmd_standard_install(int f_shell)
{
  SHOW_ARG *sa; // [esp+0h] [ebp-4h]

  ds_cmd_install("set", "[all] [<var>[=<val>]]", "show/set options", ds_set_cmd);
  ds_cmd_install("alias", "[-r] [<name> [<value>]]", "show/set alias", ds_alias_cmd);
  ds_cmd_install("subst", "[-r] [<pattern> <replace>...]", "show/set substitution", ds_subst_cmd);
  ds_cmd_install("source", "<fname>", "read and execute commands", ds_source_cmd);
  if ( f_shell )
    ds_cmd_install("!", "[<cmd> [<args>]...]]", "invoke shell", ds_shell_cmd);
  ds_cmd_install("cd", "[<dir>]", "change directory", ds_cd_cmd);
  ds_cmd_install("printf", "\"<fmt>\" [<args>]...", "format and print", ds_printf_cmd);
  ds_cmd_install("record", "[[-a] <fname>]", "start/stop recording", ds_record_cmd);
  ds_cmd_install("repeat", "[-c <cnt>] [-i <sec>] [-while <expr>] <cmd>...", "repeat commands", ds_repeat_cmd);
  ds_cmd_install("if", "<expr> <cmd> [elif <expr> <cmd>]... [else <cmd>]", "conditional executing", ds_if_cmd);
  strcpy(show_arg_77, "[log|status|history");
  for ( sa = show_arg_list.head; sa; sa = sa->forw )
  {
    strcat(show_arg_77, "|");
    strcat(show_arg_77, sa->name);
  }
  strcat(show_arg_77, "]...");
  ds_cmd_install("show", show_arg_77, "show log/status/history", ds_show_cmd);
  ds_cmd_install("quit", "", "quit", ds_quit_cmd);
  ds_cmd_install("help", "[<cmd>]...", "print help", ds_help_cmd);
}

static int __cdecl ds_enable_cmd(int ac, char **av)
{
  CMD_ENTRY *p; // [esp+0h] [ebp-4h]
  int aca; // [esp+Ch] [ebp+8h]
  char **ava; // [esp+10h] [ebp+Ch]

  if ( ac <= 0 )
    return 0;
  if ( ac > 1 && !strcmp("-help", av[1]) )
    return ds_error("Usage: enable [cmds]...\n");
  aca = ac - 1;
  ava = av + 1;
  while ( aca > 0 )
  {
    for ( p = cmd_list.head; p; p = p->forw )
    {
      if ( !strcmp(*ava, p->name) )
      {
        p->f_enable = 1;
        break;
      }
    }
    if ( !p )
      return ds_error("%s: Command not found\n", *ava);
    --aca;
    ++ava;
  }
  return 0;
}

static int __cdecl ds_disable_cmd(int ac, char **av)
{
  CMD_ENTRY *p; // [esp+0h] [ebp-4h]
  int aca; // [esp+Ch] [ebp+8h]
  char **ava; // [esp+10h] [ebp+Ch]

  if ( ac <= 0 )
    return 0;
  if ( ac > 1 && !strcmp("-help", av[1]) )
    return ds_error("Usage: disable [cmds]...\n");
  aca = ac - 1;
  ava = av + 1;
  while ( aca > 0 )
  {
    for ( p = cmd_list.head; p; p = p->forw )
    {
      if ( !strcmp(*ava, p->name) )
      {
        p->f_enable = 0;
        break;
      }
    }
    if ( !p )
      return ds_error("%s: Command not found\n", *ava);
    --aca;
    ++ava;
  }
  return 0;
}

void __cdecl ds_cmd_xxxable_install()
{
  ds_cmd_install("enable", "[cmds]...", "enable commands", ds_enable_cmd);
  ds_cmd_install("disable", "[cmds]...", "disable commands", ds_disable_cmd);
}

int __cdecl ds_alias_cmd(int ac, char **av)
{
  size_t v3; // eax
  char *v4; // eax
  char *v5; // eax
  char *v6; // eax
  char v7; // dl
  ALIAS *tail; // edx
  char *s; // [esp+18h] [ebp-18h]
  int n_3; // [esp+1Ch] [ebp-14h]
  int i; // [esp+20h] [ebp-10h]
  int i_1; // [esp+20h] [ebp-10h]
  int f_r; // [esp+24h] [ebp-Ch]
  ALIAS *q; // [esp+28h] [ebp-8h]
  ALIAS *p; // [esp+2Ch] [ebp-4h]
  ALIAS *p_1; // [esp+2Ch] [ebp-4h]
  ALIAS *p_2; // [esp+2Ch] [ebp-4h]
  ALIAS *p_3; // [esp+2Ch] [ebp-4h]
  int aca; // [esp+38h] [ebp+8h]
  char **ava; // [esp+3Ch] [ebp+Ch]

  f_r = 0;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  for ( ava = av + 1; aca > 0 && **ava == 45; ++ava )
  {
    if ( strcmp("-r", *ava) )
      return ds_error("Usage: alias [-r] [<name> [<value]]");
    ++f_r;
    --aca;
  }
  if ( aca )
  {
    if ( aca == 1 )
    {
      for ( p_2 = aliases.head; p_2 && strcmp(*ava, p_2->name); p_2 = p_2->forw )
        ;
      if ( p_2 )
      {
        if ( f_r )
        {
          if ( p_2->forw )
            p_2->forw->back = p_2->back;
          else
            aliases.tail = p_2->back;
          if ( p_2->back )
            p_2->back->forw = p_2->forw;
          else
            aliases.head = p_2->forw;
          ds_free_mem_low(p_2->name, "cmds.c", "ds_alias_cmd");
          ds_free_mem_low(p_2->value, "cmds.c", "ds_alias_cmd");
          ds_free_mem_low(p_2, "cmds.c", "ds_alias_cmd");
        }
        else
        {
          ds_printf("alias %s %s\n", p_2->name, p_2->value);
        }
        return 0;
      }
      else
      {
        return ds_error("alias: %s not found", *ava);
      }
    }
    else
    {
      for ( p_3 = aliases.head; p_3 && strcmp(*ava, p_3->name); p_3 = p_3->forw )
        ;
      if ( p_3 )
      {
        if ( p_3->forw )
          p_3->forw->back = p_3->back;
        else
          aliases.tail = p_3->back;
        if ( p_3->back )
          p_3->back->forw = p_3->forw;
        else
          aliases.head = p_3->forw;
        ds_free_mem_low(p_3->value, "cmds.c", "ds_alias_cmd");
      }
      else
      {
        p_3 = (ALIAS *)ds_alloc_mem_low("cmds.c", "ds_alias_cmd", sizeof(ALIAS));
        if ( !p_3 )
          return -1;
        v3 = strlen(*ava);
        v4 = (char *)ds_alloc_mem_low("cmds.c", "ds_alias_cmd", v3 + 1);
        p_3->name = v4;
        if ( !v4 )
        {
          ds_free_mem_low(p_3, "cmds.c", "ds_alias_cmd");
          return -1;
        }
        strcpy(p_3->name, *ava);
      }
      n_3 = 0;
      for ( i = 1; aca > i; ++i )
        n_3 += strlen(ava[i]) + 1;
      v5 = (char *)ds_alloc_mem_low("cmds.c", "ds_alias_cmd", n_3);
      p_3->value = v5;
      s = v5;
      if ( v5 )
      {
        for ( i_1 = 1; aca > i_1; ++i_1 )
        {
          strcpy(s, ava[i_1]);
          v6 = &s[strlen(ava[i_1])];
          s = v6 + 1;
          if ( i_1 >= aca - 1 )
            v7 = 0;
          else
            v7 = 32;
          *v6 = v7;
        }
        tail = aliases.tail;
        p_3->back = aliases.tail;
        if ( tail )
          p_3->back->forw = p_3;
        else
          aliases.head = p_3;
        p_3->forw = 0;
        aliases.tail = p_3;
        return 0;
      }
      else
      {
        ds_free_mem_low(p_3->name, "cmds.c", "ds_alias_cmd");
        ds_free_mem_low(p_3, "cmds.c", "ds_alias_cmd");
        return -1;
      }
    }
  }
  else
  {
    if ( f_r )
    {
      for ( p = aliases.head; p; p = q )
      {
        q = p->forw;
        ds_free_mem_low(p->name, "cmds.c", "ds_alias_cmd");
        ds_free_mem_low(p->value, "cmds.c", "ds_alias_cmd");
        ds_free_mem_low(p, "cmds.c", "ds_alias_cmd");
      }
      aliases.tail = 0;
      aliases.head = 0;
    }
    else
    {
      for ( p_1 = aliases.head; p_1; p_1 = p_1->forw )
        ds_printf("alias %s %s\n", p_1->name, p_1->value);
    }
    return 0;
  }
}

int __cdecl ds_cmd_call(int ac, char **av, int f_repeat)
{
  int (__cdecl *func)(int, char **); // eax
  size_t v5; // [esp-4h] [ebp-10h]
  size_t v6; // [esp-4h] [ebp-10h]
  int r; // [esp+4h] [ebp-8h]
  CMD_ENTRY *p; // [esp+8h] [ebp-4h]
  CMD_ENTRY *p_1; // [esp+8h] [ebp-4h]
  CMD_ENTRY *p_2; // [esp+8h] [ebp-4h]

  if ( ac > 0 )
  {
    for ( p = cmd_list.head; p && strcmp(*av, p->name); p = p->forw )
      ;
    if ( p )
    {
      if ( p->f_enable )
      {
        r = p->func(ac, av);
        if ( f_repeat )
        {
          if ( r < 0 )
            func = 0;
          else
            func = p->func;
          last_func_88 = func;
        }
        return r;
      }
      else
      {
        return ds_error("%s: Command disabled", *av);
      }
    }
    else
    {
      ds_printf("%s: ", ds_program_name);
      ds_printf("%s: Command not found", *av);
      for ( p_1 = cmd_list.head; p_1; p_1 = p_1->forw )
      {
        v5 = strlen(*av);
        if ( !strncmp(*av, p_1->name, v5) )
          break;
      }
      if ( p_1 )
      {
        ds_printf(" (");
        for ( p_2 = cmd_list.head; p_2; p_2 = p_2->forw )
        {
          v6 = strlen(*av);
          if ( !strncmp(*av, p_2->name, v6) )
            ds_printf(" %s", p_2->name);
        }
        ds_printf(" )");
      }
      ds_printf("\n");
      return -1;
    }
  }
  else if ( f_repeat && last_func_88 )
  {
    return last_func_88(0, 0);
  }
  else
  {
    return 0;
  }
}

static ALIAS *__cdecl alias(char *buf, int nbuf, int ac, char **av)
{
  char *v5; // eax
  int v6; // ebx
  size_t v7; // eax
  int v8; // ebx
  size_t v9; // eax
  int i_3; // [esp+8h] [ebp-414h] BYREF
  char *tp; // [esp+Ch] [ebp-410h]
  char token[1024]; // [esp+10h] [ebp-40Ch] BYREF
  char *sp; // [esp+410h] [ebp-Ch]
  char *dp; // [esp+414h] [ebp-8h]
  ALIAS *p; // [esp+418h] [ebp-4h]

  for ( p = aliases.head; p && strcmp(*av, p->name); p = p->forw )
    ;
  if ( !p || p->used )
    return 0;
  dp = buf;
  sp = p->value;
  while ( 1 )
  {
    while ( 1 )
    {
      while ( 1 )
      {
        if ( !*sp || dp - buf >= nbuf - 1 )
        {
          *dp = 0;
          return p;
        }
        if ( *sp == 36 && sp[1] == 123 )
          break;
        *dp++ = *sp++;
      }
      tp = token;
      for ( sp += 2; *sp && *sp != 125; *tp++ = *sp++ )
        ;
      *tp = 0;
      v5 = sp++;
      if ( *v5 != 125 )
      {
        ds_error("alias: unblanced {}");
        return 0;
      }
      if ( strcmp("*", token) )
        break;
      for ( i_3 = 1; ac > i_3; ++i_3 )
      {
        v6 = dp - buf;
        if ( strlen(av[i_3]) + v6 < nbuf - 1 )
        {
          strcpy(dp, av[i_3]);
          v7 = strlen(av[i_3]);
          dp += v7;
          if ( i_3 < ac - 1 )
            *dp++ = 32;
        }
      }
    }
    if ( (isdigit(token[0])) == 0 || ds_scan_digit_word(token, (unsigned int *)&i_3) )
      break;
    if ( i_3 >= 0 && ac > i_3 )
    {
      v8 = dp - buf;
      if ( strlen(av[i_3]) + v8 < nbuf - 1 )
      {
        strcpy(dp, av[i_3]);
        v9 = strlen(av[i_3]);
        dp += v9;
        if ( i_3 < ac - 1 )
          *dp++ = 32;
      }
    }
  }
  ds_error("alias: {%s} ???", token);
  return 0;
}

static int __cdecl ds_cmd_execute_internal(char *s, int f_repeat)
{
  char v3; // al
  size_t v4; // eax
  size_t v5; // eax
  ALIAS *p; // [esp+0h] [ebp-9A4h]
  char sbuf[1024]; // [esp+4h] [ebp-9A0h] BYREF
  char ch; // [esp+407h] [ebp-59Dh]
  char buf[1024]; // [esp+408h] [ebp-59Ch] BYREF
  char *av[101]; // [esp+808h] [ebp-19Ch] BYREF
  int ac; // [esp+99Ch] [ebp-8h] BYREF
  int r; // [esp+9A0h] [ebp-4h]

  r = 0;
  while ( *s && (isspace(*s)) != 0 )
    ++s;
  if ( *s )
  {
    if ( *s != 92 )
      s = ds_subst(sbuf, 1024, s);
    while ( 1 )
    {
      do
      {
        if ( !*s )
          return r;
        s = ds_scan_args(s, &ac, av, 100);
      }
      while ( ac <= 0 );
      v3 = *av[0];
      ch = v3;
      if ( v3 == 39 )
        goto LABEL_20;
      if ( v3 > 39 )
      {
        if ( v3 == 40 )
        {
          ch = 41;
          goto LABEL_20;
        }
LABEL_29:
        if ( *av[0] == 92 )
        {
          while ( *av[0] == 92 )
            ++av[0];
          if ( *av[0] )
          {
            r = ds_cmd_call(ac, av, f_repeat);
            if ( r < 0 )
              return r;
          }
        }
        else
        {
          p = alias(buf, 1024, ac, av);
          if ( p )
          {
            ++p->used;
            r = ds_cmd_execute_internal(buf, f_repeat);
            p->used = 0;
            if ( r < 0 )
              return -1;
          }
          else
          {
            r = ds_cmd_call(ac, av, f_repeat);
            if ( r < 0 )
              return r;
          }
        }
      }
      else
      {
        if ( v3 != 34 )
          goto LABEL_29;
LABEL_20:
        if ( strlen(av[0]) <= 1 )
          return ds_error("unblanced %c", ch);
        v4 = strlen(av[0]);
        if ( ch != av[0][v4 - 1] )
          return ds_error("unblanced %c", ch);
        v5 = strlen(av[0]);
        av[0][v5 - 1] = 0;
        strcpy(buf, av[0] + 1);
        for ( r = 1; ac > r; ++r )
        {
          strcat(buf, " ");
          strcat(buf, av[r]);
        }
        r = ds_cmd_execute_internal(buf, 1);
        if ( r < 0 )
          return -1;
      }
    }
  }
  if ( f_repeat )
    return ds_cmd_call(0, 0, f_repeat);
  else
    return 0;
}

int __cdecl ds_cmd_execute(char *s, int f_repeat)
{
  ALIAS *p; // [esp+0h] [ebp-4h]

  for ( p = aliases.head; p; p = p->forw )
    p->used = 0;
  return ds_cmd_execute_internal(s, f_repeat);
}

static int __cdecl ds_path_completion(DS_HISTBUF *hb, char *path)
{
  char *v2; // eax
  struct stat stbuf; // [esp+4h] [ebp-C8Ch] BYREF
  int col; // [esp+5Ch] [ebp-C34h]
  int r; // [esp+60h] [ebp-C30h]
  int lns; // [esp+64h] [ebp-C2Ch]
  int ns; // [esp+68h] [ebp-C28h]
  int nm; // [esp+6Ch] [ebp-C24h]
  int n; // [esp+70h] [ebp-C20h]
  struct dirent *dirp_3; // [esp+74h] [ebp-C1Ch]
  DIR *dp; // [esp+78h] [ebp-C18h]
  char dname[1025]; // [esp+7Ch] [ebp-C14h] BYREF
  char *p; // [esp+480h] [ebp-810h]
  char cn[1025]; // [esp+484h] [ebp-80Ch] BYREF
  char *name; // [esp+888h] [ebp-408h]
  char buf[1025]; // [esp+88Ch] [ebp-404h] BYREF

  r = 0;
  ds_tilde_expand(buf, path);
  name = strrchr(buf, 47);
  if ( name )
  {
    *name++ = 0;
    if ( buf[0] )
      v2 = buf;
    else
      v2 = "/";
    strcpy(dname, v2);
  }
  else
  {
    strcpy(dname, ".");
    name = path;
  }
  dp = opendir(dname);
  if ( !dp )
    return 0;
  n = strlen(name);
  nm = 0;
  while ( 1 )
  {
    dirp_3 = readdir(dp);
    if ( !dirp_3 )
      break;
    if ( n > 0 && !strncmp(name, dirp_3->d_name, n) || !n && dirp_3->d_name[0] != 46 )
    {
      if ( ++nm == 1 )
      {
        strcpy(cn, dirp_3->d_name);
        ns = strlen(dirp_3->d_name);
      }
      else
      {
        lns = ns;
        for ( ns = 0; lns > ns && dirp_3->d_name[ns] == cn[ns]; ++ns )
          ;
      }
    }
  }
  if ( nm )
  {
    if ( nm == 1 )
    {
      for ( p = &cn[n]; *p; ++p )
        ds_editline(hb, *p, 0);
      strcpy(buf, dname);
      strcat(buf, "/");
      strcat(buf, cn);
      if ( stat(buf, &stbuf) >= 0 && (stbuf.st_mode & 0xF000) == 0x4000 )
        ds_editline(hb, 47, 0);
      r = 1;
    }
    else
    {
      for ( p = &cn[n]; *p && p < &cn[ns]; ++p )
        ds_editline(hb, *p, 0);
      ds_printf("\n");
      col = 0;
      rewinddir(dp);
      while ( 1 )
      {
        dirp_3 = readdir(dp);
        if ( !dirp_3 )
          break;
        if ( n > 0 && !strncmp(name, dirp_3->d_name, n) || !n && dirp_3->d_name[0] != 46 )
        {
          if ( col + strlen(dirp_3->d_name) + 1 > 0x4F )
          {
            ds_printf("\n");
            col = 0;
          }
          col += ds_printf(" %s", dirp_3->d_name);
        }
      }
      r = -1;
    }
  }
  closedir(dp);
  return r;
}

static int __cdecl ds_symbol_completion(DS_HISTBUF *hb, char *name)
{
  if ( ds_symbol_completion_func )
    return ds_symbol_completion_func(hb, name);
  else
    return 0;
}

static int __cdecl ds_get_last_keyword(char *buf, char *str)
{
  char *p; // [esp+4h] [ebp-8h]
  char *s; // [esp+8h] [ebp-4h]

  for ( s = &str[strlen(str) - 1]; s > str && (isspace(*s)) == 0; --s )
    ;
  if ( (isspace(*s)) != 0 )
    ++s;
  p = buf;
  while ( *s && *s != 59 && *s != 35 && (isspace(*s)) == 0 )
    *p++ = *s++;
  *p = 0;
  if ( *s || !*buf )
    return 0;
  else
    return strlen(buf);
}

static int __cdecl ds_set_completion(DS_HISTBUF *hb, char *str)
{
  int col; // [esp+0h] [ebp-420h]
  int col_1; // [esp+0h] [ebp-420h]
  int lns; // [esp+4h] [ebp-41Ch]
  int ns; // [esp+8h] [ebp-418h]
  int nm; // [esp+Ch] [ebp-414h]
  int n; // [esp+10h] [ebp-410h]
  char *cn; // [esp+14h] [ebp-40Ch]
  char *p; // [esp+18h] [ebp-408h]
  char *p_1; // [esp+18h] [ebp-408h]
  char *p_2; // [esp+18h] [ebp-408h]
  DS_OPTION *op; // [esp+1Ch] [ebp-404h]
  DS_OPTION *op_1; // [esp+1Ch] [ebp-404h]
  DS_OPTION *op_2; // [esp+1Ch] [ebp-404h]
  char name[1024]; // [esp+20h] [ebp-400h] BYREF

  n = ds_get_last_keyword(name, str);
  if ( n > 0 )
  {
    nm = 0;
    for ( op_1 = ds_option_list.head; op_1; op_1 = op_1->forw )
    {
      p = op_1->name;
      if ( !strncmp(name, p, n) )
      {
        if ( ++nm == 1 )
        {
          cn = p;
          ns = strlen(p);
        }
        else
        {
          lns = ns;
          for ( ns = 0; lns > ns && p[ns] == cn[ns]; ++ns )
            ;
        }
      }
    }
    if ( nm )
    {
      if ( nm == 1 )
      {
        for ( p_1 = &cn[n]; *p_1; ++p_1 )
          ds_editline(hb, *p_1, 0);
        return 1;
      }
      else
      {
        for ( p_2 = &cn[n]; *p_2 && p_2 < &cn[ns]; ++p_2 )
          ds_editline(hb, *p_2, 0);
        ds_printf("\n");
        col_1 = 0;
        for ( op_2 = ds_option_list.head; op_2; op_2 = op_2->forw )
        {
          if ( !strncmp(name, op_2->name, n) )
          {
            if ( col_1 + strlen(op_2->name) + 1 > 0x4F )
            {
              ds_printf("\n");
              col_1 = 0;
            }
            col_1 += ds_printf(" %s", op_2->name);
          }
        }
        return -1;
      }
    }
    else
    {
      return 0;
    }
  }
  else
  {
    ds_printf("\n");
    col = 0;
    for ( op = ds_option_list.head; op; op = op->forw )
    {
      if ( col + strlen(op->name) + 1 > 0x4F )
      {
        ds_printf("\n");
        col = 0;
      }
      col += ds_printf(" %s", op->name);
    }
    return -1;
  }
}

static int __cdecl ds_help_completion(DS_HISTBUF *hb, char *str)
{
  int col; // [esp+4h] [ebp-420h]
  int col_1; // [esp+4h] [ebp-420h]
  int lns; // [esp+8h] [ebp-41Ch]
  int ns; // [esp+Ch] [ebp-418h]
  int nm; // [esp+10h] [ebp-414h]
  int n; // [esp+14h] [ebp-410h]
  char *cn; // [esp+18h] [ebp-40Ch]
  char *p; // [esp+1Ch] [ebp-408h]
  char *p_1; // [esp+1Ch] [ebp-408h]
  char *p_2; // [esp+1Ch] [ebp-408h]
  CMD_ENTRY *cp; // [esp+20h] [ebp-404h]
  CMD_ENTRY *cp_1; // [esp+20h] [ebp-404h]
  CMD_ENTRY *cp_2; // [esp+20h] [ebp-404h]
  char name[1024]; // [esp+24h] [ebp-400h] BYREF

  n = ds_get_last_keyword(name, str);
  if ( ds_help_completion_func )
    return ds_help_completion_func(hb, name);
  if ( n > 0 )
  {
    nm = 0;
    for ( cp_1 = cmd_list.head; cp_1; cp_1 = cp_1->forw )
    {
      p = cp_1->name;
      if ( !strncmp(name, p, n) )
      {
        if ( ++nm == 1 )
        {
          cn = p;
          ns = strlen(p);
        }
        else
        {
          lns = ns;
          for ( ns = 0; lns > ns && p[ns] == cn[ns]; ++ns )
            ;
        }
      }
    }
    if ( nm )
    {
      if ( nm == 1 )
      {
        for ( p_1 = &cn[n]; *p_1; ++p_1 )
          ds_editline(hb, *p_1, 0);
        return 1;
      }
      else
      {
        for ( p_2 = &cn[n]; *p_2 && p_2 < &cn[ns]; ++p_2 )
          ds_editline(hb, *p_2, 0);
        ds_printf("\n");
        col_1 = 0;
        for ( cp_2 = cmd_list.head; cp_2; cp_2 = cp_2->forw )
        {
          if ( !strncmp(name, cp_2->name, n) )
          {
            if ( col_1 + strlen(cp_2->name) + 1 > 0x4F )
            {
              ds_printf("\n");
              col_1 = 0;
            }
            col_1 += ds_printf(" %s", cp_2->name);
          }
        }
        return -1;
      }
    }
    else
    {
      return 0;
    }
  }
  else
  {
    ds_printf("\n");
    col = 0;
    for ( cp = cmd_list.head; cp; cp = cp->forw )
    {
      if ( col + strlen(cp->name) + 1 > 0x4F )
      {
        ds_printf("\n");
        col = 0;
      }
      col += ds_printf(" %s", cp->name);
    }
    return -1;
  }
}

static int __cdecl ds_path_or_symbol_completion(DS_HISTBUF *hb, char *str)
{
  char v2; // al
  int f_path; // [esp+4h] [ebp-40Ch]
  char *v5; // [esp+8h] [ebp-408h]
  char *s; // [esp+Ch] [ebp-404h]
  char name[1024]; // [esp+10h] [ebp-400h] BYREF

  f_path = 0;
  for ( s = &str[strlen(str) - 1]; s > str && (isspace(*s)) == 0; --s )
    ;
  if ( (isspace(*s)) != 0 )
    ++s;
  v5 = name;
  while ( *s && *s != 59 && *s != 35 && (isspace(*s)) == 0 )
  {
    v2 = *s;
    *v5++ = *s++;
    if ( v2 <= 47 && v2 >= 46 )
      ++f_path;
  }
  *v5 = 0;
  if ( *s || !name[0] )
    return 0;
  if ( f_path )
    return ds_path_completion(hb, name);
  return ds_symbol_completion(hb, name);
}

int __cdecl ds_cmd_completion(DS_HISTBUF *hb, char *s)
{
  int col; // [esp+4h] [ebp-420h]
  int col_1; // [esp+4h] [ebp-420h]
  int lns; // [esp+8h] [ebp-41Ch]
  int i; // [esp+Ch] [ebp-418h]
  int nm; // [esp+10h] [ebp-414h]
  int n; // [esp+14h] [ebp-410h]
  CMD_ENTRY *ce; // [esp+18h] [ebp-40Ch]
  CMD_ENTRY *ce_1; // [esp+18h] [ebp-40Ch]
  CMD_ENTRY *ce_2; // [esp+18h] [ebp-40Ch]
  char *v12; // [esp+1Ch] [ebp-408h]
  char *p; // [esp+20h] [ebp-404h]
  char *p_1; // [esp+20h] [ebp-404h]
  char *p_2; // [esp+20h] [ebp-404h]
  char name[1024]; // [esp+24h] [ebp-400h] BYREF

  while ( *s && (isspace(*s)) != 0 )
    ++s;
  p = name;
  while ( *s && *s != 59 && *s != 35 && (isspace(*s)) == 0 )
    *p++ = *s++;
  *p = 0;
  if ( *s != 32 )
    goto LABEL_62;
  if ( !strcmp("set", name) )
    return ds_set_completion(hb, s);
  if ( !strcmp("help", name) )
    return ds_help_completion(hb, s);
LABEL_62:
  if ( *s )
    return ds_path_or_symbol_completion(hb, s);
  if ( name[0] )
  {
    n = strlen(name);
    nm = 0;
    for ( ce_1 = cmd_list.head; ce_1; ce_1 = ce_1->forw )
    {
      if ( ce_1->f_enable && !strncmp(name, ce_1->name, n) )
      {
        if ( ++nm == 1 )
        {
          v12 = ce_1->name;
          i = strlen(v12);
        }
        else
        {
          lns = i;
          for ( i = 0; lns > i && ce_1->name[i] == v12[i]; ++i )
            ;
        }
      }
    }
    if ( nm )
    {
      if ( nm == 1 )
      {
        for ( p_1 = &v12[n]; *p_1 && *p_1 != 33; ++p_1 )
          ds_editline(hb, *p_1, 0);
        return 1;
      }
      else
      {
        for ( p_2 = &v12[n]; *p_2 && *p_2 != 33 && p_2 < &v12[i]; ++p_2 )
          ds_editline(hb, *p_2, 0);
        ds_printf("\n");
        col_1 = 0;
        for ( ce_2 = cmd_list.head; ce_2; ce_2 = ce_2->forw )
        {
          if ( ce_2->f_enable && !strncmp(name, ce_2->name, n) )
          {
            if ( col_1 + strlen(ce_2->name) + 1 > 0x4F )
            {
              ds_printf("\n");
              col_1 = 0;
            }
            col_1 += ds_printf(" %s", ce_2->name);
          }
        }
        return -1;
      }
    }
    else
    {
      return 0;
    }
  }
  else
  {
    ds_printf("\n");
    col = 0;
    for ( ce = cmd_list.head; ce; ce = ce->forw )
    {
      if ( ce->f_enable )
      {
        if ( col + strlen(ce->name) + 1 > 0x4F )
        {
          ds_printf("\n");
          col = 0;
        }
        col += ds_printf(" %s", ce->name);
      }
    }
    return -1;
  }
}

DSP_BUF *__cdecl ds_cmd_input(DS_DESC *desc, DSP_BUF *db)
{
  int v3; // eax
  char *p; // [esp+0h] [ebp-414h]
  unsigned __int8 *bp; // [esp+8h] [ebp-40Ch]
  unsigned __int8 buf[1024]; // [esp+Ch] [ebp-408h] BYREF
  int len; // [esp+40Ch] [ebp-8h]
  DECI2_HDR *dh; // [esp+410h] [ebp-4h]

  dh = (DECI2_HDR *)db->buf;
  len = *(unsigned __int16 *)db->buf - 12;
  if ( len < 0 )
    return db;
  for ( bp = (unsigned __int8 *)&dh[1].protocol; ; ++bp )
  {
    v3 = len--;
    if ( v3 <= 0 )
      break;
    if ( ds_cmd_executing )
    {
      if ( *bp == 3 || *bp == 7 )
        ++ds_abort_input;
    }
    else
    {
      switch ( ds_editline(&ds_histbuf, (char)*bp, (char *)buf) )
      {
        case 3:
        case 7:
          ds_printf("\n");
          ds_prompt();
          break;
        case 9:
          if ( !*ds_histbuf.ptr && ds_cmd_completion(&ds_histbuf, (char *)buf) < 0 )
          {
            ds_printf("\n");
            ds_prompt();
            p = ds_histbuf.ptr;
            ds_histbuf.ptr = ds_histbuf.buf;
            ds_editline(&ds_histbuf, 12, 0);
            while ( p > ds_histbuf.ptr )
              ds_editline(&ds_histbuf, 6, 0);
          }
          break;
        case 10:
        case 13:
          ds_printf("\n");
          ds_cmd_executing = 1;
          ds_cmd_execute((char *)buf, 1);
          ds_cmd_executing = 0;
          ds_prompt();
          break;
        default:
          continue;
      }
    }
  }
  return ds_free_buf(db);
}

