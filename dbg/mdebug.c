
#include "mdebug.h"

#include "dsxdb_prototypes.h"
#include "list.h"

static LIST_HEAD(mdebug_list);

static char *cur_fname = NULL;
static char *cur_buf = NULL;
static int cur_size = 0;

static void print_sym(SYM *p, char *strs);
static char *set_abs_path(char *fname);
static void clear_source_line_buffer();
static char *search_source_file(char *fname, char *obj_path, char *src_dirs);

void clear_mdebug()
{
  MDEBUG *md;

  list_for_each_safe(md, &mdebug_list, list) {
    list_remove(&md->list);

    ds_free(md->shdr);
    ds_free(md->fdt_adrs);
    ds_free(md->pdt_adrs);
    ds_free(md->path);
    ds_free(md);
  }

  clear_source_line_buffer();
}

void clear_mdebug_with_id(int id)
{
  MDEBUG *md;

  list_for_each_safe(md, &mdebug_list, list) {
    if (md->id == id) {
      list_remove(&md->list);

      ds_free(md->shdr);
      ds_free(md->fdt_adrs);
      ds_free(md->pdt_adrs);
      ds_free(md->path);
      ds_free(md);
    }
  }

  clear_source_line_buffer();
}

static void print_sym(SYM *p, char *strs)
{
  char *st;
  char *sc;
  char *stab;

  ds_printf(" 0x%W", p->value);
  switch ( p->st )
  {
    case ST_NIL:
      st = "Nil";
      break;
    case ST_GLOBAL:
      st = "Global";
      break;
    case ST_STATIC:
      st = "Static";
      break;
    case ST_PARAM:
      st = "Param";
      break;
    case ST_LOCAL:
      st = "Local";
      break;
    case ST_LABEL:
      st = "Label";
      break;
    case ST_PROC:
      st = "Proc";
      break;
    case ST_BLOCK:
      st = "Block";
      break;
    case ST_END:
      st = "End";
      break;
    case ST_MEMBER:
      st = "Member";
      break;
    case ST_TYPEDEF:
      st = "Typedef";
      break;
    case ST_FILE_SYMBOL:
      st = "File";
      break;
    case ST_STATICPROC:
      st = "StaticProc";
      break;
    case ST_CONSTANT:
      st = "Constant";
      break;
    default:
      st = 0;
      break;
  }

  if ( st )
    ds_printf(" %-10s", st);
  else
    ds_printf(" %10d", p->st);

  switch ( p->sc )
  {
    case SC_NIL:
      sc = "Nil";
      break;
    case SC_TEXT:
      sc = "Text";
      break;
    case SC_DATA:
      sc = "Data";
      break;
    case SC_BSS:
      sc = "Bss";
      break;
    case SC_REGISTER:
      sc = "Register";
      break;
    case SC_ABS:
      sc = "Abs";
      break;
    case SC_UNDEFINED:
      sc = "Undefined";
      break;
    case SC_LOCAL:
      sc = "Local";
      break;
    case SC_BITS:
      sc = "Bits";
      break;
    case SC_DBX:
      sc = "Dbx";
      break;
    case SC_REG_IMAGE:
      sc = "RegImage";
      break;
    case SC_INFO:
      sc = "Info";
      break;
    case SC_USER_STRUCT:
      sc = "UserStruct";
      break;
    case SC_SDATA:
      sc = "SData";
      break;
    case SC_SBSS:
      sc = "SBss";
      break;
    case SC_RDATA:
      sc = "RData";
      break;
    case SC_VAR:
      sc = "Var";
      break;
    case SC_COMMON:
      sc = "Common";
      break;
    case SC_SCOMMON:
      sc = "SCommon";
      break;
    case SC_VAR_REGISTER:
      sc = "VarRegister";
      break;
    case SC_VARIANT:
      sc = "Variant";
      break;
    case SC_SUNDEFINED:
      sc = "SUndefined";
      break;
    default:
      sc = 0;
      break;
  }

  if ( sc )
    ds_printf(" %-11s", sc);
  else
    ds_printf(" %11d", p->sc);

  ds_printf(" 0x%05x", p->index);

  stab = 0;
  // is STAB
  if ( (p->index & 0xFFF00) == 0x8f300)
  {
    switch ( p->index -  0x8f300 )
    {
      case SC_STAB:
        stab = "UNDF";
        break;
      case SC_N_GSYM:
        stab = "GSYM";
        break;
      case SC_N_FNAME:
        stab = "FNAME";
        break;
      case SC_N_FUN:
        stab = "FUN";
        break;
      case SC_N_STSYM:
        stab = "STSYM";
        break;
      case SC_N_LCSYM:
        stab = "LCSYM";
        break;
      case SC_N_MAIN:
        stab = "MAIN";
        break;
      case SC_N_ROSYM:
        stab = "ROSYM";
        break;
      case SC_N_PC:
        stab = "PC";
        break;
      case SC_N_NSYMS:
        stab = "NSYMS";
        break;
      case SC_N_NOMAP:
        stab = "NOMAP";
        break;
      case SC_N_OBJ:
        stab = "OBJ";
        break;
      case SC_N_OPT:
        stab = "OPT";
        break;
      case SC_N_RSYM:
        stab = "RSYM";
        break;
      case SC_N_M2C:
        stab = "M2C";
        break;
      case SC_N_SLINE:
        stab = "SLINE";
        break;
      case SC_N_DSLINE:
        stab = "DSLINE";
        break;
      case SC_N_BSLINE:
        stab = "BSLINE";
        break;
      case SC_N_EFD:
        stab = "DEFD";
        break;
      case SC_N_FLINE:
        stab = "FLINE";
        break;
      case SC_N_EHDECL:
        stab = "EHDECL";
        break;
      case SC_N_CATCH:
        stab = "CATCH";
        break;
      case SC_N_SSYM:
        stab = "SSYM";
        break;
      case SC_N_ENDM:
        stab = "ENDM";
        break;
      case SC_N_SO:
        stab = "SO";
        break;
      case SC_N_ALIAS:
        stab = "ALIAS";
        break;
      case SC_N_LSYM:
        stab = "LSYM";
        break;
      case SC_N_BINCL:
        stab = "BINCL";
        break;
      case SC_N_SOL:
        stab = "SOL";
        break;
      case SC_N_PSYM:
        stab = "PSYM";
        break;
      case SC_N_EINCL:
        stab = "EINCL";
        break;
      case SC_N_ENTRY:
        stab = "ENTRY";
        break;
      case SC_N_LBRAC:
        stab = "LBRAC";
        break;
      case SC_N_EXCL:
        stab = "EXCL";
        break;
      case SC_N_SCOPE:
        stab = "SCOPE";
        break;
      case SC_N_RBRAC:
        stab = "RBRAC";
        break;
      case SC_N_BCOMM:
        stab = "BCOMM";
        break;
      case SC_N_ECOMM:
        stab = "ECOMM";
        break;
      case SC_N_ECOML:
        stab = "ECOML";
        break;
      case SC_N_WITH:
        stab = "WITH";
        break;
      case SC_N_NBTEXT:
        stab = "NBTEXT";
        break;
      case SC_N_NBDATA:
        stab = "NBDATA";
        break;
      case SC_N_NBBSS:
        stab = "NBBSS";
        break;
      case SC_N_NBSTS:
        stab = "NBSTS";
        break;
      case SC_N_NBLCS:
        stab = "NBLCS";
        break;
      case SC_N_LENG:
        stab = "LENG";
        break;
      default:
        stab = 0;
        break;
    }
  }

  if ( stab )
    ds_printf(" %-6s", stab);
  else
    ds_printf(" %6s", "");

  ds_printf(" %s\n", &strs[p->iss]);
}

