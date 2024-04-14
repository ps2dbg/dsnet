#include "dsnet_prototypes.h"

static struct handle handle_map[256];

static int once_28 = 0;

int ds_drfp_opened_files = 0;
void (*ds_drfp_err_func)(DSP_BUF *db);

static void __cdecl drfp_errmsg(int proto, char *tag, char *fname, char *err);
static void __cdecl init_handle_map();
static int __cdecl alloc_handle();
static void __cdecl free_handle(int handle);
static int __cdecl get_ino(char *name);
static int __cdecl is_found(char *name);
static int __cdecl is_dir(char *name);
static int __cdecl is_file(char *name);
static int __cdecl is_opened_ino(int ino);
static int __cdecl handle_to_fd(int handle);
static DIR *__cdecl handle_to_dir(int handle);
static int __cdecl drfp_recv_reset();
static DSP_BUF *__cdecl send_drfp(
        DS_DESC *desc,
        DSP_BUF *rdb,
        DECI2_HDR *rdh,
        unsigned int code,
        unsigned int seq,
        unsigned int result,
        void *data,
        unsigned int addlen);
static unsigned int __cdecl drfp_flag(unsigned int flag, unsigned int *pmode);
static unsigned int __cdecl drfp_err(unsigned int err);
static unsigned int __cdecl drfp_base(unsigned int base);
static void __cdecl ds_option_expand(char *path, char *name);
static DSP_BUF *__cdecl recv_drfp_open(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen);
static DSP_BUF *__cdecl recv_drfp_dopen(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen);
static void __cdecl set_scestat_time(unsigned __int8 *st_time, int *time);
static void __cdecl set_stat_time(int *time, unsigned __int8 *st_time);
static unsigned int __cdecl sce_mode2st_mode(unsigned int sce_mode);
static unsigned int __cdecl st_mode2sce_mode(unsigned int st_mode);
static void __cdecl sce_stat2stat(struct stat *to, struct sce_stat *from);
static void __cdecl stat2sce_stat(struct sce_stat *to, struct stat *from);
static int __cdecl getsce_stat(char *name, struct sce_stat *data);
static DSP_BUF *__cdecl recv_drfp_getstat(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen);
static DSP_BUF *__cdecl recv_drfp_remove(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen);
static DSP_BUF *__cdecl recv_drfp_rename(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen);
static DSP_BUF *__cdecl recv_drfp_chdir(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen);
static DSP_BUF *__cdecl recv_drfp_rmdir(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen);
static DSP_BUF *__cdecl recv_drfp_mkdir(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen);
static DSP_BUF *__cdecl recv_drfp_dclose(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen);
static DSP_BUF *__cdecl recv_drfp_close(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen);
static DSP_BUF *__cdecl recv_drfp_read(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen);
static DSP_BUF *__cdecl recv_drfp_write(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen);
static DSP_BUF *__cdecl recv_drfp_seek(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen);
static DSP_BUF *__cdecl recv_drfp_seek64(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen);
static DSP_BUF *__cdecl recv_drfp_dread(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen);
static DSP_BUF *__cdecl recv_drfp_symlink(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen);
static DSP_BUF *__cdecl recv_drfp_readlink(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen);
static DSP_BUF *__cdecl recv_drfp_chstat(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen);

static void __cdecl drfp_errmsg(int proto, char *tag, char *fname, char *err)
{
  char *v4; // eax
  size_t v5; // ebx
  size_t v6; // ebx
  size_t v7; // eax
  size_t v8; // eax
  size_t v9; // eax
  size_t v10; // eax
  DSP_BUF *db; // [esp+4h] [ebp-418h]
  int len; // [esp+8h] [ebp-414h]
  char *p; // [esp+Ch] [ebp-410h]
  char *p_1; // [esp+Ch] [ebp-410h]
  char *msg; // [esp+10h] [ebp-40Ch]
  char *prog; // [esp+14h] [ebp-408h]
  char path[1025]; // [esp+18h] [ebp-404h] BYREF

  if ( ds_drfp_err_func )
  {
    if ( ds_program_name && *ds_program_name )
      v4 = ds_program_name;
    else
      v4 = "unknown-prog";
    prog = v4;
    if ( !ds_abs_path(path, 1025, fname) )
      strcpy(path, fname);
    v5 = strlen(prog);
    v6 = strlen(path) + 7 + v5;
    v7 = strlen(err);
    len = v7 + 3 + v6 + 1;
    msg = (char *)ds_alloc(v7 + 3 + v6 + 15);
    if ( msg )
    {
      ds_bzero(msg, 4u);
      msg[4] = 10;
      msg[5] = 42;
      msg[6] = 42;
      msg[7] = 42;
      msg[8] = 32;
      v8 = strlen(prog);
      memcpy(msg + 9, prog, v8);
      p = &msg[strlen(prog) + 9];
      *p++ = 58;
      *p++ = 32;
      v9 = strlen(path);
      memcpy(p, path, v9);
      p_1 = &p[strlen(path)];
      *p_1++ = 32;
      *p_1++ = 45;
      *p_1++ = 32;
      v10 = strlen(err);
      memcpy(p_1, err, v10);
      p_1[strlen(err)] = 10;
      db = ds_alloc_buf(1043, 72, msg, len + 4);
      if ( db )
        ds_drfp_err_func(db);
      ds_free(msg);
    }
  }
}

static void __cdecl init_handle_map()
{
  int i; // [esp+0h] [ebp-4h]

  if ( !once_28 )
  {
    for ( i = 0; i <= 255; ++i )
    {
      handle_map[i].fd = -1;
      handle_map[i].dir = 0;
      handle_map[i].ino = 0;
      handle_map[i].name = 0;
    }
    ds_drfp_opened_files = 0;
    once_28 = 1;
  }
}

