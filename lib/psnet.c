#include "dsnet_prototypes.h"

static int __cdecl set_deci1_hdr(DS_PSNETD_PRIV *priv, DS_DECI1 *p, unsigned int req, int len);
static DS_PSNETD_PRIV *__cdecl init_psnet(DS_DESC *desc);
static int __cdecl recv_deci1_hook(DS_PSNETD_PRIV *p, DS_DECI1 *deci1, void *ptr, int len);
static int __cdecl send_treset(DS_PSNETD_PRIV *p);
static int __cdecl send_deci2_hook(DS_PSNETD_PRIV *p, DECI2_HDR *dh, int len);

static int __cdecl set_deci1_hdr(DS_PSNETD_PRIV *priv, DS_DECI1 *p, unsigned int req, int len)
{
  int i; // [esp+4h] [ebp-Ch]
  unsigned int *wp; // [esp+8h] [ebp-8h]
  unsigned int sum; // [esp+Ch] [ebp-4h]

  ds_bzero(p, sizeof(DS_DECI1));
  p->mag = -24244;
  p->len = len + sizeof(DS_DECI1);
  p->cid = priv->cid;
  p->req = req;
  if ( HIBYTE(req) == 71 )
  {
    p->ctg = 18;
    p->pri = 113;
    p->rep = 0;
    p->seq = priv->gseq;
    ++priv->gseq;
  }
  else if ( HIBYTE(req) == 84 )
  {
    p->ctg = 10;
    p->pri = 10;
    p->rep = 1;
    p->seq = priv->tseq;
    ++priv->tseq;
  }
  sum = 0;
  wp = (unsigned int *)&p->mag;
  for ( i = 0; i <= 6; ++i )
    sum += *wp++;
  p->sum = sum;
  return sizeof(DS_DECI1);
}

static DS_PSNETD_PRIV *__cdecl init_psnet(DS_DESC *desc)
{
  pid_t v2; // ebx
  char *v3; // eax
  int v4; // eax
  int n; // [esp+8h] [ebp-10h]
  char *app; // [esp+Ch] [ebp-Ch]
  char *cp; // [esp+10h] [ebp-8h]
  char *cp_1; // [esp+10h] [ebp-8h]
  char *cp_2; // [esp+10h] [ebp-8h]
  char *cp_3; // [esp+10h] [ebp-8h]
  DS_PSNETD_PRIV *p; // [esp+14h] [ebp-4h]

  app = (char *)ds_program_name;
  if ( desc->psnet_priv )
    return (DS_PSNETD_PRIV *)desc->psnet_priv;
  p = (DS_PSNETD_PRIV *)ds_alloc_mem_low("psnet.c", "init_psnet", sizeof(DS_PSNETD_PRIV));
  if ( !p )
    return 0;
  ds_bzero(p, sizeof(DS_PSNETD_PRIV));
  v2 = getpid();
  p->cid = time(0) ^ v2;
  p->cid ^= p->cid >> 16;
  p->gseq = 1;
  p->tseq = 1;
  p->snd_len = 156;
  v3 = (char *)ds_alloc_mem_low("psnet.c", "init_psnet", p->snd_len);
  p->snd_ptr = v3;
  p->snd_buf = v3;
  if ( !v3 )
    return 0;
  cp = p->snd_buf;
  ds_bzero(cp, p->snd_len);
  cp_1 = &cp[set_deci1_hdr(p, (DS_DECI1 *)cp, 0x54080101u, sizeof(DS_PSNETD_PRIV))];
  *(_DWORD *)cp_1 = 18;
  cp_1 += 4;
  *(_DWORD *)cp_1 = 113;
  cp_2 = cp_1 + 12;
  n = strlen(app);
  if ( n > 31 )
    v4 = 32;
  else
    v4 = n;
  memcpy(cp_2, app, v4);
  cp_3 = &cp_2[set_deci1_hdr(p, (DS_DECI1 *)cp_2 + 2, 0x54100101u, 12) + 64];
  *(_DWORD *)cp_3 = 18;
  cp_3 += 4;
  *(_DWORD *)cp_3 = 0;
  *((_DWORD *)cp_3 + 1) = 1;
  desc->psnet_priv = p;
  return p;
}

void __cdecl ds_free_psnet(DS_DESC *desc)
{
  DS_PSNETD_PRIV *p; // [esp+0h] [ebp-4h]

  p = (DS_PSNETD_PRIV *)desc->psnet_priv;
  if ( p )
  {
    ds_free_mem_low(p->snd_buf, "psnet.c", "_ds_free_psnet");
    ds_free_mem_low(p->rcv_buf, "psnet.c", "_ds_free_psnet");
  }
}

