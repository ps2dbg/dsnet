#include "dsnet_prototypes.h"

unsigned char ds_sid = 72u;

DSP_BUF *ds_alloc_buf(int proto, int did, void *ptr, int len)
{
  DSP_BUF *db; // [esp+Ch] [ebp-8h]

  if ( (unsigned int)(len + sizeof(DSP_BUF)) <= 0xFFFF )
  {
    db = (DSP_BUF *)ds_alloc(len + sizeof(DSP_BUF) + 8);
    if ( db )
    {
      *(_WORD *)db->buf = len + 8;
      *(_WORD *)&db->buf[2] = 0;
      *(_WORD *)&db->buf[4] = proto;
      db->buf[6] = ds_sid;
      db->buf[7] = did;
      if ( ptr )
      {
        if ( len > 0 )
          memcpy(&db->buf[8], ptr, len);
      }
      return db;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    ds_error("ds_alloc_buf - too big");
    return 0;
  }
}

DSP_BUF *ds_free_buf(DSP_BUF *db)
{
  return (DSP_BUF *)ds_free(db);
}

DSP_BUF *ds_dup_buf(DSP_BUF *db)
{
  int len; // [esp+0h] [ebp-Ch]
  DSP_BUF *r; // [esp+8h] [ebp-4h]

  len = *(unsigned __int16 *)db->buf;
  r = (DSP_BUF *)ds_alloc(len + sizeof(DSP_BUF));
  if ( !r )
    return 0;
  memcpy(r->buf, db->buf, len);
  return r;
}

