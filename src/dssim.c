#include "dsnet_prototypes.h"

char *ds_stamp_str[2] = { DSNET_STAMP_STRING, DSNET_VERSION_STRING };
char *ds_version_str = DSNET_VERSION_STRING;

static int target_mem_size = 256;
static int cbrk = 0;
static int dsip_id = 20;

static DSP_BUF *last_db_2 = NULL;

// FIXME: VLAs nested in structs cause ICE in GCC
static ILOADP_MODINFO_DATA mi_01 = {
      257u,
      0u,
      2080u,
      2656u,
      48u,
      16u,
      0u,
      0u,
      0u,
      0u,
      0u,
      { 1953724787u, 1830841701u, 1919905125u, 1634541689u, 1701273966u }
    };
static ILOADP_MODINFO_DATA mi_02 = {
      257u,
      0u,
      5152u,
      5328u,
      64u,
      32u,
      0u,
      0u,
      0u,
      0u,
      0u,
      { 1684107116u, 1663070821u, 6648431u }
    };
static ILOADP_MODINFO_DATA mi_03 = {
      257u,
      0u,
      10784u,
      1744u,
      48u,
      80u,
      0u,
      0u,
      0u,
      0u,
      0u,
      { 1701017701u, 1869182064u, 1634541678u, 1701273966u }
    };
static ILOADP_MODINFO_DATA mi_04 = {
      257u,
      0u,
      13088u,
      2816u,
      112u,
      16u,
      0u,
      0u,
      0u,
      0u,
      0u,
      { 1702129225u, 1886745202u, 1632444532u, 1701273966u }
    };
static ILOADP_MODINFO_DATA mi_05 = {
      257u,
      0u,
      16160u,
      5888u,
      736u,
      0u,
      0u,
      0u,
      0u,
      0u,
      0u,
      { 1953724787u, 1126198629u, 1651076128u }
    };
static ILOADP_MODINFO_DATA mi_06 = {
      257u,
      0u,
      22816u,
      6400u,
      1216u,
      928u,
      0u,
      0u,
      0u,
      0u,
      0u,
      { 1177505609u, 543517801u, 1634623821u, 7497063u }
    };
static ILOADP_MODINFO_DATA mi_07 = { 257u, 0u, 31520u, 1264u, 64u, 16u, 0u, 0u, 0u, 0u, 0u, { 1768191091u } };
static ILOADP_MODINFO_DATA mi_08 = {
      257u,
      0u,
      33056u,
      4928u,
      272u,
      2752u,
      0u,
      0u,
      0u,
      0u,
      0u,
      { 542134089u, 541477203u, 1634623853u, 7497063u }
    };
static ILOADP_MODINFO_DATA mi_09 = {
      257u,
      0u,
      41248u,
      12048u,
      2368u,
      33872u,
      0u,
      0u,
      0u,
      0u,
      0u,
      { 1768121668u, 1634541618u, 1701273966u }
    };
static ILOADP_MODINFO_DATA mi_10 = {
      257u,
      0u,
      24096u,
      4000u,
      800u,
      48u,
      0u,
      0u,
      0u,
      0u,
      0u,
      {
        1768121668u,
        1229987890u,
        1852383302u,
        1718773108u,
        543515489u,
        1986622052u
      }
    };
static ILOADP_MODINFO_DATA mi_11 = {
      257u,
      0u,
      29216u,
      2752u,
      784u,
      48u,
      0u,
      0u,
      0u,
      0u,
      0u,
      {
        1768121668u,
        1230184498u,
        1763717702u,
        1919251566u,
        1701011814u,
        1769104416u,
        7497078u
      }
    };
static ILOADP_MODINFO_DATA mi_12 = {
      257u,
      0u,
      33056u,
      6096u,
      704u,
      704u,
      0u,
      0u,
      0u,
      0u,
      0u,
      { 1768121668u, 1414799410u, 1229336409u, 1679836492u, 1702259058u }
    };
static ILOADP_MODINFO_DATA mi_13 = {
      257u,
      0u,
      40736u,
      5360u,
      688u,
      640u,
      0u,
      0u,
      0u,
      0u,
      0u,
      { 1768121668u, 1869357106u, 1830839393u, 1734438497u }
    };
static ILOADP_MODINFO_DATA mi_14 = {
      257u,
      0u,
      43968u,
      8192u,
      256u,
      512u,
      0u,
      0u,
      0u,
      0u,
      0u,
      { 1953264973u, 1750359913u, 1684104562u, 1851870559u, 1919248225u }
    };
#define EMPTY_MI(ind) \
      static ILOADP_MODINFO_DATA mi_##ind = { \
            0u, \
            0u, \
            0u, \
            0u, \
            0u, \
            0u, \
            0u, \
            0u, \
            0u, \
            0u, \
            0u, \
            { \
              0u \
            } \
          };
EMPTY_MI(15)
EMPTY_MI(16)
EMPTY_MI(17)
EMPTY_MI(18)
EMPTY_MI(19)
EMPTY_MI(20)
EMPTY_MI(21)
EMPTY_MI(22)
EMPTY_MI(23)
EMPTY_MI(24)
EMPTY_MI(25)
EMPTY_MI(26)
EMPTY_MI(27)
EMPTY_MI(28)
EMPTY_MI(29)
EMPTY_MI(30)
EMPTY_MI(31)
EMPTY_MI(32)
EMPTY_MI(33)
EMPTY_MI(34)
EMPTY_MI(35)
EMPTY_MI(36)
EMPTY_MI(37)
EMPTY_MI(38)
EMPTY_MI(39)

