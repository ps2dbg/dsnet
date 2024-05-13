
#include "dsxdb_prototypes.h"

extern unsigned int dot; // defined in mem.c

static const char *gpr_name[32] = {
    "zero",
    "at",
    "v0",
    "v1",
    "a0",
    "a1",
    "a2",
    "a3",
    "t0",
    "t1",
    "t2",
    "t3",
    "t4",
    "t5",
    "t6",
    "t7",
    "s0",
    "s1",
    "s2",
    "s3",
    "s4",
    "s5",
    "s6",
    "s7",
    "t8",
    "t9",
    "k0",
    "k1",
    "gp",
    "sp",
    "fp",
    "ra",
};

static const char *cpr_name[4][32] =
    {
        {
#ifdef DSNET_COMPILING_E
            "index",
            "random",
            "entrylo0",
            "entrylo1",
            "context",
            "pagemask",
            "wired",
            "rsvd7",
            "badvaddr",
            "count",
            "entryhi",
            "compare",
            "status",
            "cause",
            "epc",
            "prid",
            "config",
            "rsvd17",
            "rsvd18",
            "rsvd19",
            "rsvd20",
            "rsvd21",
            "rsvd22",
            "badpaddr",
            "debug",
            "perf",
            "rsvd26",
            "rsvd27",
            "taglo",
            "taghi",
            "errorepc",
            "rsvd31",
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
            "0",
            "1",
            "2",
            "bpc",
            "4",
            "bda",
            "tar",
            "dcic",
            "bada",
            "bdam",
            "10",
            "bpcm",
            "sr",
            "cr",
            "epc",
            "prid",
            "16",
            "17",
            "18",
            "19",
            "20",
            "21",
            "22",
            "23",
            "24",
            "25",
            "26",
            "27",
            "28",
            "29",
            "30",
            "31",
#endif /* DSNET_COMPILING_I */
        },
        {
#ifdef DSNET_COMPILING_E
            "fpr0",
            "fpr1",
            "fpr2",
            "fpr3",
            "fpr4",
            "fpr5",
            "fpr6",
            "fpr7",
            "fpr8",
            "fpr9",
            "fpr10",
            "fpr11",
            "fpr12",
            "fpr13",
            "fpr14",
            "fpr15",
            "fpr16",
            "fpr17",
            "fpr18",
            "fpr19",
            "fpr20",
            "fpr21",
            "fpr22",
            "fpr23",
            "fpr24",
            "fpr25",
            "fpr26",
            "fpr27",
            "fpr28",
            "fpr29",
            "fpr30",
            "fpr31",
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
            "0",
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9",
            "10",
            "11",
            "12",
            "13",
            "14",
            "15",
            "16",
            "17",
            "18",
            "19",
            "20",
            "21",
            "22",
            "23",
            "24",
            "25",
            "26",
            "27",
            "28",
            "29",
            "30",
            "31",
#endif /* DSNET_COMPILING_I */
        },
        {
#ifdef DSNET_COMPILING_E
            "vf00",
            "vf01",
            "vf02",
            "vf03",
            "vf04",
            "vf05",
            "vf06",
            "vf07",
            "vf08",
            "vf09",
            "vf10",
            "vf11",
            "vf12",
            "vf13",
            "vf14",
            "vf15",
            "vf16",
            "vf17",
            "vf18",
            "vf19",
            "vf20",
            "vf21",
            "vf22",
            "vf23",
            "vf24",
            "vf25",
            "vf26",
            "vf27",
            "vf28",
            "vf29",
            "vf30",
            "vf31",
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
            "0",
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9",
            "10",
            "11",
            "12",
            "13",
            "14",
            "15",
            "16",
            "17",
            "18",
            "19",
            "20",
            "21",
            "22",
            "23",
            "24",
            "25",
            "26",
            "27",
            "28",
            "29",
            "30",
            "31",
#endif /* DSNET_COMPILING_I */
        },
        {
            "0",
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9",
            "10",
            "11",
            "12",
            "13",
            "14",
            "15",
            "16",
            "17",
            "18",
            "19",
            "20",
            "21",
            "22",
            "23",
            "24",
            "25",
            "26",
            "27",
            "28",
            "29",
            "30",
            "31",
        },
};

static const char *ccr_name[4][32] =
    {
        {
            "0",
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9",
            "10",
            "11",
            "12",
            "13",
            "14",
            "15",
            "16",
            "17",
            "18",
            "19",
            "20",
            "21",
            "22",
            "23",
            "24",
            "25",
            "26",
            "27",
            "28",
            "29",
            "30",
            "31",
        },
        {
            "0",
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9",
            "10",
            "11",
            "12",
            "13",
            "14",
            "15",
            "16",
            "17",
            "18",
            "19",
            "20",
            "21",
            "22",
            "23",
            "24",
            "25",
            "26",
            "27",
            "28",
            "29",
            "30",
            "31",
        },
        {
#ifdef DSNET_COMPILING_E
            "vi00",
            "vi01",
            "vi02",
            "vi03",
            "vi04",
            "vi05",
            "vi06",
            "vi07",
            "vi08",
            "vi09",
            "vi10",
            "vi11",
            "vi12",
            "vi13",
            "vi14",
            "vi15",
            "vi16",
            "vi17",
            "vi18",
            "vi19",
            "vi20",
            "vi21",
            "vi22",
            "vi23",
            "vi24",
            "vi25",
            "vi26",
            "vi27",
            "vi28",
            "vi29",
            "vi30",
            "vi31",
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
            "0",
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9",
            "10",
            "11",
            "12",
            "13",
            "14",
            "15",
            "16",
            "17",
            "18",
            "19",
            "20",
            "21",
            "22",
            "23",
            "24",
            "25",
            "26",
            "27",
            "28",
            "29",
            "30",
            "31",
#endif /* DSNET_COMPILING_I */
        },
        {
            "0",
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9",
            "10",
            "11",
            "12",
            "13",
            "14",
            "15",
            "16",
            "17",
            "18",
            "19",
            "20",
            "21",
            "22",
            "23",
            "24",
            "25",
            "26",
            "27",
            "28",
            "29",
            "30",
            "31",
        },
};

static const char *dest_name[] =
    {
        "    ",
        "w   ",
        "z   ",
        "wz  ",
        "y   ",
        "wy  ",
        "yz  ",
        "wyz ",
        "x   ",
        "wx  ",
        "xz  ",
        "wxz ",
        "xy  ",
        "wxy ",
        "xyz ",
        "wxyz",
};

static const char *xyzw_name[] =
    {"x", "y", "z", "w"};

