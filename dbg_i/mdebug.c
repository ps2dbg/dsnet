#include "dsidb_prototypes.h"

static struct {MDEBUG *head;MDEBUG *tail;} mdebug_list = { NULL, NULL };
static char *cur_fname = NULL;
static char *cur_buf = NULL;
static int cur_size = 0;
static char *next_file_27 = NULL;
static int next_line_28 = 0;
static int next_cnt_29 = 0;
static char *next_obj_path_30 = NULL;

static char path_22[1024];

static void __cdecl print_sym(SYM *p, char *strs);
static char *__cdecl set_abs_path(char *fname);
static void __cdecl clear_source_line_buffer();
static char *__cdecl search_source_file(char *fname, char *obj_path, char *src_dirs);

void __cdecl clear_mdebug()
{
  MDEBUG *q; // [esp+0h] [ebp-8h]
  MDEBUG *p; // [esp+4h] [ebp-4h]

  for ( p = mdebug_list.head; p; p = q )
  {
    q = p->forw;
    ds_free_mem_low(p->shdr, "mdebug.c", "clear_mdebug");
    ds_free_mem_low(p->fdt_adrs, "mdebug.c", "clear_mdebug");
    ds_free_mem_low(p->pdt_adrs, "mdebug.c", "clear_mdebug");
    ds_free_mem_low(p->path, "mdebug.c", "clear_mdebug");
    ds_free_mem_low(p, "mdebug.c", "clear_mdebug");
  }
  mdebug_list.tail = 0;
  mdebug_list.head = 0;
  clear_source_line_buffer();
}

void __cdecl clear_mdebug_with_id(int id)
{
  MDEBUG *q; // [esp+8h] [ebp-8h]
  MDEBUG *p; // [esp+Ch] [ebp-4h]

  for ( p = mdebug_list.head; p; p = q )
  {
    q = p->forw;
    if ( p->id == id )
    {
      if ( p->forw )
        p->forw->back = p->back;
      else
        mdebug_list.tail = p->back;
      if ( p->back )
        p->back->forw = p->forw;
      else
        mdebug_list.head = p->forw;
      ds_free_mem_low(p->shdr, "mdebug.c", "clear_mdebug_with_id");
      ds_free_mem_low(p->fdt_adrs, "mdebug.c", "clear_mdebug_with_id");
      ds_free_mem_low(p->pdt_adrs, "mdebug.c", "clear_mdebug_with_id");
      ds_free_mem_low(p->path, "mdebug.c", "clear_mdebug_with_id");
      ds_free_mem_low(p, "mdebug.c", "clear_mdebug_with_id");
    }
  }
  clear_source_line_buffer();
}

