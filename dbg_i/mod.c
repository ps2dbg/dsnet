#include "dsidb_prototypes.h"

static struct {IMOD *head;IMOD *tail;} imod_list = { NULL, NULL };
static IMOD *last_imod = NULL;
static unsigned int *mblks = NULL;
static unsigned int *mblke = NULL;

static int __cdecl mod_id_by_name_internal(int id, char *name, int ver);
static IMOD *__cdecl look_imod(unsigned int id);
static IMOD *__cdecl alloc_imod(int id, int ac, char **av);
static IMOD *__cdecl free_imod(IMOD *p);
static void __cdecl set_modname(IMOD *p, char *name);
static void __cdecl load_mod_symbols(IMOD *p);
static void __cdecl print_id(char *str, IMOD *p);
static void __cdecl print_addr(char *str, IMOD *p);
static void __cdecl print_size_0(char *str, IMOD *p);
static void __cdecl print_last_addr(char *str, IMOD *p);
static void __cdecl print_vers(char *str, IMOD *p);
static void __cdecl print_name(char *str, IMOD *p);
static void __cdecl print_flags(char *str, IMOD *p);
static void __cdecl print_entry(char *str, IMOD *p);
static void __cdecl print_gp(char *str, IMOD *p);
static void __cdecl print_tsize(char *str, IMOD *p);
static void __cdecl print_dsize(char *str, IMOD *p);
static void __cdecl print_bsize(char *str, IMOD *p);
static void __cdecl print_args(IMOD *p);
static void __cdecl print_status(char *fmt, IMOD *p);
static void __cdecl print_retval(char *fmt, IMOD *p);

static int __cdecl mod_id_by_name_internal(int id, char *name, int ver)
{
  IMOD *p; // [esp+4h] [ebp-8h]

  for ( p = imod_list.head; ; p = p->forw )
  {
    if ( !p )
      return 0;
    if ( (p->flags & 3) == 3 && !strcmp(name, p->name) && p->info.version == (_WORD)ver )
      break;
  }
  if ( !id || p->id == id )
    return p->id;
  else
    return ds_error("module ID mismatch (0x%x->0x%x)", id, p->id);
}

int __cdecl mod_id_by_name(int id, char *name, int ver)
{
  int r; // [esp+4h] [ebp-Ch]
  int r_1; // [esp+4h] [ebp-Ch]
  IMOD *p; // [esp+8h] [ebp-8h]

  r = mod_id_by_name_internal(id, name, (unsigned __int16)ver);
  if ( r )
    return r;
  if ( send_iload_and_wait(4, 0, 0, 0, 0, 0) )
    return -1;
  for ( p = imod_list.head; p; p = p->forw )
  {
    if ( (p->flags & 3) == 1 && send_iload_and_wait(6, 0, p->id, 0, 0, 0) )
      return -1;
  }
  r_1 = mod_id_by_name_internal(id, name, (unsigned __int16)ver);
  if ( r_1 )
    return r_1;
  ds_printf("module not found (name=\"%s\", ver=%d.%d)\n", name, BYTE1(ver), (unsigned __int8)ver);
  return 0;
}

int __cdecl mod_fetch_id(int id)
{
  IMOD *p; // [esp+0h] [ebp-4h]
  IMOD *p_1; // [esp+0h] [ebp-4h]

  for ( p = imod_list.head; p; p = p->forw )
  {
    if ( (p->flags & 3) == 3 && p->id == id )
      return 0;
  }
  if ( send_iload_and_wait(6, 0, id, 0, 0, 0) )
    return -1;
  for ( p_1 = imod_list.head; p_1; p_1 = p_1->forw )
  {
    if ( (p_1->flags & 3) == 3 && p_1->id == id )
      return 0;
  }
  return ds_error("module id (0x%x) not found", 0);
}

unsigned int __cdecl mod_address_by_id(int id)
{
  IMOD *p; // [esp+0h] [ebp-4h]

  for ( p = imod_list.head; ; p = p->forw )
  {
    if ( !p )
      return 0;
    if ( (p->flags & 1) != 0 && p->id == id )
      break;
  }
  if ( (p->flags & 2) == 0 )
    return 0;
  if ( (p->flags & 0x100) != 0 && p->vaddr == p->info.mod_addr )
    return 0;
  return p->info.mod_addr;
}

