#include "dsnet_prototypes.h"

quad ds_qzero = { { 0u, 0u, 0u, 0u } };

static int __cdecl ds_qcmp(quad x, quad y);
static int __cdecl ds_qdivide(quad *rdp, quad *rmp, quad x, quad y);

twin __cdecl ds_cwt(unsigned int v)
{
  twin result; // rax
  int v2; // [esp+14h] [ebp+Ch]

#if 0
  result = (twin)v;
#endif
  // TODO: is this correct?
  result.xa[0] = v;
  *(_DWORD *)v = v2;
  *(_DWORD *)(v + 4) = 0;
  return result;
}

unsigned int __cdecl ds_ctw(twin x)
{
  return x.xa[0];
}

quad *__cdecl ds_cwq(quad *retstr, unsigned int v)
{
  quad *result; // eax

  result = retstr;
  retstr->xa[0] = v;
  retstr->xa[1] = 0;
  retstr->xa[2] = 0;
  retstr->xa[3] = 0;
  return result;
}

unsigned int __cdecl ds_cqw(quad x)
{
  return x.xa[0];
}

quad *__cdecl ds_ctq(quad *retstr, twin v)
{
  quad *result; // eax

  result = retstr;
  retstr->xa[0] = v.xa[0];
  retstr->xa[1] = v.xa[1];
  retstr->xa[2] = 0;
  retstr->xa[3] = 0;
  return result;
}

twin __cdecl ds_cqt(quad x)
{
  twin result; // rax

#if 0
  result = (twin)__PAIR64__(x.xa[2], x.xa[0]);
#endif
  // FIXME: Is this correct?
  result.xa[0] = x.xa[0];
  result.xa[1] = x.xa[2];
  *(_DWORD *)x.xa[0] = x.xa[1];
  *(_DWORD *)(x.xa[0] + 4) = x.xa[2];
  return result;
}

quad *__cdecl ds_qneg(quad *retstr, quad x)
{
  quad *result; // eax

  result = retstr;
  retstr->xa[0] = ~x.xa[0];
  retstr->xa[1] = ~x.xa[1];
  retstr->xa[2] = ~x.xa[2];
  retstr->xa[3] = ~x.xa[3];
  return result;
}

quad *__cdecl ds_qor(quad *retstr, quad x, quad y)
{
  quad *result; // eax

  result = retstr;
  retstr->xa[0] = y.xa[0] | x.xa[0];
  retstr->xa[1] = y.xa[1] | x.xa[1];
  retstr->xa[2] = y.xa[2] | x.xa[2];
  retstr->xa[3] = y.xa[3] | x.xa[3];
  return result;
}

quad *__cdecl ds_qxor(quad *retstr, quad x, quad y)
{
  quad *result; // eax

  result = retstr;
  retstr->xa[0] = y.xa[0] ^ x.xa[0];
  retstr->xa[1] = y.xa[1] ^ x.xa[1];
  retstr->xa[2] = y.xa[2] ^ x.xa[2];
  retstr->xa[3] = y.xa[3] ^ x.xa[3];
  return result;
}

quad *__cdecl ds_qand(quad *retstr, quad x, quad y)
{
  quad *result; // eax

  result = retstr;
  retstr->xa[0] = y.xa[0] & x.xa[0];
  retstr->xa[1] = y.xa[1] & x.xa[1];
  retstr->xa[2] = y.xa[2] & x.xa[2];
  retstr->xa[3] = y.xa[3] & x.xa[3];
  return result;
}

int __cdecl ds_qblt(quad x, quad y)
{
  int i; // [esp+0h] [ebp-4h]

  for ( i = 3; i >= 0; --i )
  {
    if ( y.xa[i] > x.xa[i] )
      return 1;
    if ( y.xa[i] < x.xa[i] )
      return 0;
  }
  return 0;
}

int __cdecl ds_qble(quad x, quad y)
{
  int i; // [esp+0h] [ebp-4h]

  for ( i = 3; i >= 0; --i )
  {
    if ( y.xa[i] > x.xa[i] )
      return 1;
    if ( y.xa[i] < x.xa[i] )
      return 0;
  }
  return 1;
}

int __cdecl ds_qbgt(quad x, quad y)
{
  int i; // [esp+0h] [ebp-4h]

  for ( i = 3; i >= 0; --i )
  {
    if ( y.xa[i] < x.xa[i] )
      return 1;
    if ( y.xa[i] > x.xa[i] )
      return 0;
  }
  return 0;
}

