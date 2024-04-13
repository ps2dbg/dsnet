
#include "dsxdb_prototypes.h"

extern unsigned int dot; // defined in mem.c
static struct {SYMS *head;SYMS *tail;} syms_list = { NULL, NULL };
static struct {MOD_SYMS *head;MOD_SYMS *tail;} mod_syms_list = { NULL, NULL };

static struct {SYMLINE *head;SYMLINE *tail;} symlines;

static int __cdecl add_symline(unsigned int value, char *bp);
static void __cdecl show_and_free_symline();
static int __cdecl is_dup_sym(char *bp, char *str);
static int __cdecl is_gcc_label(char *str);

void __cdecl clear_module_symbol()
{
  MOD_SYMS *q; // [esp+0h] [ebp-8h]
  MOD_SYMS *p; // [esp+4h] [ebp-4h]

  for ( p = mod_syms_list.head; p; p = q )
  {
    q = p->forw;
    ds_free_mem_low(p, "sym.c", "clear_module_symbol");
  }
  mod_syms_list.tail = 0;
  mod_syms_list.head = 0;
}

void __cdecl clear_module_symbol_with_name(char *name)
{
  MOD_SYMS *q; // [esp+8h] [ebp-8h]
  MOD_SYMS *p; // [esp+Ch] [ebp-4h]

  for ( p = mod_syms_list.head; p; p = q )
  {
    q = p->forw;
    if ( !strcmp(name, p->name) )
    {
      if ( p->forw )
        p->forw->back = p->back;
      else
        mod_syms_list.tail = p->back;
      if ( p->back )
        p->back->forw = p->forw;
      else
        mod_syms_list.head = p->forw;
      ds_free_mem_low(p, "sym.c", "clear_module_symbol_with_name");
    }
  }
}

void __cdecl add_module_symbol(char *name, int adr, int siz, int id)
{
  size_t v4; // eax
  MOD_SYMS *tail; // edx
  MOD_SYMS *p; // [esp+8h] [ebp-4h]

  v4 = strlen(name);
  p = (MOD_SYMS *)ds_alloc_mem_low("sym.c", "add_module_symbol", v4 + sizeof(MOD_SYMS) + 1);
  if ( p )
  {
    strcpy(p->name, name);
    p->adr = adr;
    p->siz = siz;
    p->id = id;
    tail = mod_syms_list.tail;
    p->back = mod_syms_list.tail;
    if ( tail )
      p->back->forw = p;
    else
      mod_syms_list.head = p;
    p->forw = 0;
    mod_syms_list.tail = p;
  }
}

void __cdecl clear_symbol()
{
  SYMS *q; // [esp+0h] [ebp-8h]
  SYMS *p; // [esp+4h] [ebp-4h]

  for ( p = syms_list.head; p; p = q )
  {
    q = p->forw;
    ds_free_mem_low(p->symtab, "sym.c", "clear_symbol");
    ds_free_mem_low(p->shstrtab, "sym.c", "clear_symbol");
    ds_free_mem_low(p->strtab, "sym.c", "clear_symbol");
    ds_free_mem_low(p->shdr, "sym.c", "clear_symbol");
    ds_free_mem_low(p, "sym.c", "clear_symbol");
  }
  syms_list.tail = 0;
  syms_list.head = 0;
  clear_module_symbol();
}

void __cdecl clear_symbol_with_id(int id)
{
  SYMS *q; // [esp+8h] [ebp-8h]
  SYMS *p; // [esp+Ch] [ebp-4h]

  for ( p = syms_list.head; p; p = q )
  {
    q = p->forw;
    if ( p->id == id )
    {
      if ( p->forw )
        p->forw->back = p->back;
      else
        syms_list.tail = p->back;
      if ( p->back )
        p->back->forw = p->forw;
      else
        syms_list.head = p->forw;
      ds_free_mem_low(p->symtab, "sym.c", "clear_symbol_with_id");
      ds_free_mem_low(p->shstrtab, "sym.c", "clear_symbol_with_id");
      ds_free_mem_low(p->strtab, "sym.c", "clear_symbol_with_id");
      ds_free_mem_low(p->shdr, "sym.c", "clear_symbol_with_id");
      ds_free_mem_low(p, "sym.c", "clear_symbol_with_id");
    }
  }
}

