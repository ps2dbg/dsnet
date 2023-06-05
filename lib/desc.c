#include "dsnet_prototypes.h"

DS_DESC_LIST ds_select_list = { NULL, NULL };
static int desc_id = 0;
static int dev_desc_id = -1;

static unsigned __int8 last_38 = 0u;

static int __cdecl xrecv_common(DS_DESC *desc, DSP_BUF *db);
static int __cdecl xrecv_kbd(DS_DESC *desc);
static int __cdecl xrecv_dev(DS_DESC *desc);
static int __cdecl xsend_dev(DS_DESC *desc);
static int __cdecl xrecv_net(DS_DESC *desc);
static int __cdecl xrecv_comport(DS_DESC *desc);
static int __cdecl xsend_net(DS_DESC *desc);
static int __cdecl xsend_comport(DS_DESC *desc);
static int __cdecl xsend_netdev(DS_DESC *desc);
static int __cdecl xrecv_netdev(DS_DESC *desc);
static void __cdecl ds_flush_tty(DS_DESC *dp, int proto);
static int __cdecl isttyp(int proto);

DS_DESC *__cdecl ds_add_select_list(
        int type,
        int fd,
        char *name,
        int (__cdecl *accept_func)(DS_DESC *desc),
        DS_RECV_FUNC *recv_func)
{
  DS_DESC *tail; // edx
  unsigned len; // [esp+8h] [ebp-Ch]
  char *msg; // [esp+Ch] [ebp-8h]
  DS_DESC *desc; // [esp+10h] [ebp-4h]

  msg = 0;
  if ( name )
    len = strlen(name) + 1;
  else
    len = 0;

  desc = ds_alloc_mem_low("desc.c", "ds_add_select_list", len + sizeof(DS_DESC) + 8 /* FIXME: allocated size is 108 */);
  if ( !desc )
  {
    close(fd);
    return 0;
  }

  ds_bzero(desc, sizeof(DS_DESC) + 8);
  if ( type == 2 )
  {
    if ( dev_desc_id < 0 )
      dev_desc_id = desc_id++;
    desc->id = dev_desc_id;
  }
  else
  {
    desc->id = desc_id++;
  }
  desc->type = type;
  desc->f_psnet = 0;
  desc->psnet_priv = 0;
  desc->fd = fd;
  ds_gettime(&desc->sec, &desc->usec);
  desc->comport = 0;
  desc->msg = 0;
  desc->tty_len = 0;
  desc->recv_func_list.tail = 0;
  desc->recv_func_list.head = 0;
  desc->accept_func = accept_func;
  desc->protos = 0;
  desc->nprotos = 0;
  if ( name && len > 0 )
    memcpy(&desc[1], name, len);
  switch ( type )
  {
    case 2:
    case 32:
    case 64:
      msg = "open";
      break;
    case 4:
      msg = "listen";
      break;
    case 8:
      msg = "accept";
      break;
    case 16:
      msg = "connect";
      break;
  }
  if ( msg )
    ds_add_log(desc, msg, 0);
  tail = ds_select_list.tail;
  desc->back = ds_select_list.tail;
  if ( tail )
    desc->back->forw = desc;
  else
    ds_select_list.head = desc;
  desc->forw = 0;
  ds_select_list.tail = desc;

  if ( type == 8 && accept_func && accept_func(desc) < 0
    || recv_func && !ds_add_recv_func(desc, -1, -1, -1, recv_func) )
  {
    return ds_close_desc(desc);
  }

  return desc;
}

DS_RECV_FUNC_DESC *__cdecl ds_add_recv_func(DS_DESC *desc, int proto, int type, int code, DS_RECV_FUNC *func)
{
  DS_RECV_FUNC_DESC *tail; // ecx
  DS_RECV_FUNC_DESC *rf; // [esp+4h] [ebp-4h]

  if ( !desc || !func )
    return 0;
  if ( isttyp(proto) )
  {
    if ( type >= 0 || code >= 0 )
    {
LABEL_7:
      ds_error("TTYP has no type/code");
      return 0;
    }
    goto LABEL_39;
  }
  if ( proto != 320 )
  {
    if ( proto > 320 )
    {
      if ( proto == 1024 )
      {
        if ( type >= 0 )
        {
          ds_error("NETMP has no type");
          return 0;
        }
        goto LABEL_39;
      }
      if ( proto > 1024 )
      {
        if ( proto <= 1041 && proto >= 1040 )
        {
          if ( type >= 0 || code >= 0 )
            goto LABEL_7;
          goto LABEL_39;
        }
      }
      else if ( proto == 560 || proto == 576 )
      {
        goto LABEL_39;
      }
      goto LABEL_36;
    }
    if ( proto > 292 )
    {
      if ( proto == 304 )
        goto LABEL_39;
      goto LABEL_36;
    }
    if ( proto >= 288 )
    {
      if ( type >= 0 )
      {
        ds_error("DRFP has no type");
        return 0;
      }
      goto LABEL_39;
    }
    if ( proto != 1 )
    {
LABEL_36:
      if ( type >= 0 || code >= 0 )
      {
        ds_error("proto=0x%x has no type/code", proto);
        return 0;
      }
    }
  }
LABEL_39:
  rf = (DS_RECV_FUNC_DESC *)ds_alloc_mem_low("desc.c", "ds_add_recv_func", sizeof(DS_RECV_FUNC_DESC));
  if ( !rf )
    return 0;
  rf->proto = proto;
  rf->type = type;
  rf->code = code;
  rf->func = func;
  tail = desc->recv_func_list.tail;
  rf->back = tail;
  if ( tail )
    rf->back->forw = rf;
  else
    desc->recv_func_list.head = rf;
  rf->forw = 0;
  desc->recv_func_list.tail = rf;
  return rf;
}