static struct
{
    BT *head;
    BT *tail;
} bts = {NULL, NULL};
static int bt_no = 0;
OPCODE opcodes[] =
{
  { "NOP", 0xffffffff, 0x00000000, 1 },
  { "LI      rt,simm", 0xffe00000, 0x24000000, 0x1001 },
  { "LI      rt,imm", 0xffe00000, 0x34000000, 0x1001 },
#ifdef DSNET_COMPILING_E
  { "MOVE    rd,rt", 0xffe007ff, 0x0000002d, 0x1004 },
  { "MOVE    rd,rs", 0xfc1f07ff, 0x0000002d, 0x1004 },
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  { "MOVE    rd,rt", 0xffe007ff, 0x00000021, 0x1001 },
  { "MOVE    rd,rs", 0xfc1f07ff, 0x00000021, 0x1001 },
#endif /* DSNET_COMPILING_I */
  { "MOVE    rd,rt", 0xffe007ff, 0x00000025, 0x1001 },
  { "MOVE    rd,rs", 0xfc1f07ff, 0x00000025, 0x1001 },
  { "B       off", 0xffff0000, 0x10000000, 0x1001 },
  { "B       off", 0xffff0000, 0x04010000, 0x1001 },
  { "BAL     off", 0xffff0000, 0x04110000, 0x1001 },
  { "ADD     rd,rs,rt", 0xfc0007ff, 0x00000020, 0x1 },
  { "ADDI    rt,rs,simm", 0xfc000000, 0x20000000, 0x1 },
  { "ADDIU   rt,rs,simm", 0xfc000000, 0x24000000, 0x1 },
  { "ADDU    rd,rs,rt", 0xfc0007ff, 0x00000021, 0x1 },
  { "AND     rd,rs,rt", 0xfc0007ff, 0x00000024, 0x1 },
  { "ANDI    rt,rs,imm", 0xfc000000, 0x30000000, 0x1 },
#ifdef DSNET_COMPILING_I
  { "BC0F    off", 0xffff0000, 0x41000000, 0x1 },
  { "BC1F    off", 0xffff0000, 0x45000000, 0x1 },
  { "BC2F    off", 0xffff0000, 0x49000000, 0x1 },
  { "BC3F    off", 0xffff0000, 0x4d000000, 0x1 },
  { "BC0T    off", 0xffff0000, 0x41010000, 0x1 },
  { "BC1T    off", 0xffff0000, 0x45010000, 0x1 },
  { "BC2T    off", 0xffff0000, 0x49010000, 0x1 },
  { "BC3T    off", 0xffff0000, 0x4d010000, 0x1 },
#endif /* DSNET_COMPILING_I */
  { "BEQ     rs,rt,off", 0xfc000000, 0x10000000, 0x1 },
#ifdef DSNET_COMPILING_E
  { "BEQL    rs,rt,off", 0xfc000000, 0x50000000, 0x82 },
#endif /* DSNET_COMPILING_E */
  { "BGEZ    rs,off", 0xfc1f0000, 0x04010000, 0x1 },
  { "BGEZAL  rs,off", 0xfc1f0000, 0x04110000, 0x1 },
#ifdef DSNET_COMPILING_E
  { "BGEZALL rs,off", 0xfc1f0000, 0x04130000, 0x82 },
  { "BGEZL   rs,off", 0xfc1f0000, 0x04030000, 0x82 },
#endif /* DSNET_COMPILING_E */
  { "BGTZ    rs,off", 0xfc1f0000, 0x1c000000, 0x1 },
#ifdef DSNET_COMPILING_E
  { "BGTZL   rs,off", 0xfc1f0000, 0x5c000000, 0x82 },
#endif /* DSNET_COMPILING_E */
  { "BLEZ    rs,off", 0xfc1f0000, 0x18000000, 0x1 },
#ifdef DSNET_COMPILING_E
  { "BLEZL   rs,off", 0xfc1f0000, 0x58000000, 0x82 },
#endif /* DSNET_COMPILING_E */
  { "BLTZ    rs,off", 0xfc1f0000, 0x04000000, 0x1 },
  { "BLTZAL  rs,off", 0xfc1f0000, 0x04100000, 0x1 },
#ifdef DSNET_COMPILING_E
  { "BLTZALL rs,off", 0xfc1f0000, 0x04120000, 0x82 },
  { "BLTZL   rs,off", 0xfc1f0000, 0x04020000, 0x82 },
#endif /* DSNET_COMPILING_E */
  { "BNE     rs,rt,off", 0xfc000000, 0x14000000, 0x1 },
#ifdef DSNET_COMPILING_E
  { "BNEL    rs,rt,off", 0xfc000000, 0x54000000, 0x82 },
#endif /* DSNET_COMPILING_E */
  { "BREAK   code", 0xfc00003f, 0x0000000d, 0x1 },
#ifdef DSNET_COMPILING_E
  { "DADD    rd,rs,rt", 0xfc0007ff, 0x0000002c, 0x4 },
  { "DADDI   rt,rs,simm", 0xfc000000, 0x60000000, 0x4 },
  { "DADDIU  rt,rs,simm", 0xfc000000, 0x64000000, 0x4 },
  { "DADDU   rd,rs,rt", 0xfc0007ff, 0x0000002d, 0x4 },
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  { "RFE", 0xffffffff, 0x42000010, 0x81 },
#endif /* DSNET_COMPILING_I */
  { "DIV     rs,rt", 0xfc00ffff, 0x0000001a, 0x1 },
  { "DIVU    rs,rt", 0xfc00ffff, 0x0000001b, 0x1 },
#ifdef DSNET_COMPILING_E
  { "DSLL    rd,rt,shamt", 0xffe0003f, 0x00000038, 0x4 },
  { "DSLL32  rd,rt,shamt", 0xffe0003f, 0x0000003c, 0x4 },
  { "DSLLV   rd,rt,rs", 0xfc0007ff, 0x00000014, 0x4 },
  { "DSRA    rd,rt,shamt", 0xffe0003f, 0x0000003b, 0x4 },
  { "DSRA32  rd,rt,shamt", 0xffe0003f, 0x0000003f, 0x4 },
  { "DSRAV   rd,rt,rs", 0xfc0007ff, 0x00000017, 0x4 },
  { "DSRL    rd,rt,shamt", 0xffe0003f, 0x0000003a, 0x4 },
  { "DSRL32  rd,rt,shamt", 0xffe0003f, 0x0000003e, 0x4 },
  { "DSRLV   rd,rt,rs", 0xfc0007ff, 0x00000016, 0x4 },
  { "DSUB    rd,rs,rt", 0xfc0007ff, 0x0000002e, 0x4 },
  { "DSUBU   rd,rs,rt", 0xfc0007ff, 0x0000002f, 0x4 },
#endif /* DSNET_COMPILING_E */
  { "J       target", 0xfc000000, 0x08000000, 0x1 },
  { "JAL     target", 0xfc000000, 0x0c000000, 0x1 },
  { "JALR    rd,rs", 0xfc1f07ff, 0x00000009, 0x1 },
  { "JR      rs", 0xfc1fffff, 0x00000008, 0x1 },
  { "LB      rt,simm(rs)", 0xfc000000, 0x80000000, 0x1 },
  { "LBU     rt,simm(rs)", 0xfc000000, 0x90000000, 0x1 },
#ifdef DSNET_COMPILING_E
  { "LD      rt,simm(rs)", 0xfc000000, 0xdc000000, 0x4 },
  { "LDL     rt,simm(rs)", 0xfc000000, 0x68000000, 0x4 },
  { "LDR     rt,simm(rs)", 0xfc000000, 0x6c000000, 0x4 },
#endif /* DSNET_COMPILING_E */
  { "LH      rt,simm(rs)", 0xfc000000, 0x84000000, 0x1 },
  { "LHU     rt,simm(rs)", 0xfc000000, 0x94000000, 0x1 },
  { "LUI     rt,imm", 0xfc000000, 0x3c000000, 0x1 },
  { "LW      rt,simm(rs)", 0xfc000000, 0x8c000000, 0x1 },
  { "LWL     rt,simm(rs)", 0xfc000000, 0x88000000, 0x1 },
  { "LWR     rt,simm(rs)", 0xfc000000, 0x98000000, 0x1 },
#ifdef DSNET_COMPILING_E
  { "LWU     rt,simm(rs)", 0xfc000000, 0x9c000000, 0x4 },
#endif /* DSNET_COMPILING_E */
  { "MFHI    rd", 0xffff07ff, 0x00000010, 0x1 },
  { "MFLO    rd", 0xffff07ff, 0x00000012, 0x1 },
#ifdef DSNET_COMPILING_E
  { "MOVN    rd,rs,rt", 0xfc0007ff, 0x0000000b, 0x8 },
  { "MOVZ    rd,rs,rt", 0xfc0007ff, 0x0000000a, 0x8 },
#endif /* DSNET_COMPILING_E */
  { "MTHI    rs", 0xfc1fffff, 0x00000011, 0x1 },
  { "MTLO    rs", 0xfc1fffff, 0x00000013, 0x1 },
  { "MULT    rs,rt", 0xfc00ffff, 0x00000018, 0x1 },
  { "MULTU   rs,rt", 0xfc00ffff, 0x00000019, 0x1 },
  { "NOR     rd,rs,rt", 0xfc0007ff, 0x00000027, 0x1 },
  { "OR      rd,rs,rt", 0xfc0007ff, 0x00000025, 0x1 },
  { "ORI     rt,rs,imm", 0xfc000000, 0x34000000, 0x1 },
#ifdef DSNET_COMPILING_E
  { "PREF    hint,simm(rs)", 0xfc000000, 0xcc000000, 0x8 },
#endif /* DSNET_COMPILING_E */
  { "SB      rt,simm(rs)", 0xfc000000, 0xa0000000, 0x1 },
#ifdef DSNET_COMPILING_E
  { "SD      rt,simm(rs)", 0xfc000000, 0xfc000000, 0x4 },
  { "SDL     rt,simm(rs)", 0xfc000000, 0xb0000000, 0x4 },
  { "SDR     rt,simm(rs)", 0xfc000000, 0xb4000000, 0x4 },
#endif /* DSNET_COMPILING_E */
  { "SH      rt,simm(rs)", 0xfc000000, 0xa4000000, 0x1 },
  { "SLL     rd,rt,shamt", 0xffe0003f, 0x00000000, 0x1 },
  { "SLLV    rd,rt,rs", 0xfc0007ff, 0x00000004, 0x1 },
  { "SLT     rd,rs,rt", 0xfc0007ff, 0x0000002a, 0x1 },
  { "SLTI    rt,rs,simm", 0xfc000000, 0x28000000, 0x1 },
  { "SLTIU   rt,rs,simm", 0xfc000000, 0x2c000000, 0x1 },
  { "SLTU    rd,rs,rt", 0xfc0007ff, 0x0000002b, 0x1 },
  { "SRA     rd,rt,shamt", 0xffe0003f, 0x00000003, 0x1 },
  { "SRAV    rd,rt,rs", 0xfc0007ff, 0x00000007, 0x1 },
  { "SRL     rd,rt,shamt", 0xffe0003f, 0x00000002, 0x1 },
  { "SRLV    rd,rt,rs", 0xfc0007ff, 0x00000006, 0x1 },
  { "SUB     rd,rs,rt", 0xfc0007ff, 0x00000022, 0x1 },
  { "SUBU    rd,rs,rt", 0xfc0007ff, 0x00000023, 0x1 },
  { "SW      rt,simm(rs)", 0xfc000000, 0xac000000, 0x1 },
  { "SWL     rt,simm(rs)", 0xfc000000, 0xa8000000, 0x1 },
  { "SWR     rt,simm(rs)", 0xfc000000, 0xb8000000, 0x1 },
#ifdef DSNET_COMPILING_E
  { "SYNC", 0xffffffff, 0x0000000f, 0x182 },
  { "SYNC.L", 0xffffffff, 0x0000000f, 0x2 },
  { "SYNC.P", 0xffffffff, 0x0000040f, 0x2 },
#endif /* DSNET_COMPILING_E */
  { "SYSCALL code", 0xfc00003f, 0x0000000c, 0x1 },
#ifdef DSNET_COMPILING_E
  { "TEQ     rs,rt", 0xfc00003f, 0x00000034, 0x2 },
  { "TEQI    rs,simm", 0xfc1f0000, 0x040c0000, 0x2 },
  { "TGE     rs,rt", 0xfc00ffff, 0x00000030, 0x2 },
  { "TGEI    rs,simm", 0xfc1f0000, 0x04080000, 0x2 },
  { "TGEIU   rs,simm", 0xfc1f0000, 0x04090000, 0x2 },
  { "TGEU    rs,rt", 0xfc00ffff, 0x00000031, 0x2 },
  { "TLT     rs,rt", 0xfc00ffff, 0x00000032, 0x2 },
  { "TLTI    rs,simm", 0xfc1f0000, 0x040a0000, 0x2 },
  { "TLTIU   rs,simm", 0xfc1f0000, 0x040b0000, 0x2 },
  { "TLTU    rs,rt", 0xfc00ffff, 0x00000033, 0x2 },
  { "TNE     rs,rt", 0xfc00ffff, 0x00000036, 0x2 },
  { "TNEI    rs,simm", 0xfc1f0000, 0x040e0000, 0x2 },
#endif /* DSNET_COMPILING_E */
  { "XOR     rd,rs,rt", 0xfc0007ff, 0x00000026, 0x1 },
  { "XORI    rt,rs,imm", 0xfc000000, 0x38000000, 0x1 },
#ifdef DSNET_COMPILING_E
  { "DIV1    rs,rt", 0xfc00ffff, 0x7000001a, 0x100 },
  { "DIVU1   rs,rt", 0xfc00ffff, 0x7000001b, 0x100 },
  { "LQ      rt,simm(rs)", 0xfc000000, 0x78000000, 0x100 },
  { "MADD    rd,rs,rt", 0xfc0007ff, 0x70000000, 0x180 },
  { "MADD1   rd,rs,rt", 0xfc0007ff, 0x70000020, 0x100 },
  { "MADDU   rd,rs,rt", 0xfc0007ff, 0x70000001, 0x180 },
  { "MADDU1  rd,rs,rt", 0xfc0007ff, 0x70000021, 0x100 },
  { "MFHI1   rd", 0xffff07ff, 0x70000010, 0x100 },
  { "MFLO1   rd", 0xffff07ff, 0x70000012, 0x100 },
  { "MFSA    rd", 0xffff07ff, 0x00000028, 0x100 },
  { "MTHI1   rs", 0xfc1fffff, 0x70000011, 0x100 },
  { "MTLO1   rs", 0xfc1fffff, 0x70000013, 0x100 },
  { "MTSA    rs", 0xfc1fffff, 0x00000029, 0x100 },
  { "MTSAB   rs,simm", 0xfc1f0000, 0x04180000, 0x100 },
  { "MTSAH   rs,simm", 0xfc1f0000, 0x04190000, 0x100 },
  { "MULT    rd,rs,rt", 0xfc0007ff, 0x00000018, 0x180 },
  { "MULT1   rd,rs,rt", 0xfc0007ff, 0x70000018, 0x100 },
  { "MULTU   rd,rs,rt", 0xfc0007ff, 0x00000019, 0x180 },
  { "MULTU1  rd,rs,rt", 0xfc0007ff, 0x70000019, 0x100 },
  { "PABSH   rd,rt", 0xffe007ff, 0x70000168, 0x100 },
  { "PABSW   rd,rt", 0xffe007ff, 0x70000068, 0x100 },
  { "PADDB   rd,rs,rt", 0xfc0007ff, 0x70000208, 0x100 },
  { "PADDH   rd,rs,rt", 0xfc0007ff, 0x70000108, 0x100 },
  { "PADDSB  rd,rs,rt", 0xfc0007ff, 0x70000608, 0x100 },
  { "PADDSH  rd,rs,rt", 0xfc0007ff, 0x70000508, 0x100 },
  { "PADDSW  rd,rs,rt", 0xfc0007ff, 0x70000408, 0x100 },
  { "PADDUB  rd,rs,rt", 0xfc0007ff, 0x70000628, 0x100 },
  { "PADDUH  rd,rs,rt", 0xfc0007ff, 0x70000528, 0x100 },
  { "PADDUW  rd,rs,rt", 0xfc0007ff, 0x70000428, 0x100 },
  { "PADDW   rd,rs,rt", 0xfc0007ff, 0x70000008, 0x100 },
  { "PADSBH  rd,rs,rt", 0xfc0007ff, 0x70000128, 0x100 },
  { "PAND    rd,rs,rt", 0xfc0007ff, 0x70000489, 0x100 },
  { "PCEQB   rd,rs,rt", 0xfc0007ff, 0x700002a8, 0x100 },
  { "PCEQH   rd,rs,rt", 0xfc0007ff, 0x700001a8, 0x100 },
  { "PCEQW   rd,rs,rt", 0xfc0007ff, 0x700000a8, 0x100 },
  { "PCGTB   rd,rs,rt", 0xfc0007ff, 0x70000288, 0x100 },
  { "PCGTH   rd,rs,rt", 0xfc0007ff, 0x70000188, 0x100 },
  { "PCGTW   rd,rs,rt", 0xfc0007ff, 0x70000088, 0x100 },
  { "PCPYH   rd,rt", 0xffe007ff, 0x700006e9, 0x100 },
  { "PCPYLD  rd,rs,rt", 0xfc0007ff, 0x70000389, 0x100 },
  { "PCPYUD  rd,rs,rt", 0xfc0007ff, 0x700003a9, 0x100 },
  { "PDIVBW  rs,rt", 0xfc00ffff, 0x70000749, 0x100 },
  { "PDIVUW  rs,rt", 0xfc00ffff, 0x70000369, 0x100 },
  { "PDIVW   rs,rt", 0xfc00ffff, 0x70000349, 0x100 },
  { "PEXCH   rd,rt", 0xffe007ff, 0x700006a9, 0x100 },
  { "PEXCW   rd,rt", 0xffe007ff, 0x700007a9, 0x100 },
  { "PEXEH   rd,rt", 0xffe007ff, 0x70000689, 0x100 },
  { "PEXEW   rd,rt", 0xffe007ff, 0x70000789, 0x100 },
  { "PEXT5   rd,rt", 0xffe007ff, 0x70000788, 0x100 },
  { "PEXTLB  rd,rs,rt", 0xfc0007ff, 0x70000688, 0x100 },
  { "PEXTLH  rd,rs,rt", 0xfc0007ff, 0x70000588, 0x100 },
  { "PEXTLW  rd,rs,rt", 0xfc0007ff, 0x70000488, 0x100 },
  { "PEXTUB  rd,rs,rt", 0xfc0007ff, 0x700006a8, 0x100 },
  { "PEXTUH  rd,rs,rt", 0xfc0007ff, 0x700005a8, 0x100 },
  { "PEXTUW  rd,rs,rt", 0xfc0007ff, 0x700004a8, 0x100 },
  { "PHMADH  rd,rs,rt", 0xfc0007ff, 0x70000449, 0x100 },
  { "PHMSBH  rd,rs,rt", 0xfc0007ff, 0x70000549, 0x100 },
  { "PINTH   rd,rs,rt", 0xfc0007ff, 0x70000289, 0x100 },
  { "PINTEH  rd,rs,rt", 0xfc0007ff, 0x700002a9, 0x100 },
  { "PLZCW   rd,rs", 0xfc1f07ff, 0x70000004, 0x100 },
  { "PMADDH  rd,rs,rt", 0xfc0007ff, 0x70000409, 0x100 },
  { "PMADDUW rd,rs,rt", 0xfc0007ff, 0x70000029, 0x100 },
  { "PMADDW  rd,rs,rt", 0xfc0007ff, 0x70000009, 0x100 },
  { "PMAXH   rd,rs,rt", 0xfc0007ff, 0x700001c8, 0x100 },
  { "PMAXW   rd,rs,rt", 0xfc0007ff, 0x700000c8, 0x100 },
  { "PMFHI   rd", 0xffff07ff, 0x70000209, 0x100 },
  { "PMFLO   rd", 0xffff07ff, 0x70000249, 0x100 },
  { "PMFHL.LW rd", 0xffff07ff, 0x70000030, 0x100 },
  { "PMFHL.UW rd", 0xffff07ff, 0x70000070, 0x100 },
  { "PMFHL.SLW rd", 0xffff07ff, 0x700000b0, 0x100 },
  { "PMFHL.LH rd", 0xffff07ff, 0x700000f0, 0x100 },
  { "PMFHL.SH rd", 0xffff07ff, 0x70000130, 0x100 },
  { "PMINH   rd,rs,rt", 0xfc0007ff, 0x700001e8, 0x100 },
  { "PMINW   rd,rs,rt", 0xfc0007ff, 0x700000e8, 0x100 },
  { "PMSUBH  rd,rs,rt", 0xfc0007ff, 0x70000509, 0x100 },
  { "PMSUBW  rd,rs,rt", 0xfc0007ff, 0x70000109, 0x100 },
  { "PMTHI   rs", 0xfc1fffff, 0x70000229, 0x100 },
  { "PMTHL.LW rs", 0xfc1fffff, 0x70000031, 0x100 },
  { "PMTLO   rs", 0xfc1fffff, 0x70000269, 0x100 },
  { "PMULTH  rd,rs,rt", 0xfc0007ff, 0x70000709, 0x100 },
  { "PMULTUW rd,rs,rt", 0xfc0007ff, 0x70000329, 0x100 },
  { "PMULTW  rd,rs,rt", 0xfc0007ff, 0x70000309, 0x100 },
  { "PNOR    rd,rs,rt", 0xfc0007ff, 0x700004e9, 0x100 },
  { "POR     rd,rs,rt", 0xfc0007ff, 0x700004a9, 0x100 },
  { "PPAC5   rd,rt", 0xffe007ff, 0x700007c8, 0x100 },
  { "PPACB   rd,rs,rt", 0xfc0007ff, 0x700006c8, 0x100 },
  { "PPACH   rd,rs,rt", 0xfc0007ff, 0x700005c8, 0x100 },
  { "PPACW   rd,rs,rt", 0xfc0007ff, 0x700004c8, 0x100 },
  { "PREVH   rd,rt", 0xffe007ff, 0x700006c9, 0x100 },
  { "PROT3W  rd,rt", 0xffe007ff, 0x700007c9, 0x100 },
  { "PSLLH   rd,rt,shamt", 0xffe0003f, 0x70000034, 0x100 },
  { "PSLLVW  rd,rt,rs", 0xfc0007ff, 0x70000089, 0x100 },
  { "PSLLW   rd,rt,shamt", 0xffe0003f, 0x7000003c, 0x100 },
  { "PSRAH   rd,rt,shamt", 0xffe0003f, 0x70000037, 0x100 },
  { "PSRAVW  rd,rt,rs", 0xfc0007ff, 0x700000e9, 0x100 },
  { "PSRAW   rd,rt,shamt", 0xffe0003f, 0x7000003f, 0x100 },
  { "PSRLH   rd,rt,shamt", 0xffe0003f, 0x70000036, 0x100 },
  { "PSRLVW  rd,rt,rs", 0xfc0007ff, 0x700000c9, 0x100 },
  { "PSRLW   rd,rt,shamt", 0xffe0003f, 0x7000003e, 0x100 },
  { "PSUBB   rd,rs,rt", 0xfc0007ff, 0x70000248, 0x100 },
  { "PSUBH   rd,rs,rt", 0xfc0007ff, 0x70000148, 0x100 },
  { "PSUBSB  rd,rs,rt", 0xfc0007ff, 0x70000648, 0x100 },
  { "PSUBSH  rd,rs,rt", 0xfc0007ff, 0x70000548, 0x100 },
  { "PSUBSW  rd,rs,rt", 0xfc0007ff, 0x70000448, 0x100 },
  { "PSUBUB  rd,rs,rt", 0xfc0007ff, 0x70000668, 0x100 },
  { "PSUBUH  rd,rs,rt", 0xfc0007ff, 0x70000568, 0x100 },
  { "PSUBUW  rd,rs,rt", 0xfc0007ff, 0x70000468, 0x100 },
  { "PSUBW   rd,rs,rt", 0xfc0007ff, 0x70000048, 0x100 },
  { "PXOR    rd,rs,rt", 0xfc0007ff, 0x700004c9, 0x100 },
  { "QFSRV   rd,rs,rt", 0xfc0007ff, 0x700006e8, 0x100 },
  { "SQ      rt,simm(rs)", 0xfc000000, 0x7c000000, 0x100 },
  { "BC0F    off", 0xffff0000, 0x41000000, 0x1 },
  { "BC1F    off", 0xffff0000, 0x45000000, 0x1 },
  { "BC2F    off", 0xffff0000, 0x49000000, 0x1 },
  { "BC3F    off", 0xffff0000, 0x4d000000, 0x1 },
  { "BC0T    off", 0xffff0000, 0x41010000, 0x1 },
  { "BC1T    off", 0xffff0000, 0x45010000, 0x1 },
  { "BC2T    off", 0xffff0000, 0x49010000, 0x1 },
  { "BC3T    off", 0xffff0000, 0x4d010000, 0x1 },
  { "BC0FL   off", 0xffff0000, 0x41020000, 0x82 },
  { "BC1FL   off", 0xffff0000, 0x45020000, 0x82 },
  { "BC2FL   off", 0xffff0000, 0x49020000, 0x82 },
  { "BC3FL   off", 0xffff0000, 0x4d020000, 0x82 },
  { "BC0TL   off", 0xffff0000, 0x41030000, 0x82 },
  { "BC1TL   off", 0xffff0000, 0x45030000, 0x82 },
  { "BC2TL   off", 0xffff0000, 0x49030000, 0x82 },
  { "BC3TL   off", 0xffff0000, 0x4d030000, 0x82 },
  { "CACHE   op,simm(rs)", 0xfc000000, 0xbc000000, 0x100 },
  { "DI", 0xffffffff, 0x42000039, 0x100 },
  { "EI", 0xffffffff, 0x42000038, 0x100 },
  { "ERET", 0xffffffff, 0x42000018, 0x4 },
  { "MFBPC   rt", 0xffe0ffff, 0x4000c000, 0x100 },
  { "MFDAB   rt", 0xffe0ffff, 0x4000c004, 0x100 },
  { "MFDABM  rt", 0xffe0ffff, 0x4000c005, 0x100 },
  { "MFDVB   rt", 0xffe0ffff, 0x4000c006, 0x100 },
  { "MFDVBM  rt", 0xffe0ffff, 0x4000c007, 0x100 },
  { "MFIAB   rt", 0xffe0ffff, 0x4000c002, 0x100 },
  { "MFIABM  rt", 0xffe0ffff, 0x4000c003, 0x100 },
  { "MFPC    rt,reg", 0xffe0ffc1, 0x4000c801, 0x100 },
  { "MFPS    rt,reg", 0xffe0ffc1, 0x4000c800, 0x100 },
  { "MTBPC   rt", 0xffe0ffff, 0x4080c000, 0x100 },
  { "MTDAB   rt", 0xffe0ffff, 0x4080c004, 0x100 },
  { "MTDABM  rt", 0xffe0ffff, 0x4080c005, 0x100 },
  { "MTDVB   rt", 0xffe0ffff, 0x4080c006, 0x100 },
  { "MTDVBM  rt", 0xffe0ffff, 0x4080c007, 0x100 },
  { "MTIAB   rt", 0xffe0ffff, 0x4080c002, 0x100 },
  { "MTIABM  rt", 0xffe0ffff, 0x4080c003, 0x100 },
  { "MTPC    rt,reg", 0xffe0ffc1, 0x4080c801, 0x100 },
  { "MTPS    rt,reg", 0xffe0ffc1, 0x4080c800, 0x100 },
  { "TLBP", 0xffffffff, 0x42000008, 0x1 },
  { "TLBR", 0xffffffff, 0x42000001, 0x1 },
  { "TLBWI", 0xffffffff, 0x42000002, 0x1 },
  { "TLBWR", 0xffffffff, 0x42000006, 0x1 },
  { "ABS.S   fd,fs", 0xffff003f, 0x46000005, 0x1 },
  { "ADD.S   fd,fs,ft", 0xffe0003f, 0x46000000, 0x1 },
  { "ADDA.S  fs,ft", 0xffe007ff, 0x46000018, 0x100 },
  { "C.F.S   fs,ft", 0xffe007ff, 0x46000030, 0x1 },
  { "C.EQ.S  fs,ft", 0xffe007ff, 0x46000032, 0x1 },
  { "C.LT.S  fs,ft", 0xffe007ff, 0x46000034, 0x1 },
  { "C.LE.S  fs,ft", 0xffe007ff, 0x46000036, 0x1 },
  { "CVT.S.W fd,fs", 0xffff003f, 0x46800020, 0x1 },
  { "CVT.W.S fd,fs", 0xffff003f, 0x46000024, 0x1 },
  { "DIV.S   fd,fs,ft", 0xffe0003f, 0x46000003, 0x1 },
  { "MADD.S  fd,fs,ft", 0xffe0003f, 0x4600001c, 0x100 },
  { "MADDA.S fs,ft", 0xffe007ff, 0x4600001e, 0x100 },
  { "MAX.S   fd,fs,ft", 0xffe0003f, 0x46000028, 0x100 },
  { "MIN.S   fd,fs,ft", 0xffe0003f, 0x46000029, 0x100 },
  { "MOV.S   fd,fs", 0xffff003f, 0x46000006, 0x1 },
  { "MSUB.S  fd,fs,ft", 0xffe0003f, 0x4600001d, 0x100 },
  { "MSUBA.S fs,ft", 0xffe007ff, 0x4600001f, 0x100 },
  { "MUL.S   fd,fs,ft", 0xffe0003f, 0x46000002, 0x1 },
  { "MULA.S  fs,ft", 0xffe007ff, 0x4600001a, 0x100 },
  { "NEG.S   fd,fs", 0xffff003f, 0x46000007, 0x1 },
  { "RSQRT.S fd,fs,ft", 0xffe0003f, 0x46000016, 0x100 },
  { "SQRT.S  fd,ft", 0xffe0f83f, 0x46000004, 0x100 },
  { "SUB.S   fd,fs,ft", 0xffe0003f, 0x46000001, 0x1 },
  { "SUBA.S  fs,ft", 0xffe007ff, 0x46000019, 0x100 },
  { "CFC2         rt,vis", 0xffe007ff, 0x48400000, 0x100 },
  { "CFC2.I       rt,vis", 0xffe007ff, 0x48400001, 0x100 },
  { "CTC2         rt,vis", 0xffe007ff, 0x48c00000, 0x100 },
  { "CTC2.I       rt,vis", 0xffe007ff, 0x48c00001, 0x100 },
  { "LQC2         vft,simm(rs)", 0xfc000000, 0xd8000000, 0x100 },
  { "QMFC2        rt,vfs", 0xffe007ff, 0x48200000, 0x100 },
  { "QMFC2.I      rt,vfs", 0xffe007ff, 0x48200001, 0x100 },
  { "QMTC2        rt,vfs", 0xffe007ff, 0x48a00000, 0x100 },
  { "QMTC2.I      rt,vfs", 0xffe007ff, 0x48a00001, 0x100 },
  { "SQC2         vft,simm(rs)", 0xfc000000, 0xf8000000, 0x100 },
  { "VABS&dest    vft,vfs", 0xfe0007ff, 0x4a0001fd, 0x100 },
  { "VADD&dest    vfd,vfs,vft", 0xfe00003f, 0x4a000028, 0x100 },
  { "VADDI&dest   vfd,vfs,I", 0xfe1f003f, 0x4a000022, 0x100 },
  { "VADDQ&dest   vfd,vfs,Q", 0xfe1f003f, 0x4a000020, 0x100 },
  { "VADD*&dest   vfd,vfs,vft", 0xfe00003c, 0x4a000000, 0x100 },
  { "VADDA&dest   ACC,vft,vfs", 0xfe0007ff, 0x4a0002bc, 0x100 },
  { "VADDAI&dest  ACC,vfs,I", 0xfe1f07ff, 0x4a00023e, 0x100 },
  { "VADDAQ&dest  ACC,vfs,Q", 0xfe1f07ff, 0x4a00023c, 0x100 },
  { "VADDA*&dest  ACC,vfs,vft", 0xfe0007fc, 0x4a00003c, 0x100 },
  { "VCALLMS      callms", 0xffe0003f, 0x4a000038, 0x100 },
  { "VCALLMSR     vis", 0xffffffff, 0x4a00d839, 0x100 },
  { "VCLIP*&dest  vfs,vft", 0xfe0007ff, 0x4a0001ff, 0x100 },
  { "VDIV         Q,vfs&fsf,vft&ftf", 0xfe0007ff, 0x4a0003bc, 0x100 },
  { "VFTOI0&dest  vft,vfs", 0xfe0007ff, 0x4a00017c, 0x100 },
  { "VFTOI4&dest  vft,vfs", 0xfe0007ff, 0x4a00017d, 0x100 },
  { "VFTOI12&dest vft,vfs", 0xfe0007ff, 0x4a00017e, 0x100 },
  { "VFTOI15&dest vft,vfs", 0xfe0007ff, 0x4a00017f, 0x100 },
  { "VIADD        vid,vis,vit", 0xffe0003f, 0x4a000030, 0x100 },
  { "VIADDI       vit,vis,iaddi", 0xffe0003f, 0x4a000032, 0x100 },
  { "VIAND        vid,vis,vit", 0xffe0003f, 0x4a000034, 0x100 },
  { "VILWR&dest   vit,(vis)", 0xfe0007ff, 0x4a0003fe, 0x100 },
  { "VIOR         vid,vis,vit", 0xffe0003f, 0x4a000035, 0x100 },
  { "VISUB        vid,vis,vit", 0xffe0003f, 0x4a000031, 0x100 },
  { "VISWR&dest   vit,(vis)", 0xfe0007ff, 0x4a0003ff, 0x100 },
  { "VITOF0&dest  vft,vfs", 0xfe0007ff, 0x4a00013c, 0x100 },
  { "VITOF4&dest  vft,vfs", 0xfe0007ff, 0x4a00013d, 0x100 },
  { "VITOF12&dest vft,vfs", 0xfe0007ff, 0x4a00013e, 0x100 },
  { "VITOF15&dest vft,vfs", 0xfe0007ff, 0x4a00013f, 0x100 },
  { "VLQD&dest    vft,(--vis)", 0xfe0007ff, 0x4a00037e, 0x100 },
  { "VLQI&dest    vft,(vis++)", 0xfe0007ff, 0x4a00037c, 0x100 },
  { "VMADD&dest   vfd,vfs,vft", 0xfe00003f, 0x4a000029, 0x100 },
  { "VMADDI&dest  vfd,vfs,I", 0xfe1f003f, 0x4a000023, 0x100 },
  { "VMADDQ&dest  vfd,vfs,Q", 0xfe1f003f, 0x4a000021, 0x100 },
  { "VMADD*&dest  vfd,vfs,vft", 0xfe00003c, 0x4a000008, 0x100 },
  { "VMADDA&dest  ACC,vfs,vft", 0xfe0007ff, 0x4a0002bd, 0x100 },
  { "VMADDAI&dest ACC,vfs,I", 0xfe1f07ff, 0x4a00023f, 0x100 },
  { "VMADDAQ&dest ACC,vfs,Q", 0xfe1f07ff, 0x4a00023d, 0x100 },
  { "VMADDA*&dest ACC,vfs,vft", 0xfe0007fc, 0x4a0000bc, 0x100 },
  { "VMAX&dest    vfd,vfs,vft", 0xfe00003f, 0x4a00002b, 0x100 },
  { "VMAXI&dest   vfd,vfs,I", 0xfe1f003f, 0x4a00001d, 0x100 },
  { "VMAX*&dest   vfd,vfs,vft", 0xfe00003c, 0x4a000010, 0x100 },
  { "VMFIR&dest   vft,vis", 0xfe0007ff, 0x4a0003fd, 0x100 },
  { "VMINI&dest   vfd,vfs,vft", 0xfe00003f, 0x4a00002f, 0x100 },
  { "VMINII&dest  vfd,vfs,I", 0xfe1f003f, 0x4a00001f, 0x100 },
  { "VMINI*&dest  vfd,vfs,vft", 0xfe00003c, 0x4a000014, 0x100 },
  { "VMOVE&dest   vft,vfs", 0xfe0007ff, 0x4a00033c, 0x100 },
  { "VMR32&dest   vft,vfs", 0xfe0007ff, 0x4a00033d, 0x100 },
  { "VMSUB&dest   vfd,vfs,vft", 0xfe00003f, 0x4a00002d, 0x100 },
  { "VMSUBI&dest  vfd,vfs,I", 0xfe1f003f, 0x4a000027, 0x100 },
  { "VMSUBQ&dest  vfd,vfs,Q", 0xfe1f003f, 0x4a000025, 0x100 },
  { "VMSUB*&dest  vfd,vfs,vft", 0xfe00003c, 0x4a00000c, 0x100 },
  { "VMSUBA&dest  ACC,vft,vfs", 0xfe0007ff, 0x4a0002fd, 0x100 },
  { "VMSUBAI&dest ACC,vfs,I", 0xfe1f07ff, 0x4a00027f, 0x100 },
  { "VMSUBAQ&dest ACC,vfs,Q", 0xfe1f07ff, 0x4a00027d, 0x100 },
  { "VMSUBA*&dest ACC,vfs,vft", 0xfe0007fc, 0x4a0000fc, 0x100 },
  { "VMTIR        vit,vfs&fsf", 0xff8007ff, 0x4a0003fc, 0x100 },
  { "VMUL&dest    vfd,vfs,vft", 0xfe00003f, 0x4a00002a, 0x100 },
  { "VMULI&dest   vfd,vfs,I", 0xfe1f003f, 0x4a00001e, 0x100 },
  { "VMULQ&dest   vfd,vfs,Q", 0xfe1f003f, 0x4a00001c, 0x100 },
  { "VMUL*&dest   vfd,vfs,vft", 0xfe00003c, 0x4a000018, 0x100 },
  { "VMULA&dest   ACC,vfs,vft", 0xfe0007ff, 0x4a0002be, 0x100 },
  { "VMULAI&dest  ACC,vfs,I", 0xfe1f07ff, 0x4a0001fe, 0x100 },
  { "VMULAQ&dest  ACC,vfs,Q", 0xfe1f07ff, 0x4a0001fc, 0x100 },
  { "VMULA*&dest  ACC,vfs,vft", 0xfe0007fc, 0x4a0001bc, 0x100 },
  { "VNOP", 0xffffffff, 0x4a0002ff, 0x100 },
  { "VOPMSUB&dest vfd,vfs,vft", 0xffe0003f, 0x4bc0002e, 0x100 },
  { "VOPMULA&dest ACC,vfs,vft", 0xffe007ff, 0x4bc002fe, 0x100 },
  { "VRGET&dest   vft,R", 0xfe00ffff, 0x4a00043d, 0x100 },
  { "VRINIT       R,vfs&fsf", 0xff9f07ff, 0x4a00043e, 0x100 },
  { "VRNEXT&dest  vft,R", 0xfe00ffff, 0x4a00043c, 0x100 },
  { "VRSQRT       Q,vfs&fsf,vft&ftf", 0xfe0007ff, 0x4a0003be, 0x100 },
  { "VRXOR        R,vfs&fsf", 0xff9f07ff, 0x4a00043f, 0x100 },
  { "VSQD&dest    vfs,(--vit)", 0xfe0007ff, 0x4a00037f, 0x100 },
  { "VSQI&dest    vfs,(vit++)", 0xfe0007ff, 0x4a00037d, 0x100 },
  { "VSQRT        Q,vft&ftf", 0xfe00ffff, 0x4a0003bd, 0x100 },
  { "VSUB&dest    vfd,vfs,vft", 0xfe00003f, 0x4a00002c, 0x100 },
  { "VSUBI&dest   vfd,vfs,I", 0xfe1f003f, 0x4a000026, 0x100 },
  { "VSUBQ&dest   vfd,vfs,Q", 0xfe1f003f, 0x4a000024, 0x100 },
  { "VSUB*&dest   vfd,vfs,vft", 0xfe00003c, 0x4a000004, 0x100 },
  { "VSUBA&dest   ACC,vfs,vft", 0xfe0007ff, 0x4a0002fc, 0x100 },
  { "VSUBAI&dest  ACC,vfs,I", 0xfe1f07ff, 0x4a00027e, 0x100 },
  { "VSUBAQ&dest  ACC,vfs,Q", 0xfe1f07ff, 0x4a00027c, 0x100 },
  { "VSUBA*&dest  ACC,vfs,vft", 0xfe0007fc, 0x4a00007c, 0x100 },
  { "VWAITQ", 0xffffffff, 0x4a0003bf, 0x100 },
#endif /* DSNET_COMPILING_E */
  { "CFC0    rt,cd", 0xffe007ff, 0x40400000, 0x1 },
  { "CFC1    rt,cd", 0xffe007ff, 0x44400000, 0x1 },
  { "CFC2    rt,cd", 0xffe007ff, 0x48400000, 0x1 },
  { "CFC3    rt,cd", 0xffe007ff, 0x4c400000, 0x1 },
  { "CTC0    rt,cd", 0xffe007ff, 0x40c00000, 0x1 },
  { "CTC1    rt,cd", 0xffe007ff, 0x44c00000, 0x1 },
  { "CTC2    rt,cd", 0xffe007ff, 0x48c00000, 0x1 },
  { "CTC3    rt,cd", 0xffe007ff, 0x4cc00000, 0x1 },
  { "MFC0    rt,zd", 0xffe007ff, 0x40000000, 0x1 },
  { "MFC1    rt,zd", 0xffe007ff, 0x44000000, 0x1 },
  { "MFC2    rt,zd", 0xffe007ff, 0x48000000, 0x1 },
  { "MFC3    rt,zd", 0xffe007ff, 0x4c000000, 0x1 },
  { "MTC0    rt,zd", 0xffe007ff, 0x40800000, 0x1 },
  { "MTC1    rt,zd", 0xffe007ff, 0x44800000, 0x1 },
  { "MTC2    rt,zd", 0xffe007ff, 0x48800000, 0x1 },
  { "MTC3    rt,zd", 0xffe007ff, 0x4c800000, 0x1 },
  { "LWC0    zt,simm(rs)", 0xfc000000, 0xc0000000, 0x1 },
  { "LWC1    zt,simm(rs)", 0xfc000000, 0xc4000000, 0x1 },
  { "LWC2    zt,simm(rs)", 0xfc000000, 0xc8000000, 0x1 },
  { "LWC3    zt,simm(rs)", 0xfc000000, 0xcc000000, 0x1 },
  { "SWC0    zt,simm(rs)", 0xfc000000, 0xe0000000, 0x1 },
  { "SWC1    zt,simm(rs)", 0xfc000000, 0xe4000000, 0x1 },
  { "SWC2    zt,simm(rs)", 0xfc000000, 0xe8000000, 0x1 },
  { "SWC3    zt,simm(rs)", 0xfc000000, 0xec000000, 0x1 },
  { "COP0    cofun", 0xfe000000, 0x42000000, 0x1 },
  { "COP1    cofun", 0xfe000000, 0x46000000, 0x1 },
  { "COP2    cofun", 0xfe000000, 0x4a000000, 0x1 },
  { "COP3    cofun", 0xfe000000, 0x4e000000, 0x1 },
  { NULL, 0x00000000, 0x00000000, 0x0 },
};

