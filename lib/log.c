#include "dsnet_prototypes.h"

static struct __anon_struct_122 {LOG_DATA *head; LOG_DATA *tail;} log_data_list = { NULL, NULL };
static int ds_log_cur_size = 0;

static LOG_DATA *__cdecl print_log(LOG_DATA *p, LOG_DATA *last, int f_l, int f_n);

void __cdecl ds_add_log(DS_DESC *desc, char *msg, DECI2_HDR *dh)
{
  int val; // eax
  int v4; // eax
  size_t v5; // eax
  int v6; // ecx
  int id; // edx
  LOG_DATA *tail; // edx
  unsigned __int8 *dest; // [esp+10h] [ebp-1Ch]
  int dlen; // [esp+14h] [ebp-18h]
  int nlen; // [esp+18h] [ebp-14h]
  int size; // [esp+1Ch] [ebp-10h]
  int n_1; // [esp+1Ch] [ebp-10h]
  int psiz; // [esp+20h] [ebp-Ch]
  int tsiz; // [esp+24h] [ebp-8h]
  LOG_DATA *p; // [esp+28h] [ebp-4h]
  LOG_DATA *p_1; // [esp+28h] [ebp-4h]

  if ( ds_opt_log_total_size )
    val = ds_opt_log_total_size->val;
  else
    val = 0;
  tsiz = val;
  if ( ds_opt_log_packet_size )
    v4 = ds_opt_log_packet_size->val;
  else
    v4 = 0;
  psiz = v4;
  nlen = 0;
  dlen = 0;
  if ( tsiz > 0 )
  {
    if ( desc )
    {
      v5 = strlen((const char *)&desc[1]);
      v6 = v5 + 4;
      LOBYTE(v6) = (v5 + 4) & 0xFC;
      nlen = v6;
    }
    if ( dh )
    {
      dlen = dh->length;
      if ( psiz >= 0 && dlen > psiz )
        dlen = psiz;
    }
    size = nlen + dlen + sizeof(LOG_DATA) + 8 /* FIXME: correct size for struct timeval? */;
    if ( tsiz >= size )
    {
      p = (LOG_DATA *)ds_alloc_mem_low("log.c", "ds_add_log", size);
      if ( p )
      {
        gettimeofday(&p->tval, 0);
        p->msg = msg;
        if ( desc )
          id = desc->id;
        else
          id = -1;
        p->id = id;
        p->nlen = nlen;
        p->dlen = dlen;
        dest = (unsigned __int8 *)&p[1];
        if ( nlen > 0 )
        {
          memcpy(dest, &desc[1], nlen);
          dest += nlen;
        }
        if ( dlen > 0 )
          memcpy(dest, dh, dlen);
        tail = log_data_list.tail;
        p->back = log_data_list.tail;
        if ( tail )
          p->back->forw = p;
        else
          log_data_list.head = p;
        p->forw = 0;
        log_data_list.tail = p;
        ds_log_cur_size += size;
        if ( tsiz < ds_log_cur_size )
        {
          while ( ds_log_cur_size > tsiz )
          {
            p_1 = log_data_list.head;
            if ( !log_data_list.head )
              break;
            if ( log_data_list.head->forw )
              log_data_list.head->forw->back = log_data_list.head->back;
            else
              log_data_list.tail = log_data_list.head->back;
            if ( p_1->back )
              p_1->back->forw = p_1->forw;
            else
              log_data_list.head = p_1->forw;
            n_1 = p_1->nlen + p_1->dlen + 32;
            ds_free_mem_low(p_1, "log.c", "ds_add_log");
            ds_log_cur_size -= n_1;
          }
        }
      }
    }
  }
}