static void __cdecl print_sym(SYM *p, char *strs)
{
  char *s; // [esp+0h] [ebp-4h]
  char *s_1; // [esp+0h] [ebp-4h]
  char *s_2; // [esp+0h] [ebp-4h]

  ds_printf(" 0x%W", p->value);
  switch ( p->st_sc_index & 0x3F )
  {
    case 0u:
      s = "Nil";
      break;
    case 1u:
      s = "Global";
      break;
    case 2u:
      s = "Static";
      break;
    case 3u:
      s = "Param";
      break;
    case 4u:
      s = "Local";
      break;
    case 5u:
      s = "Label";
      break;
    case 6u:
      s = "Proc";
      break;
    case 7u:
      s = "Block";
      break;
    case 8u:
      s = "End";
      break;
    case 9u:
      s = "Member";
      break;
    case 0xAu:
      s = "Typedef";
      break;
    case 0xBu:
      s = "File";
      break;
    case 0xEu:
      s = "StaticProc";
      break;
    case 0xFu:
      s = "Constant";
      break;
    default:
      s = 0;
      break;
  }
  if ( s )
    ds_printf(" %-10s", s);
  else
    ds_printf(" %10d", p->st_sc_index & 0x3F);
  switch ( (p->st_sc_index >> 6) & 0x1F )
  {
    case 0u:
      s_1 = "Nil";
      break;
    case 1u:
      s_1 = "Text";
      break;
    case 2u:
      s_1 = "Data";
      break;
    case 3u:
      s_1 = "Bss";
      break;
    case 4u:
      s_1 = "Register";
      break;
    case 5u:
      s_1 = "Abs";
      break;
    case 6u:
      s_1 = "Undefined";
      break;
    case 8u:
      s_1 = "Bits";
      break;
    case 9u:
      s_1 = "Dbx";
      break;
    case 0xAu:
      s_1 = "RegImage";
      break;
    case 0xBu:
      s_1 = "Info";
      break;
    case 0xCu:
      s_1 = "UserStruct";
      break;
    case 0xDu:
      s_1 = "SData";
      break;
    case 0xEu:
      s_1 = "SBss";
      break;
    case 0xFu:
      s_1 = "RData";
      break;
    case 0x10u:
      s_1 = "Var";
      break;
    case 0x11u:
      s_1 = "Common";
      break;
    case 0x12u:
      s_1 = "SCommon";
      break;
    case 0x13u:
      s_1 = "VarRegister";
      break;
    case 0x14u:
      s_1 = "Variant";
      break;
    case 0x15u:
      s_1 = "SUndefined";
      break;
    default:
      s_1 = 0;
      break;
  }
  if ( s_1 )
    ds_printf(" %-11s", s_1);
  else
    ds_printf(" %11d", (p->st_sc_index >> 6) & 0x1F);
  ds_printf(" 0x%05x", (unsigned __int16)HIWORD(p->st_sc_index) >> 4);
  if ( ((p->st_sc_index >> 12) & 0xFFF00) == 586496 )
  {
    switch ( (unsigned __int8)(p->st_sc_index >> 12) )
    {
      case 0u:
        s_2 = "UNDF";
        break;
      case 0x20u:
        s_2 = "GSYM";
        break;
      case 0x22u:
        s_2 = "FNAME";
        break;
      case 0x24u:
        s_2 = "FUN";
        break;
      case 0x26u:
        s_2 = "STSYM";
        break;
      case 0x28u:
        s_2 = "LCSYM";
        break;
      case 0x2Au:
        s_2 = "MAIN";
        break;
      case 0x2Cu:
        s_2 = "ROSYM";
        break;
      case 0x30u:
        s_2 = "PC";
        break;
      case 0x32u:
        s_2 = "NSYMS";
        break;
      case 0x34u:
        s_2 = "NOMAP";
        break;
      case 0x38u:
        s_2 = "OBJ";
        break;
      case 0x3Cu:
        s_2 = "OPT";
        break;
      case 0x40u:
        s_2 = "RSYM";
        break;
      case 0x42u:
        s_2 = "M2C";
        break;
      case 0x44u:
        s_2 = "SLINE";
        break;
      case 0x46u:
        s_2 = "DSLINE";
        break;
      case 0x48u:
        s_2 = "BSLINE";
        break;
      case 0x4Au:
        s_2 = "DEFD";
        break;
      case 0x4Cu:
        s_2 = "FLINE";
        break;
      case 0x50u:
        s_2 = "EHDECL";
        break;
      case 0x54u:
        s_2 = "CATCH";
        break;
      case 0x60u:
        s_2 = "SSYM";
        break;
      case 0x62u:
        s_2 = "ENDM";
        break;
      case 0x64u:
        s_2 = "SO";
        break;
      case 0x6Cu:
        s_2 = "ALIAS";
        break;
      case 0x80u:
        s_2 = "LSYM";
        break;
      case 0x82u:
        s_2 = "BINCL";
        break;
      case 0x84u:
        s_2 = "SOL";
        break;
      case 0xA0u:
        s_2 = "PSYM";
        break;
      case 0xA2u:
        s_2 = "EINCL";
        break;
      case 0xA4u:
        s_2 = "ENTRY";
        break;
      case 0xC0u:
        s_2 = "LBRAC";
        break;
      case 0xC2u:
        s_2 = "EXCL";
        break;
      case 0xC4u:
        s_2 = "SCOPE";
        break;
      case 0xE0u:
        s_2 = "RBRAC";
        break;
      case 0xE2u:
        s_2 = "BCOMM";
        break;
      case 0xE4u:
        s_2 = "ECOMM";
        break;
      case 0xE8u:
        s_2 = "ECOML";
        break;
      case 0xEAu:
        s_2 = "WITH";
        break;
      case 0xF0u:
        s_2 = "NBTEXT";
        break;
      case 0xF2u:
        s_2 = "NBDATA";
        break;
      case 0xF4u:
        s_2 = "NBBSS";
        break;
      case 0xF6u:
        s_2 = "NBSTS";
        break;
      case 0xF8u:
        s_2 = "NBLCS";
        break;
      case 0xFEu:
        s_2 = "LENG";
        break;
      default:
        s_2 = 0;
        break;
    }
  }
  else
  {
    s_2 = 0;
  }
  if ( s_2 )
    ds_printf(" %-6s", s_2);
  else
    ds_printf(" %6s", "");
  ds_printf(" %s\n", &strs[p->iss]);
}