int __cdecl match_symbol(char *str, int nstr, char *name)
{
  if ( !str )
    return 1;
  while ( *name )
  {
    if ( !ds_strncmp(str, name, nstr) )
      return 1;
    ++name;
  }
  return 0;
}

static int __cdecl add_symline(unsigned int value, char *bp)
{
  size_t v2; // eax
  struct symline *back; // ecx
  SYMLINE *tail; // edx
  int n; // [esp+Ch] [ebp-Ch]
  SYMLINE *q; // [esp+10h] [ebp-8h]
  SYMLINE *p; // [esp+14h] [ebp-4h]

  v2 = strlen(bp);
  n = v2 + 1;
  p = (SYMLINE *)ds_alloc_mem_low("sym.c", "add_symline", v2 + sizeof(SYMLINE) + 1);
  if ( !p )
    return -1;
  p->value = value;
  memcpy(&p[1], bp, n);
  for ( q = symlines.head; q; q = q->forw )
  {
    if ( q->value > value )
    {
      back = q->back;
      p->back = back;
      if ( back )
        p->back->forw = p;
      else
        symlines.head = p;
      p->forw = q;
      q->back = p;
      return 0;
    }
  }
  tail = symlines.tail;
  p->back = symlines.tail;
  if ( tail )
    p->back->forw = p;
  else
    symlines.head = p;
  p->forw = 0;
  symlines.tail = p;
  return 0;
}

static void __cdecl show_and_free_symline()
{
  SYMLINE *q; // [esp+0h] [ebp-8h]
  SYMLINE *p; // [esp+4h] [ebp-4h]

  for ( p = symlines.head; p; p = q )
  {
    q = p->forw;
    ds_printf("%s\n", (const char *)&p[1]);
    ds_free_mem_low(p, "sym.c", "show_and_free_symline");
  }
  symlines.tail = 0;
  symlines.head = 0;
}

int __cdecl module_base(int id, int base, int shndx, int info)
{
  if ( base == -1 )
    return 0;
  if ( shndx != 65311 && (shndx <= 0 || shndx > 65279) )
    return 0;
  if ( base )
    return base;
  return mod_address_by_id(id);
}