static int __cdecl alloc_handle()
{
  int i; // [esp+0h] [ebp-4h]

  init_handle_map();
  for ( i = 0; i <= 255; ++i )
  {
    if ( handle_map[i].fd < 0 )
    {
      ++ds_drfp_opened_files;
      return i;
    }
  }
  return -1;
}

static void __cdecl free_handle(int handle)
{
  handle_map[handle].fd = -1;
  handle_map[handle].dir = 0;
  handle_map[handle].ino = 0;
  if ( handle_map[handle].name )
    ds_free(handle_map[handle].name);
  handle_map[handle].name = 0;
  --ds_drfp_opened_files;
}

static int __cdecl get_ino(char *name)
{
#ifndef _WIN32
  struct stat stat; // [esp+0h] [ebp-58h] BYREF

  lstat(name, &stat);
  return stat.st_ino;
#else
  return 0;
#endif
}

static int __cdecl is_found(char *name)
{
#ifndef _WIN32
  struct stat stat; // [esp+0h] [ebp-58h] BYREF

  return lstat(name, &stat) >= 0;
#else
  return 0;
#endif
}

static int __cdecl is_dir(char *name)
{
#ifndef _WIN32
  struct stat stat; // [esp+0h] [ebp-58h] BYREF

  return lstat(name, &stat) >= 0 && (stat.st_mode & 0xF000) == 0x4000;
#else
  return 0;
#endif
}

static int __cdecl is_file(char *name)
{
#ifndef _WIN32
  struct stat stat; // [esp+0h] [ebp-58h] BYREF

  return lstat(name, &stat) >= 0 && (stat.st_mode & 0xF000) == 0x8000;
#else
  return 0;
#endif
}

static int __cdecl is_opened_ino(int ino)
{
  int i; // [esp+0h] [ebp-4h]

  for ( i = 0; i <= 255; ++i )
  {
    if ( handle_map[i].fd >= 0 && handle_map[i].ino == ino )
      return 1;
  }
  return 0;
}

static int __cdecl handle_to_fd(int handle)
{
  init_handle_map();
  if ( handle >= 0 && handle <= 255 )
    return handle_map[handle].fd;
  else
    return -1;
}

static DIR *__cdecl handle_to_dir(int handle)
{
  init_handle_map();
  if ( handle >= 0 && handle <= 255 && handle_map[handle].dir )
    return handle_map[handle].dir;
  else
    return 0;
}

static int __cdecl drfp_recv_reset()
{
  int handle; // [esp+0h] [ebp-8h]
  int r; // [esp+4h] [ebp-4h]

  r = 0;
  init_handle_map();
  for ( handle = 0; handle <= 255; ++handle )
  {
    if ( handle_map[handle].fd >= 0 )
    {
      if ( handle_map[handle].dir )
        closedir(handle_map[handle].dir);
      else
        close(handle_map[handle].fd);
      free_handle(handle);
      ++r;
    }
  }
  ds_drfp_opened_files = 0;
  return r;
}

static DSP_BUF *__cdecl send_drfp(
        DS_DESC *desc,
        DSP_BUF *rdb,
        DECI2_HDR *rdh,
        unsigned int code,
        unsigned int seq,
        unsigned int result,
        void *data,
        unsigned int addlen)
{
  DSP_BUF *db; // [esp+0h] [ebp-Ch]

  db = ds_alloc_buf(rdh->protocol, rdh->source, 0, addlen + 12);
  if ( !db )
    return rdb;
  *(_DWORD *)&db->buf[8] = code;
  *(_DWORD *)&db->buf[12] = seq;
  *(_DWORD *)&db->buf[16] = result;
  if ( addlen )
    memcpy(&db->buf[20], data, addlen);
  ds_send_desc(desc, db);
  return ds_free_buf(rdb);
}

static unsigned int __cdecl drfp_flag(unsigned int flag, unsigned int *pmode)
{
  unsigned int v2; // eax
  unsigned int r; // [esp+0h] [ebp-4h]

  r = 0;
  v2 = flag & 3;
  if ( v2 == 2 )
  {
    r = 1;
  }
  else if ( v2 > 2 )
  {
    if ( v2 == 3 )
      r = 2;
  }
  else if ( v2 == 1 )
  {
    r = 0;
  }
  if ( (flag & 0x200) != 0 )
    LOBYTE(r) = r | 0x40;
  if ( (flag & 0x400) != 0 )
    r |= 0x200u;
  if ( (flag & 0x800) != 0 )
    LOBYTE(r) = r | 0x80;
  if ( (flag & 0x100) != 0 )
    r |= 0x400u;
  *pmode = HIWORD(flag);
  if ( !HIWORD(flag) )
    *pmode = 438;
  return r;
}

static unsigned int __cdecl drfp_err(unsigned int err)
{
  unsigned int r; // [esp+0h] [ebp-4h]

  switch ( err )
  {
    case 2u:
      r = 2;
      break;
    case 5u:
      r = 5;
      break;
    case 9u:
      r = 9;
      break;
    case 0xDu:
      r = 13;
      break;
    case 0x10u:
      r = 16;
      break;
    case 0x11u:
      r = 17;
      break;
    case 0x12u:
      r = 18;
      break;
    case 0x13u:
      r = 19;
      break;
    case 0x14u:
      r = 20;
      break;
    case 0x15u:
      r = 21;
      break;
    case 0x18u:
      r = 24;
      break;
    case 0x1Bu:
      r = 27;
      break;
    case 0x1Cu:
      r = 28;
      break;
    case 0x1Eu:
      r = 30;
      break;
    case 0x24u:
      r = 91;
      break;
    case 0x26u:
      r = 48;
      break;
    case 0x27u:
      r = 90;
      break;
    case 0x28u:
      r = 92;
      break;
    case 0x7Au:
      r = 132;
      break;
    default:
      r = 0xFFFF;
      break;
  }
  return r;
}

