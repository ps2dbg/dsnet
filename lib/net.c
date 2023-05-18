#include "dsnet_prototypes.h"

static int __cdecl getport_net(char *portp);
static int __cdecl getaddr_net(char *name, struct sockaddr_in *sin);
static int __cdecl setup_net(int s);
static DS_DESC *__cdecl ds_open_dev(char *name, DS_RECV_FUNC *recv_func);

static int __cdecl getport_net(char *portp)
{
  int v1; // eax
  int f_rel; // [esp+0h] [ebp-10h]
  int value; // [esp+4h] [ebp-Ch] BYREF
  int delta; // [esp+8h] [ebp-8h] BYREF
  struct servent *sp; // [esp+Ch] [ebp-4h]

  sp = 0;
  delta = 0;
  f_rel = 0;
  if ( portp )
  {
    while ( *portp == 58 || *portp == 47 || *portp == 43 )
    {
      v1 = 0;
      if ( *portp == 47 || *portp == 43 )
        v1 = 1;
      f_rel += v1;
      ++portp;
    }
    if ( f_rel )
    {
      if ( sscanf(portp, "%d", &delta) != 1 )
      {
        ds_error("invalid port %s", portp);
        return 0;
      }
      portp = "8510";
    }
    else if ( !*portp )
    {
      portp = "8510";
    }
  }
  else
  {
    portp = "8510";
  }
  if ( sscanf(portp, "%d", &value) != 1 || !value )
  {
    sp = getservbyname(portp, "tcp");
    if ( !sp )
    {
      ds_error("unknown port %s", portp);
      return 0;
    }
    value = ntohs(sp->s_port);
  }
  if ( delta > 0 )
    value += delta;
  return htons(value);
}

static int __cdecl getaddr_net(char *name, struct sockaddr_in *sin)
{
  unsigned __int16 v3; // ax
  int n; // [esp+0h] [ebp-54h]
  unsigned int temp; // [esp+4h] [ebp-50h]
  struct hostent *hp; // [esp+8h] [ebp-4Ch]
  char *portp; // [esp+Ch] [ebp-48h]
  char *hostp; // [esp+10h] [ebp-44h]
  char hostname[64]; // [esp+14h] [ebp-40h] BYREF

  if ( !name )
    name = "";
  for ( portp = name; *portp && *portp != 58 && *portp != 47 && *portp != 43; ++portp )
    ;
  if ( portp )
  {
    n = portp - name;
    if ( (unsigned int)(portp - name) > 0x3F )
      n = 63;
    hostp = strncpy(hostname, name, n);
    hostname[n] = 0;
  }
  else
  {
    hostp = name;
  }
  if ( !*hostp )
    hostp = "127.0.0.1";
  ds_bzero(sin, 0x10u);
  temp = inet_addr(hostp);
  if ( temp == -1 )
  {
    hp = gethostbyname(hostp);
    if ( !hp )
      return ds_error("unknown host %s", hostp);
    sin->sin_family = hp->h_addrtype;
    memcpy(&sin->sin_addr, *(const void **)hp->h_addr_list, hp->h_length);
  }
  else
  {
    sin->sin_addr.s_addr = temp;
    sin->sin_family = 2;
  }
  v3 = getport_net(portp);
  sin->sin_port = v3;
  if ( v3 )
    return 0;
  else
    return -1;
}

static int __cdecl setup_net(int s)
{
  char *v3; // eax
  int flag; // [esp+0h] [ebp-Ch]
  int hold; // [esp+4h] [ebp-8h] BYREF
  struct protoent *protoent; // [esp+8h] [ebp-4h]

#ifndef _WIN32
  signal(SIGPIPE, SIG_IGN);
#endif
  hold = 1;
  if ( setsockopt(s, 1, 9, &hold, 4u) < 0 )
    return ds_error("!setsockopt(KEEPALIVE)");
  protoent = getprotobyname("tcp");
  if ( !protoent )
    return ds_error("!getprotobyname(tcp)");
  hold = 1;
  if ( setsockopt(s, protoent->p_proto, 1, &hold, 4u) < 0 )
    return ds_error("!setsockopt(TCP_NODELAY)");
  protoent = getprotobyname("ip");
  if ( !protoent )
    return ds_error("!getprotobyname(ip)");
  hold = 16;
  if ( setsockopt(s, protoent->p_proto, 1, &hold, 4u) < 0 && errno != 92 )
  {
    v3 = strerror(errno);
    ds_error("setsockopt(IP_TOS) - %s (ignored)", v3);
  }
#ifndef _WIN32
  flag = fcntl(s, 3, 0);
  if ( flag == -1 )
    return ds_error("!fcntl(F_GETFL)");
  if ( fcntl(s, 4, flag | 0x800) >= 0 )
    return 0;
  return ds_error("!fcntl(F_SETFL)");
#else
  return 0;
#endif
}