static int __cdecl recv_deci1_hook(DS_PSNETD_PRIV *p, DS_DECI1 *deci1, void *ptr, int len)
{
  char *v5; // eax
  char *rcv_buf; // ebx
  char *v7; // ecx
  int v8; // [esp+10h] [ebp-8h]
  unsigned int req; // [esp+14h] [ebp-4h]

  req = deci1->req;
  if ( req == 1191247872 )
    return 0;
  if ( req == 1308754176 )
  {
    p->rcv_buf = (char *)ds_free_mem_low(p->rcv_buf, "psnet.c", "recv_deci1_hook");
    p->read_len = 10;
    v5 = (char *)ds_alloc_mem_low("psnet.c", "recv_deci1_hook", p->read_len);
    p->rcv_buf = v5;
    if ( v5 )
    {
      rcv_buf = p->rcv_buf;
      p->rcv_ptr = rcv_buf;
      p->read_ptr = rcv_buf;
      v7 = p->rcv_buf;
      p->rcv_ptr = v7;
      *(_WORD *)v7 = 10;
      *((_WORD *)v7 + 1) = 0;
      *((_WORD *)v7 + 2) = 1024;
      v7[6] = 72;
      v7[7] = 72;
      v7[8] = 3;
      v7[9] = 0;
      return 0;
    }
    else
    {
      return -1;
    }
  }
  else
  {
    if ( req == 1409810689 || req == 1410334977 )
    {
      v8 = *(_DWORD *)ptr;
      if ( (unsigned int)(len - 32) <= 3 || v8 )
      {
        ds_printf("_recv_deci1_hook:");
        ds_printf(" len=%d req=0x%W result=0x%W\n", len, req, v8);
        ds_exit(255);
      }
    }
    return -1;
  }
}

int __cdecl ds_read_psnet(DS_DESC *desc, char *buf, int len)
{
  unsigned int v4; // edx
  char *v5; // eax
  char *rcv_buf; // ebx
  int v7; // edx
  int v8; // edx
  unsigned int sum; // [esp+8h] [ebp-20h]
  unsigned int *wp; // [esp+Ch] [ebp-1Ch]
  int v11; // [esp+10h] [ebp-18h]
  int nbytes; // [esp+14h] [ebp-14h]
  int n_1; // [esp+14h] [ebp-14h]
  int i; // [esp+18h] [ebp-10h]
  int i_1; // [esp+18h] [ebp-10h]
  int r; // [esp+1Ch] [ebp-Ch]
  int r_1; // [esp+1Ch] [ebp-Ch]
  int r_2; // [esp+1Ch] [ebp-Ch]
  DS_DECI1 *deci1; // [esp+20h] [ebp-8h]
  DS_PSNETD_PRIV *p; // [esp+24h] [ebp-4h]

  p = init_psnet(desc);
  if ( !p )
    return -1;
  deci1 = &p->rcv_hdr;
  if ( p->rcv_len <= 0 )
  {
    p->rcv_ptr = (char *)&p->rcv_hdr;
    p->rcv_len = 0;
    p->read_len = 0;
  }
  if ( p->read_len <= 0 )
  {
    nbytes = 32 - p->rcv_len;
    if ( nbytes <= 0 )
    {
      v11 = p->rcv_hdr.len;
    }
    else
    {
      r = ds_read(desc->fd, p->rcv_ptr, nbytes);
      if ( r < 0 )
        return -1;
      if ( !r )
        return -2;
      p->rcv_ptr += r;
      v4 = r + p->rcv_len;
      p->rcv_len = v4;
      if ( v4 <= 0x1F )
        goto LABEL_12;
      if ( deci1->mag != 0xA14C )
      {
        for ( i = 0; (unsigned int)i <= 0x1E; ++i )
          *((_BYTE *)&p->rcv_hdr.mag + i) = *((_BYTE *)&p->rcv_hdr.mag + i + 1);
        --p->rcv_ptr;
        --p->rcv_len;
        errno = 11;
        return -1;
      }
      if ( p->rcv_hdr.len <= 0x1Fu )
      {
        p->rcv_len = 0;
        return 0;
      }
      sum = 0;
      wp = (unsigned int *)&p->rcv_hdr.mag;
      for ( i_1 = 0; i_1 <= 6; ++i_1 )
        sum += *wp++;
      if ( sum != p->rcv_hdr.sum )
      {
        p->rcv_len = 0;
        errno = 11;
        return -1;
      }
      v11 = p->rcv_hdr.len;
      v5 = (char *)ds_alloc_mem_low("psnet.c", "_ds_read_psnet", v11 - 32);
      p->rcv_buf = v5;
      if ( !v5 )
        return -1;
      rcv_buf = p->rcv_buf;
      p->rcv_ptr = rcv_buf;
      p->read_ptr = rcv_buf;
      p->read_len = v11 - 32;
    }
    n_1 = v11 - p->rcv_len;
    if ( n_1 > 0 )
    {
      r_1 = ds_read(desc->fd, p->rcv_ptr, n_1);
      if ( r_1 < 0 )
        return -1;
      if ( !r_1 )
        return -2;
      p->rcv_ptr += r_1;
      v7 = r_1 + p->rcv_len;
      p->rcv_len = v7;
      if ( v11 > v7 )
      {
LABEL_12:
        errno = 11;
        return -1;
      }
    }
    if ( recv_deci1_hook(p, deci1, p->rcv_buf, v11) )
    {
      p->rcv_len = 0;
      p->rcv_buf = (char *)ds_free_mem_low(p->rcv_buf, "psnet.c", "_ds_read_psnet");
      errno = 11;
      return -1;
    }
  }
  r_2 = p->read_len;
  if ( len < r_2 )
    r_2 = len;
  if ( r_2 > 0 )
  {
    memcpy(buf, p->read_ptr, r_2);
    p->read_ptr += r_2;
    v8 = p->read_len - r_2;
    p->read_len = v8;
    if ( v8 <= 0 )
    {
      p->rcv_len = 0;
      p->rcv_buf = (char *)ds_free_mem_low(p->rcv_buf, "psnet.c", "_ds_read_psnet");
    }
  }
  return r_2;
}