static unsigned int __cdecl drfp_base(unsigned int base)
{
  switch ( base )
  {
    case 1u:
      return 1;
    case 0u:
      return 0;
    case 2u:
      return 2;
  }
  return 0;
}

static void __cdecl ds_option_expand(char *path, char *name)
{
  size_t v2; // [esp-4h] [ebp-410h]
  size_t v3; // [esp-4h] [ebp-410h]
  int n; // [esp+0h] [ebp-40Ch]
  char *p; // [esp+4h] [ebp-408h]
  char buf[1025]; // [esp+8h] [ebp-404h] BYREF

  p = 0;
  n = 0;
  memset(buf, 0, sizeof(buf));
  v2 = strlen("${IOPMOUDULES}");
  if ( !strncmp("${IOPCONF}", name, v2) )
  {
    n = strlen("${IOPCONF}");
    p = ds_ref_option_str("IOPCONF");
  }
  else
  {
    v3 = strlen("${IOPMOUDULES}");
    if ( !strncmp("${IOPMOUDULES}", name, v3) )
    {
      n = strlen("${IOPMOUDULES}");
      p = ds_ref_option_str("IOPMODULES");
    }
  }
  if ( p )
    strcpy(buf, p);
  strcat(buf, &name[n]);
  ds_tilde_expand(path, buf);
}

static DSP_BUF *__cdecl recv_drfp_open(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen)
{
  size_t v9; // eax
  char *v10; // eax
  char *v11; // [esp-4h] [ebp-484h]
  struct stat stbuf; // [esp+4h] [ebp-47Ch] BYREF
  char path[1025]; // [esp+5Ch] [ebp-424h] BYREF
  int handle; // [esp+460h] [ebp-20h]
  int r; // [esp+464h] [ebp-1Ch]
  char *name; // [esp+468h] [ebp-18h]
  unsigned int fd; // [esp+46Ch] [ebp-14h] BYREF
  unsigned int v18; // [esp+470h] [ebp-10h]
  unsigned int mode; // [esp+474h] [ebp-Ch] BYREF
  unsigned int flag; // [esp+478h] [ebp-8h]
  unsigned int seq; // [esp+47Ch] [ebp-4h]
  unsigned int *argsa; // [esp+494h] [ebp+14h]

  handle = -1;
  if ( (unsigned int)arglen <= 8 )
    return db;
  seq = *args;
  argsa = args + 1;
  flag = *argsa;
  mode = HIWORD(flag);
  flag = (unsigned __int16)flag;
  name = (char *)(argsa + 1);
  flag = drfp_flag((unsigned __int16)flag, &mode);
  ds_option_expand(path, (char *)argsa + 4);
  r = open(path, flag, mode);
  if ( r >= 0 )
  {
    v18 = 0;
    if ( fstat(r, &stbuf) >= 0 )
    {
      if ( (stbuf.st_mode & 0xF000) == 0x4000 )
      {
        v18 = drfp_err(0x15u);
      }
      else
      {
        handle = alloc_handle();
        if ( handle >= 0 )
        {
          v9 = strlen(path);
          v10 = (char *)ds_alloc(v9 + 1);
          handle_map[handle].name = v10;
          if ( v10 )
          {
            handle_map[handle].fd = r;
            handle_map[handle].dir = 0;
            handle_map[handle].ino = stbuf.st_ino;
            strcpy(handle_map[handle].name, path);
          }
          else
          {
            v18 = drfp_err(0xCu);
            free_handle(handle);
          }
        }
        else
        {
          v18 = drfp_err(0x18u);
        }
      }
    }
    else
    {
      v18 = drfp_err(errno);
    }
    if ( v18 )
      close(r);
  }
  else
  {
    v11 = strerror(errno);
    drfp_errmsg(dh->protocol, "open", path, v11);
    v18 = drfp_err(errno);
  }
  fd = handle;
  return send_drfp(desc, db, dh, 1u, seq, v18, &fd, 4u);
}

static DSP_BUF *__cdecl recv_drfp_dopen(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen)
{
  size_t v8; // eax
  char *v9; // eax
  int v10; // eax
  int ino; // eax
  char *v12; // [esp-4h] [ebp-420h]
  char path[1025]; // [esp+0h] [ebp-41Ch] BYREF
  DIR *dirstream; // [esp+404h] [ebp-18h]
  int handle; // [esp+408h] [ebp-14h]
  char *s2; // [esp+40Ch] [ebp-10h]
  unsigned int fd; // [esp+410h] [ebp-Ch] BYREF
  unsigned int result; // [esp+414h] [ebp-8h]
  unsigned int seq; // [esp+418h] [ebp-4h]

  handle = -1;
  if ( (unsigned int)arglen <= 4 )
    return db;
#ifndef _WIN32
  seq = *args;
  s2 = (char *)(args + 1);
  ds_option_expand(path, (char *)args + 4);
  if ( !strlen(path) )
    strcpy(path, ".");
  dirstream = opendir(path);
  if ( dirstream )
  {
    result = 0;
    handle = alloc_handle();
    if ( handle >= 0 )
    {
      v8 = strlen(path);
      v9 = (char *)ds_alloc(v8 + 1);
      handle_map[handle].name = v9;
      if ( v9 )
      {
        v10 = dirfd(dirstream);
        handle_map[handle].fd = v10;
        handle_map[handle].dir = dirstream;
        ino = get_ino(path);
        handle_map[handle].ino = ino;
        strcpy(handle_map[handle].name, path);
      }
      else
      {
        result = drfp_err(0xCu);
        free_handle(handle);
      }
    }
    else
    {
      result = drfp_err(0x18u);
    }
    if ( result )
      closedir(dirstream);
  }
  else
  {
    v12 = strerror(errno);
    drfp_errmsg(dh->protocol, "dopen", path, v12);
    result = drfp_err(errno);
  }
#else
  result = drfp_err(0x18u);
#endif
  fd = handle;
  return send_drfp(desc, db, dh, 0x13u, seq, result, &fd, 4u);
}