int __cdecl ds_qbge(quad x, quad y)
{
  int i; // [esp+0h] [ebp-4h]

  for ( i = 3; i >= 0; --i )
  {
    if ( y.xa[i] < x.xa[i] )
      return 1;
    if ( y.xa[i] > x.xa[i] )
      return 0;
  }
  return 1;
}

int __cdecl ds_qbeq(quad x, quad y)
{
  int result; // eax

  result = 0;
  if ( y.xa[0] == x.xa[0] && y.xa[1] == x.xa[1] && y.xa[2] == x.xa[2] && y.xa[3] == x.xa[3] )
    return 1;
  return result;
}

quad *__cdecl ds_qshr(quad *retstr, quad x, quad y)
{
  unsigned int v3; // eax
  int sa; // [esp+8h] [ebp-14h]
  quad r; // [esp+Ch] [ebp-10h] BYREF

  v3 = y.xa[0];
  LOBYTE(v3) = y.xa[0] & 0x80;
  if ( y.xa[3] | y.xa[2] | y.xa[1] | v3 )
  {
    ds_bzero(&r, sizeof(r));
    *retstr = r;
  }
  else
  {
    memcpy(&r, &x, sizeof(r));
    for ( sa = y.xa[0] & 0x7F; sa > 31; sa -= 32 )
    {
      r.xa[0] = r.xa[1];
      r.xa[1] = r.xa[2];
      r.xa[2] = r.xa[3];
      r.xa[3] = 0;
    }
    if ( sa > 0 )
    {
      r.xa[0] >>= sa;
      r.xa[0] |= r.xa[1] << (32 - sa);
      r.xa[1] >>= sa;
      r.xa[1] |= r.xa[2] << (32 - sa);
      r.xa[2] >>= sa;
      r.xa[2] |= r.xa[3] << (32 - sa);
      r.xa[3] >>= sa;
    }
    *retstr = r;
  }
  return retstr;
}

quad *__cdecl ds_qshl(quad *retstr, quad x, quad y)
{
  unsigned int v3; // eax
  int sa; // [esp+8h] [ebp-14h]
  quad r; // [esp+Ch] [ebp-10h] BYREF

  v3 = y.xa[0];
  LOBYTE(v3) = y.xa[0] & 0x80;
  if ( y.xa[3] | y.xa[2] | y.xa[1] | v3 )
  {
    ds_bzero(&r, sizeof(r));
    *retstr = r;
  }
  else
  {
    memcpy(&r, &x, sizeof(r));
    for ( sa = y.xa[0] & 0x7F; sa > 31; sa -= 32 )
    {
      r.xa[3] = r.xa[2];
      r.xa[2] = r.xa[1];
      r.xa[1] = r.xa[0];
      r.xa[0] = 0;
    }
    if ( sa > 0 )
    {
      r.xa[3] <<= sa;
      r.xa[3] |= r.xa[2] >> (32 - sa);
      r.xa[2] <<= sa;
      r.xa[2] |= r.xa[1] >> (32 - sa);
      r.xa[1] <<= sa;
      r.xa[1] |= r.xa[0] >> (32 - sa);
      r.xa[0] <<= sa;
    }
    *retstr = r;
  }
  return retstr;
}

quad *__cdecl ds_qadd(quad *retstr, quad x, quad y)
{
  unsigned int l; // [esp+4h] [ebp-14h]
  unsigned int l_1; // [esp+4h] [ebp-14h]
  unsigned int l_2; // [esp+4h] [ebp-14h]
  quad r; // [esp+8h] [ebp-10h] BYREF

  memcpy(&r, &x, sizeof(r));
  l = r.xa[0];
  r.xa[0] += y.xa[0];
  if ( (l > r.xa[0] || y.xa[0] > r.xa[0]) && !++r.xa[1] && !++r.xa[2] )
    ++r.xa[3];
  l_1 = r.xa[1];
  r.xa[1] += y.xa[1];
  if ( (l_1 > r.xa[1] || y.xa[1] > r.xa[1]) && !++r.xa[2] )
    ++r.xa[3];
  l_2 = r.xa[2];
  r.xa[2] += y.xa[2];
  if ( l_2 > r.xa[2] || y.xa[2] > r.xa[2] )
    ++r.xa[3];
  r.xa[3] += y.xa[3];
  *retstr = r;
  return retstr;
}

