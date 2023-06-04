#include "dsnet_prototypes.h"

char *ds_stamp_str[2] = { DSNET_STAMP_STRING, DSNET_VERSION_STRING };
char *ds_version_str = DSNET_VERSION_STRING;

static char *device_name = "/dev/mrp0";
static void *cur_pointer = NULL;
static int cur_state = 0;
static int no_kbd = 0;
static int argc = 0;
static unsigned int oldoffset_32 = 0u;
static unsigned int oldcount_33 = 0u;
static unsigned int oldoffset_36 = 0u;
static unsigned int oldsize_37 = 0u;
static unsigned int oldoffset_40 = 0u;
static unsigned int oldsize_41 = 0u;
trig_st trig_val[] =
{
  { "eegs", 2147483649u },
  { "gifa", 2147483650u },
  { "gifb", 2147483652u },
  { "iopa", 2147483656u },
  { "iopb", 2147483664u },
  { "switch", 2147483904u },
  { "none", 0u }
};
trig_st trig_putcode[] =
{ { "triga", 18u }, { "trigb", 22u }, { "trigc", 26u }, { "trigd", 30u } };
trig_st trig_getcode[] =
{ { "triga", 16u }, { "trigb", 20u }, { "trigc", 24u }, { "trigd", 28u } };
trig_st vblank_val[] =
{
  { "up", 2147483649u },
  { "down", 2147483650u },
  { "high", 2147483652u },
  { "low", 2147483656u }
};
trig_st intr_val[] =
{ { "spu2", 2147483649u } };
gif_st gif_code[] =
{ { "gifa", 131094u, 131090u, 131098u }, { "gifb", 131110u, 131106u, 131114u } };
iop_st iop_code[] =
{ { "iopa", 196634u, 196626u, 196630u }, { "iopb", 196650u, 196642u, 196646u } };
static DS_OPTION *opt_iopconf;
static DS_OPTION *opt_iopmodules;
static DS_OPTION *opt_netdev;
static DS_DESC *target_desc;
static char **argv;
static gs_pa gs_120;
static gs_pa gs_121;
static int a01_128;
static int a02_129;
static int a03_130;
static int a04_131;
static int a05_132;
static int a06_133;
static int a07_134;
static int a08_135;
static int a09_136;
static int a10_137;
static int a11_138;
static int a12_139;
static int a13_140;
static int a14_141;
static int a15_142;
static int a16_143;
static int b01_144;
static int b02_145;
static int b03_146;
static int b04_147;
static int b05_148;
static int b06_149;
static int b07_150;
static int b08_151;
static int b09_152;
static int b10_153;
static int b11_154;
static int b12_155;
static int b13_156;
static int b14_157;
static int b15_158;
static int b16_159;
static int c01_160;
static int c02_161;
static int c03_162;
static int c04_163;
static int c05_164;
static int c06_165;
static int c07_166;
static int c08_167;
static int c09_168;
static int c10_169;
static int c11_170;
static int c12_171;
static int c13_172;
static int c14_173;
static int c15_174;
static int c16_175;
static int d01_176;
static int d02_177;
static int d03_178;
static int m_179;
static int m01_180;
static int m02_181;
static int m03_182;
static int m04_183;
static int m05_184;
static int m06_185;
static int m07_186;
static int m08_187;
static int m09_188;
static int m10_189;
static int m11_190;
static int m12_191;
static int m13_192;
static int m14_193;
static int m15_194;
static int m16_195;

extern int __cdecl pa_getreg(unsigned int code, unsigned int *data);
extern void __cdecl print_hex(char *bp, int offset, int cnt);

static int __cdecl pamp_error(unsigned int no);
static void __cdecl batch(int ac, char **av);
static DSP_BUF *__cdecl recv_pamp(DS_DESC *desc, DSP_BUF *db);
static void __cdecl tty_flush_check(DSP_BUF *db);
static DSP_BUF *__cdecl send_net(DS_DESC *desc, DSP_BUF *db, int proto);
static DSP_BUF *__cdecl recv_target(DS_DESC *desc, DSP_BUF *db);
static int __cdecl usage(int f_true);
static void __cdecl connect_dev(char *device_name, int f_first);
static void __cdecl idle_func();
static void __cdecl show_statics(int tb, int tst, int tut);
static int __cdecl pa_puttrigpos(char **av);
static int __cdecl pa_sr(int ac, char **av);
static int __cdecl pa_getgif(unsigned int code, gifreg_t *gifreg);
static int __cdecl pa_putgif(unsigned int code, gifreg_t *gifreg);
static int __cdecl pa_putiopmem(unsigned int code, iopmem_t *pmem);
static int __cdecl pa_putreg(unsigned int code, unsigned int value);
static void __cdecl print_gifreg(gifreg_t *gifreg, unsigned int vblank);
static void __cdecl print_trig_gif(unsigned int reg);
static void __cdecl print_trig(unsigned int reg);

static int __cdecl pamp_error(unsigned int no)
{
  switch ( no )
  {
    case 0u:
      return 0;
    case 1u:
      ds_error("*** ERROR INVAL ***");
      break;
    case 2u:
      ds_error("*** ERROR BUSY ***");
      break;
    case 3u:
      ds_error("*** ERROR NOMEM ***");
      break;
    case 4u:
      ds_error("*** ERROR UNSUP ***");
      break;
    case 5u:
      ds_error("*** ERROR MMAP ***");
      break;
    case 6u:
      ds_error("*** ERROR NOTPA ***");
      break;
    default:
      ds_error("*** ERROR UNKNOWN ***");
      break;
  }
  return 0;
}

static void __cdecl batch(int ac, char **av)
{
  int r; // [esp+4h] [ebp-40Ch]
  char *v3; // [esp+8h] [ebp-408h]
  char *p; // [esp+Ch] [ebp-404h]
  char buf[1024]; // [esp+10h] [ebp-400h] BYREF

  v3 = buf;
  while ( ac > 0 )
  {
    for ( p = *av; *p && (unsigned int)(v3 - buf) <= 0x3FE; ++p )
      *v3++ = *p;
    if ( ac > 1 && (unsigned int)(v3 - buf) <= 0x3FE )
      *v3++ = 32;
    --ac;
    ++av;
  }
  *v3 = 0;
  r = ds_cmd_execute(buf, 0);
  ds_exit(r);
}

static DSP_BUF *__cdecl recv_pamp(DS_DESC *desc, DSP_BUF *db)
{
  int len; // [esp+0h] [ebp-Ch]
  unsigned int *ph; // [esp+4h] [ebp-8h]

  ph = (unsigned int *)&db->buf[8];
  if ( !db )
    return 0;
  len = *(unsigned __int16 *)db->buf - 12;
  if ( len < 0 )
    return db;
  if ( (*ph & 3) == 1 )
    memcpy(cur_pointer, &db->buf[12], len);
  if ( (*ph & 3) == 3 )
    memcpy(cur_pointer, &db->buf[12], len);
  cur_state &= ~1u;
  return ds_free_buf(db);
}

static void __cdecl tty_flush_check(DSP_BUF *db)
{
  if ( *(_WORD *)db->buf == 12 && (unsigned __int8)*(_DWORD *)&db->buf[8] == 1 )
    ds_flush_desc_by_proto(*(unsigned __int16 *)&db->buf[4]);
}

static DSP_BUF *__cdecl send_net(DS_DESC *desc, DSP_BUF *db, int proto)
{
  if ( proto > 527 && proto <= 543 || proto > 271 && proto <= 287 )
    tty_flush_check(db);
  if ( !proto || ds_desc_by_proto(proto) )
    return ds_send_net(proto, db);
  else
    return ds_send_dcmp_error(desc, db, 1);
}

static DSP_BUF *__cdecl recv_target(DS_DESC *desc, DSP_BUF *db)
{
  int v2; // eax
  DSP_BUF *result; // eax
  int v4; // eax
  int n; // [esp+0h] [ebp-Ch]

  if ( !db )
    ds_exit(130);
  v2 = *(unsigned __int16 *)&db->buf[4];
  if ( v2 == 290 )
    return ds_recv_drfp(desc, db);
  if ( *(unsigned __int16 *)&db->buf[4] <= 0x122u )
  {
    if ( v2 == 1 )
      return db;
    return send_net(desc, db, *(unsigned __int16 *)&db->buf[4]);
  }
  if ( v2 != 1024 )
  {
    if ( v2 == 1296 )
      return db;
    return send_net(desc, db, *(unsigned __int16 *)&db->buf[4]);
  }
  switch ( db->buf[8] )
  {
    case 0:
    case 1:
      ds_send_netmp_default_msg(target_desc);
      v4 = strlen((const char *)ds_version_str);
      ds_send_netmp(target_desc, 10, 0, ds_version_str, v4);
      goto LABEL_21;
    case 3:
      return ds_send_net(0, db);
    case 0xB:
      n = *(unsigned __int16 *)db->buf - 10;
      if ( db->buf[9] )
        ds_exit(148);
      if ( n != strlen((const char *)ds_version_str) )
        ds_exit_invalid_version(&db->buf[10], n, (char *)ds_version_str);
      if ( ds_strncmp((char *)ds_version_str, &db->buf[10], n) )
        ds_exit_invalid_version(&db->buf[10], n, (char *)ds_version_str);
      goto LABEL_21;
    default:
LABEL_21:
      result = db;
      break;
  }
  return result;
}

static int __cdecl usage(int f_true)
{
  if ( !f_true )
    return 0;
  ds_printf("Usage: %s [<options>]...\n", ds_program_name);
  ds_printf("  <options>:\n");
  ds_printf("    -d <device>      set device name to <device>\n");
  ds_printf("    -p <port>        set listen port to <port>\n");
  ds_printf("    -nokbd           do not use keyboard when batch\n");
  return ds_exit(129);
}

static void __cdecl connect_dev(char *device_name, int f_first)
{
  if ( !argc && !no_kbd )
  {
    if ( !f_first )
      ds_printf("\n");
    ds_printf("\rConnecting to %s ...\n", device_name);
  }
  target_desc = ds_connect_net(device_name, recv_target);
  if ( !target_desc )
    ds_exit(132);
  if ( !ds_add_recv_func(target_desc, 1280, -1, -1, recv_pamp) )
    ds_exit(135);
  if ( ds_send_netmp1(target_desc, 0, 0, 240, 1280) < 0 )
    ds_exit(135);
  if ( !ds_cmd_executing && !no_kbd )
    ds_prompt();
}

static void __cdecl idle_func()
{
  if ( !target_desc )
    connect_dev(device_name, 0);
}

int __cdecl recv_pa_rdmem(int ac, char **av)
{
  return 0;
}

int __cdecl pa_wait_cmd(int ac, char **av)
{
  unsigned int status; // [esp+0h] [ebp-4h] BYREF

  while ( !pa_getreg(8u, &status) )
  {
    if ( (status & 0x80000000) == 0 )
    {
      ds_printf("*** stopped ***\n");
      return 0;
    }
  }
  return 0;
}

int __cdecl pa_de(unsigned int *data, unsigned int offset, unsigned int size)
{
  int v4; // [esp+8h] [ebp-18h]
  int r; // [esp+Ch] [ebp-14h]
  DSP_BUF *db; // [esp+14h] [ebp-Ch]

  v4 = -1;
  db = ds_alloc_buf(1280, 72, 0, 12);
  if ( !db )
    return -1;
  *(_DWORD *)&db->buf[8] = 0x10000;
  *(_DWORD *)&db->buf[12] = offset;
  *(_DWORD *)&db->buf[16] = size;
  cur_state = 1;
  ds_send_desc(target_desc, db);
  do
    r = ds_select_desc(120, 0);
  while ( r >= 0 && (cur_state & 1) != 0 );
  if ( r )
  {
    if ( *(_DWORD *)cur_pointer )
    {
      pamp_error(*(_DWORD *)cur_pointer);
    }
    else
    {
      v4 = *(_DWORD *)cur_pointer;
      *data = *((_DWORD *)cur_pointer + 1);
    }
  }
  else
  {
    ds_error("Timeout\n");
  }
  return v4;
}

