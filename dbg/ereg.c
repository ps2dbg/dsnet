
#include "dsxdb_prototypes.h"

extern int dsc_connected; // defined in dbg.c
static EE_REGS ee_regs[] =
{
  { 0, 0, 16, "zero" },
  { 0, 1, 16, "at" },
  { 0, 2, 16, "v0" },
  { 0, 3, 16, "v1" },
  { 0, 4, 16, "a0" },
  { 0, 5, 16, "a1" },
  { 0, 6, 16, "a2" },
  { 0, 7, 16, "a3" },
  { 0, 8, 16, "t0" },
  { 0, 9, 16, "t1" },
  { 0, 10, 16, "t2" },
  { 0, 11, 16, "t3" },
  { 0, 12, 16, "t4" },
  { 0, 13, 16, "t5" },
  { 0, 14, 16, "t6" },
  { 0, 15, 16, "t7" },
  { 0, 16, 16, "s0" },
  { 0, 17, 16, "s1" },
  { 0, 18, 16, "s2" },
  { 0, 19, 16, "s3" },
  { 0, 20, 16, "s4" },
  { 0, 21, 16, "s5" },
  { 0, 22, 16, "s6" },
  { 0, 23, 16, "s7" },
  { 0, 24, 16, "t8" },
  { 0, 25, 16, "t9" },
  { 0, 26, 16, "k0" },
  { 0, 27, 16, "k1" },
  { 0, 28, 16, "gp" },
  { 0, 29, 16, "sp" },
  { 0, 30, 16, "fp" },
  { 0, 31, 16, "ra" },
  { 1, 0, 8, "hi" },
  { 1, 1, 8, "lo" },
  { 1, 2, 8, "hi1" },
  { 1, 3, 8, "lo1" },
  { 1, 4, 4, "sa" },
  { 2, 0, 4, "index" },
  { 2, 1, 4, "random" },
  { 2, 2, 4, "entrylo0" },
  { 2, 3, 4, "entrylo1" },
  { 2, 4, 4, "context" },
  { 2, 5, 4, "pagemask" },
  { 2, 6, 4, "wired" },
  { 2, 7, 4, "rsvd7" },
  { 2, 8, 4, "badvaddr" },
  { 2, 9, 4, "count" },
  { 2, 10, 4, "entryhi" },
  { 2, 11, 4, "compare" },
  { 2, 12, 4, "status" },
  { 2, 13, 4, "cause" },
  { 2, 14, 4, "epc" },
  { 2, 15, 4, "prid" },
  { 2, 16, 4, "config" },
  { 2, 17, 4, "rsvd17" },
  { 2, 18, 4, "rsvd18" },
  { 2, 19, 4, "rsvd19" },
  { 2, 20, 4, "rsvd20" },
  { 2, 21, 4, "rsvd21" },
  { 2, 22, 4, "rsvd22" },
  { 2, 23, 4, "badpaddr" },
  { 2, 24, 4, "debug" },
  { 2, 25, 4, "perf" },
  { 2, 26, 4, "rsvd26" },
  { 2, 27, 4, "rsvd27" },
  { 2, 28, 4, "taglo" },
  { 2, 29, 4, "taghi" },
  { 2, 30, 4, "errorepc" },
  { 2, 31, 4, "rsvd31" },
  { 3, 0, 4, "ccr" },
  { 3, 1, 4, "ctr0" },
  { 3, 2, 4, "ctr1" },
  { 4, 0, 4, "bpc" },
  { 4, 1, 4, "rsvd" },
  { 4, 2, 4, "iab" },
  { 4, 3, 4, "iabm" },
  { 4, 4, 4, "dab" },
  { 4, 5, 4, "dabm" },
  { 4, 6, 4, "dvb" },
  { 4, 7, 4, "dvbm" },
  { 5, 0, 4, "fpr0" },
  { 5, 1, 4, "fpr1" },
  { 5, 2, 4, "fpr2" },
  { 5, 3, 4, "fpr3" },
  { 5, 4, 4, "fpr4" },
  { 5, 5, 4, "fpr5" },
  { 5, 6, 4, "fpr6" },
  { 5, 7, 4, "fpr7" },
  { 5, 8, 4, "fpr8" },
  { 5, 9, 4, "fpr9" },
  { 5, 10, 4, "fpr10" },
  { 5, 11, 4, "fpr11" },
  { 5, 12, 4, "fpr12" },
  { 5, 13, 4, "fpr13" },
  { 5, 14, 4, "fpr14" },
  { 5, 15, 4, "fpr15" },
  { 5, 16, 4, "fpr16" },
  { 5, 17, 4, "fpr17" },
  { 5, 18, 4, "fpr18" },
  { 5, 19, 4, "fpr19" },
  { 5, 20, 4, "fpr20" },
  { 5, 21, 4, "fpr21" },
  { 5, 22, 4, "fpr22" },
  { 5, 23, 4, "fpr23" },
  { 5, 24, 4, "fpr24" },
  { 5, 25, 4, "fpr25" },
  { 5, 26, 4, "fpr26" },
  { 5, 27, 4, "fpr27" },
  { 5, 28, 4, "fpr28" },
  { 5, 29, 4, "fpr29" },
  { 5, 30, 4, "fpr30" },
  { 5, 31, 4, "fpr31" },
  { 6, 0, 4, "fcr0" },
  { 6, 1, 4, "fcr31" },
  { 7, 0, 16, "vu0vf00" },
  { 7, 1, 16, "vu0vf01" },
  { 7, 2, 16, "vu0vf02" },
  { 7, 3, 16, "vu0vf03" },
  { 7, 4, 16, "vu0vf04" },
  { 7, 5, 16, "vu0vf05" },
  { 7, 6, 16, "vu0vf06" },
  { 7, 7, 16, "vu0vf07" },
  { 7, 8, 16, "vu0vf08" },
  { 7, 9, 16, "vu0vf09" },
  { 7, 10, 16, "vu0vf10" },
  { 7, 11, 16, "vu0vf11" },
  { 7, 12, 16, "vu0vf12" },
  { 7, 13, 16, "vu0vf13" },
  { 7, 14, 16, "vu0vf14" },
  { 7, 15, 16, "vu0vf15" },
  { 7, 16, 16, "vu0vf16" },
  { 7, 17, 16, "vu0vf17" },
  { 7, 18, 16, "vu0vf18" },
  { 7, 19, 16, "vu0vf19" },
  { 7, 20, 16, "vu0vf20" },
  { 7, 21, 16, "vu0vf21" },
  { 7, 22, 16, "vu0vf22" },
  { 7, 23, 16, "vu0vf23" },
  { 7, 24, 16, "vu0vf24" },
  { 7, 25, 16, "vu0vf25" },
  { 7, 26, 16, "vu0vf26" },
  { 7, 27, 16, "vu0vf27" },
  { 7, 28, 16, "vu0vf28" },
  { 7, 29, 16, "vu0vf29" },
  { 7, 30, 16, "vu0vf30" },
  { 7, 31, 16, "vu0vf31" },
  { 8, 0, 2, "vu0vi00" },
  { 8, 1, 2, "vu0vi01" },
  { 8, 2, 2, "vu0vi02" },
  { 8, 3, 2, "vu0vi03" },
  { 8, 4, 2, "vu0vi04" },
  { 8, 5, 2, "vu0vi05" },
  { 8, 6, 2, "vu0vi06" },
  { 8, 7, 2, "vu0vi07" },
  { 8, 8, 2, "vu0vi08" },
  { 8, 9, 2, "vu0vi09" },
  { 8, 10, 2, "vu0vi10" },
  { 8, 11, 2, "vu0vi11" },
  { 8, 12, 2, "vu0vi12" },
  { 8, 13, 2, "vu0vi13" },
  { 8, 14, 2, "vu0vi14" },
  { 8, 15, 2, "vu0vi15" },
  { 8, 16, 4, "vu0vi16" },
  { 8, 17, 4, "vu0vi17" },
  { 8, 18, 4, "vu0vi18" },
  { 8, 19, 4, "vu0vi19" },
  { 8, 20, 4, "vu0vi20" },
  { 8, 21, 4, "vu0vi21" },
  { 8, 22, 4, "vu0vi22" },
  { 8, 23, 4, "vu0vi23" },
  { 8, 24, 4, "vu0vi24" },
  { 8, 25, 4, "vu0vi25" },
  { 8, 26, 4, "vu0vi26" },
  { 8, 27, 4, "vu0vi27" },
  { 8, 28, 4, "vu0vi28" },
  { 8, 29, 4, "vu0vi29" },
  { 8, 30, 4, "vu0vi30" },
  { 8, 31, 4, "vu0vi31" },
  { 9, 0, 16, "vu1vf00" },
  { 9, 1, 16, "vu1vf01" },
  { 9, 2, 16, "vu1vf02" },
  { 9, 3, 16, "vu1vf03" },
  { 9, 4, 16, "vu1vf04" },
  { 9, 5, 16, "vu1vf05" },
  { 9, 6, 16, "vu1vf06" },
  { 9, 7, 16, "vu1vf07" },
  { 9, 8, 16, "vu1vf08" },
  { 9, 9, 16, "vu1vf09" },
  { 9, 10, 16, "vu1vf10" },
  { 9, 11, 16, "vu1vf11" },
  { 9, 12, 16, "vu1vf12" },
  { 9, 13, 16, "vu1vf13" },
  { 9, 14, 16, "vu1vf14" },
  { 9, 15, 16, "vu1vf15" },
  { 9, 16, 16, "vu1vf16" },
  { 9, 17, 16, "vu1vf17" },
  { 9, 18, 16, "vu1vf18" },
  { 9, 19, 16, "vu1vf19" },
  { 9, 20, 16, "vu1vf20" },
  { 9, 21, 16, "vu1vf21" },
  { 9, 22, 16, "vu1vf22" },
  { 9, 23, 16, "vu1vf23" },
  { 9, 24, 16, "vu1vf24" },
  { 9, 25, 16, "vu1vf25" },
  { 9, 26, 16, "vu1vf26" },
  { 9, 27, 16, "vu1vf27" },
  { 9, 28, 16, "vu1vf28" },
  { 9, 29, 16, "vu1vf29" },
  { 9, 30, 16, "vu1vf30" },
  { 9, 31, 16, "vu1vf31" },
  { 10, 0, 2, "vu1vi00" },
  { 10, 1, 2, "vu1vi01" },
  { 10, 2, 2, "vu1vi02" },
  { 10, 3, 2, "vu1vi03" },
  { 10, 4, 2, "vu1vi04" },
  { 10, 5, 2, "vu1vi05" },
  { 10, 6, 2, "vu1vi06" },
  { 10, 7, 2, "vu1vi07" },
  { 10, 8, 2, "vu1vi08" },
  { 10, 9, 2, "vu1vi09" },
  { 10, 10, 2, "vu1vi10" },
  { 10, 11, 2, "vu1vi11" },
  { 10, 12, 2, "vu1vi12" },
  { 10, 13, 2, "vu1vi13" },
  { 10, 14, 2, "vu1vi14" },
  { 10, 15, 2, "vu1vi15" },
  { 10, 16, 4, "vu1vi16" },
  { 10, 17, 4, "vu1vi17" },
  { 10, 18, 4, "vu1vi18" },
  { 10, 19, 4, "vu1vi19" },
  { 10, 20, 4, "vu1vi20" },
  { 10, 21, 4, "vu1vi21" },
  { 10, 22, 4, "vu1vi22" },
  { 10, 23, 4, "vu1vi23" },
  { 10, 24, 4, "vu1vi24" },
  { 10, 25, 4, "vu1vi25" },
  { 10, 26, 4, "vu1vi26" },
  { 10, 27, 4, "vu1vi27" },
  { 10, 28, 4, "vu1vi28" },
  { 10, 29, 4, "vu1vi29" },
  { 10, 30, 4, "vu1vi30" },
  { 10, 31, 4, "vu1vi31" },
  { 0, 0, 0, NULL }
};