quad *__cdecl ds_qsub(quad *retstr, quad x, quad y)
{
  unsigned int l; // [esp+4h] [ebp-14h]
  unsigned int l_1; // [esp+4h] [ebp-14h]
  unsigned int l_2; // [esp+4h] [ebp-14h]
  quad r; // [esp+8h] [ebp-10h] BYREF

  memcpy(&r, &x, sizeof(r));
  l = r.xa[0];
  r.xa[0] -= y.xa[0];
  if ( y.xa[0] > l && --r.xa[1] == -1 && --r.xa[2] == -1 )
    --r.xa[3];
  l_1 = r.xa[1];
  r.xa[1] -= y.xa[1];
  if ( y.xa[1] > l_1 && --r.xa[2] == -1 )
    --r.xa[3];
  l_2 = r.xa[2];
  r.xa[2] -= y.xa[2];
  if ( y.xa[2] > l_2 )
    --r.xa[3];
  r.xa[3] -= y.xa[3];
  *retstr = r;
  return retstr;
}

quad *__cdecl ds_qmul(quad *retstr, quad x, quad y)
{
  quad v4; // [esp+8h] [ebp-48h] BYREF
  int n; // [esp+18h] [ebp-38h]
  int i; // [esp+1Ch] [ebp-34h]
  quad ty; // [esp+20h] [ebp-30h] BYREF
  quad tx; // [esp+30h] [ebp-20h] BYREF
  quad r; // [esp+40h] [ebp-10h] BYREF

  ds_bzero(&r, sizeof(r));
  memcpy(&tx, &x, sizeof(tx));
  memcpy(&ty, &y, sizeof(ty));
  for ( i = 0; i <= 127; i += n )
  {
    if ( (ty.xa[0] & 1) != 0 )
      ds_qadd(&r, r, tx);
    for ( n = 1; ; ++n )
    {
      ds_cwq(&v4, 1u);
      ds_qshr(&ty, ty, v4);
      if ( ds_qbeq(ds_qzero, ty) || (ty.xa[0] & 1) != 0 )
        break;
    }
    if ( (ty.xa[0] & 1) == 0 )
    {
      *retstr = r;
      return retstr;
    }
    ds_cwq(&v4, n);
    ds_qshl(&tx, tx, v4);
    if ( ds_qbeq(ds_qzero, tx) )
    {
      *retstr = r;
      return retstr;
    }
  }
  *retstr = r;
  return retstr;
}

static int __cdecl ds_qcmp(quad x, quad y)
{
  int i; // [esp+0h] [ebp-4h]

  for ( i = 3; i >= 0; --i )
  {
    if ( y.xa[i] < x.xa[i] )
      return 1;
    if ( y.xa[i] > x.xa[i] )
      return -1;
  }
  return 0;
}

static int __cdecl ds_qdivide(quad *rdp, quad *rmp, quad x, quad y)
{
  quad v5; // [esp+8h] [ebp-44h] BYREF
  quad v6; // [esp+18h] [ebp-34h] BYREF
  int i; // [esp+28h] [ebp-24h]
  quad z; // [esp+2Ch] [ebp-20h] BYREF
  quad d; // [esp+3Ch] [ebp-10h] BYREF

  ds_bzero(&d, sizeof(d));
  for ( i = 0; i <= 127 && (y.xa[3 - (i >> 5)] & (1 << (31 - (i & 0x1F)))) == 0; ++i )
    ;
  ds_cwq(&v6, i);
  ds_qshl(&y, y, v6);
  ds_cwq(&v6, i);
  ds_cwq(&v5, 1u);
  ds_qshl(&z, v5, v6);
  do
  {
    if ( ds_qcmp(x, y) >= 0 )
    {
      ds_qadd(&d, d, z);
      ds_qsub(&x, x, y);
    }
    ds_cwq(&v5, 1u);
    ds_qshr(&y, y, v5);
    ds_cwq(&v5, 1u);
    ds_qshr(&z, z, v5);
  }
  while ( !ds_qbeq(ds_qzero, z) );
  if ( rdp )
    *rdp = d;
  if ( rmp )
    *rmp = x;
  return 0;
}

quad *__cdecl ds_qdiv(quad *retstr, quad x, quad y)
{
  quad rv; // [esp+4h] [ebp-10h] BYREF

  ds_qdivide(&rv, 0, x, y);
  *retstr = rv;
  return retstr;
}

quad *__cdecl ds_qmod(quad *retstr, quad x, quad y)
{
  quad rv; // [esp+4h] [ebp-10h] BYREF

  ds_qdivide(0, &rv, x, y);
  *retstr = rv;
  return retstr;
}