static char *detect_load_32bits(char *bp, unsigned int adr, unsigned int inst, unsigned int next);
static int tabs(char *buf, char *bp);
static char *func_and_line(char *bp, unsigned int adr, int kind);
static char *source_line(unsigned int adr, char **pfile, int *pline);
static char *disasm(
        char *buf,
        unsigned int adr,
        unsigned int prev,
        unsigned int inst,
        unsigned int next,
        int f_adr,
        int f_iw,
        int f_ba,
        int f_ma);
static void display_symbol_once(char *old, unsigned int adr, int f_iw, int f_adr);
static int name_regno(int kind, int z, char *name, int *pval);
static int assemble_pseudo_instruction(char *name, char *bp, unsigned int *ovals, int olen);
static int assemble(char *buf, unsigned int *ovals, int olen, int f_ma);
static void clear_bts();
static BT *append_bt(unsigned int adr, unsigned int sp);
static int bt_fetch_word(unsigned int adr, unsigned int *pval);
static int backtrace_to_end_of_prologue(unsigned __int8 *ins_buf, unsigned int adr0, BT_REG *br);
static int backtrace_to_top_of_function(unsigned __int8 *ins_buf, unsigned int adr0, BT_REG *br, BT *bt);
static int backtrace_to_prev_function(BT_REG *br);