void __cdecl mod_set_vaddr(int id, unsigned int vaddr)
{
  int flags; // ecx
  IMOD *p; // [esp+0h] [ebp-4h]

  for ( p = imod_list.head; p; p = p->forw )
  {
    if ( (p->flags & 1) != 0 && p->id == id )
    {
      if ( (p->flags & 0x100) == 0 )
      {
        flags = p->flags;
        BYTE1(flags) |= 1u;
        p->flags = flags;
        p->vaddr = vaddr;
      }
      return;
    }
  }
}

static IMOD *__cdecl look_imod(unsigned int id)
{
  IMOD *p; // [esp+0h] [ebp-4h]

  for ( p = imod_list.head; p; p = p->forw )
  {
    if ( (p->flags & 1) != 0 && p->id == id )
      return p;
  }
  return 0;
}

static IMOD *__cdecl alloc_imod(int id, int ac, char **av)
{
  IMOD *tail; // edx
  struct imod *back; // ecx
  int i; // [esp+Ch] [ebp-18h]
  int f_dev; // [esp+10h] [ebp-14h]
  char *buf; // [esp+14h] [ebp-10h]
  char *bp; // [esp+18h] [ebp-Ch]
  char *bp_1; // [esp+18h] [ebp-Ch]
  IMOD *q; // [esp+1Ch] [ebp-8h]
  IMOD *p; // [esp+20h] [ebp-4h]

  f_dev = 0;
  p = (IMOD *)ds_alloc_mem_low("mod.c", "alloc_imod", sizeof(IMOD));
  if ( !p )
    return 0;
  ds_bzero(p, sizeof(IMOD));
  if ( ac > 0 )
  {
    buf = ds_decode_args(ac, av);
    if ( !buf )
      return 0;
    for ( bp = *av; *bp; ++bp )
    {
      if ( *bp == 58 )
        ++f_dev;
    }
    bp_1 = p->args;
    if ( !f_dev )
    {
      strcpy(bp_1, "host1:");
      bp_1 = &p->args[6];
    }
    for ( i = 0; ac > i; ++i )
    {
      if ( (unsigned int)(&bp_1[strlen(av[i])] - ((char *)&p->arg_siz + 3)) > 0xA0 )
      {
        ds_free_mem_low(buf, "mod.c", "alloc_imod");
        ds_error("too long argument");
        return (IMOD *)ds_free_mem_low(p, "mod.c", "alloc_imod");
      }
      strcpy(bp_1, av[i]);
      bp_1 += strlen(av[i]) + 1;
    }
    ds_free_mem_low(buf, "mod.c", "alloc_imod");
    p->arg_siz = bp_1 - 64 - (char *)p;
    p->flags |= 8u;
  }
  if ( !id )
    goto LABEL_20;
  for ( q = imod_list.head; q && q->id && q->id <= id; q = q->forw )
    ;
  if ( q )
  {
    back = q->back;
    p->back = back;
    if ( back )
      p->back->forw = p;
    else
      imod_list.head = p;
    p->forw = q;
    q->back = p;
  }
  else
  {
LABEL_20:
    tail = imod_list.tail;
    p->back = imod_list.tail;
    if ( tail )
      p->back->forw = p;
    else
      imod_list.head = p;
    p->forw = 0;
    imod_list.tail = p;
  }
  return p;
}

static IMOD *__cdecl free_imod(IMOD *p)
{
  if ( p )
  {
    if ( p->forw )
      p->forw->back = p->back;
    else
      imod_list.tail = p->back;
    if ( p->back )
      p->back->forw = p->forw;
    else
      imod_list.head = p->forw;
    if ( p->name )
      ds_free_mem_low(p->name, "mod.c", "free_imod");
    ds_free_mem_low(p, "mod.c", "free_imod");
  }
  return 0;
}

void __cdecl clear_mod_list()
{
  IMOD *forw; // [esp+0h] [ebp-8h]
  IMOD *p; // [esp+4h] [ebp-4h]

  for ( p = imod_list.head; p; p = forw )
  {
    forw = p->forw;
    free_imod(p);
  }
  imod_list.tail = 0;
  imod_list.head = 0;
}