DS_RECV_FUNC_DESC *__cdecl ds_del_recv_func(DS_DESC *desc, DS_RECV_FUNC_DESC *rf)
{
  DS_RECV_FUNC_DESC *back; // ebx
  DS_RECV_FUNC_DESC *forw; // esi

  back = rf->back;
  if ( rf->forw )
    rf->forw->back = back;
  else
    desc->recv_func_list.tail = back;
  forw = rf->forw;
  if ( rf->back )
    rf->back->forw = forw;
  else
    desc->recv_func_list.head = forw;
  return (DS_RECV_FUNC_DESC *)ds_free_mem_low(rf, "desc.c", "ds_del_recv_func");
}

DS_DESC *__cdecl ds_close_desc(DS_DESC *desc)
{
  int type; // eax
  DS_RECV_FUNC_DESC *forw; // [esp+Ch] [ebp-10h]
  DS_RECV_FUNC_DESC *prf; // [esp+10h] [ebp-Ch]
  DSP_BUF *q; // [esp+14h] [ebp-8h]
  DSP_BUF *p; // [esp+18h] [ebp-4h]

  type = desc->type;
  if ( type == 8 )
    goto LABEL_11;
  if ( type <= 8 )
  {
    if ( type != 2 && type != 4 )
      goto LABEL_12;
    goto LABEL_11;
  }
  if ( type == 32 )
    goto LABEL_11;
  if ( type > 32 )
  {
    if ( type != 64 )
      goto LABEL_12;
    goto LABEL_11;
  }
  if ( type == 16 )
LABEL_11:
    ds_add_log(desc, "close", 0);
LABEL_12:
  if ( desc->forw )
    desc->forw->back = desc->back;
  else
    ds_select_list.tail = desc->back;
  if ( desc->back )
    desc->back->forw = desc->forw;
  else
    ds_select_list.head = desc->forw;
  for ( p = desc->sque.head; p; p = q )
  {
    q = p->forw;
    ds_free_buf(p);
  }
  if ( desc->rbuf )
    ds_free_buf(desc->rbuf);
  for ( prf = desc->recv_func_list.head; prf; prf = forw )
  {
    forw = prf->forw;
    if ( desc->type != 32 && desc->type != 64 )
      prf->func(desc, 0);
    ds_free_mem_low(prf, "desc.c", "ds_close_desc");
  }
  close(desc->fd);
  ds_free_mem_low(desc->protos, "desc.c", "ds_close_desc");
  if ( desc->f_psnet )
    ds_free_psnet(desc);
  return (DS_DESC *)ds_free_mem_low(desc, "desc.c", "ds_close_desc");
}

static int __cdecl xrecv_common(DS_DESC *desc, DSP_BUF *db)
{
  int v3; // [esp+4h] [ebp-28h]
  int type; // [esp+8h] [ebp-24h]
  int proto; // [esp+Ch] [ebp-20h]
  int len; // [esp+10h] [ebp-1Ch]
  int len_1; // [esp+10h] [ebp-1Ch]
  DS_RECV_FUNC_DESC *rf; // [esp+18h] [ebp-14h]

  type = 0;
  v3 = 0;
  len = *(unsigned __int16 *)db->buf - 8;
  if ( len < 0 )
    goto LABEL_59;
  proto = *(unsigned __int16 *)&db->buf[4];
  if ( isttyp(proto) )
    goto LABEL_48;
  if ( proto == 320 )
    goto LABEL_46;
  if ( proto <= 320 )
  {
    if ( proto <= 292 )
    {
      if ( proto >= 288 )
      {
        if ( (unsigned int)len > 3 )
        {
          v3 = *(_DWORD *)&db->buf[8];
          switch ( v3 )
          {
            case 0:
              if ( (unsigned int)len > 0xB )
                goto LABEL_48;
              goto LABEL_59;
            case 1:
            case 4:
            case 7:
            case 9:
              if ( (unsigned int)len > 0xF )
                goto LABEL_48;
              goto LABEL_59;
            case 2:
            case 3:
              if ( (unsigned int)len > 0xB )
                goto LABEL_48;
              goto LABEL_59;
            case 5:
            case 6:
              if ( (unsigned int)len > 0xF && *(_DWORD *)&db->buf[20] >= (unsigned int)(len - 16) )
                goto LABEL_48;
              goto LABEL_59;
            case 8:
              if ( (unsigned int)len > 0x13 )
                goto LABEL_48;
              goto LABEL_59;
            default:
              goto LABEL_48;
          }
        }
        goto LABEL_59;
      }
      if ( proto == 1 )
      {
        len_1 = len - 4;
        if ( len_1 < 0 )
          goto LABEL_59;
        type = (unsigned __int8)db->buf[8];
        v3 = (unsigned __int8)db->buf[9];
        if ( type == 1 )
        {
          if ( (unsigned int)len_1 <= 3 )
            goto LABEL_59;
        }
        else if ( (unsigned __int8)db->buf[8] <= 1u )
        {
          if ( (unsigned int)len_1 <= 3 )
            goto LABEL_59;
        }
        else if ( type == 2 )
        {
          if ( (unsigned int)len_1 <= 1 )
            goto LABEL_59;
        }
        else if ( type == 3 && (unsigned int)len_1 <= 0x17 )
        {
          goto LABEL_59;
        }
      }
      goto LABEL_48;
    }
    if ( proto != 304 )
      goto LABEL_48;
LABEL_46:
    if ( len - 8 < 0 )
      goto LABEL_59;
    type = (unsigned __int8)db->buf[10];
    v3 = (unsigned __int8)db->buf[11];
    goto LABEL_48;
  }
  if ( proto == 1024 )
  {
    if ( len - 2 < 0 )
      goto LABEL_59;
    v3 = (unsigned __int8)db->buf[8];
    goto LABEL_48;
  }
  if ( proto <= 1024 && (proto == 560 || proto == 576) )
    goto LABEL_46;
LABEL_48:
  for ( rf = desc->recv_func_list.tail; rf; rf = rf->back )
  {
    if ( (proto == rf->proto || rf->proto < 0) && (type == rf->type || rf->type < 0) && (v3 == rf->code || rf->code < 0) )
    {
      db = rf->func(desc, db);
      if ( db )
        break;
      return 0;
    }
  }
LABEL_59:
  ds_free_buf(db);
  return -1;
}

