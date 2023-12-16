#include "dsnet_prototypes.h"

static char *tags[9];
static char *tage[9];

static struct __anon_struct_141 {RERP *head; RERP *tail;} rerps = { NULL, NULL };

static void *__cdecl ds_new(int size);
static void *__cdecl ds_free_re(RE *re);
static void *__cdecl ds_free_rerp(RERP *rerp);
static char *__cdecl ds_alloc_rp(int ac, char **av);
static char *__cdecl ds_alloc_str(char *str);
static RE *__cdecl ds_compile_re(char *str);
static char *__cdecl ds_match_re(RE *re, char *str);

static void *__cdecl ds_new(int size)
{
  void *r; // [esp+0h] [ebp-4h]

  r = ds_alloc_mem_low("subst.c", "_ds_new", size);
  if ( !r )
    return 0;
  ds_bzero(r, size);
  return r;
}

static void *__cdecl ds_free_re(RE *re)
{
  if ( re )
  {
    ds_free_re(re->next);
    ds_free_re(re->sub);
    ds_free_mem_low(re->class_, "subst.c", "ds_free_re");
    ds_free_mem_low(re, "subst.c", "ds_free_re");
  }
  return 0;
}

static void *__cdecl ds_free_rerp(RERP *rerp)
{
  ds_free_re(rerp->re);
  ds_free_mem_low(rerp->re_str, "subst.c", "ds_free_rerp");
  ds_free_mem_low(rerp->rp, "subst.c", "ds_free_rerp");
  ds_free_mem_low(rerp, "subst.c", "ds_free_rerp");
  return 0;
}

static char *__cdecl ds_alloc_rp(int ac, char **av)
{
  int v2; // eax
  char *v4; // eax
  char v5; // dl
  int n; // [esp+4h] [ebp-10h]
  int i; // [esp+8h] [ebp-Ch]
  int i_1; // [esp+8h] [ebp-Ch]
  char *q; // [esp+Ch] [ebp-8h]
  char *p; // [esp+10h] [ebp-4h]

  n = 0;
  for ( i = 0; ac > i; ++i )
    n += strlen(av[i]) + 1;
  v2 = n;
  if ( !n )
    v2 = 1;
  p = (char *)ds_alloc_mem_low("subst.c", "ds_alloc_rp", v2);
  if ( !p )
    return 0;
  if ( !n )
    *p = 0;
  q = p;
  for ( i_1 = 0; ac > i_1; ++i_1 )
  {
    strcpy(q, av[i_1]);
    v4 = &q[strlen(av[i_1])];
    q = v4 + 1;
    if ( i_1 >= ac - 1 )
      v5 = 0;
    else
      v5 = 32;
    *v4 = v5;
  }
  return p;
}

static char *__cdecl ds_alloc_str(char *str)
{
  char *p; // [esp+0h] [ebp-8h]
  int n; // [esp+4h] [ebp-4h]

  n = strlen(str);
  if ( *str == 34 )
  {
    if ( n <= 1 || str[n - 1] != 34 )
    {
      ds_error("ds_alloc_str: invalid string - %s", str);
      return 0;
    }
    ++str;
    n -= 2;
  }
  p = (char *)ds_alloc_mem_low("subst.c", "ds_alloc_str", n + 1);
  if ( !p )
    return 0;
  if ( n > 0 )
    memcpy(p, str, n);
  p[n] = 0;
  return p;
}