static struct id_mi_name id_mi_names[] =
{
  {
    1,
    &mi_01,
    {
      's',
      'y',
      's',
      't',
      'e',
      'm',
      ' ',
      'm',
      'e',
      'm',
      'o',
      'r',
      'y',
      ' ',
      'm',
      'a',
      'n',
      'a',
      'g',
      'e',
      'r',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0'
    }
  },
  {
    2,
    &mi_02,
    {
      'l',
      'o',
      'a',
      'd',
      'e',
      'r',
      ' ',
      'c',
      'o',
      'r',
      'e',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0'
    }
  },
  {
    4,
    &mi_03,
    {
      'e',
      'x',
      'c',
      'e',
      'p',
      't',
      'i',
      'o',
      'n',
      ' ',
      'm',
      'a',
      'n',
      'a',
      'g',
      'e',
      'r',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0'
    }
  },
  {
    5,
    &mi_04,
    {
      'I',
      'n',
      't',
      'e',
      'r',
      'r',
      'u',
      'p',
      't',
      ' ',
      'M',
      'a',
      'n',
      'a',
      'g',
      'e',
      'r',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0'
    }
  },
  {
    6,
    &mi_05,
    {
      's',
      'y',
      's',
      't',
      'e',
      'm',
      ' ',
      'C',
      ' ',
      'l',
      'i',
      'b',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0'
    }
  },
  {
    7,
    &mi_06,
    {
      'I',
      'O',
      '/',
      'F',
      'i',
      'l',
      'e',
      ' ',
      'M',
      'a',
      'n',
      'a',
      'g',
      'e',
      'r',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0'
    }
  },
  {
    8,
    &mi_07,
    {
      's',
      't',
      'd',
      'i',
      'o',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0'
    }
  },
  {
    9,
    &mi_08,
    {
      'I',
      'O',
      'P',
      ' ',
      'S',
      'I',
      'F',
      ' ',
      'm',
      'a',
      'n',
      'a',
      'g',
      'e',
      'r',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0'
    }
  },
  {
    10,
    &mi_09,
    {
      'D',
      'e',
      'c',
      'i',
      '2',
      ' ',
      'm',
      'a',
      'n',
      'a',
      'g',
      'e',
      'r',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0'
    }
  },
  {
    11,
    &mi_10,
    {
      'D',
      'e',
      'c',
      'i',
      '2',
      ' ',
      'P',
      'I',
      'F',
      ' ',
      'i',
      'n',
      't',
      'e',
      'r',
      'f',
      'a',
      'c',
      'e',
      ' ',
      'd',
      'r',
      'i',
      'v',
      'e',
      'r',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0'
    }
  },
  {
    12,
    &mi_11,
    {
      'D',
      'e',
      'c',
      'i',
      '2',
      ' ',
      'S',
      'I',
      'F',
      '2',
      ' ',
      'i',
      'n',
      't',
      'e',
      'r',
      'f',
      'a',
      'c',
      'e',
      ' ',
      'd',
      'r',
      'i',
      'v',
      'e',
      'r',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0'
    }
  },
  {
    13,
    &mi_12,
    {
      'D',
      'e',
      'c',
      'i',
      '2',
      ' ',
      'T',
      'T',
      'Y',
      '/',
      'F',
      'I',
      'L',
      'E',
      ' ',
      'd',
      'r',
      'i',
      'v',
      'e',
      'r',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0'
    }
  },
  {
    14,
    &mi_13,
    {
      'D',
      'e',
      'c',
      'i',
      '2',
      ' ',
      'l',
      'o',
      'a',
      'd',
      ' ',
      'm',
      'a',
      'n',
      'a',
      'g',
      'e',
      'r',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0'
    }
  },
  {
    15,
    &mi_14,
    {
      'M',
      'u',
      'l',
      't',
      'i',
      '_',
      'T',
      'h',
      'r',
      'e',
      'a',
      'd',
      '_',
      'M',
      'a',
      'n',
      'a',
      'g',
      'e',
      'r',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0',
      '\0'
    }
  },
  // TODO: count the NULLs
  {
    0,
    &mi_15,
    ""
  },
  {
    0,
    &mi_16,
    ""
  },
  {
    0,
    &mi_17,
    ""
  },
  {
    0,
    &mi_18,
    ""
  },
  {
    0,
    &mi_19,
    ""
  },
  {
    0,
    &mi_20,
    ""
  },
  {
    0,
    &mi_21,
    ""
  },
  {
    0,
    &mi_22,
    ""
  },
  {
    0,
    &mi_23,
    ""
  },
  {
    0,
    &mi_24,
    ""
  },
  {
    0,
    &mi_25,
    ""
  },
  {
    0,
    &mi_26,
    ""
  },
  {
    0,
    &mi_27,
    ""
  },
  {
    0,
    &mi_28,
    ""
  },
  {
    0,
    &mi_29,
    ""
  },
  {
    0,
    &mi_30,
    ""
  },
  {
    0,
    &mi_31,
    ""
  },
  {
    0,
    &mi_32,
    ""
  },
  {
    0,
    &mi_33,
    ""
  },
  {
    0,
    &mi_34,
    ""
  },
  {
    0,
    &mi_35,
    ""
  },
  {
    0,
    &mi_36,
    ""
  },
  {
    0,
    &mi_37,
    ""
  },
  {
    0,
    &mi_38,
    ""
  },
  {
    0,
    &mi_39,
    ""
  }
};
static int terror_code = -1;

static DS_DESC *target_desc;
static int f_target_mode;
static int f_netm_mode;
static int f_noconnectr;
static unsigned __int8 *target_mems;
static unsigned int iregs[10][32];
static quad eregs[11][32];
static DBGP_BRKPT_DATA bps[32];

static int send_cmd(int ac, char **av);
static int breakr_cmd(int ac, char **av);
static int connectr_cmd(int ac, char **av);
static int tty_cmd(int ac, char **av);
static int send_dsdb_getconf(DS_DESC *desc, int proto);
static int send_dsdb_readmem(DS_DESC *desc, int proto, int id, DBGP_MEM *mh);
static int send_dsdb_writemem(DS_DESC *desc, int proto, int id, DBGP_MEM *mh);
static int send_dsdb_getreg(DS_DESC *desc, DSP_BUF *sdb, int reg_size, int n);
static int send_dsdb_putreg(DS_DESC *desc, DSP_BUF *sdb, int reg_size, int n);
static int send_getbrkptr(DS_DESC *desc, int proto, unsigned int *rp);
static int send_putbrkptr(DS_DESC *desc, int proto, unsigned int *rp, int n);
static int send_xgktctlr(DS_DESC *desc, DBGP_XGKT_CTL *xc);
static int send_dbgctlr(DS_DESC *desc, int id, int flag);
static int send_setbpfuncr(DS_DESC *desc, int proto, int adr);
static int send_rdimgr(DS_DESC *desc);
static int send_iloadp_reply(DS_DESC *desc, ILOADP_HDR *dsip, void *ptr, int len);
static int ireport_cmd(int ac, char **av);
static int terror_cmd(int ac, char **av);
static DSP_BUF *recv_as_target(DS_DESC *desc, DSP_BUF *db);
static DSP_BUF *recv_func(DS_DESC *desc, DSP_BUF *db);
static int accept_func(DS_DESC *desc);
static int usage(int f_true);

