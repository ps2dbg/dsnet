#include "dsnet_prototypes.h"

static __sighandler_t sigint;
static int kbd_raw = 0;

int __cdecl ds_ioctl(int fd, int cmd, void *arg)
{
  return ioctl(fd, cmd, arg);
}

void *__cdecl ds_fopen(char *fname, char *mode)
{
  char path[1025]; // [esp+0h] [ebp-408h] BYREF
  void *r; // [esp+404h] [ebp-4h]

  ds_tilde_expand(path, fname);
  r = fopen(path, mode);
  if ( !r )
    ds_error("!fopen(%s, %s)", path, mode);
  return r;
}

void *__cdecl ds_fopen_low(char *fname, char *mode)
{
  char path[1025]; // [esp+0h] [ebp-404h] BYREF

  ds_tilde_expand(path, fname);
  return fopen(path, mode);
}

int __cdecl ds_fclose(void *stream)
{
  int r; // [esp+0h] [ebp-4h]

  if ( !stream )
    return 0;
  r = fclose((FILE *)stream);
  if ( r == -1 )
    ds_error("!fclose");
  return r;
}

int __cdecl ds_fread(void *ptr, int size, int n, void *stream, int f_allow_eof)
{
  int r; // [esp+0h] [ebp-4h]

  r = fread(ptr, size, n, (FILE *)stream);
  if ( n != r )
  {
    if ( ferror((FILE *)stream) )
      ds_error("!fread");
    if ( !f_allow_eof )
      ds_error("fread - unexpected eof");
  }
  return r;
}

int __cdecl ds_fwrite(void *ptr, int size, int n, void *stream)
{
  int r; // [esp+0h] [ebp-4h]

  r = fwrite(ptr, size, n, (FILE *)stream);
  if ( n != r && ferror((FILE *)stream) )
    ds_error("!fwrite");
  return r;
}

int __cdecl ds_fseek(void *stream, int offset, int whence)
{
  int r; // [esp+0h] [ebp-4h]
  int whencea; // [esp+14h] [ebp+10h]

  if ( whence == 1 )
  {
    whencea = 1;
  }
  else if ( whence > 1 )
  {
    if ( whence != 2 )
      return ds_error("ds_fseek: invalid whence");
    whencea = 2;
  }
  else
  {
    if ( whence )
      return ds_error("ds_fseek: invalid whence");
    whencea = 0;
  }
  r = fseek((FILE *)stream, offset, whencea);
  if ( r == -1 )
    ds_error("!fseek");
  return r;
}

void *__cdecl ds_fload(void *stream, int whence, int off, int siz, int cnt)
{
  void *p; // [esp+0h] [ebp-4h]

  p = ds_alloc_mem_low("file.c", "ds_fload", cnt * siz + 1);
  if ( !p )
    return 0;
  if ( ds_fseek(stream, off, whence) == -1 || cnt != ds_fread(p, siz, cnt, stream, 0) )
    return ds_free_mem_low(p, "file.c", "ds_fload");
  *((_BYTE *)p + cnt * siz) = 0;
  return p;
}

int __cdecl ds_fsize(char *fname, int *psize)
{
  char path[1025]; // [esp+0h] [ebp-45Ch] BYREF
  struct stat stbuf; // [esp+404h] [ebp-58h] BYREF

  ds_tilde_expand(path, fname);
  if ( stat(path, &stbuf) < 0 )
    return ds_error("!stat(%s)", path);
  *psize = stbuf.st_size;
  return 0;
}

void *__cdecl ds_popen(char *cmd, char *type)
{
  void *r; // [esp+0h] [ebp-4h]

  sigint = signal(2, (__sighandler_t)1);
  kbd_raw = ds_resume_kbd();
  r = popen(cmd, type);
  if ( !r )
    ds_error("!popen(\"%s\", \"%s\")", cmd, type);
  return r;
}

int __cdecl ds_pclose(void *stream)
{
  int r; // [esp+0h] [ebp-4h]

  r = 0;
  if ( stream )
    r = pclose((FILE *)stream);
  if ( kbd_raw )
    ds_raw_kbd();
  signal(2, sigint);
  return r;
}

int __cdecl ds_access(char *fname, int rmode)
{
  char path[1025]; // [esp+0h] [ebp-408h] BYREF
  int mode; // [esp+404h] [ebp-4h]

  mode = 0;
  if ( (rmode & 1) != 0 )
    LOBYTE(mode) = mode | 4;
  if ( (rmode & 2) != 0 )
    LOBYTE(mode) = mode | 2;
  if ( (rmode & 4) != 0 )
    LOBYTE(mode) = mode | 1;
  ds_tilde_expand(path, fname);
  return access(path, mode);
}

