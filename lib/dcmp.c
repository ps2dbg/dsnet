#include "dsnet_prototypes.h"

int __cdecl ds_send_dcmp_connect(DS_DESC *desc, int did, int result, unsigned int *param)
{
  unsigned __int8 v5; // al
  int v6; // eax
  int n; // [esp+8h] [ebp-18h]
  unsigned __int8 code; // [esp+Fh] [ebp-11h]
  DSP_BUF *db; // [esp+18h] [ebp-8h]
  unsigned __int8 v10; // [esp+1Eh] [ebp-2h]

  v10 = result;
  if ( !desc )
    return -1;
  if ( (_BYTE)result == 0xFF )
  {
    v10 = 0;
    v5 = 0;
  }
  else
  {
    v5 = 1;
  }
  code = v5;
  if ( v5 )
    v6 = 0;
  else
    v6 = 16;
  n = v6;
  db = ds_alloc_buf(1, (unsigned __int8)did, 0, v6 + 8);
  if ( !db )
    return -1;
  db->buf[8] = 0;
  db->buf[9] = code;
  *(_WORD *)&db->buf[10] = 0;
  ds_bzero(&db->buf[12], 4);
  db->buf[12] = v10;
  if ( param )
  {
    if ( n > 0 )
      memcpy(&db->buf[16], param, n);
  }
  ds_send_desc(desc, db);
  return 0;
}

int __cdecl ds_send_dcmp_status(DS_DESC *desc, int nid, int code, int proto)
{
  DSP_BUF *db; // [esp+14h] [ebp-8h]

  db = ds_alloc_buf(1, (unsigned __int8)nid, 0, 6);
  if ( !db )
    return -1;
  db->buf[8] = 2;
  db->buf[9] = code;
  *(_WORD *)&db->buf[10] = 0;
  *(_WORD *)&db->buf[12] = proto;
  ds_send_desc(desc, db);
  return 0;
}

int __cdecl ds_send_dcmp_status_net(int code, int proto)
{
  DSP_BUF *db; // [esp+10h] [ebp-8h]

  db = ds_alloc_buf(1, 72, 0, 6);
  if ( !db )
    return -1;
  db->buf[8] = 2;
  db->buf[9] = code;
  *(_WORD *)&db->buf[10] = 0;
  *(_WORD *)&db->buf[12] = proto;
  ds_send_net(0, db);
  return 0;
}

DSP_BUF *__cdecl ds_send_dcmp_error(DS_DESC *desc, DSP_BUF *odb, int code)
{
  int len; // [esp+4h] [ebp-34h]
  char data[28]; // [esp+10h] [ebp-28h] BYREF
  DSP_BUF *db; // [esp+2Ch] [ebp-Ch]
  DECI2_HDR *odh; // [esp+30h] [ebp-8h]
  unsigned __int8 v8; // [esp+37h] [ebp-1h]

  v8 = code;
  odh = (DECI2_HDR *)odb->buf;
  len = *(unsigned __int16 *)odb->buf - 8;
  if ( len < 0 )
    return odb;
  data[0] = 3;
  data[1] = v8;
  *(_WORD *)&data[2] = 0;
  ds_bzero(&data[12], 16);
  if ( (unsigned int)len > 0x10 )
    len = 16;
  memcpy(&data[4], odh, len + 8);
  db = ds_alloc_buf(1, odh->source, data, 28);
  if ( !db )
    return odb;
  ds_send_desc(desc, db);
  return ds_free_buf(odb);
}