static void __cdecl set_scestat_time(unsigned __int8 *st_time, int *time)
{
  struct tm *v2; // eax

  v2 = localtime(time);
  v2->tm_year += 1900;
  ++v2->tm_mon;
  *st_time = 0;
  st_time[1] = v2->tm_sec;
  st_time[2] = v2->tm_min;
  st_time[3] = v2->tm_hour;
  st_time[4] = v2->tm_mday;
  st_time[5] = v2->tm_mon;
  *((_WORD *)st_time + 3) = __PAIR16__(BYTE1(v2->tm_year), v2->tm_year);
}

static void __cdecl set_stat_time(int *time, unsigned __int8 *st_time)
{
  struct tm tm; // [esp+4h] [ebp-2Ch] BYREF

  tm.tm_sec = st_time[1];
  tm.tm_min = st_time[2];
  tm.tm_hour = st_time[3];
  tm.tm_mday = st_time[4];
  tm.tm_mon = st_time[5];
  tm.tm_year = st_time[6];
  tm.tm_year = st_time[7] + (tm.tm_year << 8) - 1900;
  *time = mktime(&tm);
}

static unsigned int __cdecl sce_mode2st_mode(unsigned int sce_mode)
{
  unsigned int st_mode; // [esp+0h] [ebp-4h]

  st_mode = 0;
  if ( (sce_mode & 0x4000) != 0 )
    st_mode = 40960;
  if ( (sce_mode & 0x2000) != 0 )
    st_mode |= 0x8000u;
  if ( (sce_mode & 0x1000) != 0 )
    st_mode |= 0x4000u;
  if ( (sce_mode & 1) != 0 )
    LOBYTE(st_mode) = st_mode | 1;
  if ( (sce_mode & 2) != 0 )
    LOBYTE(st_mode) = st_mode | 2;
  if ( (sce_mode & 4) != 0 )
    LOBYTE(st_mode) = st_mode | 4;
  if ( (sce_mode & 8) != 0 )
    LOBYTE(st_mode) = st_mode | 8;
  if ( (sce_mode & 0x10) != 0 )
    LOBYTE(st_mode) = st_mode | 0x10;
  if ( (sce_mode & 0x20) != 0 )
    LOBYTE(st_mode) = st_mode | 0x20;
  if ( (sce_mode & 0x40) != 0 )
    LOBYTE(st_mode) = st_mode | 0x40;
  if ( (sce_mode & 0x80u) != 0 )
    LOBYTE(st_mode) = st_mode | 0x80;
  if ( (sce_mode & 0x100) != 0 )
    st_mode |= 0x100u;
  return st_mode;
}

static unsigned int __cdecl st_mode2sce_mode(unsigned int st_mode)
{
  unsigned int sce_mode; // [esp+0h] [ebp-4h]

  sce_mode = 0;
  if ( (st_mode & 0xF000) == 40960 )
    sce_mode = 0x4000;
  if ( (st_mode & 0xF000) == 0x8000 )
    sce_mode |= 0x2000u;
  if ( (st_mode & 0xF000) == 0x4000 )
    sce_mode |= 0x1000u;
  if ( (st_mode & 1) != 0 )
    LOBYTE(sce_mode) = sce_mode | 1;
  if ( (st_mode & 2) != 0 )
    LOBYTE(sce_mode) = sce_mode | 2;
  if ( (st_mode & 4) != 0 )
    LOBYTE(sce_mode) = sce_mode | 4;
  if ( (st_mode & 8) != 0 )
    LOBYTE(sce_mode) = sce_mode | 8;
  if ( (st_mode & 0x10) != 0 )
    LOBYTE(sce_mode) = sce_mode | 0x10;
  if ( (st_mode & 0x20) != 0 )
    LOBYTE(sce_mode) = sce_mode | 0x20;
  if ( (st_mode & 0x40) != 0 )
    LOBYTE(sce_mode) = sce_mode | 0x40;
  if ( (st_mode & 0x80u) != 0 )
    LOBYTE(sce_mode) = sce_mode | 0x80;
  if ( (st_mode & 0x100) != 0 )
    sce_mode |= 0x100u;
  return sce_mode;
}

static void __cdecl sce_stat2stat(struct stat *to, struct sce_stat *from)
{
  to->st_mode = sce_mode2st_mode(from->mode);
  set_stat_time(&to->st_atime, from->atime);
  set_stat_time(&to->st_mtime, from->mtime);
  set_stat_time(&to->st_ctime, from->ctime);
  to->st_size = from->size;
}

static void __cdecl stat2sce_stat(struct sce_stat *to, struct stat *from)
{
  to->mode = st_mode2sce_mode(from->st_mode);
  set_scestat_time(to->atime, &from->st_atime);
  set_scestat_time(to->mtime, &from->st_mtime);
  set_scestat_time(to->ctime, &from->st_ctime);
  to->mode = to->mode;
  to->attr = 0;
  to->size = from->st_size;
  to->hisize = 0;
  memset(to->private_, 0, sizeof(to->private_));
}

static int __cdecl getsce_stat(char *name, struct sce_stat *data)
{
  unsigned int v3; // [esp+4h] [ebp-5Ch]
  struct stat stbuf; // [esp+8h] [ebp-58h] BYREF

  v3 = 0;
  if ( stat(name, &stbuf) < 0 )
    return -1;
  stat2sce_stat(data, &stbuf);
  return v3;
}