int __cdecl show_mdebug(int ac, char **av)
{
  int n; // [esp+0h] [ebp-14h]
  char *str; // [esp+4h] [ebp-10h]
  EXT_SYM *esym; // [esp+8h] [ebp-Ch]
  SYM *sym; // [esp+Ch] [ebp-8h]
  MDEBUG *md; // [esp+10h] [ebp-4h]
  int aca; // [esp+1Ch] [ebp+8h]
  char **ava; // [esp+20h] [ebp+Ch]

  str = 0;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  ava = av + 1;
  if ( aca > 0 && **ava == 45 )
    return ds_error("Usage: show mdebug [<str>]");
  if ( aca > 0 )
  {
    str = *ava;
    n = strlen(*ava);
    if ( aca > 1 )
      return ds_error("Usage: show mdebug [<str>]");
  }
  for ( md = mdebug_list.head; md; md = md->forw )
  {
    ds_printf("<<%s>>\n", md->path);
    for ( sym = md->lsyms; md->lsyme > sym; ++sym )
    {
      if ( match_symbol(str, n, &md->lstrs[sym->iss]) )
      {
        ds_printf(" L");
        print_sym(sym, md->lstrs);
      }
    }
    for ( esym = md->esyms; md->esyme > esym; ++esym )
    {
      if ( match_symbol(str, n, &md->estrs[esym->sym.iss]) )
      {
        ds_printf(" E");
        print_sym(&esym->sym, md->estrs);
      }
    }
  }
  return 0;
}

static char *__cdecl set_abs_path(char *fname)
{
  size_t v2; // eax
  char *r; // [esp+0h] [ebp-404h]
  char path[1024]; // [esp+4h] [ebp-400h] BYREF

  if ( !ds_abs_path(path, 1024, fname) )
    return 0;
  v2 = strlen(path);
  r = (char *)ds_alloc_mem_low("mdebug.c", "set_abs_path", v2 + 1);
  if ( r )
    return strcpy(r, path);
  else
    return 0;
}

