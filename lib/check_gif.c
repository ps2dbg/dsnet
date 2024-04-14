#include "dsnet_prototypes.h"

typedef struct {unsigned int sdram_data1; unsigned int sdram_data2; unsigned int sdram_data3; unsigned int sdram_data4;} MM;
typedef struct {unsigned char gif_data[32];} NN;

extern int DataConv(NN *nn, unsigned __int8 *dat);
extern int GIF_SDRAMCheck(MM *mm, MM *end, int opt);
extern int GIF_EXAMPLECheck(NN *nn, NN *end);
extern int GIF_GINTCheck(NN *nn, NN *end, int value);
extern int GIF_TEXSWAPCheck(NN *nn, NN *end);

int DataConv(NN *nn, unsigned __int8 *dat)
{
  memcpy(dat, &nn->gif_data[5], 0xAu);
  memcpy(dat + 10, &nn->gif_data[26], 5u);
  memcpy(dat + 15, nn, 5u);
  memcpy(dat + 20, &nn->gif_data[16], 0xAu);
  return 1;
}

int DataFileRead1()
{
  FILE *fp; // [esp+0h] [ebp-4h]

  fp = fopen("./GIF_REF_V0.pdat", "rb");
  if ( fp )
  {
    fseek(fp, 0, 2);
    gif.m_size1 = ftell(fp);
    fseek(fp, 0, 0);
    gif.m_buff1 = (unsigned __int8 *)calloc(gif.m_size1, 1u);
    if ( gif.m_buff1 )
    {
      fread(gif.m_buff1, gif.m_size1, 1u, fp);
      fclose(fp);
      return 1;
    }
    else
    {
      ds_printf("NO MEMORY m_buff1!\n");
      fclose(fp);
      free(gif.m_buff1);
      return 0;
    }
  }
  else
  {
    ds_printf("DONT OPEN FILE ./GIF_REF_V0.pdat \n");
    fclose(0);
    return 0;
  }
}

int DataFileRead2()
{
  FILE *fp4; // [esp+0h] [ebp-10h]
  FILE *fp3; // [esp+4h] [ebp-Ch]
  FILE *fp2; // [esp+8h] [ebp-8h]
  FILE *fp1; // [esp+Ch] [ebp-4h]

  fp1 = fopen("./GIF_REF_tex_swap1_write0.pdat", "rb");
  if ( fp1 )
  {
    fp2 = fopen("./GIF_REF_tex_swap1_read0.pdat", "rb");
    if ( fp2 )
    {
      fp3 = fopen("./GIF_REF_tex_swap1_write1.pdat", "rb");
      if ( fp3 )
      {
        fp4 = fopen("./GIF_REF_tex_swap1_read1.pdat", "rb");
        if ( fp4 )
        {
          fseek(fp1, 0, 2);
          gif.m_size1 = ftell(fp1);
          fseek(fp1, 0, 0);
          gif.m_buff1 = (unsigned __int8 *)calloc(gif.m_size1, 1u);
          fseek(fp2, 0, 2);
          gif.m_size2 = ftell(fp2);
          fseek(fp2, 0, 0);
          gif.m_buff2 = (unsigned __int8 *)calloc(gif.m_size2, 1u);
          fseek(fp3, 0, 2);
          gif.m_size3 = ftell(fp3);
          fseek(fp3, 0, 0);
          gif.m_buff3 = (unsigned __int8 *)calloc(gif.m_size3, 1u);
          fseek(fp4, 0, 2);
          gif.m_size4 = ftell(fp4);
          fseek(fp4, 0, 0);
          gif.m_buff4 = (unsigned __int8 *)calloc(gif.m_size4, 1u);
          if ( gif.m_buff1 )
          {
            if ( gif.m_buff2 )
            {
              if ( gif.m_buff3 )
              {
                if ( gif.m_buff4 )
                {
                  fread(gif.m_buff1, gif.m_size1, 1u, fp1);
                  fread(gif.m_buff2, gif.m_size2, 1u, fp2);
                  fread(gif.m_buff3, gif.m_size3, 1u, fp3);
                  fread(gif.m_buff4, gif.m_size4, 1u, fp4);
                  fclose(fp1);
                  fclose(fp2);
                  fclose(fp3);
                  fclose(fp4);
                  return 1;
                }
                else
                {
                  ds_printf("NO MEMORY m_buff4!\n");
                  fclose(fp4);
                  free(gif.m_buff4);
                  return 0;
                }
              }
              else
              {
                ds_printf("NO MEMORY m_buff3!\n");
                fclose(fp3);
                free(gif.m_buff3);
                return 0;
              }
            }
            else
            {
              ds_printf("NO MEMORY m_buff2!\n");
              fclose(fp2);
              free(gif.m_buff2);
              return 0;
            }
          }
          else
          {
            ds_printf("NO MEMORY m_buff1!\n");
            fclose(fp1);
            free(gif.m_buff1);
            return 0;
          }
        }
        else
        {
          ds_printf("DONT OPEN FILE ./GIF_REF_tex_swap1_read1.pdat \n");
          fclose(0);
          return 0;
        }
      }
      else
      {
        ds_printf("DONT OPEN FILE ./GIF_REF_tex_swap1_write1.pdat \n");
        fclose(0);
        return 0;
      }
    }
    else
    {
      ds_printf("DONT OPEN FILE ./GIF_REF_tex_swap1_read0.pdat \n");
      fclose(0);
      return 0;
    }
  }
  else
  {
    ds_printf("DONT OPEN FILE ./GIF_REF_tex_swap1_write0.pdat \n");
    fclose(0);
    return 0;
  }
}