int __cdecl show_symbol(int ac, char **av)
{
  char *v3; // eax
  int v4; // eax
  int shndx; // eax
  MOD_SYMS *mp; // [esp+8h] [ebp-420h]
  char *bp; // [esp+Ch] [ebp-41Ch]
  char *bp_1; // [esp+Ch] [ebp-41Ch]
  char *bp_2; // [esp+Ch] [ebp-41Ch]
  char *bp_3; // [esp+Ch] [ebp-41Ch]
  char *bp_4; // [esp+Ch] [ebp-41Ch]
  char *bp_5; // [esp+Ch] [ebp-41Ch]
  char *bp_6; // [esp+Ch] [ebp-41Ch]
  char *bp_7; // [esp+Ch] [ebp-41Ch]
  char buf[1024]; // [esp+10h] [ebp-418h] BYREF
  unsigned int value; // [esp+410h] [ebp-18h]
  char *str; // [esp+414h] [ebp-14h]
  int nstr; // [esp+418h] [ebp-10h]
  int i; // [esp+41Ch] [ebp-Ch]
  DS_ELF_SYMTAB *p; // [esp+420h] [ebp-8h]
  SYMS *syms; // [esp+424h] [ebp-4h]
  int aca; // [esp+430h] [ebp+8h]
  char **ava; // [esp+434h] [ebp+Ch]

  str = 0;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  ava = av + 1;
  if ( aca > 0 && **ava == 45 )
    return ds_error("Usage: show symbol [<str>]");
  if ( aca > 0 )
  {
    str = *ava;
    nstr = strlen(str);
    if ( aca > 1 )
      return ds_error("Usage: show symbol [<str>]");
  }
  ds_printf(" Value     Size Section  Bind    Type    Name\n");
  symlines.tail = 0;
  symlines.head = 0;
  syms = syms_list.head;
LABEL_10:
  if ( syms )
  {
    p = syms->symtab;
    i = 0;
    while ( 1 )
    {
      if ( syms->nsymtab <= i )
      {
LABEL_53:
        syms = syms->forw;
        goto LABEL_10;
      }
      if ( i )
      {
        v3 = p->name ? &syms->strtab[p->name] : "";
        if ( match_symbol(str, nstr, v3) )
          break;
      }
LABEL_52:
      ++i;
      ++p;
    }
    value = p->value;
    v4 = module_base(syms->id, syms->base, p->shndx, p->info);
    value += v4;
    bp = &buf[ds_sprintf(buf, " %W +%H", value, p->size)];
    shndx = p->shndx;
    if ( shndx == 65311 )
    {
      bp_1 = &bp[ds_sprintf(bp, " RADDR   ")];
      goto LABEL_33;
    }
    if ( p->shndx > 0xFF1Fu )
    {
      if ( shndx == 65521 )
      {
        bp_1 = &bp[ds_sprintf(bp, " ABS     ")];
        goto LABEL_33;
      }
      if ( shndx == 65522 )
      {
        bp_1 = &bp[ds_sprintf(bp, " COMMON  ")];
        goto LABEL_33;
      }
    }
    else if ( !p->shndx )
    {
      bp_1 = &bp[ds_sprintf(bp, " UNDEF   ")];
      goto LABEL_33;
    }
    if ( p->shndx && syms->shnum > p->shndx )
      bp_1 = &bp[ds_sprintf(bp, " %-8s", &syms->shstrtab[syms->shdr[p->shndx].name])];
    else
      bp_1 = &bp[ds_sprintf(bp, "      %H", p->shndx)];
LABEL_33:
    switch ( p->info >> 4 )
    {
      case 0:
        bp_2 = &bp_1[ds_sprintf(bp_1, " local  ")];
        break;
      case 1:
        bp_2 = &bp_1[ds_sprintf(bp_1, " global ")];
        break;
      case 2:
        bp_2 = &bp_1[ds_sprintf(bp_1, " weak   ")];
        break;
      case 13:
        bp_2 = &bp_1[ds_sprintf(bp_1, " loproc ")];
        break;
      case 15:
        bp_2 = &bp_1[ds_sprintf(bp_1, " hiproc ")];
        break;
      default:
        bp_2 = &bp_1[ds_sprintf(bp_1, " BIND=%d", p->info >> 4)];
        break;
    }
    switch ( p->info & 0xF )
    {
      case 0:
        bp_3 = &bp_2[ds_sprintf(bp_2, " notype ")];
        break;
      case 1:
        bp_3 = &bp_2[ds_sprintf(bp_2, " object ")];
        break;
      case 2:
        bp_3 = &bp_2[ds_sprintf(bp_2, " func   ")];
        break;
      case 3:
        bp_3 = &bp_2[ds_sprintf(bp_2, " section")];
        break;
      case 4:
        bp_3 = &bp_2[ds_sprintf(bp_2, " file   ")];
        break;
      case 0xD:
        bp_3 = &bp_2[ds_sprintf(bp_2, " loproc ")];
        break;
      case 0xE:
        bp_3 = &bp_2[ds_sprintf(bp_2, " hiproc ")];
        break;
      default:
        bp_3 = &bp_2[ds_sprintf(bp_2, " TYPE=%d", p->info >> 4)];
        break;
    }
    if ( p->name )
      ds_sprintf(bp_3, " %s", &syms->strtab[p->name]);
    if ( add_symline(value, buf) < 0 )
      goto LABEL_53;
    goto LABEL_52;
  }
  for ( mp = mod_syms_list.head; mp; mp = mp->forw )
  {
    if ( match_symbol(str, nstr, mp->name) )
    {
      bp_4 = &buf[ds_sprintf(buf, " %W +%H", mp->adr, mp->siz)];
      bp_5 = &bp_4[ds_sprintf(bp_4, " Module  ")];
      bp_6 = &bp_5[ds_sprintf(bp_5, " %-7x", mp->id)];
      bp_7 = &bp_6[ds_sprintf(bp_6, " module ")];
      ds_sprintf(bp_7, " %s", mp->name);
      if ( add_symline(value, buf) < 0 )
        break;
    }
  }
  show_and_free_symline();
  return 0;
}