static int send_cmd(int ac, char **av)
{
  unsigned int val; // [esp+0h] [ebp-18h] BYREF
  int n; // [esp+4h] [ebp-14h]
  int size; // [esp+8h] [ebp-10h]
  unsigned __int8 *bp; // [esp+Ch] [ebp-Ch]
  DECI2_HDR *ds; // [esp+10h] [ebp-8h]
  DSP_BUF *db; // [esp+14h] [ebp-4h]

  size = 1024 + sizeof(DSP_BUF);
  if ( !target_desc )
  {
    ds_printf("no target\n");
    return -1;
  }
  if ( ac > 0 )
  {
    if ( last_db_2 )
      last_db_2 = ds_free_buf(last_db_2);
  }
  else if ( last_db_2 )
  {
    db = last_db_2;
    goto LABEL_36;
  }
  db = (DSP_BUF *)ds_alloc(size);
  if ( !db )
    return -1;
  ds = (DECI2_HDR *)db->buf;
  bp = (unsigned __int8 *)db->buf;
  if ( ac > 0 )
  {
    --ac;
    ++av;
  }
  while ( ac > 0 && size > bp - (unsigned __int8 *)db )
  {
    n = strlen(*av);
    if ( ds_scan_hex_word(*av, &val) )
    {
      ds_free(db);
      return -1;
    }
    if ( n <= 0 || n > 2 )
    {
      if ( n <= 2 || n > 4 )
      {
        if ( n > 4 && n <= 8 && size > (unsigned int)(bp - (unsigned __int8 *)db + 4) )
        {
          *(_DWORD *)bp = val;
          bp += 4;
        }
      }
      else if ( size > (unsigned int)(bp - (unsigned __int8 *)db + 2) )
      {
        *(_WORD *)bp = val;
        bp += 2;
      }
    }
    else if ( size > (unsigned int)(bp - (unsigned __int8 *)db + 1) )
    {
      *bp++ = val;
    }
    --ac;
    ++av;
  }
  size = bp - (unsigned __int8 *)ds;
  if ( (unsigned int)(bp - (unsigned __int8 *)ds) <= 7 || size != ds->length )
  {
    ds_printf("send_cmd: invalid length (0x%x<-0x%x)\n", ds->length, size);
    ds_free(db);
    return -1;
  }
LABEL_36:
  last_db_2 = ds_dup_buf(db);
  ds_send_desc(target_desc, db);
  return 0;
}

static int breakr_cmd(int ac, char **av)
{
  int id; // [esp+0h] [ebp-18h] BYREF
  int v4; // [esp+4h] [ebp-14h]
  int v5; // [esp+8h] [ebp-10h] BYREF
  DBGP_HDR *ch; // [esp+Ch] [ebp-Ch]
  DECI2_HDR *dh; // [esp+10h] [ebp-8h]
  DSP_BUF *db; // [esp+14h] [ebp-4h]

  id = 0;
  if ( !f_target_mode )
    return ds_printf("target mode is disabled\n");
  if ( ac != 2 && ac != 3 )
    return ds_printf("no result code\n");
  if ( ds_scan_hex_word(av[1], (unsigned int *)&v5) )
    return -1;
  if ( ac == 3 && ds_scan_hex_word(av[2], (unsigned int *)&id) )
    return -1;
  if ( **av == 105 )
  {
    ds_sid = 73;
    v4 = 304;
  }
  else
  {
    ds_sid = 69;
    v4 = 560;
  }
  db = ds_alloc_buf((unsigned __int16)v4, 72, 0, 8);
  if ( !db )
    return -1;
  dh = (DECI2_HDR *)db->buf;
  ch = (DBGP_HDR *)&db->buf[8];
  db->buf[8] = id;
  ch->type = 21;
  ch->code = 0;
  ch->result = v5;
  ch->count = 0;
  ch->unused = 0;
  ds_send_desc(target_desc, db);
  return 0;
}

static int connectr_cmd(int ac, char **av)
{
  unsigned __int8 v3; // al

  if ( !f_target_mode )
    return ds_printf("target mode is disabled\n");
  if ( ac != 1 )
    return ds_printf("invalid argument\n");
  if ( **av == 105 )
    v3 = 73;
  else
    v3 = 69;
  ds_sid = v3;
  ds_send_dcmp_connect(target_desc, 72, 0, 0);
  return 0;
}

static int tty_cmd(int ac, char **av)
{
  unsigned __int8 v3; // al
  int v4; // [esp+4h] [ebp-414h]
  int n; // [esp+8h] [ebp-410h]
  int proto; // [esp+Ch] [ebp-40Ch]
  DSP_BUF *db; // [esp+14h] [ebp-404h]
  char buf[1024]; // [esp+18h] [ebp-400h] BYREF
  int aca; // [esp+420h] [ebp+8h]
  char **ava; // [esp+424h] [ebp+Ch]

  LOWORD(v4) = 0;
  if ( !f_target_mode )
    return ds_printf("target mode is disabled\n");
  if ( **av == 105 )
  {
    LOWORD(proto) = 272;
    v3 = 73;
  }
  else
  {
    LOWORD(proto) = 528;
    v3 = 69;
  }
  ds_sid = v3;
  aca = ac - 1;
  for ( ava = av + 1; aca > 0 && **ava == 45; ++ava )
  {
    if ( !strcmp("-0", *ava) )
    {
      LOWORD(v4) = 0;
    }
    else if ( !strcmp("-1", *ava) )
    {
      LOWORD(v4) = 1;
    }
    else if ( !strcmp("-2", *ava) )
    {
      LOWORD(v4) = 2;
    }
    else if ( !strcmp("-3", *ava) )
    {
      LOWORD(v4) = 3;
    }
    else if ( !strcmp("-4", *ava) )
    {
      LOWORD(v4) = 4;
    }
    else if ( !strcmp("-5", *ava) )
    {
      LOWORD(v4) = 5;
    }
    else if ( !strcmp("-6", *ava) )
    {
      LOWORD(v4) = 6;
    }
    else if ( !strcmp("-7", *ava) )
    {
      LOWORD(v4) = 7;
    }
    else if ( !strcmp("-8", *ava) )
    {
      LOWORD(v4) = 8;
    }
    else if ( !strcmp("-9", *ava) )
    {
      LOWORD(v4) = 9;
    }
    else
    {
      if ( strcmp("-k", *ava) )
        return ds_printf("invalid argument\n");
      LOWORD(v4) = 15;
    }
    --aca;
  }
  if ( aca != 1 )
    return ds_printf("invalid argument\n");
  n = ds_scan_string(buf, *ava);
  db = ds_alloc_buf((unsigned __int16)(v4 + proto), 72, 0, n + 4);
  if ( !db )
    return -1;
  *(_DWORD *)&db->buf[8] = 0;
  memcpy(&db->buf[12], buf, n);
  ds_send_desc(target_desc, db);
  return 0;
}