static void __cdecl set_modname(IMOD *p, char *name)
{
  size_t v2; // eax
  char *v3; // eax

  if ( p->name )
    ds_free_mem_low(p->name, "mod.c", "set_modname");
  v2 = strlen(name);
  v3 = (char *)ds_alloc_mem_low("mod.c", "set_modname", v2 + 1);
  p->name = v3;
  if ( v3 )
    strcpy(p->name, name);
}

static void __cdecl load_mod_symbols(IMOD *p)
{
  int flags; // ebx
  char *dp; // [esp+4h] [ebp-808h]
  char *sp; // [esp+8h] [ebp-804h]
  char path[1024]; // [esp+Ch] [ebp-800h] BYREF
  char buf[1024]; // [esp+40Ch] [ebp-400h] BYREF

  if ( !ds_strncmp("host1:", p->args, 6) )
  {
    dp = path;
    for ( sp = &p->args[6]; *sp && *sp != 32; ++sp )
      *dp++ = *sp;
    *dp = 0;
    ds_sprintf(buf, "sload -id 0x%x %s", p->id, path);
    ds_cmd_execute(buf, 0);
    flags = p->flags;
    LOBYTE(flags) = flags | 0x80;
    p->flags = flags;
  }
}

int __cdecl iload_callback(unsigned int id, int cmd, void *ptr, int len)
{
  unsigned int *ids; // [esp+Ch] [ebp-10h]
  IMOD *p; // [esp+14h] [ebp-8h]
  IMOD *p_1; // [esp+14h] [ebp-8h]
  IMOD *p_2; // [esp+14h] [ebp-8h]
  IMOD *p_3; // [esp+14h] [ebp-8h]

  switch ( (char)cmd )
  {
    case 1:
      if ( id && last_imod )
      {
        last_imod->id = id;
        last_imod->flags |= 1u;
      }
      goto LABEL_5;
    case 5:
    case 65:
      for ( ids = (unsigned int *)ptr; (char *)ptr + 4 * ((unsigned int)len >> 2) > (char *)ids; ++ids )
      {
        if ( *ids != -1 && *ids )
        {
          p_1 = look_imod(*ids);
          if ( p_1 )
          {
            p_1->flags |= 0x200u;
          }
          else
          {
            p_2 = alloc_imod(*ids, 0, 0);
            if ( p_2 )
            {
              p_2->id = *ids;
              p_2->flags |= 0x221u;
            }
          }
        }
      }
      return 0;
    case 7:
    case 67:
LABEL_5:
      if ( id )
      {
        p = look_imod(id);
        if ( p )
        {
          if ( (unsigned int)len > 0x20 )
          {
            memcpy(&p->info, ptr, sizeof(p->info));
            set_modname(p, (char *)ptr + 4 * *((unsigned __int8 *)ptr + 28) + 32);
            p->flags |= 2u;
            if ( SLOBYTE(p->flags) >= 0 )
              load_mod_symbols(p);
          }
        }
      }
      return 0;
    case 11:
    case 69:
      mblke = (unsigned int *)ds_free_mem_low(mblks, "mod.c", "iload_callback");
      mblks = mblke;
      mblks = (unsigned int *)ds_alloc_mem_low("mod.c", "iload_callback", len);
      if ( mblks )
      {
        memcpy(mblks, ptr, len);
        mblke = &mblks[(unsigned int)len >> 2];
      }
      return 0;
    case 16:
      if ( !id )
        return 0;
      p_3 = look_imod(id);
      if ( !p_3 )
        return 0;
      p_3->status = *(_BYTE *)ptr;
      p_3->retval = *((_DWORD *)ptr + 1);
      p_3->flags |= 4u;
      if ( p_3->status == 3 )
      {
        clear_symbol_with_id(p_3->id);
        clear_mdebug_with_id(p_3->id);
        p_3->flags &= ~1u;
        clear_module_symbol_with_name(p_3->name);
      }
      if ( (p_3->flags & 0x40) == 0 )
        return 0;
      if ( p_3->status == 3 || p_3->status == 4 )
        return 37;
      return 0;
    default:
      return 0;
  }
}