int __cdecl pa_di(unsigned int *data, unsigned int offset, unsigned int size)
{
  int v4; // [esp+8h] [ebp-18h]
  int r; // [esp+Ch] [ebp-14h]
  DSP_BUF *db; // [esp+14h] [ebp-Ch]

  v4 = -1;
  db = ds_alloc_buf(1280, 72, 0, 12);
  if ( !db )
    return -1;
  *(_DWORD *)&db->buf[8] = 196608;
  *(_DWORD *)&db->buf[12] = offset;
  *(_DWORD *)&db->buf[16] = size;
  cur_state = 1;
  ds_send_desc(target_desc, db);
  do
    r = ds_select_desc(120, 0);
  while ( r >= 0 && (cur_state & 1) != 0 );
  if ( r )
  {
    if ( *(_DWORD *)cur_pointer )
    {
      pamp_error(*(_DWORD *)cur_pointer);
    }
    else
    {
      v4 = *(_DWORD *)cur_pointer;
      *data = *((_DWORD *)cur_pointer + 1);
    }
  }
  else
  {
    ds_error("Timeout\n");
  }
  return v4;
}

int __cdecl pa_mem(unsigned int code, unsigned int *data, unsigned int offset, unsigned int count)
{
  int v5; // [esp+8h] [ebp-18h]
  int r; // [esp+Ch] [ebp-14h]
  DSP_BUF *db; // [esp+14h] [ebp-Ch]

  v5 = -1;
  db = ds_alloc_buf(1280, 72, 0, 12);
  if ( !db )
    return -1;
  *(_DWORD *)&db->buf[8] = code;
  *(_DWORD *)&db->buf[12] = offset;
  *(_DWORD *)&db->buf[16] = count;
  cur_state = 1;
  ds_send_desc(target_desc, db);
  do
    r = ds_select_desc(120, 0);
  while ( r >= 0 && (cur_state & 1) != 0 );
  if ( r )
  {
    if ( *(_DWORD *)cur_pointer )
    {
      pamp_error(*(_DWORD *)cur_pointer);
    }
    else
    {
      v5 = *(_DWORD *)cur_pointer;
      *data = *((_DWORD *)cur_pointer + 1);
    }
  }
  else
  {
    ds_error("Timeout\n");
  }
  return v5;
}

int __cdecl pa_dg(unsigned int *data, unsigned int offset, unsigned int size)
{
  int v4; // [esp+8h] [ebp-18h]
  int r; // [esp+Ch] [ebp-14h]
  DSP_BUF *db; // [esp+14h] [ebp-Ch]

  v4 = -1;
  db = ds_alloc_buf(1280, 72, 0, 12);
  if ( !db )
    return -1;
  *(_DWORD *)&db->buf[8] = 0x20000;
  *(_DWORD *)&db->buf[12] = offset;
  *(_DWORD *)&db->buf[16] = size;
  cur_state = 1;
  ds_send_desc(target_desc, db);
  do
    r = ds_select_desc(120, 0);
  while ( r >= 0 && (cur_state & 1) != 0 );
  if ( r )
  {
    if ( *(_DWORD *)cur_pointer )
    {
      pamp_error(*(_DWORD *)cur_pointer);
    }
    else
    {
      v4 = *(_DWORD *)cur_pointer;
      *data = *((_DWORD *)cur_pointer + 1);
    }
  }
  else
  {
    ds_error("Timeout\n");
  }
  return v4;
}

int __cdecl pa_dg_cmd(int ac, char **av)
{
  const char *v3; // eax
  const char *v4; // eax
  const char *v5; // eax
  const char *v6; // eax
  const char *v7; // eax
  gifdata_t *gif; // [esp+0h] [ebp-10h]
  int i; // [esp+4h] [ebp-Ch]
  unsigned int count; // [esp+8h] [ebp-8h] BYREF
  unsigned int offset; // [esp+Ch] [ebp-4h] BYREF

  if ( ac > 3 )
    return ds_error("Usage : dg [<offset>] [<count>]");
  switch ( ac )
  {
    case 0:
    case 1:
      offset = oldoffset_32;
      count = oldcount_33;
      if ( !oldcount_33 )
        return ds_error("Usage : dg [<offset>] [<count>]");
      goto LABEL_14;
    case 3:
      if ( ds_eval_word(av[2], &count) )
        return ds_error("Invalid argument");
      break;
    case 2:
      count = 16;
      break;
  }
  if ( ds_eval_word(av[1], &offset) )
    return ds_error("Invalid argument");
LABEL_14:
  oldoffset_32 = count + offset;
  oldcount_33 = count;
  if ( count > 0x100 )
    return ds_error("error: count > 0x100");
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_dg_cmd", 10 * count + 4);
  if ( !cur_pointer )
    return ds_error("no memory");
  if ( !pa_dg((unsigned int *)cur_pointer, offset, count) )
  {
    gif = (gifdata_t *)((char *)cur_pointer + 4);
    for ( i = 0; count > i; ++i )
    {
      ds_printf(" 0x%W: ", i + offset);
      if ( (gif->GINT & 1) != 0 )
        v3 = " ";
      else
        v3 = "I";
      ds_printf("%s", v3);
      if ( (gif->GINT & 0x20) != 0 )
        v4 = "R";
      else
        v4 = "W";
      ds_printf("%s", v4);
      if ( (gif->GINT & 0x40) != 0 )
        v5 = " ";
      else
        v5 = "S";
      ds_printf("%s", v5);
      if ( (gif->GINT & 0x80u) == 0 )
        v6 = " ";
      else
        v6 = "*";
      ds_printf("%s", v6);
      if ( (gif->GRDYGC & 0x80u) == 0 )
        v7 = " ";
      else
        v7 = "V";
      ds_printf("%s", v7);
      ds_printf("%02x: %08x %08x\n", gif->GRDYGC & 0x7F, *(_DWORD *)&gif->GACK, *(_DWORD *)&gif->GVLDGC);
      gif = (gifdata_t *)((char *)gif + 10);
    }
  }
  ds_free_mem_low(cur_pointer, "pacons.c", "pa_dg_cmd");
  return 0;
}

int __cdecl pa_dgi_cmd(int ac, char **av)
{
  unsigned int size; // [esp+0h] [ebp-8h] BYREF
  unsigned int offset; // [esp+4h] [ebp-4h] BYREF

  if ( ac > 3 )
    return ds_error("Usage : dgi [<offset>] [<size>]");
  switch ( ac )
  {
    case 0:
    case 1:
      offset = oldoffset_36;
      size = oldsize_37;
      if ( !oldsize_37 )
        return ds_error("Usage : dgi [<offset>] [<size>]");
      goto LABEL_14;
    case 3:
      if ( ds_eval_word(av[2], &size) )
        return ds_error("Invalid argument");
      break;
    case 2:
      size = 256;
      break;
  }
  if ( ds_eval_word(av[1], &offset) )
    return ds_error("Invalid argument");
LABEL_14:
  oldoffset_36 = size + offset;
  oldsize_37 = size;
  if ( size > 0xFFE0 )
    return ds_error("error: size > 0xffe0");
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_dgi_cmd", size + 4);
  if ( !cur_pointer )
    return ds_error("no memory");
  if ( !pa_mem(0x20000000u, (unsigned int *)cur_pointer, offset, size) )
    print_hex((char *)cur_pointer, offset, size);
  ds_free_mem_low(cur_pointer, "pacons.c", "pa_dgi_cmd");
  return 0;
}

int __cdecl pa_dii_cmd(int ac, char **av)
{
  unsigned int size; // [esp+0h] [ebp-8h] BYREF
  unsigned int offset; // [esp+4h] [ebp-4h] BYREF

  if ( ac > 3 )
    return ds_error("Usage : dii [<offset>] [<size>]");
  switch ( ac )
  {
    case 0:
    case 1:
      offset = oldoffset_40;
      size = oldsize_41;
      if ( !oldsize_41 )
        return ds_error("Usage : dii [<offset>] [<size>]");
      goto LABEL_14;
    case 3:
      if ( ds_eval_word(av[2], &size) )
        return ds_error("Invalid argument");
      break;
    case 2:
      size = 256;
      break;
  }
  if ( ds_eval_word(av[1], &offset) )
    return ds_error("Invalid argument");
LABEL_14:
  oldoffset_40 = size + offset;
  oldsize_41 = size;
  if ( size > 0xFFE0 )
    return ds_error("error: size > 0xffe0");
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_dii_cmd", size + 4);
  if ( !cur_pointer )
    return ds_error("no memory");
  if ( !pa_mem(0x30000000u, (unsigned int *)cur_pointer, offset, size) )
    print_hex((char *)cur_pointer, offset, size);
  ds_free_mem_low(cur_pointer, "pacons.c", "pa_dii_cmd");
  return 0;
}

void __cdecl print_hex(char *bp, int offset, int cnt)
{
  const char *v3; // eax
  int v4; // eax
  int n; // [esp+0h] [ebp-Ch]
  int j; // [esp+4h] [ebp-8h]
  int j_1; // [esp+4h] [ebp-8h]
  int i; // [esp+8h] [ebp-4h]

  for ( i = 0; cnt > i; i += 16 )
  {
    ds_printf(" 0x%W: ", i + offset);
    n = cnt - i;
    if ( cnt - i > 16 )
      n = 16;
    for ( j = 0; j <= 15 && n > j; ++j )
    {
      if ( j == 7 )
        v3 = "  ";
      else
        v3 = " ";
      ds_printf("%B%s", bp[j], v3);
    }
    ds_printf(" ");
    while ( j <= 15 )
    {
      ds_printf("   ");
      ++j;
    }
    for ( j_1 = 0; j_1 <= 15 && n > j_1; ++j_1 )
    {
      if ( bp[j_1] <= 31 || bp[j_1] == 127 )
        v4 = 46;
      else
        v4 = bp[j_1];
      ds_printf("%c", v4);
    }
    ds_printf("\n");
    bp += 16;
  }
}

int __cdecl pa_de_cmd(int ac, char **av)
{
  unsigned int size; // [esp+0h] [ebp-8h] BYREF
  unsigned int offset; // [esp+4h] [ebp-4h] BYREF

  if ( ac != 3 && ac != 2 )
    return ds_error("Usage : de <offset> [<size>]");
  if ( ac == 3 )
  {
    if ( ds_eval_word(av[2], &size) )
      return ds_error("Invalid argument");
  }
  else
  {
    size = 32;
  }
  if ( size > 0x20 )
    return ds_error("error: size > 0x20");
  if ( ds_eval_word(av[1], &offset) )
    return ds_error("Invalid argument");
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_de_cmd", 10 * size + 4);
  if ( !cur_pointer )
    return ds_error("no memory");
  if ( !pa_de((unsigned int *)cur_pointer, offset, size) )
    print_hex((char *)cur_pointer, offset, 10 * size);
  ds_free_mem_low(cur_pointer, "pacons.c", "pa_de_cmd");
  return 0;
}

int __cdecl pa_dei_cmd(int ac, char **av)
{
  unsigned int size; // [esp+0h] [ebp-8h] BYREF
  unsigned int offset; // [esp+4h] [ebp-4h] BYREF

  if ( ac != 3 && ac != 2 )
    return ds_error("Usage : de <offset> [<size>]");
  if ( ac == 3 )
  {
    if ( ds_eval_word(av[2], &size) )
      return ds_error("Invalid argument");
  }
  else
  {
    size = 256;
  }
  if ( size > 0xFFE0 )
    return ds_error("error: size > 0xffe0");
  if ( ds_eval_word(av[1], &offset) )
    return ds_error("Invalid argument");
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_dei_cmd", size + 4);
  if ( !cur_pointer )
    return ds_error("no memory");
  if ( !pa_mem(0x10000000u, (unsigned int *)cur_pointer, offset, size) )
    print_hex((char *)cur_pointer, offset, size);
  ds_free_mem_low(cur_pointer, "pacons.c", "pa_dei_cmd");
  return 0;
}

int __cdecl pa_di_cmd(int ac, char **av)
{
  unsigned int size; // [esp+0h] [ebp-8h] BYREF
  unsigned int offset; // [esp+4h] [ebp-4h] BYREF

  if ( ac != 3 && ac != 2 )
    return ds_error("Usage : di <offset> [<size>]");
  if ( ac == 3 )
  {
    if ( ds_eval_word(av[2], &size) )
      return ds_error("Invalid argument");
  }
  else
  {
    size = 32;
  }
  if ( size > 0x20 )
    return ds_error("error: size > 0x20");
  if ( ds_eval_word(av[1], &offset) )
    return ds_error("Invalid argument");
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_di_cmd", 16 * size + 4);
  if ( !cur_pointer )
    return ds_error("no memory");
  if ( !pa_di((unsigned int *)cur_pointer, offset, size) )
    print_hex((char *)cur_pointer, 16 * offset, 16 * size);
  ds_free_mem_low(cur_pointer, "pacons.c", "pa_di_cmd");
  return 0;
}