static int send_dsdb_getconf(DS_DESC *desc, int proto)
{
  int v3; // ecx
  DSP_BUF *db; // [esp+Ch] [ebp-4h]

  db = ds_alloc_buf((unsigned __int16)proto, 72, 0, 76);
  if ( !db )
    return -1;
  db->buf[8] = 0;
  db->buf[10] = 1;
  db->buf[11] = 0;
  db->buf[12] = 0;
  db->buf[13] = 0;
  *(_WORD *)&db->buf[14] = 0;
  ds_bzero(&db->buf[16], 68);
  *(_DWORD *)&db->buf[16] = 3;
  *(_DWORD *)&db->buf[20] = 0;
  *(_DWORD *)&db->buf[24] = proto;
  if ( proto == 320 )
    goto LABEL_10;
  if ( proto > 320 )
  {
    if ( proto == 560 || proto == 576 )
    {
      *(_DWORD *)&db->buf[32] = 1055;
      *(_DWORD *)&db->buf[40] = 7;
      v3 = *(_DWORD *)&db->buf[32];
      BYTE1(v3) &= ~4u;
      *(_DWORD *)&db->buf[64] = v3;
      *(_DWORD *)&db->buf[68] = 1024;
      *(_DWORD *)&db->buf[72] = 2;
    }
    goto LABEL_12;
  }
  if ( proto == 304 )
  {
LABEL_10:
    *(_DWORD *)&db->buf[32] = 15;
    *(_DWORD *)&db->buf[40] = 5;
    *(_DWORD *)&db->buf[64] = 0;
    *(_DWORD *)&db->buf[68] = 0;
    *(_DWORD *)&db->buf[72] = 1;
  }
LABEL_12:
  *(_DWORD *)&db->buf[44] = 32;
  *(_DWORD *)&db->buf[48] = 32;
  *(_DWORD *)&db->buf[52] = 1;
  *(_DWORD *)&db->buf[56] = 255;
  *(_DWORD *)&db->buf[60] = 255;
  ds_send_desc(desc, db);
  return 0;
}

static int send_dsdb_readmem(DS_DESC *desc, int proto, int id, DBGP_MEM *mh)
{
  unsigned __int8 v5; // [esp+7h] [ebp-21h]
  int npad; // [esp+Ch] [ebp-1Ch]
  int v7; // [esp+10h] [ebp-18h]
  unsigned int len; // [esp+14h] [ebp-14h]
  unsigned int adr; // [esp+18h] [ebp-10h]
  DSP_BUF *db; // [esp+20h] [ebp-8h]

  adr = mh->address;
  len = mh->length;
  v7 = 20;
  v5 = 0;
  npad = (~((1 << mh->align) - 1) & ((1 << mh->align) - 1 + 28)) - 28;
  if ( adr <= target_mem_size && target_mem_size > len + adr + 1 )
    v7 = len + 20;
  else
    v5 = 1;
  db = ds_alloc_buf((unsigned __int16)proto, 72, 0, v7 + npad);
  if ( !db )
    return -1;
  db->buf[8] = id;
  db->buf[10] = 9;
  db->buf[11] = 0;
  db->buf[12] = v5;
  db->buf[13] = 0;
  *(_WORD *)&db->buf[14] = 0;
  memcpy(&db->buf[16], mh, 0xCu);
  if ( !v5 )
  {
    if ( npad > 0 )
      ds_bzero(&db->buf[28], npad);
    memcpy(&db->buf[npad + 28], &target_mems[adr], len);
  }
  ds_send_desc(desc, db);
  return 0;
}

static int send_dsdb_writemem(DS_DESC *desc, int proto, int id, DBGP_MEM *mh)
{
  unsigned __int8 v5; // [esp+7h] [ebp-21h]
  unsigned int len; // [esp+14h] [ebp-14h]
  unsigned int adr; // [esp+18h] [ebp-10h]
  DSP_BUF *db; // [esp+20h] [ebp-8h]

  adr = mh->address;
  len = mh->length;
  v5 = 0;
  if ( adr > target_mem_size || target_mem_size <= len + adr + 1 )
    v5 = 1;
  if ( !v5 )
    memcpy(&target_mems[adr], (char *)&mh[-1] + (~((1 << mh->align) - 1) & ((1 << mh->align) - 1 + 28)) - 4, len);
  db = ds_alloc_buf((unsigned __int16)proto, 72, 0, 20);
  if ( !db )
    return -1;
  db->buf[8] = id;
  db->buf[10] = 11;
  db->buf[11] = 0;
  db->buf[12] = v5;
  db->buf[13] = 0;
  *(_WORD *)&db->buf[14] = 0;
  memcpy(&db->buf[16], mh, 0xCu);
  ds_send_desc(desc, db);
  return 0;
}

static int send_dsdb_getreg(DS_DESC *desc, DSP_BUF *sdb, int reg_size, int n)
{
  int i; // [esp+4h] [ebp-1Ch]
  int i_1; // [esp+4h] [ebp-1Ch]
  DSP_BUF *db; // [esp+14h] [ebp-Ch]
  DBGP_REG *rh; // [esp+18h] [ebp-8h]

  db = ds_dup_buf(sdb);
  if ( !db )
    return -1;
  db->buf[6] = db->buf[7];
  db->buf[7] = 72;
  db->buf[8] = 0;
  db->buf[10] = 5;
  db->buf[11] = 0;
  db->buf[12] = 0;
  db->buf[13] = n;
  *(_WORD *)&db->buf[14] = 0;
  rh = (DBGP_REG *)&db->buf[16];
  if ( reg_size == 4 )
  {
    for ( i = 0; n > i; ++i )
    {
      memcpy(&rh[1], &iregs[rh->kind][rh->number], sizeof(DBGP_REG));
      rh += 2;
    }
  }
  else if ( reg_size == 16 )
  {
    for ( i_1 = 0; n > i_1; ++i_1 )
    {
      memcpy(&rh[1], &eregs[rh->kind][rh->number], 0x10u);
      rh += 5;
    }
  }
  ds_send_desc(desc, db);
  return 0;
}