int MemoryFree1()
{
  free(gif.m_buff1);
  return 1;
}

int MemoryFree2()
{
  free(gif.m_buff1);
  free(gif.m_buff2);
  free(gif.m_buff3);
  free(gif.m_buff4);
  return 1;
}

int GIF_SDRAMCheck(MM *mm, MM *end, int opt)
{
  unsigned int v3; // edx
  unsigned int v4; // edx
  unsigned int v5; // edx
  unsigned int v6; // edx
  int error; // [esp+0h] [ebp-Ch]
  int test; // [esp+4h] [ebp-8h]
  int status; // [esp+8h] [ebp-4h]

  status = 0;
  test = 1;
  error = 0;
  while ( end > mm )
  {
    if ( opt == 2 )
    {
      if ( (status & 1) != 0 )
      {
        if ( mm->sdram_data1 != -16711936 )
        {
          if ( error <= 9 )
            printf("sdram_data1 = %x %d\n", mm->sdram_data1, status);
          ++error;
        }
        if ( mm->sdram_data2 != -16711936 )
        {
          if ( error <= 9 )
            printf("sdram_data2 = %x %d\n", mm->sdram_data2, status);
          ++error;
        }
        if ( mm->sdram_data3 != -16711936 )
        {
          if ( error <= 9 )
            printf("sdram_data3 = %x %d\n", mm->sdram_data3, status);
          ++error;
        }
        if ( mm->sdram_data4 != -16711936 )
        {
          if ( error <= 9 )
            printf("sdram_data4 = %x %d\n", mm->sdram_data4, status);
          ++error;
        }
      }
      else
      {
        if ( mm->sdram_data1 != 16711935 )
        {
          if ( error <= 9 )
            printf("sdram_data1 = %x %d\n", mm->sdram_data1, status);
          ++error;
        }
        if ( mm->sdram_data2 != 16711935 )
        {
          if ( error <= 9 )
            printf("sdram_data2 = %x %d\n", mm->sdram_data2, status);
          ++error;
        }
        if ( mm->sdram_data3 != 16711935 )
        {
          if ( error <= 9 )
            printf("sdram_data3 = %x %d\n", mm->sdram_data3, status);
          ++error;
        }
        if ( mm->sdram_data4 != 16711935 )
        {
          if ( error <= 9 )
            printf("sdram_data4 = %x %d\n", mm->sdram_data4, status);
          ++error;
        }
      }
      ++status;
    }
    else if ( opt > 2 )
    {
      if ( opt == 3 )
      {
        v3 = m_sdram++;
        if ( mm->sdram_data1 != v3 )
        {
          if ( error <= 9 )
            printf("sdram_data1 = %x %x\n", mm->sdram_data1, m_sdram);
          ++error;
        }
        if ( m_sdram == 0x4000000 )
          m_sdram = 0;
        v4 = m_sdram++;
        if ( mm->sdram_data2 != v4 )
        {
          if ( error <= 9 )
            printf("sdram_data2 = %x %x\n", mm->sdram_data2, m_sdram);
          ++error;
        }
        if ( m_sdram == 0x4000000 )
          m_sdram = 0;
        v5 = m_sdram++;
        if ( mm->sdram_data3 != v5 )
        {
          if ( error <= 9 )
            printf("sdram_data3 = %x %x\n", mm->sdram_data3, m_sdram);
          ++error;
        }
        if ( m_sdram == 0x4000000 )
          m_sdram = 0;
        v6 = m_sdram++;
        if ( mm->sdram_data4 != v6 )
        {
          if ( error <= 9 )
            printf("sdram_data4 = %x %x\n", mm->sdram_data4, m_sdram);
          ++error;
        }
        if ( m_sdram == 0x4000000 )
          m_sdram = 0;
      }
    }
    else if ( opt == 1 )
    {
      if ( (status & 1) != 0 )
      {
        if ( mm->sdram_data1 != -1437226411 )
        {
          if ( error <= 9 )
            printf("sdram_data1 = %x %d\n", mm->sdram_data1, status);
          ++error;
        }
        if ( mm->sdram_data2 != -1437226411 )
        {
          if ( error <= 9 )
            printf("sdram_data2 = %x %d\n", mm->sdram_data2, status);
          ++error;
        }
        if ( mm->sdram_data3 != -1437226411 )
        {
          if ( error <= 9 )
            printf("sdram_data3 = %x %d\n", mm->sdram_data3, status);
          ++error;
        }
        if ( mm->sdram_data4 != -1437226411 )
        {
          if ( error <= 9 )
            printf("sdram_data4 = %x %d\n", mm->sdram_data4, status);
          ++error;
        }
      }
      else
      {
        if ( mm->sdram_data1 != 1437226410 )
        {
          if ( error <= 9 )
            printf("sdram_data1 = %x %d\n", mm->sdram_data1, status);
          ++error;
        }
        if ( mm->sdram_data2 != 1437226410 )
        {
          if ( error <= 9 )
            printf("sdram_data2 = %x %d\n", mm->sdram_data2, status);
          ++error;
        }
        if ( mm->sdram_data3 != 1437226410 )
        {
          if ( error <= 9 )
            printf("sdram_data3 = %x %d\n", mm->sdram_data3, status);
          ++error;
        }
        if ( mm->sdram_data4 != 1437226410 )
        {
          if ( error <= 9 )
            printf("sdram_data4 = %x %d\n", mm->sdram_data4, status);
          ++error;
        }
      }
      ++status;
    }
    ++mm;
  }
  if ( !error )
    return 0;
  return test;
}