static DS_DESC *__cdecl ds_open_dev(char *name, DS_RECV_FUNC *recv_func)
{
  int fd; // [esp+0h] [ebp-4h]

  fd = open(name, 2050);
  if ( fd >= 0 )
  {
    if ( ds_ioctl(fd, 1093009410, 0) >= 0 || errno == 32 )
    {
      return ds_add_select_list(2, fd, name, 0, recv_func);
    }
    else
    {
      close(fd);
      ds_error("open(%s) - invalid device", name);
      return 0;
    }
  }
  else
  {
    ds_error("!open(%s)", name);
    return 0;
  }
}

DS_DESC *__cdecl ds_connect_net(char *targetp, DS_RECV_FUNC *recv_func)
{
  char *v3; // eax
  int v4; // [esp-4h] [ebp-474h]
  char name[1024]; // [esp+0h] [ebp-470h] BYREF
  int i; // [esp+400h] [ebp-70h]
  int fd; // [esp+404h] [ebp-6Ch]
  struct sockaddr_in sin; // [esp+408h] [ebp-68h] BYREF
  struct stat stbuf; // [esp+418h] [ebp-58h] BYREF

  if ( targetp && stat(targetp, &stbuf) >= 0 && (stbuf.st_mode & 0xF000) == 0x2000 )
    return ds_open_dev(targetp, recv_func);
  if ( getaddr_net(targetp, &sin) < 0 )
    return 0;
  for ( i = 0; ; ++i )
  {
    if ( i > 4 )
    {
      ds_error("ds_connect - too many refused");
      return 0;
    }
    fd = socket(2, 1, 0);
    if ( fd < 0 )
    {
      ds_error("!socket");
      return 0;
    }
    if ( !connect(fd, (const struct sockaddr *)&sin, 0x10u) )
      break;
    close(fd);
    if ( errno != 111 )
    {
      ds_error("!unable to connect");
      return 0;
    }
    sleep(1u);
  }
  if ( setup_net(fd) >= 0 )
  {
    v4 = ntohs(sin.sin_port);
    v3 = inet_ntoa(sin.sin_addr);
    sprintf(name, "%s:%d", v3, v4);
    return ds_add_select_list(16, fd, name, 0, recv_func);
  }
  else
  {
    close(fd);
    return 0;
  }
}

DS_DESC *__cdecl ds_listen_net(char *portp, int (__cdecl *accept_func)(DS_DESC *desc))
{
  char *v3; // eax
  int v4; // [esp-4h] [ebp-41Ch]
  char name[1024]; // [esp+0h] [ebp-418h] BYREF
  int hold; // [esp+400h] [ebp-18h] BYREF
  int fd; // [esp+404h] [ebp-14h]
  struct sockaddr_in sin; // [esp+408h] [ebp-10h] BYREF

  ds_bzero(&sin, 0x10u);
  sin.sin_family = 2;
  sin.sin_port = getport_net(portp);
  if ( !sin.sin_port )
    return 0;
  sin.sin_addr.s_addr = 0;
  fd = socket(2, 1, 0);
  if ( fd >= 0 )
  {
    hold = 1;
    if ( setsockopt(fd, 1, 2, &hold, 4u) >= 0 )
    {
      if ( bind(fd, (const struct sockaddr *)&sin, 0x10u) >= 0 )
      {
        if ( listen(fd, 5) >= 0 )
        {
          v4 = ntohs(sin.sin_port);
          v3 = inet_ntoa(sin.sin_addr);
          sprintf(name, "%s:%d", v3, v4);
          return ds_add_select_list(4, fd, name, accept_func, 0);
        }
        else
        {
          close(fd);
          ds_error("!listen");
          return 0;
        }
      }
      else
      {
        close(fd);
        ds_error("!bind");
        return 0;
      }
    }
    else
    {
      close(fd);
      ds_error("!setsockopt(REUSEADDR)");
      return 0;
    }
  }
  else
  {
    ds_error("!socket");
    return 0;
  }
}

DS_DESC *__cdecl ds_accept(int s, int (__cdecl *accept_func)(DS_DESC *desc))
{
  char *v3; // eax
  int v4; // [esp-4h] [ebp-41Ch]
  char name[1024]; // [esp+0h] [ebp-418h] BYREF
  unsigned int len; // [esp+400h] [ebp-18h] BYREF
  int fd; // [esp+404h] [ebp-14h]
  struct sockaddr_in sin; // [esp+408h] [ebp-10h] BYREF

  len = 16;
  fd = accept(s, (struct sockaddr *)&sin, &len);
  if ( fd >= 0 )
  {
    if ( setup_net(fd) >= 0 )
    {
      v4 = ntohs(sin.sin_port);
      v3 = inet_ntoa(sin.sin_addr);
      sprintf(name, "%s:%d", v3, v4);
      return ds_add_select_list(8, fd, name, accept_func, 0);
    }
    else
    {
      close(fd);
      return 0;
    }
  }
  else
  {
    ds_error("!accept");
    return 0;
  }
}