int __cdecl mload_cmd(int ac, char **av)
{
  int flags; // ecx
  int v4; // ecx
  int r; // [esp+4h] [ebp-8h]
  IMOD *p; // [esp+8h] [ebp-4h]
  int aca; // [esp+14h] [ebp+8h]
  char **ava; // [esp+18h] [ebp+Ch]

  if ( ac <= 0 )
    return 0;
  ava = av + 1;
  aca = ac - 1;
  if ( aca <= 0 || **ava == 45 )
    return ds_error("Usage: mload <fname> [<args>]...");
  p = alloc_imod(0, aca, ava);
  if ( !p )
    return -1;
  flags = p->flags;
  LOBYTE(flags) = flags | 0x10;
  p->flags = flags;
  last_imod = p;
  r = send_iload_and_wait(0, 5, 0, p->args, p->arg_siz, 0);
  p->status = 1;
  v4 = p->flags;
  LOBYTE(v4) = v4 | 4;
  p->flags = v4;
  if ( (p->flags & 1) == 0 )
    last_imod = free_imod(p);
  return r;
}

int __cdecl mstart_cmd(int ac, char **av)
{
  int flags; // ecx
  int v4; // ecx
  int v5; // ecx
  int v6; // ecx
  int status; // eax
  unsigned int id; // [esp+0h] [ebp-18h]
  int f_debug; // [esp+4h] [ebp-14h]
  int action; // [esp+8h] [ebp-10h]
  int r; // [esp+Ch] [ebp-Ch]
  int r_1; // [esp+Ch] [ebp-Ch]
  IMOD *q; // [esp+10h] [ebp-8h]
  IMOD *p; // [esp+14h] [ebp-4h]
  int aca; // [esp+20h] [ebp+8h]
  char **ava; // [esp+24h] [ebp+Ch]

  LOBYTE(action) = 14;
  f_debug = 0;
  id = 0;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  for ( ava = av + 1; aca > 0 && **ava == 45; ++ava )
  {
    if ( strcmp("-d", *ava) )
      return ds_error("Usage: mstart [-d] [<args>]...");
    ++f_debug;
    --aca;
  }
  if ( aca <= 0 )
  {
    p = last_imod;
    if ( !last_imod )
      return ds_error("no module (mload)");
    id = last_imod->id;
  }
  else
  {
    p = alloc_imod(0, aca, ava);
    if ( !p )
      return -1;
    flags = p->flags;
    LOBYTE(flags) = flags | 0x10;
    p->flags = flags;
    LOBYTE(action) = 15;
    last_imod = p;
  }
  if ( f_debug && store_user_breakpoints() < 0 )
  {
    free_imod(p);
    return -1;
  }
  else
  {
    r = send_iload_and_wait(0, (unsigned __int8)action, id, p->args, p->arg_siz, 0);
    last_imod = 0;
    if ( r )
    {
      for ( q = imod_list.head; q; q = q->forw )
      {
        if ( q == p )
        {
          free_imod(p);
          break;
        }
      }
    }
    else
    {
      p->status = 2;
      v4 = p->flags;
      LOBYTE(v4) = v4 | 4;
      p->flags = v4;
    }
    if ( f_debug )
    {
      if ( r )
      {
        restore_user_breakpoints();
        return r;
      }
      else
      {
        v5 = p->flags;
        LOBYTE(v5) = v5 | 0x40;
        p->flags = v5;
        r_1 = wait_halt();
        v6 = p->flags;
        LOBYTE(v6) = v6 & 0xBF;
        p->flags = v6;
        restore_user_breakpoints();
        display_current_informations(r_1);
        if ( r_1 == 37 )
        {
          if ( (p->flags & 4) != 0 )
          {
            r_1 = p->retval;
            ds_printf("***");
            status = p->status;
            if ( status == 3 )
            {
              ds_printf(" Removed");
            }
            else if ( status == 4 )
            {
              ds_printf(" Resident");
            }
            ds_printf(" retval=0x%W\n", r_1);
            if ( r_1 < 0 || r_1 > 127 )
              return 255;
          }
          else
          {
            return 255;
          }
        }
        else if ( r_1 )
        {
          r_1 += 160;
          if ( r_1 <= 159 || r_1 > 223 )
            return 255;
        }
        return r_1;
      }
    }
    else
    {
      return r;
    }
  }
}

