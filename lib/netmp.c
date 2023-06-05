#include "dsnet_prototypes.h"

int __cdecl ds_send_netmp(DS_DESC *desc, int code, int result, void *ptr, int len)
{
  DSP_BUF *db; // [esp+10h] [ebp-8h]

  db = ds_alloc_buf(1024, 72, 0, len + 2);
  if ( !db )
    return -1;
  db->buf[8] = code;
  db->buf[9] = result;
  if ( ptr )
  {
    if ( len > 0 )
      memcpy(&db->buf[10], ptr, len);
  }
  ds_send_desc(desc, db);
  return 0;
}

int __cdecl ds_send_netmp1(DS_DESC *desc, int code, int result, int pri, int proto)
{
  NETMP_PROTOS protos; // [esp+10h] [ebp-Ch] BYREF
  unsigned __int16 v7; // [esp+16h] [ebp-6h]
  unsigned __int8 v8; // [esp+19h] [ebp-3h]
  unsigned __int8 v9; // [esp+1Ah] [ebp-2h]
  unsigned __int8 v10; // [esp+1Bh] [ebp-1h]

  v10 = code;
  v9 = result;
  v8 = pri;
  v7 = proto;
  protos.pri = pri;
  protos.reserved = 0;
  protos.proto = proto;
  return ds_send_netmp(desc, (unsigned __int8)code, (unsigned __int8)result, &protos, 4);
}

int __cdecl ds_reset(DS_DESC *desc, void *ptr, int len)
{
  if ( !desc )
    return -1;
  if ( (desc->type & 2) == 0 )
    return ds_send_netmp(desc, 2, 0, ptr, len);
  if ( ds_ioctl(desc->fd, 1093009409, 0) >= 0 )
    return 0;
  return ds_error("!ioctl(MRPIOC_RESET)");
}

int __cdecl ds_poweroff(DS_DESC *desc)
{
  if ( !desc )
    return -1;
  if ( (desc->type & 2) == 0 )
    return ds_send_netmp(desc, 12, 0, 0, 0);
  if ( ds_ioctl(desc->fd, 1093009412, 0) >= 0 )
    return 0;
  return ds_error("!ioctl(MRPIOC_POWEROFF)");
}

int __cdecl ds_send_netmp_msg(DS_DESC *desc, char *msg)
{
  int v3; // [esp-4h] [ebp-4h]

  v3 = strlen(msg);
  return ds_send_netmp(desc, 4, 0, msg, v3);
}

int __cdecl ds_send_netmp_default_msg(DS_DESC *desc)
{
  char *pw_name; // eax
  char *v2; // eax
  char *v3; // eax
  size_t v4; // ebx
  size_t v5; // ebx
  size_t v7; // eax
  size_t v8; // eax
  size_t v9; // eax
  int r; // [esp+4h] [ebp-64h]
  int len; // [esp+8h] [ebp-60h]
  char *p; // [esp+Ch] [ebp-5Ch]
  char *p_1; // [esp+Ch] [ebp-5Ch]
  char *msg; // [esp+10h] [ebp-58h]
  char *prog; // [esp+14h] [ebp-54h]
  char *host; // [esp+18h] [ebp-50h]
  char *user; // [esp+1Ch] [ebp-4Ch]
  char hostname[64]; // [esp+20h] [ebp-48h] BYREF
#ifndef _WIN32
  struct passwd *pw; // [esp+60h] [ebp-8h]
  unsigned int uid; // [esp+64h] [ebp-4h]

  uid = getuid();
  pw = getpwuid(uid);
  if ( pw )
    pw_name = pw->pw_name;
  else
    pw_name = "unknown-user";
#else
  pw_name = "unknown-user";
#endif
  user = pw_name;
  if ( gethostname(hostname, 0x40u) < 0 )
    v2 = "unknown-host";
  else
    v2 = hostname;
  host = v2;
  if ( ds_program_name && *ds_program_name )
    v3 = (char *)ds_program_name;
  else
    v3 = "unknown-prog";
  prog = v3;
  v4 = strlen(user);
  v5 = strlen(host) + 1 + v4;
  len = strlen(prog) + 1 + v5;
  msg = (char *)ds_alloc_mem_low("netmp.c", "ds_send_netmp_default_msg", len);
  if ( !msg )
    return -1;
  v7 = strlen(user);
  memcpy(msg, user, v7);
  p = &msg[strlen(user)];
  *p++ = 64;
  v8 = strlen(host);
  memcpy(p, host, v8);
  p_1 = &p[strlen(host)];
  *p_1 = 44;
  v9 = strlen(prog);
  memcpy(p_1 + 1, prog, v9);
  strlen(prog);
  r = ds_send_netmp(desc, 4, 0, msg, len);
  ds_free_mem_low(msg, "netmp.c", "ds_send_netmp_default_msg");
  return r;
}

int __cdecl ds_send_netmp_status_reply(DS_DESC *desc)
{
  int sec; // [esp+8h] [ebp-34h]
  uint64_t cusec; // [esp+Ch] [ebp-30h] BYREF
  uint64_t csec; // [esp+10h] [ebp-2Ch] BYREF
  int len; // [esp+14h] [ebp-28h]
  int v8; // [esp+18h] [ebp-24h]
  int ml; // [esp+1Ch] [ebp-20h]
  int n; // [esp+20h] [ebp-1Ch]
  int i_3; // [esp+24h] [ebp-18h]
  char *p; // [esp+28h] [ebp-14h]
  char *msg; // [esp+2Ch] [ebp-10h]
  NETMP_STATUS_DATA *status; // [esp+30h] [ebp-Ch]
  NETMP_PROTOS *protos; // [esp+34h] [ebp-8h]
  DS_DESC *dp; // [esp+38h] [ebp-4h]

  len = 2;
  for ( dp = ds_select_list.head; dp; dp = dp->forw )
  {
    if ( dp->type == 8 )
    {
      n = 8;
      ml = 1;
      if ( dp->msg )
      {
        ml = strlen(dp->msg) + 1;
        n += ml;
      }
      else
      {
        ++n;
      }
      while ( ml++ & 3 )
        ++n;
      len += n * dp->nprotos;
    }
  }
  msg = (char *)ds_alloc_mem_low("netmp.c", "ds_send_netmp_status_reply", len);
  p = msg;
  if ( !msg )
    return -1;
  *(_WORD *)p = 0;
  p += 2;
  ds_gettime(&csec, &cusec);
  for ( dp = ds_select_list.head; dp; dp = dp->forw )
  {
    if ( dp->type == 8 )
    {
      protos = dp->protos;
      i_3 = 0;
      while ( dp->nprotos > i_3 )
      {
        status = (NETMP_STATUS_DATA *)p;
        p += 8;
        memcpy(status, protos, 4u);
        status->protos.proto = status->protos.proto;
        sec = csec - dp->sec;
        if ( cusec - dp->usec < 0 )
          --sec;
        status->connect_time = sec;
        ml = 1;
        if ( dp->msg )
        {
          strcpy(p, dp->msg);
          ml = strlen(dp->msg) + 1;
          p += ml;
        }
        else
        {
          *p++ = 0;
        }
        while ( ml++ & 3 )
          *p++ = 0;
        ++i_3;
        ++protos;
      }
    }
  }
  v8 = ds_send_netmp(desc, 7, 0, msg, len);
  ds_free_mem_low(msg, "netmp.c", "ds_send_netmp_status_reply");
  return v8;
}