extern quad regbuf_vals[11][32]; // defined in dbg.c
extern unsigned int regbuf_mask[11]; // defined in dbg.c

static void print_symcr(unsigned int rcr);
static void print_rcr(unsigned int rcr);
static void print_symsr(unsigned int rsr);
static void print_rsr(unsigned int rsr);
static void print_bpc(unsigned int bpc);
static void print_vuc(char *name, int kind, int number, unsigned int val);
static int set_regmasks(unsigned int *masks, char *name);
static int dr_default_setmask(unsigned int *masks, int kind, int number);
static int dr_default(char *fmt, unsigned int *masks, quad *vals);

static void print_symcr(unsigned int rcr)
{
  unsigned int v1; // eax

  if ( (rcr & 0x80000000) == 0 )
    ds_printf("");
  else
    ds_printf(" BD");
  if ( (rcr & 0x40000000) != 0 )
    ds_printf(" BD2");
  else
    ds_printf("");
  ds_printf(" CE%d", (rcr & 0x30000000) >> 28);
  ds_printf(" EXC2=");
  v1 = rcr & 0x70000;
  if ( (rcr & 0x70000) == 0x10000 )
  {
    ds_printf("NMI");
    goto LABEL_19;
  }
  if ( v1 > 0x10000 )
  {
    if ( v1 == 0x20000 )
    {
      ds_printf("PerfC");
      goto LABEL_19;
    }
    if ( v1 == 196608 )
    {
      ds_printf("Debug");
      goto LABEL_19;
    }
  }
  else if ( !v1 )
  {
    ds_printf("Reset");
    goto LABEL_19;
  }
  ds_printf("???");
LABEL_19:
  if ( (rcr & 0x8000u) == 0 )
    ds_printf("");
  else
    ds_printf(" IP7");
  if ( (rcr & 0x1000) != 0 )
    ds_printf(" SIOP");
  else
    ds_printf("");
  if ( (rcr & 0x800) != 0 )
    ds_printf(" IP3");
  else
    ds_printf("");
  if ( (rcr & 0x400) != 0 )
    ds_printf(" IP2");
  else
    ds_printf("");
  ds_printf(" EXC=\"");
  switch ( rcr & 0x3C )
  {
    case 0u:
      ds_printf("External Interrupt");
      break;
    case 4u:
      ds_printf("TLB modification");
      break;
    case 8u:
      ds_printf("TLB miss,load/fetch");
      break;
    case 0xCu:
      ds_printf("TLB miss,store");
      break;
    case 0x10u:
      ds_printf("Address Error (load/fetch)");
      break;
    case 0x14u:
      ds_printf("Address Error (store)");
      break;
    case 0x18u:
      ds_printf("Bus Error (fetch)");
      break;
    case 0x1Cu:
      ds_printf("Bus Error (load/store)");
      break;
    case 0x20u:
      ds_printf("SYSCALL");
      break;
    case 0x24u:
      ds_printf("Breakpoint");
      break;
    case 0x28u:
      ds_printf("Reserved Instruction");
      break;
    case 0x2Cu:
      ds_printf("Coprocessor Unusable");
      break;
    case 0x30u:
      ds_printf("Arithmetic Overflow");
      break;
    case 0x34u:
      ds_printf("Trap exception");
      break;
    default:
      ds_printf("???");
      break;
  }
  ds_printf("\" ");
}

static void print_rcr(unsigned int rcr)
{
  ds_printf("$cause   = 0x%W [", rcr);
  print_symcr(rcr);
  ds_printf("]");
}

static void print_symsr(unsigned int rsr)
{
  char *v1; // eax
  char *v2; // eax
  char *v3; // eax
  char *v4; // eax
  const char *v5; // eax
  const char *v6; // eax
  const char *v7; // eax
  const char *v8; // eax
  const char *v9; // eax
  const char *v10; // eax
  const char *v11; // eax
  const char *v12; // eax
  const char *v13; // eax
  const char *v14; // eax
  unsigned int v15; // eax
  const char *v16; // eax
  const char *v17; // eax
  const char *v18; // eax
  const char *v19; // [esp-10h] [ebp-10h]
  const char *v20; // [esp-Ch] [ebp-Ch]
  const char *v21; // [esp-8h] [ebp-8h]
  const char *v22; // [esp-4h] [ebp-4h]

  if ( (rsr & 0x10000000) != 0 )
    v1 = "0";
  else
    v1 = "";
  v22 = v1;
  if ( (rsr & 0x20000000) != 0 )
    v2 = "1";
  else
    v2 = "";
  v21 = v2;
  if ( (rsr & 0x40000000) != 0 )
    v3 = "2";
  else
    v3 = "";
  v20 = v3;
  if ( (rsr & 0x80000000) == 0 )
    v4 = "";
  else
    v4 = "3";
  v19 = v4;
  if ( (rsr & 0xF0000000) != 0 )
    v5 = " Cu";
  else
    v5 = "";
  ds_printf("%s%s%s%s%s", v5, v19, v20, v21, v22);
  if ( (rsr & 0x800000) != 0 )
    v6 = " DEV";
  else
    v6 = "";
  ds_printf("%s", v6);
  if ( (rsr & 0x400000) != 0 )
    v7 = " BEV";
  else
    v7 = "";
  ds_printf("%s", v7);
  if ( (rsr & 0x40000) != 0 )
    v8 = " CH";
  else
    v8 = "";
  ds_printf("%s", v8);
  if ( (rsr & 0x20000) != 0 )
    v9 = " EDI";
  else
    v9 = "";
  ds_printf("%s", v9);
  if ( (rsr & 0x10000) != 0 )
    v10 = " EIE";
  else
    v10 = "";
  ds_printf("%s", v10);
  if ( (rsr & 0x8000u) == 0 )
    v11 = "";
  else
    v11 = " IM7";
  ds_printf("%s", v11);
  if ( (rsr & 0x1000) != 0 )
    v12 = " BEM";
  else
    v12 = "";
  ds_printf("%s", v12);
  if ( (rsr & 0x800) != 0 )
    v13 = " IM3";
  else
    v13 = "";
  ds_printf("%s", v13);
  if ( (rsr & 0x400) != 0 )
    v14 = " IM2";
  else
    v14 = "";
  ds_printf("%s", v14);
  ds_printf(" KSU=");
  v15 = rsr & 0x18;
  if ( v15 == 8 )
  {
    ds_printf("Supervisor");
  }
  else if ( v15 > 8 )
  {
    if ( v15 == 16 )
    {
      ds_printf("User");
    }
    else if ( v15 == 24 )
    {
      ds_printf("Reserved");
    }
  }
  else if ( (rsr & 0x18) == 0 )
  {
    ds_printf("Kernel");
  }
  if ( (rsr & 4) != 0 )
    v16 = " ERL";
  else
    v16 = "";
  ds_printf("%s", v16);
  if ( (rsr & 2) != 0 )
    v17 = " EXL";
  else
    v17 = "";
  ds_printf("%s", v17);
  if ( (rsr & 1) != 0 )
    v18 = " IE";
  else
    v18 = "";
  ds_printf("%s", v18);
  ds_printf(" ");
}

static void print_rsr(unsigned int rsr)
{
  ds_printf("$status  = 0x%W [", rsr);
  print_symsr(rsr);
  ds_printf("]");
}

static void print_bpc(unsigned int bpc)
{
  const char *v1; // eax
  const char *v2; // eax
  const char *v3; // eax
  const char *v4; // eax
  const char *v5; // eax
  const char *v6; // eax
  const char *v7; // eax
  const char *v8; // eax
  const char *v9; // eax
  const char *v10; // eax
  const char *v11; // eax
  const char *v12; // eax
  const char *v13; // eax
  const char *v14; // eax
  const char *v15; // eax
  const char *v16; // eax
  const char *v17; // eax
  const char *v18; // eax

  ds_printf("$bpc     = 0x%W [", bpc);
  if ( (bpc & 0x80000000) == 0 )
    v1 = "";
  else
    v1 = " IAE";
  ds_printf("%s", v1);
  if ( (bpc & 0x40000000) != 0 )
    v2 = " DRE";
  else
    v2 = "";
  ds_printf("%s", v2);
  if ( (bpc & 0x20000000) != 0 )
    v3 = " DWE";
  else
    v3 = "";
  ds_printf("%s", v3);
  if ( (bpc & 0x10000000) != 0 )
    v4 = " DVE";
  else
    v4 = "";
  ds_printf("%s", v4);
  if ( (bpc & 0x4000000) != 0 )
    v5 = " IUE";
  else
    v5 = "";
  ds_printf("%s", v5);
  if ( (bpc & 0x2000000) != 0 )
    v6 = " ISE";
  else
    v6 = "";
  ds_printf("%s", v6);
  if ( (bpc & 0x1000000) != 0 )
    v7 = " IKE";
  else
    v7 = "";
  ds_printf("%s", v7);
  if ( (bpc & 0x800000) != 0 )
    v8 = " IXE";
  else
    v8 = "";
  ds_printf("%s", v8);
  if ( (bpc & 0x200000) != 0 )
    v9 = " DUE";
  else
    v9 = "";
  ds_printf("%s", v9);
  if ( (bpc & 0x100000) != 0 )
    v10 = " DSE";
  else
    v10 = "";
  ds_printf("%s", v10);
  if ( (bpc & 0x80000) != 0 )
    v11 = " DKE";
  else
    v11 = "";
  ds_printf("%s", v11);
  if ( (bpc & 0x40000) != 0 )
    v12 = " DXE";
  else
    v12 = "";
  ds_printf("%s", v12);
  if ( (bpc & 0x20000) != 0 )
    v13 = " ITE";
  else
    v13 = "";
  ds_printf("%s", v13);
  if ( (bpc & 0x10000) != 0 )
    v14 = " DTE";
  else
    v14 = "";
  ds_printf("%s", v14);
  if ( (bpc & 0x8000u) == 0 )
    v15 = "";
  else
    v15 = " BED";
  ds_printf("%s", v15);
  if ( (bpc & 4) != 0 )
    v16 = " DWB";
  else
    v16 = "";
  ds_printf("%s", v16);
  if ( (bpc & 2) != 0 )
    v17 = " DRB";
  else
    v17 = "";
  ds_printf("%s", v17);
  if ( (bpc & 1) != 0 )
    v18 = " IAB";
  else
    v18 = "";
  ds_printf("%s", v18);
  ds_printf(" ]");
}