int __cdecl mremove_cmd(int ac, char **av)
{
  unsigned int pv; // [esp+0h] [ebp-Ch] BYREF
  IMOD *q; // [esp+4h] [ebp-8h]
  IMOD *p; // [esp+8h] [ebp-4h]
  int aca; // [esp+14h] [ebp+8h]
  char **ava; // [esp+18h] [ebp+Ch]

  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  ava = av + 1;
  while ( aca > 0 )
  {
    if ( **ava == 45 )
      return ds_error("Usage: mremove [all] <id>...");
    if ( !strcmp(*ava, "all") )
    {
      for ( p = imod_list.head; p; p = q )
      {
        q = p->forw;
        free_imod(p);
      }
      return 0;
    }
    if ( ds_eval_word(*ava, &pv) )
      return -1;
    p = look_imod(pv);
    if ( !p )
      return ds_error("id=0x%x not found", pv);
    free_imod(p);
    --aca;
    ++ava;
  }
  return 0;
}

static void __cdecl print_id(char *str, IMOD *p)
{
  if ( (p->flags & 1) != 0 )
    ds_printf("%s%2x", str, p->id);
  else
    ds_printf("%s--", str);
}

static void __cdecl print_addr(char *str, IMOD *p)
{
  if ( (p->flags & 2) != 0 )
    ds_printf("%s%6x", str, p->info.mod_addr);
  else
    ds_printf("%s------", str);
}

static void __cdecl print_size_0(char *str, IMOD *p)
{
  if ( (p->flags & 2) != 0 )
    ds_printf("%s%5x", str, p->info.bss_size + p->info.data_size + p->info.text_size);
  else
    ds_printf("%s-----", str);
}

static void __cdecl print_last_addr(char *str, IMOD *p)
{
  if ( (p->flags & 2) != 0 )
    ds_printf("%s%6x", str, p->info.mod_addr + p->info.bss_size + p->info.data_size + p->info.text_size - 1);
  else
    ds_printf("%s------", str);
}

static void __cdecl print_vers(char *str, IMOD *p)
{
  if ( (p->flags & 2) != 0 )
    ds_printf("%s%2d.%-2d", str, HIBYTE(p->info.version), (unsigned __int8)p->info.version);
  else
    ds_printf("%s--.--", str);
}

static void __cdecl print_name(char *str, IMOD *p)
{
  if ( (p->flags & 2) != 0 )
    ds_printf("%s%S", str, p->name);
  else
    ds_printf("%s", str);
}

static void __cdecl print_flags(char *str, IMOD *p)
{
  const char *v2; // eax
  const char *v3; // eax
  const char *v4; // eax

  if ( (p->flags & 2) != 0 )
    ds_printf("%s%x", str, p->info.flags);
  else
    ds_printf("%s--", str);
  if ( (p->flags & 2) != 0 )
  {
    ds_printf(" (");
    switch ( p->info.flags & 0xF )
    {
      case 1:
        ds_printf(" Loaded");
        break;
      case 2:
        ds_printf(" Starting");
        break;
      case 3:
        ds_printf(" Resident");
        break;
      case 4:
        ds_printf(" Stopping");
        break;
      case 5:
        ds_printf(" Self-Stopping");
        break;
      case 6:
        ds_printf(" Stopped");
        break;
      case 7:
        ds_printf(" Self-Stopped");
        break;
      default:
        ds_printf(" ???");
        break;
    }
    if ( (p->info.flags & 0x10) != 0 )
      v2 = " Removable";
    else
      v2 = "";
    ds_printf("%s", v2);
    if ( (p->info.flags & 0x20) != 0 )
      v3 = " No-Freemem-by-Unload";
    else
      v3 = "";
    ds_printf("%s", v3);
    if ( (p->info.flags & 0x40) != 0 )
      v4 = " Clearmem-by-Unload";
    else
      v4 = "";
    ds_printf("%s", v4);
    ds_printf(" )");
  }
}

static void __cdecl print_entry(char *str, IMOD *p)
{
  if ( (p->flags & 2) != 0 )
  {
    if ( p->info.entry_address )
      ds_printf("%s%x", str, p->info.entry_address);
  }
}

static void __cdecl print_gp(char *str, IMOD *p)
{
  if ( (p->flags & 2) != 0 )
  {
    if ( p->info.gp_value )
      ds_printf("%s%x", str, p->info.gp_value);
  }
}

static void __cdecl print_tsize(char *str, IMOD *p)
{
  if ( (p->flags & 2) != 0 )
    ds_printf("%s%5x", str, p->info.text_size);
  else
    ds_printf("%s-----", str);
}