int is_branch_instruction(unsigned int adr, unsigned int ins, unsigned int *ptadr)
{
  unsigned int v3; // edx
  quad pq; // [esp+Ch] [ebp-1Ch] BYREF
  char tmp[10]; // [esp+1Ch] [ebp-Ch] BYREF

  if ( (ins & 0xFC000000) == 0x4000000 || (ins & 0xB0000000) == 0x10000000 || (ins & 0xF3FC0000) == 1090519040 )
  {
    if ( ptadr )
    {
      if ( (ins & 0x8000u) == 0 )
        v3 = adr + 4 * ((unsigned __int16)ins + 1);
      else
        v3 = adr + 4 * ((ins | 0xFFFF0000) + 1);
      *ptadr = v3;
    }
    if ( (ins & 0xFFFF0000) == 0x10000000 )
      return 1;
    else
      return 2;
  }
  else if ( (ins & 0xF8000000) == 0x8000000 )
  {
    if ( ptadr )
      *ptadr = (4 * (ins & 0x3FFFFFF)) | adr & 0xF0000000;
    return 1;
  }
  else if ( (ins & 0xFC00003E) == 8 )
  {
    if ( ptadr )
    {
      ds_sprintf(tmp, "$%d", (ins >> 21) & 0x1F);
      if ( ds_load_quad_reg(tmp, &pq) )
        return -1;
      *ptadr = ds_cqw(pq);
    }
    return 1;
  }
  else
  {
    return 0;
  }
}

static char *detect_load_32bits(char *bp, unsigned int adr, unsigned int inst, unsigned int next)
{
  unsigned int v4; // eax
  char *result; // eax
  unsigned int v6; // eax
  unsigned int v7; // eax
  char v8[1024]; // [esp+8h] [ebp-40Ch] BYREF
  unsigned int imm1; // [esp+408h] [ebp-Ch]
  unsigned int imm0; // [esp+40Ch] [ebp-8h]
  unsigned int v11; // [esp+410h] [ebp-4h]
  char *buf; // [esp+41Ch] [ebp+8h]

  v4 = inst >> 26;
  if ( inst >> 26 == 13 )
  {
    if ( ((inst >> 21) & 0x1F) != 0 )
      return bp;
    imm0 = (unsigned __int16)inst;
    goto LABEL_18;
  }
  if ( v4 <= 0xD )
  {
    if ( v4 <= 9 && v4 >= 8 )
    {
      if ( ((inst >> 21) & 0x1F) != 0 )
        return bp;
      v6 = (unsigned __int16)inst;
      if ( (inst & 0x8000u) != 0 )
        v6 = inst | 0xFFFF0000;
      imm0 = v6;
      goto LABEL_18;
    }
    return bp;
  }
  if ( v4 != 15 )
    return bp;
  imm0 = (unsigned __int16)inst << 16;
LABEL_18:
  v11 = BYTE2(inst) & 0x1F;
  if ( (inst & 0x1F0000) == 0 || v11 != ((next >> 21) & 0x1F) )
    return bp;
  v7 = (unsigned __int16)next;
  if ( (next & 0x8000u) != 0 )
    v7 = next | 0xFFFF0000;
  imm1 = v7;
  switch ( next >> 26 )
  {
    case 8u:
    case 9u:
      imm0 += imm1;
      goto LABEL_30;
    case 0xCu:
      imm0 &= (unsigned __int16)imm1;
      goto LABEL_30;
    case 0xDu:
      imm0 |= (unsigned __int16)imm1;
      goto LABEL_30;
    case 0xEu:
      imm0 ^= (unsigned __int16)imm1;
      goto LABEL_30;
    case 0x20u:
    case 0x21u:
    case 0x22u:
    case 0x23u:
    case 0x24u:
    case 0x25u:
    case 0x26u:
    case 0x28u:
    case 0x29u:
    case 0x2Au:
    case 0x2Bu:
    case 0x2Eu:
    case 0x30u:
    case 0x31u:
    case 0x32u:
    case 0x33u:
    case 0x38u:
    case 0x39u:
    case 0x3Au:
    case 0x3Bu:
      imm0 += imm1;
LABEL_30:
      buf = &bp[ds_sprintf(bp, " 0x%08x", imm0)];
      if ( address_to_symbol(v8, imm0, 0) )
        buf += ds_sprintf(buf, " <%s>", v8);
      result = buf;
      break;
    default:
      result = bp;
      break;
  }
  return result;
}