static void print_vuc(char *name, int kind, int number, unsigned int val)
{
  const char *v4; // eax
  const char *v5; // eax
  const char *v6; // eax
  const char *v7; // eax
  const char *v8; // eax
  const char *v9; // eax
  const char *v10; // eax
  const char *v11; // eax
  const char *v12; // eax
  const char *v13; // eax
  const char *v14; // eax
  const char *v15; // eax
  const char *v16; // eax
  const char *v17; // eax
  const char *v18; // eax
  const char *v19; // eax
  const char *v20; // eax
  const char *v21; // eax
  const char *v22; // eax
  const char *v23; // eax
  const char *v24; // eax
  const char *v25; // eax
  const char *v26; // eax
  const char *v27; // eax
  const char *v28; // eax
  const char *v29; // eax
  const char *v30; // eax
  const char *v31; // eax
  const char *v32; // eax
  const char *v33; // eax
  const char *v34; // eax
  const char *v35; // eax
  const char *v36; // eax
  const char *v37; // eax
  const char *v38; // eax
  const char *v39; // eax
  const char *v40; // eax
  const char *v41; // eax
  const char *v42; // eax
  const char *v43; // eax
  const char *v44; // eax
  const char *v45; // eax
  const char *v46; // eax
  const char *v47; // eax
  const char *v48; // eax
  const char *v49; // eax
  const char *v50; // eax
  const char *v51; // eax
  const char *v52; // eax
  const char *v53; // eax
  const char *v54; // eax
  const char *v55; // eax
  const char *v56; // eax
  const char *v57; // eax
  const char *v58; // eax
  const char *v59; // eax
  const char *v60; // eax
  const char *v61; // eax
  const char *v62; // eax
  const char *v63; // eax
  const char *v64; // eax
  const char *v65; // eax
  const char *v66; // eax
  const char *v67; // eax
  const char *v68; // eax
  const char *v69; // eax
  const char *v70; // eax
  const char *v71; // eax
  const char *v72; // eax
  EE_REGS *reg; // [esp+0h] [ebp-4h]

  if ( !name )
  {
    for ( reg = ee_regs; reg->name; ++reg )
    {
      if ( reg->kind == kind && reg->number == number )
      {
        name = reg->name;
        break;
      }
    }
  }
  ds_printf(" $%-8s= 0x%W", name, val);
  switch ( number )
  {
    case 16:
      ds_printf(" [status flag:");
      if ( (val & 0x800) != 0 )
        v4 = " DS";
      else
        v4 = "";
      ds_printf("%s", v4);
      if ( (val & 0x400) != 0 )
        v5 = " IS";
      else
        v5 = "";
      ds_printf("%s", v5);
      if ( (val & 0x200) != 0 )
        v6 = " OS";
      else
        v6 = "";
      ds_printf("%s", v6);
      if ( (val & 0x100) != 0 )
        v7 = " US";
      else
        v7 = "";
      ds_printf("%s", v7);
      if ( (val & 0x80u) == 0 )
        v8 = "";
      else
        v8 = " SS";
      ds_printf("%s", v8);
      if ( (val & 0x40) != 0 )
        v9 = " ZS";
      else
        v9 = "";
      ds_printf("%s", v9);
      if ( (val & 0x20) != 0 )
        v10 = " D";
      else
        v10 = "";
      ds_printf("%s", v10);
      if ( (val & 0x10) != 0 )
        v11 = " I";
      else
        v11 = "";
      ds_printf("%s", v11);
      if ( (val & 8) != 0 )
        v12 = " O";
      else
        v12 = "";
      ds_printf("%s", v12);
      if ( (val & 4) != 0 )
        v13 = " U";
      else
        v13 = "";
      ds_printf("%s", v13);
      if ( (val & 2) != 0 )
        v14 = " S";
      else
        v14 = "";
      ds_printf("%s", v14);
      if ( (val & 1) != 0 )
        v15 = " Z";
      else
        v15 = "";
      break;
    case 17:
      ds_printf(" [MAC flag:");
      if ( (val & 0x8000u) == 0 )
        v16 = "";
      else
        v16 = " OX";
      ds_printf("%s", v16);
      if ( (val & 0x4000) != 0 )
        v17 = " OY";
      else
        v17 = "";
      ds_printf("%s", v17);
      if ( (val & 0x2000) != 0 )
        v18 = " OZ";
      else
        v18 = "";
      ds_printf("%s", v18);
      if ( (val & 0x1000) != 0 )
        v19 = " OW";
      else
        v19 = "";
      ds_printf("%s", v19);
      if ( (val & 0x800) != 0 )
        v20 = " UX";
      else
        v20 = "";
      ds_printf("%s", v20);
      if ( (val & 0x400) != 0 )
        v21 = " UY";
      else
        v21 = "";
      ds_printf("%s", v21);
      if ( (val & 0x200) != 0 )
        v22 = " UZ";
      else
        v22 = "";
      ds_printf("%s", v22);
      if ( (val & 0x100) != 0 )
        v23 = " UW";
      else
        v23 = "";
      ds_printf("%s", v23);
      if ( (val & 0x80u) == 0 )
        v24 = "";
      else
        v24 = " SX";
      ds_printf("%s", v24);
      if ( (val & 0x40) != 0 )
        v25 = " SY";
      else
        v25 = "";
      ds_printf("%s", v25);
      if ( (val & 0x20) != 0 )
        v26 = " SZ";
      else
        v26 = "";
      ds_printf("%s", v26);
      if ( (val & 0x10) != 0 )
        v27 = " SW";
      else
        v27 = "";
      ds_printf("%s", v27);
      if ( (val & 8) != 0 )
        v28 = " ZX";
      else
        v28 = "";
      ds_printf("%s", v28);
      if ( (val & 4) != 0 )
        v29 = " ZY";
      else
        v29 = "";
      ds_printf("%s", v29);
      if ( (val & 2) != 0 )
        v30 = " ZZ";
      else
        v30 = "";
      ds_printf("%s", v30);
      if ( (val & 1) != 0 )
        v15 = " ZW";
      else
        v15 = "";
      break;
    case 18:
      ds_printf(" [clipping flag:");
      if ( (val & 0x800000) != 0 )
        v31 = " z3-";
      else
        v31 = "";
      ds_printf("%s", v31);
      if ( (val & 0x400000) != 0 )
        v32 = " z3+";
      else
        v32 = "";
      ds_printf("%s", v32);
      if ( (val & 0x200000) != 0 )
        v33 = " y3-";
      else
        v33 = "";
      ds_printf("%s", v33);
      if ( (val & 0x100000) != 0 )
        v34 = " y3+";
      else
        v34 = "";
      ds_printf("%s", v34);
      if ( (val & 0x80000) != 0 )
        v35 = " x3-";
      else
        v35 = "";
      ds_printf("%s", v35);
      if ( (val & 0x40000) != 0 )
        v36 = " x3+";
      else
        v36 = "";
      ds_printf("%s", v36);
      if ( (val & 0x20000) != 0 )
        v37 = " z2-";
      else
        v37 = "";
      ds_printf("%s", v37);
      if ( (val & 0x10000) != 0 )
        v38 = " z2+";
      else
        v38 = "";
      ds_printf("%s", v38);
      if ( (val & 0x8000u) == 0 )
        v39 = "";
      else
        v39 = " y2-";
      ds_printf("%s", v39);
      if ( (val & 0x4000) != 0 )
        v40 = " y2+";
      else
        v40 = "";
      ds_printf("%s", v40);
      if ( (val & 0x2000) != 0 )
        v41 = " x2-";
      else
        v41 = "";
      ds_printf("%s", v41);
      if ( (val & 0x1000) != 0 )
        v42 = " x2+";
      else
        v42 = "";
      ds_printf("%s", v42);
      if ( (val & 0x800) != 0 )
        v43 = " z1-";
      else
        v43 = "";
      ds_printf("%s", v43);
      if ( (val & 0x400) != 0 )
        v44 = " z1+";
      else
        v44 = "";
      ds_printf("%s", v44);
      if ( (val & 0x200) != 0 )
        v45 = " y1-";
      else
        v45 = "";
      ds_printf("%s", v45);
      if ( (val & 0x100) != 0 )
        v46 = " y1+";
      else
        v46 = "";
      ds_printf("%s", v46);
      if ( (val & 0x80u) == 0 )
        v47 = "";
      else
        v47 = " x1-";
      ds_printf("%s", v47);
      if ( (val & 0x40) != 0 )
        v48 = " x1+";
      else
        v48 = "";
      ds_printf("%s", v48);
      if ( (val & 0x20) != 0 )
        v49 = " z0-";
      else
        v49 = "";
      ds_printf("%s", v49);
      if ( (val & 0x10) != 0 )
        v50 = " z0+";
      else
        v50 = "";
      ds_printf("%s", v50);
      if ( (val & 8) != 0 )
        v51 = " y0-";
      else
        v51 = "";
      ds_printf("%s", v51);
      if ( (val & 4) != 0 )
        v52 = " y0+";
      else
        v52 = "";
      ds_printf("%s", v52);
      if ( (val & 2) != 0 )
        v53 = " x0-";
      else
        v53 = "";
      ds_printf("%s", v53);
      if ( (val & 1) != 0 )
        v15 = " x0+";
      else
        v15 = "";
      break;
    case 20:
      ds_printf(" [R register]");
      return;
    case 21:
      ds_printf(" [I register]");
      return;
    case 22:
      ds_printf(" [Q register]");
      return;
    case 23:
      if ( kind == 10 )
        ds_printf(" [P register]");
      return;
    case 26:
      ds_printf(" [TPC]");
      return;
    case 27:
      if ( kind == 8 )
        ds_printf(" [CMSAR0]");
      return;
    case 28:
      if ( kind != 8 )
        return;
      ds_printf(" [FBRST:");
      if ( (val & 0x800) != 0 )
        v54 = " TE1";
      else
        v54 = "";
      ds_printf("%s", v54);
      if ( (val & 0x400) != 0 )
        v55 = " DE1";
      else
        v55 = "";
      ds_printf("%s", v55);
      if ( (val & 0x200) != 0 )
        v56 = " RS1";
      else
        v56 = "";
      ds_printf("%s", v56);
      if ( (val & 0x100) != 0 )
        v57 = " FB1";
      else
        v57 = "";
      ds_printf("%s", v57);
      if ( (val & 8) != 0 )
        v58 = " TE0";
      else
        v58 = "";
      ds_printf("%s", v58);
      if ( (val & 4) != 0 )
        v59 = " DE0";
      else
        v59 = "";
      ds_printf("%s", v59);
      if ( (val & 2) != 0 )
        v60 = " RS0";
      else
        v60 = "";
      ds_printf("%s", v60);
      if ( (val & 1) != 0 )
        v15 = " FB0";
      else
        v15 = "";
      break;
    case 29:
      if ( kind != 8 )
        return;
      ds_printf(" [VPU-STAT:");
      if ( (val & 0x4000) != 0 )
        v61 = " EFU1";
      else
        v61 = "";
      ds_printf("%s", v61);
      if ( (val & 0x2000) != 0 )
        v62 = " DIV1";
      else
        v62 = "";
      ds_printf("%s", v62);
      if ( (val & 0x1000) != 0 )
        v63 = " VGW1";
      else
        v63 = "";
      ds_printf("%s", v63);
      if ( (val & 0x800) != 0 )
        v64 = " VFS1";
      else
        v64 = "";
      ds_printf("%s", v64);
      if ( (val & 0x400) != 0 )
        v65 = " VTS1";
      else
        v65 = "";
      ds_printf("%s", v65);
      if ( (val & 0x200) != 0 )
        v66 = " VDS1";
      else
        v66 = "";
      ds_printf("%s", v66);
      if ( (val & 0x100) != 0 )
        v67 = " VBS1";
      else
        v67 = "";
      ds_printf("%s", v67);
      if ( (val & 0x80u) == 0 )
        v68 = "";
      else
        v68 = " IBS0";
      ds_printf("%s", v68);
      if ( (val & 0x20) != 0 )
        v69 = " DIV0";
      else
        v69 = "";
      ds_printf("%s", v69);
      if ( (val & 8) != 0 )
        v70 = " VFS0";
      else
        v70 = "";
      ds_printf("%s", v70);
      if ( (val & 4) != 0 )
        v71 = " VTS0";
      else
        v71 = "";
      ds_printf("%s", v71);
      if ( (val & 2) != 0 )
        v72 = " VDS0";
      else
        v72 = "";
      ds_printf("%s", v72);
      if ( (val & 1) != 0 )
        v15 = " VBS0";
      else
        v15 = "";
      break;
    case 31:
      if ( kind == 8 )
        ds_printf(" [CMSAR1]");
      return;
    default:
      return;
  }
  ds_printf("%s", v15);
  ds_printf(" ]");
}