int __cdecl look_eemod(
        void *stream,
        DS_ELF_EHDR *ehdr,
        DS_ELF_SHDR *shdr,
        int id,
        int base,
        void (__cdecl *clear_func)())
{
  int i; // [esp+4h] [ebp-Ch]
  EEMOD *eemod; // [esp+8h] [ebp-8h]
  DS_ELF_SHDR *sheemod; // [esp+Ch] [ebp-4h]

  sheemod = 0;
  eemod = 0;
  if ( ehdr->type != 0xFF91 )
    return id;
  if ( base )
    return id;
  for ( i = 0; i < ehdr->shnum; ++i )
  {
    if ( shdr[i].type == 1879048336 )
      sheemod = &shdr[i];
  }
  if ( !sheemod || sheemod->size <= 0x2B )
    goto LABEL_16;
  eemod = (EEMOD *)ds_fload(stream, 0, sheemod->offset, sheemod->size, 1);
  if ( !eemod )
    goto LABEL_20;
  if ( eemod->moduleinfo == -1 )
  {
LABEL_16:
    if ( id )
    {
      if ( mod_fetch_id(id) >= 0 )
        goto LABEL_19;
    }
    else
    {
      ds_error("no module name, -b or -id option is needed");
    }
  }
  else
  {
    id = mod_id_by_name(id, eemod->modulename, eemod->moduleversion);
    if ( id > 0 )
    {
      ((void (__cdecl *)(int))clear_func)(id);
LABEL_19:
      ds_free_mem_low(eemod, "sym.c", "look_eemod");
      return id;
    }
  }
LABEL_20:
  ds_free_mem_low(eemod, "sym.c", "look_eemod");
  return -1;
}

int __cdecl look_iopmod(
        void *stream,
        DS_ELF_EHDR *ehdr,
        DS_ELF_SHDR *shdr,
        int id,
        int base,
        void (__cdecl *clear_func)())
{
  int i; // [esp+4h] [ebp-Ch]
  IOPMOD *iopmod; // [esp+8h] [ebp-8h]
  DS_ELF_SHDR *shiopmod; // [esp+Ch] [ebp-4h]

  shiopmod = 0;
  iopmod = 0;
  if ( base )
    return id;
  for ( i = 0; i < ehdr->shnum; ++i )
  {
    if ( shdr[i].type == 1879048320 )
      shiopmod = &shdr[i];
  }
  if ( !shiopmod || shiopmod->size <= 0x1B )
    goto LABEL_14;
  iopmod = (IOPMOD *)ds_fload(stream, 0, shiopmod->offset, shiopmod->size, 1);
  if ( !iopmod )
    goto LABEL_18;
  if ( iopmod->moduleinfo == -1 )
  {
LABEL_14:
    if ( id )
    {
      if ( mod_fetch_id(id) >= 0 )
        goto LABEL_17;
    }
    else
    {
      ds_error("no module name, -b or -id option is needed");
    }
  }
  else
  {
    id = mod_id_by_name(id, iopmod->modulename, iopmod->moduleversion);
    if ( id > 0 )
    {
      ((void (__cdecl *)(int))clear_func)(id);
LABEL_17:
      ds_free_mem_low(iopmod, "sym.c", "look_iopmod");
      return id;
    }
  }
LABEL_18:
  ds_free_mem_low(iopmod, "sym.c", "look_iopmod");
  return -1;
}