int __cdecl load_mdebug(
        void *stream,
        DS_ELF_EHDR *elf_header,
        DS_ELF_SHDR *section_header,
        int mindex,
        int id,
        int base,
        char *path)
{
  char *v8; // eax
  ADRS *v9; // eax
  ADRS *v10; // eax
  MDEBUG *tail; // edx
  unsigned int pdt_eadr; // [esp+Ch] [ebp-40h]
  unsigned int pdt_sadr; // [esp+10h] [ebp-3Ch]
  unsigned int fdt_eadr; // [esp+14h] [ebp-38h]
  unsigned int fdt_sadr; // [esp+18h] [ebp-34h]
  EXT_SYM *esym; // [esp+1Ch] [ebp-30h]
  SYM *syme; // [esp+20h] [ebp-2Ch]
  SYM *syms; // [esp+24h] [ebp-28h]
  SYM *i; // [esp+28h] [ebp-24h]
  SYM *sym_1; // [esp+28h] [ebp-24h]
  PDT *pdte; // [esp+2Ch] [ebp-20h]
  PDT *pdt; // [esp+30h] [ebp-1Ch]
  PDT *pdt_1; // [esp+30h] [ebp-1Ch]
  FDT *fdt; // [esp+34h] [ebp-18h]
  FDT *fdt_1; // [esp+34h] [ebp-18h]
  MDEBUG *md; // [esp+38h] [ebp-14h]
  unsigned __int8 *buf; // [esp+40h] [ebp-Ch]
  int size; // [esp+44h] [ebp-8h]
  int offset; // [esp+48h] [ebp-4h]
  int ida; // [esp+64h] [ebp+18h]

  ida = look_iopmod(stream, elf_header, section_header, id, base, (void (__cdecl *)())clear_mdebug_with_id);
  if ( ida < 0 )
    return -1;
  offset = section_header[mindex].offset;
  size = section_header[mindex].size;
  buf = (unsigned __int8 *)ds_fload(stream, 0, offset, size, 1);
  if ( !buf )
    return -1;
  md = (MDEBUG *)ds_alloc_mem_low("mdebug.c", "load_mdebug", sizeof(MDEBUG));
  if ( !md )
  {
    ds_free_mem_low(buf, "mdebug.c", "load_mdebug");
    return -1;
  }
  ds_bzero(md, sizeof(MDEBUG));
  v8 = set_abs_path(path);
  md->path = v8;
  if ( !v8 )
    goto LABEL_91;
  md->shdr = (SYM_HDR *)buf;
  md->pdt_adrs = 0;
  md->fdt_adrs = 0;
  if ( *((_DWORD *)buf + 2) )
  {
    md->lins = &buf[*((_DWORD *)buf + 3) - offset];
    md->line = &md->lins[*((_DWORD *)buf + 2)];
    if ( buf > md->lins || md->line > &buf[size] )
    {
      ds_error("invalid cblineOffset,cbLine");
LABEL_91:
      ds_free_mem_low(md->shdr, "mdebug.c", "load_mdebug");
      ds_free_mem_low(md->fdt_adrs, "mdebug.c", "load_mdebug");
      ds_free_mem_low(md->pdt_adrs, "mdebug.c", "load_mdebug");
      ds_free_mem_low(md->path, "mdebug.c", "load_mdebug");
      ds_free_mem_low(md, "mdebug.c", "load_mdebug");
      return -1;
    }
  }
  else
  {
    md->line = 0;
    md->lins = 0;
  }
  if ( *((_DWORD *)buf + 6) )
  {
    md->pdts = (PDT *)&buf[*((_DWORD *)buf + 7) - offset];
    md->pdte = &md->pdts[*((_DWORD *)buf + 6)];
    if ( (PDT *)buf > md->pdts || md->pdte > (PDT *)&buf[size] )
    {
      ds_error("invalid cbPdOffset,ipdMax");
      goto LABEL_91;
    }
  }
  else
  {
    md->pdte = 0;
    md->pdts = 0;
  }
  for ( pdt = md->pdts; md->pdte > pdt; ++pdt )
    ;
  if ( *((_DWORD *)buf + 8) )
  {
    md->lsyms = (SYM *)&buf[*((_DWORD *)buf + 9) - offset];
    md->lsyme = &md->lsyms[*((_DWORD *)buf + 8)];
    if ( (SYM *)buf > md->lsyms || md->lsyme > (SYM *)&buf[size] )
    {
      ds_error("invalid cbSymOffset,isymMax");
      goto LABEL_91;
    }
  }
  else
  {
    md->lsyme = 0;
    md->lsyms = 0;
  }
  for ( i = md->lsyms; md->lsyme > i; ++i )
    ;
  if ( *((_DWORD *)buf + 14) )
  {
    md->lstrs = (char *)&buf[*((_DWORD *)buf + 15) - offset];
    md->lstre = &md->lstrs[*((_DWORD *)buf + 14)];
    if ( (char *)buf > md->lstrs || md->lstre > (char *)&buf[size] )
    {
      ds_error("invalid cbSsOffset,issMax");
      goto LABEL_91;
    }
  }
  else
  {
    md->lstre = 0;
    md->lstrs = 0;
  }
  if ( *((_DWORD *)buf + 22) )
  {
    md->esyms = (EXT_SYM *)&buf[*((_DWORD *)buf + 23) - offset];
    md->esyme = &md->esyms[*((_DWORD *)buf + 22)];
    if ( (EXT_SYM *)buf > md->esyms || md->esyme > (EXT_SYM *)&buf[size] )
    {
      ds_error("invalid cbExtOffset,iextMax");
      goto LABEL_91;
    }
  }
  else
  {
    md->esyme = 0;
    md->esyms = 0;
  }
  for ( esym = md->esyms; md->esyme > esym; ++esym )
    ;
  if ( *((_DWORD *)buf + 16) )
  {
    md->estrs = (char *)&buf[*((_DWORD *)buf + 17) - offset];
    md->estre = &md->estrs[*((_DWORD *)buf + 16)];
    if ( (char *)buf > md->estrs || md->estre > (char *)&buf[size] )
    {
      ds_error("invalid cbSsExtOffset,issExtMax");
      goto LABEL_91;
    }
  }
  else
  {
    md->estre = 0;
    md->estrs = 0;
  }
  if ( *((_DWORD *)buf + 18) )
  {
    md->fdts = (FDT *)&buf[*((_DWORD *)buf + 19) - offset];
    md->fdte = &md->fdts[*((_DWORD *)buf + 18)];
    if ( (FDT *)buf > md->fdts || md->fdte > (FDT *)&buf[size] )
    {
      ds_error("invalid cbFdOffset,ifdMax");
      goto LABEL_91;
    }
  }
  else
  {
    md->fdte = 0;
    md->fdts = 0;
  }
  for ( fdt = md->fdts; md->fdte > fdt; ++fdt )
    ;
  v9 = (ADRS *)ds_alloc_mem_low("mdebug.c", "load_mdebug", sizeof(ADRS) * *((_DWORD *)buf + 18));
  md->fdt_adrs = v9;
  if ( !v9 )
    goto LABEL_91;
  v10 = (ADRS *)ds_alloc_mem_low("mdebug.c", "load_mdebug", sizeof(ADRS) * *((_DWORD *)buf + 6));
  md->pdt_adrs = v10;
  if ( !v10 )
    goto LABEL_91;
  for ( fdt_1 = md->fdts; md->fdte > fdt_1; ++fdt_1 )
  {
    fdt_eadr = fdt_1->adr;
    fdt_sadr = fdt_1->adr;
    pdt_1 = &md->pdts[fdt_1->ipdFirst];
    pdte = &pdt_1[fdt_1->cpd];
    while ( pdte > pdt_1 )
    {
      pdt_eadr = pdt_1->adr + fdt_1->adr;
      pdt_sadr = pdt_eadr;
      syme = 0;
      syms = 0;
      for ( sym_1 = &md->lsyms[fdt_1->isymBase + pdt_1->isym]; md->lsyme > sym_1; ++sym_1 )
      {
        if ( !syms && ((sym_1->st_sc_index & 0x3F) == 6 || (sym_1->st_sc_index & 0x3F) == 14) )
        {
          syms = sym_1;
        }
        else if ( (sym_1->st_sc_index & 0x3F) == 8 )
        {
          syme = sym_1;
          break;
        }
      }
      if ( syms )
      {
        pdt_eadr = syms->value;
        pdt_sadr = pdt_eadr;
      }
      if ( syme )
        pdt_eadr += syme->value;
      md->pdt_adrs[(-991146299 * ((char *)pdt_1 - (char *)md->pdts)) >> 2].sadr = pdt_sadr;
      md->pdt_adrs[(-991146299 * ((char *)pdt_1 - (char *)md->pdts)) >> 2].eadr = pdt_eadr;
      if ( fdt_sadr > pdt_sadr )
        fdt_sadr = pdt_sadr;
      if ( pdt_eadr > fdt_eadr )
        fdt_eadr = pdt_eadr;
      ++pdt_1;
    }
    md->fdt_adrs[(954437177 * ((char *)fdt_1 - (char *)md->fdts)) >> 3].sadr = fdt_sadr;
    md->fdt_adrs[(954437177 * ((char *)fdt_1 - (char *)md->fdts)) >> 3].eadr = fdt_eadr;
  }
  md->id = ida;
  md->base = base;
  tail = mdebug_list.tail;
  md->back = mdebug_list.tail;
  if ( tail )
    md->back->forw = md;
  else
    mdebug_list.head = md;
  md->forw = 0;
  mdebug_list.tail = md;
  clear_source_line_buffer();
  return 0;
}