static int send_dsdb_putreg(DS_DESC *desc, DSP_BUF *sdb, int reg_size, int n)
{
  int i; // [esp+4h] [ebp-1Ch]
  int i_1; // [esp+4h] [ebp-1Ch]
  DSP_BUF *db; // [esp+14h] [ebp-Ch]
  DBGP_REG *rh; // [esp+18h] [ebp-8h]

  db = ds_dup_buf(sdb);
  if ( !db )
    return -1;
  db->buf[6] = db->buf[7];
  db->buf[7] = 72;
  db->buf[8] = 0;
  db->buf[10] = 7;
  db->buf[11] = 0;
  db->buf[12] = 0;
  db->buf[13] = n;
  *(_WORD *)&db->buf[14] = 0;
  rh = (DBGP_REG *)&db->buf[16];
  if ( reg_size == 4 )
  {
    for ( i = 0; n > i; ++i )
    {
      memcpy(&iregs[rh->kind][rh->number], &rh[1], sizeof(iregs[rh->kind][rh->number]));
      rh += 2;
    }
  }
  else if ( reg_size == 16 )
  {
    for ( i_1 = 0; n > i_1; ++i_1 )
    {
      memcpy(&eregs[rh->kind][rh->number], &rh[1], sizeof(eregs[rh->kind][rh->number]));
      rh += 5;
    }
  }
  ds_send_desc(desc, db);
  return 0;
}

static int send_getbrkptr(DS_DESC *desc, int proto, unsigned int *rp)
{
  int n; // [esp+0h] [ebp-10h]
  DSP_BUF *db; // [esp+Ch] [ebp-4h]

  n = cbrk;
  db = ds_alloc_buf((unsigned __int16)proto, 72, 0, 8 * cbrk + 8);
  if ( !db )
    return -1;
  db->buf[8] = 0;
  db->buf[10] = 17;
  db->buf[11] = 0;
  db->buf[12] = 0;
  db->buf[13] = n;
  *(_WORD *)&db->buf[14] = 0;
  memcpy(&db->buf[16], bps, 8 * n);
  ds_send_desc(desc, db);
  return 0;
}

static int send_putbrkptr(DS_DESC *desc, int proto, unsigned int *rp, int n)
{
  DSP_BUF *db; // [esp+Ch] [ebp-4h]

  if ( n > 32 )
    n = 0;
  if ( n > 0 )
    memcpy(bps, rp, 8 * n);
  cbrk = n;
  db = ds_alloc_buf((unsigned __int16)proto, 72, 0, 8);
  if ( !db )
    return -1;
  db->buf[8] = 0;
  db->buf[10] = 19;
  db->buf[11] = 0;
  db->buf[12] = 0;
  db->buf[13] = 0;
  *(_WORD *)&db->buf[14] = 0;
  ds_send_desc(desc, db);
  return 0;
}

static int send_xgktctlr(DS_DESC *desc, DBGP_XGKT_CTL *xc)
{
  DSP_BUF *db; // [esp+8h] [ebp-4h]

  db = ds_alloc_buf(560, 72, 0, 20);
  if ( !db )
    return -1;
  db->buf[8] = 2;
  db->buf[10] = 33;
  db->buf[11] = 0;
  db->buf[12] = 0;
  db->buf[13] = 0;
  *(_WORD *)&db->buf[14] = 0;
  memcpy(&db->buf[16], xc, 0xCu);
  ds_send_desc(desc, db);
  return 0;
}

static int send_dbgctlr(DS_DESC *desc, int id, int flag)
{
  DSP_BUF *db; // [esp+Ch] [ebp-4h]

  db = ds_alloc_buf(560, 72, 0, 12);
  if ( !db )
    return -1;
  db->buf[8] = id;
  db->buf[10] = 37;
  db->buf[11] = 0;
  db->buf[12] = 0;
  db->buf[13] = 0;
  *(_WORD *)&db->buf[14] = 0;
  *(_DWORD *)&db->buf[16] = flag;
  ds_send_desc(desc, db);
  return 0;
}

static int send_setbpfuncr(DS_DESC *desc, int proto, int adr)
{
  DSP_BUF *db; // [esp+Ch] [ebp-4h]

  db = ds_alloc_buf((unsigned __int16)proto, 72, 0, 12);
  if ( !db )
    return -1;
  db->buf[8] = 0;
  db->buf[10] = 47;
  db->buf[11] = 0;
  db->buf[12] = 0;
  db->buf[13] = 0;
  *(_WORD *)&db->buf[14] = 0;
  *(_DWORD *)&db->buf[16] = adr;
  ds_send_desc(desc, db);
  return 0;
}

static int send_rdimgr(DS_DESC *desc)
{
  int i; // [esp+0h] [ebp-14h]
  int i_1; // [esp+0h] [ebp-14h]
  DSP_BUF *db; // [esp+10h] [ebp-4h]
  DSP_BUF *db_1; // [esp+10h] [ebp-4h]

  db = ds_alloc_buf(560, 72, 0, 40);
  if ( !db )
    return -1;
  db->buf[8] = 0;
  db->buf[10] = 41;
  db->buf[11] = 0;
  db->buf[12] = 0;
  db->buf[13] = 0;
  *(_WORD *)&db->buf[14] = 0;
  *(_DWORD *)&db->buf[16] = 32;
  *(_DWORD *)&db->buf[20] = 0;
  *(_DWORD *)&db->buf[24] = 0;
  *(_DWORD *)&db->buf[28] = 0;
  for ( i = 0; i <= 15; ++i )
    db->buf[i + 32] = i;
  ds_send_desc(desc, db);
  db_1 = ds_alloc_buf(560, 72, 0, 40);
  if ( !db_1 )
    return -1;
  db_1->buf[8] = 0;
  db_1->buf[10] = 41;
  db_1->buf[11] = 0;
  db_1->buf[12] = 0;
  db_1->buf[13] = 0;
  *(_DWORD *)&db_1->buf[16] = 32;
  *(_DWORD *)&db_1->buf[20] = 1;
  *(_DWORD *)&db_1->buf[24] = 0;
  *(_DWORD *)&db_1->buf[28] = 0;
  for ( i_1 = 0; i_1 <= 15; ++i_1 )
    db_1->buf[i_1 + 32] = i_1 + 16;
  ds_send_desc(desc, db_1);
  return 0;
}

