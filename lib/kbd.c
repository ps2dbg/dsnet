#include "dsnet_prototypes.h"

static struct termios oterm_area;

static struct termios *oterm = NULL;

int __cdecl ds_raw_kbd()
{
  unsigned int c_oflag; // ecx
  unsigned int c_lflag; // ecx
  struct termios new_area; // [esp+4h] [ebp-44h] BYREF
  struct termios *old; // [esp+40h] [ebp-8h]
  int fd; // [esp+44h] [ebp-4h]

  fd = 0;
  old = &oterm_area;
  if ( oterm )
    return -1;
  if ( tcgetattr(fd, old) < 0 )
    return ds_error("!tcgetattr()");
  memcpy(&new_area, old, sizeof(new_area));
  new_area.c_iflag &= 0xFFFFFEDF;
  c_oflag = new_area.c_oflag;
  LOBYTE(c_oflag) = LOBYTE(new_area.c_oflag) | 5;
  new_area.c_oflag = c_oflag;
  c_lflag = new_area.c_lflag;
  LOBYTE(c_lflag) = new_area.c_lflag & 0xF4;
  new_area.c_lflag = c_lflag;
  bzero(new_area.c_cc, 0x20u);
  new_area.c_cc[6] = 1;
  if ( tcsetattr(fd, 0, &new_area) < 0 )
    return ds_error("!tcsetattr()");
  oterm = old;
  return fd;
}

int __cdecl ds_resume_kbd()
{
  int r; // [esp+0h] [ebp-4h]

  r = 0;
  if ( oterm )
  {
    tcsetattr(0, 0, oterm);
    r = 1;
  }
  oterm = 0;
  return r;
}

DS_DESC *__cdecl ds_open_kbd(DS_RECV_FUNC *recv_func, int f_prompt)
{
  int fd; // [esp+0h] [ebp-4h]

  fd = ds_raw_kbd();
  if ( fd < 0 )
    return 0;
  if ( f_prompt )
  {
    if ( recv_func == ds_cmd_input )
      ds_prompt();
  }
  return ds_add_select_list(1, fd, "(kbd)", 0, recv_func);
}

