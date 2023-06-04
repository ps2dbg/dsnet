#include "dsnet_prototypes.h"

int ds_errno;
static CQ kbdq;
static CQ devq;

char *ds_program_name = "";
int (*ds_rdwr_mem_align_func)(int code, int align, int cpuid, int space, unsigned int adr, void *ptr, int len) = NULL;
int (*ds_load_mem_func)(unsigned int adr, void *ptr, int len) = NULL;
int (*ds_store_mem_func)(unsigned int adr, void *ptr, int len) = NULL;
int (*ds_load_quad_reg_func)(char *name, quad *pq) = NULL;
int (*ds_store_quad_reg_func)(char *name, quad qv) = NULL;
int (*ds_symbol_to_value_func)(char *name, unsigned int *pv) = NULL;
int (*ds_symbol_completion_func)(DS_HISTBUF *hb, char *name) = NULL;
int (*ds_help_completion_func)(DS_HISTBUF *hb, char *str) = NULL;
void (*ds_idle_func)() = NULL;
int (*ds_check_reserved_name_func)(char *name) = NULL;

static int *pstatus_by_child = NULL;

static void __cdecl sigchld(int arg);
static int __cdecl read_kbd(int kbd, int escape);
static int __cdecl read_dev(int dev);
static int __cdecl write_dev(int dev);
static int __cdecl write_dsp(int dsp);

int __cdecl ds_rdwr_mem_align(int code, int align, int cpuid, int space, unsigned int adr, void *ptr, int len)
{
  if ( ds_rdwr_mem_align_func )
    return ds_rdwr_mem_align_func(
             (unsigned __int8)code,
             (unsigned __int8)align,
             (unsigned __int16)cpuid,
             (unsigned __int8)space,
             adr,
             ptr,
             len);
  else
    return ds_error("invalid memory reference (rdwr_align)");
}

int __cdecl ds_load_mem(unsigned int adr, void *ptr, int len)
{
  if ( ds_load_mem_func )
    return ds_load_mem_func(adr, ptr, len);
  else
    return ds_error("invalid memory reference (load)");
}

int __cdecl ds_store_mem(unsigned int adr, void *ptr, int len)
{
  if ( ds_store_mem_func )
    return ds_store_mem_func(adr, ptr, len);
  else
    return ds_error("invalid memory reference (store)");
}

int __cdecl ds_load_quad_reg(char *name, quad *pq)
{
  if ( ds_load_quad_reg_func )
    return ds_load_quad_reg_func(name, pq);
  else
    return ds_error("invalid register reference (load quad)");
}

int __cdecl ds_store_quad_reg(char *name, quad qv)
{
  if ( ds_store_quad_reg_func )
    return ds_store_quad_reg_func(name, qv);
  else
    return ds_error("invalid register reference (store quad)");
}

int __cdecl ds_symbol_to_value(char *name, unsigned int *pv)
{
  if ( ds_symbol_to_value_func )
    return ds_symbol_to_value_func(name, pv);
  else
    return ds_error("invalid symbol reference (symbol_to_value)");
}

int __cdecl ds_exit(int code)
{
  int result; // eax

  switch ( code )
  {
    case 130:
      ds_error("\ntarget connection closed");
      break;
    case 131:
      ds_error("cannot open");
      break;
    case 132:
      ds_error("cannot connect");
      break;
    case 133:
      ds_error("cannot listen");
      break;
    case 134:
      ds_error("cannot reset");
      break;
    case 135:
      ds_error("no memory");
      break;
    case 136:
      ds_error("no connectr");
      break;
    case 137:
      ds_error("no resetr");
      break;
    case 138:
      ds_error("no getconfr");
      break;
    case 139:
      ds_error("invalid getconfr");
      break;
    case 141:
      ds_error("no statr");
      break;
    case 142:
      ds_error("no killr");
      break;
    case 143:
      ds_error("cannot stat");
      break;
    case 144:
      ds_error("cannot kill");
      break;
    case 145:
      ds_error("resetted");
      break;
    case 146:
      ds_error("no messager");
      break;
    case 147:
      ds_error("no versionr");
      break;
    case 148:
      ds_error("invalid version");
      break;
    case 149:
      ds_error("cannot poweroff");
      break;
    case 150:
      ds_error("no poweroffr");
      break;
    case 151:
      ds_error("no protocol");
      break;
    default:
      break;
  }
  ds_resume_kbd();
  ds_save_history();
  exit(code);
  return result;
}

int __cdecl ds_exit_invalid_version(char *manager_version, int n, char *client_version)
{
  int v3; // eax
  int v5; // [esp-4h] [ebp-8h]

  ds_printf("Invalid version - dsnetm version ");
  while ( 1 )
  {
    v3 = n--;
    if ( v3 <= 0 )
      break;
    v5 = *manager_version++;
    ds_printf("%C", v5);
  }
  ds_printf(" while %s version %s\n", ds_program_name, client_version);
  ds_printf("*** Please restart dsnetm !!\n");
  ds_exit(255);
  return -1;
}