int __cdecl pa_reset()
{
  int v1; // [esp+4h] [ebp-18h]
  int r; // [esp+8h] [ebp-14h]
  DSP_BUF *db; // [esp+10h] [ebp-Ch]

  v1 = -1;
  db = ds_alloc_buf(1280, 72, 0, 4);
  if ( !db )
    return -1;
  *(_DWORD *)&db->buf[8] = 2;
  cur_state = 1;
  ds_send_desc(target_desc, db);
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_reset", 4);
  if ( cur_pointer )
  {
    do
      r = ds_select_desc(120, 0);
    while ( r >= 0 && (cur_state & 1) != 0 );
    if ( r )
    {
      if ( *(_DWORD *)cur_pointer )
        pamp_error(*(_DWORD *)cur_pointer);
      else
        v1 = *(_DWORD *)cur_pointer;
    }
    else
    {
      ds_error("Timeout\n");
    }
    ds_free_mem_low(cur_pointer, "pacons.c", "pa_reset");
    return v1;
  }
  else
  {
    ds_error("no memory");
    return -1;
  }
}

int __cdecl pa_reset_cmd(int ac, char **av)
{
  if ( !ac )
    return 0;
  if ( ac != 1 )
    return ds_error("Usage : reset");
  pa_reset();
  return 0;
}

static void __cdecl show_statics(int tb, int tst, int tut)
{
  int v3; // [esp+Ch] [ebp-14h]
  int v4; // [esp+1Ch] [ebp-4h]

  v4 = (tut + 500) / 1000 + 1000 * tst;
  if ( v4 <= 0 )
    v3 = 0;
  else
    v3 = (int)((long double)tb * 1000.0 / (long double)v4);
  ds_printf("%d bytes %d.%03d sec %.1f kB/s\n", tb, v4 / 1000, v4 % 1000, (double)((long double)v3 / 1024.0));
}

int __cdecl pa_sg_cmd(int ac, char **av)
{
  char *ptr; // [esp+0h] [ebp-2Ch]
  int64_t u; // [esp+4h] [ebp-28h] BYREF
  int64_t s; // [esp+8h] [ebp-24h] BYREF
  int64_t tu0; // [esp+Ch] [ebp-20h] BYREF
  int64_t ts0; // [esp+10h] [ebp-1Ch] BYREF
  int v8; // [esp+14h] [ebp-18h]
  unsigned int v9; // [esp+18h] [ebp-14h]
  int left; // [esp+1Ch] [ebp-10h]
  void *fpw; // [esp+20h] [ebp-Ch]
  unsigned int offset; // [esp+24h] [ebp-8h] BYREF
  unsigned int count; // [esp+28h] [ebp-4h] BYREF

  v8 = -1;
  if ( ac != 4 )
    return ds_error("Usage : sg <file> <offset> <count>");
  if ( ds_eval_word(av[2], &offset) || ds_eval_word(av[3], &count) )
    return ds_error("Invalid argument");
  left = count;
  v9 = count;
  if ( count > 0x800 )
    count = 2048;
  fpw = ds_fopen(av[1], "w");
  if ( !fpw )
    return v8;
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_sg_cmd", 10 * count + 4);
  if ( cur_pointer )
  {
    ptr = (char *)cur_pointer + 4;
    ds_gettime(&ts0, &tu0);
    while ( left > 0 )
    {
      if ( !pa_dg((unsigned int *)cur_pointer, offset, count) )
        ds_fwrite(ptr, 10 * count, 1, fpw);
      left -= count;
      offset += count;
      if ( count > left )
        count = left;
    }
    ds_gettime(&s, &u);
    s -= ts0;
    u -= tu0;
    if ( u < 0 )
    {
      --s;
      u += 1000000;
    }
    show_statics(10 * v9, s, u);
    ds_fclose(fpw);
    ds_free_mem_low(cur_pointer, "pacons.c", "pa_sg_cmd");
    return 0;
  }
  else
  {
    ds_error("no memory");
    ds_fclose(fpw);
  }
  return v8;
}

int __cdecl pa_sgi_cmd(int ac, char **av)
{
  char *ptr; // [esp+0h] [ebp-28h]
  int64_t tut; // [esp+4h] [ebp-24h] BYREF
  int64_t s; // [esp+8h] [ebp-20h] BYREF
  int64_t tu0; // [esp+Ch] [ebp-1Ch] BYREF
  int64_t psec; // [esp+10h] [ebp-18h] BYREF
  unsigned int total; // [esp+14h] [ebp-14h]
  int left; // [esp+18h] [ebp-10h]
  void *fpw; // [esp+1Ch] [ebp-Ch]
  unsigned int offset; // [esp+20h] [ebp-8h] BYREF
  unsigned int size; // [esp+24h] [ebp-4h] BYREF

  if ( ac != 4 )
    return ds_error("Usage : sgi <file> <offset> <size>");
  if ( ds_eval_word(av[2], &offset) || ds_eval_word(av[3], &size) )
    return ds_error("Invalid argument");
  left = size;
  total = size;
  if ( size > 0x8000 )
    size = 0x8000;
  fpw = ds_fopen(av[1], "w");
  if ( !fpw )
    return -1;
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_sgi_cmd", size + 4);
  if ( cur_pointer )
  {
    ptr = (char *)cur_pointer + 4;
    ds_gettime(&psec, &tu0);
    while ( left > 0 )
    {
      if ( !pa_mem(0x20000000u, (unsigned int *)cur_pointer, offset, size) )
        ds_fwrite(ptr, size, 1, fpw);
      left -= size;
      offset += size;
      if ( size > left )
        size = left;
    }
    ds_gettime(&s, &tut);
    s -= psec;
    tut -= tu0;
    if ( tut < 0 )
    {
      --s;
      tut += 1000000;
    }
    show_statics(total, s, tut);
    ds_fclose(fpw);
    ds_free_mem_low(cur_pointer, "pacons.c", "pa_sgi_cmd");
    return 0;
  }
  else
  {
    ds_error("no memory");
    ds_fclose(fpw);
    return ds_error("no memory");
  }
}

int __cdecl pa_sei_cmd(int ac, char **av)
{
  char *ptr; // [esp+0h] [ebp-28h]
  int64_t tut; // [esp+4h] [ebp-24h] BYREF
  int64_t s; // [esp+8h] [ebp-20h] BYREF
  int64_t tu0; // [esp+Ch] [ebp-1Ch] BYREF
  int64_t psec; // [esp+10h] [ebp-18h] BYREF
  unsigned int total; // [esp+14h] [ebp-14h]
  int left; // [esp+18h] [ebp-10h]
  void *fpw; // [esp+1Ch] [ebp-Ch]
  unsigned int offset; // [esp+20h] [ebp-8h] BYREF
  unsigned int size; // [esp+24h] [ebp-4h] BYREF

  if ( ac != 4 )
    return ds_error("Usage : sei <file> <offset> <size>");
  if ( ds_eval_word(av[2], &offset) || ds_eval_word(av[3], &size) )
    return ds_error("Invalid argument");
  left = size;
  total = size;
  if ( size > 0x8000 )
    size = 0x8000;
  fpw = ds_fopen(av[1], "w");
  if ( !fpw )
    return -1;
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_sei_cmd", size + 4);
  if ( cur_pointer )
  {
    ptr = (char *)cur_pointer + 4;
    ds_gettime(&psec, &tu0);
    while ( left > 0 )
    {
      if ( !pa_mem(0x10000000u, (unsigned int *)cur_pointer, offset, size) )
        ds_fwrite(ptr, size, 1, fpw);
      left -= size;
      offset += size;
      if ( size > left )
        size = left;
    }
    ds_gettime(&s, &tut);
    s -= psec;
    tut -= tu0;
    if ( tut < 0 )
    {
      --s;
      tut += 1000000;
    }
    show_statics(total, s, tut);
    ds_fclose(fpw);
    ds_free_mem_low(cur_pointer, "pacons.c", "pa_sei_cmd");
    return 0;
  }
  else
  {
    ds_error("no memory");
    ds_fclose(fpw);
    return ds_error("no memory");
  }
}

int __cdecl pa_sii_cmd(int ac, char **av)
{
  char *ptr; // [esp+0h] [ebp-28h]
  int64_t tut; // [esp+4h] [ebp-24h] BYREF
  int64_t s; // [esp+8h] [ebp-20h] BYREF
  int64_t tu0; // [esp+Ch] [ebp-1Ch] BYREF
  int64_t psec; // [esp+10h] [ebp-18h] BYREF
  unsigned int total; // [esp+14h] [ebp-14h]
  int left; // [esp+18h] [ebp-10h]
  void *fpw; // [esp+1Ch] [ebp-Ch]
  unsigned int offset; // [esp+20h] [ebp-8h] BYREF
  unsigned int size; // [esp+24h] [ebp-4h] BYREF

  if ( ac != 4 )
    return ds_error("Usage : sii <file> <offset> <size>");
  if ( ds_eval_word(av[2], &offset) || ds_eval_word(av[3], &size) )
    return ds_error("Invalid argument");
  left = size;
  total = size;
  if ( size > 0x8000 )
    size = 0x8000;
  fpw = ds_fopen(av[1], "w");
  if ( !fpw )
    return -1;
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_sii_cmd", size + 4);
  if ( cur_pointer )
  {
    ptr = (char *)cur_pointer + 4;
    ds_gettime(&psec, &tu0);
    while ( left > 0 )
    {
      if ( !pa_mem(0x30000000u, (unsigned int *)cur_pointer, offset, size) )
        ds_fwrite(ptr, size, 1, fpw);
      left -= size;
      offset += size;
      if ( size > left )
        size = left;
    }
    ds_gettime(&s, &tut);
    s -= psec;
    tut -= tu0;
    if ( tut < 0 )
    {
      --s;
      tut += 1000000;
    }
    show_statics(total, s, tut);
    ds_fclose(fpw);
    ds_free_mem_low(cur_pointer, "pacons.c", "pa_sii_cmd");
    return 0;
  }
  else
  {
    ds_error("no memory");
    ds_fclose(fpw);
    return ds_error("no memory");
  }
}

int __cdecl pa_si_cmd(int ac, char **av)
{
  char *ptr; // [esp+0h] [ebp-28h]
  int64_t u; // [esp+4h] [ebp-24h] BYREF
  int64_t s; // [esp+8h] [ebp-20h] BYREF
  int64_t tu0; // [esp+Ch] [ebp-1Ch] BYREF
  int64_t psec; // [esp+10h] [ebp-18h] BYREF
  unsigned int v8; // [esp+14h] [ebp-14h]
  int left; // [esp+18h] [ebp-10h]
  void *fpw; // [esp+1Ch] [ebp-Ch]
  unsigned int offset; // [esp+20h] [ebp-8h] BYREF
  unsigned int size; // [esp+24h] [ebp-4h] BYREF

  if ( ac != 4 )
    return ds_error("Usage : si <file> <offset> <size>");
  if ( ds_eval_word(av[2], &offset) || ds_eval_word(av[3], &size) )
    return ds_error("Invalid argument");
  left = size;
  v8 = size;
  if ( size > 0x800 )
    size = 2048;
  fpw = ds_fopen(av[1], "w");
  if ( !fpw )
    return -1;
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_si_cmd", 16 * size + 4);
  if ( cur_pointer )
  {
    ptr = (char *)cur_pointer + 4;
    ds_gettime(&psec, &tu0);
    while ( left > 0 )
    {
      if ( !pa_di((unsigned int *)cur_pointer, offset, size) )
        ds_fwrite(ptr, 16 * size, 1, fpw);
      left -= size;
      offset += size;
      if ( size > left )
        size = left;
    }
    ds_gettime(&s, &u);
    s -= psec;
    u -= tu0;
    if ( u < 0 )
    {
      --s;
      u += 1000000;
    }
    show_statics(16 * v8, s, u);
    ds_fclose(fpw);
    ds_free_mem_low(cur_pointer, "pacons.c", "pa_si_cmd");
    return 0;
  }
  else
  {
    ds_error("no memory");
    ds_fclose(fpw);
    return ds_error("no memory");
  }
}

int __cdecl pa_save_cmd(int ac, char **av)
{
  return 0;
}

