#include "dsnet_prototypes.h"

#ifndef _WIN32
static struct termios oterm_area;

static struct termios *oterm = NULL;
#endif

int __cdecl ds_raw_kbd()
{
#ifndef _WIN32
  struct termios new_area; // [esp+4h] [ebp-44h] BYREF
  struct termios *old; // [esp+40h] [ebp-8h]
#endif
  int fd; // [esp+44h] [ebp-4h]

  fd = 0;
#ifndef _WIN32
  old = &oterm_area;
  if ( oterm )
    return -1;
  if ( tcgetattr(fd, old) < 0 )
    return ds_error("!tcgetattr()");
  memcpy(&new_area, old, sizeof(new_area));
  new_area.c_iflag &= ~(ISTRIP | ICRNL);
  new_area.c_oflag |= OPOST | TAB3;
  new_area.c_lflag &= ~(ECHOKE | ECHOE | ECHO);
  ds_bzero(new_area.c_cc, sizeof(new_area.c_cc));
  new_area.c_cc[VMIN] = 1;
  if ( tcsetattr(fd, 0, &new_area) < 0 )
    return ds_error("!tcsetattr()");
  oterm = old;
#endif
  return fd;
}

int __cdecl ds_resume_kbd()
{
  int r; // [esp+0h] [ebp-4h]

  r = 0;
#ifndef _WIN32
  if ( oterm )
  {
    tcsetattr(0, 0, oterm);
    r = 1;
  }
  oterm = 0;
#endif
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