static int tabs(char *buf, char *bp)
{
  return ds_sprintf(bp, "%*s", 4 - (bp - buf) % 4, "");
}

static char *func_and_line(char *bp, unsigned int adr, int kind)
{
  int f_has; // [esp+0h] [ebp-Ch] BYREF
  int line; // [esp+4h] [ebp-8h] BYREF
  char *file; // [esp+8h] [ebp-4h]

  file = address_to_file_and_line(adr, &line, &f_has, 0, 0);
  if ( kind )
  {
    if ( kind == 1 )
    {
      if ( file )
        bp += ds_sprintf(bp, "%s", file);
      if ( line > 0 )
        bp += ds_sprintf(bp, ":%d", line);
    }
  }
  else if ( line <= 0 )
  {
    if ( f_has )
      bp += ds_sprintf(bp, "    ");
  }
  else
  {
    bp += ds_sprintf(bp, " %3d", line);
  }
  return bp;
}

static char *source_line(unsigned int adr, char **pfile, int *pline)
{
  int line; // [esp+0h] [ebp-Ch] BYREF
  char *obj_path; // [esp+4h] [ebp-8h] BYREF
  char *file; // [esp+8h] [ebp-4h]

  if ( pline )
    *pline = 0;
  file = address_to_file_and_line(adr, &line, 0, 0, &obj_path);
  if ( !file )
    return 0;
  if ( pfile )
    *pfile = file;
  if ( pline )
    *pline = line;
  return string_by_file_and_line(file, line, obj_path);
}

static char *disasm(
        char *buf,
        unsigned int adr,
        unsigned int prev,
        unsigned int inst,
        unsigned int next,
        int f_adr,
        int f_iw,
        int f_ba,
        int f_ma)
{
  int v9; // eax
  int v10; // eax
  int v11; // eax
  int v12; // eax
  int v13; // eax
  int v14; // eax
  int v15; // eax
  int v17; // eax
  int v18; // eax
  int v19; // eax
  int v20; // eax
  int v21; // eax
  int v22; // eax
  int v23; // eax
  int v24; // eax
  int v25; // eax
  signed int v26; // eax
  const char *v27; // eax
  int v28; // eax
  int v29; // eax
  const char *v30; // eax
  int v31; // eax
  unsigned int v32; // edi
  int v33; // eax
  int v34; // eax
  int v35; // eax
  int v36; // eax
  int v37; // eax
  int v38; // eax
  int v39; // eax
  int v40; // eax
  int v41; // eax
  int v42; // eax
  int v43; // eax
  int v44; // eax
  int v45; // eax
  int v46; // eax
  int v47; // eax
  int v48; // eax
#ifdef DSNET_COMPILING_E
  int v49; // eax
  int v50; // eax
  int v51; // eax
  int v52; // eax
  int v53; // eax
  int v54; // eax
  int v55; // eax
  int v56; // eax
  int v57; // eax
  int v58; // eax
  int v59; // eax
  int v60; // eax
  int v61; // eax
#endif /* DSNET_COMPILING_E */
  int si; // [esp+24h] [ebp-420h]
  int cm; // [esp+28h] [ebp-41Ch]
#ifdef DSNET_COMPILING_E
  signed int v64; // [esp+2Ch] [ebp-418h]
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  signed int v51; // [esp+2Ch] [ebp-418h]
#endif /* DSNET_COMPILING_I */
  int v; // [esp+2Ch] [ebp-418h]
#ifdef DSNET_COMPILING_E
  unsigned int v66; // [esp+2Ch] [ebp-418h]
#endif /* DSNET_COMPILING_E */
  char ch; // [esp+33h] [ebp-411h]
  char *dp; // [esp+34h] [ebp-410h]
  char *dp_1; // [esp+34h] [ebp-410h]
  char *sp; // [esp+38h] [ebp-40Ch]
  char *sp_1; // [esp+38h] [ebp-40Ch]
  char opr[1024]; // [esp+3Ch] [ebp-408h] BYREF
  char *bp; // [esp+43Ch] [ebp-8h]
  OPCODE *opcode; // [esp+440h] [ebp-4h]

  bp = buf;
  cm = 0;
  si = 0;
  switch ( f_adr )
  {
    case 1:
    case 3:
    case 5:
    case 7:
    case 9:
    case 11:
      bp += ds_sprintf(bp, "0x%08x", adr);
      break;
    case 2:
    case 6:
    case 10:
      if ( address_to_symbol(opr, adr, 1) )
        v10 = ds_sprintf(bp, "%s", opr);
      else
        v10 = ds_sprintf(bp, "0x%08x", adr);
      bp += v10;
      break;
    default:
      bp += ds_sprintf(bp, "0x%08x", adr);
      if ( address_to_symbol(opr, adr, 1) )
      {
        v9 = ds_sprintf(bp, " <%s>", opr);
        bp += v9;
      }
      break;
  }
  if ( f_adr > 3 && f_adr <= 6 || f_adr > 7 && f_adr <= 10 )
    bp = func_and_line(bp, adr, 0);
  if ( is_branch_instruction(adr - 4, prev, 0) <= 0 )
    v11 = 58;
  else
    v11 = 124;
  v12 = ds_sprintf(bp, "%c", v11);
  bp += v12;
  if ( f_iw != 1 )
  {
    v13 = ds_sprintf(bp, " 0x%W", inst);
    bp += v13;
  }
  v14 = ds_sprintf(bp, "  ");
  bp += v14;
  for ( opcode = opcodes;
        opcode->fmt && (opcode->code != (opcode->mask & inst) || (opcode->isa & 0x1000) != 0 && !f_ma);
        ++opcode )
  {
    ;
  }
  if ( !opcode->fmt )
  {
    v15 = ds_sprintf(bp, ".word   0x%08x", inst);
    bp += v15;
    return buf;
  }
  sp = opcode->fmt;
  while ( *sp )
  {
    v17 = 0;
    if ( *sp > 96 && *sp <= 122 )
      v17 = 1;
    if ( v17 || *sp == 42 || *sp == 38 )
    {
      opr[0] = *sp++;
      dp = &opr[1];
      opr[1] = 0;
      while ( 1 )
      {
        v19 = 0;
        if ( *sp > 96 && *sp <= 122 )
          v19 = 1;
        if ( !v19 )
          break;
        *dp = *sp++;
        *++dp = 0;
      }
      if ( !strcmp("rs", opr) )
      {
        v20 = ds_sprintf(bp, "$%s", gpr_name[(inst >> 21) & 0x1F]);
        bp += v20;
      }
      else if ( !strcmp("rt", opr) )
      {
        v21 = ds_sprintf(bp, "$%s", gpr_name[BYTE2(inst) & 0x1F]);
        bp += v21;
      }
      else if ( !strcmp("rd", opr) )
      {
        v22 = ds_sprintf(bp, "$%s", gpr_name[(unsigned __int16)inst >> 11]);
        bp += v22;
      }
      else if ( !strcmp("zt", opr) )
      {
        v23 = ds_sprintf(bp, "$%s", cpr_name[(inst >> 26) & 7][BYTE2(inst) & 0x1F]);
        bp += v23;
      }
      else if ( !strcmp("zd", opr) )
      {
        v24 = ds_sprintf(bp, "$%s", cpr_name[(inst >> 26) & 7][(unsigned __int16)inst >> 11]);
        bp += v24;
      }
      else if ( !strcmp("cd", opr) )
      {
        v25 = ds_sprintf(bp, "$%s", ccr_name[(inst >> 26) & 7][(unsigned __int16)inst >> 11]);
        bp += v25;
      }
      else if ( !strcmp("simm", opr) )
      {
        if ( (inst & 0x8000u) == 0 )
          v26 = (unsigned __int16)inst;
        else
          v26 = inst | 0xFFFF0000;
#ifdef DSNET_COMPILING_E
        v64 = v26;
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
        v51 = v26;
#endif /* DSNET_COMPILING_I */
        if ( v26 < 0 )
        {
          *bp++ = 45;
#ifdef DSNET_COMPILING_E
          v64 = -v26;
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
          v51 = -v26;
#endif /* DSNET_COMPILING_I */
        }
#ifdef DSNET_COMPILING_E
        if ( v64 < 0 || v64 > 9 )
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
        if ( v51 < 0 || v51 > 9 )
#endif /* DSNET_COMPILING_I */
          v27 = "0x";
        else
          v27 = "";
#ifdef DSNET_COMPILING_E
        v28 = ds_sprintf(bp, "%s%x", v27, v64);
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
        v28 = ds_sprintf(bp, "%s%x", v27, v51);
#endif /* DSNET_COMPILING_I */
        bp += v28;
        ++si;
      }
      else if ( !strcmp("imm", opr) )
      {
        v29 = ds_sprintf(bp, "0x%04x", (unsigned __int16)inst);
        bp += v29;
      }
      else if ( !strcmp("shamt", opr) )
      {
        if ( ((inst >> 6) & 0x1F) > 9 )
          v30 = "0x";
        else
          v30 = "";
        v31 = ds_sprintf(bp, "%s%x", v30, (inst >> 6) & 0x1F);
        bp += v31;
      }
      else
      {
        if ( !strcmp("off", opr) )
        {
          v32 = adr + 4;
          if ( (inst & 0x8000u) == 0 )
            v33 = 4 * (unsigned __int16)inst + v32;
          else
            v33 = 4 * (inst | 0xFFFF0000) + v32;
          v = v33;
          goto LABEL_84;
        }
        if ( !strcmp("target", opr) )
        {
          v = (4 * (inst & 0x3FFFFFF)) | adr & 0xF0000000;
LABEL_84:
          if ( f_ba == 1 )
          {
            v37 = ds_sprintf(bp, "0x%08x", v);
            bp += v37;
          }
          else if ( f_ba > 1 && f_ba == 2 )
          {
            if ( address_to_symbol(opr, v, 0) )
              v38 = ds_sprintf(bp, "%s", opr);
            else
              v38 = ds_sprintf(bp, "0x%08x", v);
            bp += v38;
          }
          else
          {
            v34 = ds_sprintf(bp, "0x%08x", v);
            bp += v34;
            if ( address_to_symbol(opr, v, 0) )
            {
              v35 = tabs(buf, bp);
              bp += v35;
              v36 = ds_sprintf(bp, "# <%s>", opr);
              bp += v36;
              ++cm;
            }
          }
        }
        else if ( !strcmp("code", opr) )
        {
          v39 = ds_sprintf(bp, "0x%x", (inst >> 6) & 0xFFFFF);
          bp += v39;
        }
        else if ( !strcmp("cofun", opr) )
        {
          v40 = ds_sprintf(bp, "0x%x", inst & 0x1FFFFFF);
          bp += v40;
        }
        else if ( !strcmp("z", opr) )
        {
          v41 = ds_sprintf(bp, "%d", (inst >> 26) & 7);
          bp += v41;
        }
        else if ( !strcmp("hint", opr) )
        {
          v42 = ds_sprintf(bp, "%d", BYTE2(inst) & 0x1F);
          bp += v42;
        }
        else if ( !strcmp("reg", opr) )
        {
          v43 = ds_sprintf(bp, "%d", (inst >> 1) & 0x1F);
          bp += v43;
        }
        else if ( !strcmp("op", opr) )
        {
          v44 = ds_sprintf(bp, "0x%x", BYTE2(inst) & 0x1F);
          bp += v44;
        }
        else if ( !strcmp("ft", opr) )
        {
          v45 = ds_sprintf(bp, "$%s", cpr_name[1][BYTE2(inst) & 0x1F]);
          bp += v45;
        }
        else if ( !strcmp("fs", opr) )
        {
          v46 = ds_sprintf(bp, "$%s", cpr_name[1][(unsigned __int16)inst >> 11]);
          bp += v46;
        }
#ifdef DSNET_COMPILING_E
        else if ( !strcmp("fd", opr) )
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
        else
#endif /* DSNET_COMPILING_I */
        {
#ifdef DSNET_COMPILING_E
          v47 = ds_sprintf(bp, "$%s", cpr_name[1][(inst >> 6) & 0x1F]);
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
          if ( !strcmp("fd", opr) )
            v47 = ds_sprintf(bp, "$%s", cpr_name[1][(inst >> 6) & 0x1F]);
          else
            v47 = ds_sprintf(bp, "??%s??", opr);
#endif /* DSNET_COMPILING_I */
          bp += v47;
        }
#ifdef DSNET_COMPILING_E
        else if ( !strcmp("&dest", opr) )
        {
          v48 = ds_sprintf(bp, ".%s", dest_name[(inst >> 21) & 0xF]);
          bp += v48;
        }
        else if ( !strcmp("*", opr) )
        {
          v49 = ds_sprintf(bp, "%s", xyzw_name[inst & 3]);
          bp += v49;
        }
        else if ( !strcmp("vft", opr) )
        {
          v50 = ds_sprintf(bp, "%s", cpr_name[2][BYTE2(inst) & 0x1F]);
          bp += v50;
        }
        else if ( !strcmp("vfs", opr) )
        {
          v51 = ds_sprintf(bp, "%s", cpr_name[2][(unsigned __int16)inst >> 11]);
          bp += v51;
        }
        else if ( !strcmp("vfd", opr) )
        {
          v52 = ds_sprintf(bp, "%s", cpr_name[2][(inst >> 6) & 0x1F]);
          bp += v52;
        }
        else if ( !strcmp("vit", opr) )
        {
          v53 = ds_sprintf(bp, "%s", ccr_name[2][BYTE2(inst) & 0x1F]);
          bp += v53;
        }
        else if ( !strcmp("vis", opr) )
        {
          v54 = ds_sprintf(bp, "%s", ccr_name[2][(unsigned __int16)inst >> 11]);
          bp += v54;
        }
        else if ( !strcmp("vid", opr) )
        {
          v55 = ds_sprintf(bp, "%s", ccr_name[2][(inst >> 6) & 0x1F]);
          bp += v55;
        }
        else if ( !strcmp("&ftf", opr) )
        {
          v56 = ds_sprintf(bp, "%s", xyzw_name[(inst >> 23) & 3]);
          bp += v56;
        }
        else if ( !strcmp("&fsf", opr) )
        {
          v57 = ds_sprintf(bp, "%s", xyzw_name[(inst >> 21) & 3]);
          bp += v57;
        }
        else if ( !strcmp("callms", opr) )
        {
          v58 = ds_sprintf(bp, "0x%x", 8 * ((inst >> 6) & 0x7FFF));
          bp += v58;
        }
        else if ( !strcmp("iaddi", opr) )
        {
          v66 = (inst >> 6) & 0x1F;
          if ( ((inst >> 6) & 0x10) != 0 )
            v66 = (inst >> 6) & 0xF | 0xFFFFFFF0;
          v59 = ds_sprintf(bp, "%d", v66);
          bp += v59;
        }
        else
        {
          v60 = ds_sprintf(bp, "??%s??", opr);
          bp += v60;
        }
#endif /* DSNET_COMPILING_E */
      }
    }
    else
    {
      ch = *sp++;
      v18 = 0;
      if ( ch > 64 && ch <= 90 )
        v18 = 1;
      if ( v18 )
        ch += 32;
      *bp++ = ch;
      *bp = 0;
    }
  }
  sp_1 = bp;
  dp_1 = bp;
  if ( !cm )
  {
#ifdef DSNET_COMPILING_E
    v61 = tabs(buf, bp);
    bp += v61;
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
    v48 = tabs(buf, bp);
    bp += v48;
#endif /* DSNET_COMPILING_I */
    *bp++ = 35;
    sp_1 = bp;
    *bp = 0;
  }
  bp = detect_load_32bits(bp, adr, inst, next);
  if ( bp == sp_1 )
    *dp_1 = 0;
  return buf;
}