static LOG_DATA *__cdecl print_log(LOG_DATA *p, LOG_DATA *last, int f_l, int f_n)
{
  unsigned __int8 *bp; // [esp+0h] [ebp-20h]
  int v6; // [esp+4h] [ebp-1Ch]
  int v7; // [esp+8h] [ebp-18h]
  int i; // [esp+Ch] [ebp-14h]

  if ( !last )
    last = p;
  if ( p->id < 0 )
    ds_printf("     ");
  else
    ds_printf(" 0x%02x", p->id);
  ds_printf(" ");
  if ( f_l )
  {
    v6 = p->tval.tv_sec - last->tval.tv_sec;
    v7 = p->tval.tv_usec - last->tval.tv_usec;
    if ( v7 < 0 )
    {
      --v6;
      v7 += 1000000;
    }
    ds_printf(" +%2d.%06d ", v6, v7);
  }
  bp = (unsigned __int8 *)&p[1];
  if ( p->nlen > 0 )
  {
    if ( f_l )
      ds_printf(" %s", (const char *)bp);
    bp += p->nlen;
  }
  if ( f_l )
    ds_printf(" ");
  ds_printf("%s", p->msg);
  if ( p->dlen <= 0 )
    goto LABEL_23;
  if ( f_n )
  {
    for ( i = 0; p->dlen > i; ++i )
      ds_printf(" %02x", bp[i]);
LABEL_23:
    ds_printf("\n");
    return p;
  }
  ds_dump("", (DECI2_HDR *)bp, p->dlen);
  return p;
}

