#include "dsnet_prototypes.h"

#ifndef _WIN32
static sig_t sigint;
#endif
static int kbd_raw = 0;

int ds_ioctl(int fd, int cmd, void *arg)
{
#ifndef _WIN32
  return ioctl(fd, cmd, arg);
#else
  ds_error("ioctl not supported on this platform");
  return 0;
#endif
}

void *ds_fopen(char *fname, char *mode)
{
  char path[1025]; // [esp+0h] [ebp-408h] BYREF
  void *r; // [esp+404h] [ebp-4h]

  ds_tilde_expand(path, fname);
  r = fopen(path, mode);
  if ( !r )
    ds_error("!fopen(%s, %s)", path, mode);
  return r;
}

void *ds_fopen_low(char *fname, char *mode)
{
  char path[1025]; // [esp+0h] [ebp-404h] BYREF

  ds_tilde_expand(path, fname);
  return fopen(path, mode);
}

int ds_fclose(void *stream)
{
  int r; // [esp+0h] [ebp-4h]

  if ( !stream )
    return 0;
  r = fclose((FILE *)stream);
  if ( r == -1 )
    ds_error("!fclose");
  return r;
}

int ds_fread(void *ptr, int size, int n, void *stream, int f_allow_eof)
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

int ds_fwrite(void *ptr, int size, int n, void *stream)
{
  int r; // [esp+0h] [ebp-4h]

  r = fwrite(ptr, size, n, (FILE *)stream);
  if ( n != r && ferror((FILE *)stream) )
    ds_error("!fwrite");
  return r;
}

int ds_fseek(void *stream, int offset, int whence)
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

void *ds_fload(void *stream, int whence, int off, int siz, int cnt)
{
  void *p; // [esp+0h] [ebp-4h]

  p = ds_alloc(cnt * siz + 1);
  if ( !p )
    return 0;
  if ( ds_fseek(stream, off, whence) == -1 || cnt != ds_fread(p, siz, cnt, stream, 0) )
    return ds_free(p);
  *((_BYTE *)p + cnt * siz) = 0;
  return p;
}

int ds_fsize(char *fname, int *psize)
{
  char path[1025]; // [esp+0h] [ebp-45Ch] BYREF
  struct stat stbuf; // [esp+404h] [ebp-58h] BYREF

  ds_tilde_expand(path, fname);
  if ( stat(path, &stbuf) < 0 )
    return ds_error("!stat(%s)", path);
  *psize = stbuf.st_size;
  return 0;
}

void *ds_popen(char *cmd, char *type)
{
  void *r; // [esp+0h] [ebp-4h]

#ifndef _WIN32
  sigint = signal(SIGINT, SIG_IGN);
  kbd_raw = ds_resume_kbd();
  r = popen(cmd, type);
  if ( !r )
    ds_error("!popen(\"%s\", \"%s\")", cmd, type);
#else
  r = NULL;
  ds_error("popen not supported on this platform");
#endif
  return r;
}

int ds_pclose(void *stream)
{
  int r; // [esp+0h] [ebp-4h]

  r = 0;
#ifndef _WIN32
  if ( stream )
    r = pclose((FILE *)stream);
  if ( kbd_raw )
    ds_raw_kbd();
  signal(SIGINT, sigint);
#endif
  return r;
}

int ds_access(char *fname, int rmode)
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