static void display_symbol_once(char *old, unsigned int adr, int f_iw, int f_adr)
{
  int line; // [esp+4h] [ebp-C10h] BYREF
  char *sl; // [esp+8h] [ebp-C0Ch]
  char fl_buf[1024]; // [esp+Ch] [ebp-C08h] BYREF
  char *q; // [esp+40Ch] [ebp-808h]
  char *p; // [esp+410h] [ebp-804h]
  char v9[1024]; // [esp+414h] [ebp-800h] BYREF
  char buf[1024]; // [esp+814h] [ebp-400h] BYREF

  if ( !address_to_symbol(buf, adr, 0) )
  {
    *old = 0;
    return;
  }
  p = buf;
  q = v9;
  while ( *p && *p != 43 && *p != 44 )
    *q++ = *p++;
  *q = 0;
  if ( f_adr == 7 )
    goto LABEL_9;
  if ( f_adr == 11 )
  {
    sl = source_line(adr, 0, &line);
    if ( !sl )
    {
LABEL_9:
      fl_buf[0] = 0;
      func_and_line(fl_buf, adr, 1);
      if ( fl_buf[0] )
        ds_printf("   # %s\n", fl_buf);
      goto LABEL_16;
    }
    ds_printf("%-8d%s\n", line, sl);
    ds_free(sl);
  }
LABEL_16:
  if ( !*old || strcmp(old, v9) )
    ds_printf("   <%s>:\n", buf);
  strcpy(old, v9);
}

int di_cmd(int ac, char **av)
{
  int v2; // eax
  const char *v4; // eax
  char *v5; // eax
  char *sl; // [esp+0h] [ebp-10824h]
  char sym[1024]; // [esp+4h] [ebp-10820h] BYREF
  int line; // [esp+404h] [ebp-10420h] BYREF
  int f_ma; // [esp+408h] [ebp-1041Ch] BYREF
  int f_ba; // [esp+40Ch] [ebp-10418h] BYREF
  int f_iw; // [esp+410h] [ebp-10414h] BYREF
  int f_adr; // [esp+414h] [ebp-10410h] BYREF
  int v13; // [esp+418h] [ebp-1040Ch]
  int mark; // [esp+41Ch] [ebp-10408h] BYREF
  int len; // [esp+420h] [ebp-10404h]
  int cnt; // [esp+424h] [ebp-10400h] BYREF
  int i; // [esp+428h] [ebp-103FCh]
  int adr; // [esp+42Ch] [ebp-103F8h] BYREF
  unsigned int next; // [esp+430h] [ebp-103F4h]
  unsigned int inst; // [esp+434h] [ebp-103F0h]
  unsigned int prev; // [esp+438h] [ebp-103ECh]
  unsigned __int8 *dp; // [esp+43Ch] [ebp-103E8h]
  unsigned __int8 dat[65507]; // [esp+440h] [ebp-103E4h] BYREF
  unsigned __int8 buf[1024]; // [esp+10424h] [ebp-400h] BYREF
  static int last_cnt = 20;

  mark = 0;
  v13 = 0;
  di_format(&f_adr, &f_iw, &f_ba, &f_ma);
  if ( ac > 0 )
    v2 = 20;
  else
    v2 = last_cnt;
  cnt = v2;
  if ( ac > 0 )
  {
    --ac;
    ++av;
  }
  while ( ac > 0 && **av == 45 )
  {
    if ( strcmp("-m", *av) )
      return ds_error("Usage: di [-m <mark>] [<adr> [<cnt>]]");
    ++v13;
    if ( ac <= 1 )
      return ds_error("Usage: di [-m <mark>] [<adr> [<cnt>]]");
    if ( ds_eval_word(av[1], (unsigned int *)&mark) )
      return -1;
    ac -= 2;
    av += 2;
  }
  if ( ac > 2 )
    return ds_error("Usage: di [-m <mark>] [<adr> [<cnt>]]");
  if ( ac <= 0 )
  {
    adr = dot;
  }
  else if ( ds_eval_word(*av, (unsigned int *)&adr) )
  {
    return -1;
  }
  if ( ac > 1 && ds_eval_word(av[1], (unsigned int *)&cnt) )
    return -1;
  if ( (adr & 3) != 0 )
    return ds_error("address alignment error");
  last_cnt = cnt;
  cnt *= 4;
  if ( cnt <= 0 )
    return 0;
  sym[0] = 0;
  i = 0;
  len = 0;
  while ( cnt > i )
  {
    if ( len <= 11 )
    {
      len = cnt - i + 8;
      if ( (unsigned int)len > 0xFFE3 )
        len = 65507;
      dp = dat;
      if ( i + adr )
      {
        if ( ds_rdwr_mem_align(8, 2, 0, 0, i + adr - 4, dp, len) )
        {
          *dp = 0;
          if ( ds_rdwr_mem_align(8, 2, 0, 0, i + adr, dp + 4, len - 4) )
            return -1;
        }
      }
      else
      {
        *dp = 0;
        if ( ds_rdwr_mem_align(8, 2, 0, 0, i + adr, dp + 4, len - 4) )
          return -1;
      }
    }
    prev = *(_DWORD *)dp;
    inst = *((_DWORD *)dp + 1);
    next = *((_DWORD *)dp + 2);
    if ( f_adr == 3 || f_adr == 7 || f_adr == 11 )
    {
      display_symbol_once(sym, i + adr, f_iw, f_adr);
    }
    else if ( f_adr == 8 || f_adr == 9 || f_adr == 10 )
    {
      sl = source_line(i + adr, 0, &line);
      if ( sl )
      {
        ds_printf("%-8d%s\n", line, sl);
        ds_free(sl);
      }
    }
    if ( v13 )
    {
      if ( mark == i + adr )
        v4 = "->";
      else
        v4 = "  ";
      ds_printf("%s", v4);
    }
    else
    {
      ds_printf("  ");
    }
    v5 = disasm((char *)buf, i + adr, prev, inst, next, f_adr, f_iw, f_ba, f_ma);
    ds_printf("%s\n", v5);
    i += 4;
    dp += 4;
    len -= 4;
  }
  dot = cnt + adr;
  return 0;
}

static int name_regno(int kind, int z, char *name, int *pval)
{
  int v6; // eax
  int v7; // eax
  int val; // [esp+4h] [ebp-4h] BYREF
  char *s2; // [esp+18h] [ebp+10h]

  s2 = name + 1;
  if ( *name != 36 )
    return -1;
  if ( !strcmp("cause", s2) )
    s2 = "cr";
  for ( val = 0; val <= 31; ++val )
  {
    if ( kind == 1 )
    {
      if ( !strcmp(s2, gpr_name[val]) )
        break;
    }
    else if ( kind == 2 )
    {
      if ( !strcmp(s2, cpr_name[z][val]) )
        break;
    }
    else if ( kind == 3 && !strcmp(s2, ccr_name[z][val]) )
    {
      break;
    }
  }
  if ( val > 31 )
  {
    v6 = 0;
    if ( *s2 > 47 && *s2 <= 57 )
      v6 = 1;
    if ( v6 )
    {
      if ( ds_scan_digit_word(s2, (unsigned int *)&val) )
      {
        return -1;
      }
      else
      {
        v7 = val;
        LOBYTE(v7) = val & 0xE0;
        if ( v7 )
        {
          return -1;
        }
        else
        {
          *pval = val;
          return 0;
        }
      }
    }
    else
    {
      return -1;
    }
  }
  else
  {
    *pval = val;
    return 0;
  }
}

static int assemble_pseudo_instruction(char *name, char *bp, unsigned int *ovals, int olen)
{
  int v4; // eax
  char _c; // [esp+Bh] [ebp-815h]
  int v7; // [esp+Ch] [ebp-814h]
  int i; // [esp+Ch] [ebp-814h]
  int siz; // [esp+10h] [ebp-810h]
  int val; // [esp+14h] [ebp-80Ch] BYREF
  unsigned __int8 buf[1024]; // [esp+18h] [ebp-808h] BYREF
  char *p; // [esp+418h] [ebp-408h]
  char *op; // [esp+41Ch] [ebp-404h]
  char opr[1024]; // [esp+420h] [ebp-400h] BYREF

  siz = 0;
  strcpy(opr, bp);
  for ( op = opr; *op && *op != 59 && *op != 35; ++op )
    ;
  while ( op > opr )
  {
    _c = *(op - 1);
    v4 = 0;
    if ( _c == 32 || _c == 12 || _c == 10 || _c == 13 || _c == 9 || _c == 11 )
      v4 = 1;
    if ( !v4 )
      break;
    --op;
  }
  *op = 0;
  if ( !strcmp(".WORD", name) || !strcmp(".HALF", name) || !strcmp(".BYTE", name) )
  {
    v7 = 0;
    op = opr;
    while ( op && *op )
    {
      p = ds_strchr(op, 44);
      if ( p )
        *p++ = 0;
      if ( ds_eval_word(op, (unsigned int *)&val) )
        return -1;
      if ( !strcmp(".WORD", name) )
      {
        *(_DWORD *)&buf[4 * v7] = val;
        siz += 4;
      }
      else if ( !strcmp(".HALF", name) )
      {
        *(_WORD *)&buf[2 * v7] = val;
        siz += 2;
      }
      else
      {
        buf[v7] = val;
        ++siz;
      }
      op = p;
      ++v7;
    }
    if ( !ovals || olen < siz )
      goto LABEL_62;
    for ( i = 0; siz > i && ovals[i] == buf[i]; ++i )
      ;
    if ( i < siz )
    {
LABEL_62:
      if ( ds_store_mem(dot, buf, siz) )
        return -1;
    }
    goto LABEL_59;
  }
  if ( !strcmp(".ALIGN", name) )
  {
    if ( ds_eval_word(opr, (unsigned int *)&val) )
      return -1;
    if ( val > 31 )
      return ds_error("invalid range for .align - %s", opr);
    val = (1 << val) - 1;
    siz = (~val & (val + dot)) - dot;
LABEL_59:
    dot += siz;
    return siz;
  }
  if ( !strcmp(".SPACE", name) )
  {
    if ( ds_eval_word(opr, (unsigned int *)&val) )
      return -1;
    siz = val;
    goto LABEL_59;
  }
  if ( !strcmp(".ASCII", name) )
    return ds_error(".ascii is not yet supported");
  if ( !strcmp(".ASCIIZ", name) )
    return ds_error(".asciiz is not yet supported");
  return ds_error("invalid pseudo instrution - %s", name);
}