int __cdecl ds_fork()
{
  int pid; // [esp+0h] [ebp-4h]

#ifndef _WIN32
  pid = fork();
  if ( pid <= 0 )
  {
    if ( pid < 0 )
    {
      ds_error("!fork");
      ds_exit(140);
    }
  }
  else
  {
    ds_printf("Entering server mode (pid=%d)\n", pid);
    ds_exit(0);
  }
  close(0);
  close(1);
  close(2);
  open("/dev/null", 0);
  open("/dev/null", 1);
  open("/dev/null", 1);
#else
  ds_error("!fork");
  ds_exit(140);
#endif
  return 0;
}

#ifndef _WIN32
static void __cdecl sigchld(int arg)
{
  int status; // [esp+4h] [ebp-4h] BYREF

  if ( pstatus_by_child )
  {
    wait(&status);
    *pstatus_by_child = (status & 0xFF00) >> 8;
  }
}
#endif

int __cdecl ds_cmd_execution_for_filesv(char *cmd, int *pstatus)
{
  char *v4; // eax
  int fd; // [esp+0h] [ebp-Ch]
  int pid; // [esp+4h] [ebp-8h]
  char *shell; // [esp+8h] [ebp-4h]

#ifndef _WIN32
  shell = getenv("SHELL");
  if ( !shell )
    shell = "/bin/sh";
  pstatus_by_child = pstatus;
  pid = fork();
  if ( pid < 0 )
    return ds_error("!fork");
  if ( !pid )
  {
    for ( fd = 0; fd <= 255; ++fd )
    {
      if ( fd > 2 )
        close(fd);
    }
    execlp(shell, shell, "-c", cmd, 0);
    v4 = strerror(errno);
    printf("execlp - %s\n", v4);
    exit(1);
  }
  signal(SIGINT, SIG_IGN);
  signal(SIGCHLD /* This was 17 (SIGSTOP), which was incorrect */, (sig_t)sigchld);
  return 0;
#else
  return ds_error("Fork not supported on this platform");
#endif
}

int __cdecl ds_read(int fd, void *ptr, int n)
{
  int r; // [esp+0h] [ebp-4h]

  r = read(fd, ptr, n);
  if ( r < 0 )
  {
    ds_errno = errno;
    if ( errno != 11 && errno != 32 && errno != 104 )
      ds_error("!ds_read");
  }
  return r;
}

int __cdecl ds_write(int fd, void *ptr, int n)
{
  int r; // [esp+0h] [ebp-4h]

  r = write(fd, ptr, n);
  if ( r < 0 )
  {
    ds_errno = errno;
    if ( errno != 11 && errno != 32 && errno != 104 )
      ds_error("!ds_write");
  }
  return r;
}

char *__cdecl ds_basename(char *name)
{
  char *p; // [esp+0h] [ebp-4h]

  p = strrchr(name, 47);
  if ( p )
    return p + 1;
  else
    return name;
}

char *__cdecl ds_getenv(char *env)
{
  return getenv(env);
}

void __cdecl ds_bzero(void *ptr, int len)
{
#ifdef _WIN32
  memset(ptr, 0, len);
#else
  bzero(ptr, len);
#endif
}

int __cdecl ds_gettime(int64_t *psec, int64_t *pusec)
{
  struct timeval tval; // [esp+0h] [ebp-8h] BYREF

  if ( gettimeofday(&tval, 0) < 0 )
    return ds_error("!gettimeofday");

  *psec = tval.tv_sec;
  *pusec = tval.tv_usec;

  return 0;
}

char *__cdecl ds_strchr(char *str, int ch)
{
  return strchr(str, ch);
}

char *__cdecl ds_strcat(char *dst, char *src)
{
  return strcat(dst, src);
}

int __cdecl ds_strncmp(char *s1, char *s2, int n)
{
  return strncmp(s1, s2, n);
}

char *__cdecl ds_tilde_expand(char *buf, char *str)
{
#ifndef _WIN32
  struct passwd *pw; // [esp+4h] [ebp-410h]
#endif
  char *home; // [esp+8h] [ebp-40Ch]
  char *dp; // [esp+Ch] [ebp-408h]
  char *sp; // [esp+10h] [ebp-404h]
  char user[1024]; // [esp+14h] [ebp-400h] BYREF

  if ( *str != 126 )
    return strcpy(buf, str);
  sp = str + 1;
  dp = user;
  while ( *sp && *sp != 47 )
    *dp++ = *sp++;
  *dp = 0;
  if ( *sp == 47 )
    ++sp;
#ifndef _WIN32
  if ( user[0] )
  {
    pw = getpwnam(user);
    if ( !pw )
      return strcpy(buf, str);
    strcpy(buf, pw->pw_dir);
    strcat(buf, "/");
    strcat(buf, sp);
    return buf;
  }
  else
#endif
  {
    home = getenv("HOME");
    if ( !home )
      return strcpy(buf, str);
    strcpy(buf, home);
    strcat(buf, "/");
    strcat(buf, sp);
    return buf;
  }
}