int __cdecl pa_se_cmd(int ac, char **av)
{
  char *ptr; // [esp+0h] [ebp-28h]
  int64_t u; // [esp+4h] [ebp-24h] BYREF
  int64_t s; // [esp+8h] [ebp-20h] BYREF
  int64_t tu0; // [esp+Ch] [ebp-1Ch] BYREF
  int64_t psec; // [esp+10h] [ebp-18h] BYREF
  unsigned int v8; // [esp+14h] [ebp-14h]
  int left; // [esp+18h] [ebp-10h]
  void *fpw; // [esp+1Ch] [ebp-Ch]
  unsigned int offset; // [esp+20h] [ebp-8h] BYREF
  unsigned int size; // [esp+24h] [ebp-4h] BYREF

  if ( ac != 4 )
    return ds_error("Usage : se <file> <offset> <size>");
  if ( ds_eval_word(av[2], &offset) || ds_eval_word(av[3], &size) )
    return ds_error("Invalid argument");
  left = size;
  v8 = size;
  if ( size > 0x800 )
    size = 2048;
  fpw = ds_fopen(av[1], "w");
  if ( !fpw )
    return -1;
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_se_cmd", 10 * size + 4);
  if ( cur_pointer )
  {
    ptr = (char *)cur_pointer + 4;
    ds_gettime(&psec, &tu0);
    while ( left > 0 )
    {
      if ( !pa_de((unsigned int *)cur_pointer, offset, size) )
        ds_fwrite(ptr, 10 * size, 1, fpw);
      left -= size;
      offset += size;
      if ( size > left )
        size = left;
    }
    ds_gettime(&s, &u);
    s -= psec;
    u -= tu0;
    if ( u < 0 )
    {
      --s;
      u += 1000000;
    }
    show_statics(10 * v8, s, u);
    ds_fclose(fpw);
    ds_free_mem_low(cur_pointer, "pacons.c", "pa_se_cmd");
    return 0;
  }
  else
  {
    ds_error("no memory");
    ds_fclose(fpw);
    return ds_error("no memory");
  }
}

int __cdecl pa_getreg(unsigned int code, unsigned int *data)
{
  int v3; // [esp+8h] [ebp-18h]
  int r; // [esp+Ch] [ebp-14h]
  DSP_BUF *db; // [esp+14h] [ebp-Ch]

  v3 = -1;
  db = ds_alloc_buf(1280, 72, 0, 4);
  if ( !db )
    return -1;
  *(_DWORD *)&db->buf[8] = code;
  cur_state = 1;
  ds_send_desc(target_desc, db);
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_getreg", 8);
  if ( cur_pointer )
  {
    do
      r = ds_select_desc(120, 0);
    while ( r >= 0 && (cur_state & 1) != 0 );
    if ( r )
    {
      if ( *(_DWORD *)cur_pointer )
      {
        pamp_error(*(_DWORD *)cur_pointer);
      }
      else
      {
        v3 = *(_DWORD *)cur_pointer;
        *data = *((_DWORD *)cur_pointer + 1);
      }
    }
    else
    {
      ds_error("Timeout\n");
    }
    ds_free_mem_low(cur_pointer, "pacons.c", "pa_getreg");
    return v3;
  }
  else
  {
    ds_error("no memory");
    return -1;
  }
}

int __cdecl pa_dr(unsigned int addr, unsigned int *data)
{
  int v3; // [esp+8h] [ebp-18h]
  int r; // [esp+Ch] [ebp-14h]
  DSP_BUF *db; // [esp+14h] [ebp-Ch]

  v3 = -1;
  db = ds_alloc_buf(1280, 72, 0, 4);
  if ( !db )
    return -1;
  *(_DWORD *)&db->buf[8] = addr | 0x60000000;
  cur_state = 1;
  ds_send_desc(target_desc, db);
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_dr", 8);
  if ( cur_pointer )
  {
    do
      r = ds_select_desc(120, 0);
    while ( r >= 0 && (cur_state & 1) != 0 );
    if ( r )
    {
      if ( *(_DWORD *)cur_pointer )
      {
        pamp_error(*(_DWORD *)cur_pointer);
      }
      else
      {
        v3 = *(_DWORD *)cur_pointer;
        *data = *((_DWORD *)cur_pointer + 1);
      }
    }
    else
    {
      ds_error("Timeout\n");
    }
    ds_free_mem_low(cur_pointer, "pacons.c", "pa_dr");
    return v3;
  }
  else
  {
    ds_error("no memory");
    return -1;
  }
}

int __cdecl pa_start()
{
  int v1; // [esp+4h] [ebp-18h]
  int r; // [esp+8h] [ebp-14h]
  DSP_BUF *db; // [esp+10h] [ebp-Ch]

  v1 = -1;
  db = ds_alloc_buf(1280, 72, 0, 4);
  if ( !db )
    return -1;
  *(_DWORD *)&db->buf[8] = 4;
  cur_state = 1;
  ds_send_desc(target_desc, db);
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_start", 4);
  if ( cur_pointer )
  {
    do
      r = ds_select_desc(120, 0);
    while ( r >= 0 && (cur_state & 1) != 0 );
    if ( r )
    {
      if ( *(_DWORD *)cur_pointer )
        pamp_error(*(_DWORD *)cur_pointer);
      else
        v1 = *(_DWORD *)cur_pointer;
    }
    else
    {
      ds_error("Timeout\n");
    }
    ds_free_mem_low(cur_pointer, "pacons.c", "pa_start");
    return v1;
  }
  else
  {
    ds_error("no memory");
    return -1;
  }
}

int __cdecl pa_stop()
{
  int v1; // [esp+4h] [ebp-18h]
  int r; // [esp+8h] [ebp-14h]
  DSP_BUF *db; // [esp+10h] [ebp-Ch]

  v1 = -1;
  db = ds_alloc_buf(1280, 72, 0, 4);
  if ( !db )
    return -1;
  *(_DWORD *)&db->buf[8] = 6;
  cur_state = 1;
  ds_send_desc(target_desc, db);
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_stop", 4);
  if ( cur_pointer )
  {
    do
      r = ds_select_desc(120, 0);
    while ( r >= 0 && (cur_state & 1) != 0 );
    if ( r )
    {
      if ( *(_DWORD *)cur_pointer )
        pamp_error(*(_DWORD *)cur_pointer);
      else
        v1 = *(_DWORD *)cur_pointer;
    }
    else
    {
      ds_error("Timeout\n");
    }
    ds_free_mem_low(cur_pointer, "pacons.c", "pa_stop");
    return v1;
  }
  else
  {
    ds_error("no memory");
    return -1;
  }
}

int __cdecl pa_ver(unsigned int *data)
{
  int v2; // [esp+4h] [ebp-18h]
  int r; // [esp+8h] [ebp-14h]
  DSP_BUF *db; // [esp+10h] [ebp-Ch]

  v2 = -1;
  db = ds_alloc_buf(1280, 72, 0, 4);
  if ( !db )
    return -1;
  *(_DWORD *)&db->buf[8] = 0;
  cur_state = 1;
  ds_send_desc(target_desc, db);
  cur_pointer = data;
  do
    r = ds_select_desc(120, 0);
  while ( r >= 0 && (cur_state & 1) != 0 );
  if ( r )
  {
    v2 = *(_DWORD *)cur_pointer;
    if ( *(_DWORD *)cur_pointer )
      pamp_error(*(_DWORD *)cur_pointer);
  }
  else
  {
    ds_error("Timeout\n");
  }
  ds_free_mem_low(cur_pointer, "pacons.c", "pa_ver");
  return v2;
}

int __cdecl pa_dr_default()
{
  unsigned int value; // [esp+0h] [ebp-4h] BYREF

  ds_printf("=== MASTER ===\n");
  if ( !pa_dr(0x60000000u, &value) )
    ds_printf("bid       (%08x)  %08x\n", 0, value);
  if ( !pa_dr(0x60000004u, &value) )
    ds_printf("reset     (%08x)  %08x\n", 4, value);
  if ( !pa_dr(0x60000008u, &value) )
    ds_printf("status    (%08x)  %08x\n", 8, value);
  if ( !pa_dr(0x6000000Cu, &value) )
    ds_printf("select    (%08x)  %08x\n", 12, value);
  if ( !pa_dr(0x60000010u, &value) )
    ds_printf("Aselect   (%08x)  %08x\n", 16, value);
  if ( !pa_dr(0x60000014u, &value) )
    ds_printf("Bselect   (%08x)  %08x\n", 20, value);
  if ( !pa_dr(0x60000018u, &value) )
    ds_printf("Cselect   (%08x)  %08x\n", 24, value);
  if ( !pa_dr(0x6000001Cu, &value) )
    ds_printf("Dselect   (%08x)  %08x\n", 28, value);
  if ( !pa_dr(0x60000020u, &value) )
    ds_printf("State     (%08x)  %08x\n", 32, value);
  if ( !pa_dr(0x60000024u, &value) )
    ds_printf("Enable    (%08x)  %08x\n", 36, value);
  if ( !pa_dr(0x60000030u, &value) )
    ds_printf("Compress  (%08x)  %08x\n", 48, value);
  if ( !pa_dr(0x60000200u, &value) )
    ds_printf("Mversion  (%08x)  %08x\n", 512, value);
  ds_printf("=== EEGS ===\n");
  if ( !pa_dr(0x60010000u, &value) )
    ds_printf("EEcontrol (%08x)  %08x\n", 0x10000, value);
  if ( !pa_dr(0x60010004u, &value) )
    ds_printf("EEstop    (%08x)  %08x\n", 65540, value);
  if ( !pa_dr(0x6001009Cu, &value) )
    ds_printf("counter (%08x)  %08x\n", 65692, value);
  if ( !pa_dr(0x60010200u, &value) )
    ds_printf("EEversion (%08x)  %08x\n", 66048, value);
  ds_printf("=== GIF ===\n");
  if ( !pa_dr(0x60020000u, &value) )
    ds_printf("GIFcontrol (%08x)  %08x\n", 0x20000, value);
  if ( !pa_dr(0x60020004u, &value) )
    ds_printf("GIFstop    (%08x)  %08x\n", 131076, value);
  if ( !pa_dr(0x60020008u, &value) )
    ds_printf("GIFstatus  (%08x)  %08x\n", 131080, value);
  if ( !pa_dr(0x60020010u, &value) )
    ds_printf("Aaddr      (%08x)  %08x\n", 131088, value);
  if ( !pa_dr(0x60020014u, &value) )
    ds_printf("Aaddrmsk   (%08x)  %08x\n", 131092, value);
  if ( !pa_dr(0x60020020u, &value) )
    ds_printf("Adata      (%08x)  %08x", 131104, value);
  if ( !pa_dr(0x60020018u, &value) )
    ds_printf(" _ (%08x)  %08x\n", 131096, value);
  if ( !pa_dr(0x60020024u, &value) )
    ds_printf("Adatamsk   (%08x)  %08x", 131108, value);
  if ( !pa_dr(0x6002001Cu, &value) )
    ds_printf(" _ (%08x)  %08x\n", 131100, value);
  if ( !pa_dr(0x60020028u, &value) )
    ds_printf("Avblank    (%08x)  %08x\n", 131112, value);
  if ( !pa_dr(0x6002002Cu, &value) )
    ds_printf("Aintc      (%08x)  %08x\n", 131116, value);
  if ( !pa_dr(0x60020030u, &value) )
    ds_printf("Baddr      (%08x)  %08x\n", 131120, value);
  if ( !pa_dr(0x60020034u, &value) )
    ds_printf("Baddrmsk   (%08x)  %08x\n", 131124, value);
  if ( !pa_dr(0x60020040u, &value) )
    ds_printf("Bdata      (%08x)  %08x", 131136, value);
  if ( !pa_dr(0x60020038u, &value) )
    ds_printf(" _ (%08x)  %08x\n", 131128, value);
  if ( !pa_dr(0x60020044u, &value) )
    ds_printf("Bdatamsk   (%08x)  %08x", 131140, value);
  if ( !pa_dr(0x6002003Cu, &value) )
    ds_printf(" _ (%08x)  %08x\n", 131132, value);
  if ( !pa_dr(0x60020048u, &value) )
    ds_printf("Bvblank    (%08x)  %08x\n", 131144, value);
  if ( !pa_dr(0x6002004Cu, &value) )
    ds_printf("Bintc      (%08x)  %08x\n", 131148, value);
  if ( !pa_dr(0x6002009Cu, &value) )
    ds_printf("counter (%08x)  %08x\n", 131228, value);
  if ( !pa_dr(0x60020100u, &value) )
    ds_printf("GIFdata (%08x)  %08x\n", 131328, value);
  if ( !pa_dr(0x60020200u, &value) )
    ds_printf("GIFversion (%08x)  %08x\n", 131584, value);
  ds_printf("=== IOP ===\n");
  if ( !pa_dr(0x60030000u, &value) )
    ds_printf("IOPcontrol (%08x)  %08x\n", 196608, value);
  if ( !pa_dr(0x60030004u, &value) )
    ds_printf("IOPstop    (%08x)  %08x\n", 196612, value);
  if ( !pa_dr(0x60030008u, &value) )
    ds_printf("status    A(%08x)  %08x ", 196616, value);
  if ( !pa_dr(0x6003000Cu, &value) )
    ds_printf("B(%08x)  %08x\n", 196620, value);
  if ( !pa_dr(0x60030010u, &value) )
    ds_printf("select   AA(%08x)  %08x ", 196624, value);
  if ( !pa_dr(0x60030018u, &value) )
    ds_printf("BA(%08x)  %08x\n", 196628, value);
  if ( !pa_dr(0x60030014u, &value) )
    ds_printf("select   AB(%08x)  %08x ", 196628, value);
  if ( !pa_dr(0x6003001Cu, &value) )
    ds_printf("BB(%08x)  %08x\n", 196636, value);
  if ( !pa_dr(0x60030020u, &value) )
    ds_printf("sif      A(%08x)  %08x ", 196640, value);
  if ( !pa_dr(0x60030050u, &value) )
    ds_printf("B(%08x)  %08x\n", 196688, value);
  if ( !pa_dr(0x60030024u, &value) )
    ds_printf("sifintc  A(%08x)  %08x ", 196644, value);
  if ( !pa_dr(0x60030054u, &value) )
    ds_printf("B(%08x)  %08x\n", 196692, value);
  if ( !pa_dr(0x60030028u, &value) )
    ds_printf("ssbuf    A(%08x)  %08x ", 196648, value);
  if ( !pa_dr(0x60030058u, &value) )
    ds_printf("B(%08x)  %08x\n", 196696, value);
  if ( !pa_dr(0x6003002Cu, &value) )
    ds_printf("ssbufintc A(%08x)  %08x ", 196652, value);
  if ( !pa_dr(0x6003005Cu, &value) )
    ds_printf("B(%08x)  %08x\n", 196700, value);
  if ( !pa_dr(0x60030030u, &value) )
    ds_printf("memrow    A(%08x)  %08x ", 196656, value);
  if ( !pa_dr(0x60030060u, &value) )
    ds_printf("B(%08x)  %08x\n", 196704, value);
  if ( !pa_dr(0x60030034u, &value) )
    ds_printf("memrowmsk A(%08x)  %08x ", 196660, value);
  if ( !pa_dr(0x60030064u, &value) )
    ds_printf("B(%08x)  %08x\n", 196708, value);
  if ( !pa_dr(0x60030038u, &value) )
    ds_printf("memcol   A(%08x)  %08x ", 196664, value);
  if ( !pa_dr(0x60030068u, &value) )
    ds_printf("B(%08x)  %08x\n", 196712, value);
  if ( !pa_dr(0x6003003Cu, &value) )
    ds_printf("memcolmsk A(%08x)  %08x ", 196668, value);
  if ( !pa_dr(0x6003006Cu, &value) )
    ds_printf("B(%08x)  %08x\n", 196716, value);
  if ( !pa_dr(0x60030040u, &value) )
    ds_printf("memdata   A(%08x)  %08x ", 196672, value);
  if ( !pa_dr(0x60030070u, &value) )
    ds_printf("B(%08x)  %08x\n", 196720, value);
  if ( !pa_dr(0x60030044u, &value) )
    ds_printf("memdatamsk A(%08x)  %08x ", 196676, value);
  if ( !pa_dr(0x60030074u, &value) )
    ds_printf("B(%08x)  %08x\n", 196724, value);
  if ( !pa_dr(0x60030048u, &value) )
    ds_printf("vblanking A(%08x)  %08x ", 196680, value);
  if ( !pa_dr(0x60030078u, &value) )
    ds_printf("B(%08x)  %08x\n", 196728, value);
  if ( !pa_dr(0x60030080u, &value) )
    ds_printf("timeout AA(%08x)  %08x ", 196736, value);
  if ( !pa_dr(0x60030084u, &value) )
    ds_printf("AB(%08x)  %08x\n", 196740, value);
  if ( !pa_dr(0x6003009Cu, &value) )
    ds_printf("counter  (%08x)  %08x\n", 196764, value);
  if ( !pa_dr(0x60030200u, &value) )
    ds_printf("IOPversion (%08x)  %08x\n", 197120, value);
  return 0;
}