static DSP_BUF *__cdecl recv_drfp_getstat(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen)
{
  char *v8; // [esp-4h] [ebp-45Ch]
  struct sce_stat data; // [esp+0h] [ebp-458h] BYREF
  char path[1025]; // [esp+40h] [ebp-418h] BYREF
  int r; // [esp+444h] [ebp-14h]
  char *s2; // [esp+448h] [ebp-10h]
  unsigned int len; // [esp+44Ch] [ebp-Ch]
  unsigned int result; // [esp+450h] [ebp-8h]
  unsigned int seq; // [esp+454h] [ebp-4h]

  len = 64;
  if ( (unsigned int)arglen <= 4 )
    return db;
  seq = *args;
  s2 = (char *)(args + 1);
  ds_option_expand(path, (char *)args + 4);
  r = getsce_stat(path, &data);
  if ( r >= 0 )
  {
    result = 0;
    len = 64;
  }
  else
  {
    v8 = strerror(errno);
    drfp_errmsg(dh->protocol, "getstat", path, v8);
    result = drfp_err(errno);
  }
  send_drfp(desc, db, dh, 0x19u, seq, result, &data, len);
  return 0;
}

static DSP_BUF *__cdecl recv_drfp_remove(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen)
{
  int ino; // eax
  char path[1025]; // [esp+0h] [ebp-414h] BYREF
  int v9; // [esp+404h] [ebp-10h]
  char *name; // [esp+408h] [ebp-Ch]
  unsigned int result; // [esp+40Ch] [ebp-8h]
  unsigned int seq; // [esp+410h] [ebp-4h]

  if ( (unsigned int)arglen <= 4 )
    return db;
  seq = *args;
  name = (char *)(args + 1);
  ds_option_expand(path, (char *)args + 4);
  if ( is_found(path) )
  {
    ino = get_ino(path);
    if ( is_opened_ino(ino) )
    {
      result = drfp_err(0x10u);
      return send_drfp(desc, db, dh, 0xDu, seq, result, 0, 0);
    }
    v9 = remove(path);
    if ( v9 >= 0 )
    {
      result = 0;
      return send_drfp(desc, db, dh, 0xDu, seq, result, 0, 0);
    }
  }
  result = drfp_err(errno);
  return send_drfp(desc, db, dh, 0xDu, seq, result, 0, 0);
}

static DSP_BUF *__cdecl recv_drfp_rename(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen)
{
  int ino; // eax
  char newpath[1025]; // [esp+0h] [ebp-81Ch] BYREF
  char oldpath[1025]; // [esp+404h] [ebp-418h] BYREF
  int r; // [esp+808h] [ebp-14h]
  char *name; // [esp+80Ch] [ebp-10h]
  char *oldname; // [esp+810h] [ebp-Ch]
  unsigned int result; // [esp+814h] [ebp-8h]
  unsigned int seq; // [esp+818h] [ebp-4h]

  if ( (unsigned int)arglen <= 5 )
    return db;
  seq = *args;
  oldname = (char *)(args + 1);
  name = (char *)args + strlen((const char *)args + 4) + 5;
  ds_option_expand(oldpath, (char *)args + 4);
  ds_option_expand(newpath, name);
  if ( is_found(oldpath) )
  {
    ino = get_ino(oldpath);
    if ( is_opened_ino(ino) )
    {
      result = drfp_err(0x10u);
      return send_drfp(desc, db, dh, 0x1Fu, seq, result, 0, 0);
    }
    if ( is_found(newpath) )
    {
      if ( is_file(oldpath) && is_dir(newpath) )
      {
        result = drfp_err(0x15u);
      }
      else if ( is_dir(oldpath) )
      {
        if ( is_file(newpath) )
          result = drfp_err(0x14u);
      }
      return send_drfp(desc, db, dh, 0x1Fu, seq, result, 0, 0);
    }
    r = rename(oldpath, newpath);
    if ( r >= 0 )
    {
      result = r;
      return send_drfp(desc, db, dh, 0x1Fu, seq, result, 0, 0);
    }
  }
  result = drfp_err(errno);
  return send_drfp(desc, db, dh, 0x1Fu, seq, result, 0, 0);
}

static DSP_BUF *__cdecl recv_drfp_chdir(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen)
{
  unsigned int v6; // [esp-Ch] [ebp-18h]
  unsigned int seq; // [esp+8h] [ebp-4h]

  if ( (unsigned int)arglen <= 4 )
    return db;
  seq = *args;
  v6 = drfp_err(0x26u);
  return send_drfp(desc, db, dh, 0x21u, seq, v6, 0, 0);
}

static DSP_BUF *__cdecl recv_drfp_rmdir(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen)
{
  int ino; // eax
  char *v9; // [esp-4h] [ebp-418h]
  char path[1025]; // [esp+0h] [ebp-414h] BYREF
  int v11; // [esp+404h] [ebp-10h]
  char *name; // [esp+408h] [ebp-Ch]
  unsigned int result; // [esp+40Ch] [ebp-8h]
  unsigned int seq; // [esp+410h] [ebp-4h]

  if ( (unsigned int)arglen <= 4 )
    return db;
  seq = *args;
  name = (char *)(args + 1);
  ds_option_expand(path, (char *)args + 4);
  if ( !is_found(path) )
    goto LABEL_8;
  ino = get_ino(path);
  if ( !is_opened_ino(ino) )
  {
    v11 = rmdir(path);
    if ( v11 < 0 )
    {
      v9 = strerror(errno);
      drfp_errmsg(dh->protocol, "rmdir", path, v9);
    }
LABEL_8:
    result = drfp_err(errno);
    return send_drfp(desc, db, dh, 0x21u, seq, result, 0, 0);
  }
  result = drfp_err(0x10u);
  return send_drfp(desc, db, dh, 0x21u, seq, result, 0, 0);
}