unsigned int __cdecl file_and_line_to_address(int line, char *path)
{
  int base; // [esp+Ch] [ebp-20h]
  SYM *sym; // [esp+14h] [ebp-18h]
  ADRS *adrs; // [esp+18h] [ebp-14h]
  FDT *fdte; // [esp+1Ch] [ebp-10h]
  FDT *fdt; // [esp+20h] [ebp-Ch]
  MDEBUG *md; // [esp+28h] [ebp-4h]

  for ( md = mdebug_list.head; md; md = md->forw )
  {
    base = module_base(md->id, md->base, 65311, 0);
    fdt = md->fdts;
    fdte = &fdt[md->shdr->ifdMax];
    adrs = md->fdt_adrs;
    while ( fdte > fdt )
    {
      if ( !strcmp(&md->lstrs[fdt->issBase + fdt->rss], path) && fdt < fdte )
      {
        for ( sym = &md->lsyms[fdt->isymBase]; &md->lsyms[fdt->isymBase + fdt->csym] > sym; ++sym )
        {
          if ( (sym->st_sc_index & 0x3F) == 5
            && ((sym->st_sc_index >> 6) & 0x1F) == 1
            && line == (unsigned __int16)HIWORD(sym->st_sc_index) >> 4 )
          {
            return sym->value + base;
          }
        }
      }
      ++adrs;
      ++fdt;
    }
  }
  return 0;
}