static RE *__cdecl ds_compile_re(char *str)
{
  CLASS *v2; // eax
  CLASS *v3; // eax
  int v4; // eax
  int v5; // edx
  int v6; // eax
  CLASS *_p; // [esp+2Ch] [ebp-34h]
  CLASS *_p_3; // [esp+2Ch] [ebp-34h]
  int ctag; // [esp+34h] [ebp-2Ch]
  int ntag; // [esp+38h] [ebp-28h]
  int f_neg; // [esp+3Ch] [ebp-24h]
  int d; // [esp+40h] [ebp-20h]
  int c; // [esp+44h] [ebp-1Ch]
  int c_1; // [esp+44h] [ebp-1Ch]
  int i; // [esp+48h] [ebp-18h]
  RE *t; // [esp+4Ch] [ebp-14h]
  RE *r; // [esp+50h] [ebp-10h]
  RE *v18; // [esp+54h] [ebp-Ch]
  RE *p; // [esp+58h] [ebp-8h]
  char *s; // [esp+5Ch] [ebp-4h]

  ntag = 0;
  ctag = -1;
  s = str;
  v18 = 0;
  p = 0;
  while ( 1 )
  {
    c = *s;
    if ( !*s )
      break;
    r = (RE *)ds_new(sizeof(RE));
    if ( !r )
      return (RE *)ds_free_re(p);
    r->type = c;
    if ( c == 91 )
    {
      v2 = (CLASS *)ds_new(sizeof(CLASS));
      r->class_ = v2;
      if ( !v2 )
        goto LABEL_14;
      f_neg = 0;
      c_1 = *++s;
      if ( c_1 == 94 )
      {
        f_neg = 1;
        ++s;
      }
      while ( *s && *s != 93 )
      {
        if ( *s == 92 )
        {
          c_1 = *++s;
          if ( !*s )
            goto LABEL_14;
        }
        if ( *s && *s == 45 )
        {
          d = *++s;
          if ( d == 92 )
          {
            d = *++s;
            if ( !*s )
              goto LABEL_14;
          }
          for ( i = c_1; d >= i; ++i )
            r->class_->bits[i >> 5] |= 1 << (i & 0x1F);
        }
        else
        {
          r->class_->bits[*s >> 5] |= 1 << (*s & 0x1F);
        }
        ++s;
      }
      if ( *s != 93 )
        goto LABEL_14;
      if ( f_neg )
      {
        _p = r->class_;
        _p->bits[0] = ~_p->bits[0];
        _p->bits[1] = ~_p->bits[1];
        _p->bits[2] = ~_p->bits[2];
        _p->bits[3] = ~_p->bits[3];
        _p->bits[4] = ~_p->bits[4];
        _p->bits[5] = ~_p->bits[5];
        _p->bits[6] = ~_p->bits[6];
        _p->bits[7] = ~_p->bits[7];
        r->class_->bits[0] &= ~0x400u;
      }
      r->class_->bits[0] &= ~1u;
    }
    else if ( c > 91 )
    {
      if ( c != 92 )
      {
        if ( c == 94 && s <= str )
          goto LABEL_56;
        goto LABEL_55;
      }
      v4 = *++s;
      c = v4;
      r->type = v4;
      if ( v4 == 40 )
      {
        v5 = ntag;
        ctag = ntag;
        r->val = ntag++;
        if ( v5 > 9 )
        {
          ds_error("too many tag open");
LABEL_14:
          ds_free_re(r);
          return (RE *)ds_free_re(p);
        }
      }
      else
      {
        if ( v4 <= 40 )
        {
          if ( !v4 )
            goto LABEL_14;
LABEL_55:
          r->type = 88;
          r->val = c;
          goto LABEL_56;
        }
        if ( v4 != 41 )
          goto LABEL_55;
        v6 = ctag;
        r->val = ctag--;
        if ( v6 < 0 )
        {
          ds_error("too many tag close");
          goto LABEL_14;
        }
      }
    }
    else if ( c == 36 )
    {
      if ( s[1] )
        goto LABEL_55;
    }
    else
    {
      if ( c != 46 )
        goto LABEL_55;
      r->type = 91;
      v3 = (CLASS *)ds_new(sizeof(CLASS));
      r->class_ = v3;
      if ( !v3 )
        goto LABEL_14;
      _p_3 = r->class_;
      _p_3->bits[0] = ~_p_3->bits[0];
      _p_3->bits[1] = ~_p_3->bits[1];
      _p_3->bits[2] = ~_p_3->bits[2];
      _p_3->bits[3] = ~_p_3->bits[3];
      _p_3->bits[4] = ~_p_3->bits[4];
      _p_3->bits[5] = ~_p_3->bits[5];
      _p_3->bits[6] = ~_p_3->bits[6];
      _p_3->bits[7] = ~_p_3->bits[7];
      r->class_->bits[0] &= ~0x400u;
      r->class_->bits[0] &= ~1u;
    }
LABEL_56:
    if ( (r->type == 88 || r->type == 91) && s[1] == 42 )
    {
      t = (RE *)ds_new(sizeof(RE));
      if ( !t )
        return (RE *)ds_free_re(p);
      t->type = *++s;
      t->sub = r;
      r = t;
    }
    if ( p )
      v18->next = r;
    else
      p = r;
    v18 = r;
    ++s;
  }
  if ( ctag >= 0 )
  {
    ds_error("not enough tag close");
    goto LABEL_14;
  }
  return p;
}