int __cdecl pa_ver_cmd(int ac, char **av)
{
  if ( ac && ac != 1 )
    return ds_error("Usage: ver ");
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_ver_cmd", 24);
  if ( !cur_pointer )
    return ds_error("no memory");
  if ( !pa_ver((unsigned int *)cur_pointer) )
    ds_printf(
      "BID %08x MASTER %08x EEGS %08x GIF %08x IOP %08x\n",
      *((_DWORD *)cur_pointer + 1),
      *((_DWORD *)cur_pointer + 2),
      *((_DWORD *)cur_pointer + 3),
      *((_DWORD *)cur_pointer + 4),
      *((_DWORD *)cur_pointer + 5));
  return 0;
}

int __cdecl pa_start_cmd(int ac, char **av)
{
  if ( ac == 1 || !ac )
    return pa_start();
  else
    return ds_error("Usage: start");
}

int __cdecl pa_stop_cmd(int ac, char **av)
{
  if ( ac != 1 && ac )
    return ds_error("Usage: stop");
  pa_stop();
  return 0;
}

int __cdecl pa_dr_cmd(int ac, char **av)
{
  unsigned int data; // [esp+0h] [ebp-8h] BYREF
  unsigned int addr; // [esp+4h] [ebp-4h] BYREF

  if ( ac != 2 && ac != 1 )
    return ds_error("Usage: dr [<reg>]");
  if ( ac == 1 )
  {
    pa_dr_default();
  }
  else
  {
    if ( ds_eval_word(av[1], &addr) )
      return ds_error("Invalid argument");
    if ( (addr & 3) != 0 )
      return ds_error("Invalid register");
    if ( !pa_dr(addr, &data) )
      ds_printf("%08x : %08x\n", addr, data);
  }
  return 0;
}

static int __cdecl pa_puttrigpos(char **av)
{
  int r; // [esp+4h] [ebp-18h]
  unsigned int pos; // [esp+8h] [ebp-14h] BYREF
  unsigned int sec; // [esp+Ch] [ebp-10h]
  DSP_BUF *db; // [esp+10h] [ebp-Ch]
  DECI2_HDR *dh; // [esp+14h] [ebp-8h]
  unsigned int *ph; // [esp+18h] [ebp-4h]

  sec = 120;
  ds_eval_word(*av, &pos);
  if ( pos > 0x400 )
    return ds_error("Too large trigger position");
  db = ds_alloc_buf(1280, 72, 0, 8);
  if ( !db )
    return -1;
  dh = (DECI2_HDR *)db->buf;
  ph = (unsigned int *)&db->buf[8];
  *(_DWORD *)&db->buf[8] = 34;
  *++ph = pos;
  cur_state = 1;
  ds_send_desc(target_desc, db);
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_puttrigpos", 4);
  if ( cur_pointer )
  {
    do
      r = ds_select_desc(sec, 0);
    while ( r >= 0 && (cur_state & 1) != 0 );
    if ( r )
    {
      if ( *(_DWORD *)cur_pointer )
        pamp_error(*(_DWORD *)cur_pointer);
    }
    else
    {
      ds_error("Timeout\n");
    }
    ds_free_mem_low(cur_pointer, "pacons.c", "pa_puttrigpos");
    return 0;
  }
  else
  {
    ds_error("no memory");
    return -1;
  }
}

static int __cdecl pa_sr(int ac, char **av)
{
  int v3; // [esp+4h] [ebp-1Ch]
  unsigned int data; // [esp+8h] [ebp-18h] BYREF
  unsigned int reg; // [esp+Ch] [ebp-14h] BYREF
  unsigned int sec; // [esp+10h] [ebp-10h]
  DSP_BUF *db; // [esp+14h] [ebp-Ch]
  DECI2_HDR *dh; // [esp+18h] [ebp-8h]
  unsigned int *ph; // [esp+1Ch] [ebp-4h]

  sec = 120;
  ds_eval_word(*av, &reg);
  ds_eval_word(av[1], &data);
  if ( (reg & 3) != 0 )
    return ds_error("Invalid register");
  db = ds_alloc_buf(1280, 72, 0, 8);
  if ( !db )
    return -1;
  dh = (DECI2_HDR *)db->buf;
  ph = (unsigned int *)&db->buf[8];
  *(_DWORD *)&db->buf[8] = reg | 0x60000002;
  *++ph = data;
  cur_state = 1;
  ds_send_desc(target_desc, db);
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_sr", 4);
  if ( cur_pointer )
  {
    do
      v3 = ds_select_desc(sec, 0);
    while ( v3 >= 0 && (cur_state & 1) != 0 );
    if ( v3 )
    {
      if ( *(_DWORD *)cur_pointer )
        pamp_error(*(_DWORD *)cur_pointer);
    }
    else
    {
      ds_error("Timeout\n");
    }
    ds_free_mem_low(cur_pointer, "pacons.c", "pa_sr");
    return 0;
  }
  else
  {
    ds_error("no memory");
    return -1;
  }
}

int __cdecl Compare(trig_st *s, int count, char *str, unsigned int *val)
{
  int i; // [esp+0h] [ebp-4h]

  for ( i = 0; count > i && strcmp(s->str, str); ++i )
    ++s;
  if ( count == i )
    return 0;
  *val = i;
  return 1;
}

int __cdecl pa_gettrigpos(unsigned int *data)
{
  int v2; // [esp+8h] [ebp-18h]
  int r; // [esp+Ch] [ebp-14h]
  DSP_BUF *db; // [esp+14h] [ebp-Ch]

  v2 = -1;
  db = ds_alloc_buf(1280, 72, 0, 4);
  if ( !db )
    return -1;
  *(_DWORD *)&db->buf[8] = 32;
  cur_state = 1;
  ds_send_desc(target_desc, db);
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_gettrigpos", 8);
  if ( cur_pointer )
  {
    do
      r = ds_select_desc(120, 0);
    while ( r >= 0 && (cur_state & 1) != 0 );
    if ( r )
    {
      if ( *(_DWORD *)cur_pointer )
      {
        pamp_error(*(_DWORD *)cur_pointer);
      }
      else
      {
        v2 = *(_DWORD *)cur_pointer;
        *data = *((_DWORD *)cur_pointer + 1);
      }
    }
    else
    {
      ds_error("Timeout\n");
    }
    ds_free_mem_low(cur_pointer, "pacons.c", "pa_gettrigpos");
    return v2;
  }
  else
  {
    ds_error("no memory");
    return -1;
  }
}

static int __cdecl pa_getgif(unsigned int code, gifreg_t *gifreg)
{
  int r; // [esp+10h] [ebp-14h]
  DSP_BUF *db; // [esp+18h] [ebp-Ch]

  db = ds_alloc_buf(1280, 72, 0, 4);
  if ( !db )
    return -1;
  *(_DWORD *)&db->buf[8] = code;
  cur_state = 1;
  ds_send_desc(target_desc, db);
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_getgif", 28);
  if ( cur_pointer )
  {
    do
      r = ds_select_desc(120, 0);
    while ( r >= 0 && (cur_state & 1) != 0 );
    if ( r )
    {
      if ( *(_DWORD *)cur_pointer )
        pamp_error(*(_DWORD *)cur_pointer);
      else
        qmemcpy(gifreg, (char *)cur_pointer + 4, sizeof(gifreg_t));
    }
    else
    {
      ds_error("Timeout\n");
    }
    ds_free_mem_low(cur_pointer, "pacons.c", "pa_getgif");
    return 0;
  }
  else
  {
    ds_error("no memory");
    return -1;
  }
}