static DSP_BUF *__cdecl recv_drfp_mkdir(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen)
{
  char *v8; // [esp-4h] [ebp-41Ch]
  char path[1025]; // [esp+0h] [ebp-418h] BYREF
  int r; // [esp+404h] [ebp-14h]
  char *s2; // [esp+408h] [ebp-10h]
  unsigned int result; // [esp+40Ch] [ebp-Ch]
  unsigned int mode; // [esp+410h] [ebp-8h]
  unsigned int seq; // [esp+414h] [ebp-4h]
  unsigned int *argsa; // [esp+42Ch] [ebp+14h]

  result = 0;
  if ( (unsigned int)arglen <= 8 )
    return db;
  seq = *args;
  argsa = args + 1;
  mode = *argsa;
  s2 = (char *)(argsa + 1);
  mode = sce_mode2st_mode(HIWORD(mode));
  ds_option_expand(path, (char *)argsa + 4);
#ifndef _WIN32
  r = mkdir(path, mode);
#else
  r = mkdir(path);
#endif
  if ( r < 0 )
  {
    v8 = strerror(errno);
    drfp_errmsg(dh->protocol, "mkdir", path, v8);
    result = drfp_err(errno);
  }
  return send_drfp(desc, db, dh, 0xFu, seq, result, 0, 0);
}

static DSP_BUF *__cdecl recv_drfp_dclose(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen)
{
  DIR *dirent; // [esp+0h] [ebp-14h]
  int handle; // [esp+8h] [ebp-Ch]
  unsigned int v9; // [esp+Ch] [ebp-8h]
  unsigned int seq; // [esp+10h] [ebp-4h]

  if ( arglen != 8 )
    return db;
  seq = *args;
  handle = args[1];
  dirent = handle_to_dir(handle);
  if ( dirent )
  {
    v9 = 0;
    if ( closedir(dirent) >= 0 )
    {
      free_handle(handle);
    }
    else
    {
      v9 = drfp_err(errno);
    }
  }
  else
  {
    v9 = drfp_err(9u);
  }
  return send_drfp(desc, db, dh, 0x15u, seq, v9, 0, 0);
}

static DSP_BUF *__cdecl recv_drfp_close(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen)
{
  int fd; // [esp+0h] [ebp-10h]
  int handle; // [esp+4h] [ebp-Ch]
  unsigned int v9; // [esp+8h] [ebp-8h]
  unsigned int seq; // [esp+Ch] [ebp-4h]

  if ( arglen != 8 )
    return db;
  seq = *args;
  handle = args[1];
  fd = handle_to_fd(handle);
  if ( fd >= 0 )
  {
    v9 = 0;
    if ( close(fd) >= 0 )
    {
      free_handle(handle);
    }
    else
    {
      v9 = drfp_err(errno);
    }
  }
  else
  {
    v9 = drfp_err(9u);
  }
  return send_drfp(desc, db, dh, 3u, seq, v9, 0, 0);
}

static DSP_BUF *__cdecl recv_drfp_read(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen)
{
  int fd; // [esp+0h] [ebp-20h]
  int r; // [esp+8h] [ebp-18h]
  unsigned __int8 *data; // [esp+Ch] [ebp-14h]
  unsigned int v10; // [esp+14h] [ebp-Ch]
  unsigned int len; // [esp+18h] [ebp-8h]
  unsigned int seq; // [esp+1Ch] [ebp-4h]
  unsigned int *argsa; // [esp+34h] [ebp+14h]

  data = 0;
  if ( arglen != 12 )
    return db;
  seq = *args;
  argsa = args + 1;
  len = argsa[1];
  fd = handle_to_fd(*argsa);
  if ( fd >= 0 )
  {
    data = (unsigned __int8 *)ds_alloc(len + 4);
    if ( data )
    {
      r = read(fd, data + 4, len);
      if ( r >= 0 )
      {
        v10 = 0;
        len = r;
      }
      else
      {
        v10 = drfp_err(errno);
        len = 0;
      }
    }
    else
    {
      v10 = drfp_err(0xCu);
    }
  }
  else
  {
    v10 = drfp_err(9u);
  }
  *(_DWORD *)data = len;
  send_drfp(desc, db, dh, 5u, seq, v10, data, len + 4);
  if ( data )
    ds_free(data);
  return 0;
}

static DSP_BUF *__cdecl recv_drfp_write(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen)
{
  int fd; // [esp+0h] [ebp-1Ch]
  int handle; // [esp+4h] [ebp-18h]
  int r; // [esp+8h] [ebp-14h]
  unsigned int v10; // [esp+10h] [ebp-Ch]
  unsigned int len; // [esp+14h] [ebp-8h] BYREF
  unsigned int seq; // [esp+18h] [ebp-4h]
  unsigned int *argsb; // [esp+30h] [ebp+14h]
  unsigned int *argsa; // [esp+30h] [ebp+14h]

  if ( (unsigned int)arglen <= 0xB )
    return db;
  seq = *args;
  argsb = args + 1;
  handle = *argsb++;
  len = *argsb;
  argsa = argsb + 1;
  fd = handle_to_fd(handle);
  if ( fd >= 0 )
  {
    if ( len != arglen - 12 )
      return db;
    r = write(fd, argsa, len);
    if ( r >= 0 )
    {
      v10 = 0;
      len = r;
    }
    else
    {
      v10 = drfp_err(errno);
      len = 0;
    }
  }
  else
  {
    v10 = drfp_err(9u);
  }
  return send_drfp(desc, db, dh, 7u, seq, v10, &len, 4u);
}