char *__cdecl address_to_file_and_line(unsigned int loc, int *pline, int *f_has, int *pdelta, char **ppath)
{
  unsigned int value; // [esp+4h] [ebp-44h]
  unsigned int pdt_eadr; // [esp+Ch] [ebp-3Ch]
  unsigned int pdt_sadr; // [esp+10h] [ebp-38h]
  int delta; // [esp+14h] [ebp-34h]
  int line; // [esp+18h] [ebp-30h]
  int nfound; // [esp+1Ch] [ebp-2Ch]
  int base; // [esp+20h] [ebp-28h]
  SYM *syme; // [esp+24h] [ebp-24h]
  SYM *sym; // [esp+28h] [ebp-20h]
  ADRS *adrs; // [esp+2Ch] [ebp-1Ch]
  ADRS *adrs_1; // [esp+2Ch] [ebp-1Ch]
  PDT *pdte; // [esp+30h] [ebp-18h]
  PDT *pdt; // [esp+34h] [ebp-14h]
  FDT *fdte; // [esp+38h] [ebp-10h]
  FDT *fdt; // [esp+3Ch] [ebp-Ch]
  MDEBUG *md; // [esp+44h] [ebp-4h]

  if ( f_has )
    *f_has = 0;
  if ( pline )
    *pline = 0;
  if ( ppath )
    *ppath = 0;
  for ( md = mdebug_list.head; ; md = md->forw )
  {
    if ( !md )
      return 0;
    base = module_base(md->id, md->base, 65311, 0);
    fdt = md->fdts;
    fdte = &fdt[md->shdr->ifdMax];
    for ( adrs = md->fdt_adrs; fdte > fdt && (loc < adrs->sadr + base || loc >= adrs->eadr + base); ++adrs )
      ++fdt;
    if ( fdt < fdte )
    {
      pdt = &md->pdts[fdt->ipdFirst];
      pdte = &pdt[fdt->cpd];
      for ( adrs_1 = &md->pdt_adrs[fdt->ipdFirst];
            pdte > pdt && (loc < adrs_1->sadr + base || loc >= adrs_1->eadr + base);
            ++adrs_1 )
      {
        ++pdt;
      }
      if ( pdt < pdte )
      {
        pdt_sadr = adrs_1->sadr;
        pdt_eadr = adrs_1->eadr;
        sym = &md->lsyms[fdt->isymBase];
        syme = &sym[fdt->csym];
        nfound = 0;
        while ( syme > sym )
        {
          if ( (sym->st_sc_index & 0x3F) == 5 && ((sym->st_sc_index >> 6) & 0x1F) == 1 )
          {
            value = sym->value;
            if ( value < pdt_sadr || pdt_eadr <= value )
              goto LABEL_33;
            if ( f_has )
              *f_has = 1;
            if ( pdelta && (delta = loc - (value + base), delta >= 0) )
            {
              *pdelta = delta;
            }
            else
            {
LABEL_33:
              if ( loc != value + base )
                goto LABEL_35;
            }
            line = (unsigned __int16)HIWORD(sym->st_sc_index) >> 4;
            ++nfound;
          }
LABEL_35:
          ++sym;
        }
        if ( nfound )
          break;
      }
    }
  }
  if ( pline )
    *pline = line;
  if ( ppath )
    *ppath = md->path;
  return &md->lstrs[fdt->issBase + fdt->rss];
}