static int set_regmasks(unsigned int *masks, char *name)
{
  int v3; // eax
  int rk = 0; // [esp+18h] [ebp-10h]
  int n = 0; // [esp+1Ch] [ebp-Ch] BYREF
  EE_REGS *reg; // [esp+20h] [ebp-8h]

  if ( !strcmp("all", name) )
  {
    rk = 2047;
  }
  else if ( !strcmp("gpr", name) )
  {
    rk = 1;
  }
  else if ( !strcmp("hls", name) )
  {
    rk = 2;
  }
  else if ( !strcmp("scr", name)
         || !strcmp("scc", name)
         || !strcmp("c0r", name)
         || !strcmp("cop0", name)
         || !strcmp("cp0", name) )
  {
    rk = 4;
  }
  else if ( !strcmp("pcr", name) )
  {
    rk = 8;
  }
  else if ( !strcmp("hdr", name) )
  {
    rk = 16;
  }
  else if ( !strcmp("cop1", name) || !strcmp("cp1", name) )
  {
    rk = 96;
  }
  else if ( !strcmp("fpr", name) || !strcmp("c1r", name) )
  {
    rk = 32;
  }
  else if ( !strcmp("fpc", name) || !strcmp("c1c", name) )
  {
    rk = 64;
  }
  else if ( !strcmp("vu0", name) || !strcmp("cop2", name) || !strcmp("cp2", name) )
  {
    rk = 384;
  }
  else if ( !strcmp("vu0f", name) || !strcmp("c2r", name) )
  {
    rk = 128;
  }
  else if ( !strcmp("vu0i", name) || !strcmp("c2c", name) )
  {
    rk = 256;
  }
  else if ( !strcmp("vu1", name) )
  {
    rk = 1536;
  }
  else if ( !strcmp("vu1f", name) )
  {
    rk = 512;
  }
  else if ( !strcmp("vu1i", name) )
  {
    rk = 1024;
  }
  if ( rk )
  {
    for ( reg = ee_regs; reg->name; ++reg )
    {
      if ( ((rk >> reg->kind) & 1) != 0 )
      {
        masks[reg->kind] |= 1 << reg->number;
        ++n;
      }
    }
    return n;
  }
  else
  {
    if ( *name == '$' )
      name++;

    if ( *name == '_' )
      name++;

    if ( !strcmp("cr", name) )
    {
      name = "cause";
    }
    else if ( !strcmp("sr", name) )
    {
      name = "status";
    }

    for ( reg = ee_regs; reg->name; ++reg )
    {
      if ( !strcmp(name, reg->name) )
      {
        masks[reg->kind] |= 1 << reg->number;
        return 1;
      }
    }

    v3 = 0;
    if ( *name > '/' && *name <= 'C' )
      v3 = 1;

    if ( v3 && !ds_scan_digit_word(name, &n) )
    {
      for ( reg = ee_regs; reg->name; ++reg )
      {
        if ( !reg->kind && n == reg->number )
        {
          masks[reg->kind] |= 1 << reg->number;
          return 1;
        }
      }
    }
    ds_printf("invalid register name - %s\n", name);
    return -1;
  }
}

int check_reserved_name(char *name)
{
  EE_REGS *reg; // [esp+0h] [ebp-4h]

  if ( !ds_strncmp("BP", name, 2) )
    return 1;
  if ( !ds_strncmp("BT", name, 2) )
    return 1;
  if ( !ds_strncmp("SFA", name, 3) )
    return 1;
  if ( !ds_strncmp("SFS", name, 3) )
    return 1;
  if ( !strcmp("REBOOTCOUNT", name) )
    return 1;
  if ( !strcmp("cr", name) || !strcmp("sr", name) )
    return 1;
  for ( reg = ee_regs; reg->name; ++reg )
  {
    if ( !strcmp(name, reg->name) )
      return 1;
  }
  return 0;
}

int load_quad_reg(char *name, quad *pv)
{
  unsigned int v3; // edx
  unsigned int v4; // edx
  quad v5; // [esp+8h] [ebp-ACh] BYREF
  quad v6; // [esp+18h] [ebp-9Ch] BYREF
  int j; // [esp+28h] [ebp-8Ch]
  int i; // [esp+2Ch] [ebp-88h]
  int r; // [esp+30h] [ebp-84h]
  quad vals[4]; // [esp+34h] [ebp-80h] BYREF
  unsigned int wv; // [esp+74h] [ebp-40h] BYREF
  unsigned int rerpc; // [esp+78h] [ebp-3Ch]
  unsigned int repc; // [esp+7Ch] [ebp-38h]
  unsigned int rcr; // [esp+80h] [ebp-34h]
  unsigned int rsr; // [esp+84h] [ebp-30h]
  unsigned int masks[11]; // [esp+88h] [ebp-2Ch] BYREF

  ds_bzero(masks, sizeof(masks));
  if ( !ds_strncmp("$BP", name, 3) )
  {
    r = eval_bp_reg(name, &wv);
    if ( r < 0 )
      return r;
LABEL_16:
    ds_cwq(&v6, wv);
    *pv = v6;
    return 0;
  }
  if ( !ds_strncmp("$BT", name, 3) )
  {
    r = eval_bt_reg(name, &wv);
    if ( r < 0 )
      return r;
    goto LABEL_16;
  }
  if ( !ds_strncmp("$SFA", name, 4) )
  {
    r = eval_sfa_reg(name, &wv);
    if ( r < 0 )
      return r;
    goto LABEL_16;
  }
  if ( !ds_strncmp("$SFS", name, 4) )
  {
    r = eval_sfs_reg(name, &wv);
    if ( r < 0 )
      return r;
    goto LABEL_16;
  }
  if ( !strcmp("$REBOOTCOUNT", name) )
  {
    ds_cwq(&v6, dsc_connected);
    *pv = v6;
    return 0;
  }
  if ( !strcmp("$PC", name) || !strcmp("$_PC", name) )
  {
    masks[2] = 1073770496;
    if ( name[1] == 95 && masks[2] == (masks[2] & regbuf_mask[2]) )
    {
      rsr = ds_cqw(regbuf_vals[2][12]);
      rcr = ds_cqw(regbuf_vals[2][13]);
      repc = ds_cqw(regbuf_vals[2][14]);
      rerpc = ds_cqw(regbuf_vals[2][30]);
    }
    else
    {
      if ( load_quad_registers(masks, vals, 4) )
        return -1;
      rsr = ds_cqw(vals[0]);
      rcr = ds_cqw(vals[1]);
      repc = ds_cqw(vals[2]);
      rerpc = ds_cqw(vals[3]);
    }
    if ( (rsr & 4) != 0 )
    {
      v3 = rerpc;
      if ( (rcr & 0x40000000) != 0 )
        v3 = rerpc + 4;
      ds_cwq(&v6, v3);
      *pv = v6;
    }
    else
    {
      v4 = repc;
      if ( (rcr & 0x80000000) != 0 )
        v4 = repc + 4;
      ds_cwq(&v5, v4);
      *pv = v5;
    }
    return 0;
  }
  else if ( set_regmasks(masks, name) == 1 )
  {
    if ( *name == 36 && name[1] == 95 )
    {
      for ( i = 0; i <= 10; ++i )
      {
        if ( masks[i] )
        {
          if ( (masks[i] & regbuf_mask[i]) != 0 )
          {
            for ( j = 0; j <= 31; ++j )
            {
              if ( (masks[i] & (1 << j)) != 0 )
              {
                *pv = regbuf_vals[i][j];
                return 0;
              }
            }
          }
          return load_quad_registers(masks, pv, 1);
        }
      }
    }
    return load_quad_registers(masks, pv, 1);
  }
  else
  {
    return -1;
  }
}

int store_quad_reg(char *name, quad val)
{
  int n; // [esp+0h] [ebp-1634h]
  int i; // [esp+4h] [ebp-1630h]
  quad vals[352]; // [esp+8h] [ebp-162Ch] BYREF
  unsigned int masks[11]; // [esp+1608h] [ebp-2Ch] BYREF

  ds_bzero(masks, sizeof(masks));
  n = set_regmasks(masks, name);
  if ( n <= 0 )
    return -1;
  for ( i = 0; n > i; ++i )
    vals[i] = val;
  return store_quad_registers(masks, vals, n);
}

static int dr_default_setmask(unsigned int *masks, int kind, int number)
{
  if ( (masks[kind] & (1 << number)) != 0 )
    return 0;
  masks[kind] |= 1 << number;
  return 1;
}