int GIF_EXAMPLECheck(NN *nn, NN *end)
{
  int dat2; // [esp+4h] [ebp-2Ch]
  int dat1; // [esp+8h] [ebp-28h]
  int ci; // [esp+Ch] [ebp-24h]
  unsigned __int8 dat[30]; // [esp+10h] [ebp-20h] BYREF

  while ( end > nn )
  {
    DataConv(nn, dat);
    for ( ci = 0; ci <= 29; ++ci )
    {
      dat1 = dat[ci];
      dat2 = gif.m_buff1[gif.m_count1];
      if ( ci % 10 == 1 )
      {
        dat1 = dat[ci] & 0x7F;
        dat2 = gif.m_buff1[gif.m_count1] & 0x7F;
      }
      if ( dat2 == dat1 )
      {
        ++gif.m_count1;
      }
      else if ( gif.m_count1 > 20 && gif.m_complete )
      {
        if ( gif.m_error1 <= 9 )
        {
          printf("NG-->%d %02X %02X\n", gif.m_count1, dat1, dat2);
          printf("m_complete->%d\n", gif.m_complete);
        }
        ++gif.m_error1;
        gif.m_count1 = 0;
      }
      else
      {
        gif.m_count1 = 0;
      }
      if ( gif.m_size1 == gif.m_count1 )
      {
        ++gif.m_complete;
        gif.m_count1 = 0;
      }
    }
    ++nn;
  }
  return 0;
}