static int __cdecl xrecv_kbd(DS_DESC *desc)
{
  int n; // [esp+0h] [ebp-10h]
  struct xrecv_kbd_dat_struct dat; // [esp+4h] [ebp-Ch] BYREF
  DSP_BUF *db; // [esp+Ch] [ebp-4h]

  n = ds_read(desc->fd, dat.str, 1);
  if ( n != 1 )
    return ds_error("!read(%s)", (const char *)&desc[1]);
  if ( last_38 == 27 )
  {
    switch ( dat.str[0] )
    {
      case '[':
        last_38 = 91;
        return 0;
      case '/':
        dat.str[0] = 18;
        break;
      case '?':
        dat.str[0] = 19;
        break;
      default:
        dat.str[1] = dat.str[0];
        dat.str[0] = 27;
        n = 2;
        break;
    }
  }
  else
  {
    if ( last_38 <= 0x1Bu )
    {
      if ( !last_38 && dat.str[0] == 27 )
      {
        last_38 = 27;
        return 0;
      }
      goto LABEL_31;
    }
    if ( last_38 == 91 )
    {
      if ( dat.str[0] == 66 )
      {
        dat.str[0] = 14;
        goto LABEL_31;
      }
      if ( dat.str[0] > 0x42u )
      {
        if ( dat.str[0] == 67 )
        {
          dat.str[0] = 6;
          goto LABEL_31;
        }
        if ( dat.str[0] == 68 )
        {
          dat.str[0] = 2;
          goto LABEL_31;
        }
      }
      else if ( dat.str[0] == 65 )
      {
        dat.str[0] = 16;
        goto LABEL_31;
      }
      dat.str[2] = dat.str[0];
      dat.str[1] = 91;
      dat.str[0] = 27;
      n = 3;
    }
  }
LABEL_31:
  last_38 = 0;
  dat.zero = 0;
  db = ds_alloc_buf(1040, 72, &dat, n + 4);
  if ( db )
    return xrecv_common(desc, db);
  else
    return -1;
}

static int __cdecl xrecv_dev(DS_DESC *desc)
{
  int nbytes; // [esp+0h] [ebp-10h]
  int r; // [esp+4h] [ebp-Ch]
  DECI2_HDR *dh; // [esp+8h] [ebp-8h]
  DSP_BUF *db; // [esp+Ch] [ebp-4h]

  nbytes = ds_ioctl(desc->fd, 1093009410, 0);
  if ( nbytes >= 0 )
  {
    if ( (unsigned int)nbytes > 7 )
    {
      db = (DSP_BUF *)ds_alloc_mem_low("desc.c", "xrecv_dev", nbytes + sizeof(DSP_BUF));
      if ( db )
      {
        dh = (DECI2_HDR *)db->buf;
        r = ds_read(desc->fd, db->buf, nbytes);
        if ( r >= 0 )
        {
          if ( r == nbytes )
          {
            if ( dh->length > 7u )
            {
              ds_add_log(desc, "recv", dh);
              return xrecv_common(desc, db);
            }
            else
            {
              ds_free_buf(db);
              return ds_error("xrecv_dev - invalid length (0x%x)", nbytes);
            }
          }
          else
          {
            ds_free_buf(db);
            return ds_error("xrecv_dev: paritial read");
          }
        }
        else if ( errno == 32 )
        {
          ds_free_buf(db);
          return -3;
        }
        else
        {
          ds_free_buf(db);
          return -1;
        }
      }
      else
      {
        return -1;
      }
    }
    else
    {
      return ds_error("xrecv_dev - too short (%d)", nbytes);
    }
  }
  else if ( errno == 32 )
  {
    return -3;
  }
  else
  {
    return ds_error("!ioctl(MRPIOC_RCVLEN)");
  }
}