static int dr_default(char *fmt, unsigned int *masks, quad *vals)
{
  char *v3; // eax
  int v4; // eax
  int v5; // eax
  int v6; // eax
  unsigned int v8; // eax
  unsigned int v9; // eax
  unsigned int v10; // eax
  unsigned int v11; // eax
  int v12; // eax
  unsigned int v13; // eax
  int v14; // eax
  int v15; // eax
  int v16; // [esp-4h] [ebp-34h]
  unsigned int v17; // [esp+4h] [ebp-2Ch]
  unsigned int v18; // [esp+8h] [ebp-28h]
  unsigned int v19; // [esp+Ch] [ebp-24h]
  signed int v20; // [esp+10h] [ebp-20h]
  char v21; // [esp+14h] [ebp-1Ch]
  char _c_4; // [esp+1Ah] [ebp-16h]
  char _c_5; // [esp+1Ah] [ebp-16h]
  char _c; // [esp+1Bh] [ebp-15h]
  char _c_3; // [esp+1Bh] [ebp-15h]
  int ch; // [esp+1Ch] [ebp-14h]
  int nreg; // [esp+20h] [ebp-10h]
  int nreg_1; // [esp+20h] [ebp-10h]
  int nreg_2; // [esp+20h] [ebp-10h]
  int nreg_3; // [esp+20h] [ebp-10h]
  int n; // [esp+24h] [ebp-Ch]
  int n_1; // [esp+24h] [ebp-Ch]
  EE_REGS *reg; // [esp+28h] [ebp-8h]
  EE_REGS *reg_1; // [esp+28h] [ebp-8h]
  char *p; // [esp+2Ch] [ebp-4h]
  char *p_1; // [esp+2Ch] [ebp-4h]

  nreg = 0;
  p = fmt;
  while ( *p )
  {
    v3 = p++;
    if ( *v3 == 37 && *p != 37 )
    {
      n = 2;
      if ( ds_strncmp("PC", p, 2) )
      {
        n = 8;
        if ( ds_strncmp("symcause", p, 8) )
        {
          n = 9;
          if ( ds_strncmp("symstatus", p, 9) )
          {
            for ( reg = ee_regs; reg->name; ++reg )
            {
              n = strlen(reg->name);
              if ( !ds_strncmp(p, reg->name, n) )
              {
                _c = p[n];
                v4 = 0;
                if ( _c > 47 && _c <= 57 )
                  v4 = 1;
                if ( !v4 )
                {
                  nreg += dr_default_setmask(masks, reg->kind, reg->number);
                  break;
                }
              }
            }
            if ( !reg->name )
              n = 0;
          }
          else
          {
            nreg += dr_default_setmask(masks, 2, 12);
          }
        }
        else
        {
          nreg += dr_default_setmask(masks, 2, 13);
        }
      }
      else
      {
        nreg_1 = dr_default_setmask(masks, 2, 12) + nreg;
        nreg_2 = dr_default_setmask(masks, 2, 13) + nreg_1;
        nreg_3 = dr_default_setmask(masks, 2, 14) + nreg_2;
        nreg = dr_default_setmask(masks, 2, 30) + nreg_3;
      }
      if ( n <= 0 )
      {
        while ( 1 )
        {
          if ( *p != 95 )
          {
            _c_3 = *p;
            v5 = 0;
            if ( *p > 96 && _c_3 <= 122 || _c_3 > 64 && _c_3 <= 90 )
              v5 = 1;
            if ( !v5 )
            {
              v6 = 0;
              if ( *p > 47 && *p <= 57 )
                v6 = 1;
              if ( !v6 )
                break;
            }
          }
          ++p;
        }
      }
      else
      {
        p += n;
      }
    }
  }
  if ( load_quad_registers(masks, vals, nreg) )
    return -1;
  p_1 = fmt;
  while ( *p_1 )
  {
    ch = *p_1++;
    if ( ch == 37 )
    {
      if ( *p_1 == 37 )
      {
        ds_printf("%c", 37);
      }
      else
      {
        n_1 = 2;
        if ( ds_strncmp("PC", p_1, 2) )
        {
          n_1 = 8;
          if ( ds_strncmp("symcause", p_1, 8) )
          {
            n_1 = 9;
            if ( ds_strncmp("symstatus", p_1, 9) )
            {
              for ( reg_1 = ee_regs; reg_1->name; ++reg_1 )
              {
                n_1 = strlen(reg_1->name);
                if ( !ds_strncmp(p_1, reg_1->name, n_1) )
                {
                  _c_4 = p_1[n_1];
                  v12 = 0;
                  if ( _c_4 > 47 && _c_4 <= 57 )
                    v12 = 1;
                  if ( !v12 )
                  {
                    v13 = ds_cqw(regbuf_vals[reg_1->kind][reg_1->number]);
                    ds_printf("%W", v13);
                    break;
                  }
                }
              }
              if ( !reg_1->name )
                n_1 = 0;
            }
            else
            {
              v11 = ds_cqw(regbuf_vals[2][12]);
              print_symsr(v11);
            }
          }
          else
          {
            v10 = ds_cqw(regbuf_vals[2][13]);
            print_symcr(v10);
          }
        }
        else
        {
          v21 = ds_cqw(regbuf_vals[2][12]);
          v20 = ds_cqw(regbuf_vals[2][13]);
          v19 = ds_cqw(regbuf_vals[2][14]);
          v18 = ds_cqw(regbuf_vals[2][30]);
          if ( (v21 & 4) != 0 )
          {
            v8 = v18;
            if ( (v20 & 0x40000000) != 0 )
              v8 = v18 + 4;
            v17 = v8;
          }
          else
          {
            v9 = v19;
            if ( v20 < 0 )
              v9 = v19 + 4;
            v17 = v9;
          }
          ds_printf("%W", v17);
        }
        if ( n_1 <= 0 )
        {
          ds_printf("%%");
          while ( 1 )
          {
            if ( *p_1 != 95 )
            {
              _c_5 = *p_1;
              v14 = 0;
              if ( *p_1 > 96 && _c_5 <= 122 || _c_5 > 64 && _c_5 <= 90 )
                v14 = 1;
              if ( !v14 )
              {
                v15 = 0;
                if ( *p_1 > 47 && *p_1 <= 57 )
                  v15 = 1;
                if ( !v15 )
                  break;
              }
            }
            v16 = *p_1++;
            ds_printf("%c", v16);
          }
        }
        else
        {
          p_1 += n_1;
        }
      }
    }
    else
    {
      ds_printf("%c", ch);
    }
  }
  return 0;
}