int __cdecl symbol_to_value_by_mdebug(char *file, char *name, unsigned int *pv)
{
  int v4; // eax
  unsigned int rpc; // [esp+8h] [ebp-20h] BYREF
  int delta; // [esp+Ch] [ebp-1Ch] BYREF
  int index; // [esp+10h] [ebp-18h]
  int line; // [esp+14h] [ebp-14h] BYREF
  SYM *syme; // [esp+18h] [ebp-10h]
  SYM *sym; // [esp+1Ch] [ebp-Ch]
  FDT *fdt; // [esp+20h] [ebp-8h]
  MDEBUG *md; // [esp+24h] [ebp-4h]

  line = 0;
  if ( file )
    goto LABEL_6;
  if ( ds_eval_word("$_PC", &rpc) )
    return -1;
  file = address_to_file_and_line(rpc, 0, 0, &delta, 0);
  if ( !file )
    return ds_error("*** No line for $PC");
LABEL_6:
  v4 = 0;
  if ( *name > 47 && *name <= 57 )
    v4 = 1;
  if ( v4 && ds_scan_digit_word(name, (unsigned int *)&line) )
    return -1;
  for ( md = mdebug_list.head; ; md = md->forw )
  {
    if ( !md )
      return -1;
    for ( fdt = md->fdts; md->fdte > fdt && strcmp(file, &md->lstrs[fdt->issBase + fdt->rss]); ++fdt )
      ;
    if ( fdt < md->fdte )
      break;
  }
  sym = &md->lsyms[fdt->isymBase];
  syme = &md->lsyms[fdt->isymBase + fdt->csym];
  if ( line )
  {
    while ( syme > sym )
    {
      if ( (sym->st_sc_index & 0x3F) == 5 && ((sym->st_sc_index >> 6) & 0x1F) == 1 )
      {
        index = (unsigned __int16)HIWORD(sym->st_sc_index) >> 4;
        if ( index == line )
          goto LABEL_25;
      }
      ++sym;
    }
  }
  else
  {
    while ( syme > sym )
    {
      if ( ((sym->st_sc_index & 0x3F) == 6 || (sym->st_sc_index & 0x3F) == 14) && !strcmp(name, &md->lstrs[sym->iss]) )
      {
LABEL_25:
        *pv = sym->value;
        *pv += module_base(md->id, md->base, 65311, 0);
        return 0;
      }
      ++sym;
    }
  }
  return -1;
}

static void __cdecl clear_source_line_buffer()
{
  cur_fname = (char *)ds_free_mem_low(cur_fname, "mdebug.c", "clear_source_line_buffer");
  cur_buf = (char *)ds_free_mem_low(cur_buf, "mdebug.c", "clear_source_line_buffer");
  cur_size = 0;
}

static char *__cdecl search_source_file(char *fname, char *obj_path, char *src_dirs)
{
  char *q; // [esp+8h] [ebp-808h]
  char *q_1; // [esp+8h] [ebp-808h]
  char *p; // [esp+Ch] [ebp-804h]
  char *p_1; // [esp+Ch] [ebp-804h]
  char tmp[1024]; // [esp+10h] [ebp-800h] BYREF
  char obj_dir[1024]; // [esp+410h] [ebp-400h] BYREF

  if ( *fname == 47 || !*src_dirs )
    return fname;
  if ( obj_path )
  {
    strcpy(obj_dir, obj_path);
    for ( p = &obj_dir[strlen(obj_dir) - 1]; p >= obj_dir && *p != 47; --p )
      ;
    if ( p >= obj_dir && *p == 47 )
      *p = 0;
  }
  p_1 = src_dirs;
  while ( *p_1 )
  {
    q = path_22;
    while ( *p_1 && *p_1 != 58 )
      *q++ = *p_1++;
    if ( *p_1 == 58 )
      ++p_1;
    *q = 0;
    if ( path_22[0] != 64 )
      goto LABEL_25;
    if ( obj_path )
    {
      strcpy(tmp, &path_22[1]);
      strcpy(path_22, obj_dir);
      q_1 = &path_22[strlen(path_22)];
      strcpy(q_1, tmp);
      q = &q_1[strlen(tmp)];
LABEL_25:
      if ( q > path_22 && *(q - 1) != 47 )
        *q++ = 47;
      strcpy(q, fname);
      if ( !ds_access(path_22, 1) )
        return path_22;
    }
  }
  return 0;
}

char *__cdecl string_by_file_and_line(char *fname, int line, char *obj_path)
{
  size_t v4; // eax
  char *v5; // eax
  char *v6; // eax
  char *v7; // [esp-4h] [ebp-1Ch]
  char *r; // [esp+0h] [ebp-18h]
  char *q; // [esp+4h] [ebp-14h]
  char *pe; // [esp+8h] [ebp-10h]
  char *p; // [esp+Ch] [ebp-Ch]
  int i; // [esp+10h] [ebp-8h]
  void *stream; // [esp+14h] [ebp-4h]
  char *s2; // [esp+20h] [ebp+8h]

  if ( !fname || !line )
    return 0;
  v7 = source_directories_str();
  s2 = search_source_file(fname, obj_path, v7);
  if ( !s2 )
    return 0;
  if ( cur_fname && cur_buf && !strcmp(cur_fname, s2) )
    goto LABEL_17;
  clear_source_line_buffer();
  stream = ds_fopen_low(s2, "r");
  if ( !stream )
    return 0;
  if ( ds_fsize(s2, &cur_size) < 0 )
    return 0;
  v4 = strlen(s2);
  cur_fname = (char *)ds_alloc_mem_low("mdebug.c", "string_by_file_and_line", v4 + 1);
  if ( !cur_fname )
    return 0;
  strcpy(cur_fname, s2);
  cur_buf = (char *)ds_fload(stream, 0, 0, 1, cur_size);
  ds_fclose(stream);
  if ( !cur_buf )
    return 0;
LABEL_17:
  p = cur_buf;
  pe = &cur_buf[cur_size];
  for ( i = 1; pe > p && line > i; ++i )
  {
    do
    {
      if ( pe <= p )
        break;
      v5 = p++;
    }
    while ( *v5 != 10 );
  }
  if ( p >= pe || line != i )
    return 0;
  for ( q = p; pe > q && *q != 10; ++q )
    ;
  v6 = (char *)ds_alloc_mem_low("mdebug.c", "string_by_file_and_line", q - p + 1);
  r = v6;
  if ( !v6 )
    return 0;
  memcpy(v6, p, q - p);
  r[q - p] = 0;
  return r;
}