static int assemble(char *buf, unsigned int *ovals, int olen, int f_ma)
{
  int v4; // eax
  int v5; // eax
  char *v6; // eax
  int v7; // edx
  char v8; // dl
  int v9; // eax
  char v11; // al
  char v12; // al
  int v13; // eax
  int v14; // eax
  int v15; // eax
  int v16; // eax
  int v17; // eax
  int v18; // eax
  int v19; // eax
  int v20; // eax
  int v21; // eax
  int v22; // eax
#ifdef DSNET_COMPILING_E
  int v23; // eax
  int v24; // eax
  char v25; // al
  char *v26; // eax
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
  char v23; // al
  char *v24; // eax
#endif /* DSNET_COMPILING_I */
  char _c_5; // [esp+6h] [ebp-82Ah]
  char _c_6; // [esp+6h] [ebp-82Ah]
  char _c_7; // [esp+6h] [ebp-82Ah]
  char _c; // [esp+7h] [ebp-829h]
  char _c_3; // [esp+7h] [ebp-829h]
  char _c_4; // [esp+7h] [ebp-829h]
  int dest; // [esp+8h] [ebp-828h]
  int bc; // [esp+Ch] [ebp-824h]
  int v; // [esp+10h] [ebp-820h] BYREF
  unsigned int inst; // [esp+14h] [ebp-81Ch] BYREF
  char ch; // [esp+1Bh] [ebp-815h]
  char *fmt; // [esp+1Ch] [ebp-814h]
  char *op; // [esp+20h] [ebp-810h]
  char *tp; // [esp+24h] [ebp-80Ch]
  char opr[1024]; // [esp+28h] [ebp-808h] BYREF
  char tok[1024]; // [esp+428h] [ebp-408h] BYREF
  char *bp; // [esp+828h] [ebp-8h]
  OPCODE *opcode; // [esp+82Ch] [ebp-4h]

  bp = buf;
  inst = 0;
  bc = -1;
  dest = -1;
  while ( *bp )
  {
    _c = *bp;
    v4 = 0;
    if ( *bp == 32 || _c == 12 || _c == 10 || _c == 13 || _c == 9 || _c == 11 )
      v4 = 1;
    if ( !v4 )
      break;
    ++bp;
  }
  tp = tok;
  while ( 1 )
  {
    ch = *bp;
    if ( !ch )
      break;
    v5 = 0;
    if ( ch == 32 || ch == 12 || ch == 10 || ch == 13 || ch == 9 || ch == 11 )
      v5 = 1;
    if ( v5 )
      break;
    v6 = tp++;
    v7 = 0;
    if ( ch > 96 && ch <= 122 )
      v7 = 1;
    if ( v7 )
      v8 = ch - 32;
    else
      v8 = ch;
    *v6 = v8;
    ++bp;
  }
  *tp = 0;
  while ( *bp )
  {
    _c_3 = *bp;
    v9 = 0;
    if ( *bp == 32 || _c_3 == 12 || _c_3 == 10 || _c_3 == 13 || _c_3 == 9 || _c_3 == 11 )
      v9 = 1;
    if ( !v9 )
      break;
    ++bp;
  }
  if ( tok[0] == 46 )
    return assemble_pseudo_instruction(tok, bp, ovals, olen);
  for ( opcode = opcodes; opcode->fmt; ++opcode )
  {
    if ( (opcode->isa & 0x1000) == 0 || f_ma )
    {
      fmt = opcode->fmt;
      tp = tok;
      while ( *tp )
      {
        if ( *fmt == 42 && (*tp == 88 || *tp == 89 || *tp == 90 || *tp == 87) )
        {
          v11 = *tp;
          if ( *tp == 88 )
          {
            bc = 0;
          }
          else if ( v11 > 88 )
          {
            if ( v11 == 89 )
            {
              bc = 1;
            }
            else if ( v11 == 90 )
            {
              bc = 2;
            }
          }
          else if ( v11 == 87 )
          {
            bc = 3;
          }
        }
        else
        {
          if ( *fmt == 38 && *tp == 46 )
          {
            fmt += 4;
            dest = 0;
            while ( 1 )
            {
              v12 = tp[1];
              if ( v12 == 88 )
              {
                dest |= 0x1000000u;
              }
              else if ( v12 > 88 )
              {
                if ( v12 == 89 )
                {
                  dest |= 0x800000u;
                }
                else
                {
                  if ( v12 != 90 )
                    goto LABEL_81;
                  dest |= 0x400000u;
                }
              }
              else
              {
                if ( v12 != 87 )
                  goto LABEL_81;
                dest |= 0x200000u;
              }
              ++tp;
            }
          }
          if ( *tp != *fmt )
            break;
        }
LABEL_81:
        ++tp;
        ++fmt;
      }
      if ( !*tp && (*fmt == 32 || !*fmt) )
        break;
    }
  }
  if ( !opcode->fmt )
    return ds_error("invalid instrution - %s", tok);
  inst |= opcode->code;
  if ( bc >= 0 )
    inst |= bc;
  if ( dest >= 0 )
    inst |= dest;
  while ( *fmt )
  {
    _c_4 = *fmt;
    v13 = 0;
    if ( *fmt == 32 || _c_4 == 12 || _c_4 == 10 || _c_4 == 13 || _c_4 == 9 || _c_4 == 11 )
      v13 = 1;
    if ( !v13 )
      break;
    ++fmt;
  }
  do
  {
    while ( 1 )
    {
      op = opr;
      opr[0] = 0;
      if ( *fmt == 38 )
      {
LABEL_105:
        *op = *fmt++;
        *++op = 0;
      }
      v14 = 0;
      if ( *fmt > 96 && *fmt <= 122 )
        v14 = 1;
      if ( v14 )
        goto LABEL_105;
      v15 = 0;
      if ( *fmt > 64 && *fmt <= 90 )
        v15 = 1;
      if ( v15 )
        goto LABEL_105;
      if ( opr[0] )
        break;
      if ( !*fmt || *bp != *fmt )
#ifdef DSNET_COMPILING_E
        goto LABEL_350;
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
        goto LABEL_274;
#endif /* DSNET_COMPILING_I */
      ++fmt;
      ++bp;
    }
    ch = *fmt;
    if ( ch && ch != 38 )
      ++fmt;
    tp = tok;
    tok[0] = 0;
    while ( *bp )
    {
      _c_5 = *bp;
      v16 = 0;
      if ( *bp == 32 || _c_5 == 12 || _c_5 == 10 || _c_5 == 13 || _c_5 == 9 || _c_5 == 11 )
        v16 = 1;
      if ( v16 || ch == *bp || ch == 38 && (*bp == 120 || *bp == 121 || *bp == 122 || *bp == 119) )
        break;
      *tp = *bp++;
      *++tp = 0;
    }
    if ( ch != 44 )
      goto LABEL_163;
    while ( *bp == 32 )
      ++bp;
    if ( *bp == 44 )
    {
      ++bp;
      while ( *bp == 32 )
        ++bp;
    }
    else
    {
LABEL_163:
      if ( ch != *bp && (ch != 38 || *bp != 120 && *bp != 121 && *bp != 122 && *bp != 119) )
      {
        if ( ch )
          return ds_error("missing - '%c'", ch);
        _c_6 = *bp;
        v17 = 0;
        if ( *bp == 32 || _c_6 == 12 || _c_6 == 10 || _c_6 == 13 || _c_6 == 9 || _c_6 == 11 )
          v17 = 1;
        if ( !v17 )
          return ds_error("missing - '%c'", ch);
      }
      if ( *bp && ch != 38 )
        ++bp;
    }
    if ( !strcmp("rs", opr) )
    {
      if ( name_regno(1, 0, tok, &v) )
        return ds_error("invalid rs - %s", tok);
      inst |= (v & 0x1F) << 21;
    }
    else if ( !strcmp("rt", opr) )
    {
      if ( name_regno(1, 0, tok, &v) )
        return ds_error("invalid rt - %s", tok);
      inst |= (v & 0x1F) << 16;
    }
    else if ( !strcmp("rd", opr) )
    {
      if ( name_regno(1, 0, tok, &v) )
        return ds_error("invalid rd - %s", tok);
      inst |= (v & 0x1F) << 11;
    }
    else if ( !strcmp("zt", opr) )
    {
      if ( name_regno(2, (inst >> 26) & 7, tok, &v) )
        return ds_error("invalid c%drt - %s", (inst >> 26) & 7, tok);
      inst |= (v & 0x1F) << 16;
    }
    else if ( !strcmp("zd", opr) )
    {
      if ( name_regno(2, (inst >> 26) & 7, tok, &v) )
        return ds_error("invalid c%drd - %s", (inst >> 26) & 7, tok);
      inst |= (v & 0x1F) << 11;
    }
    else if ( !strcmp("cd", opr) )
    {
      if ( name_regno(3, (inst >> 26) & 7, tok, &v) )
        return ds_error("invalid c%dcd - %s", (inst >> 26) & 7, tok);
      inst |= (v & 0x1F) << 11;
    }
    else if ( !strcmp("simm", opr) )
    {
      if ( ds_eval_word(tok, (unsigned int *)&v) )
        return -1;
      if ( (v & 0xFFFF8000) != 0 && (v & 0xFFFF8000) != -32768 )
      {
        if ( opcode->mask != -2097152 || opcode->code != 603979776 )
          return ds_error("invalid simm - %s", tok);
        if ( (v & 0xFFFF0000) != 0 )
          return ds_error("invalid imm - %s", tok);
        inst &= 0x1FFFFFu;
        inst |= 0x34000000u;
      }
      inst |= (unsigned __int16)v;
    }
    else if ( !strcmp("imm", opr) )
    {
      if ( ds_eval_word(tok, (unsigned int *)&v) )
        return -1;
      if ( (v & 0xFFFF0000) != 0 )
        return ds_error("invalid imm - %s", tok);
      inst |= (unsigned __int16)v;
    }
    else if ( !strcmp("shamt", opr) )
    {
      if ( ds_eval_word(tok, (unsigned int *)&v) )
        return -1;
      v18 = v;
      LOBYTE(v18) = v & 0xE0;
      if ( v18 )
        return ds_error("invalid shamt - %s", tok);
      inst |= (v & 0x1F) << 6;
    }
    else if ( !strcmp("off", opr) )
    {
      if ( ds_eval_word(tok, (unsigned int *)&v) )
        return -1;
      v = v - 4 - dot;
      if ( (v & 3) != 0 || ((v >> 2) & 0xFFFF8000) != 0 && ((v >> 2) & 0xFFFF8000) != -32768 )
        return ds_error("invalid off range - %s", tok);
      inst |= (unsigned __int16)(v >> 2);
    }
    else if ( !strcmp("target", opr) )
    {
      if ( ds_eval_word(tok, (unsigned int *)&v) )
        return -1;
      if ( (v & 3) != 0 || (dot & 0xF000000) != (v & 0xF000000) )
        return ds_error("invalid target - %s", tok);
      inst |= (v >> 2) & 0x3FFFFFF;
    }
    else if ( !strcmp("code", opr) )
    {
      if ( ds_eval_word(tok, (unsigned int *)&v) )
        return -1;
      if ( (v & 0xFFF00000) != 0 )
        return ds_error("invalid code - %s", tok);
      inst |= (v & 0xFFFFF) << 6;
    }
    else if ( !strcmp("cofun", opr) )
    {
      if ( ds_eval_word(tok, (unsigned int *)&v) )
        return -1;
      if ( (v & 0xFE000000) != 0 )
        return ds_error("invalid cofun - %s", tok);
      inst |= v & 0x1FFFFFF;
    }
    else if ( !strcmp("hint", opr) )
    {
      if ( ds_eval_word(tok, (unsigned int *)&v) )
        return -1;
      v19 = v;
      LOBYTE(v19) = v & 0xE0;
      if ( v19 )
        return ds_error("invalid hint - %s", tok);
      inst |= (v & 0x1F) << 16;
    }
    else if ( !strcmp("reg", opr) )
    {
      if ( ds_eval_word(tok, (unsigned int *)&v) )
        return -1;
      v20 = v;
      LOBYTE(v20) = v & 0xE0;
      if ( v20 )
        return ds_error("invalid reg - %s", tok);
      inst |= 2 * (v & 0x1F);
    }
    else if ( !strcmp("op", opr) )
    {
      if ( ds_eval_word(tok, (unsigned int *)&v) )
        return -1;
      v21 = v;
      LOBYTE(v21) = v & 0xE0;
      if ( v21 )
        return ds_error("invalid op - %s", tok);
      inst |= (v & 0x1F) << 16;
    }
    else if ( !strcmp("ft", opr) )
    {
      if ( name_regno(2, 1, tok, &v) )
        return ds_error("invalid ft - %s", tok);
      inst |= (v & 0x1F) << 16;
    }
    else if ( !strcmp("fs", opr) )
    {
      if ( name_regno(2, 1, tok, &v) )
        return ds_error("invalid fs - %s", tok);
      inst |= (v & 0x1F) << 11;
    }
#ifdef DSNET_COMPILING_E
    else if ( !strcmp("fd", opr) )
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
    else
#endif /* DSNET_COMPILING_I */
    {
#ifdef DSNET_COMPILING_I
      if ( strcmp("fd", opr) )
        return ds_error("as: internal error - ??%s??", opr);
#endif /* DSNET_COMPILING_I */
      if ( name_regno(2, 1, tok, &v) )
        return ds_error("invalid fd - %s", tok);
      inst |= (v & 0x1F) << 6;
    }
#ifdef DSNET_COMPILING_E
    else if ( !strcmp("I", opr) )
    {
      if ( strcmp("i", tok) )
        return ds_error("invalid I - %s", tok);
    }
    else if ( !strcmp("R", opr) )
    {
      if ( strcmp("r", tok) )
        return ds_error("invalid R - %s", tok);
    }
    else if ( !strcmp("Q", opr) )
    {
      if ( strcmp("q", tok) )
        return ds_error("invalid Q - %s", tok);
    }
    else if ( !strcmp("ACC", opr) )
    {
      if ( strcmp("acc", tok) )
        return ds_error("invalid ACC - %s", tok);
    }
    else if ( strcmp("dest", opr) && strcmp("*", opr) )
    {
      if ( !strcmp("vft", opr) )
      {
        if ( name_regno(2, 2, tok, &v) )
          return ds_error("invalid vft - %s", tok);
        inst |= (v & 0x1F) << 16;
      }
      else if ( !strcmp("vfs", opr) )
      {
        if ( name_regno(2, 2, tok, &v) )
          return ds_error("invalid vfs - %s", tok);
        inst |= (v & 0x1F) << 11;
      }
      else if ( !strcmp("vfd", opr) )
      {
        if ( name_regno(2, 2, tok, &v) )
          return ds_error("invalid vfd - %s", tok);
        inst |= (v & 0x1F) << 6;
      }
      else if ( !strcmp("vit", opr) )
      {
        if ( name_regno(3, 2, tok, &v) )
          return ds_error("invalid vit - %s", tok);
        inst |= (v & 0x1F) << 16;
      }
      else if ( !strcmp("vis", opr) )
      {
        if ( name_regno(3, 2, tok, &v) )
          return ds_error("invalid vis - %s", tok);
        inst |= (v & 0x1F) << 11;
      }
      else if ( !strcmp("vid", opr) )
      {
        if ( name_regno(3, 2, tok, &v) )
          return ds_error("invalid vid - %s", tok);
        inst |= (v & 0x1F) << 6;
      }
      else if ( !strcmp("&ftf", opr) )
      {
        if ( strcmp("x", tok) )
        {
          if ( !strcmp("y", tok) )
          {
            inst |= 0x800000u;
          }
          else if ( !strcmp("z", tok) )
          {
            inst |= 0x1000000u;
          }
          else
          {
            if ( strcmp("w", tok) )
              return ds_error("invalid ftf - %s", tok);
            inst |= 0x1800000u;
          }
        }
      }
      else if ( !strcmp("&fsf", opr) )
      {
        if ( strcmp("x", tok) )
        {
          if ( !strcmp("y", tok) )
          {
            inst |= 0x200000u;
          }
          else if ( !strcmp("z", tok) )
          {
            inst |= 0x400000u;
          }
          else
          {
            if ( strcmp("w", tok) )
              return ds_error("invalid fsf - %s", tok);
            inst |= 0x600000u;
          }
        }
      }
      else if ( !strcmp("callms", opr) )
      {
        if ( ds_eval_word(tok, (unsigned int *)&v) )
          return -1;
        if ( (v & 0xFFFF8000) != 0 )
          return ds_error("invalid vcallms - %s", tok);
        inst |= ((v >> 3) & 0x7FFF) << 6;
      }
      else
      {
        if ( strcmp("iaddi", opr) )
          return ds_error("as: internal error - ??%s??", opr);
        if ( ds_eval_word(tok, (unsigned int *)&v) )
          return -1;
        v22 = v;
        LOBYTE(v22) = v & 0xF0;
        if ( v22 == -16 )
          v &= 0x1Fu;
        v23 = v;
        LOBYTE(v23) = v & 0xF0;
        if ( v23 )
          return ds_error("invalid vaddi - %s", tok);
        inst |= (v & 0x1F) << 6;
      }
    }
#endif /* DSNET_COMPILING_E */
  }
  while ( *fmt );
  while ( 1 )
  {
#ifdef DSNET_COMPILING_E
LABEL_350:
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
LABEL_274:
#endif /* DSNET_COMPILING_I */
    while ( *bp )
    {
      _c_7 = *bp;
#ifdef DSNET_COMPILING_E
      v24 = 0;
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
      v22 = 0;
#endif /* DSNET_COMPILING_I */
      if ( *bp == 32 || _c_7 == 12 || _c_7 == 10 || _c_7 == 13 || _c_7 == 9 || _c_7 == 11 )
#ifdef DSNET_COMPILING_E
        v24 = 1;
      if ( !v24 )
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
        v22 = 1;
      if ( !v22 )
#endif /* DSNET_COMPILING_I */
        break;
      ++bp;
    }
    ch = *bp;
    if ( ch != 91 && ch != 60 && ch != 123 )
      break;
    if ( ch == 91 )
    {
#ifdef DSNET_COMPILING_E
      v25 = 93;
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
      v23 = 93;
#endif /* DSNET_COMPILING_I */
    }
    else if ( ch == 60 )
    {
#ifdef DSNET_COMPILING_E
      v25 = 62;
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
      v23 = 62;
#endif /* DSNET_COMPILING_I */
    }
    else
    {
#ifdef DSNET_COMPILING_E
      v25 = 125;
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
      v23 = 125;
#endif /* DSNET_COMPILING_I */
    }
#ifdef DSNET_COMPILING_E
    ch = v25;
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
    ch = v23;
#endif /* DSNET_COMPILING_I */
    ++bp;
    while ( *bp && ch != *bp )
      ++bp;
#ifdef DSNET_COMPILING_E
    v26 = bp++;
    if ( ch != *v26 )
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
    v24 = bp++;
    if ( ch != *v24 )
#endif /* DSNET_COMPILING_I */
      return ds_error("unblanced - '%c'", ch);
  }
  if ( *bp && *bp != 59 && *bp != 35 )
    return ds_error("unexpected extra characters - %s", bp);
  if ( (!ovals || (unsigned int)olen <= 3 || inst != *ovals) && ds_rdwr_mem_align(10, 2, 0, 0, dot, &inst, 4) )
    return -1;
  dot += 4;
  return 4;
}