int dreg_cmd(int ac, char **av)
{
  char v3; // al
  unsigned int v4; // eax
  unsigned int v5; // eax
  unsigned int v6; // eax
  unsigned int v7; // eax
  unsigned int v8; // eax
  unsigned int v9; // eax
  unsigned int v10; // eax
  unsigned int v11; // eax
  unsigned int v12; // eax
  unsigned int v13; // eax
  char *v14; // eax
  char *v15; // eax
  unsigned int v16; // eax
  unsigned int v17; // eax
  unsigned int v18; // eax
  unsigned int v19; // eax
  unsigned int v20; // eax
  unsigned int v21; // eax
  unsigned int v22; // eax
  int size; // eax
  unsigned int v24; // eax
  unsigned int v25; // eax
  unsigned int v26; // [esp-1Ch] [ebp-16D0h]
  unsigned int v27; // [esp-1Ch] [ebp-16D0h]
  unsigned int v28; // [esp-18h] [ebp-16CCh]
  unsigned int v29; // [esp-18h] [ebp-16CCh]
  unsigned int v30; // [esp-18h] [ebp-16CCh]
  unsigned int v31; // [esp-14h] [ebp-16C8h]
  unsigned int v32; // [esp-14h] [ebp-16C8h]
  unsigned int v33; // [esp-14h] [ebp-16C8h]
  unsigned int v34; // [esp-14h] [ebp-16C8h]
  quad v35; // [esp-14h] [ebp-16C8h]
  quad v36; // [esp-10h] [ebp-16C4h]
  quad v37; // [esp-10h] [ebp-16C4h]
  quad v38; // [esp-10h] [ebp-16C4h]
  quad v39; // [esp-10h] [ebp-16C4h]
  quad v40; // [esp-10h] [ebp-16C4h]
  quad v41; // [esp-10h] [ebp-16C4h]
  unsigned int v42; // [esp-10h] [ebp-16C4h]
  unsigned int v43; // [esp-10h] [ebp-16C4h]
  unsigned int v44; // [esp-10h] [ebp-16C4h]
  unsigned int v45; // [esp-10h] [ebp-16C4h]
  quad v46; // [esp-10h] [ebp-16C4h]
  quad v47; // [esp-10h] [ebp-16C4h]
  quad v48; // [esp-10h] [ebp-16C4h]
  quad v49; // [esp-10h] [ebp-16C4h]
  quad v50; // [esp-10h] [ebp-16C4h]
  quad v51; // [esp-10h] [ebp-16C4h]
  quad v52; // [esp-10h] [ebp-16C4h]
  quad v53; // [esp-10h] [ebp-16C4h]
  unsigned int v54; // [esp-Ch] [ebp-16C0h]
  unsigned int v55; // [esp-Ch] [ebp-16C0h]
  unsigned int v56; // [esp-Ch] [ebp-16C0h]
  unsigned int v57; // [esp-Ch] [ebp-16C0h]
  unsigned int v58; // [esp-8h] [ebp-16BCh]
  unsigned int v59; // [esp-8h] [ebp-16BCh]
  unsigned int v60; // [esp-8h] [ebp-16BCh]
  unsigned int v61; // [esp-8h] [ebp-16BCh]
  unsigned int v62; // [esp-4h] [ebp-16B8h]
  unsigned int v63; // [esp-4h] [ebp-16B8h]
  unsigned int v64; // [esp-4h] [ebp-16B8h]
  unsigned int v65; // [esp-4h] [ebp-16B8h]
  unsigned int v66; // [esp-4h] [ebp-16B8h]
  unsigned int v67; // [esp-4h] [ebp-16B8h]
  unsigned int v68; // [esp-4h] [ebp-16B8h]
  float v69; // [esp+4h] [ebp-16B0h]
  float v70; // [esp+8h] [ebp-16ACh]
  float v71; // [esp+Ch] [ebp-16A8h]
  float v72; // [esp+10h] [ebp-16A4h]
  quad x; // [esp+14h] [ebp-16A0h] BYREF
  quad src; // [esp+24h] [ebp-1690h] BYREF
  float fv; // [esp+34h] [ebp-1680h]
  int kind; // [esp+38h] [ebp-167Ch]
  char *fmt; // [esp+3Ch] [ebp-1678h]
  unsigned int wz; // [esp+40h] [ebp-1674h]
  unsigned int rerpc; // [esp+44h] [ebp-1670h]
  unsigned int v80; // [esp+48h] [ebp-166Ch]
  unsigned int repc; // [esp+4Ch] [ebp-1668h]
  unsigned int rcr; // [esp+50h] [ebp-1664h]
  unsigned int rsr; // [esp+54h] [ebp-1660h]
  unsigned int v84; // [esp+58h] [ebp-165Ch]
  quad *v85; // [esp+5Ch] [ebp-1658h]
  quad *hilo; // [esp+60h] [ebp-1654h]
  char *p; // [esp+64h] [ebp-1650h]
  int cpuid; // [esp+68h] [ebp-164Ch]
  int f_xyzw; // [esp+6Ch] [ebp-1648h]
  int f_float; // [esp+70h] [ebp-1644h]
  int n; // [esp+74h] [ebp-1640h]
  int i; // [esp+78h] [ebp-163Ch]
  int r; // [esp+7Ch] [ebp-1638h]
  EE_REGS *reg; // [esp+80h] [ebp-1634h]
  quad *pv; // [esp+84h] [ebp-1630h]
  quad vals[352]; // [esp+88h] [ebp-162Ch] BYREF
  unsigned int masks[11]; // [esp+1688h] [ebp-2Ch] BYREF
  int aca; // [esp+16BCh] [ebp+8h]
  char **ava; // [esp+16C0h] [ebp+Ch]

  pv = vals;
  f_float = 0;
  f_xyzw = 0;
  cpuid = 0;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  for ( ava = av + 1; aca > 0 && **ava == 45; ++ava )
  {
    if ( !strcmp("-help", *ava) )
      return ds_error("Usage: dr [-<cpuid>] [-[hfxw]*] [<reg>]...");
    if ( !strcmp("-cpu", *ava) )
    {
      cpuid = 0;
    }
    else if ( !strcmp("-vu0", *ava) )
    {
      cpuid = 1;
    }
    else if ( !strcmp("-vu1", *ava) )
    {
      cpuid = 2;
    }
    else
    {
      for ( p = *ava + 1; *p; ++p )
      {
        v3 = *p;
        if ( *p == 104 )
        {
          f_float = 0;
        }
        else if ( v3 > 104 )
        {
          if ( v3 == 119 )
          {
            f_xyzw = 0;
          }
          else
          {
            if ( v3 != 120 )
              return ds_error("Usage: dr [-<cpuid>] [-[hfxw]*] [<reg>]...");
            f_xyzw = 1;
          }
        }
        else
        {
          if ( v3 != 102 )
            return ds_error("Usage: dr [-<cpuid>] [-[hfxw]*] [<reg>]...");
          f_float = 1;
        }
      }
    }
    --aca;
  }
  ds_bzero(masks, sizeof(masks));
  if ( aca > 0 || cpuid )
  {
    if ( aca <= 0 )
    {
      if ( cpuid == 1 )
      {
        kind = 8;
        v85 = (quad *)32;
        masks[8] = -1;
        n = 32;
        wz = 0;
      }
      else
      {
        kind = 10;
        v85 = (quad *)32;
        masks[10] = -1;
        n = 32;
        wz = 1;
      }
      if ( load_quad_registers(masks, pv, n) )
        return -1;
      ds_printf(" $vu%dvi00-07 =", wz);
      for ( i = 0; i <= 7; ++i )
      {
        v67 = ds_cqw(pv[i]);
        if ( i == 4 )
          v14 = " ";
        else
          v14 = "";
        ds_printf(" %s0x%H", v14, v67);
      }
      ds_printf("\n");
      ds_printf(" $vu%dvi08-15 =", wz);
      for ( i = 8; i <= 15; ++i )
      {
        v68 = ds_cqw(pv[i]);
        if ( i == 12 )
          v15 = " ";
        else
          v15 = "";
        ds_printf(" %s0x%H", v15, v68);
      }
      ds_printf("\n");
      v16 = ds_cqw(pv[26]);
      print_vuc(0, kind, 26, v16);
      ds_printf("\n");
      v17 = ds_cqw(pv[28]);
      print_vuc(0, kind, 28, v17);
      ds_printf("\n");
      v18 = ds_cqw(pv[29]);
      print_vuc(0, kind, 29, v18);
      ds_printf("\n");
      if ( cpuid == 1 )
        dr0_default_di();
      else
        dr1_default_di();
      return 0;
    }
    n = 0;
    while ( aca > 0 )
    {
      r = set_regmasks(masks, *ava);
      if ( r <= 0 )
        return -1;
      n += r;
      --aca;
      ++ava;
    }
    if ( load_quad_registers(masks, pv, n) )
      return -1;
    i = 0;
    for ( reg = ee_regs; ; ++reg )
    {
      if ( !reg->name )
        return 0;
      if ( (masks[reg->kind] & (1 << reg->number)) != 0 )
        break;
LABEL_117:
      ;
    }
    if ( reg->kind == 2 )
    {
      if ( reg->number == 12 )
      {
        ds_printf(" ");
        v46 = *pv++;
        v19 = ds_cqw(v46);
        print_rsr(v19);
        ds_printf("\n");
        goto LABEL_117;
      }
      if ( reg->number == 13 )
      {
        ds_printf(" ");
        v47 = *pv++;
        v20 = ds_cqw(v47);
        print_rcr(v20);
        ds_printf("\n");
        goto LABEL_117;
      }
    }
    else if ( reg->kind == 4 )
    {
      if ( !reg->number )
      {
        ds_printf(" ");
        v48 = *pv++;
        v21 = ds_cqw(v48);
        print_bpc(v21);
        ds_printf("\n");
        goto LABEL_117;
      }
    }
    else
    {
      if ( reg->kind == 5 && f_float )
      {
        v49 = *pv++;
        {
          unsigned int tmp;

          tmp = ds_cqw(v49);
          fv = *((float*)&tmp);
        }
        ds_printf(" $%-8s= %+g\n", reg->name, fv);
        goto LABEL_117;
      }
      if ( reg->kind == 7 || reg->kind == 9 )
      {
        src = *pv++;
        ds_cwq(&x, 0x20u);
        if ( f_float )
        {
          {
            unsigned int tmp;

            tmp = ds_cqw(src);
            v69 = *((float*)&tmp);
          }
          ds_qshr(&src, src, x);
          {
            unsigned int tmp;

            tmp = ds_cqw(src);
            v70 = *((float*)&tmp);
          }
          ds_qshr(&src, src, x);
          {
            unsigned int tmp;

            tmp = ds_cqw(src);
            v71 = *((float*)&tmp);
          }
          ds_qshr(&src, src, x);
          {
            unsigned int tmp;

            tmp = ds_cqw(src);
            v72 = *((float*)&tmp);
          }
          if ( f_xyzw )
          {
            ds_printf(" $%-8s (xyzw) = ", reg->name);
            ds_printf(" %+13g", v69);
            ds_printf(" %+13g", v70);
            ds_printf(" %+13g", v71);
            ds_printf(" %+13g", v72);
          }
          else
          {
            ds_printf(" $%-8s (wzyx) = ", reg->name);
            ds_printf(" %+13g", v72);
            ds_printf(" %+13g", v71);
            ds_printf(" %+13g", v70);
            ds_printf(" %+13g", v69);
          }
        }
        else
        {
          hilo = (quad *)ds_cqw(src);
          ds_qshr(&src, src, x);
          v85 = (quad *)ds_cqw(src);
          ds_qshr(&src, src, x);
          wz = ds_cqw(src);
          ds_qshr(&src, src, x);
          kind = ds_cqw(src);
          if ( f_xyzw )
          {
            ds_printf(" $%-8s (xyzw) = ", reg->name);
            ds_printf("    0x%W", hilo);
            ds_printf("    0x%W", v85);
            ds_printf("    0x%W", wz);
            ds_printf("    0x%W", kind);
          }
          else
          {
            ds_printf(" $%-8s (wzyx) = ", reg->name);
            ds_printf("    0x%W", kind);
            ds_printf("    0x%W", wz);
            ds_printf("    0x%W", v85);
            ds_printf("    0x%W", hilo);
          }
        }
LABEL_116:
        ds_printf("\n");
        goto LABEL_117;
      }
      if ( (reg->kind == 8 || reg->kind == 10) && reg->number > 15 )
      {
        v50 = *pv++;
        v22 = ds_cqw(v50);
        print_vuc(reg->name, reg->kind, reg->number, v22);
        ds_printf("\n");
        goto LABEL_117;
      }
    }
    ds_printf(" $%-8s= ", reg->name);
    size = reg->size;
    if ( size == 4 )
    {
      v52 = *pv++;
      v25 = ds_cqw(v52);
      ds_printf("0x%W", v25);
    }
    else if ( size > 4 )
    {
      if ( size == 8 )
      {
        v35.xa[3] = pv->xa[2];
        *(_QWORD *)&v35.xa[1] = *(_QWORD *)pv->xa;
        ++pv;
        v35.xa[0] = (unsigned int)&x;
        ds_cqt(v35);
        ds_printf("0x%T", x.xa[0], x.xa[1]);
      }
      else if ( size == 16 )
      {
        v53 = *pv++;
        ds_printf("0x%Q", v53.xa[0], v53.xa[1], v53.xa[2], v53.xa[3]);
      }
    }
    else if ( size == 2 )
    {
      v51 = *pv++;
      v24 = ds_cqw(v51);
      ds_printf("0x%H", v24);
    }
    goto LABEL_116;
  }
  fmt = dr_format_str();
  if ( fmt && *fmt )
  {
    if ( dr_default(fmt, masks, vals) )
    {
      return -1;
    }
    else
    {
      dr_default_di();
      return 0;
    }
  }
  else
  {
    masks[0] = -1;
    kind = 5;
    masks[1] = 31;
    masks[2] = 1082159360;
    n = 43;
    if ( load_quad_registers(masks, pv, 43) )
    {
      return -1;
    }
    else
    {
      v85 = pv;
      hilo = pv + 32;
      pv += 37;
      v36 = *pv++;
      v84 = ds_cqw(v36);
      v37 = *pv++;
      rsr = ds_cqw(v37);
      v38 = *pv++;
      rcr = ds_cqw(v38);
      v39 = *pv++;
      repc = ds_cqw(v39);
      v40 = *pv++;
      v80 = ds_cqw(v40);
      v41 = *pv++;
      rerpc = ds_cqw(v41);
      if ( (rsr & 4) != 0 )
      {
        v4 = rerpc;
        if ( (rcr & 0x40000000) != 0 )
          v4 = rerpc + 4;
        wz = v4;
      }
      else
      {
        v5 = repc;
        if ( (rcr & 0x80000000) != 0 )
          v5 = repc + 4;
        wz = v5;
      }
      v62 = ds_cqw(v85[7]);
      v58 = ds_cqw(v85[6]);
      v54 = ds_cqw(v85[5]);
      v42 = ds_cqw(v85[4]);
      v31 = ds_cqw(v85[3]);
      v28 = ds_cqw(v85[2]);
      v6 = ds_cqw(v85[1]);
      ds_printf(" at=%W  v0-1=%W,%W  a0-3=%W,%W,%W,%W\n", v6, v28, v31, v42, v54, v58, v62);
      v63 = ds_cqw(v85[15]);
      v59 = ds_cqw(v85[14]);
      v55 = ds_cqw(v85[13]);
      v43 = ds_cqw(v85[12]);
      v32 = ds_cqw(v85[11]);
      v29 = ds_cqw(v85[10]);
      v26 = ds_cqw(v85[9]);
      v7 = ds_cqw(v85[8]);
      ds_printf(" t0-7=%W,%W,%W,%W, %W,%W,%W,%W\n", v7, v26, v29, v32, v43, v55, v59, v63);
      v64 = ds_cqw(v85[23]);
      v60 = ds_cqw(v85[22]);
      v56 = ds_cqw(v85[21]);
      v44 = ds_cqw(v85[20]);
      v33 = ds_cqw(v85[19]);
      v30 = ds_cqw(v85[18]);
      v27 = ds_cqw(v85[17]);
      v8 = ds_cqw(v85[16]);
      ds_printf(" s0-7=%W,%W,%W,%W, %W,%W,%W,%W\n", v8, v27, v30, v33, v44, v56, v60, v64);
      v65 = ds_cqw(v85[29]);
      v61 = ds_cqw(v85[28]);
      v57 = ds_cqw(v85[27]);
      v45 = ds_cqw(v85[26]);
      v34 = ds_cqw(v85[25]);
      v9 = ds_cqw(v85[24]);
      ds_printf(" t8=%W t9=%W   k0=%W k1=%W   gp=%W sp=%W\n", v9, v34, v45, v57, v61, v65);
      v66 = ds_cqw(v85[31]);
      v10 = ds_cqw(v85[30]);
      ds_printf(" fp=%W ra=%W", v10, v66);
      v11 = ds_cqw(hilo[1]);
      ds_printf("   lo=%W", v11);
      v12 = ds_cqw(*hilo);
      ds_printf(" hi=%W", v12);
      v13 = ds_cqw(hilo[4]);
      ds_printf("   sa=%W", v13);
      ds_printf(" PC=%W", wz);
      ds_printf("\n");
      ds_printf(" badvaddr=%W badpaddr=%W\n", v84, v80);
      ds_printf(" ");
      print_rcr(rcr);
      ds_printf("\n");
      ds_printf(" ");
      print_rsr(rsr);
      ds_printf("\n");
      dr_default_di();
      return 0;
    }
  }
}