static int __cdecl pa_putgif(unsigned int code, gifreg_t *gifreg)
{
  int r; // [esp+10h] [ebp-14h]
  DSP_BUF *db; // [esp+18h] [ebp-Ch]

  db = ds_alloc_buf(1280, 72, 0, 28);
  if ( !db )
    return -1;
  *(_DWORD *)&db->buf[8] = code;
  qmemcpy(&db->buf[12], gifreg, 0x18u);
  cur_state = 1;
  ds_send_desc(target_desc, db);
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_putgif", 4);
  if ( cur_pointer )
  {
    do
      r = ds_select_desc(120, 0);
    while ( r >= 0 && (cur_state & 1) != 0 );
    if ( r )
    {
      if ( *(_DWORD *)cur_pointer )
        pamp_error(*(_DWORD *)cur_pointer);
    }
    else
    {
      ds_error("Timeout\n");
    }
    ds_free_mem_low(cur_pointer, "pacons.c", "pa_putgif");
    return 0;
  }
  else
  {
    ds_error("no memory");
    return -1;
  }
}

static int __cdecl pa_putiopmem(unsigned int code, iopmem_t *pmem)
{
  int r; // [esp+Ch] [ebp-14h]
  DSP_BUF *db; // [esp+14h] [ebp-Ch]

  db = ds_alloc_buf(1280, 72, 0, 20);
  if ( !db )
    return -1;
  *(_DWORD *)&db->buf[8] = code;
  *(_DWORD *)&db->buf[12] = pmem->addr;
  *(_QWORD *)&db->buf[16] = *(_QWORD *)&pmem->spu2core0;
  *(_DWORD *)&db->buf[24] = pmem->cdrom;
  cur_state = 1;
  ds_send_desc(target_desc, db);
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_putiopmem", 4);
  if ( cur_pointer )
  {
    do
      r = ds_select_desc(120, 0);
    while ( r >= 0 && (cur_state & 1) != 0 );
    if ( r )
    {
      if ( *(_DWORD *)cur_pointer )
        pamp_error(*(_DWORD *)cur_pointer);
    }
    else
    {
      ds_error("Timeout\n");
    }
    ds_free_mem_low(cur_pointer, "pacons.c", "pa_putiopmem");
    return 0;
  }
  else
  {
    ds_error("no memory");
    return -1;
  }
}

static int __cdecl pa_putreg(unsigned int code, unsigned int value)
{
  int r; // [esp+4h] [ebp-18h]
  DSP_BUF *db; // [esp+10h] [ebp-Ch]

  db = ds_alloc_buf(1280, 72, 0, 8);
  if ( !db )
    return -1;
  *(_DWORD *)&db->buf[8] = code;
  *(_DWORD *)&db->buf[12] = value;
  cur_state = 1;
  ds_send_desc(target_desc, db);
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_putreg", 4);
  if ( cur_pointer )
  {
    do
      r = ds_select_desc(120, 0);
    while ( r >= 0 && (cur_state & 1) != 0 );
    if ( r )
    {
      if ( *(_DWORD *)cur_pointer )
        pamp_error(*(_DWORD *)cur_pointer);
    }
    else
    {
      ds_error("Timeout\n");
    }
    ds_free_mem_low(cur_pointer, "pacons.c", "pa_putreg");
    return 0;
  }
  else
  {
    ds_error("no memory");
    return -1;
  }
}

static void __cdecl print_gifreg(gifreg_t *gifreg, unsigned int vblank)
{
  int i; // [esp+0h] [ebp-4h]

  ds_printf("[");
  if ( (vblank & 0x80000000) != 0 )
  {
    for ( i = 0; (unsigned int)i <= 6 && vblank != vblank_val[i].val; ++i )
      ;
    if ( i != 7 )
      ds_printf(" -v %s", vblank_val[i].str);
  }
  if ( (gifreg->addr & 0x80000000) != 0 )
  {
    ds_printf(" -r ");
    if ( (gifreg->addr & 0x20000000) != 0 )
      ds_printf("w");
    else
      ds_printf("r");
    if ( (gifreg->addr & 0x10000000) != 0 )
      ds_printf("s");
    else
      ds_printf("g");
    ds_printf(
      ":0x%x,0x%x=0x%08x%08x,0x%08x%08x",
      gifreg->addr & 0x7F,
      gifreg->amsk & 0x7F,
      gifreg->hidata,
      gifreg->lodata,
      gifreg->hidmsk,
      gifreg->lodmsk);
  }
  ds_printf(" ]");
}

static void __cdecl print_trig_gif(unsigned int reg)
{
  unsigned int vblank; // [esp+0h] [ebp-1Ch] BYREF
  gifreg_t gifreg; // [esp+4h] [ebp-18h] BYREF

  if ( reg == -2147483646 )
  {
    if ( pa_getgif(0x20010u, &gifreg) || pa_getreg(0x20014u, &vblank) )
      return;
    print_gifreg(&gifreg, vblank);
  }
  if ( reg == -2147483644 && !pa_getgif(0x20020u, &gifreg) && !pa_getreg(0x20024u, &vblank) )
    print_gifreg(&gifreg, vblank);
}

static void __cdecl print_trig(unsigned int reg)
{
  if ( reg != -2147483647 && reg > 0x80000001 && (reg == -2147483646 || reg == -2147483644) )
    print_trig_gif(reg);
}

int __cdecl pa_trig_get_cmd(int ac, char **av)
{
  unsigned int pos; // [esp+0h] [ebp-10h] BYREF
  unsigned int value; // [esp+4h] [ebp-Ch] BYREF
  int j; // [esp+8h] [ebp-8h]
  int i; // [esp+Ch] [ebp-4h]

  if ( !ac )
    return 0;
  if ( ac != 1 )
    return ds_error("Usage: trig");
  for ( i = 0; (unsigned int)i <= 3 && !pa_getreg(trig_getcode[i].val, &value); ++i )
  {
    for ( j = 0; (unsigned int)j <= 6 && trig_val[j].val != value; ++j )
      ;
    if ( j == 7 )
      break;
    ds_printf("%s = %s ", trig_getcode[i].str, trig_val[j].str);
    print_trig(trig_val[j].val);
    ds_printf("\n");
  }
  if ( pa_gettrigpos(&pos) >= 0 )
  {
    ds_printf("trigpos = %x\n", pos);
    pos >>= 6;
    while ( pos-- )
      ds_printf(" ");
    ds_printf("|\n");
    ds_printf("0--------------400\n");
  }
  return 0;
}

int __cdecl pa_trig_put_gif_cmd(int ac, char **av)
{
  char *v2; // eax
  char *v3; // eax
  char *v4; // eax
  char *v5; // eax
  char *pintc; // [esp+4h] [ebp-144Ch]
  char *pvblank; // [esp+8h] [ebp-1448h]
  char *q; // [esp+Ch] [ebp-1444h]
  char *q_1; // [esp+Ch] [ebp-1444h]
  char *q_2; // [esp+Ch] [ebp-1444h]
  char *q_3; // [esp+Ch] [ebp-1444h]
  char *q_4; // [esp+Ch] [ebp-1444h]
  char *p; // [esp+10h] [ebp-1440h]
  char *p_1; // [esp+10h] [ebp-1440h]
  char *p_2; // [esp+10h] [ebp-1440h]
  char *p_3; // [esp+10h] [ebp-1440h]
  char *p_4; // [esp+10h] [ebp-1440h]
  char vmsks[1024]; // [esp+14h] [ebp-143Ch] BYREF
  char vals[1024]; // [esp+414h] [ebp-103Ch] BYREF
  char amsks[1024]; // [esp+814h] [ebp-C3Ch] BYREF
  char adrs[1024]; // [esp+C14h] [ebp-83Ch] BYREF
  char type[1024]; // [esp+1014h] [ebp-43Ch] BYREF
  unsigned int index; // [esp+1414h] [ebp-3Ch]
  gifreg_t gifreg; // [esp+1418h] [ebp-38h] BYREF
  unsigned int vblank; // [esp+1430h] [ebp-20h] BYREF
  unsigned int v27; // [esp+1434h] [ebp-1Ch] BYREF
  unsigned int addr; // [esp+1438h] [ebp-18h] BYREF
  twin vmsk; // [esp+143Ch] [ebp-14h] BYREF
  twin val; // [esp+1444h] [ebp-Ch] BYREF
  int i; // [esp+144Ch] [ebp-4h]

  pvblank = 0;
  pintc = 0;
  for ( i = 0; (unsigned int)i <= 1; ++i )
  {
    if ( !strcmp(*av, gif_code[i].str) )
    {
      index = i;
      break;
    }
  }
  if ( i == 2 )
    return ds_error("Usage: [-v {high|low|up|down}] [-i gs] [-r {r|w}{g|s}[:<addr>[,<amsk>][=<data>[,<dmsk>]]]]");
  while ( ac > 0 )
  {
    if ( !strcmp("-v", *av) )
    {
      if ( --ac <= 0 )
        return ds_error("Usage: [-v {high|low|up|down}] [-i gs] [-r {r|w}{g|s}[:<addr>[,<amsk>][=<data>[,<dmsk>]]]]");
      pvblank = *++av;
    }
    else if ( !strcmp("-r", *av) )
    {
      gifreg.addr = 0x80000000;
      if ( --ac <= 0 )
        return ds_error("Usage: [-v {high|low|up|down}] [-i gs] [-r {r|w}{g|s}[:<addr>[,<amsk>][=<data>[,<dmsk>]]]]");
      p = *++av;
      q = type;
      while ( *p && *p != 58 )
        *q++ = *p++;
      *q = 0;
      v2 = p;
      if ( *p == 58 )
        v2 = p + 1;
      p_1 = v2;
      q_1 = adrs;
      while ( *p_1 && *p_1 != 44 && *p_1 != 61 )
        *q_1++ = *p_1++;
      *q_1 = 0;
      v3 = p_1;
      if ( *p_1 == 44 )
        v3 = p_1 + 1;
      p_2 = v3;
      q_2 = amsks;
      while ( *p_2 && *p_2 != 61 )
        *q_2++ = *p_2++;
      *q_2 = 0;
      v4 = p_2;
      if ( *p_2 == 61 )
        v4 = p_2 + 1;
      p_3 = v4;
      q_3 = vals;
      while ( *p_3 && *p_3 != 44 )
        *q_3++ = *p_3++;
      *q_3 = 0;
      v5 = p_3;
      if ( *p_3 == 44 )
        v5 = p_3 + 1;
      p_4 = v5;
      q_4 = vmsks;
      while ( *p_4 )
        *q_4++ = *p_4++;
      *q_4 = 0;
    }
    else if ( !strcmp("-i", *av) )
    {
      if ( --ac <= 0 )
        return ds_error("Usage: [-v {high|low|up|down}] [-i gs] [-r {r|w}{g|s}[:<addr>[,<amsk>][=<data>[,<dmsk>]]]]");
      pintc = *++av;
    }
    --ac;
    ++av;
  }
  if ( pintc )
  {
    if ( pa_putreg(gif_code[index].putintc, 0x80000001) )
      return -1;
  }
  else if ( pa_putreg(gif_code[index].putintc, 0x80000000) )
  {
    return -1;
  }
  if ( pvblank )
  {
    if ( !Compare(vblank_val, 4, pvblank, &vblank) )
      return ds_error("Usage: [-v {high|low|up|down}] [-i gs] [-r {r|w}{g|s}[:<addr>[,<amsk>][=<data>[,<dmsk>]]]]");
    if ( pa_putreg(gif_code[index].putvblank, vblank_val[vblank].val) )
      return -1;
  }
  else if ( pa_putreg(gif_code[index].putvblank, 0) )
  {
    return -1;
  }
  if ( gifreg.addr == 0x80000000 )
  {
    if ( type[0] == 119 )
    {
      gifreg.addr += 0x20000000;
    }
    else if ( type[0] != 114 )
    {
      return ds_error("Usage: [-v {high|low|up|down}] [-i gs] [-r {r|w}{g|s}[:<addr>[,<amsk>][=<data>[,<dmsk>]]]]");
    }
    if ( type[1] == 115 )
    {
      gifreg.addr += 0x10000000;
LABEL_70:
      if ( ds_eval_word(adrs, &addr) )
        addr = 127;
      if ( ds_eval_word(amsks, &v27) )
        v27 = 127;
      if ( ds_eval_twin(vals, &val) )
      {
        val.xa[0] = -1;
        val.xa[1] = -1;
      }
      if ( ds_eval_twin(vmsks, &vmsk) )
      {
        vmsk.xa[0] = -1;
        vmsk.xa[1] = -1;
      }
      gifreg.hidata = val.xa[1];
      gifreg.lodata = ds_ctw(val);
      gifreg.hidmsk = vmsk.xa[1];
      gifreg.lodmsk = ds_ctw(vmsk);
      gifreg.addr += addr;
      gifreg.amsk = v27;
      goto LABEL_80;
    }
    if ( type[1] == 103 )
      goto LABEL_70;
    return ds_error("Usage: [-v {high|low|up|down}] [-i gs] [-r {r|w}{g|s}[:<addr>[,<amsk>][=<data>[,<dmsk>]]]]");
  }
  memset(&gifreg, 0, sizeof(gifreg));
LABEL_80:
  if ( pa_putgif(gif_code[index].putreg, &gifreg) )
    return -1;
  else
    return 0;
}