int GIF_GINTCheck(NN *nn, NN *end, int value)
{
  int ci; // [esp+0h] [ebp-24h]
  unsigned __int8 dat[30]; // [esp+4h] [ebp-20h] BYREF

  while ( end > nn )
  {
    DataConv(nn, dat);
    for ( ci = 0; ci <= 2; ++ci )
    {
      if ( value != (dat[10 * ci] & 1) )
      {
        printf("NG-->%d %02X %02X\n", ci, dat[10 * ci], value);
        return 1;
      }
    }
    ++nn;
  }
  return 0;
}

int GIF_TEXSWAPCheck(NN *nn, NN *end)
{
  int offset; // [esp+4h] [ebp-34h]
  int dat2; // [esp+8h] [ebp-30h]
  int dat2_1; // [esp+8h] [ebp-30h]
  int dat2_2; // [esp+8h] [ebp-30h]
  int dat2_3; // [esp+8h] [ebp-30h]
  int dat1; // [esp+Ch] [ebp-2Ch]
  int dat1_1; // [esp+Ch] [ebp-2Ch]
  int dat1_2; // [esp+Ch] [ebp-2Ch]
  int dat1_3; // [esp+Ch] [ebp-2Ch]
  int cj; // [esp+10h] [ebp-28h]
  int cj_1; // [esp+10h] [ebp-28h]
  int cj_2; // [esp+10h] [ebp-28h]
  int cj_3; // [esp+10h] [ebp-28h]
  int ci; // [esp+14h] [ebp-24h]
  unsigned __int8 dat[30]; // [esp+18h] [ebp-20h] BYREF

  while ( end > nn )
  {
    DataConv(nn, dat);
    offset = 0;
    for ( ci = 0; ci <= 2; ++ci )
    {
      if ( (dat[10 * ci] & 0x7C) == 76 )
      {
        offset = 10 * ci;
        gif.m_flag = 1;
      }
      else if ( (dat[10 * ci] & 0x76) == 114 )
      {
        offset = 10 * ci;
        gif.m_flag = 2;
      }
      else if ( (dat[10 * ci] & 0x7E) == 14 )
      {
        offset = 10 * ci;
        gif.m_flag = 3;
      }
      else if ( (dat[10 * ci] & 0x7E) == 58 )
      {
        offset = 10 * ci;
        gif.m_flag = 4;
      }
      else
      {
        gif.m_flag = 0;
      }
      if ( gif.m_flag == 2 )
      {
        for ( cj_1 = offset + 2; cj_1 < offset + 10; ++cj_1 )
        {
          dat1_1 = dat[cj_1];
          dat2_1 = gif.m_buff2[gif.m_count2];
          if ( dat2_1 == dat1_1 )
          {
            ++gif.m_count2;
            gif.m_flag = 0;
          }
          if ( dat2_1 != dat1_1 && gif.m_size2 > gif.m_count2 )
          {
            if ( gif.m_count2 <= 10 )
            {
              gif.m_count2 = 0;
            }
            else
            {
              if ( gif.m_error2 <= 9 )
                printf("NG[MODE2]->%d %02X %02X\n", gif.m_count2, dat1_1, dat2_1);
              ++gif.m_error2;
            }
            gif.m_flag = 0;
          }
          if ( gif.m_size2 == gif.m_count2 )
          {
            ++gif.m_complete;
            gif.m_flag = 0;
          }
        }
      }
      else if ( gif.m_flag > 2 )
      {
        if ( gif.m_flag == 3 )
        {
          for ( cj_2 = offset; cj_2 < offset + 10; ++cj_2 )
          {
            dat1_2 = dat[cj_2];
            dat2_2 = gif.m_buff3[gif.m_count3];
            if ( !(cj_2 % 10) )
            {
              dat1_2 &= 0x7Eu;
              dat2_2 &= 0x7Eu;
            }
            if ( cj_2 % 10 == 1 )
            {
              dat1_2 &= 0x7Fu;
              dat2_2 &= 0x7Fu;
            }
            if ( dat2_2 == dat1_2 )
            {
              ++gif.m_count3;
              gif.m_flag = 0;
            }
            if ( dat2_2 != dat1_2 && gif.m_size3 > gif.m_count3 )
            {
              if ( gif.m_count3 <= 10 )
              {
                gif.m_count3 = 0;
              }
              else
              {
                if ( gif.m_error3 <= 9 )
                  printf("NG[MODE3]->%d %02X %02X\n", gif.m_count3, dat1_2, dat2_2);
                ++gif.m_error3;
              }
              gif.m_flag = 0;
            }
            if ( gif.m_size3 == gif.m_count3 )
            {
              ++gif.m_complete;
              gif.m_flag = 0;
            }
          }
        }
        else if ( gif.m_flag == 4 )
        {
          for ( cj_3 = offset; cj_3 < offset + 10; ++cj_3 )
          {
            dat1_3 = dat[cj_3];
            dat2_3 = gif.m_buff4[gif.m_count4];
            if ( !(cj_3 % 10) )
            {
              dat1_3 &= 0x7Eu;
              dat2_3 &= 0x7Eu;
            }
            if ( cj_3 % 10 == 1 )
            {
              dat1_3 &= 0x7Fu;
              dat2_3 &= 0x7Fu;
            }
            if ( dat2_3 == dat1_3 )
            {
              ++gif.m_count4;
              gif.m_flag = 0;
            }
            if ( dat2_3 != dat1_3 && gif.m_size4 > gif.m_count4 )
            {
              if ( gif.m_count4 <= 10 )
              {
                gif.m_count4 = 0;
              }
              else
              {
                if ( gif.m_error4 <= 9 )
                  printf("NG[MODE4]->%d %02X %02X\n", gif.m_count4, dat1_3, dat2_3);
                ++gif.m_error4;
              }
              gif.m_flag = 0;
            }
            if ( gif.m_size4 == gif.m_count4 )
            {
              ++gif.m_complete;
              gif.m_flag = 0;
            }
          }
        }
      }
      else if ( gif.m_flag == 1 )
      {
        for ( cj = offset; cj < offset + 10; ++cj )
        {
          dat1 = dat[cj];
          dat2 = gif.m_buff1[gif.m_count1];
          if ( !(cj % 10) )
          {
            dat1 &= 0x7Cu;
            dat2 &= 0x7Cu;
          }
          if ( cj % 10 == 1 )
          {
            dat1 &= 0x7Fu;
            dat2 &= 0x7Fu;
          }
          if ( dat2 == dat1 )
          {
            ++gif.m_count1;
            gif.m_flag = 0;
          }
          if ( dat2 != dat1 && gif.m_size1 > gif.m_count1 )
          {
            if ( gif.m_count1 <= 10 )
            {
              gif.m_count1 = 0;
            }
            else
            {
              if ( gif.m_error1 <= 9 )
                printf("NG[MODE1]->%d %02X %02X\n", gif.m_count1, dat1, dat2);
              ++gif.m_error1;
            }
            gif.m_flag = 0;
          }
          if ( gif.m_size1 == gif.m_count1 )
          {
            ++gif.m_complete;
            gif.m_flag = 0;
          }
        }
      }
    }
    ++nn;
  }
  return 0;
}

int check_gif(char *p, int mode, int size, char *checkStr)
{
  NN *v5; // [esp+4h] [ebp-14h]
  int qword; // [esp+14h] [ebp-4h]

  qword = 1;
  v5 = (NN *)&p[size];
  if ( !strcmp(checkStr, "GIF_SDRAMCheck") )
    return GIF_SDRAMCheck((MM *)p, (MM *)&p[size], mode);
  if ( !strcmp(checkStr, "GIF_EXAMPLECheck") )
    return GIF_EXAMPLECheck((NN *)p, v5);
  if ( !strcmp(checkStr, "GIF_TEXSWAPCheck") )
    return GIF_TEXSWAPCheck((NN *)p, v5);
  if ( !strcmp(checkStr, "GIF_GINT_H_Check") )
    return GIF_GINTCheck((NN *)p, v5, 1);
  if ( !strcmp(checkStr, "GIF_GINT_L_Check") )
    return GIF_GINTCheck((NN *)p, v5, 0);
  return qword;
}