int sreg_cmd(int ac, char **av)
{
  int f_force; // [esp+0h] [ebp-14h]
  quad val; // [esp+4h] [ebp-10h] BYREF
  int aca; // [esp+1Ch] [ebp+8h]
  char **ava; // [esp+20h] [ebp+Ch]

  f_force = 0;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  for ( ava = av + 1; aca > 0 && **ava == 45; ++ava )
  {
    if ( strcmp("-f", *ava) )
      return ds_error("Usage: sr [-f] [<reg> <val>]...");
    ++f_force;
    --aca;
  }
  if ( aca <= 0 || (aca & 1) != 0 )
    return ds_error("Usage: sr [-f] [<reg> <val>]...");
  if ( !f_force && is_target_is_running() )
    return ds_error("target is not stopped state");
  while ( aca > 0 )
  {
    if ( ds_eval_quad(ava[1], &val) )
      return -1;
    if ( ds_store_quad_reg(*ava, val) )
      return -1;
    aca -= 2;
    ava += 2;
  }
  return 0;
}

int hbp_cmd(int ac, char **av)
{
  char *v3; // eax
  char *v4; // eax
  char *v5; // eax
  char *v6; // eax
  char *v7; // eax
  char *v8; // eax
  char *v9; // eax
  char *v10; // eax
  char *v11; // eax
  char *v12; // eax
  char *v13; // eax
  char *v14; // eax
  char *v15; // eax
  char *v16; // eax
  char *v17; // eax
  char *v18; // eax
  char v19; // dl
  char v20; // dl
  char *v21; // [esp-1Ch] [ebp-1568h]
  char *v22; // [esp-1Ch] [ebp-1568h]
  char *v23; // [esp-18h] [ebp-1564h]
  char *v24; // [esp-18h] [ebp-1564h]
  char *v25; // [esp-14h] [ebp-1560h]
  char *v26; // [esp-14h] [ebp-1560h]
  char *v27; // [esp-10h] [ebp-155Ch]
  char *v28; // [esp-10h] [ebp-155Ch]
  char *v29; // [esp-Ch] [ebp-1558h]
  char *v30; // [esp-Ch] [ebp-1558h]
  quad v31; // [esp+4h] [ebp-1548h] BYREF
  quad v32; // [esp+14h] [ebp-1538h] BYREF
  quad v33; // [esp+24h] [ebp-1528h] BYREF
  quad v34; // [esp+34h] [ebp-1518h] BYREF
  quad v35; // [esp+44h] [ebp-1508h] BYREF
  quad v36; // [esp+54h] [ebp-14F8h] BYREF
  quad v37; // [esp+64h] [ebp-14E8h] BYREF
  int f_debug; // [esp+74h] [ebp-14D8h]
  int hub; // [esp+78h] [ebp-14D4h]
  unsigned int vmsk; // [esp+7Ch] [ebp-14D0h] BYREF
  unsigned int val; // [esp+80h] [ebp-14CCh] BYREF
  unsigned int amsk; // [esp+84h] [ebp-14C8h] BYREF
  unsigned int adr; // [esp+88h] [ebp-14C4h] BYREF
  char *q; // [esp+8Ch] [ebp-14C0h]
  char *p; // [esp+90h] [ebp-14BCh]
  char vmsks[1024]; // [esp+94h] [ebp-14B8h] BYREF
  char vals[1024]; // [esp+494h] [ebp-10B8h] BYREF
  char amsks[1024]; // [esp+894h] [ebp-CB8h] BYREF
  char adrs[1024]; // [esp+C94h] [ebp-8B8h] BYREF
  char type[1024]; // [esp+1094h] [ebp-4B8h] BYREF
  quad regs[7]; // [esp+1494h] [ebp-B8h] BYREF
  unsigned int dvbm; // [esp+1504h] [ebp-48h]
  unsigned int dvb; // [esp+1508h] [ebp-44h]
  unsigned int dabm; // [esp+150Ch] [ebp-40h]
  unsigned int dab; // [esp+1510h] [ebp-3Ch]
  unsigned int iabm; // [esp+1514h] [ebp-38h]
  unsigned int iab; // [esp+1518h] [ebp-34h]
  unsigned int bpc; // [esp+151Ch] [ebp-30h]
  unsigned int masks[11]; // [esp+1520h] [ebp-2Ch] BYREF
  int aca; // [esp+1554h] [ebp+8h]
  char **ava; // [esp+1558h] [ebp+Ch]

  hub = 0;
  f_debug = 0;
  if ( ac <= 0 )
    return 0;
  if ( !strcmp("hub", *av) )
    ++hub;
  aca = ac - 1;
  for ( ava = av + 1; aca > 0 && **ava == 45; ++ava )
  {
    if ( strcmp("-debug", *ava) )
    {
LABEL_173:
      if ( hub )
        return ds_error("Usage: hub [{pc|da|dr|dw}]...");
      else
        return ds_error("Usage: hbp [{pc}[uskxnt][:<adr>[,<amsk>]]]...\n"
                        "   or: hbp [{da|dr|dw}[uskxnt][:<adr>[,<amsk>][=<val>[,<vmsk>]]]]...");
    }
    ++f_debug;
    --aca;
  }
  ds_bzero(masks, sizeof(masks));
  masks[4] = 253;
  if ( load_quad_registers(masks, regs, 7) )
    return -1;
  bpc = ds_cqw(regs[0]);
  iab = ds_cqw(regs[1]);
  iabm = ds_cqw(regs[2]);
  dab = ds_cqw(regs[3]);
  dabm = ds_cqw(regs[4]);
  dvb = ds_cqw(regs[5]);
  dvbm = ds_cqw(regs[6]);
  if ( f_debug )
  {
    ds_printf("load:\n");
    print_bpc(bpc);
    ds_printf("\n");
    ds_printf("$iab =0x%W $iabm=0x%W\n", iab, iabm);
    ds_printf("$dab =0x%W $dabm=0x%W\n", dab, dabm);
    ds_printf("$dvb =0x%W $dvbm=0x%W\n", dvb, dvbm);
  }
  if ( aca )
  {
    bpc &= 0xFFFF7FF8;
    while ( aca > 0 )
    {
      p = *ava;
      if ( *p == 45 )
        goto LABEL_173;
      q = type;
      while ( *p && *p != 58 )
        *q++ = *p++;
      *q = 0;
      v15 = p;
      if ( *p == 58 )
        v15 = p + 1;
      p = v15;
      q = adrs;
      while ( *p && *p != 44 && *p != 61 )
        *q++ = *p++;
      *q = 0;
      v16 = p;
      if ( *p == 44 )
        v16 = p + 1;
      p = v16;
      q = amsks;
      while ( *p && *p != 61 )
        *q++ = *p++;
      *q = 0;
      v17 = p;
      if ( *p == 61 )
        v17 = p + 1;
      p = v17;
      q = vals;
      while ( *p && *p != 44 )
        *q++ = *p++;
      *q = 0;
      v18 = p;
      if ( *p == 44 )
        v18 = p + 1;
      p = v18;
      q = vmsks;
      while ( *p )
        *q++ = *p++;
      *q = 0;
      if ( hub )
      {
        if ( !ds_strncmp("pc", type, 2) )
        {
          bpc &= ~0x80000000;
          goto LABEL_168;
        }
        if ( type[0] == 100 )
        {
          bpc &= 0x9FFFFFFF;
          goto LABEL_168;
        }
        goto LABEL_173;
      }
      if ( ds_strncmp("pc", type, 2) )
      {
        if ( type[0] != 100 || !type[1] )
          goto LABEL_173;
        bpc &= ~0x10000000u;
        bpc |= 0x60000000u;
        switch ( type[1] )
        {
          case 'r':
            bpc &= ~0x20000000u;
            break;
          case 'w':
            bpc &= ~0x40000000u;
            break;
          case 'a':
            break;
          default:
            goto LABEL_173;
        }
        adr = dab;
        amsk = dabm;
        if ( adrs[0] )
        {
          if ( ds_eval_word(adrs, &adr) )
            return -1;
          amsk = -1;
          if ( amsks[0] )
          {
            if ( ds_eval_word(amsks, &amsk) )
              return -1;
          }
        }
        dab = adr;
        dabm = amsk;
        if ( vals[0] )
        {
          if ( ds_eval_word(vals, &val) )
            return -1;
          vmsk = -1;
          if ( vmsks[0] && ds_eval_word(vmsks, &vmsk) )
            return -1;
          bpc |= 0x10000000u;
          dvb = val;
          dvbm = vmsk;
        }
        p = &type[2];
        if ( type[2] )
        {
          bpc &= 0xFFC27FFF;
          while ( 2 )
          {
            if ( *p )
            {
              v20 = *p++ - 107;
              switch ( v20 )
              {
                case 0:
                  bpc |= 0x80000u;
                  continue;
                case 3:
                  bpc |= 0x8000u;
                  continue;
                case 8:
                  bpc |= 0x100000u;
                  continue;
                case 9:
                  bpc |= 0x10000u;
                  continue;
                case 10:
                  bpc |= 0x200000u;
                  continue;
                case 13:
                  bpc |= 0x40000u;
                  continue;
                default:
                  goto LABEL_173;
              }
            }
            break;
          }
        }
      }
      else
      {
        bpc |= 0x80000000;
        adr = iab;
        amsk = iabm;
        if ( adrs[0] )
        {
          if ( ds_eval_word(adrs, &adr) )
            return -1;
          amsk = -1;
          if ( amsks[0] )
          {
            if ( ds_eval_word(amsks, &amsk) )
              return -1;
          }
        }
        iab = adr;
        iabm = amsk;
        if ( vals[0] || vmsks[0] )
          goto LABEL_173;
        p = &type[2];
        if ( type[2] )
        {
          bpc &= 0xF87D7FFF;
          while ( 2 )
          {
            if ( *p )
            {
              v19 = *p++ - 107;
              switch ( v19 )
              {
                case 0:
                  bpc |= 0x1000000u;
                  continue;
                case 3:
                  bpc |= 0x8000u;
                  continue;
                case 8:
                  bpc |= 0x2000000u;
                  continue;
                case 9:
                  bpc |= 0x20000u;
                  continue;
                case 10:
                  bpc |= 0x4000000u;
                  continue;
                case 13:
                  bpc |= 0x800000u;
                  continue;
                default:
                  goto LABEL_173;
              }
            }
            break;
          }
        }
      }
      --aca;
      ++ava;
    }
    goto LABEL_168;
  }
  if ( hub )
  {
    bpc &= 0xFFFFFFFu;
LABEL_168:
    if ( f_debug )
    {
      ds_printf("store regs:\n");
      print_bpc(bpc);
      ds_printf("\n");
      ds_printf("$iab =0x%W $iabm=0x%W\n", iab, iabm);
      ds_printf("$dab =0x%W $dabm=0x%W\n", dab, dabm);
      ds_printf("$dvb =0x%W $dvbm=0x%W\n", dvb, dvbm);
    }
    ds_cwq(&v37, bpc);
    regs[0] = v37;
    ds_cwq(&v36, iab);
    regs[1] = v36;
    ds_cwq(&v35, iabm);
    regs[2] = v35;
    ds_cwq(&v34, dab);
    regs[3] = v34;
    ds_cwq(&v33, dabm);
    regs[4] = v33;
    ds_cwq(&v32, dvb);
    regs[5] = v32;
    ds_cwq(&v31, dvbm);
    regs[6] = v31;
    if ( store_quad_registers(masks, regs, 7) )
      return -1;
    else
      return 0;
  }
  if ( (bpc & 0x8000u) != 0 )
    ds_printf("Breakpoint Exception Disabled\n");
  if ( (bpc & 0xE0000000) == 0 )
    return 0;
  if ( (bpc & 0x80000000) != 0 )
  {
    if ( (bpc & 0x20000) != 0 )
      v3 = "t";
    else
      v3 = "";
    v29 = v3;
    if ( (bpc & 0x8000u) == 0 )
      v4 = "";
    else
      v4 = "n";
    v27 = v4;
    if ( (bpc & 0x800000) != 0 )
      v5 = "x";
    else
      v5 = "";
    v25 = v5;
    if ( (bpc & 0x1000000) != 0 )
      v6 = "k";
    else
      v6 = "";
    v23 = v6;
    if ( (bpc & 0x2000000) != 0 )
      v7 = "s";
    else
      v7 = "";
    v21 = v7;
    if ( (bpc & 0x4000000) != 0 )
      v8 = "u";
    else
      v8 = "";
    ds_printf(" pc%s%s%s%s%s%s:%W,%W", v8, v21, v23, v25, v27, v29, iab, iabm);
  }
  if ( (bpc & 0x60000000) != 0 )
  {
    ds_printf(" d");
    switch ( bpc & 0x60000000 )
    {
      case 0x60000000u:
        ds_printf("a");
        break;
      case 0x40000000u:
        ds_printf("r");
        break;
      case 0x20000000u:
        ds_printf("w");
        break;
    }
    if ( (bpc & 0x10000) != 0 )
      v9 = "t";
    else
      v9 = "";
    v30 = v9;
    if ( (bpc & 0x8000u) == 0 )
      v10 = "";
    else
      v10 = "n";
    v28 = v10;
    if ( (bpc & 0x40000) != 0 )
      v11 = "x";
    else
      v11 = "";
    v26 = v11;
    if ( (bpc & 0x80000) != 0 )
      v12 = "k";
    else
      v12 = "";
    v24 = v12;
    if ( (bpc & 0x100000) != 0 )
      v13 = "s";
    else
      v13 = "";
    v22 = v13;
    if ( (bpc & 0x200000) != 0 )
      v14 = "u";
    else
      v14 = "";
    ds_printf("%s%s%s%s%s%s:%W,%W", v14, v22, v24, v26, v28, v30, dab, dabm);
    if ( (bpc & 0x10000000) != 0 )
      ds_printf("=%W,%W", dvb, dvbm);
  }
  ds_printf("\n");
  return 0;
}