int __cdecl load_symbol(void *stream, DS_ELF_EHDR *ehdr, DS_ELF_SHDR *shdr, int symndx, int strndx, int id, int base)
{
  DS_ELF_SHDR *v8; // eax
  SYMS *tail; // edx
  int v10; // [esp+4h] [ebp-2Ch]
  int n; // [esp+8h] [ebp-28h]
  int i; // [esp+Ch] [ebp-24h]
  SYMS *syms; // [esp+14h] [ebp-1Ch]
  char *strtab; // [esp+18h] [ebp-18h]
  char *shstrtab; // [esp+1Ch] [ebp-14h]
  DS_ELF_SYMTAB *psym; // [esp+20h] [ebp-10h]
  DS_ELF_SYMTAB *symtab; // [esp+24h] [ebp-Ch]
  DS_ELF_SHDR *shsym; // [esp+2Ch] [ebp-4h]
  int ida; // [esp+4Ch] [ebp+1Ch]

  shsym = &shdr[symndx];
  symtab = 0;
  shstrtab = 0;
  strtab = 0;
  if ( ehdr->shstrndx )
  {
    if ( ehdr->shnum > ehdr->shstrndx )
    {
      shstrtab = (char *)ds_fload(stream, 0, shdr[ehdr->shstrndx].offset, shdr[ehdr->shstrndx].size, 1);
      if ( !shstrtab )
        goto LABEL_20;
    }
  }
  v10 = shsym->size >> 4;
  if ( v10 == 1 )
  {
    ds_printf("Loading no symbols ...\n");
    return 0;
  }
  if ( shsym->size >> 4 > 1 )
    ds_printf("Loading %d symbols ...\n", v10 - 1);
  symtab = (DS_ELF_SYMTAB *)ds_fload(stream, 0, shsym->offset, 16, v10);
  if ( !symtab )
    goto LABEL_20;
  strtab = (char *)ds_fload(stream, 0, shdr[strndx].offset, 1, shdr[strndx].size);
  if ( !strtab )
    goto LABEL_20;
  psym = symtab;
  for ( i = 0; v10 > i; ++i )
    ++psym;
#ifdef DSNET_COMPILING_E
  ida = look_eemod(stream, ehdr, shdr, id, base, (void (__cdecl *)())clear_symbol_with_id);
#elif DSNET_COMPILING_I
  ida = look_iopmod(stream, ehdr, shdr, id, base, (void (__cdecl *)())clear_symbol_with_id);
#endif /* DSNET_COMPILING_I */
  syms = (SYMS *)ds_alloc_mem_low("sym.c", "load_symbol", sizeof(SYMS));
  if ( !syms )
    goto LABEL_20;
  syms->id = ida;
  syms->base = base;
  syms->symtab = symtab;
  syms->shstrtab = shstrtab;
  syms->strtab = strtab;
  syms->shnum = ehdr->shnum;
  syms->nsymtab = v10;
  n = sizeof(DS_ELF_SHDR) * ehdr->shnum;
  v8 = (DS_ELF_SHDR *)ds_alloc_mem_low("sym.c", "load_symbol", n);
  syms->shdr = v8;
  if ( !v8 )
  {
    ds_free_mem_low(syms, "sym.c", "load_symbol");
LABEL_20:
    ds_free_mem_low(shstrtab, "sym.c", "load_symbol");
    ds_free_mem_low(symtab, "sym.c", "load_symbol");
    ds_free_mem_low(strtab, "sym.c", "load_symbol");
    return -1;
  }
  memcpy(syms->shdr, shdr, n);
  tail = syms_list.tail;
  syms->back = syms_list.tail;
  if ( tail )
    syms->back->forw = syms;
  else
    syms_list.head = syms;
  syms->forw = 0;
  syms_list.tail = syms;
  return 0;
}

static int __cdecl is_dup_sym(char *bp, char *str)
{
  int n; // [esp+4h] [ebp-408h]
  char *p; // [esp+8h] [ebp-404h]
  char tmp[1024]; // [esp+Ch] [ebp-400h] BYREF

  n = strlen(str);
  while ( *bp )
  {
    p = tmp;
    while ( *bp && *bp != 44 )
      *p++ = *bp++;
    *p = 0;
    if ( !strcmp(tmp, str) )
      return 1;
    if ( !ds_strncmp(tmp, str, n) && !strcmp("+0x00", &tmp[n]) )
      return 1;
    if ( *bp == 44 )
      ++bp;
  }
  return 0;
}