static int __cdecl send_treset(DS_PSNETD_PRIV *p)
{
  char *v1; // eax
  char *cp; // [esp+0h] [ebp-4h]

  p->snd_len = 36;
  v1 = (char *)ds_alloc_mem_low("psnet.c", "send_treset", p->snd_len);
  p->snd_ptr = v1;
  p->snd_buf = v1;
  if ( !v1 )
    return -1;
  cp = p->snd_buf;
  ds_bzero(cp, p->snd_len);
  *(_DWORD *)&cp[set_deci1_hdr(p, (DS_DECI1 *)cp, 0x54010000u, 4)] = 1;
  return 0;
}

static int __cdecl send_deci2_hook(DS_PSNETD_PRIV *p, DECI2_HDR *dh, int len)
{
  int lena; // [esp+14h] [ebp+10h]

  lena = len - 8;
  if ( lena < 0 )
    return -1;
  if ( dh->protocol != 1024 )
    return 0;
  if ( lena - 2 < 0 )
    return -1;
  if ( LOBYTE(dh[1].length) == 2 && send_treset(p) < 0 )
    return -1;
  return 1;
}

int __cdecl ds_write_psnet(DS_DESC *desc, char *buf, int len)
{
  int v4; // ebx
  int v5; // edx
  char *v6; // eax
  int v7; // edx
  char *snd_buf; // [esp+10h] [ebp-14h]
  char *dp_1; // [esp+10h] [ebp-14h]
  int n; // [esp+14h] [ebp-10h]
  int r; // [esp+18h] [ebp-Ch]
  int r_1; // [esp+18h] [ebp-Ch]
  DS_DECI1 *deci1; // [esp+1Ch] [ebp-8h]
  DS_PSNETD_PRIV *p; // [esp+20h] [ebp-4h]

  p = init_psnet(desc);
  if ( !p )
    return -1;
  if ( !p->snd_buf || p->snd_len <= 0 )
  {
    r = send_deci2_hook(p, (DECI2_HDR *)buf, len);
    if ( r <= 0 )
    {
      if ( r < 0 )
        return -1;
      v4 = len >> 9;
      p->snd_dslen = len;
      if ( (len & 0x1FF) != 0 )
        v5 = len + 32 * (v4 + 1);
      else
        v5 = len + 32 * v4;
      p->snd_len = v5;
      v6 = (char *)ds_alloc_mem_low("psnet.c", "_ds_write_psnet", p->snd_len);
      p->snd_buf = v6;
      p->snd_ptr = v6;
      if ( !v6 )
        return -1;
      snd_buf = p->snd_buf;
      while ( len > 0 )
      {
        n = len;
        if ( len > 512 )
          n = 512;
        deci1 = (DS_DECI1 *)snd_buf;
        dp_1 = &snd_buf[set_deci1_hdr(p, (DS_DECI1 *)snd_buf, 0x47010000u, n)];
        memcpy(&deci1[1], buf, n);
        buf += n;
        snd_buf = &dp_1[n];
        len -= n;
      }
    }
  }
  if ( p->snd_len <= 0 )
    return 0;
  r_1 = ds_write(desc->fd, p->snd_ptr, p->snd_len);
  if ( r_1 < 0 )
    return -1;
  p->snd_ptr += r_1;
  v7 = p->snd_len - r_1;
  p->snd_len = v7;
  if ( v7 > 0 )
    return 0;
  p->snd_buf = (char *)ds_free_mem_low(p->snd_buf, "psnet.c", "_ds_write_psnet");
  return p->snd_dslen;
}