int rload_cmd(int ac, char **av)
{
  int v3; // eax
  char *v4; // eax
  char *v5; // eax
  char *v6; // eax
  quad *v7; // eax
  quad *v8; // edx
  char _c; // [esp+Bh] [ebp-3491h]
  int n_3; // [esp+Ch] [ebp-3490h]
  int n_1; // [esp+Ch] [ebp-3490h]
  int siz; // [esp+10h] [ebp-348Ch] BYREF
  int r; // [esp+14h] [ebp-3488h]
  EE_REGS *reg; // [esp+18h] [ebp-3484h]
  quad val; // [esp+1Ch] [ebp-3480h] BYREF
  quad tvals[352]; // [esp+2Ch] [ebp-3470h] BYREF
  quad avals[352]; // [esp+162Ch] [ebp-1E70h] BYREF
  unsigned int tmasks[11]; // [esp+2C2Ch] [ebp-870h] BYREF
  unsigned int amasks[11]; // [esp+2C58h] [ebp-844h] BYREF
  void *stream; // [esp+2C84h] [ebp-818h]
  char vstr[1024]; // [esp+2C88h] [ebp-814h] BYREF
  char rstr[1024]; // [esp+3088h] [ebp-414h] BYREF
  char *q; // [esp+3488h] [ebp-14h]
  char *pe; // [esp+348Ch] [ebp-10h]
  char *p; // [esp+3490h] [ebp-Ch]
  char *buf; // [esp+3494h] [ebp-8h]
  char *fname; // [esp+3498h] [ebp-4h]
  int aca; // [esp+34A4h] [ebp+8h]
  char **ava; // [esp+34A8h] [ebp+Ch]

  r = -1;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  ava = av + 1;
  if ( aca > 0 && **ava == 45 )
    return ds_error("Usage: rload <fname>");
  fname = *ava;
  if ( aca != 1 )
    return ds_error("Usage: rload <fname>");
  if ( ds_fsize(fname, &siz) < 0 )
    return -1;
  stream = ds_fopen(fname, "r");
  if ( !stream )
    return -1;
  buf = (char *)ds_fload(stream, 0, 0, 1, siz);
  ds_fclose(stream);
  if ( !buf )
    return -1;
  ds_bzero(amasks, sizeof(amasks));
  p = buf;
  pe = &buf[siz];
  while ( pe > p )
  {
    while ( pe > p )
    {
      _c = *p;
      v3 = 0;
      if ( *p == 32 || _c == 12 || _c == 10 || _c == 13 || _c == 9 || _c == 11 )
        v3 = 1;
      if ( !v3 )
        break;
      ++p;
    }
    if ( *p == 10 || *p == 35 || *p == 59 || *p == 47 && p[1] == 47 )
    {
      while ( pe > p && *p != 10 )
        ++p;
      v4 = p;
      if ( *p == 10 )
        v4 = p + 1;
      p = v4;
    }
    else
    {
      q = rstr;
      while ( pe > p && *p != 10 && *p != 61 )
        *q++ = *p++;
      *q = 0;
      v5 = p;
      if ( pe > p && *p == 61 )
        v5 = p + 1;
      p = v5;
      q = vstr;
      while ( *p != 10 )
        *q++ = *p++;
      *q = 0;
      v6 = p;
      if ( pe > p && *p == 10 )
        v6 = p + 1;
      p = v6;
      ds_bzero(tmasks, sizeof(tmasks));
      if ( set_regmasks(tmasks, rstr) <= 0 || ds_eval_quad(vstr, &val) )
        goto LABEL_72;
      for ( reg = ee_regs; reg->name; ++reg )
      {
        if ( (tmasks[reg->kind] & (1 << reg->number)) != 0 )
          avals[32 * reg->kind + reg->number] = val;
      }
      for ( n_3 = 0; n_3 <= 10; ++n_3 )
        amasks[n_3] |= tmasks[n_3];
    }
  }
  n_1 = 0;
  for ( reg = ee_regs; reg->name; ++reg )
  {
    if ( (amasks[reg->kind] & (1 << reg->number)) != 0 )
    {
      v7 = &tvals[n_1];
      v8 = &avals[32 * reg->kind + reg->number];
      v7->xa[0] = v8->xa[0];
      v7->xa[1] = v8->xa[1];
      v7->xa[2] = v8->xa[2];
      v7->xa[3] = v8->xa[3];
      ++n_1;
    }
  }
  if ( !store_quad_registers(amasks, tvals, n_1) )
    r = 0;
LABEL_72:
  ds_free(buf);
  return r;
}

int rsave_cmd(int ac, char **av)
{
  int v3; // eax
  int quad_registers; // eax
  quad *v5; // eax
  int v6; // eax
  void *v7; // [esp-4h] [ebp-1A4Ch]
  int n; // [esp+8h] [ebp-1A40h]
  int r; // [esp+Ch] [ebp-1A3Ch]
  int r_1; // [esp+Ch] [ebp-1A3Ch]
  EE_REGS *reg; // [esp+10h] [ebp-1A38h]
  quad vals[352]; // [esp+14h] [ebp-1A34h] BYREF
  unsigned int masks[11]; // [esp+1614h] [ebp-434h] BYREF
  void *stream; // [esp+1640h] [ebp-408h]
  char buf[1024]; // [esp+1644h] [ebp-404h] BYREF
  char *fname; // [esp+1A44h] [ebp-4h]
  int aca; // [esp+1A50h] [ebp+8h]
  int acb; // [esp+1A50h] [ebp+8h]
  char **ava; // [esp+1A54h] [ebp+Ch]
  char **avb; // [esp+1A54h] [ebp+Ch]

  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  ava = av + 1;
  if ( aca > 0 && **ava == 45 )
    return ds_error("Usage: rsave <fname> [<reg>]...");
  fname = *ava;
  avb = ava + 1;
  v3 = aca;
  acb = aca - 1;
  if ( v3 <= 0 )
    return ds_error("Usage: rsave <fname> [<reg>]...");
  ds_bzero(masks, sizeof(masks));
  if ( acb > 0 )
  {
    n = 0;
    while ( acb > 0 )
    {
      r = set_regmasks(masks, *avb);
      if ( r <= 0 )
        return -1;
      --acb;
      ++avb;
      n += r;
    }
    quad_registers = load_quad_registers(masks, vals, n);
  }
  else
  {
    masks[0] = -1;
    masks[1] = 31;
    masks[2] = 1073770496;
    quad_registers = load_quad_registers(masks, vals, 41);
  }
  if ( quad_registers )
    return -1;
  stream = ds_fopen(fname, "w");
  if ( !stream )
    return -1;
  r_1 = 0;
  for ( reg = ee_regs; reg->name; ++reg )
  {
    if ( (masks[reg->kind] & (1 << reg->number)) != 0 )
    {
      v5 = &vals[r_1++];
      ds_sprintf(buf, "$%s=0x%Q\n", reg->name, v5->xa[0], v5->xa[1], v5->xa[2], v5->xa[3]);
      v7 = stream;
      v6 = strlen(buf);
      if ( ds_fwrite(buf, v6, 1, v7) != 1 )
        break;
    }
  }
  ds_fclose(stream);
  return 0;
}