static DSP_BUF *__cdecl recv_drfp_seek(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen)
{
  int v8; // [esp+0h] [ebp-24h]
  int newpos; // [esp+4h] [ebp-20h]
  int fd; // [esp+8h] [ebp-1Ch]
  int handle; // [esp+Ch] [ebp-18h]
  unsigned int pos; // [esp+10h] [ebp-14h] BYREF
  unsigned int result; // [esp+14h] [ebp-10h]
  unsigned int base; // [esp+18h] [ebp-Ch]
  unsigned int off; // [esp+1Ch] [ebp-8h]
  unsigned int seq; // [esp+20h] [ebp-4h]
  unsigned int *argsa; // [esp+38h] [ebp+14h]

  if ( arglen != 16 )
    return db;
  seq = *args;
  argsa = args + 1;
  handle = *argsa++;
  off = *argsa;
  base = drfp_base(argsa[1]);
  result = 0;
  fd = handle_to_fd(handle);
  if ( fd >= 0 )
  {
    v8 = lseek(fd, 0, 1);
    if ( v8 == -1 || (newpos = lseek(fd, off, base), newpos == -1) )
    {
      result = drfp_err(errno);
    }
    else if ( (unsigned int)newpos <= 0x7FFFFFFF )
    {
      pos = newpos;
    }
    else
    {
      result = drfp_err(0x1Bu);
      if ( lseek(fd, v8, 0) == -1 )
      {
        result = drfp_err(errno);
      }
      pos = v8;
    }
  }
  else
  {
    result = drfp_err(9u);
  }
  return send_drfp(desc, db, dh, 9u, seq, result, &pos, 4u);
}

static DSP_BUF *__cdecl recv_drfp_seek64(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen)
{
  int curpos; // [esp+0h] [ebp-34h]
  int newpos; // [esp+4h] [ebp-30h]
  unsigned int pos[2]; // [esp+8h] [ebp-2Ch] BYREF
  unsigned int result; // [esp+10h] [ebp-24h]
  int fd; // [esp+14h] [ebp-20h]
  int handle; // [esp+18h] [ebp-1Ch]
  unsigned int lopos; // [esp+1Ch] [ebp-18h]
  unsigned int hipos; // [esp+20h] [ebp-14h]
  unsigned int whence; // [esp+24h] [ebp-10h]
  unsigned int hioff; // [esp+28h] [ebp-Ch]
  unsigned int looff; // [esp+2Ch] [ebp-8h]
  unsigned int seq; // [esp+30h] [ebp-4h]
  unsigned int *argsa; // [esp+48h] [ebp+14h]

  result = 0;
  if ( arglen != 20 )
    return db;
  seq = *args;
  argsa = args + 1;
  handle = *argsa++;
  looff = *argsa++;
  hioff = *argsa;
  whence = drfp_base(argsa[1]);
  fd = handle_to_fd(handle);
  if ( fd >= 0 )
  {
    if ( hioff )
      result = drfp_err(0x1Bu);
    curpos = lseek(fd, 0, 1);
    if ( curpos == -1 )
    {
      result = drfp_err(errno);
    }
    else
    {
      newpos = lseek(fd, looff, whence);
      if ( newpos == -1 )
      {
        result = drfp_err(errno);
        lopos = curpos;
      }
      else
      {
        lopos = newpos;
        hipos = 0;
      }
    }
  }
  else
  {
    result = drfp_err(9u);
  }
  pos[0] = lopos;
  pos[1] = hipos;
  return send_drfp(desc, db, dh, 0x29u, seq, result, pos, 8u);
}

static DSP_BUF *__cdecl recv_drfp_dread(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen)
{
  size_t v6; // ebx
  size_t v7; // eax
  char *dest; // [esp+4h] [ebp-24h]
  DIR *dirstream; // [esp+8h] [ebp-20h]
  struct sce_dirent *sce_dent; // [esp+Ch] [ebp-1Ch]
  struct dirent *dent; // [esp+10h] [ebp-18h]
  unsigned int handle; // [esp+18h] [ebp-10h]
  unsigned int result; // [esp+1Ch] [ebp-Ch]
  unsigned int len; // [esp+20h] [ebp-8h]
  unsigned int seq; // [esp+24h] [ebp-4h]

  sce_dent = 0;
  if ( arglen != 8 )
    return db;
  seq = *args;
  handle = args[1];
  len = 0;
  while ( 1 )
  {
    dirstream = handle_to_dir(handle);
    if ( !dirstream )
    {
      result = drfp_err(9u);
      goto LABEL_15;
    }
    dent = readdir(dirstream);
    if ( !dent )
    {
      result = 0;
      goto LABEL_15;
    }
    len = strlen(dent->d_name) + 69; /* TODO: calculate size using sizeof */
    sce_dent = (struct sce_dirent *)ds_alloc(len);
    if ( !sce_dent )
      break;
    v6 = strlen(dent->d_name);
    v7 = strlen(handle_map[handle].name);
    dest = (char *)ds_alloc(v7 + v6 + 2);
    if ( !dest )
      break;
    strcpy(dest, handle_map[handle].name);
    strcat(dest, "/");
    strcat(dest, dent->d_name);
    if ( getsce_stat(dest, &sce_dent->d_stat) >= 0 )
    {
      strcpy((char *)sce_dent->d_name, dent->d_name);
      result = 0;
    }
    else
    {
      result = drfp_err(errno);
      len = 0;
      ds_free(sce_dent);
    }
    ds_free(dest);
    if ( !result )
      goto LABEL_15;
  }
  result = drfp_err(0xCu);
  len = 0;
LABEL_15:
  send_drfp(desc, db, dh, 0x17u, seq, result, sce_dent, len);
  if ( sce_dent )
    ds_free(sce_dent);
  return 0;
}

static DSP_BUF *__cdecl recv_drfp_symlink(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen)
{
  char *v8; // [esp-4h] [ebp-81Ch]
  unsigned int result; // [esp+0h] [ebp-818h]
  unsigned int seq; // [esp+4h] [ebp-814h]
  char newpath[1025]; // [esp+8h] [ebp-810h] BYREF
  char existpath[1025]; // [esp+40Ch] [ebp-40Ch] BYREF
  char *newname; // [esp+810h] [ebp-8h]
  char *existname; // [esp+814h] [ebp-4h]

  if ( (unsigned int)arglen <= 4 )
    return db;
  seq = *args;
  existname = (char *)(args + 1);
  newname = (char *)args + strlen((const char *)args + 4) + 5;
  ds_option_expand(existpath, (char *)args + 4);
  ds_option_expand(newpath, newname);
#ifndef _WIN32
  if ( symlink(existpath, newname) )
  {
    v8 = strerror(errno);
    drfp_errmsg(dh->protocol, "symlink", newpath, v8);
    result = drfp_err(errno);
  }
  else
#endif
  {
    result = 0;
  }
  return send_drfp(desc, db, dh, 0x2Du, seq, result, 0, 0);
}