// local variable allocation has failed, the output may be wrong!
int __cdecl pa_trig_put_iop_cmd(int ac, char **av)
{
  char *v2; // eax
  char *v3; // eax
  char *v4; // eax
  char *v5; // eax
  char *ptype; // [esp+4h] [ebp-1444h]
  char *pintr; // [esp+8h] [ebp-1440h]
  char *pvblank; // [esp+Ch] [ebp-143Ch]
  char *q; // [esp+10h] [ebp-1438h]
  char *q_1; // [esp+10h] [ebp-1438h]
  char *q_2; // [esp+10h] [ebp-1438h]
  char *q_3; // [esp+10h] [ebp-1438h]
  char *q_4; // [esp+10h] [ebp-1438h]
  char *p; // [esp+14h] [ebp-1434h]
  char *p_1; // [esp+14h] [ebp-1434h]
  char *p_2; // [esp+14h] [ebp-1434h]
  char *p_3; // [esp+14h] [ebp-1434h]
  char *p_4; // [esp+14h] [ebp-1434h]
  char vmsks[1024]; // [esp+18h] [ebp-1430h] BYREF
  char vals[1024]; // [esp+418h] [ebp-1030h] BYREF
  char amsks[1024]; // [esp+818h] [ebp-C30h] BYREF
  char adrs[1024]; // [esp+C18h] [ebp-830h] BYREF
  char type[1024]; // [esp+1018h] [ebp-430h] BYREF
  unsigned int intr; // [esp+1418h] [ebp-30h] BYREF
  unsigned int index; // [esp+141Ch] [ebp-2Ch]
  iopmem_t s; // [esp+1420h] [ebp-28h] OVERLAPPED BYREF

  pvblank = 0;
  pintr = 0;
  ptype = type;
  memset(&s, 0, 0x10u);
  for ( s.addrmask = 0; s.addrmask <= 1; ++s.addrmask )
  {
    if ( !strcmp(*av, iop_code[s.addrmask].str) )
    {
      index = s.addrmask;
      break;
    }
  }
  if ( s.addrmask == 2 || ac == 1 )
    return ds_error("Usage: [-v {high|low|up|down}] [-m {core0|core1|cdrom|dev9|sif}{r|w|a}[:<addr>[,<amsk>][=<data>[,<dm"
                    "sk>]]]] [-i spu2]");
  while ( ac > 0 )
  {
    if ( !strcmp("-v", *av) )
    {
      if ( --ac <= 0 )
        return ds_error("Usage: [-v {high|low|up|down}] [-m {core0|core1|cdrom|dev9|sif}{r|w|a}[:<addr>[,<amsk>][=<data>["
                        ",<dmsk>]]]] [-i spu2]");
      pvblank = *++av;
    }
    else if ( !strcmp("-i", *av) )
    {
      if ( --ac <= 0 )
        return ds_error("Usage: [-v {high|low|up|down}] [-m {core0|core1|cdrom|dev9|sif}{r|w|a}[:<addr>[,<amsk>][=<data>["
                        ",<dmsk>]]]] [-i spu2]");
      pintr = *++av;
    }
    else if ( !strcmp("-m", *av) )
    {
      HIBYTE(s.addr) |= 0x80u;
      if ( --ac <= 0 )
        return ds_error("Usage: [-v {high|low|up|down}] [-m {core0|core1|cdrom|dev9|sif}{r|w|a}[:<addr>[,<amsk>][=<data>["
                        ",<dmsk>]]]] [-i spu2]");
      p = *++av;
      q = type;
      while ( *p && *p != 58 )
        *q++ = *p++;
      *q = 0;
      v2 = p;
      if ( *p == 58 )
        v2 = p + 1;
      p_1 = v2;
      q_1 = adrs;
      while ( *p_1 && *p_1 != 44 && *p_1 != 61 )
        *q_1++ = *p_1++;
      *q_1 = 0;
      v3 = p_1;
      if ( *p_1 == 44 )
        v3 = p_1 + 1;
      p_2 = v3;
      q_2 = amsks;
      while ( *p_2 && *p_2 != 61 )
        *q_2++ = *p_2++;
      *q_2 = 0;
      v4 = p_2;
      if ( *p_2 == 61 )
        v4 = p_2 + 1;
      p_3 = v4;
      q_3 = vals;
      while ( *p_3 && *p_3 != 44 )
        *q_3++ = *p_3++;
      *q_3 = 0;
      v5 = p_3;
      if ( *p_3 == 44 )
        v5 = p_3 + 1;
      p_4 = v5;
      q_4 = vmsks;
      while ( *p_4 )
        *q_4++ = *p_4++;
      *q_4 = 0;
    }
    --ac;
    ++av;
  }
  if ( pvblank )
  {
    if ( !Compare(vblank_val, 4, pvblank, &s.dev9) )
      return ds_error("Usage: [-v {high|low|up|down}] [-m {core0|core1|cdrom|dev9|sif}{r|w|a}[:<addr>[,<amsk>][=<data>[,<"
                      "dmsk>]]]] [-i spu2]");
    if ( pa_putreg(iop_code[index].putvblank, vblank_val[s.dev9].val) )
      return -1;
  }
  else if ( pa_putreg(iop_code[index].putvblank, 0) )
  {
    return -1;
  }
  if ( pintr )
  {
    if ( !Compare(intr_val, 1, pintr, &intr) )
      return ds_error("Usage: [-v {high|low|up|down}] [-m {core0|core1|cdrom|dev9|sif}{r|w|a}[:<addr>[,<amsk>][=<data>[,<"
                      "dmsk>]]]] [-i spu2]");
    if ( pa_putreg(iop_code[index].putintc, intr_val[intr].val) )
      return -1;
  }
  else if ( pa_putreg(iop_code[index].putintc, 0) )
  {
    return -1;
  }
  if ( (s.addr & 0x80000000) != 0 )
  {
    if ( !ds_strncmp("core0", type, 5) )
    {
      HIBYTE(s.addr) |= 1u;
      ptype = &type[5];
    }
    if ( !ds_strncmp("core1", ptype, 5) )
    {
      HIBYTE(s.addr) |= 2u;
      ptype += 5;
    }
    if ( !ds_strncmp("cdrom", ptype, 5) )
    {
      HIBYTE(s.addr) |= 4u;
      ptype += 5;
    }
    if ( !ds_strncmp("dev9", ptype, 4) )
    {
      HIBYTE(s.addr) |= 8u;
      ptype += 4;
    }
    if ( !ds_strncmp("sif", ptype, 3) )
    {
      HIBYTE(s.addr) |= 0x10u;
      ptype += 3;
    }
    if ( (s.addr & 0x10000000) == 0
      || !(((s.addr & 0x2000000) != 0) | HIBYTE(s.addr) & 1 | ((s.addr & 0x4000000) != 0 || (s.addr & 0x8000000) != 0)) )
    {
      switch ( *ptype )
      {
        case 'w':
          HIBYTE(s.addr) |= 0x40u;
LABEL_85:
          memset(&s.sif, 0, 16);
          if ( adrs[0] )
          {
            if ( ds_eval_word(adrs, &s.read) )
              s.read = 0xFFFFFFF;
            if ( ds_eval_word(amsks, &s.sif) )
              s.sif = 0xFFFFFFF;
          }
          if ( vals[0] )
          {
            if ( ds_eval_word(vals, &s.enable) )
              s.enable = -1;
            if ( ds_eval_word(vmsks, &s.write) )
              s.write = -1;
          }
          s.spu2core1 = s.enable;
          s.cdrom = s.write;
          s.addr &= 0xFF000000;
          s.addr |= s.read & 0xFFFFFF;
          s.spu2core0 = s.sif;
          goto LABEL_96;
        case 'r':
          HIBYTE(s.addr) |= 0x20u;
          goto LABEL_85;
        case 'a':
          HIBYTE(s.addr) |= 0x40u;
          HIBYTE(s.addr) |= 0x20u;
          goto LABEL_85;
      }
    }
    return ds_error("Usage: [-v {high|low|up|down}] [-m {core0|core1|cdrom|dev9|sif}{r|w|a}[:<addr>[,<amsk>][=<data>[,<dm"
                    "sk>]]]] [-i spu2]");
  }
LABEL_96:
  if ( pa_putiopmem(iop_code[index].putdata, &s) )
    return -1;
  else
    return 0;
}

int __cdecl pa_trig_put_cmd(int ac, char **av)
{
  int i; // [esp+0h] [ebp-Ch]
  int i_1; // [esp+0h] [ebp-Ch]
  int code; // [esp+4h] [ebp-8h]
  int value; // [esp+8h] [ebp-4h]

  if ( !ac )
    return 0;
  if ( ac != 2 )
    return ds_error("Usage: %s <{eegs|gifa|gifb|switch|none}>", *av);
  for ( i = 0; (unsigned int)i <= 3; ++i )
  {
    if ( !strcmp(*av, trig_putcode[i].str) )
    {
      code = trig_putcode[i].val;
      break;
    }
  }
  if ( i == 4 )
    return ds_error("Usage: %s <{eegs|gifa|gifb|switch|none}>", *av);
  for ( i_1 = 0; (unsigned int)i_1 <= 6; ++i_1 )
  {
    if ( !strcmp(av[1], trig_val[i_1].str) )
    {
      value = trig_val[i_1].val;
      break;
    }
  }
  if ( i_1 == 7 )
    return ds_error("Usage: %s <{eegs|gifa|gifb|switch|none}>", *av);
  return pa_putreg(code, value);
}

int __cdecl pa_sr_cmd(int ac, char **av)
{
  if ( ac != 3 )
    return ds_error("Usage: sr <reg> <val>");
  pa_sr(2, av + 1);
  return 0;
}

int __cdecl pa_trigpos_cmd(int ac, char **av)
{
  if ( ac != 2 )
    return ds_error("Usage: trigpos [<val>]");
  pa_puttrigpos(av + 1);
  return 0;
}

int __cdecl pa_ce_cmd(int ac, char **av)
{
  char *p; // [esp+0h] [ebp-24h]
  int v4; // [esp+4h] [ebp-20h]
  int left; // [esp+Ch] [ebp-18h]
  unsigned int offset; // [esp+10h] [ebp-14h] BYREF
  unsigned int size; // [esp+14h] [ebp-10h] BYREF
  unsigned int mode; // [esp+18h] [ebp-Ch] BYREF
  VERIFY_PFM verify; // [esp+1Ch] [ebp-8h] BYREF

  if ( ac != 5 )
    return ds_error("Usage : ce <name> <mode> <offset> <size>");
  if ( ds_eval_word_ex(av[2], &mode) )
    return ds_error("Invalid argument");
  if ( ds_eval_word(av[3], &offset) || ds_eval_word(av[4], &size) )
    return ds_error("Invalid argument");
  left = size;
  if ( size > 0x800 )
    size = 2048;
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_ce_cmd", 10 * size + 4);
  if ( cur_pointer )
  {
    p = (char *)cur_pointer + 4;
    reset_pfm(&verify, av[1]);
    while ( left > 0 )
    {
      if ( !pa_de((unsigned int *)cur_pointer, offset, size) )
      {
        v4 = check_pfm(&verify, p, mode, 10 * size, av[1]);
        if ( v4 )
          break;
      }
      left -= size;
      offset += size;
      if ( size > left )
        size = left;
    }
    if ( !strcmp(av[1], "DISP_Check") )
      v4 = result_pfm(&verify, mode, av[1]);
    ds_free_mem_low(cur_pointer, "pacons.c", "pa_ce_cmd");
    return v4;
  }
  else
  {
    ds_error("no memory");
    return ds_error("no memory");
  }
}