int as_cmd(int ac, char **av)
{
  int f_ma; // [esp+0h] [ebp-404h] BYREF
  char buf[1024]; // [esp+4h] [ebp-400h] BYREF
  int aca; // [esp+40Ch] [ebp+8h]
  int acb; // [esp+40Ch] [ebp+8h]
  char **ava; // [esp+410h] [ebp+Ch]
  char **avb; // [esp+410h] [ebp+Ch]

  di_format(0, 0, 0, &f_ma);
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  ava = av + 1;
  if ( aca > 0 && **ava == 45 || aca <= 1 )
    return ds_error("Usage: as <adr> <inst>");
  if ( ds_eval_word(*ava, &dot) )
    return -1;
  if ( (dot & 3) != 0 )
    return ds_error("address alignment error");
  buf[0] = 0;
  acb = aca - 1;
  avb = ava + 1;
  while ( acb > 0 )
  {
    ds_strcat(buf, *avb);
    if ( acb > 1 )
      ds_strcat(buf, " ");
    --acb;
    ++avb;
  }
  if ( assemble(buf, 0, 0, f_ma) >= 0 )
    return 0;
  else
    return -1;
}

static void clear_bts()
{
  BT *q; // [esp+0h] [ebp-8h]
  BT *p; // [esp+4h] [ebp-4h]

  for ( p = bts.head; p; p = q )
  {
    q = p->forw;
    ds_free(p);
  }
  bts.tail = 0;
  bts.head = 0;
  bt_no = 0;
}

static BT *append_bt(unsigned int adr, unsigned int sp)
{
  BT *tail; // edx
  BT *p; // [esp+4h] [ebp-4h]

  p = (BT *)ds_alloc(sizeof(BT));
  if ( !p )
    return 0;
  p->no = bt_no++;
  p->adr = adr;
  p->sp_hi = sp;
  p->sp_lo = sp;
  tail = bts.tail;
  p->back = bts.tail;
  if ( tail )
    p->back->forw = p;
  else
    bts.head = p;
  p->forw = 0;
  bts.tail = p;
  return p;
}

int eval_bt_reg(char *name, unsigned int *padr)
{
  unsigned int wv; // [esp+0h] [ebp-8h] BYREF
  BT *bt; // [esp+4h] [ebp-4h]

  if ( ds_strncmp("$BT", name, 3) || ds_scan_digit_word(name + 3, &wv) )
    return ds_error("invalid backtrace register - %s", name);
  for ( bt = bts.head; bt; bt = bt->forw )
  {
    if ( bt->no == wv )
    {
      *padr = bt->adr;
      return 0;
    }
  }
  return ds_error("backtrace register not found - %s", name);
}

int eval_sfa_reg(char *name, unsigned int *padr)
{
  unsigned int wv; // [esp+0h] [ebp-8h] BYREF
  BT *bt; // [esp+4h] [ebp-4h]

  if ( ds_strncmp("$SFA", name, 4) || ds_scan_digit_word(name + 4, &wv) )
    return ds_error("invalid frame address register - %s", name);
  for ( bt = bts.head; bt; bt = bt->forw )
  {
    if ( bt->no == wv )
    {
      *padr = bt->sp_lo;
      return 0;
    }
  }
  return ds_error("frame address register not found - %s", name);
}

int eval_sfs_reg(char *name, unsigned int *padr)
{
  unsigned int wv; // [esp+4h] [ebp-8h] BYREF
  BT *bt; // [esp+8h] [ebp-4h]

  if ( ds_strncmp("$SFS", name, 4) || ds_scan_digit_word(name + 4, &wv) )
    return ds_error("invalid frame size register - %s", name);
  for ( bt = bts.head; bt; bt = bt->forw )
  {
    if ( bt->no == wv )
    {
      *padr = bt->sp_hi - bt->sp_lo;
      return 0;
    }
  }
  return ds_error("frame size register not found - %s", name);
}

static int bt_fetch_word(unsigned int adr, unsigned int *pval)
{
  unsigned int val; // [esp+0h] [ebp-4h] BYREF

  if ( ds_rdwr_mem_align(8, 2, 0, 0, adr, &val, 4) )
    return -1;
  *pval = val;
  return 0;
}

static int backtrace_to_end_of_prologue(unsigned __int8 *ins_buf, unsigned int adr0, BT_REG *br)
{
  unsigned int adr; // [esp+4h] [ebp-4h]

  for ( adr = adr0; br->pc > adr && !is_branch_instruction(adr, *(_DWORD *)&ins_buf[adr - adr0], 0); adr += 4 )
    ;
  br->pc = adr;
  return 0;
}

static int backtrace_to_top_of_function(unsigned __int8 *ins_buf, unsigned int adr0, BT_REG *br, BT *bt)
{
  unsigned int sp; // esi
  unsigned int v5; // edx
  unsigned int v6; // eax
  unsigned int v8; // [esp+10h] [ebp-10h]
  unsigned int ins; // [esp+18h] [ebp-8h]
  unsigned int adr; // [esp+1Ch] [ebp-4h]

  for ( adr = br->pc - 4; adr >= adr0; adr -= 4 )
  {
    ins = *(_DWORD *)&ins_buf[adr - adr0];
    if ( HIWORD(ins) == 10173 || HIWORD(ins) == 9149 || HIWORD(ins) == 26557 )
    {
      sp = br->sp;
      if ( (ins & 0x8000u) == 0 )
        v5 = sp - (unsigned __int16)ins;
      else
        v5 = sp - (ins | 0xFFFF0000);
      br->sp = v5;
      bt->sp_hi = br->sp;
    }
    else if ( HIWORD(ins) == 44991 || HIWORD(ins) == 65471 || HIWORD(ins) == 32703 )
    {
      v8 = br->sp;
      v6 = (ins & 0x8000u) == 0 ? (unsigned __int16)ins + v8 : (ins | 0xFFFF0000) + v8;
      if ( bt_fetch_word(v6, &br->ra) )
        return -1;
    }
    br->pc = adr;
  }
  return 0;
}

static int backtrace_to_prev_function(BT_REG *br)
{
  br->pc = br->ra - 8;
  return 0;
}

void disp_bt(BT_REG *br, int cnt)
{
  unsigned int adr1; // [esp+0h] [ebp-418h] BYREF
  unsigned int adr0; // [esp+4h] [ebp-414h] BYREF
  BT *bt; // [esp+8h] [ebp-410h]
  char buf[1024]; // [esp+Ch] [ebp-40Ch] BYREF
  unsigned __int8 *ins_buf; // [esp+40Ch] [ebp-Ch]
  int n; // [esp+410h] [ebp-8h]
  int i; // [esp+414h] [ebp-4h]

  ins_buf = 0;
  clear_bts();
  for ( i = 0; cnt <= 0 || cnt > i; ++i )
  {
    bt = append_bt(br->pc, br->sp);
    ds_printf(" $BT%d=0x%W", i, br->pc);
    if ( address_to_symbol(buf, br->pc, 0) )
      ds_printf(" <%s>", buf);
    address_to_func(br->pc, &adr0, &adr1);
    if ( !adr0
      || !br->pc
      || !br->sp
      || !br->ra
      || (n = br->pc - adr0, (ins_buf = (unsigned __int8 *)ds_alloc(n)) == 0)
      || ds_rdwr_mem_align(8, 2, 0, 0, adr0, ins_buf, n) )
    {
      ds_printf("\n");
      break;
    }
    backtrace_to_end_of_prologue(ins_buf, adr0, br);
    backtrace_to_top_of_function(ins_buf, adr0, br, bt);
    backtrace_to_prev_function(br);
    n = 26 - strlen(buf);
    if ( n > 0 )
      ds_printf("%*s", n, "");
    ds_printf(" $SFA%d=0x%W $SFS%d=0x%W\n", i, bt->sp_lo, i, bt->sp_hi - bt->sp_lo);
    if ( !br->ra || br->pc >= adr0 && adr1 > br->pc )
      break;
    ins_buf = (unsigned __int8 *)ds_free(ins_buf);
  }
  ds_free(ins_buf);
}

int bt_cmd(int ac, char **av)
{
  BT_REG br; // [esp+0h] [ebp-10h] BYREF
  int cnt; // [esp+Ch] [ebp-4h] BYREF
  int aca; // [esp+18h] [ebp+8h]
  char **ava; // [esp+1Ch] [ebp+Ch]

  cnt = 0;
  if ( ac <= 0 )
    return 0;
  aca = ac - 1;
  ava = av + 1;
  if ( aca > 0 && **ava == 45 )
    return ds_error("Usage: bt [<cnt>]");
  if ( aca <= 0 )
    goto LABEL_11;
  if ( aca > 1 )
    return ds_error("Usage: bt [<cnt>]");
  if ( ds_eval_word(*ava, (unsigned int *)&cnt) )
    return -1;
LABEL_11:
  if ( ds_eval_word("$_PC", &br.pc) )
    return -1;
  if ( ds_eval_word("$_sp", &br.sp) )
    return -1;
  if ( ds_eval_word("$_ra", &br.ra) )
    return -1;
  disp_bt(&br, cnt);
  return 0;
}