static void __cdecl print_dsize(char *str, IMOD *p)
{
  if ( (p->flags & 2) != 0 )
    ds_printf("%s%5x", str, p->info.data_size);
  else
    ds_printf("%s-----", str);
}

static void __cdecl print_bsize(char *str, IMOD *p)
{
  if ( (p->flags & 2) != 0 )
    ds_printf("%s%5x", str, p->info.bss_size);
  else
    ds_printf("%s-----", str);
}

static void __cdecl print_args(IMOD *p)
{
  size_t v1; // ecx
  int na; // [esp+0h] [ebp-Ch]
  char *pa; // [esp+8h] [ebp-4h]

  pa = p->args;
  na = p->arg_siz;
  if ( (p->flags & 8) != 0 )
  {
    do
    {
      ds_printf(" ");
      if ( !*pa )
        break;
      ds_printf("%S", (const wchar_t *)pa);
      v1 = strlen(pa) + 1;
      pa += v1;
      na -= v1;
    }
    while ( na >= 0 );
  }
}

static void __cdecl print_status(char *fmt, IMOD *p)
{
  int status; // eax

  if ( (p->flags & 4) != 0 )
  {
    status = p->status;
    if ( status == 2 )
    {
      ds_printf(fmt, "Running");
    }
    else if ( p->status > 2u )
    {
      if ( status == 3 )
      {
        ds_printf(fmt, "Removed");
      }
      else if ( status == 4 )
      {
        ds_printf(fmt, "Resident");
      }
    }
    else if ( status == 1 )
    {
      ds_printf(fmt, "Loaded");
    }
  }
}

static void __cdecl print_retval(char *fmt, IMOD *p)
{
  if ( (p->flags & 4) != 0 && p->status <= 4u && p->status >= 3u )
    ds_printf(fmt, p->retval);
}

int __cdecl mlist_cmd(int ac, char **av)
{
  int flags; // ecx
  int f_long; // [esp+0h] [ebp-14h]
  int f_mem; // [esp+4h] [ebp-10h]
  int r; // [esp+8h] [ebp-Ch]
  IMOD *q; // [esp+Ch] [ebp-8h]
  IMOD *p; // [esp+10h] [ebp-4h]
  IMOD *p_1; // [esp+10h] [ebp-4h]
  IMOD *p_2; // [esp+10h] [ebp-4h]
  IMOD *p_3; // [esp+10h] [ebp-4h]
  int aca; // [esp+1Ch] [ebp+8h]
  char **ava; // [esp+20h] [ebp+Ch]

  f_mem = 1;
  f_long = 0;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  ava = av + 1;
  while ( aca > 0 )
  {
    if ( strcmp("-m", *ava) )
    {
      if ( !strcmp("-i", *ava) )
      {
        f_mem = 0;
      }
      else
      {
        if ( strcmp("-l", *ava) )
          return ds_error("Usage: mlist [-m] [-i] [-l]");
        ++f_long;
      }
    }
    --aca;
    ++ava;
  }
  for ( p = imod_list.head; p; p = q )
  {
    q = p->forw;
    if ( (p->flags & 0x20) != 0 )
    {
      free_imod(p);
    }
    else
    {
      flags = p->flags;
      BYTE1(flags) &= ~2u;
      p->flags = flags;
    }
  }
  if ( aca )
    return ds_error("Usage: mlist [-m] [-i] [-l]");
  if ( f_mem && send_iload_and_wait(4, 0, 0, 0, 0, 0) )
    return -1;
  for ( p_1 = imod_list.head; p_1; p_1 = p_1->forw )
  {
    if ( (p_1->flags & 3) == 1 )
    {
      r = send_iload_and_wait(6, 0, p_1->id, 0, 0, 4);
      if ( r )
      {
        if ( r != 4 )
          return -1;
      }
    }
  }
  if ( f_mem )
  {
    clear_module_symbol();
    ds_printf(" Id  Begin    End  Size (Text  Data   Bss) Ver  Name\n");
    for ( p_2 = imod_list.head; p_2; p_2 = p_2->forw )
    {
      if ( (p_2->flags & 0x200) != 0 )
      {
        print_id(" ", p_2);
        print_addr(" ", p_2);
        print_last_addr("-", p_2);
        print_size_0(" ", p_2);
        print_tsize(" ", p_2);
        print_dsize(" ", p_2);
        print_bsize(" ", p_2);
        print_vers(" ", p_2);
        print_name(" ", p_2);
        ds_printf("\n");
        if ( f_long )
        {
          ds_printf("    ");
          print_flags(" Status=", p_2);
          print_entry(" Entry=", p_2);
          print_gp(" GP=", p_2);
          print_status(" ST=%s", p_2);
          print_retval(" RV=%W", p_2);
          print_args(p_2);
          ds_printf("\n");
        }
        if ( (p_2->flags & 2) != 0 )
          add_module_symbol(
            p_2->name,
            p_2->info.mod_addr,
            p_2->info.bss_size + p_2->info.data_size + p_2->info.text_size,
            p_2->id);
      }
    }
  }
  else
  {
    ds_printf(" Id Status   Retval   Fname [Args]...\n");
    for ( p_3 = imod_list.head; p_3; p_3 = p_3->forw )
    {
      if ( (p_3->flags & 0x20) == 0 )
      {
        print_id(" ", p_3);
        print_status(" %-8s", p_3);
        if ( p_3->status == 3 || p_3->status == 4 )
          ds_printf(" %W", p_3->retval);
        else
          ds_printf(" %8s", "");
        print_args(p_3);
        ds_printf("\n");
        if ( f_long )
        {
          ds_printf("   ");
          print_flags(" flags=", p_3);
          ds_printf(" vers=%d.%d", HIBYTE(p_3->info.version), (unsigned __int8)p_3->info.version);
          ds_printf(" mod_addr=%W", p_3->info.mod_addr);
          print_tsize(" T=", p_3);
          print_dsize(" D=", p_3);
          print_bsize(" B=", p_3);
          print_name(" ", p_3);
          ds_printf("\n");
        }
      }
    }
  }
  return 0;
}