int __cdecl list_cmd(int ac, char **av)
{
  int v3; // eax
  const char *v4; // eax
  int v5; // [esp-8h] [ebp-38h]
  const char *v6; // [esp-4h] [ebp-34h]
  int pdelta; // [esp+0h] [ebp-30h] BYREF
  int pline; // [esp+4h] [ebp-2Ch] BYREF
  int delta; // [esp+8h] [ebp-28h] BYREF
  int cnt; // [esp+Ch] [ebp-24h] BYREF
  int back; // [esp+10h] [ebp-20h] BYREF
  int line; // [esp+14h] [ebp-1Ch] BYREF
  char *ppath; // [esp+18h] [ebp-18h] BYREF
  char *rpc_file; // [esp+1Ch] [ebp-14h]
  char *v15; // [esp+20h] [ebp-10h]
  char *file; // [esp+24h] [ebp-Ch]
  unsigned int rpc; // [esp+28h] [ebp-8h] BYREF
  unsigned int adr; // [esp+2Ch] [ebp-4h] BYREF
  int aca; // [esp+38h] [ebp+8h]
  char **ava; // [esp+3Ch] [ebp+Ch]

  if ( ds_eval_word("$_PC", &rpc) )
    return -1;
  rpc_file = address_to_file_and_line(rpc, &pline, 0, &pdelta, &ppath);
  if ( !rpc_file )
  {
    rpc_file = "";
    pline = 0;
  }
  if ( ac > 0 )
  {
    aca = ac - 1;
    ava = av + 1;
    if ( aca > 0 && **ava == 45 )
      return ds_error("Usage: list [<adr> [<cnt> [<back>]]]");
    adr = rpc;
    back = 10;
    cnt = 21;
    if ( aca > 0 )
    {
      if ( ds_eval_word(*ava, &adr) )
        return -1;
      if ( aca > 1 )
      {
        if ( ds_eval_word(ava[1], (unsigned int *)&cnt) )
          return -1;
        if ( aca > 2 )
        {
          if ( ds_eval_word(ava[2], (unsigned int *)&back) )
            return -1;
          if ( aca > 3 )
            return ds_error("Usage: list [<adr> [<cnt> [<back>]]]");
        }
      }
    }
    file = address_to_file_and_line(adr, &line, 0, &delta, &ppath);
    if ( !file || line <= 0 )
      return ds_error("*** No line for 0x%W", adr);
    ds_printf(" %s:%d\n", file, line);
  }
  else
  {
    file = next_file_27;
    if ( !next_file_27 )
      return 0;
    line = next_line_28;
    if ( next_line_28 <= 0 )
      return 0;
    ppath = next_obj_path_30;
    back = 0;
    cnt = next_cnt_29;
  }
  next_cnt_29 = cnt;
  for ( line -= back; ; ++line )
  {
    v3 = cnt--;
    if ( v3 <= 0 )
      break;
    if ( line > 0 )
    {
      v15 = string_by_file_and_line(file, line, ppath);
      if ( v15 )
      {
        v6 = v15;
        v5 = line;
        if ( !strcmp(rpc_file, file) && pline == line )
          v4 = "->";
        else
          v4 = "  ";
        ds_printf("%s%5d %s\n", v4, v5, v6);
      }
    }
  }
  next_file_27 = file;
  next_line_28 = line;
  next_obj_path_30 = ppath;
  return 0;
}