static int send_iloadp_reply(DS_DESC *desc, ILOADP_HDR *dsip, void *ptr, int len)
{
  int v4; // edx
  unsigned int list[40]; // [esp+4h] [ebp-C8h] BYREF
  void *rptr; // [esp+A4h] [ebp-28h]
  void *rptr1;
  int v8; // [esp+A8h] [ebp-24h]
  int rlen; // [esp+ACh] [ebp-20h]
  int rlen1;
  int id; // [esp+B0h] [ebp-1Ch]
  int j; // [esp+B4h] [ebp-18h]
  int v12; // [esp+B8h] [ebp-14h]
  struct id_mi_name *p; // [esp+BCh] [ebp-10h]
  ILOADP_HDR *rh; // [esp+C0h] [ebp-Ch]
  DECI2_HDR *dh; // [esp+C4h] [ebp-8h]
  DSP_BUF *db; // [esp+C8h] [ebp-4h]

  rlen = 0;
  rlen1 = 0;
  v8 = 0;
  rptr = 0;
  rptr1 = 0;
  id = dsip->module_id;
  switch ( dsip->cmd )
  {
    case 0u:
      p = id_mi_names;
      v12 = 0;
      while ( (unsigned int)v12 <= 0x27 && p->id )
      {
        ++v12;
        ++p;
      }
      if ( (unsigned int)v12 <= 0x27 )
      {
        v4 = dsip_id;
        p->id = dsip_id;
        id = v4;
        ++dsip_id;
        p->mi->version = 258;
        p->mi->mod_addr = p->id << 16;
        p->mi->text_size = p->id << 10;
        p->mi->data_size = (p->id + 1) << 10;
        p->mi->bss_size = (p->id + 2) << 10;
        ds_sprintf(p->name, "start-%d", p->id);
        if ( (dsip->action & 4) != 0 )
        {
          rptr = &p->mi;
          rptr1 = p->name;
          rlen = 32;
          rlen1 = strlen(p->name) + 1;
        }
      }
      else
      {
        v8 = 2;
      }
      break;
    case 2u:
      p = id_mi_names;
      v12 = 0;
      while ( (unsigned int)v12 <= 0x27 )
      {
        if ( id == p->id )
        {
          p->id = 0;
          goto LABEL_28;
        }
        ++v12;
        ++p;
      }
      break;
    case 4u:
      p = id_mi_names;
      j = 0;
      v12 = 0;
      while ( (unsigned int)v12 <= 0x27 )
      {
        if ( p->id )
        {
          list[j++] = p->id;
          rlen += 4;
        }
        ++v12;
        ++p;
      }
      rptr = list;
      break;
    case 6u:
      p = id_mi_names;
      v12 = 0;
      while ( (unsigned int)v12 <= 0x27 )
      {
        if ( id == p->id )
        {
          rptr = &p->mi;
          rlen = strlen(p->name) + 33;
          goto LABEL_28;
        }
        ++v12;
        ++p;
      }
      break;
    default:
      break;
  }
LABEL_28:
  db = ds_alloc_buf(336, 72, 0, (rlen + rlen1) + 8);
  if ( !db )
    return -1;
  dh = (DECI2_HDR *)db->buf;
  rh = (ILOADP_HDR *)&db->buf[8];
  db->buf[8] = dsip->cmd + 1;
  rh->action = dsip->action;
  rh->result = v8;
  rh->stamp = dsip->stamp;
  rh->module_id = id;
  if ( rptr )
  {
    if ( rlen > 0 )
      memcpy(&rh[1], rptr, rlen);
  }
  if ( rptr1 )
  {
    if ( rlen1 > 0 )
      memcpy(((char *)&rh[1]) + rlen, rptr1, rlen1);
  }
  ds_send_desc(desc, db);
  return 0;
}

static int ireport_cmd(int ac, char **av)
{
  unsigned int ret; // [esp+0h] [ebp-18h] BYREF
  unsigned int v4; // [esp+4h] [ebp-14h] BYREF
  unsigned int id; // [esp+8h] [ebp-10h] BYREF
  ILOADP_HDR *rh; // [esp+Ch] [ebp-Ch]
  DECI2_HDR *dh; // [esp+10h] [ebp-8h]
  DSP_BUF *db; // [esp+14h] [ebp-4h]

  if ( ac != 4 )
    return ds_error("invalid argument");
  if ( ds_scan_hex_word(av[1], &id) )
    return -1;
  if ( ds_scan_hex_word(av[2], &v4) )
    return -1;
  if ( ds_scan_hex_word(av[3], &ret) )
    return -1;
  ds_sid = 73;
  db = ds_alloc_buf(336, 72, 0, 12);
  if ( !db )
    return -1;
  dh = (DECI2_HDR *)db->buf;
  rh = (ILOADP_HDR *)&db->buf[8];
  db->buf[8] = 16;
  rh->action = 0;
  rh->result = v4;
  rh->stamp = 0;
  rh->module_id = id;
  *(_DWORD *)&rh[1].cmd = ret;
  ds_send_desc(target_desc, db);
  return 0;
}

static int terror_cmd(int ac, char **av)
{
  if ( ac != 2 )
    return ds_error("invalid argument");
  if ( !strcmp("none", av[1]) )
  {
    terror_code = -1;
  }
  else if ( !strcmp("noroute", av[1]) )
  {
    terror_code = 0;
  }
  else if ( !strcmp("noproto", av[1]) )
  {
    terror_code = 1;
  }
  else if ( !strcmp("locked", av[1]) )
  {
    terror_code = 2;
  }
  else if ( !strcmp("nospace", av[1]) )
  {
    terror_code = 3;
  }
  else if ( !strcmp("invalhead", av[1]) )
  {
    terror_code = 4;
  }
  else if ( !strcmp("noconnect", av[1]) )
  {
    terror_code = 5;
  }
  else
  {
    if ( strcmp("noreply", av[1]) )
      return ds_error("invalid argument");
    terror_code = -2;
  }
  return 0;
}