int show_mdebug(int ac, char **av)
{
  int n;
  char *str = NULL;
  MDEBUG *md;

  if ( ac <= 0 )
    return 0;

  ac--;
  av++;

  if ( ac > 0 && **av == '-' )
    return ds_error("Usage: show mdebug [<str>]");

  if ( ac > 0 )
  {
    str = *av;
    n = strlen(*av);
    if ( ac > 1 )
      return ds_error("Usage: show mdebug [<str>]");
  }

  list_for_each(md, &mdebug_list, list) {
    ds_printf("<<%s>>\n", md->path);
    for ( SYM* sym = md->lsyms; md->lsyme > sym; ++sym )
    {
      if ( match_symbol(str, n, &md->lstrs[sym->iss]) )
      {
        ds_printf(" L");
        print_sym(sym, md->lstrs);
      }
    }

    for ( EXT_SYM* esym = md->esyms; md->esyme > esym; ++esym )
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

static char *set_abs_path(char *fname)
{
  size_t v2;
  char *r;
  char path[1024];

  if ( !ds_abs_path(path, 1024, fname) )
    return 0;

  r = ds_alloc(strlen(path) + 1 );

  if ( r )
    return strcpy(r, path);

  return 0;
}

int load_mdebug(
        void *stream,
        DS_ELF_EHDR *elf_header,
        DS_ELF_SHDR *section_header,
        int mindex,
        int id,
        int base,
        char *path)
{
  unsigned int pdt_eadr;
  unsigned int pdt_sadr;
  unsigned int fdt_eadr;
  unsigned int fdt_sadr;
  SYM *syme;
  SYM *syms;
  SYM *sym;
  PDT *pdte;
  PDT *pdt;
  MDEBUG *md;
  SYM_HDR* shdr;
  unsigned int size;
  unsigned offset;
  int ida;

#ifdef DSNET_COMPILING_E
  ida = look_eemod(stream, elf_header, section_header, id, base, clear_mdebug_with_id);
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  ida = look_iopmod(stream, elf_header, section_header, id, base, clear_mdebug_with_id);
#endif /* DSNET_COMPILING_I */
  if ( ida < 0 )
    return -1;

  offset = section_header[mindex].offset;
  size = section_header[mindex].size;

  shdr = ds_fload(stream, 0, offset, size, 1);
  if ( !shdr )
    return -1;

  md = ds_alloc(sizeof(MDEBUG));
  if ( !md )
  {
    ds_free(shdr);
    return -1;
  }

  ds_bzero(md, sizeof(MDEBUG));

  md->path = set_abs_path(path);
  if ( !md->path )
    goto error;

  md->shdr = (SYM_HDR *)shdr;
  md->pdt_adrs = NULL;
  md->fdt_adrs = NULL;

  if ( shdr->cbLine )
  {
    md->lins = (unsigned char*)shdr + shdr->cblineOffset - offset;
    md->line = &md->lins[shdr->cbLine];
    if ( (ptrdiff_t)shdr > (ptrdiff_t)md->lins || (ptrdiff_t)md->line > (ptrdiff_t)shdr + size )
    {
      ds_error("invalid cblineOffset,cbLine");
      goto error;
    }
  }
  else
  {
    md->line = 0;
    md->lins = 0;
  }

  if ( shdr->ipdMax )
  {
    md->pdts = (PDT *)((unsigned char*)shdr + shdr->cbPdOffset - offset);
    md->pdte = &md->pdts[shdr->ipdMax];
    if ( (ptrdiff_t)shdr > (ptrdiff_t)md->pdts || (ptrdiff_t)md->pdte > (ptrdiff_t)shdr + size )
    {
      ds_error("invalid cbPdOffset,ipdMax");
      goto error;
    }
  }
  else
  {
    md->pdte = 0;
    md->pdts = 0;
  }

  for ( PDT *pdt = md->pdts; md->pdte > pdt; ++pdt )
    ;

  if ( shdr->isymMax )
  {
    md->lsyms = (SYM *)((unsigned char*)shdr + shdr->cbSymOffset - offset);
    md->lsyme = &md->lsyms[shdr->isymMax];
    if ( (ptrdiff_t)shdr > (ptrdiff_t)md->lsyms || (ptrdiff_t)md->lsyme > (ptrdiff_t)shdr + size )
    {
      ds_error("invalid cbSymOffset,isymMax");
      goto error;
    }
  }
  else
  {
    md->lsyme = 0;
    md->lsyms = 0;
  }

  for ( SYM *i = md->lsyms; md->lsyme > i; ++i )
    ;

  if ( shdr->issMax )
  {
    md->lstrs = (char *)shdr + shdr->cbSsOffset - offset;
    md->lstre = &md->lstrs[shdr->issMax];
    if ( (ptrdiff_t)shdr > (ptrdiff_t)md->lstrs || (ptrdiff_t)md->lstre > (ptrdiff_t)shdr + size )
    {
      ds_error("invalid cbSsOffset,issMax");
      goto error;
    }
  }
  else
  {
    md->lstre = 0;
    md->lstrs = 0;
  }

  if ( shdr->iextMax )
  {
    md->esyms = (EXT_SYM *)((char*)shdr + shdr->cbExtOffset - offset);
    md->esyme = &md->esyms[shdr->iextMax];
    if ( (ptrdiff_t)shdr > (ptrdiff_t)md->esyms || (ptrdiff_t)md->esyme > (ptrdiff_t)shdr + size )
    {
      ds_error("invalid cbExtOffset,iextMax");
      goto error;
    }
  }
  else
  {
    md->esyme = 0;
    md->esyms = 0;
  }

  for ( EXT_SYM *esym = md->esyms; md->esyme > esym; ++esym )
    ;

  if ( shdr->issExtMax )
  {
    md->estrs = (char *)shdr + shdr->cbSsExtOffset - offset;
    md->estre = &md->estrs[shdr->issExtMax];
    if ( (ptrdiff_t)shdr > (ptrdiff_t)md->estrs || (ptrdiff_t)md->estre > (ptrdiff_t)shdr + size )
    {
      ds_error("invalid cbSsExtOffset,issExtMax");
      goto error;
    }
  }
  else
  {
    md->estre = 0;
    md->estrs = 0;
  }

  if ( shdr->ifdMax )
  {
    md->fdts = (FDT *)((char*)shdr + shdr->cbFdOffset - offset);
    md->fdte = &md->fdts[shdr->ifdMax];
    if ( (ptrdiff_t)shdr > (ptrdiff_t)md->fdts || (ptrdiff_t)md->fdte > (ptrdiff_t)shdr + size )
    {
      ds_error("invalid cbFdOffset,ifdMax");
      goto error;
    }
  }
  else
  {
    md->fdte = 0;
    md->fdts = 0;
  }

  for ( FDT *fdt = md->fdts; md->fdte > fdt; ++fdt )
    ;

  md->fdt_adrs = ds_alloc(sizeof(ADRS) * shdr->ifdMax);

  if ( !md->fdt_adrs )
    goto error;

  md->pdt_adrs = ds_alloc(sizeof(ADRS) * shdr->ipdMax);

  if ( !md->pdt_adrs)
    goto error;

  for ( FDT *fdt = md->fdts; fdt < md->fdte ; ++fdt )
  {
    fdt_eadr = fdt->adr;
    fdt_sadr = fdt->adr;
    pdt = &md->pdts[fdt->ipdFirst];
    pdte = &pdt[fdt->cpd];

    while ( pdte > pdt )
    {
      pdt_eadr = pdt->adr + fdt->adr;
      pdt_sadr = pdt_eadr;
      syme = 0;
      syms = 0;
      for ( sym = &md->lsyms[fdt->isymBase + pdt->isym]; md->lsyme > sym; ++sym )
      {
        if ( !syms && (sym->st == ST_PROC || sym->st == ST_STATICPROC) )
        {
          syms = sym;
        }
        else if ( sym->st == ST_END )
        {
          syme = sym;
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

      md->pdt_adrs[((ptrdiff_t)pdt - (ptrdiff_t)md->pdts) / sizeof(*pdt)].sadr = pdt_sadr;
      md->pdt_adrs[((ptrdiff_t)pdt - (ptrdiff_t)md->pdts) / sizeof(*pdt)].eadr = pdt_eadr;

      if ( fdt_sadr > pdt_sadr )
        fdt_sadr = pdt_sadr;

      if ( pdt_eadr > fdt_eadr )
        fdt_eadr = pdt_eadr;

      ++pdt;
    }

    md->fdt_adrs[((ptrdiff_t)fdt - (ptrdiff_t)md->fdts) / sizeof(*fdt)].sadr = fdt_sadr;
    md->fdt_adrs[((ptrdiff_t)fdt - (ptrdiff_t)md->fdts) / sizeof(*fdt)].eadr = fdt_eadr;

  }

  md->id = ida;
  md->base = base;

  list_insert(&mdebug_list, &md->list);
  clear_source_line_buffer();

  return 0;

error:
  ds_free(md->shdr);
  ds_free(md->fdt_adrs);
  ds_free(md->pdt_adrs);
  ds_free(md->path);
  ds_free(md);

  return -1;
}

unsigned int file_and_line_to_address(int line, char *path)
{
  int base;
  SYM *sym;
  FDT *fdte;
  FDT *fdt;
  MDEBUG *md;

  list_for_each(md, &mdebug_list, list) {
    base = module_base(md->id, md->base, 0xff1f, 0);
    fdt = md->fdts;
    fdte = &fdt[md->shdr->ifdMax];

    for (fdt = md->fdts; fdt < fdte; fdt++) {
      if ( strcmp(&md->lstrs[fdt->issBase + fdt->rss], path) != 0) {
        continue;
      }

      for ( sym = &md->lsyms[fdt->isymBase]; &md->lsyms[fdt->isymBase + fdt->csym] > sym; ++sym )
      {
        if ( sym->st == ST_LABEL && sym->sc == SC_TEXT && !(sym->index & 0x80000) && sym->index == line)
        {
          return sym->value + base;
        }
      }
    }
  }

  return 0;
}

char *address_to_file_and_line(unsigned int loc, int *pline, int *f_has, int *pdelta, char **ppath)
{
  unsigned int value; // [esp+4h] [ebp-44h]
  unsigned int pdt_eadr; // [esp+Ch] [ebp-3Ch]
  unsigned int pdt_sadr; // [esp+10h] [ebp-38h]
  int delta; // [esp+14h] [ebp-34h]
  unsigned int line; // [esp+18h] [ebp-30h]
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

  list_for_each(md, &mdebug_list, list) {
    base = module_base(md->id, md->base, 0xff1f, 0);
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
          if ( sym->st == ST_LABEL && sym->sc == SC_TEXT && !(sym->index & 0x80000) )
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
            line = sym->index;
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

  if (list_entry_is_head(md, &mdebug_list, list)) {
      return NULL;
  }

  if ( pline )
    *pline = line;

  if ( ppath )
    *ppath = md->path;

  return &md->lstrs[fdt->issBase + fdt->rss];
}

int symbol_to_value_by_mdebug(char *file, char *name, unsigned int *pv)
{
  int v4;
  unsigned int rpc;
  int delta;
  int index;
  int line = 0;
  SYM *syme;
  SYM *sym;
  FDT *fdt;
  MDEBUG *md;

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

  list_for_each(md, &mdebug_list, list) {
    for ( fdt = md->fdts; md->fdte > fdt && strcmp(file, &md->lstrs[fdt->issBase + fdt->rss]); ++fdt )
      ;

    if ( fdt < md->fdte ) {
      break;
    }
  }

  if (list_entry_is_head(md, &mdebug_list, list)) {
    return NULL;
  }

  sym = &md->lsyms[fdt->isymBase];
  syme = &md->lsyms[fdt->isymBase + fdt->csym];
  if ( line )
  {
    while ( syme > sym )
    {
      if ( sym->st == ST_LABEL && sym->sc == SC_TEXT )
      {
        index = sym->index;
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
      if ( (sym->st == ST_PROC || sym->st == ST_STATICPROC ) && !strcmp(name, &md->lstrs[sym->iss]))
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

static void clear_source_line_buffer()
{
  cur_fname = (char *)ds_free(cur_fname);
  cur_buf = (char *)ds_free(cur_buf);
  cur_size = 0;
}

static char *search_source_file(char *fname, char *obj_path, char *src_dirs)
{
  char *q; // [esp+8h] [ebp-808h]
  char *q_1; // [esp+8h] [ebp-808h]
  char *p; // [esp+Ch] [ebp-804h]
  char *p_1; // [esp+Ch] [ebp-804h]
  char tmp[1024]; // [esp+10h] [ebp-800h] BYREF
  char obj_dir[1024]; // [esp+410h] [ebp-400h] BYREF
  static char path[1024];

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
    q = path;
    while ( *p_1 && *p_1 != 58 )
      *q++ = *p_1++;
    if ( *p_1 == 58 )
      ++p_1;
    *q = 0;
    if ( path[0] != 64 )
      goto LABEL_25;
    if ( obj_path )
    {
      strcpy(tmp, &path[1]);
      strcpy(path, obj_dir);
      q_1 = &path[strlen(path)];
      strcpy(q_1, tmp);
      q = &q_1[strlen(tmp)];
LABEL_25:
      if ( q > path && *(q - 1) != 47 )
        *q++ = 47;
      strcpy(q, fname);
      if ( !ds_access(path, 1) )
        return path;
    }
  }
  return 0;
}

char *string_by_file_and_line(char *fname, int line, char *obj_path)
{
  char *v5; // eax
  char *srcdir; // [esp-4h] [ebp-1Ch]
  char *ret; // [esp+0h] [ebp-18h]
  char *q; // [esp+4h] [ebp-14h]
  char *pe; // [esp+8h] [ebp-10h]
  char *p; // [esp+Ch] [ebp-Ch]
  int i; // [esp+10h] [ebp-8h]
  void *stream; // [esp+14h] [ebp-4h]
  char *srcfile; // [esp+20h] [ebp+8h]

  if ( !fname || !line )
    return 0;

  srcdir = source_directories_str();
  srcfile = search_source_file(fname, obj_path, srcdir);

  if ( !srcfile )
    return 0;

  if ( !cur_fname || !cur_buf || strcmp(cur_fname, srcfile) != 0 )
  {
    clear_source_line_buffer();
    stream = ds_fopen_low(srcfile, "r");

    if ( !stream )
        return 0;

    if ( ds_fsize(srcfile, &cur_size) < 0 )
        return 0;

    cur_fname = ds_alloc(strlen(srcfile) + 1);

    if ( !cur_fname )
        return 0;

    strcpy(cur_fname, srcfile);
    cur_buf = ds_fload(stream, 0, 0, 1, cur_size);
    ds_fclose(stream);

    if ( !cur_buf )
        return 0;
  }

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

  ret = ds_alloc(q - p + 1);

  if ( !ret )
    return 0;

  memcpy(ret, p, q - p);
  ret[q - p] = 0;

  return ret;
}

int list_cmd(int ac, char **av)
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
  static char *next_file = NULL;
  static int next_cnt = 0;
  static char *next_obj_path = NULL;
  static int next_line = 0;

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
    ac--;
    av++;

    if ( ac > 0 && **av == '-' )
      return ds_error("Usage: list [<adr> [<cnt> [<back>]]]");
    adr = rpc;
    back = 10;
    cnt = 21;
    if ( ac > 0 )
    {
      if ( ds_eval_word(*av, &adr) )
        return -1;

      if ( ac > 1 )
      {
        if ( ds_eval_word(av[1], &cnt) )
          return -1;

        if ( ac > 2 )
        {
          if ( ds_eval_word(av[2], &back) )
            return -1;

          if ( ac > 3 )
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
    file = next_file;
    if ( !next_file )
      return 0;
    line = next_line;
    if ( next_line <= 0 )
      return 0;
    ppath = next_obj_path;
    back = 0;
    cnt = next_cnt;
  }

  next_cnt = cnt;
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

  next_file = file;
  next_line = line;
  next_obj_path = ppath;
  return 0;
}