static DSP_BUF *__cdecl recv_drfp_readlink(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen)
{
  unsigned int len; // [esp+0h] [ebp-81Ch]
  unsigned int len_1; // [esp+0h] [ebp-81Ch]
  unsigned int result; // [esp+4h] [ebp-818h]
  unsigned int seq; // [esp+8h] [ebp-814h]
  char newpath[1025]; // [esp+Ch] [ebp-810h] BYREF
  char existpath[1025]; // [esp+410h] [ebp-40Ch] BYREF
  char *newname; // [esp+814h] [ebp-8h]
  char *existname; // [esp+818h] [ebp-4h]

  if ( (unsigned int)arglen <= 4 )
    return db;
  seq = *args;
  existname = (char *)(args + 1);
  ds_option_expand(existpath, (char *)args + 4);
  memset(newpath, 0, sizeof(newpath));
#ifndef _WIN32
  len = readlink(existpath, newpath, 0x400u);
  if ( len == -1 )
  {
    result = drfp_err(errno);
    len_1 = 0;
    newname = 0;
  }
  else
  {
    result = 0;
    len_1 = len + 1;
    newname = newpath;
  }
#else
  result = drfp_err(0x18u);
  len_1 = 0;
  newname = 0;
#endif
  return send_drfp(desc, db, dh, 0x2Fu, seq, result, newname, len_1);
}

static DSP_BUF *__cdecl recv_drfp_chstat(DS_DESC *desc, DSP_BUF *db, DECI2_HDR *dh, unsigned int *args, int arglen)
{
  char *v8; // [esp-4h] [ebp-474h]
  struct stat data; // [esp+4h] [ebp-46Ch] BYREF
  char path[1025]; // [esp+5Ch] [ebp-414h] BYREF
  unsigned int v11; // [esp+460h] [ebp-10h]
  char *name; // [esp+464h] [ebp-Ch]
  unsigned int result; // [esp+468h] [ebp-8h]
  unsigned int seq; // [esp+46Ch] [ebp-4h]
  unsigned int *argsa; // [esp+484h] [ebp+14h]

  if ( (unsigned int)arglen <= 4 )
    return db;
  seq = *args;
  argsa = args + 1;
  v11 = *argsa;
  name = (char *)(argsa + 17);
  ds_option_expand(path, (char *)argsa + 68);
  sce_stat2stat(&data, (struct sce_stat *)(argsa + 1));
  if ( (v11 & 1) != 0 )
  {
    if ( chmod(path, data.st_mode) >= 0 )
    {
      result = 0;
    }
    else
    {
      v8 = strerror(errno);
      drfp_errmsg(dh->protocol, "lseek", path, v8);
      result = drfp_err(errno);
    }
  }
  if ( (v11 & 2) != 0 || (v11 & 4) != 0 || (v11 & 8) != 0 || (v11 & 0x10) != 0 || (v11 & 0x20) != 0 || (v11 & 0x40) != 0 )
    result = drfp_err(0x26u);
  return send_drfp(desc, db, dh, 0x1Bu, seq, result, 0, 0);
}

DSP_BUF *__cdecl ds_recv_drfp(DS_DESC *desc, DSP_BUF *db)
{
  unsigned int *args; // [esp+0h] [ebp-Ch]
  int len; // [esp+4h] [ebp-8h]
  DECI2_HDR *dh; // [esp+8h] [ebp-4h]

  if ( db )
  {
    dh = (DECI2_HDR *)db->buf;
    len = dh->length - 12;
    if ( len >= 0 )
    {
      args = (unsigned int *)&db->buf[12];
      switch ( *(_DWORD *)&db->buf[8] )
      {
        case 0:
          db = recv_drfp_open(desc, db, dh, args, len);
          break;
        case 2:
          db = recv_drfp_close(desc, db, dh, args, len);
          break;
        case 4:
          db = recv_drfp_read(desc, db, dh, args, len);
          break;
        case 6:
          db = recv_drfp_write(desc, db, dh, args, len);
          break;
        case 8:
          db = recv_drfp_seek(desc, db, dh, args, len);
          break;
        case 0xC:
          db = recv_drfp_remove(desc, db, dh, args, len);
          break;
        case 0xE:
          db = recv_drfp_mkdir(desc, db, dh, args, len);
          break;
        case 0x10:
          db = recv_drfp_rmdir(desc, db, dh, args, len);
          break;
        case 0x12:
          db = recv_drfp_dopen(desc, db, dh, args, len);
          break;
        case 0x14:
          db = recv_drfp_dclose(desc, db, dh, args, len);
          break;
        case 0x16:
          db = recv_drfp_dread(desc, db, dh, args, len);
          break;
        case 0x18:
          db = recv_drfp_getstat(desc, db, dh, args, len);
          break;
        case 0x1A:
          db = recv_drfp_chstat(desc, db, dh, args, len);
          break;
        case 0x1E:
          db = recv_drfp_rename(desc, db, dh, args, len);
          break;
        case 0x20:
          db = recv_drfp_chdir(desc, db, dh, args, len);
          break;
        case 0x28:
          db = recv_drfp_seek64(desc, db, dh, args, len);
          break;
        case 0x2C:
          db = recv_drfp_symlink(desc, db, dh, args, len);
          break;
        case 0x2E:
          db = recv_drfp_readlink(desc, db, dh, args, len);
          break;
        default:
          return db;
      }
      return db;
    }
    else
    {
      return db;
    }
  }
  else
  {
    drfp_recv_reset();
    return 0;
  }
}