static DSP_BUF *recv_as_target(DS_DESC *desc, DSP_BUF *db)
{
  int v3; // eax
  DSP_BUF *sdb; // [esp+8h] [ebp-30h]
  DSP_BUF *sdb_1; // [esp+8h] [ebp-30h]
  DSP_BUF *sdb_2; // [esp+8h] [ebp-30h]
  int reg_size; // [esp+Ch] [ebp-2Ch]
  int len; // [esp+14h] [ebp-24h]
  int len_1; // [esp+14h] [ebp-24h]
  int len_2; // [esp+14h] [ebp-24h]
  int len_3; // [esp+14h] [ebp-24h]
  DBGP_MEM *mh; // [esp+1Ch] [ebp-1Ch]
  DBGP_HDR *ch; // [esp+20h] [ebp-18h]
  NETMP_HDR *dsm; // [esp+28h] [ebp-10h]
  DCMP_HDR *dsc; // [esp+30h] [ebp-8h]

  len = *(unsigned __int16 *)db->buf - 8;
  if ( len < 0 )
    return db;
  if ( terror_code >= 0 )
    return ds_send_dcmp_error(desc, db, (unsigned __int8)terror_code);
  v3 = *(unsigned __int16 *)&db->buf[4];
  if ( v3 == 336 )
  {
    if ( db->buf[7] == 73 )
    {
      ds_sid = 73;
      len_3 = len - 8;
      if ( len_3 >= 0 )
        send_iloadp_reply(desc, (ILOADP_HDR *)&db->buf[8], &db->buf[16], len_3);
    }
    return db;
  }
  if ( *(unsigned __int16 *)&db->buf[4] > 0x150u )
  {
    if ( v3 != 576 )
    {
      if ( *(unsigned __int16 *)&db->buf[4] > 0x240u )
      {
        if ( v3 == 1024 )
        {
          dsm = (NETMP_HDR *)&db->buf[8];
          len_1 = *(unsigned __int16 *)db->buf - 10;
          if ( len_1 >= 0 )
          {
            if ( dsm->code )
            {
              if ( dsm->code == 2 )
              {
                ds_sid = 72;
                sdb_1 = ds_alloc_buf(1024, 72, 0, 2);
                if ( sdb_1 )
                {
                  sdb_1->buf[8] = 3;
                  sdb_1->buf[9] = 0;
                  ds_send_desc(desc, sdb_1);
                  sdb_2 = ds_alloc_buf(1, 72, 0, 6);
                  if ( sdb_2 )
                  {
                    if ( f_netm_mode )
                    {
                      sdb_2->buf[8] = 2;
                      sdb_2->buf[9] = 0;
                      *(_WORD *)&sdb_2->buf[10] = 0;
                      *(_WORD *)&sdb_2->buf[12] = 0;
                      ds_send_desc(desc, sdb_2);
                      ds_sid = 73;
                      ds_send_dcmp_connect(target_desc, 72, 0, 0);
                      ds_sid = 69;
                      ds_send_dcmp_connect(target_desc, 72, 0, 0);
                    }
                  }
                }
              }
            }
            else
            {
              ds_sid = 72;
              sdb = ds_alloc_buf(1024, 72, dsm, len_1 + 2);
              if ( sdb )
              {
                sdb->buf[8] = 1;
                sdb->buf[9] = 0;
                ds_send_desc(desc, sdb);
              }
            }
          }
        }
        return db;
      }
      if ( v3 != 560 )
        return db;
    }
LABEL_38:
    if ( v3 == 304 || v3 == 320 )
    {
      if ( db->buf[7] != 73 )
        return db;
      ds_sid = 73;
    }
    else
    {
      if ( db->buf[7] != 69 )
        return db;
      ds_sid = 69;
    }
    ch = (DBGP_HDR *)&db->buf[8];
    len_2 = len - 8;
    if ( len_2 >= 0 )
    {
      switch ( db->buf[10] )
      {
        case 0:
          send_dsdb_getconf(desc, v3);
          return db;
        case 4:
        case 6:
          if ( v3 == 320 || v3 <= 320 )
            reg_size = 4;
          else
            reg_size = 16;
          if ( len_2 != (unsigned __int8)db->buf[13] * (reg_size + 4) )
            return db;
          if ( db->buf[10] == 4 )
            send_dsdb_getreg(desc, db, reg_size, (unsigned __int8)db->buf[13]);
          else
            send_dsdb_putreg(desc, db, reg_size, (unsigned __int8)db->buf[13]);
          return db;
        case 8:
        case 0xA:
          mh = (DBGP_MEM *)&db->buf[16];
          if ( len_2 - 12 < 0 )
            return db;
          if ( db->buf[10] == 8 )
            send_dsdb_readmem(desc, v3, ch->id, mh);
          else
            send_dsdb_writemem(desc, v3, ch->id, mh);
          break;
        case 0x10:
          send_getbrkptr(desc, v3, (unsigned int *)&db->buf[16]);
          return db;
        case 0x12:
          send_putbrkptr(desc, v3, (unsigned int *)&db->buf[16], (unsigned __int8)db->buf[13]);
          return db;
        case 0x20:
          send_xgktctlr(desc, (DBGP_XGKT_CTL *)&db->buf[16]);
          return db;
        case 0x24:
          send_dbgctlr(desc, (unsigned __int8)db->buf[8], *(_DWORD *)&db->buf[16]);
          return db;
        case 0x28:
          send_rdimgr(desc);
          return db;
        case 0x2E:
          send_setbpfuncr(desc, v3, *(_DWORD *)&db->buf[16]);
          return db;
        default:
          return db;
      }
    }
    return db;
  }
  if ( v3 == 304 )
    goto LABEL_38;
  if ( *(unsigned __int16 *)&db->buf[4] <= 0x130u )
  {
    if ( v3 == 1 )
    {
      dsc = (DCMP_HDR *)&db->buf[8];
      if ( *(unsigned __int16 *)db->buf - 12 >= 0 )
      {
        if ( dsc->type )
        {
          if ( dsc->type == 1 )
          {
            ds_sid = db->buf[7];
            db->buf[7] = db->buf[6];
            db->buf[6] = ds_sid;
            db->buf[9] = 1;
            return ds_send_desc(desc, db);
          }
        }
        else if ( !f_noconnectr )
        {
          ds_sid = db->buf[7];
          ds_send_dcmp_connect(desc, (unsigned __int8)db->buf[6], 0, 0);
        }
      }
    }
    return db;
  }
  if ( v3 == 320 )
    goto LABEL_38;
  return db;
}

static DSP_BUF *recv_func(DS_DESC *desc, DSP_BUF *db)
{
  if ( db )
  {
    if ( desc->type == 8 && !target_desc )
      target_desc = desc;
    if ( f_target_mode && terror_code != -2 )
      db = recv_as_target(desc, db);
    return ds_free_buf(db);
  }
  else
  {
    if ( desc->type == 16 )
    {
      ds_exit(130);
    }
    else if ( desc->type == 8 && desc == target_desc )
    {
      target_desc = 0;
    }
    return 0;
  }
}

static int accept_func(DS_DESC *desc)
{
  if ( ds_add_recv_func(desc, -1, -1, -1, recv_func) )
    return 0;
  else
    return -1;
}