int __cdecl memlist_cmd(int ac, char **av)
{
  int flags; // ebx
  unsigned int v4; // edx
  unsigned int v5; // edx
  unsigned int v6; // eax
  unsigned int v7; // ecx
  unsigned int v8; // edx
  unsigned int v9; // edx
  unsigned int v10; // eax
  unsigned int v11; // edx
  unsigned int v12; // ecx
  unsigned int v13; // ecx
  unsigned int mod_addr; // eax
  unsigned int v15; // ecx
  unsigned int v16; // eax
  unsigned int v17; // ecx
  unsigned int v18; // [esp-8h] [ebp-30h]
  unsigned int range; // [esp+4h] [ebp-24h] BYREF
  unsigned int adr; // [esp+8h] [ebp-20h] BYREF
  unsigned int *i; // [esp+Ch] [ebp-1Ch]
  int f_adr; // [esp+10h] [ebp-18h]
  int f_free; // [esp+14h] [ebp-14h]
  int f_sup; // [esp+18h] [ebp-10h]
  int r; // [esp+1Ch] [ebp-Ch]
  IMOD *q; // [esp+20h] [ebp-8h]
  IMOD *p; // [esp+24h] [ebp-4h]
  int aca; // [esp+30h] [ebp+8h]
  char **ava; // [esp+34h] [ebp+Ch]

  f_sup = 0;
  f_free = 0;
  f_adr = 0;
  adr = 0;
  range = 1;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  ava = av + 1;
  while ( aca > 0 )
  {
    if ( !strcmp("-s", *ava) )
    {
      f_sup = 1;
    }
    else if ( !strcmp("-f", *ava) )
    {
      f_free = 1;
    }
    else if ( !strcmp("-a", *ava) )
    {
      ++ava;
      if ( --aca <= 0 || ds_eval_word(*ava, &adr) )
        return ds_error("Usage: memlist [-s] [-f] [-a <adr>] [-r <range>]");
      f_adr = 1;
    }
    else
    {
      if ( strcmp("-r", *ava) )
        return ds_error("Usage: memlist [-s] [-f] [-a <adr>] [-r <range>]");
      ++ava;
      if ( --aca <= 0 || ds_eval_word(*ava, &range) )
        return ds_error("Usage: memlist [-s] [-f] [-a <adr>] [-r <range>]");
    }
    --aca;
    ++ava;
  }
  if ( !f_sup )
  {
    for ( p = imod_list.head; p; p = q )
    {
      q = p->forw;
      if ( (p->flags & 0x20) != 0 )
      {
        free_imod(p);
      }
      else
      {
        flags = p->flags;
        BYTE1(flags) &= ~2u;
        p->flags = flags;
      }
    }
    if ( send_iload_and_wait(4, 0, 0, 0, 0, 0) )
      return -1;
    for ( p = imod_list.head; p; p = p->forw )
    {
      if ( (p->flags & 3) == 1 )
      {
        r = send_iload_and_wait(6, 0, p->id, 0, 0, 4);
        if ( r )
        {
          if ( r != 4 )
            return -1;
        }
      }
    }
  }
LABEL_35:
  if ( send_iload_and_wait(10, 0, 0, 0, 0, 0) )
    return -1;
  for ( i = mblks; mblke > i; ++i )
  {
    if ( *i == -1 )
      goto LABEL_35;
  }
  for ( i = mblks; mblke > i && (*i & 0xFE) != 2; ++i )
  {
    if ( !f_free || (*i & 1) != 0 )
    {
      if ( !f_adr || (v4 = i[1], LOBYTE(v4) = 0, adr < v4) && (v5 = *i, LOBYTE(v5) = 0, v5 < range + adr) )
      {
        v6 = i[1];
        LOBYTE(v6) = 0;
        v7 = *i;
        LOBYTE(v7) = 0;
        v8 = i[1];
        LOBYTE(v8) = 0;
        v18 = v8 - 1;
        v9 = *i;
        LOBYTE(v9) = 0;
        ds_printf(" %06x-%06x %06x", v9, v18, v6 - v7);
        if ( (*i & 1) != 0 )
        {
          ds_printf(" <free>\n");
          continue;
        }
        v10 = *i & 0xFE;
        if ( v10 == 4 )
        {
          ds_printf(" <system memory manager work>\n");
          continue;
        }
        if ( v10 <= 4 )
        {
          if ( v10 )
          {
LABEL_76:
            if ( f_sup )
            {
              ds_printf(" <module body>");
            }
            else
            {
              for ( p = imod_list.head; p; p = p->forw )
              {
                if ( p->status != 3 && (p->flags & 2) != 0 )
                {
                  mod_addr = p->info.mod_addr;
                  LOBYTE(mod_addr) = 0;
                  v15 = *i;
                  LOBYTE(v15) = 0;
                  if ( mod_addr == v15 )
                  {
                    v16 = p->info.mod_addr + p->info.bss_size + p->info.data_size + p->info.text_size + 255;
                    LOBYTE(v16) = 0;
                    v17 = i[1];
                    LOBYTE(v17) = 0;
                    if ( v16 == v17 )
                    {
                      if ( *p->name )
                        ds_printf("     %S", (const wchar_t *)p->name);
                      else
                        ds_printf(" <unnamed module body>");
                    }
                  }
                }
              }
            }
          }
          ds_printf("\n");
          continue;
        }
        if ( v10 == 6 || v10 != 8 )
          goto LABEL_76;
        ds_printf(" <allocated for module load>\n");
        if ( !f_sup )
        {
          for ( p = imod_list.head; p; p = p->forw )
          {
            if ( p->status != 3 && (p->flags & 2) != 0 )
            {
              v11 = i[1];
              LOBYTE(v11) = 0;
              if ( p->info.mod_addr < v11 )
              {
                v12 = *i;
                LOBYTE(v12) = 0;
                if ( p->info.mod_addr + p->info.bss_size + p->info.data_size + p->info.text_size >= v12 )
                {
                  v13 = *i;
                  LOBYTE(v13) = 0;
                  ds_printf(
                    " %*s+%06x %06x",
                    6,
                    "",
                    p->info.mod_addr - v13,
                    p->info.bss_size + p->info.data_size + p->info.text_size);
                  if ( *p->name )
                    ds_printf("     %S", (const wchar_t *)p->name);
                  else
                    ds_printf(" <unnamed module body>");
                  ds_printf("\n");
                }
              }
            }
          }
        }
      }
    }
  }
  mblke = (unsigned int *)ds_free_mem_low(mblks, "mod.c", "memlist_cmd");
  mblks = mblke;
  return 0;
}