static char *__cdecl ds_match_re(RE *re, char *str)
{
  int v2; // eax
  int _val; // [esp+8h] [ebp-14h]
  char *r; // [esp+10h] [ebp-Ch]
  char *r_1; // [esp+10h] [ebp-Ch]
  char *t; // [esp+14h] [ebp-8h]
  char *s; // [esp+18h] [ebp-4h]

  s = str;
  while ( 2 )
  {
    if ( !re )
      return s;
    switch ( re->type )
    {
      case '$':
        if ( !*s )
          goto LABEL_26;
        return 0;
      case '(':
        tags[re->val] = s;
        goto LABEL_26;
      case ')':
        tage[re->val] = s;
        goto LABEL_26;
      case '*':
        for ( t = s; ; t = r )
        {
          r = ds_match_re(re->sub, t);
          if ( !r )
            break;
        }
        break;
      case 'X':
        v2 = *s++;
        if ( re->val == v2 )
          goto LABEL_26;
        return 0;
      case '[':
        _val = *s++;
        if ( (re->class_->bits[_val >> 5] & (1 << (_val & 0x1F))) != 0 )
          goto LABEL_26;
        return 0;
      case '^':
        if ( s <= str )
          goto LABEL_26;
        return 0;
      default:
LABEL_26:
        re = re->next;
        continue;
    }
    break;
  }
  while ( t >= s )
  {
    r_1 = ds_match_re(re->next, t);
    if ( r_1 )
      return r_1;
    --t;
  }
  return 0;
}

char *__cdecl ds_subst(char *dst, int dst_siz, char *src)
{
  int c; // [esp+4h] [ebp-424h]
  char tmp[1024]; // [esp+8h] [ebp-420h] BYREF
  char *q; // [esp+408h] [ebp-20h]
  char *i; // [esp+40Ch] [ebp-1Ch]
  char *me; // [esp+410h] [ebp-18h]
  char *ms; // [esp+414h] [ebp-14h]
  char *sp; // [esp+418h] [ebp-10h]
  char *de; // [esp+41Ch] [ebp-Ch]
  char *dp; // [esp+420h] [ebp-8h]
  RERP *rerp; // [esp+424h] [ebp-4h]

  de = &dst[dst_siz];
  ds_bzero(tags, sizeof(tags));
  ds_bzero(tage, sizeof(tage));
  for ( rerp = rerps.head; rerp; rerp = rerp->forw )
  {
    sp = src;
    dp = dst;
    while ( 1 )
    {
      for ( ms = sp; *ms; ++ms )
      {
        me = ds_match_re(rerp->re, ms);
        if ( me )
          break;
      }
      if ( !*ms )
        break;
      while ( dp < de - 1 && ms > sp )
        *dp++ = *sp++;
      for ( i = rerp->rp; *i; ++i )
      {
        if ( *i == 38 )
        {
          for ( q = ms; me > q; ++q )
          {
            if ( dp < de - 1 )
              *dp++ = *q;
          }
        }
        else if ( *i == 92 && i[1] )
        {
          c = i[1] - 49;
          if ( c < 0 || c > 8 )
          {
            if ( dp < de - 1 )
              *dp++ = i[1];
          }
          else
          {
            for ( q = tags[c]; tage[c] > q; ++q )
            {
              if ( dp < de - 1 )
                *dp++ = *q;
            }
          }
          ++i;
        }
        else if ( dp < de - 1 )
        {
          *dp++ = *i;
        }
      }
      ds_bzero(tags, sizeof(tags));
      ds_bzero(tage, sizeof(tage));
      sp = me;
    }
    while ( dp < de - 1 && *sp )
    {
      if ( dp < de - 1 )
        *dp++ = *sp;
      ++sp;
    }
    *dp = 0;
    src = strcpy(tmp, dst);
  }
  return strcpy(dst, src);
}