int __cdecl ds_reset_info(DS_DESC *desc)
{
  int r; // [esp+0h] [ebp-4h]

  if ( desc->type != 2 )
    return -1;
  r = ds_ioctl(desc->fd, 0x41260003u, 0);
  if ( r >= 0 )
    return r;
  else
    return ds_error("!ioctl(MRPIOC_RSTINFO)");
}

static int __cdecl xsend_dev(DS_DESC *desc)
{
  DSP_BUF *head; // ecx
  DSP_BUF *back; // ebx
  DSP_BUF *forw; // esi
  int v5; // ecx
  int v6; // edx
  int r; // [esp+10h] [ebp-Ch]
  DECI2_HDR *dh; // [esp+14h] [ebp-8h]
  DSP_QUE *dq; // [esp+18h] [ebp-4h]

  dq = &desc->sque;
  if ( !desc->sbuf )
  {
    head = dq->head;
    desc->sbuf = dq->head;
    if ( !head )
      return 0;
    back = desc->sbuf->back;
    if ( desc->sbuf->forw )
      desc->sbuf->forw->back = back;
    else
      desc->sque.tail = back;
    forw = desc->sbuf->forw;
    if ( desc->sbuf->back )
      desc->sbuf->back->forw = forw;
    else
      dq->head = forw;
    dh = (DECI2_HDR *)desc->sbuf->buf;
    desc->sptr = (char *)dh;
    desc->slen = dh->length;
    if ( isttyp(dh->protocol) )
    {
      v5 = desc->tty_len - desc->slen;
      desc->tty_len = v5;
      if ( v5 < 0 )
        desc->tty_len = 0;
    }
  }
  r = ds_write(desc->fd, desc->sptr, desc->slen);
  if ( r < 0 )
    return -1;
  desc->sptr += r;
  v6 = desc->slen - r;
  desc->slen = v6;
  if ( v6 > 0 )
    return ds_error("xsend_dev: partial write");
  ds_add_log(desc, "send", (DECI2_HDR *)desc->sbuf->buf);
  desc->sbuf = ds_free_buf(desc->sbuf);
  return 0;
}

static int __cdecl xrecv_net(DS_DESC *desc)
{
  unsigned int v2; // edx
  int v3; // edx
  int nbytes; // [esp+4h] [ebp-10h]
  int n_1; // [esp+4h] [ebp-10h]
  int n_2; // [esp+4h] [ebp-10h]
  int r; // [esp+8h] [ebp-Ch]
  int r_1; // [esp+8h] [ebp-Ch]
  DECI2_HDR *dh; // [esp+Ch] [ebp-8h]
  DSP_BUF *db; // [esp+10h] [ebp-4h]

  if ( desc->rlen <= 0 )
  {
    desc->rptr = (char *)&desc->rhdr;
    desc->rlen = 0;
  }
  nbytes = 8 - desc->rlen;
  if ( nbytes > 0 )
  {
    if ( desc->f_psnet )
      r = ds_read_psnet(desc, desc->rptr, nbytes);
    else
      r = ds_read(desc->fd, desc->rptr, nbytes);
    if ( r < 0 )
    {
      if ( errno == 11 )
        return 0;
      else
        return -1;
    }
    if ( !r )
      return -2;
    desc->rptr += r;
    v2 = r + desc->rlen;
    desc->rlen = v2;
    if ( v2 <= 7 )
      return 0;
  }
  db = desc->rbuf;
  if ( !db )
  {
    n_1 = desc->rhdr.length;
    if ( (unsigned int)n_1 <= 7 )
      return ds_error("xrecv_net - invalid length (0x%x)", n_1);
    db = (DSP_BUF *)ds_alloc_mem_low("desc.c", "xrecv_net", n_1 + sizeof(DSP_BUF));
    if ( !db )
      return -1;
    memcpy(db->buf, &desc->rhdr, desc->rlen);
    desc->rptr = &db->buf[desc->rlen];
    desc->rbuf = db;
  }
  dh = (DECI2_HDR *)db->buf;
  n_2 = *(unsigned __int16 *)db->buf - desc->rlen;
  if ( n_2 > 0 )
  {
    if ( desc->f_psnet )
      r_1 = ds_read_psnet(desc, desc->rptr, n_2);
    else
      r_1 = ds_read(desc->fd, desc->rptr, n_2);
    if ( r_1 < 0 )
    {
      if ( errno == 11 )
        return 0;
      else
        return -1;
    }
    if ( !r_1 )
      return -2;
    desc->rptr += r_1;
    v3 = r_1 + desc->rlen;
    desc->rlen = v3;
    if ( v3 < dh->length )
      return 0;
  }
  desc->rbuf = 0;
  desc->rlen = 0;
  if ( *(_WORD *)&db->buf[4] != 1040 && *(_WORD *)&db->buf[4] != 1041 )
    ds_add_log(desc, "recv", dh);
  return xrecv_common(desc, db);
}

static int __cdecl xrecv_comport(DS_DESC *desc)
{
  int n; // [esp+0h] [ebp-110h]
  struct xrecv_comport_dat_struct dat; // [esp+4h] [ebp-10Ch] BYREF
  DECI2_HDR *dh; // [esp+108h] [ebp-8h]
  DSP_BUF *db; // [esp+10Ch] [ebp-4h]

  n = ds_read(desc->fd, dat.str, 256);
  if ( n > 0 )
  {
    dat.zero = 0;
    db = ds_alloc_buf(1042, 72, &dat, n + 4);
    if ( db )
    {
      dh = (DECI2_HDR *)db->buf;
      db->buf[6] = 73;
      ds_add_log(desc, "recv", dh);
      return xrecv_common(desc, db);
    }
    else
    {
      return -1;
    }
  }
  else if ( errno == 11 )
  {
    return 0;
  }
  else
  {
    return ds_error("!read(%s)", (const char *)&desc[1]);
  }
}