int __cdecl ds_log_cmd(int ac, char **av)
{
  DECI2_HDR *v3; // eax
  DECI2_HDR *v4; // eax
  DECI2_HDR *dh; // [esp+0h] [ebp-50h]
  DECI2_HDR *dh_1; // [esp+0h] [ebp-50h]
  char *msg; // [esp+4h] [ebp-4Ch]
  char *arg; // [esp+8h] [ebp-48h]
  int f_v; // [esp+Ch] [ebp-44h]
  int dst; // [esp+10h] [ebp-40h] BYREF
  int src; // [esp+14h] [ebp-3Ch] BYREF
  int proto; // [esp+18h] [ebp-38h] BYREF
  int v13; // [esp+1Ch] [ebp-34h]
  int lo; // [esp+20h] [ebp-30h]
  int n; // [esp+24h] [ebp-2Ch]
  int i; // [esp+28h] [ebp-28h]
  int pv; // [esp+2Ch] [ebp-24h] BYREF
  int v18; // [esp+30h] [ebp-20h]
  int v19; // [esp+34h] [ebp-1Ch] BYREF
  int f_r; // [esp+38h] [ebp-18h]
  int f_n; // [esp+3Ch] [ebp-14h]
  int f_l; // [esp+40h] [ebp-10h]
  LOG_DATA *last; // [esp+44h] [ebp-Ch]
  LOG_DATA *q; // [esp+48h] [ebp-8h]
  LOG_DATA *p; // [esp+4Ch] [ebp-4h]

  f_l = 0;
  f_n = 0;
  f_r = 0;
  v19 = -1;
  v18 = 0;
  pv = 20;
  proto = -1;
  src = -1;
  dst = -1;
  f_v = 0;
  msg = 0;
  if ( ac > 0 )
  {
    --ac;
    ++av;
  }
  while ( ac > 0 )
  {
    arg = *av;
    if ( **av == 45 )
      ++arg;
    if ( !strcmp("long", arg) || !strcmp("l", arg) )
    {
      ++f_l;
    }
    else if ( !strcmp("num", arg) || !strcmp("n", arg) )
    {
      ++f_n;
    }
    else if ( !strcmp("remove", arg) || !strcmp("r", arg) )
    {
      ++f_r;
    }
    else if ( !strcmp("all", arg) || !strcmp("a", arg) )
    {
      ++v18;
    }
    else if ( !strcmp("tail", arg) || !strcmp("t", arg) || !strcmp("head", arg) || !strcmp("h", arg) )
    {
      ++av;
      if ( --ac <= 0 )
        goto LABEL_59;
      if ( ds_eval_word(*av, (unsigned int *)&pv) )
        return -1;
      if ( *arg == 104 )
        pv = -pv;
    }
    else if ( !strcmp("id", arg) || !strcmp("i", arg) )
    {
      ++av;
      if ( --ac <= 0 )
        goto LABEL_59;
      if ( ds_eval_word(*av, (unsigned int *)&v19) )
        return -1;
    }
    else if ( !strcmp("msg", arg) || !strcmp("m", arg) )
    {
      ++av;
      if ( --ac <= 0 )
        goto LABEL_59;
      msg = *av;
    }
    else if ( !strcmp("proto", arg) || !strcmp("p", arg) )
    {
      ++av;
      if ( --ac <= 0 )
        goto LABEL_59;
      if ( ds_scan_proto(*av, &proto) )
        return -1;
    }
    else if ( !strcmp("src", arg) || !strcmp("s", arg) )
    {
      ++av;
      if ( --ac <= 0 )
        goto LABEL_59;
      if ( ds_scan_nid(*av, &src) )
        return -1;
    }
    else if ( !strcmp("dst", arg) || !strcmp("d", arg) )
    {
      ++av;
      if ( --ac <= 0 )
        goto LABEL_59;
      if ( ds_scan_nid(*av, &dst) )
        return -1;
    }
    else
    {
      if ( strcmp("verbose", arg) && strcmp("v", arg) )
      {
LABEL_59:
        ds_printf("Usage: show log [[-]<option>]...\n");
        ds_printf("  <option>:\n");
        ds_printf("    l[ong]          format long\n");
        ds_printf("    n[um]           format numerical\n");
        ds_printf("    r[emove]        remove all entry\n");
        ds_printf("    a[ll]           range all\n");
        ds_printf("    t[ail] <N>      range last <N>\n");
        ds_printf("    h[ead] <N>      range first <N>\n");
        ds_printf("    i[d] <ID>       only <ID>\n");
        ds_printf("    m[sg] <msg>     only <msg>\n");
        ds_printf("    p[roto] <proto> only <proto>\n");
        ds_printf("    s[rc] <src>     only <src>\n");
        ds_printf("    d[st] <dst>     only <dst>\n");
        ds_printf("    v[erbose]       verbose mode\n");
        return -1;
      }
      ++f_v;
    }
    --ac;
    ++av;
  }
  n = 0;
  p = log_data_list.head;
  while ( p )
  {
    p = p->forw;
    ++n;
  }
  if ( f_v )
  {
    ds_printf("# log_cur_size=%d entries=%d", ds_log_cur_size, n);
    if ( v18 )
    {
      ds_printf(" (all");
    }
    else if ( pv < 0 )
    {
      ds_printf(" (head %d", -pv);
    }
    else
    {
      ds_printf(" (tail %d", pv);
    }
    if ( v19 >= 0 )
      ds_printf(" id=0x%x", v19);
    if ( msg )
      ds_printf(" msg=%s", msg);
    if ( proto >= 0 )
      ds_printf(" proto=%<SP>", proto);
    if ( src >= 0 )
      ds_printf(" src=%<SN>", src);
    if ( dst >= 0 )
      ds_printf(" dst=%<SN>", dst);
    ds_printf(")\n");
  }
  n = 0;
  for ( p = log_data_list.head; p; p = p->forw )
  {
    if ( !strcmp("send", p->msg) || !strcmp("recv", p->msg) )
      v3 = (DECI2_HDR *)((char *)&p[1] + p->nlen);
    else
      v3 = 0;
    dh = v3;
    if ( (v19 < 0 || p->id == v19)
      && (!msg || !strcmp(msg, p->msg))
      && (proto < 0 || dh && proto == dh->protocol)
      && (src < 0 || dh && src == dh->source)
      && (dst < 0 || dh && dst == dh->destination) )
    {
      ++n;
    }
  }
  p = log_data_list.head;
  last = 0;
  if ( v18 )
  {
    lo = 0;
    v13 = n;
  }
  else if ( pv < 0 )
  {
    lo = 0;
    v13 = -pv;
    if ( n < -pv )
      v13 = n;
  }
  else
  {
    v13 = n;
    lo = n - pv;
    if ( n - pv < 0 )
      lo = 0;
  }
  i = 0;
  while ( p )
  {
    if ( !strcmp("send", p->msg) || !strcmp("recv", p->msg) )
      v4 = (DECI2_HDR *)((char *)&p[1] + p->nlen);
    else
      v4 = 0;
    dh_1 = v4;
    if ( (v19 < 0 || p->id == v19)
      && (!msg || !strcmp(msg, p->msg))
      && (proto < 0 || dh_1 && proto == dh_1->protocol)
      && (src < 0 || dh_1 && src == dh_1->source)
      && (dst < 0 || dh_1 && dst == dh_1->destination) )
    {
      if ( i >= lo && v13 > i )
        last = print_log(p, last, f_l, f_n);
      ++i;
    }
    p = p->forw;
  }
  if ( f_r )
  {
    for ( p = log_data_list.head; p; p = q )
    {
      q = p->forw;
      ds_free_mem_low(p, "log.c", "_ds_log_cmd");
    }
    log_data_list.tail = 0;
    log_data_list.head = 0;
    ds_log_cur_size = 0;
  }
  return 0;
}

