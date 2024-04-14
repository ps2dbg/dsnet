#include "dsnet_prototypes.h"

static char *__cdecl skipsp(char *s);
static char *__cdecl expr(int prio, char *str, quad *pval);
static char *__cdecl fexpr(int prio, char *str, float *pval);

static char *__cdecl skipsp(char *s)
{
  while ( *s && (isspace(*s)) != 0 )
    ++s;
  return s;
}

static char *__cdecl expr(int prio, char *str, quad *pval)
{
  char *v4; // eax
  int v5; // eax
  size_t v6; // eax
  char *v7; // eax
  int v8; // eax
  int v9; // eax
  unsigned int v10; // esi
  unsigned int v11; // esi
  int v12; // eax
  int v13; // eax
  int v14; // eax
  int v15; // eax
  int v16; // eax
  int v17; // eax
  quad v18; // [esp+Ch] [ebp-16Ch] BYREF
  quad v19; // [esp+1Ch] [ebp-15Ch] BYREF
  quad v20; // [esp+2Ch] [ebp-14Ch] BYREF
  quad v21; // [esp+3Ch] [ebp-13Ch] BYREF
  quad v22; // [esp+4Ch] [ebp-12Ch] BYREF
  quad v23; // [esp+5Ch] [ebp-11Ch] BYREF
  quad v24; // [esp+6Ch] [ebp-10Ch] BYREF
  quad v25; // [esp+7Ch] [ebp-FCh] BYREF
  quad v26; // [esp+8Ch] [ebp-ECh] BYREF
  quad v27; // [esp+9Ch] [ebp-DCh] BYREF
  quad v28; // [esp+ACh] [ebp-CCh] BYREF
  quad v29; // [esp+BCh] [ebp-BCh] BYREF
  quad v30; // [esp+CCh] [ebp-ACh] BYREF
  quad v31; // [esp+DCh] [ebp-9Ch] BYREF
  quad v32; // [esp+ECh] [ebp-8Ch] BYREF
  quad v33; // [esp+FCh] [ebp-7Ch] BYREF
  quad v34; // [esp+10Ch] [ebp-6Ch] BYREF
  quad v35; // [esp+11Ch] [ebp-5Ch] BYREF
  quad qv; // [esp+12Ch] [ebp-4Ch] BYREF
  quad val; // [esp+13Ch] [ebp-3Ch] BYREF
  twin tv; // [esp+14Ch] [ebp-2Ch] BYREF
  unsigned int wv; // [esp+154h] [ebp-24h] BYREF
  unsigned int adr; // [esp+158h] [ebp-20h]
  unsigned __int16 hv; // [esp+15Ch] [ebp-1Ch] BYREF
  unsigned __int8 ptr; // [esp+15Fh] [ebp-19h] BYREF
  int pr; // [esp+160h] [ebp-18h]
  int ch; // [esp+164h] [ebp-14h]
  int nop; // [esp+168h] [ebp-10h]
  int op; // [esp+16Ch] [ebp-Ch]
  char *q; // [esp+170h] [ebp-8h]
  char *p; // [esp+174h] [ebp-4h]
  char *s; // [esp+184h] [ebp+Ch]
  char *sa; // [esp+184h] [ebp+Ch]
  char *sb; // [esp+184h] [ebp+Ch]
  char *sc; // [esp+184h] [ebp+Ch]

  s = skipsp(str);
  if ( !*s )
  {
    ds_printf("unexpected end\n");
    return 0;
  }
  if ( *s == 40 )
  {
    sa = expr(0, s + 1, pval);
    if ( sa )
    {
      v4 = sa;
      s = sa + 1;
      if ( *v4 == 41 )
        goto LABEL_51;
      ds_printf("unblanced '('\n");
      return 0;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    op = *s;
    if ( op != 43 && op != 45 && op != 126 && op != 42 && (op != 33 || s[1] == 61) )
    {
      v6 = strlen(s);
      p = (char *)ds_alloc(v6 + 1);
      if ( !p )
        return 0;
      q = p;
      while ( *s == 36 || *s == 58 || *s == 46 || *s == 95 || (isdigit(*s)) != 0 || (isalpha(*s)) != 0 )
        *q++ = *s++;
      *q = 0;
      if ( *p == 36 )
      {
        if ( ds_ref_user_variable(p + 1, &wv) )
        {
          ds_cwq(&v32, wv);
          *pval = v32;
        }
        else if ( ds_load_quad_reg(p, pval) )
        {
          return (char *)ds_free(p);
        }
      }
      else if ( ds_scan_symbol_or_hex_quad(p, pval) )
      {
        return (char *)ds_free(p);
      }
      ds_free(p);
      goto LABEL_51;
    }
    s = expr(14, s + 1, &val);
    if ( !s )
      return 0;
    if ( op == 43 )
    {
      *pval = val;
      goto LABEL_51;
    }
    if ( op > 43 )
    {
      if ( op == 45 )
      {
        ds_qsub(&v35, ds_qzero, val);
        *pval = v35;
      }
      else if ( op == 126 )
      {
        ds_qneg(&v34, val);
        *pval = v34;
      }
      while ( 1 )
      {
        while ( 1 )
        {
LABEL_51:
          sb = skipsp(s);
          if ( !*sb || *sb == 41 || *sb == 93 )
            return sb;
          if ( *sb != 91 )
            break;
          s = expr(0, sb + 1, &val);
          if ( !s )
            return 0;
          if ( *s == 93 )
          {
LABEL_82:
            ds_cwq(&v34, 2u);
            ds_qshl(&val, val, v34);
            ds_qadd(&val, *pval, val);
            adr = ds_cqw(val);
            if ( ds_load_mem(adr, &wv, 4) )
              return 0;
            ds_cwq(&v34, wv);
            *pval = v34;
          }
          else
          {
            v7 = s;
            sc = s + 1;
            if ( *v7 != 44 )
            {
              ds_printf("missing ','\n");
              return 0;
            }
            if ( !strcmp("b]", sc) )
            {
              ds_cwq(&qv, 1u);
              s = sc + 1;
            }
            else if ( !strcmp("h]", sc) )
            {
              ds_cwq(&qv, 2u);
              s = sc + 1;
            }
            else if ( !strcmp("w]", sc) )
            {
              ds_cwq(&qv, 4u);
              s = sc + 1;
            }
            else if ( !strcmp("d]", sc) )
            {
              ds_cwq(&qv, 8u);
              s = sc + 1;
            }
            else if ( !strcmp("q]", sc) )
            {
              ds_cwq(&qv, 0x10u);
              s = sc + 1;
            }
            else
            {
              s = expr(0, sc, &qv);
              if ( !s )
                return 0;
              if ( *s != 93 )
              {
                ds_printf("unblanced '['\n");
                return 0;
              }
            }
            switch ( ds_cqw(qv) )
            {
              case 1u:
                ds_qadd(&val, *pval, val);
                adr = ds_cqw(val);
                if ( ds_load_mem(adr, &ptr, 1) )
                  return 0;
                ds_cwq(&v32, ptr);
                *pval = v32;
                break;
              case 2u:
                ds_cwq(&v33, 1u);
                ds_qshl(&val, val, v33);
                ds_qadd(&val, *pval, val);
                adr = ds_cqw(val);
                if ( ds_load_mem(adr, &hv, 2) )
                  return 0;
                ds_cwq(&v33, hv);
                *pval = v33;
                break;
              case 4u:
                goto LABEL_82;
              case 8u:
                ds_cwq(&v35, 3u);
                ds_qshl(&val, val, v35);
                ds_qadd(&val, *pval, val);
                adr = ds_cqw(val);
                if ( ds_load_mem(adr, &tv, 8) )
                  return 0;
                ds_ctq(&v35, tv);
                *pval = v35;
                break;
              case 0x10u:
                ds_cwq(&v31, 4u);
                ds_qshl(&val, val, v31);
                ds_qadd(&val, *pval, val);
                adr = ds_cqw(val);
                if ( ds_load_mem(adr, &qv, 16) )
                  return 0;
                *pval = qv;
                break;
              default:
                ds_printf("invalid size\n");
                return 0;
            }
          }
          if ( *s == 93 )
            ++s;
        }
        nop = 1;
        op = *sb;
        switch ( op )
        {
          case '|':
            ch = sb[1] == 124;
            if ( ch )
            {
              op = (op << 8) | ch;
              ++nop;
              v8 = 4;
            }
            else
            {
              v8 = 6;
            }
            pr = v8;
            break;
          case '^':
            pr = 7;
            break;
          case '&':
            ch = sb[1] == 38;
            if ( ch )
            {
              op = (op << 8) | ch;
              ++nop;
              v9 = 5;
            }
            else
            {
              v9 = 8;
            }
            pr = v9;
            break;
          default:
            if ( op == 61 && (ch = sb[1], ch == 61) || op == 33 && (ch = sb[1], ch == 61) )
            {
              op = (op << 8) | ch;
              ++nop;
              pr = 9;
            }
            else
            {
              switch ( op )
              {
                case '<':
                case '>':
                  ch = sb[1];
                  if ( op == ch )
                  {
                    op = (op << 8) | ch;
                    ++nop;
                    pr = 11;
                  }
                  else
                  {
                    if ( sb[1] == 61 )
                    {
                      op = (op << 8) | ch;
                      ++nop;
                    }
                    pr = 10;
                  }
                  break;
                case '+':
                case '-':
                  pr = 12;
                  break;
                case '*':
                case '/':
                case '%':
                  pr = 13;
                  break;
                case ',':
                  return sb;
                default:
                  ds_printf("invalid op - '%c'\n", op);
                  return 0;
              }
            }
            break;
        }
        if ( prio >= pr )
          return sb;
        s = expr(pr, &sb[nop], &val);
        if ( !s )
          return 0;
        if ( op == 94 )
        {
          ds_qxor(&v34, *pval, val);
          *pval = v34;
        }
        else if ( op > 94 )
        {
          if ( op == 15421 )
          {
            v15 = ds_qble(*pval, val);
            ds_cwq(&v27, v15 != 0);
            *pval = v27;
          }
          else if ( op > 15421 )
          {
            if ( op == 15933 )
            {
              v17 = ds_qbge(*pval, val);
              ds_cwq(&v25, v17 != 0);
              *pval = v25;
            }
            else if ( op > 15933 )
            {
              if ( op == 15934 )
              {
                ds_qshr(&v23, *pval, val);
                *pval = v23;
              }
              else if ( op == 31868 )
              {
                v10 = 0;
                if ( !ds_qbeq(ds_qzero, *pval) || !ds_qbeq(ds_qzero, val) )
                  v10 = 1;
                ds_cwq(&v31, v10);
                *pval = v31;
              }
            }
            else if ( op == 15677 )
            {
              v12 = ds_qbeq(*pval, val);
              ds_cwq(&v30, v12 != 0);
              *pval = v30;
            }
          }
          else if ( op == 8509 )
          {
            v13 = ds_qbeq(*pval, val);
            ds_cwq(&v29, v13 == 0);
            *pval = v29;
          }
          else if ( op > 8509 )
          {
            if ( op == 9766 )
            {
              v11 = 0;
              if ( !ds_qbeq(ds_qzero, *pval) && !ds_qbeq(ds_qzero, val) )
                v11 = 1;
              ds_cwq(&v32, v11);
              *pval = v32;
            }
            else if ( op == 15420 )
            {
              ds_qshl(&v24, *pval, val);
              *pval = v24;
            }
          }
          else if ( op == 124 )
          {
            ds_qor(&v33, *pval, val);
            *pval = v33;
          }
        }
        else if ( op == 43 )
        {
          ds_qadd(&v22, *pval, val);
          *pval = v22;
        }
        else if ( op > 43 )
        {
          if ( op == 47 )
          {
            if ( ds_qbeq(ds_qzero, val) )
              goto LABEL_188;
            ds_qdiv(&v19, *pval, val);
            *pval = v19;
          }
          else if ( op > 47 )
          {
            if ( op == 60 )
            {
              v14 = ds_qblt(*pval, val);
              ds_cwq(&v28, v14 != 0);
              *pval = v28;
            }
            else if ( op == 62 )
            {
              v16 = ds_qbgt(*pval, val);
              ds_cwq(&v26, v16 != 0);
              *pval = v26;
            }
          }
          else if ( op == 45 )
          {
            ds_qsub(&v21, *pval, val);
            *pval = v21;
          }
        }
        else if ( op == 38 )
        {
          ds_qand(&v35, *pval, val);
          *pval = v35;
        }
        else if ( op > 38 )
        {
          if ( op == 42 )
          {
            ds_qmul(&v20, *pval, val);
            *pval = v20;
          }
        }
        else if ( op == 37 )
        {
          if ( ds_qbeq(ds_qzero, val) )
          {
LABEL_188:
            ds_printf("zero divided\n");
            return 0;
          }
          ds_qmod(&v18, *pval, val);
          *pval = v18;
        }
      }
    }
    if ( op == 33 )
    {
      v5 = ds_qbeq(ds_qzero, val);
      ds_cwq(&v32, v5 != 0);
      *pval = v32;
      goto LABEL_51;
    }
    if ( op != 42 )
      goto LABEL_51;
    adr = ds_cqw(val);
    if ( !ds_load_mem(adr, &wv, 4) )
    {
      ds_cwq(&v33, wv);
      *pval = v33;
      goto LABEL_51;
    }
    return 0;
  }
}

int __cdecl ds_eval_quad(char *str, quad *pv)
{
  char *s; // [esp+8h] [ebp+8h]

  if ( *str == 34 || *str == 39 )
    return ds_printf("invalid expression - %s\n", str);
  s = expr(0, str, pv);
  if ( !s )
    return -1;
  if ( *s )
    return ds_printf("invalid extra chars - '%s'\n", s);
  return 0;
}

int __cdecl ds_eval_twin(char *str, twin *pv)
{
  quad v3; // [esp-14h] [ebp-2Ch]
  _QWORD x[3]; // [esp+0h] [ebp-18h] BYREF

  if ( ds_eval_quad(str, (quad *)&x[1]) )
    return -1;
  *(_QWORD *)&v3.xa[2] = *(_QWORD *)((char *)&x[1] + 4);
  v3.xa[1] = x[1];
  v3.xa[0] = (unsigned int)x;
  ds_cqt(v3);
#if 0
  *pv = (twin)x[0];
#endif
  // FIXME: Is this correct?
  pv->xa[0] = x[0];
  pv->xa[1] = x[1];
  return 0;
}

int __cdecl ds_eval_word(char *str, unsigned int *pv)
{
  quad val; // [esp+0h] [ebp-10h] BYREF

  if ( ds_eval_quad(str, &val) )
    return -1;
  *pv = ds_cqw(val);
  return 0;
}

int __cdecl ds_eval_word_ex(char *str, unsigned int *pv)
{
  quad val; // [esp+0h] [ebp-10h] BYREF

  if ( ds_eval_quad(str, &val) )
    return -1;
  *pv = atoi(str);
  return 0;
}

static char *__cdecl fexpr(int prio, char *str, float *pval)
{
  char *v4; // eax
  size_t v5; // eax
  float val; // [esp+4h] [ebp-2Ch] BYREF
  unsigned int wv; // [esp+8h] [ebp-28h] BYREF
  unsigned int adr; // [esp+Ch] [ebp-24h]
  quad qv; // [esp+10h] [ebp-20h] BYREF
  int pr; // [esp+20h] [ebp-10h]
  char *q; // [esp+24h] [ebp-Ch]
  char *p; // [esp+28h] [ebp-8h]
  char op; // [esp+2Fh] [ebp-1h]
  char *s; // [esp+3Ch] [ebp+Ch]
  char *sa; // [esp+3Ch] [ebp+Ch]
  char *sb; // [esp+3Ch] [ebp+Ch]

  s = skipsp(str);
  if ( *s )
  {
    if ( *s == 40 )
    {
      sa = fexpr(0, s + 1, pval);
      if ( sa )
      {
        v4 = sa;
        s = sa + 1;
        if ( *v4 == 41 )
          goto LABEL_55;
        ds_printf("unblanced '('\n");
        return 0;
      }
      else
      {
        return 0;
      }
    }
    else
    {
      op = *s;
      if ( op == 43 || op == 45 || op == 42 )
      {
        if ( op == 42 )
        {
          s = expr(14, s + 1, &qv);
          if ( s )
          {
            adr = ds_cqw(qv);
            if ( ds_load_mem(adr, &wv, 4) )
            {
              return 0;
            }
            else
            {
              memcpy(pval, &wv, sizeof(float));
              while ( 1 )
              {
LABEL_55:
                sb = skipsp(s);
                if ( !*sb || *sb == 41 )
                  return sb;
                op = *sb;
                if ( op == 43 || op == 45 )
                {
                  pr = 12;
                  if ( prio >= 12 )
                    return sb;
                }
                else
                {
                  if ( op != 42 && op != 47 )
                  {
                    ds_printf("invalid op - '%c'\n", op);
                    return 0;
                  }
                  pr = 13;
                  if ( prio >= 13 )
                    return sb;
                }
                s = fexpr(pr, sb + 1, &val);
                if ( !s )
                  break;
                if ( op == 43 )
                {
                  *pval = *pval + val;
                }
                else if ( op > 43 )
                {
                  if ( op == 45 )
                  {
                    *pval = *pval - val;
                  }
                  else if ( op == 47 )
                  {
                    *pval = *pval / val;
                  }
                }
                else if ( op == 42 )
                {
                  *pval = *pval * val;
                }
              }
              return 0;
            }
          }
          else
          {
            return 0;
          }
        }
        else
        {
          s = fexpr(14, s + 1, &val);
          if ( s )
          {
            if ( op == 43 )
            {
              *pval = val;
              goto LABEL_55;
            }
            if ( op == 45 )
            {
              *pval = -val;
              goto LABEL_55;
            }
            ds_printf("invalid op '%C' for float expr\n", op);
            return 0;
          }
          else
          {
            return 0;
          }
        }
      }
      else
      {
        v5 = strlen(s);
        p = (char *)ds_alloc(v5 + 1);
        if ( p )
        {
          if ( *s == 36 )
          {
            q = p;
            while ( *s == 36 || *s == 95 || (isdigit(*s)) != 0 || (isalpha(*s)) != 0 )
              *q++ = *s++;
          }
          else if ( (isdigit(*s)) != 0 )
          {
            q = p;
            while ( (isdigit(*s)) != 0 )
              *q++ = *s++;
            if ( *s == 46 )
              *q++ = *s++;
            while ( (isdigit(*s)) != 0 )
              *q++ = *s++;
            if ( *s == 101 || *s == 69 )
            {
              *q++ = *s++;
              if ( *s == 43 || *s == 45 )
                *q++ = *s++;
              while ( (isdigit(*s)) != 0 )
                *q++ = *s++;
            }
          }
          *q = 0;
          if ( *p == 36 )
          {
            ds_printf("floating register/variable reference in fexpr - not yet supported\n");
            return (char *)ds_free(p);
          }
          else
          {
            if ( sscanf(p, "%g", pval) == 1 )
            {
              ds_free(p);
              goto LABEL_55;
            }
            ds_printf("invalid floating number (%s)\n", p);
            return (char *)ds_free(p);
          }
        }
        else
        {
          return 0;
        }
      }
    }
  }
  else
  {
    ds_printf("unexpected end\n");
    return 0;
  }
}

int __cdecl ds_eval_float(char *str, float *pv)
{
  char *s; // [esp+8h] [ebp+8h]

  if ( *str == 34 || *str == 39 )
    return ds_printf("invalid expression - %s\n", str);
  s = fexpr(0, str, pv);
  if ( !s )
    return -1;
  if ( *s )
    return ds_printf("invalid extra chars - '%s'\n", s);
  return 0;
}