int __cdecl address_to_func(unsigned int adr, unsigned int *padr0, unsigned int *padr1)
{
  unsigned int value; // [esp+0h] [ebp-18h]
  unsigned int value_1; // [esp+0h] [ebp-18h]
  unsigned int adr1; // [esp+4h] [ebp-14h]
  unsigned int adr0; // [esp+8h] [ebp-10h]
  int i; // [esp+Ch] [ebp-Ch]
  DS_ELF_SYMTAB *p; // [esp+10h] [ebp-8h]
  SYMS *syms; // [esp+14h] [ebp-4h]

  adr1 = 0;
  adr0 = 0;
  syms = syms_list.head;
LABEL_2:
  if ( syms )
  {
    p = syms->symtab;
    i = 0;
    while ( 1 )
    {
      if ( syms->nsymtab <= i )
      {
        syms = syms->forw;
        goto LABEL_2;
      }
      switch ( p->info & 0xF )
      {
        case 1:
        case 2:
          value = p->value;
          value_1 = module_base(syms->id, syms->base, p->shndx, p->info) + value;
          if ( p->size )
          {
            if ( value_1 > adr || p->size <= adr - value_1 )
              goto LABEL_18;
          }
          else if ( value_1 != adr )
          {
            goto LABEL_18;
          }
          if ( !adr0 || adr0 > value_1 )
            adr0 = value_1;
          if ( !adr1 || adr1 < p->size + value_1 )
            adr1 = p->size + value_1;
LABEL_18:
          ++i;
          ++p;
          break;
        default:
          goto LABEL_18;
      }
    }
  }
  *padr0 = adr0;
  *padr1 = adr1;
  return 0;
}

int __cdecl address_to_symbol(char *buf, unsigned int adr, int force_delta)
{
  int v3; // eax
  int v4; // eax
  int v6; // eax
  int v7; // eax
  MOD_SYMS *mp; // [esp+0h] [ebp-420h]
  unsigned int value; // [esp+4h] [ebp-41Ch]
  unsigned int value_1; // [esp+4h] [ebp-41Ch]
  int delta; // [esp+8h] [ebp-418h]
  int delta_1; // [esp+8h] [ebp-418h]
  int i; // [esp+Ch] [ebp-414h]
  char *sp; // [esp+10h] [ebp-410h]
  char *sp_1; // [esp+10h] [ebp-410h]
  char str[1024]; // [esp+14h] [ebp-40Ch] BYREF
  char *bp; // [esp+414h] [ebp-Ch]
  DS_ELF_SYMTAB *p; // [esp+418h] [ebp-8h]
  SYMS *syms; // [esp+41Ch] [ebp-4h]

  bp = buf;
  *buf = 0;
  syms = syms_list.head;
LABEL_2:
  if ( syms )
  {
    p = syms->symtab;
    i = 0;
    while ( 1 )
    {
      if ( syms->nsymtab <= i )
      {
        syms = syms->forw;
        goto LABEL_2;
      }
      switch ( p->info & 0xF )
      {
        case 0:
        case 1:
        case 2:
          value = p->value;
          value_1 = module_base(syms->id, syms->base, p->shndx, p->info) + value;
          delta = 0;
          if ( p->size )
          {
            delta = adr - value_1;
            if ( value_1 > adr || delta >= p->size )
              goto LABEL_20;
          }
          else if ( value_1 != adr )
          {
            goto LABEL_20;
          }
          if ( !is_gcc_label(&syms->strtab[p->name]) )
          {
            sp = &str[ds_sprintf(str, "%s", &syms->strtab[p->name])];
            if ( force_delta || delta > 0 )
              ds_sprintf(sp, "+0x%02x", delta);
            if ( !is_dup_sym(buf, str) )
            {
              if ( bp > buf )
              {
                v3 = ds_sprintf(bp, ",");
                bp += v3;
              }
              v4 = ds_sprintf(bp, "%s", str);
              bp += v4;
              force_delta = 0;
            }
          }
LABEL_20:
          ++i;
          ++p;
          break;
        default:
          goto LABEL_20;
      }
    }
  }
  if ( bp > buf )
    return bp - buf;
  for ( mp = mod_syms_list.head; mp; mp = mp->forw )
  {
    delta_1 = 0;
    if ( mp->siz )
    {
      delta_1 = adr - mp->adr;
      if ( mp->adr > adr || delta_1 >= mp->siz )
        continue;
    }
    else if ( mp->adr != adr )
    {
      continue;
    }
    sp_1 = &str[ds_sprintf(str, "%s", mp->name)];
    if ( force_delta || delta_1 > 0 )
      ds_sprintf(sp_1, "+0x%02x", delta_1);
    if ( !is_dup_sym(buf, str) )
    {
      if ( bp > buf )
      {
        v6 = ds_sprintf(bp, ",");
        bp += v6;
      }
      v7 = ds_sprintf(bp, "%s", str);
      bp += v7;
      force_delta = 0;
    }
  }
  return bp - buf;
}