int __cdecl ds_subst_cmd(int ac, char **av)
{
  RERP *tail; // edx
  char *rp; // [esp+Ch] [ebp-20h]
  char *s1; // [esp+10h] [ebp-1Ch]
  RERP *rerp1; // [esp+14h] [ebp-18h]
  RERP *rerp; // [esp+18h] [ebp-14h]
  RERP *rerp_1; // [esp+18h] [ebp-14h]
  RERP *rerp_2; // [esp+18h] [ebp-14h]
  RERP *rerp_3; // [esp+18h] [ebp-14h]
  RE *re; // [esp+1Ch] [ebp-10h]
  int f_r; // [esp+20h] [ebp-Ch]
  int n; // [esp+24h] [ebp-8h]
  int r; // [esp+28h] [ebp-4h]
  int aca; // [esp+34h] [ebp+8h]
  char **ava; // [esp+38h] [ebp+Ch]

  r = -1;
  n = 0;
  f_r = 0;
  re = 0;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  for ( ava = av + 1; aca > 0 && **ava == 45; ++ava )
  {
    if ( strcmp("-r", *ava) )
      return ds_error("Usage: subst [-r] [<pattern> <replace>...]");
    ++f_r;
    --aca;
  }
  if ( aca )
  {
    s1 = ds_alloc_str(*ava);
    if ( s1 )
    {
      rp = ds_alloc_rp(aca - 1, ava + 1);
      if ( rp )
      {
        if ( f_r )
        {
          for ( rerp_2 = rerps.head; rerp_2; rerp_2 = rerp_2->forw )
          {
            if ( !strcmp(s1, rerp_2->re_str) && !strcmp(rp, rerp_2->rp) )
            {
              if ( rerp_2->forw )
                rerp_2->forw->back = rerp_2->back;
              else
                rerps.tail = rerp_2->back;
              if ( rerp_2->back )
                rerp_2->back->forw = rerp_2->forw;
              else
                rerps.head = rerp_2->forw;
              ds_free_rerp(rerp_2);
              ++n;
            }
          }
          ds_printf("subst: %d substitution(s) removed\n", n);
          r = 0;
        }
        else
        {
          re = ds_compile_re(s1);
          if ( re )
          {
            rerp_3 = (RERP *)ds_alloc_mem_low("subst.c", "ds_subst_cmd", 20);
            if ( rerp_3 )
            {
              ds_bzero(rerp_3, sizeof(RERP));
              rerp_3->re_str = s1;
              s1 = 0;
              rerp_3->re = re;
              re = 0;
              rerp_3->rp = rp;
              tail = rerps.tail;
              rerp_3->back = rerps.tail;
              if ( tail )
                rerp_3->back->forw = rerp_3;
              else
                rerps.head = rerp_3;
              rerp_3->forw = 0;
              rerps.tail = rerp_3;
              r = 0;
            }
          }
          else
          {
            ds_error("subst: invalid regular expression - %s", *ava);
          }
        }
      }
    }
    ds_free_mem_low(s1, "subst.c", "ds_subst_cmd");
    ds_free_re(re);
    return r;
  }
  else
  {
    if ( f_r )
    {
      for ( rerp = rerps.head; rerp; rerp = rerp1 )
      {
        rerp1 = rerp->forw;
        ds_free_rerp(rerp);
        ++n;
      }
      rerps.tail = 0;
      rerps.head = 0;
      ds_printf("subst: %d substitution(s) removed\n", n);
    }
    else
    {
      for ( rerp_1 = rerps.head; rerp_1; rerp_1 = rerp_1->forw )
        ds_printf("subst \"%s\" %s\n", rerp_1->re_str, rerp_1->rp);
    }
    return 0;
  }
}