static int __cdecl xsend_net(DS_DESC *desc)
{
  DSP_BUF *head; // ecx
  DSP_BUF *back; // ebx
  DSP_BUF *forw; // esi
  int v5; // ecx
  int v6; // edx
  int r; // [esp+10h] [ebp-Ch]
  DECI2_HDR *dh; // [esp+14h] [ebp-8h]
  DSP_QUE *dq; // [esp+18h] [ebp-4h]

  dq = &desc->sque;
  if ( !desc->sbuf )
  {
    head = dq->head;
    desc->sbuf = dq->head;
    if ( !head )
      return 0;
    back = desc->sbuf->back;
    if ( desc->sbuf->forw )
      desc->sbuf->forw->back = back;
    else
      desc->sque.tail = back;
    forw = desc->sbuf->forw;
    if ( desc->sbuf->back )
      desc->sbuf->back->forw = forw;
    else
      dq->head = forw;
    dh = (DECI2_HDR *)desc->sbuf->buf;
    desc->sptr = (char *)dh;
    desc->slen = dh->length;
    if ( isttyp(dh->protocol) )
    {
      v5 = desc->tty_len - desc->slen;
      desc->tty_len = v5;
      if ( v5 < 0 )
        desc->tty_len = 0;
    }
  }
  if ( desc->f_psnet )
    r = ds_write_psnet(desc, desc->sptr, desc->slen);
  else
    r = ds_write(desc->fd, desc->sptr, desc->slen);
  if ( r >= 0 )
  {
    desc->sptr += r;
    v6 = desc->slen - r;
    desc->slen = v6;
    if ( v6 <= 0 )
    {
      if ( *(_WORD *)&desc->sbuf->buf[4] != 1040 && *(_WORD *)&desc->sbuf->buf[4] != 1041 )
        ds_add_log(desc, "send", (DECI2_HDR *)desc->sbuf->buf);
      desc->sbuf = ds_free_buf(desc->sbuf);
      return 1;
    }
    else
    {
      return 0;
    }
  }
  else if ( errno == 11 )
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

static int __cdecl xsend_comport(DS_DESC *desc)
{
  DSP_BUF *head; // ecx
  DSP_BUF *back; // ebx
  DSP_BUF *forw; // esi
  int v5; // ecx
  int v6; // edx
  int r; // [esp+10h] [ebp-Ch]
  DECI2_HDR *dh; // [esp+14h] [ebp-8h]
  DSP_QUE *dq; // [esp+18h] [ebp-4h]

  dq = &desc->sque;
  if ( !desc->sbuf )
  {
    head = dq->head;
    desc->sbuf = dq->head;
    if ( !head )
      return 0;
    back = desc->sbuf->back;
    if ( desc->sbuf->forw )
      desc->sbuf->forw->back = back;
    else
      desc->sque.tail = back;
    forw = desc->sbuf->forw;
    if ( desc->sbuf->back )
      desc->sbuf->back->forw = forw;
    else
      dq->head = forw;
    dh = (DECI2_HDR *)desc->sbuf->buf;
    desc->sptr = &desc->sbuf->buf[12];
    desc->slen = dh->length - 12;
    if ( isttyp(dh->protocol) )
    {
      v5 = desc->tty_len - desc->slen;
      desc->tty_len = v5;
      if ( v5 < 0 )
        desc->tty_len = 0;
    }
  }
  if ( desc->slen <= 0 )
  {
LABEL_20:
    ds_add_log(desc, "send", (DECI2_HDR *)desc->sbuf->buf);
    desc->sbuf = ds_free_buf(desc->sbuf);
    return 1;
  }
  r = ds_write(desc->fd, desc->sptr, desc->slen);
  if ( r >= 0 )
  {
    desc->sptr += r;
    v6 = desc->slen - r;
    desc->slen = v6;
    if ( v6 > 0 )
      return 0;
    goto LABEL_20;
  }
  if ( errno == 11 )
    return 0;
  else
    return -1;
}

static int __cdecl xsend_netdev(DS_DESC *desc)
{
  DSP_BUF *head; // ecx
  DSP_BUF *back; // ebx
  DSP_BUF *forw; // esi
  int v5; // edx
  int r; // [esp+10h] [ebp-Ch]
  DECI2_HDR *dh; // [esp+14h] [ebp-8h]
  DSP_QUE *dq; // [esp+18h] [ebp-4h]

  dq = &desc->sque;
  if ( !desc->sbuf )
  {
    head = dq->head;
    desc->sbuf = dq->head;
    if ( !head )
      return 0;
    back = desc->sbuf->back;
    if ( desc->sbuf->forw )
      desc->sbuf->forw->back = back;
    else
      desc->sque.tail = back;
    forw = desc->sbuf->forw;
    if ( desc->sbuf->back )
      desc->sbuf->back->forw = forw;
    else
      dq->head = forw;
    dh = (DECI2_HDR *)desc->sbuf->buf;
    if ( *(_WORD *)&desc->sbuf->buf[4] != 1296 )
    {
      desc->sbuf = ds_free_buf(desc->sbuf);
      ds_error("xsend_netdev: invalid protocol");
      return 0;
    }
    desc->sptr = &desc->sbuf->buf[8];
    desc->slen = dh->length - 8;
  }
  r = ds_write(desc->fd, desc->sptr, desc->slen);
  if ( r < 0 )
    return -1;
  desc->sptr += r;
  v5 = desc->slen - r;
  desc->slen = v5;
  if ( v5 <= 0 )
  {
    ds_add_log(desc, "send", (DECI2_HDR *)desc->sbuf->buf);
    desc->sbuf = ds_free_buf(desc->sbuf);
  }
  else
  {
    ds_error("xsend_netdev: partial write");
  }
  return 0;
}

static int __cdecl xrecv_netdev(DS_DESC *desc)
{
  int n; // [esp+0h] [ebp-Ch]
  DECI2_HDR *dh; // [esp+4h] [ebp-8h]
  DSP_BUF *db; // [esp+8h] [ebp-4h]

  db = (DSP_BUF *)ds_alloc_mem_low("desc.c", "xrecv_netdev", 4104 + sizeof(DSP_BUF));
  if ( !db )
    return -1;
  dh = (DECI2_HDR *)db->buf;
  n = ds_read(desc->fd, &db->buf[8], 4096);
  if ( n > 0 )
  {
    dh->length = n + 8;
    *(_WORD *)&db->buf[2] = 0;
    *(_WORD *)&db->buf[4] = 1296;
    db->buf[6] = 72;
    db->buf[7] = 73;
    ds_add_log(desc, "recv", dh);
    return xrecv_common(desc, db);
  }
  else
  {
    ds_free_buf(db);
    return -1;
  }
}

int __cdecl ds_select_desc(int sec, int usec)
{
  int r; // [esp+10h] [ebp-118h]
  struct timeval *tv; // [esp+14h] [ebp-114h]
  struct timeval tval; // [esp+18h] [ebp-110h] BYREF
  fd_set wfds; // [esp+20h] [ebp-108h] BYREF
  fd_set rfds; // [esp+A0h] [ebp-88h] BYREF
  DS_DESC *q; // [esp+120h] [ebp-8h]
  DS_DESC *p; // [esp+124h] [ebp-4h]

  tv = 0;
  FD_ZERO(&rfds);
  FD_ZERO(&wfds);

  for ( p = ds_select_list.head; p; p = p->forw )
  {
    FD_SET(p->fd, &rfds);
    if ( (p->type & 0x7A) != 0 && (p->sque.head || p->sbuf) )
      FD_SET(p->fd, &wfds);
  }

  if ( sec >= 0 && usec >= 0 )
  {
    tv = &tval;
    tval.tv_sec = sec;
    tval.tv_usec = usec;
  }

  r = select(256, &rfds, &wfds, 0, tv);
  if ( r < 0 )
  {
    if ( errno == EINTR )
    {
      return 1;
    }
    else
    {
      return ds_error("!select");
    }
  }

  if ( !r )
  {
    return 0;
  }

  for ( p = ds_select_list.head; p; p = q )
  {
    // list_for_each_safe
    q = p->forw;
    if ( FD_ISSET(p->fd, &rfds))
    {
      switch ( p->type )
      {
        case 1:
          xrecv_kbd(p);
          break;
        case 2:
          if ( xrecv_dev(p) == -3 )
            ds_close_desc(p);
          break;
        case 4:
            ds_accept(p->fd, p->accept_func);
          break;
        case 8:
        case 16:
          if ( xrecv_net(p) == -2 )
            ds_close_desc(p);
          break;
        case 32:
          xrecv_comport(p);
          break;
        case 64:
          xrecv_netdev(p);
          break;
      }
    }
  }

  for ( p = ds_select_list.head; p; p = q )
  {
    // list_for_each_safe
    q = p->forw;
    if ( FD_ISSET(p->fd, &wfds) )
    {
      switch ( p->type )
      {
        case 2:
          if ( xsend_dev(p) < 0 )
            ds_close_desc(p);
          break;
        case 8:
        case 16:
          if ( xsend_net(p) < 0 )
            ds_close_desc(p);
          break;
        case 32:
          if ( xsend_comport(p) < 0 )
            ds_close_desc(p);
          break;
        default:
          if ( p->type == 64 && xsend_netdev(p) < 0 )
            ds_close_desc(p);
          break;
      }
    }
  }

  return 1;
}

DS_DESC *__cdecl ds_desc_by_proto(int proto)
{
  int i_3; // [esp+0h] [ebp-14h]
  int cpri; // [esp+4h] [ebp-10h]
  NETMP_PROTOS *protos; // [esp+8h] [ebp-Ch]
  DS_DESC *pri_dp; // [esp+Ch] [ebp-8h]
  DS_DESC *dp; // [esp+10h] [ebp-4h]

  pri_dp = 0;
  cpri = -1;
  for ( dp = ds_select_list.head; dp; dp = dp->forw )
  {
    if ( (dp->type & 0x18) != 0 )
    {
      protos = dp->protos;
      if ( protos )
      {
        for ( i_3 = 0; dp->nprotos > i_3; ++i_3 )
        {
          if ( proto == protos->proto && cpri < protos->pri )
          {
            pri_dp = dp;
            cpri = protos->pri;
          }
          ++protos;
        }
      }
    }
  }
  return pri_dp;
}

int __cdecl ds_isbusy_desc(int pri, int proto)
{
  int i_3; // [esp+8h] [ebp-10h]
  NETMP_PROTOS *protos; // [esp+Ch] [ebp-Ch]
  DS_DESC *dp; // [esp+10h] [ebp-8h]

  if ( isttyp((unsigned __int16)proto) )
    return 0;
  for ( dp = ds_select_list.head; dp; dp = dp->forw )
  {
    protos = dp->protos;
    if ( protos )
    {
      for ( i_3 = 0; dp->nprotos > i_3; ++i_3 )
      {
        if ( protos->pri == (_BYTE)pri && protos->proto == (_WORD)proto )
          return 1;
        ++protos;
      }
    }
  }
  return 0;
}

static void __cdecl ds_flush_tty(DS_DESC *dp, int proto)
{
  DSP_BUF *v2; // ebx
  DSP_BUF *tail; // ecx
  char msg[100]; // [esp+8h] [ebp-78h] BYREF
  int v5; // [esp+6Ch] [ebp-14h]
  int node; // [esp+70h] [ebp-10h]
  DECI2_HDR *dh; // [esp+74h] [ebp-Ch]
  DSP_BUF *q; // [esp+78h] [ebp-8h]
  DSP_BUF *p; // [esp+7Ch] [ebp-4h]

  node = 0;
  v5 = 0;
  for ( p = dp->sque.head; p; p = q )
  {
    q = p->forw;
    dh = (DECI2_HDR *)p->buf;
    v5 += *(unsigned __int16 *)p->buf;
    node = (unsigned __int8)p->buf[7];
    ds_free_buf(p);
  }
  dp->sque.tail = 0;
  dp->sque.head = 0;
  dp->tty_len = sprintf(msg, "\n\n*** TTY buffer overflow. Skip %d bytes data ***\n\n", v5);
  p = ds_alloc_buf((unsigned __int16)proto, (unsigned __int8)node, 0, dp->tty_len + 4);
  if ( p )
  {
    dh = (DECI2_HDR *)p->buf;
    *(_DWORD *)&p->buf[8] = 0;
    memcpy(&dh[1].protocol, msg, dp->tty_len);
    v2 = p;
    tail = dp->sque.tail;
    p->back = tail;
    if ( tail )
      p->back->forw = v2;
    else
      dp->sque.head = v2;
    p->forw = 0;
    dp->sque.tail = p;
  }
}

DSP_BUF *__cdecl ds_send_net(int proto, DSP_BUF *db)
{
  DSP_BUF *tail; // ecx
  DSP_BUF *v3; // ecx
  DSP_BUF *v4; // ecx
  int len; // [esp+10h] [ebp-14h]
  int i_3; // [esp+14h] [ebp-10h]
  NETMP_PROTOS *protos; // [esp+18h] [ebp-Ch]
  DSP_BUF *cp; // [esp+1Ch] [ebp-8h]
  DSP_BUF *cp_1; // [esp+1Ch] [ebp-8h]
  DS_DESC *dp; // [esp+20h] [ebp-4h]
  DS_DESC *dp_1; // [esp+20h] [ebp-4h]
  DS_DESC *dp_2; // [esp+20h] [ebp-4h]

  if ( !proto )
  {
    for ( dp = ds_select_list.head; dp; dp = dp->forw )
    {
      if ( (dp->type & 0x18) != 0 && dp->protos )
      {
        cp = ds_dup_buf(db);
        if ( !cp )
          return ds_free_buf(db);
        tail = dp->sque.tail;
        cp->back = tail;
        if ( tail )
          cp->back->forw = cp;
        else
          dp->sque.head = cp;
        cp->forw = 0;
        dp->sque.tail = cp;
      }
    }
    return ds_free_buf(db);
  }
  if ( isttyp(proto) )
  {
    len = *(unsigned __int16 *)db->buf;
    for ( dp_1 = ds_select_list.head; dp_1; dp_1 = dp_1->forw )
    {
      if ( (dp_1->type & 0x18) != 0 )
      {
        protos = dp_1->protos;
        if ( protos )
        {
          if ( ds_opt_tty_max_size && ds_opt_tty_max_size->val > 0 && dp_1->tty_len > ds_opt_tty_max_size->val )
            ds_flush_tty(dp_1, proto);
          for ( i_3 = 0; dp_1->nprotos > i_3; ++i_3 )
          {
            if ( proto == protos->proto )
            {
              cp_1 = ds_dup_buf(db);
              if ( !cp_1 )
                return ds_free_buf(db);
              v3 = dp_1->sque.tail;
              cp_1->back = v3;
              if ( v3 )
                cp_1->back->forw = cp_1;
              else
                dp_1->sque.head = cp_1;
              cp_1->forw = 0;
              dp_1->sque.tail = cp_1;
              dp_1->tty_len += len;
              break;
            }
            ++protos;
          }
        }
      }
    }
    return ds_free_buf(db);
  }
  dp_2 = ds_desc_by_proto(proto);
  if ( !dp_2 )
    return ds_free_buf(db);
  v4 = dp_2->sque.tail;
  db->back = v4;
  if ( v4 )
    db->back->forw = db;
  else
    dp_2->sque.head = db;
  db->forw = 0;
  dp_2->sque.tail = db;
  return 0;
}

DSP_BUF *__cdecl ds_send_desc(DS_DESC *desc, DSP_BUF *db)
{
  DSP_BUF *tail; // ebx

  tail = desc->sque.tail;
  db->back = tail;
  if ( tail )
    db->back->forw = db;
  else
    desc->sque.head = db;
  db->forw = 0;
  desc->sque.tail = db;
  return 0;
}

void __cdecl ds_flush_desc_by_proto(int proto)
{
  DSP_BUF *back; // ebx
  DSP_BUF *v2; // esi
  int nb; // [esp+8h] [ebp-18h]
  int np; // [esp+Ch] [ebp-14h]
  DSP_BUF *forw; // [esp+14h] [ebp-Ch]
  DSP_BUF *p; // [esp+18h] [ebp-8h]
  DS_DESC *dp; // [esp+1Ch] [ebp-4h]

  np = 0;
  nb = 0;
  for ( dp = ds_select_list.head; dp; dp = dp->forw )
  {
    for ( p = dp->sque.head; p; p = forw )
    {
      forw = p->forw;
      if ( proto == *(unsigned __int16 *)&p->buf[4] )
      {
        ++np;
        nb += *(unsigned __int16 *)p->buf;
        back = p->back;
        if ( p->forw )
          p->forw->back = back;
        else
          dp->sque.tail = back;
        v2 = p->forw;
        if ( p->back )
          p->back->forw = v2;
        else
          dp->sque.head = v2;
        ds_free_buf(p);
      }
    }
    if ( isttyp(proto) )
      dp->tty_len = 0;
  }
}

void __cdecl ds_status_desc()
{
  int psec; // [esp+Ch] [ebp-24h]
  int64_t usec; // [esp+14h] [ebp-1Ch] BYREF
  int64_t sec; // [esp+18h] [ebp-18h] BYREF
  int i_3; // [esp+1Ch] [ebp-14h]
  int n; // [esp+20h] [ebp-10h]
  NETMP_PROTOS *protos; // [esp+24h] [ebp-Ch]
  DSP_BUF *db; // [esp+28h] [ebp-8h]
  DS_DESC *p; // [esp+2Ch] [ebp-4h]

  ds_printf("desc status:\n");
  for ( p = ds_select_list.head; p; p = p->forw )
  {
    ds_printf(" 0x%02x", p->id);
    if ( (unsigned int)(p->type - 1) <= 0x3F )
    {
      switch ( p->type )
      {
        case 1:
          ds_printf(" type=kbd");
          break;
        case 2:
          ds_printf(" type=dev");
          break;
      }
    }
    if ( p->f_psnet )
      ds_printf(",psnet");
    if ( ds_disp_desc != p )
    {
      n = 0;
      for ( db = p->sque.head; db; db = db->forw )
        ++n;
      if ( n > 0 )
        ds_printf(" sque=%d", n);
      if ( p->sbuf )
        ds_printf(" sbuf=%p", p->sbuf);
      if ( p->sptr && p->slen > 0 )
        ds_printf(" sptr=%p slen=%d", p->sptr, p->slen);
      if ( p->rbuf )
        ds_printf(" rbuf=%p", p->rbuf);
      if ( p->rptr && p->rlen > 0 )
        ds_printf(" rptr=%p rlen=%d", p->rptr, p->rlen);
    }
    protos = p->protos;
    i_3 = 0;
    while ( p->nprotos > i_3 )
    {
      ds_printf(" pri=%B,proto=%<SP>", protos->pri, protos->proto);
      ++i_3;
      ++protos;
    }
    psec = p->type;
    if ( psec == 8 )
      goto LABEL_38;
    if ( psec > 8 )
    {
      if ( psec == 32 )
        goto LABEL_38;
      if ( psec > 32 )
      {
        if ( psec != 64 )
          goto LABEL_39;
LABEL_38:
        ds_printf(" %s", (const char *)&p[1]);
        goto LABEL_39;
      }
      if ( psec == 16 )
        goto LABEL_38;
    }
    else if ( psec == 2 || psec == 4 )
    {
      goto LABEL_38;
    }
LABEL_39:
    ds_gettime(&sec, &usec);
    sec -= p->sec;
    usec -= p->usec;
    if ( usec < 0 )
    {
      --sec;
      usec -= 1000000;
    }
    ds_printf(" (");
    if ( sec / 86400 > 0 )
      ds_printf("%d+", sec / 86400);
    ds_printf("%02d:%02d:%02d)", sec / 3600 % 24, sec / 60 % 60, sec % 60);
    if ( p->msg )
      ds_printf(" [%s]", p->msg);
    if ( p->tty_len > 0 )
      ds_printf(" tty_len=%d", p->tty_len);
    ds_printf("\n");
  }
}

static int __cdecl isttyp(int proto)
{
  if ( proto > 271 && proto <= 281 )
    return 1;
  if ( proto == 287 )
    return 1;
  if ( proto > 527 && proto <= 537 )
    return 1;
  if ( proto == 543 )
    return 1;
  if ( proto == 1042 )
    return 1;
  return proto == 1043;
}
