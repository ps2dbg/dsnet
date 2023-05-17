#include "dsnet_prototypes.h"

static MEM_LIST mems;

void __cdecl ds_status_mem()
{
  struct mem *back; // ecx
  int hsiz; // [esp+Ch] [ebp-28h]
  int lsiz; // [esp+10h] [ebp-24h]
  int esiz; // [esp+14h] [ebp-20h]
  int tsiz; // [esp+18h] [ebp-1Ch]
  int n; // [esp+1Ch] [ebp-18h]
  int n_1; // [esp+1Ch] [ebp-18h]
  MEM *q; // [esp+20h] [ebp-14h]
  MEM *q_1; // [esp+20h] [ebp-14h]
  MEM *pf; // [esp+24h] [ebp-10h]
  MEM *p; // [esp+28h] [ebp-Ch]
  MEM *p_1; // [esp+28h] [ebp-Ch]
  MEM_LIST ml; // [esp+2Ch] [ebp-8h]

  ml.tail = 0;
  ml.head = 0;
  for ( p = mems.head; p; p = pf )
  {
    pf = p->forw;
    for ( q = ml.head; q; q = q->forw )
    {
      n = strcmp(p->file, q->file);
      if ( n <= 0 && (n || strcmp(p->func, q->func) < 0) )
      {
        back = q->back;
        p->back = back;
        if ( back )
          p->back->forw = p;
        else
          ml.head = p;
        p->forw = q;
        q->back = p;
        break;
      }
    }
    if ( !q )
    {
      p->back = ml.tail;
      if ( ml.tail )
        p->back->forw = p;
      else
        ml.head = p;
      p->forw = 0;
      ml.tail = p;
    }
  }
  mems = ml;
  ds_printf("memory status:\n");
  ds_printf("  %10s %-28s %8s %8s %8s %8s\n", "filename", "function", "min", "max", "total", "count");
  tsiz = 0;
  for ( p_1 = mems.head; p_1; p_1 = q_1 )
  {
    hsiz = 0;
    lsiz = -1;
    n_1 = 0;
    esiz = 0;
    for ( q_1 = p_1; q_1 && !strcmp(p_1->file, q_1->file) && !strcmp(p_1->func, q_1->func); q_1 = q_1->forw )
    {
      if ( lsiz < 0 || lsiz > q_1->size )
        lsiz = q_1->size;
      if ( q_1->size > hsiz )
        hsiz = q_1->size;
      ++n_1;
      esiz += q_1->size;
      tsiz += q_1->size;
    }
    ds_printf("  %10s %-28s %8d %8d %8d %8d\n", p_1->file, p_1->func, lsiz, hsiz, esiz, n_1);
  }
  ds_printf("  total %d bytes\n", tsiz);
}

void *__cdecl ds_alloc_mem_low(char *file, char *func, int size)
{
  MEM *tail; // edx
  MEM *p; // [esp+4h] [ebp-4h]

  if ( size >= 0 )
  {
    p = (MEM *)malloc(size + sizeof(MEM));
    if ( p )
    {
      p->file = file;
      p->func = func;
      p->size = size;
      p->magic = 1296395586;
      tail = mems.tail;
      p->back = mems.tail;
      if ( tail )
        p->back->forw = p;
      else
        mems.head = p;
      p->forw = 0;
      mems.tail = p;
      return &p[1];
    }
    else
    {
      ds_error("!malloc");
      return 0;
    }
  }
  else
  {
    ds_error("malloc - no space");
    return 0;
  }
}

void *__cdecl ds_free_mem_low(void *ptr, char *file, char *func)
{
  MEM *p; // [esp+8h] [ebp-4h]

  if ( ptr )
  {
    p = (MEM *)((char *)ptr - sizeof(MEM));
    if ( *((_DWORD *)ptr - 1) != 1296395586 )
    {
      ds_printf("ds_free_mem: bad free from %s:%s()\n", file, func);
      return 0;
    }
    p->magic = 1296395590;
    if ( p->forw )
      p->forw->back = p->back;
    else
      mems.tail = p->back;
    if ( p->back )
      p->back->forw = p->forw;
    else
      mems.head = p->forw;
    free(p);
  }
  return 0;
}