int __cdecl symbol_to_value(char *name, unsigned int *pv)
{
  int v2; // eax
  unsigned __int8 v4; // dl
  MOD_SYMS *mp; // [esp+8h] [ebp-20h]
  int r; // [esp+Ch] [ebp-1Ch]
  int n; // [esp+10h] [ebp-18h]
  int i_3; // [esp+14h] [ebp-14h]
  char *file; // [esp+18h] [ebp-10h]
  char *cp; // [esp+1Ch] [ebp-Ch]
  DS_ELF_SYMTAB *p; // [esp+20h] [ebp-8h]
  SYMS *syms; // [esp+24h] [ebp-4h]

  if ( *name == 58 )
  {
    ++name;
    v2 = 0;
    if ( *name > 47 && *name <= 57 )
      v2 = 1;
    if ( v2 )
      return symbol_to_value_by_mdebug(0, name, pv);
  }
  else
  {
    for ( cp = name; *cp; ++cp )
    {
      if ( *cp == 58 )
      {
        file = (char *)ds_alloc_mem_low("sym.c", "symbol_to_value", cp - name + 1);
        if ( !file )
          return -1;
        memcpy(file, name, cp - name);
        file[cp - name] = 0;
        r = symbol_to_value_by_mdebug(file, cp + 1, pv);
        ds_free_mem_low(file, "sym.c", "symbol_to_value");
        return r;
      }
    }
  }
  if ( !strcmp(".", name) )
  {
    *pv = dot;
    return 0;
  }
  for ( syms = syms_list.head; syms; syms = syms->forw )
  {
    n = 0;
    p = syms->symtab;
    for ( i_3 = 0; syms->nsymtab > i_3; ++i_3 )
    {
      v4 = p->info >> 4;
      if ( v4 == 1 || v4 <= 1u && !v4 )
      {
        switch ( p->info & 0xF )
        {
          case 0:
          case 1:
          case 2:
            if ( is_gcc_label(&syms->strtab[p->name]) || strcmp(name, &syms->strtab[p->name]) )
              break;
            *pv = p->value;
            *pv += module_base(syms->id, syms->base, p->shndx, p->info);
            if ( p->info >> 4 )
              return 0;
            if ( ++n != 1 )
              return ds_error("symbol_to_value:non unique local symbol");
            break;
          default:
            break;
        }
      }
      ++p;
    }
    if ( n > 0 )
      return 0;
  }
  for ( mp = mod_syms_list.head; mp; mp = mp->forw )
  {
    if ( !strcmp(name, mp->name) )
    {
      *pv = mp->adr;
      return 0;
    }
  }
  return -1;
}