static int usage(int f_true)
{
  if ( !f_true )
    return 0;
  ds_printf("Usage: %s [<option>]\n", ds_program_name);
  ds_printf("  <option>:\n");
  ds_printf("    -p <port>             listen <port>\n");
  ds_printf("    -d <device>           open <device>\n");
  ds_printf("    -d [<host>[:<port>]]  connect <host>:<port>\n");
  ds_printf("    -t                    target simulation mode\n");
  ds_printf("    -tm <size>            target memory size [KB]\n");
  ds_printf("    -netm                 netm mode\n");
  ds_printf("    -noconnectr           no connectr mode\n");
  return ds_exit(129);
}

int main(int ac, char **av)
{
  int v2; // eax
  int v3; // eax
  int v4; // eax
  int v5; // eax
  const char *v6; // eax
  const char *v7; // eax
  quad v9; // [esp+8h] [ebp-84h] BYREF
  quad v10; // [esp+18h] [ebp-74h] BYREF
  quad v11; // [esp+28h] [ebp-64h] BYREF
  quad v12; // [esp+38h] [ebp-54h] BYREF
  quad v13; // [esp+48h] [ebp-44h] BYREF
  quad v14; // [esp+58h] [ebp-34h] BYREF
  quad v15; // [esp+68h] [ebp-24h] BYREF
  int v; // [esp+78h] [ebp-14h]
  int i; // [esp+7Ch] [ebp-10h]
  char *target_name; // [esp+84h] [ebp-8h]
  char *port_name; // [esp+88h] [ebp-4h]
  int argc; // [esp+94h] [ebp+8h]
  char **argv; // [esp+98h] [ebp+Ch]

  port_name = 0;
  target_name = 0;
  ds_program_name = ds_basename(*av);
  ds_printf("%s (%s)\n", ds_program_name, ds_stamp_str[0]);
  ds_opt_standard_init();
  ds_read_option_file();
  argc = ac - 1;
  for ( argv = av + 1; argc > 0 && **argv == 45; ++argv )
  {
    if ( !strcmp("-p", *argv) )
    {
      usage(--argc <= 0);
      port_name = *++argv;
    }
    else if ( !strcmp("-d", *argv) )
    {
      usage(--argc <= 0);
      target_name = *++argv;
    }
    else if ( !strcmp("-t", *argv) )
    {
      ++f_target_mode;
    }
    else if ( !strcmp("-netm", *argv) )
    {
      ++f_netm_mode;
    }
    else if ( !strcmp("-noconnectr", *argv) )
    {
      ++f_noconnectr;
    }
    else if ( !strcmp("-tm", *argv) )
    {
      ++f_target_mode;
      usage(--argc <= 0);
      v2 = ds_scan_digit_word(*++argv, (unsigned int *)&target_mem_size);
      usage(v2);
    }
    else
    {
      usage(1);
    }
    --argc;
  }
  v3 = 0;
  if ( argc > 0 || port_name && target_name )
    v3 = 1;
  usage(v3);
  if ( f_target_mode )
  {
    ds_printf("target mode (mem=%dKB)\n", target_mem_size);
    target_mem_size <<= 10;
    target_mems = (unsigned __int8 *)ds_alloc(target_mem_size);
    if ( !target_mems )
      ds_exit(135);
    for ( i = 0; i <= 9; ++i )
    {
      for ( v = 0; v <= 31; ++v )
        iregs[i][v] = (v | (i << 8)) << 16;
    }
    *(_QWORD *)&iregs[3][12] = 0LL;
    iregs[3][14] = 0x10000;
    for ( i = 0; i <= 10; ++i )
    {
      for ( v = 0; v <= 31; ++v )
      {
        ds_cwq(&v15, (v | (i << 8)) << 16);
        eregs[i][v] = v15;
      }
    }
    ds_cwq(&v14, 0);
    eregs[2][13] = v14;
    ds_cwq(&v13, 0);
    eregs[2][12] = v13;
    ds_cwq(&v12, 0x10000u);
    eregs[2][14] = v12;
    ds_cwq(&v11, 0x20000u);
    eregs[2][30] = v11;
    for ( v = 0; v <= 31; ++v )
    {
      ds_cwq(&v10, v);
      v4 = v;
      eregs[8][v4].xa[0] = v10.xa[0];
      eregs[8][v4].xa[1] = v10.xa[1];
      eregs[8][v4].xa[2] = v10.xa[2];
      eregs[8][v4].xa[3] = v10.xa[3];
      ds_cwq(&v9, v);
      v5 = v;
      eregs[10][v5].xa[0] = v9.xa[0];
      eregs[10][v5].xa[1] = v9.xa[1];
      eregs[10][v5].xa[2] = v9.xa[2];
      eregs[10][v5].xa[3] = v9.xa[3];
    }
    ds_cwq(&v9, 0x20u);
    eregs[8][26] = v9;
    ds_cwq(&v10, 0x20u);
    eregs[10][26] = v10;
    if ( f_netm_mode )
      v6 = "enable";
    else
      v6 = "disable";
    ds_printf("netm_mode=%s\n", v6);
    if ( f_noconnectr )
      v7 = "enable";
    else
      v7 = "disable";
    ds_printf("noconnectr=%s\n", v7);
  }
  else
  {
    ds_printf("non target mode\n");
  }
  if ( target_name )
  {
    target_desc = ds_connect_net(target_name, recv_func);
    if ( !target_desc )
      ds_exit(132);
  }
  else if ( !ds_listen_net(port_name, accept_func) )
  {
    ds_exit(133);
  }
  ds_cmd_install("send", "", "send", send_cmd);
  ds_cmd_install("ibreakr", "<result>", "send IOP BREAKR", breakr_cmd);
  ds_cmd_install("ebreakr", "<result>", "send EE BREAKR", breakr_cmd);
  ds_cmd_install("ireport", "<id> <result> <ret>", "send IOP REPORT", ireport_cmd);
  ds_cmd_install("terror", "<code>", "set target error", terror_cmd);
  ds_cmd_install("iconnectr", "", "send IOP connectr", connectr_cmd);
  ds_cmd_install("econnectr", "", "send EE connectr", connectr_cmd);
  ds_cmd_install("itty", "[-<no>] \"<msg>\"", "send ITTYP", tty_cmd);
  ds_cmd_install("etty", "[-<no>] \"<msg>\"", "send ETTYP", tty_cmd);
  ds_cmd_standard_install(1);
  if ( !ds_open_kbd(ds_cmd_input, 1) )
    ds_exit(131);
  ds_read_startup_file();
  while ( ds_select_desc(-1, -1) > 0 )
    ;
  return ds_exit(0);
}