int __cdecl ds_gethostname(char *hostname, int n)
{
  return gethostname(hostname, n);
}

char *__cdecl ds_getcwd(char *buf, int size)
{
  return getcwd(buf, size);
}

char *__cdecl ds_abs_path(char *buf, int len, char *fname)
{
  int n; // [esp+0h] [ebp-4h]

  if ( *fname == 47 )
    return strcpy(buf, fname);
  if ( *fname == 126 )
    return ds_tilde_expand(buf, fname);
  if ( !getcwd(buf, len) )
    return 0;
  n = strlen(buf);
  if ( n > 0 && buf[n - 1] != 47 )
    strcat(buf, "/");
  return strcat(buf, fname);
}

static int __cdecl read_kbd(int kbd, int escape)
{
  CQ *v3; // edx
  char ch; // [esp+7h] [ebp-5h] BYREF
  CQ *q; // [esp+8h] [ebp-4h]

  q = &kbdq;
  if ( kbdq.len > 255 )
    return 0;
  if ( read(kbd, &ch, 1u) != 1 )
    return ds_error("!read kbd");
  if ( escape == ch )
    return 1;
  v3 = q;
  q->buf[LOBYTE(q->put)] = ch;
  ++v3->put;
  ++q->len;
  return 0;
}

static int __cdecl read_dev(int dev)
{
  CQ *v2; // edx
  char ch; // [esp+7h] [ebp-5h] BYREF
  CQ *q; // [esp+8h] [ebp-4h]

  q = &devq;
  if ( devq.len > 255 )
    return 0;
  if ( read(dev, &ch, 1u) != 1 )
    return ds_error("!read dev");
  v2 = q;
  q->buf[LOBYTE(q->put)] = ch;
  ++v2->put;
  ++q->len;
  return 0;
}

static int __cdecl write_dev(int dev)
{
  char ch; // [esp+3h] [ebp-5h] BYREF
  CQ *q; // [esp+4h] [ebp-4h]

  q = &kbdq;
  if ( kbdq.len <= 0 )
    return 0;
  ch = q->buf[LOBYTE(q->get)];
  ++q->get;
  --q->len;
  if ( write(dev, &ch, 1u) == 1 )
    return 0;
  else
    return ds_error("!write dev");
}

static int __cdecl write_dsp(int dsp)
{
  char ch; // [esp+3h] [ebp-5h] BYREF
  CQ *q; // [esp+4h] [ebp-4h]

  q = &devq;
  if ( devq.len <= 0 )
    return 0;
  ch = q->buf[LOBYTE(q->get)];
  ++q->get;
  --q->len;
  if ( write(dsp, &ch, 1u) == 1 )
    return 0;
  else
    return ds_error("!write dsp");
}

DS_DESC *__cdecl ds_open_comport(char *name, DS_RECV_FUNC *recv_func)
{
  int fd; // [esp+0h] [ebp-4h]

  fd = open(name, 2050);
  if ( fd >= 0 )
    return ds_add_select_list(32, fd, name, 0, recv_func);
  ds_error("!open(%s)", name);
  return 0;
}

DS_DESC *__cdecl ds_open_netdev(char *name, DS_RECV_FUNC *recv_func)
{
  int fd; // [esp+0h] [ebp-4h]

  fd = open(name, 2050);
  if ( fd >= 0 )
    return ds_add_select_list(64, fd, name, 0, recv_func);
  ds_error("!open(%s)", name);
  return 0;
}

int __cdecl ds_comp_main(char *device, int escape)
{
  fd_set wfds; // [esp+10h] [ebp-10Ch] BYREF
  fd_set rfds; // [esp+90h] [ebp-8Ch] BYREF
  int dsp; // [esp+110h] [ebp-Ch]
  int kbd; // [esp+114h] [ebp-8h]
  int dev; // [esp+118h] [ebp-4h]

  dev = open(device, 2050);
  if ( dev < 0 )
    return ds_error("!open(%s)", device);
  kbd = ds_raw_kbd();
  if ( kbd < 0 )
    return -1;
  dsp = 1;
  do
  {
    FD_ZERO(&rfds);
    if ( kbdq.len <= 255 )
      FD_SET(kbd, &rfds);
    if ( devq.len <= 255 )
      FD_SET(dev, &rfds);
    FD_ZERO(&wfds);
    if ( kbdq.len > 0 )
      FD_SET(dev, &wfds);
    if ( devq.len > 0 )
      FD_SET(dsp, &wfds);
    if ( select(256, &rfds, &wfds, 0, 0) < 0 )
      return ds_error("!select");
  }
  while ( (!FD_ISSET(kbd, &rfds)
        || !read_kbd(kbd, escape))
       && (!FD_ISSET(dev, &rfds) || !read_dev(dev))
       && (!FD_ISSET(dev, &wfds) || !write_dev(dev))
       && (!FD_ISSET(dsp, &wfds) || !write_dsp(dsp)) );
  ds_printf("\n");
  close(dev);
  return 0;
}