int __cdecl pa_cei_cmd(int ac, char **av)
{
  char *bp; // [esp+0h] [ebp-30h]
  int v4; // [esp+4h] [ebp-2Ch]
  unsigned int data; // [esp+Ch] [ebp-24h] BYREF
  unsigned int total; // [esp+10h] [ebp-20h]
  int v7; // [esp+14h] [ebp-1Ch]
  unsigned int offset; // [esp+18h] [ebp-18h] BYREF
  unsigned int size; // [esp+1Ch] [ebp-14h] BYREF
  unsigned int mode; // [esp+20h] [ebp-10h] BYREF
  int pat; // [esp+24h] [ebp-Ch]
  VERIFY_PFM verify; // [esp+28h] [ebp-8h] BYREF

  if ( ac != 5 )
    return ds_error("Usage : cei <name> <mode> <offset> <size>");
  if ( ds_eval_word(av[2], &mode) )
    return ds_error("Invalid argument");
  if ( ds_eval_word(av[3], &offset) || ds_eval_word(av[4], &size) )
    return ds_error("Invalid argument");
  pa_dr(0x20004u, &data);
  m_sdram = data >> 4;
  v7 = size;
  total = size;
  pat = 0;
  m_init = 1;
  if ( size > 0x8000 )
    size = 0x8000;
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_cei_cmd", size + 4);
  if ( cur_pointer )
  {
    bp = (char *)cur_pointer + 4;
    while ( v7 > 0 )
    {
      if ( !pa_mem(0x10000000u, (unsigned int *)cur_pointer, offset, size) )
      {
        v4 = check_pfm(&verify, bp, mode, size, av[1]);
        if ( v4 )
          break;
      }
      v7 -= size;
      offset += size;
      if ( size > v7 )
        size = v7;
    }
    ds_free_mem_low(cur_pointer, "pacons.c", "pa_cei_cmd");
    return v4;
  }
  else
  {
    ds_error("no memory");
    return ds_error("no memory");
  }
}

int __cdecl pa_cgi_cmd(int ac, char **av)
{
  char *bp; // [esp+0h] [ebp-24h]
  int ret; // [esp+4h] [ebp-20h]
  unsigned int data; // [esp+Ch] [ebp-18h] BYREF
  unsigned int v6; // [esp+10h] [ebp-14h]
  int left; // [esp+14h] [ebp-10h]
  unsigned int offset; // [esp+18h] [ebp-Ch] BYREF
  unsigned int size; // [esp+1Ch] [ebp-8h] BYREF
  unsigned int mode; // [esp+20h] [ebp-4h] BYREF

  if ( ac != 5 )
    return ds_error("Usage : cgi <name> <mode> <offset> <size>");
  if ( ds_eval_word(av[2], &mode) )
    return ds_error("Invalid argument");
  if ( ds_eval_word(av[3], &offset) || ds_eval_word(av[4], &size) )
    return ds_error("Invalid argument");
  pa_dr(0x20004u, &data);
  m_sdram = data >> 2;
  left = size;
  v6 = size;
  memset(&gif, 0, sizeof(gif));
  if ( size > 0x8000 )
    size = 0x8000;
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_cgi_cmd", size + 4);
  if ( !cur_pointer )
  {
    ds_error("no memory");
    return ds_error("no memory");
  }
  bp = (char *)cur_pointer + 4;
  if ( !strcmp(av[1], "GIF_EXAMPLECheck") )
  {
    if ( !DataFileRead1() )
      return ds_error("FileReadError");
  }
  else if ( !strcmp(av[1], "GIF_TEXSWAPCheck") && !DataFileRead2() )
  {
    return ds_error("FileReadError");
  }
  while ( left > 0 )
  {
    if ( !pa_mem(0x20000000u, (unsigned int *)cur_pointer, offset, size) )
    {
      ret = check_gif(bp, mode, size, av[1]);
      if ( ret )
        break;
    }
    left -= size;
    offset += size;
    if ( size > left )
      size = left + 1;
  }
  if ( !strcmp(av[1], "GIF_EXAMPLECheck") )
  {
    MemoryFree1();
  }
  else if ( !strcmp(av[1], "GIF_TEXSWAPCheck") )
  {
    MemoryFree2();
    if ( gif.m_complete != 4 )
    {
      ds_printf("MODE1 Error %d \n", gif.m_error1);
      ds_printf("MODE2 Error %d \n", gif.m_error2);
      ds_printf("MODE3 Error %d \n", gif.m_error3);
      ds_printf("MODE4 Error %d \n", gif.m_error4);
      return ds_error("GIF_TEXSWAPCheck Error %d \n", gif.m_complete);
    }
  }
  ds_free_mem_low(cur_pointer, "pacons.c", "pa_cgi_cmd");
  return ret;
}

int __cdecl pa_cii_cmd(int ac, char **av)
{
  char *bp; // [esp+0h] [ebp-334h]
  int ret; // [esp+4h] [ebp-330h]
  unsigned int data; // [esp+Ch] [ebp-328h] BYREF
  unsigned int v6; // [esp+10h] [ebp-324h]
  int left; // [esp+14h] [ebp-320h]
  unsigned int offset; // [esp+18h] [ebp-31Ch] BYREF
  unsigned int size; // [esp+1Ch] [ebp-318h] BYREF
  unsigned int mode; // [esp+20h] [ebp-314h] BYREF
  VERIFY_SSS verify; // [esp+24h] [ebp-310h] BYREF

  memset(&verify, 0, sizeof(verify));
  verify.step = 10;
  verify.type = 1;
  if ( ac <= 4 )
    return ds_error("Usage : cii <name> <mode> <offset> <size>");
  if ( ds_eval_word_ex(av[2], &mode) )
    return ds_error("Invalid argument");
  if ( ds_eval_word(av[3], &offset) || ds_eval_word(av[4], &size) || SetBusType(&verify, av) )
    return ds_error("Invalid argument");
  pa_dr(0x30004u, &data);
  m_sdram = data >> 4;
  m_init = 1;
  left = size;
  v6 = size;
  if ( size > 0x8000 )
    size = 0x8000;
  cur_pointer = ds_alloc_mem_low("pacons.c", "pa_cii_cmd", size + 4);
  if ( cur_pointer )
  {
    bp = (char *)cur_pointer + 4;
    while ( left > 0
         && (pa_mem(0x30000000u, (unsigned int *)cur_pointer, offset, size) || !check_sss(&verify, bp, size, av)) )
    {
      left -= size;
      offset += size;
      if ( size > left )
        size = left;
    }
    ret = result_sss(&verify);
    ds_free_mem_low(cur_pointer, "pacons.c", "pa_cii_cmd");
    return ret;
  }
  else
  {
    ds_error("no memory");
    return ds_error("no memory");
  }
}

int __cdecl pa_cr_cmd(int ac, char **av)
{
  unsigned int data; // [esp+0h] [ebp-18h] BYREF
  unsigned int v4; // [esp+4h] [ebp-14h] BYREF
  unsigned int value; // [esp+8h] [ebp-10h] BYREF
  unsigned int addr; // [esp+Ch] [ebp-Ch] BYREF
  unsigned int count; // [esp+10h] [ebp-8h] BYREF
  unsigned int loop; // [esp+14h] [ebp-4h]

  loop = 0;
  if ( ac != 5 )
    return ds_error("Usage: cr [<reg> <value> <mask> <count>]");
  if ( ds_eval_word(av[1], &addr) )
    return ds_error("Invalid argument");
  if ( (addr & 3) != 0 )
    return ds_error("Invalid register");
  if ( ds_eval_word(av[2], &value) )
    return ds_error("Invalid argument");
  if ( ds_eval_word(av[3], &v4) || ds_eval_word(av[4], &count) )
    return ds_error("Invalid argument");
  while ( 1 )
  {
    while ( pa_dr(addr, &data) )
      ;
    if ( value == (v4 & data) )
    {
      m_result = 0;
      return m_result;
    }
    if ( count == loop )
      break;
    ++loop;
  }
  m_result = 1;
  return m_result;
}

int __cdecl main(int ac, char **av)
{
  int argc_; // [esp+Ch] [ebp+8h]
  char **argv_; // [esp+10h] [ebp+Ch]

  ds_program_name = ds_basename(*av);
  device_name = ds_getenv("DSNETM");
  if ( !ac )
    ds_printf("%s (%s)\n", ds_program_name, ds_stamp_str[0]);
  opt_netdev = ds_set_option("netdev", 3, "", 0, 1);
  ds_opt_standard_init();
  opt_iopconf = ds_set_option("IOPCONF", 3, "/usr/local/sce/iop/modules", 0, 1);
  opt_iopmodules = ds_set_option("IOPMODULES", 3, "/usr/local/sce/iop/modules", 0, 1);
  ds_read_option_file();
  argc_ = ac - 1;
  for ( argv_ = av + 1; argc_ > 0 && **argv_ == 45; ++argv_ )
  {
    if ( !strcmp("-d", *argv_) )
    {
      usage(--argc_ <= 0);
      device_name = *++argv_;
    }
    else if ( !strcmp("-p", *argv_) )
    {
      usage(--argc_ <= 0);
      ++argv_;
    }
    else if ( !strcmp("-netdev", *argv_) )
    {
      usage(--argc_ <= 0);
      ds_set_option("netdev", 3, *++argv_, 0, 0);
    }
    else if ( !strcmp("-nokbd", *argv_) )
    {
      ++no_kbd;
    }
    else
    {
      usage(1);
    }
    --argc_;
  }
  argc = argc_;
  argv = argv_;
  ds_cmd_install("triga", "", "set trigger a", pa_trig_put_cmd);
  ds_cmd_install("trigb", "", "set trigger b", pa_trig_put_cmd);
  ds_cmd_install("trigc", "", "set trigger c", pa_trig_put_cmd);
  ds_cmd_install("trigd", "", "set trigger d", pa_trig_put_cmd);
  ds_cmd_install("gifa", "", "set gif-a trigger", pa_trig_put_gif_cmd);
  ds_cmd_install("gifb", "", "set gif-b trigger", pa_trig_put_gif_cmd);
  ds_cmd_install("iopa", "", "set iop-a trigger", pa_trig_put_iop_cmd);
  ds_cmd_install("iopb", "", "set iop-b trigger", pa_trig_put_iop_cmd);
  ds_cmd_install("trig", "", "display trigger condition", pa_trig_get_cmd);
  ds_cmd_install("trigpos", "", "set trigger position", pa_trigpos_cmd);
  ds_cmd_install("de", "", "display ee data", pa_de_cmd);
  ds_cmd_install("dg", "", "display gif data", pa_dg_cmd);
  ds_cmd_install("se", "", "save ee data", pa_se_cmd);
  ds_cmd_install("sg", "", "save gif data", pa_sg_cmd);
  ds_cmd_install("si", "", "save iop data", pa_si_cmd);
  ds_cmd_install("reset", "", "reset pa", pa_reset_cmd);
  ds_cmd_install("ver", "", "display version", pa_ver_cmd);
  ds_cmd_install("start", "", "start sampling", pa_start_cmd);
  ds_cmd_install("stop", "", "stop sampling", pa_stop_cmd);
  ds_cmd_install("wait", "", "wait for stop of sampling", pa_wait_cmd);
  ds_cmd_install("dr", "", "display register", pa_dr_cmd);
  ds_cmd_install("sr", "", "save register", pa_sr_cmd);
  ds_cmd_install("dei", "", "display ee data", pa_dei_cmd);
  ds_cmd_install("dgi", "", "display gif data", pa_dgi_cmd);
  ds_cmd_install("dii", "", "display iop data", pa_dii_cmd);
  ds_cmd_install("sei", "", "save eegs data", pa_sei_cmd);
  ds_cmd_install("sgi", "", "save gif data", pa_sgi_cmd);
  ds_cmd_install("sii", "", "save iop data", pa_sii_cmd);
  ds_cmd_install("ce", "", "check ee data", pa_ce_cmd);
  ds_cmd_install("cei", "", "check eegs data", pa_cei_cmd);
  ds_cmd_install("cgi", "", "check gif data", pa_cgi_cmd);
  ds_cmd_install("cii", "", "check iop data", pa_cii_cmd);
  ds_cmd_install("cr", "", "check register data", pa_cr_cmd);
  if ( (!argc || !no_kbd) && !ds_open_kbd(ds_cmd_input, 0) )
    ds_exit(131);
  ds_cmd_standard_install(1);
  connect_dev(device_name, 1);
  ds_read_startup_file();
  if ( argc_ > 0 )
    batch(argc_, argv_);
  ds_idle_func = idle_func;
  while ( ds_select_desc(1, 0) >= 0 )
    idle_func();
  return ds_exit(0);
}