int __cdecl symbol_completion(DS_HISTBUF *hb, char *name)
{
  unsigned __int8 v3; // dl
  unsigned __int8 v4; // dl
  MOD_SYMS *mp; // [esp+0h] [ebp-2Ch]
  MOD_SYMS *mp_1; // [esp+0h] [ebp-2Ch]
  int col; // [esp+4h] [ebp-28h]
  int lns; // [esp+8h] [ebp-24h]
  int lns_1; // [esp+8h] [ebp-24h]
  int ns; // [esp+Ch] [ebp-20h]
  int nm; // [esp+10h] [ebp-1Ch]
  int n; // [esp+14h] [ebp-18h]
  int i_3; // [esp+18h] [ebp-14h]
  int i_1; // [esp+18h] [ebp-14h]
  char *cn; // [esp+1Ch] [ebp-10h]
  char *p; // [esp+20h] [ebp-Ch]
  char *p_1; // [esp+20h] [ebp-Ch]
  char *p_2; // [esp+20h] [ebp-Ch]
  char *p_3; // [esp+20h] [ebp-Ch]
  char *p_4; // [esp+20h] [ebp-Ch]
  char *p_5; // [esp+20h] [ebp-Ch]
  const char *v22; // [esp+20h] [ebp-Ch]
  DS_ELF_SYMTAB *st; // [esp+24h] [ebp-8h]
  DS_ELF_SYMTAB *st_1; // [esp+24h] [ebp-8h]
  SYMS *syms; // [esp+28h] [ebp-4h]
  SYMS *syms_1; // [esp+28h] [ebp-4h]

  if ( *name == 58 )
    ++name;
  for ( p = name; *p; ++p )
  {
    if ( *p == 58 )
      return 0;
  }
  n = strlen(name);
  nm = 0;
  for ( syms = syms_list.head; syms; syms = syms->forw )
  {
    st = syms->symtab;
    for ( i_3 = 0; syms->nsymtab > i_3; ++i_3 )
    {
      v3 = st->info >> 4;
      if ( v3 == 1 || v3 <= 1u && !v3 )
      {
        switch ( st->info & 0xF )
        {
          case 0:
          case 1:
          case 2:
            if ( st->name )
            {
              if ( !is_gcc_label(&syms->strtab[st->name]) )
              {
                p_1 = &syms->strtab[st->name];
                if ( !ds_strncmp(name, p_1, n) )
                {
                  if ( ++nm == 1 )
                  {
                    cn = p_1;
                    ns = strlen(p_1);
                  }
                  else
                  {
                    lns = ns;
                    for ( ns = 0; lns > ns && p_1[ns] == cn[ns]; ++ns )
                      ;
                  }
                }
              }
            }
            break;
          default:
            break;
        }
      }
      ++st;
    }
  }
  for ( mp = mod_syms_list.head; mp; mp = mp->forw )
  {
    p_2 = mp->name;
    if ( !ds_strncmp(name, mp->name, n) )
    {
      if ( ++nm == 1 )
      {
        cn = mp->name;
        ns = strlen(p_2);
      }
      else
      {
        lns_1 = ns;
        for ( ns = 0; lns_1 > ns && p_2[ns] == cn[ns]; ++ns )
          ;
      }
    }
  }
  if ( !nm )
    return 0;
  if ( nm == 1 )
  {
    for ( p_3 = &cn[n]; *p_3; ++p_3 )
      ds_editline(hb, *p_3, 0);
    return 1;
  }
  else
  {
    for ( p_4 = &cn[n]; *p_4 && p_4 < &cn[ns]; ++p_4 )
      ds_editline(hb, *p_4, 0);
    ds_printf("\n");
    col = 0;
    for ( syms_1 = syms_list.head; syms_1; syms_1 = syms_1->forw )
    {
      st_1 = syms_1->symtab;
      for ( i_1 = 0; syms_1->nsymtab > i_1; ++i_1 )
      {
        v4 = st_1->info >> 4;
        if ( v4 == 1 || v4 <= 1u && !v4 )
        {
          switch ( st_1->info & 0xF )
          {
            case 0:
            case 1:
            case 2:
              if ( st_1->name )
              {
                if ( !is_gcc_label(&syms_1->strtab[st_1->name]) )
                {
                  p_5 = &syms_1->strtab[st_1->name];
                  if ( !ds_strncmp(name, p_5, n) )
                  {
                    if ( col + strlen(p_5) + 1 > 0x4F )
                    {
                      ds_printf("\n");
                      col = 0;
                    }
                    col += ds_printf(" %s", p_5);
                  }
                }
              }
              break;
            default:
              break;
          }
        }
        ++st_1;
      }
    }
    for ( mp_1 = mod_syms_list.head; mp_1; mp_1 = mp_1->forw )
    {
      v22 = mp_1->name;
      if ( !ds_strncmp(name, mp_1->name, n) )
      {
        if ( col + strlen(v22) + 1 > 0x4F )
        {
          ds_printf("\n");
          col = 0;
        }
        col += ds_printf(" %s", v22);
      }
    }
    return -1;
  }
}

static int __cdecl is_gcc_label(char *str)
{
  return !strcmp(str, "gcc2_compiled.") || strcmp(str, "__gnu_compiled_c") == 0;
}

