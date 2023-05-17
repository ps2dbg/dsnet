#include "dsedb_prototypes.h"

extern unsigned int dot; // defined in mem.c
static const char *gpr_name[] =
{
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
  "ra"
};
static const char *cpr_name[] =
{
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
  "rsvd31"
};
static const char *fpr_name[] =
{
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
  "fpr31"
};
static const char *vf_name[] =
{
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
  "vf31"
};
static const char *ccr_name[] =
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
  "31"
};
static const char *vi_name[] =
{
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
  "vi31"
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
  "wxyz"
};
static const char *xyzw_name[] =
{ "x", "y", "z", "w" };
static int last_cnt_16 = 20;
static struct {BT *head;BT *tail;} bts = { NULL, NULL };
static int bt_no = 0;
OPCODE opcodes[] =
{
  { "NOP", 4294967295u, 0u, 1u },
  { "LI      rt,simm", 4292870144u, 603979776u, 4097u },
  { "LI      rt,imm", 4292870144u, 872415232u, 4097u },
  { "MOVE    rd,rt", 4292872191u, 45u, 4100u },
  { "MOVE    rd,rs", 4229892095u, 45u, 4100u },
  { "MOVE    rd,rt", 4292872191u, 37u, 4097u },
  { "MOVE    rd,rs", 4229892095u, 37u, 4097u },
  { "B       off", 4294901760u, 268435456u, 4097u },
  { "B       off", 4294901760u, 67174400u, 4097u },
  { "BAL     off", 4294901760u, 68222976u, 4097u },
  { "ADD     rd,rs,rt", 4227860479u, 32u, 1u },
  { "ADDI    rt,rs,simm", 4227858432u, 536870912u, 1u },
  { "ADDIU   rt,rs,simm", 4227858432u, 603979776u, 1u },
  { "ADDU    rd,rs,rt", 4227860479u, 33u, 1u },
  { "AND     rd,rs,rt", 4227860479u, 36u, 1u },
  { "ANDI    rt,rs,imm", 4227858432u, 805306368u, 1u },
  { "BEQ     rs,rt,off", 4227858432u, 268435456u, 1u },
  { "BEQL    rs,rt,off", 4227858432u, 1342177280u, 130u },
  { "BGEZ    rs,off", 4229890048u, 67174400u, 1u },
  { "BGEZAL  rs,off", 4229890048u, 68222976u, 1u },
  { "BGEZALL rs,off", 4229890048u, 68354048u, 130u },
  { "BGEZL   rs,off", 4229890048u, 67305472u, 130u },
  { "BGTZ    rs,off", 4229890048u, 469762048u, 1u },
  { "BGTZL   rs,off", 4229890048u, 1543503872u, 130u },
  { "BLEZ    rs,off", 4229890048u, 402653184u, 1u },
  { "BLEZL   rs,off", 4229890048u, 1476395008u, 130u },
  { "BLTZ    rs,off", 4229890048u, 67108864u, 1u },
  { "BLTZAL  rs,off", 4229890048u, 68157440u, 1u },
  { "BLTZALL rs,off", 4229890048u, 68288512u, 130u },
  { "BLTZL   rs,off", 4229890048u, 67239936u, 130u },
  { "BNE     rs,rt,off", 4227858432u, 335544320u, 1u },
  { "BNEL    rs,rt,off", 4227858432u, 1409286144u, 130u },
  { "BREAK   code", 4227858495u, 13u, 1u },
  { "DADD    rd,rs,rt", 4227860479u, 44u, 4u },
  { "DADDI   rt,rs,simm", 4227858432u, 1610612736u, 4u },
  { "DADDIU  rt,rs,simm", 4227858432u, 1677721600u, 4u },
  { "DADDU   rd,rs,rt", 4227860479u, 45u, 4u },
  { "DIV     rs,rt", 4227923967u, 26u, 1u },
  { "DIVU    rs,rt", 4227923967u, 27u, 1u },
  { "DSLL    rd,rt,shamt", 4292870207u, 56u, 4u },
  { "DSLL32  rd,rt,shamt", 4292870207u, 60u, 4u },
  { "DSLLV   rd,rt,rs", 4227860479u, 20u, 4u },
  { "DSRA    rd,rt,shamt", 4292870207u, 59u, 4u },
  { "DSRA32  rd,rt,shamt", 4292870207u, 63u, 4u },
  { "DSRAV   rd,rt,rs", 4227860479u, 23u, 4u },
  { "DSRL    rd,rt,shamt", 4292870207u, 58u, 4u },
  { "DSRL32  rd,rt,shamt", 4292870207u, 62u, 4u },
  { "DSRLV   rd,rt,rs", 4227860479u, 22u, 4u },
  { "DSUB    rd,rs,rt", 4227860479u, 46u, 4u },
  { "DSUBU   rd,rs,rt", 4227860479u, 47u, 4u },
  { "J       target", 4227858432u, 134217728u, 1u },
  { "JAL     target", 4227858432u, 201326592u, 1u },
  { "JALR    rd,rs", 4229892095u, 9u, 1u },
  { "JR      rs", 4229955583u, 8u, 1u },
  { "LB      rt,simm(rs)", 4227858432u, 2147483648u, 1u },
  { "LBU     rt,simm(rs)", 4227858432u, 2415919104u, 1u },
  { "LD      rt,simm(rs)", 4227858432u, 3690987520u, 4u },
  { "LDL     rt,simm(rs)", 4227858432u, 1744830464u, 4u },
  { "LDR     rt,simm(rs)", 4227858432u, 1811939328u, 4u },
  { "LH      rt,simm(rs)", 4227858432u, 2214592512u, 1u },
  { "LHU     rt,simm(rs)", 4227858432u, 2483027968u, 1u },
  { "LUI     rt,imm", 4227858432u, 1006632960u, 1u },
  { "LW      rt,simm(rs)", 4227858432u, 2348810240u, 1u },
  { "LWL     rt,simm(rs)", 4227858432u, 2281701376u, 1u },
  { "LWR     rt,simm(rs)", 4227858432u, 2550136832u, 1u },
  { "LWU     rt,simm(rs)", 4227858432u, 2617245696u, 4u },
  { "MFHI    rd", 4294903807u, 16u, 1u },
  { "MFLO    rd", 4294903807u, 18u, 1u },
  { "MOVN    rd,rs,rt", 4227860479u, 11u, 8u },
  { "MOVZ    rd,rs,rt", 4227860479u, 10u, 8u },
  { "MTHI    rs", 4229955583u, 17u, 1u },
  { "MTLO    rs", 4229955583u, 19u, 1u },
  { "MULT    rs,rt", 4227923967u, 24u, 1u },
  { "MULTU   rs,rt", 4227923967u, 25u, 1u },
  { "NOR     rd,rs,rt", 4227860479u, 39u, 1u },
  { "OR      rd,rs,rt", 4227860479u, 37u, 1u },
  { "ORI     rt,rs,imm", 4227858432u, 872415232u, 1u },
  { "PREF    hint,simm(rs)", 4227858432u, 3422552064u, 8u },
  { "SB      rt,simm(rs)", 4227858432u, 2684354560u, 1u },
  { "SD      rt,simm(rs)", 4227858432u, 4227858432u, 4u },
  { "SDL     rt,simm(rs)", 4227858432u, 2952790016u, 4u },
  { "SDR     rt,simm(rs)", 4227858432u, 3019898880u, 4u },
  { "SH      rt,simm(rs)", 4227858432u, 2751463424u, 1u },
  { "SLL     rd,rt,shamt", 4292870207u, 0u, 1u },
  { "SLLV    rd,rt,rs", 4227860479u, 4u, 1u },
  { "SLT     rd,rs,rt", 4227860479u, 42u, 1u },
  { "SLTI    rt,rs,simm", 4227858432u, 671088640u, 1u },
  { "SLTIU   rt,rs,simm", 4227858432u, 738197504u, 1u },
  { "SLTU    rd,rs,rt", 4227860479u, 43u, 1u },
  { "SRA     rd,rt,shamt", 4292870207u, 3u, 1u },
  { "SRAV    rd,rt,rs", 4227860479u, 7u, 1u },
  { "SRL     rd,rt,shamt", 4292870207u, 2u, 1u },
  { "SRLV    rd,rt,rs", 4227860479u, 6u, 1u },
  { "SUB     rd,rs,rt", 4227860479u, 34u, 1u },
  { "SUBU    rd,rs,rt", 4227860479u, 35u, 1u },
  { "SW      rt,simm(rs)", 4227858432u, 2885681152u, 1u },
  { "SWL     rt,simm(rs)", 4227858432u, 2818572288u, 1u },
  { "SWR     rt,simm(rs)", 4227858432u, 3087007744u, 1u },
  { "SYNC", 4294967295u, 15u, 386u },
  { "SYNC.L", 4294967295u, 15u, 2u },
  { "SYNC.P", 4294967295u, 1039u, 2u },
  { "SYSCALL code", 4227858495u, 12u, 1u },
  { "TEQ     rs,rt", 4227923967u, 52u, 2u },
  { "TEQI    rs,simm", 4229890048u, 67895296u, 2u },
  { "TGE     rs,rt", 4227923967u, 48u, 2u },
  { "TGEI    rs,simm", 4229890048u, 67633152u, 2u },
  { "TGEIU   rs,simm", 4229890048u, 67698688u, 2u },
  { "TGEU    rs,rt", 4227923967u, 49u, 2u },
  { "TLT     rs,rt", 4227923967u, 50u, 2u },
  { "TLTI    rs,simm", 4229890048u, 67764224u, 2u },
  { "TLTIU   rs,simm", 4229890048u, 67829760u, 2u },
  { "TLTU    rs,rt", 4227923967u, 51u, 2u },
  { "TNE     rs,rt", 4227923967u, 54u, 2u },
  { "TNEI    rs,simm", 4229890048u, 68026368u, 2u },
  { "XOR     rd,rs,rt", 4227860479u, 38u, 1u },
  { "XORI    rt,rs,imm", 4227858432u, 939524096u, 1u },
  { "DIV1    rs,rt", 4227923967u, 1879048218u, 256u },
  { "DIVU1   rs,rt", 4227923967u, 1879048219u, 256u },
  { "LQ      rt,simm(rs)", 4227858432u, 2013265920u, 256u },
  { "MADD    rd,rs,rt", 4227860479u, 1879048192u, 384u },
  { "MADD1   rd,rs,rt", 4227860479u, 1879048224u, 256u },
  { "MADDU   rd,rs,rt", 4227860479u, 1879048193u, 384u },
  { "MADDU1  rd,rs,rt", 4227860479u, 1879048225u, 256u },
  { "MFHI1   rd", 4294903807u, 1879048208u, 256u },
  { "MFLO1   rd", 4294903807u, 1879048210u, 256u },
  { "MFSA    rd", 4294903807u, 40u, 256u },
  { "MTHI1   rs", 4229955583u, 1879048209u, 256u },
  { "MTLO1   rs", 4229955583u, 1879048211u, 256u },
  { "MTSA    rs", 4229955583u, 41u, 256u },
  { "MTSAB   rs,simm", 4229890048u, 68681728u, 256u },
  { "MTSAH   rs,simm", 4229890048u, 68747264u, 256u },
  { "MULT    rd,rs,rt", 4227860479u, 24u, 384u },
  { "MULT1   rd,rs,rt", 4227860479u, 1879048216u, 256u },
  { "MULTU   rd,rs,rt", 4227860479u, 25u, 384u },
  { "MULTU1  rd,rs,rt", 4227860479u, 1879048217u, 256u },
  { "PABSH   rd,rt", 4292872191u, 1879048552u, 256u },
  { "PABSW   rd,rt", 4292872191u, 1879048296u, 256u },
  { "PADDB   rd,rs,rt", 4227860479u, 1879048712u, 256u },
  { "PADDH   rd,rs,rt", 4227860479u, 1879048456u, 256u },
  { "PADDSB  rd,rs,rt", 4227860479u, 1879049736u, 256u },
  { "PADDSH  rd,rs,rt", 4227860479u, 1879049480u, 256u },
  { "PADDSW  rd,rs,rt", 4227860479u, 1879049224u, 256u },
  { "PADDUB  rd,rs,rt", 4227860479u, 1879049768u, 256u },
  { "PADDUH  rd,rs,rt", 4227860479u, 1879049512u, 256u },
  { "PADDUW  rd,rs,rt", 4227860479u, 1879049256u, 256u },
  { "PADDW   rd,rs,rt", 4227860479u, 1879048200u, 256u },
  { "PADSBH  rd,rs,rt", 4227860479u, 1879048488u, 256u },
  { "PAND    rd,rs,rt", 4227860479u, 1879049353u, 256u },
  { "PCEQB   rd,rs,rt", 4227860479u, 1879048872u, 256u },
  { "PCEQH   rd,rs,rt", 4227860479u, 1879048616u, 256u },
  { "PCEQW   rd,rs,rt", 4227860479u, 1879048360u, 256u },
  { "PCGTB   rd,rs,rt", 4227860479u, 1879048840u, 256u },
  { "PCGTH   rd,rs,rt", 4227860479u, 1879048584u, 256u },
  { "PCGTW   rd,rs,rt", 4227860479u, 1879048328u, 256u },
  { "PCPYH   rd,rt", 4292872191u, 1879049961u, 256u },
  { "PCPYLD  rd,rs,rt", 4227860479u, 1879049097u, 256u },
  { "PCPYUD  rd,rs,rt", 4227860479u, 1879049129u, 256u },
  { "PDIVBW  rs,rt", 4227923967u, 1879050057u, 256u },
  { "PDIVUW  rs,rt", 4227923967u, 1879049065u, 256u },
  { "PDIVW   rs,rt", 4227923967u, 1879049033u, 256u },
  { "PEXCH   rd,rt", 4292872191u, 1879049897u, 256u },
  { "PEXCW   rd,rt", 4292872191u, 1879050153u, 256u },
  { "PEXEH   rd,rt", 4292872191u, 1879049865u, 256u },
  { "PEXEW   rd,rt", 4292872191u, 1879050121u, 256u },
  { "PEXT5   rd,rt", 4292872191u, 1879050120u, 256u },
  { "PEXTLB  rd,rs,rt", 4227860479u, 1879049864u, 256u },
  { "PEXTLH  rd,rs,rt", 4227860479u, 1879049608u, 256u },
  { "PEXTLW  rd,rs,rt", 4227860479u, 1879049352u, 256u },
  { "PEXTUB  rd,rs,rt", 4227860479u, 1879049896u, 256u },
  { "PEXTUH  rd,rs,rt", 4227860479u, 1879049640u, 256u },
  { "PEXTUW  rd,rs,rt", 4227860479u, 1879049384u, 256u },
  { "PHMADH  rd,rs,rt", 4227860479u, 1879049289u, 256u },
  { "PHMSBH  rd,rs,rt", 4227860479u, 1879049545u, 256u },
  { "PINTH   rd,rs,rt", 4227860479u, 1879048841u, 256u },
  { "PINTEH  rd,rs,rt", 4227860479u, 1879048873u, 256u },
  { "PLZCW   rd,rs", 4229892095u, 1879048196u, 256u },
  { "PMADDH  rd,rs,rt", 4227860479u, 1879049225u, 256u },
  { "PMADDUW rd,rs,rt", 4227860479u, 1879048233u, 256u },
  { "PMADDW  rd,rs,rt", 4227860479u, 1879048201u, 256u },
  { "PMAXH   rd,rs,rt", 4227860479u, 1879048648u, 256u },
  { "PMAXW   rd,rs,rt", 4227860479u, 1879048392u, 256u },
  { "PMFHI   rd", 4294903807u, 1879048713u, 256u },
  { "PMFLO   rd", 4294903807u, 1879048777u, 256u },
  { "PMFHL.LW rd", 4294903807u, 1879048240u, 256u },
  { "PMFHL.UW rd", 4294903807u, 1879048304u, 256u },
  { "PMFHL.SLW rd", 4294903807u, 1879048368u, 256u },
  { "PMFHL.LH rd", 4294903807u, 1879048432u, 256u },
  { "PMFHL.SH rd", 4294903807u, 1879048496u, 256u },
  { "PMINH   rd,rs,rt", 4227860479u, 1879048680u, 256u },
  { "PMINW   rd,rs,rt", 4227860479u, 1879048424u, 256u },
  { "PMSUBH  rd,rs,rt", 4227860479u, 1879049481u, 256u },
  { "PMSUBW  rd,rs,rt", 4227860479u, 1879048457u, 256u },
  { "PMTHI   rs", 4229955583u, 1879048745u, 256u },
  { "PMTHL.LW rs", 4229955583u, 1879048241u, 256u },
  { "PMTLO   rs", 4229955583u, 1879048809u, 256u },
  { "PMULTH  rd,rs,rt", 4227860479u, 1879049993u, 256u },
  { "PMULTUW rd,rs,rt", 4227860479u, 1879049001u, 256u },
  { "PMULTW  rd,rs,rt", 4227860479u, 1879048969u, 256u },
  { "PNOR    rd,rs,rt", 4227860479u, 1879049449u, 256u },
  { "POR     rd,rs,rt", 4227860479u, 1879049385u, 256u },
  { "PPAC5   rd,rt", 4292872191u, 1879050184u, 256u },
  { "PPACB   rd,rs,rt", 4227860479u, 1879049928u, 256u },
  { "PPACH   rd,rs,rt", 4227860479u, 1879049672u, 256u },
  { "PPACW   rd,rs,rt", 4227860479u, 1879049416u, 256u },
  { "PREVH   rd,rt", 4292872191u, 1879049929u, 256u },
  { "PROT3W  rd,rt", 4292872191u, 1879050185u, 256u },
  { "PSLLH   rd,rt,shamt", 4292870207u, 1879048244u, 256u },
  { "PSLLVW  rd,rt,rs", 4227860479u, 1879048329u, 256u },
  { "PSLLW   rd,rt,shamt", 4292870207u, 1879048252u, 256u },
  { "PSRAH   rd,rt,shamt", 4292870207u, 1879048247u, 256u },
  { "PSRAVW  rd,rt,rs", 4227860479u, 1879048425u, 256u },
  { "PSRAW   rd,rt,shamt", 4292870207u, 1879048255u, 256u },
  { "PSRLH   rd,rt,shamt", 4292870207u, 1879048246u, 256u },
  { "PSRLVW  rd,rt,rs", 4227860479u, 1879048393u, 256u },
  { "PSRLW   rd,rt,shamt", 4292870207u, 1879048254u, 256u },
  { "PSUBB   rd,rs,rt", 4227860479u, 1879048776u, 256u },
  { "PSUBH   rd,rs,rt", 4227860479u, 1879048520u, 256u },
  { "PSUBSB  rd,rs,rt", 4227860479u, 1879049800u, 256u },
  { "PSUBSH  rd,rs,rt", 4227860479u, 1879049544u, 256u },
  { "PSUBSW  rd,rs,rt", 4227860479u, 1879049288u, 256u },
  { "PSUBUB  rd,rs,rt", 4227860479u, 1879049832u, 256u },
  { "PSUBUH  rd,rs,rt", 4227860479u, 1879049576u, 256u },
  { "PSUBUW  rd,rs,rt", 4227860479u, 1879049320u, 256u },
  { "PSUBW   rd,rs,rt", 4227860479u, 1879048264u, 256u },
  { "PXOR    rd,rs,rt", 4227860479u, 1879049417u, 256u },
  { "QFSRV   rd,rs,rt", 4227860479u, 1879049960u, 256u },
  { "SQ      rt,simm(rs)", 4227858432u, 2080374784u, 256u },
  { "BC0F    off", 4294901760u, 1090519040u, 1u },
  { "BC1F    off", 4294901760u, 1157627904u, 1u },
  { "BC2F    off", 4294901760u, 1224736768u, 1u },
  { "BC3F    off", 4294901760u, 1291845632u, 1u },
  { "BC0T    off", 4294901760u, 1090584576u, 1u },
  { "BC1T    off", 4294901760u, 1157693440u, 1u },
  { "BC2T    off", 4294901760u, 1224802304u, 1u },
  { "BC3T    off", 4294901760u, 1291911168u, 1u },
  { "BC0FL   off", 4294901760u, 1090650112u, 130u },
  { "BC1FL   off", 4294901760u, 1157758976u, 130u },
  { "BC2FL   off", 4294901760u, 1224867840u, 130u },
  { "BC3FL   off", 4294901760u, 1291976704u, 130u },
  { "BC0TL   off", 4294901760u, 1090715648u, 130u },
  { "BC1TL   off", 4294901760u, 1157824512u, 130u },
  { "BC2TL   off", 4294901760u, 1224933376u, 130u },
  { "BC3TL   off", 4294901760u, 1292042240u, 130u },
  { "CACHE   op,simm(rs)", 4227858432u, 3154116608u, 256u },
  { "DI", 4294967295u, 1107296313u, 256u },
  { "EI", 4294967295u, 1107296312u, 256u },
  { "ERET", 4294967295u, 1107296280u, 4u },
  { "MFBPC   rt", 4292935679u, 1073790976u, 256u },
  { "MFDAB   rt", 4292935679u, 1073790980u, 256u },
  { "MFDABM  rt", 4292935679u, 1073790981u, 256u },
  { "MFDVB   rt", 4292935679u, 1073790982u, 256u },
  { "MFDVBM  rt", 4292935679u, 1073790983u, 256u },
  { "MFIAB   rt", 4292935679u, 1073790978u, 256u },
  { "MFIABM  rt", 4292935679u, 1073790979u, 256u },
  { "MFPC    rt,reg", 4292935617u, 1073793025u, 256u },
  { "MFPS    rt,reg", 4292935617u, 1073793024u, 256u },
  { "MTBPC   rt", 4292935679u, 1082179584u, 256u },
  { "MTDAB   rt", 4292935679u, 1082179588u, 256u },
  { "MTDABM  rt", 4292935679u, 1082179589u, 256u },
  { "MTDVB   rt", 4292935679u, 1082179590u, 256u },
  { "MTDVBM  rt", 4292935679u, 1082179591u, 256u },
  { "MTIAB   rt", 4292935679u, 1082179586u, 256u },
  { "MTIABM  rt", 4292935679u, 1082179587u, 256u },
  { "MTPC    rt,reg", 4292935617u, 1082181633u, 256u },
  { "MTPS    rt,reg", 4292935617u, 1082181632u, 256u },
  { "TLBP", 4294967295u, 1107296264u, 1u },
  { "TLBR", 4294967295u, 1107296257u, 1u },
  { "TLBWI", 4294967295u, 1107296258u, 1u },
  { "TLBWR", 4294967295u, 1107296262u, 1u },
  { "ABS.S   fd,fs", 4294901823u, 1174405125u, 1u },
  { "ADD.S   fd,fs,ft", 4292870207u, 1174405120u, 1u },
  { "ADDA.S  fs,ft", 4292872191u, 1174405144u, 256u },
  { "C.F.S   fs,ft", 4292872191u, 1174405168u, 1u },
  { "C.EQ.S  fs,ft", 4292872191u, 1174405170u, 1u },
  { "C.LT.S  fs,ft", 4292872191u, 1174405172u, 1u },
  { "C.LE.S  fs,ft", 4292872191u, 1174405174u, 1u },
  { "CVT.S.W fd,fs", 4294901823u, 1182793760u, 1u },
  { "CVT.W.S fd,fs", 4294901823u, 1174405156u, 1u },
  { "DIV.S   fd,fs,ft", 4292870207u, 1174405123u, 1u },
  { "MADD.S  fd,fs,ft", 4292870207u, 1174405148u, 256u },
  { "MADDA.S fs,ft", 4292872191u, 1174405150u, 256u },
  { "MAX.S   fd,fs,ft", 4292870207u, 1174405160u, 256u },
  { "MIN.S   fd,fs,ft", 4292870207u, 1174405161u, 256u },
  { "MOV.S   fd,fs", 4294901823u, 1174405126u, 1u },
  { "MSUB.S  fd,fs,ft", 4292870207u, 1174405149u, 256u },
  { "MSUBA.S fs,ft", 4292872191u, 1174405151u, 256u },
  { "MUL.S   fd,fs,ft", 4292870207u, 1174405122u, 1u },
  { "MULA.S  fs,ft", 4292872191u, 1174405146u, 256u },
  { "NEG.S   fd,fs", 4294901823u, 1174405127u, 1u },
  { "RSQRT.S fd,fs,ft", 4292870207u, 1174405142u, 256u },
  { "SQRT.S  fd,ft", 4292933695u, 1174405124u, 256u },
  { "SUB.S   fd,fs,ft", 4292870207u, 1174405121u, 1u },
  { "SUBA.S  fs,ft", 4292872191u, 1174405145u, 256u },
  { "CFC2         rt,vis", 4292872191u, 1212153856u, 256u },
  { "CFC2.I       rt,vis", 4292872191u, 1212153857u, 256u },
  { "CTC2         rt,vis", 4292872191u, 1220542464u, 256u },
  { "CTC2.I       rt,vis", 4292872191u, 1220542465u, 256u },
  { "LQC2         vft,simm(rs)", 4227858432u, 3623878656u, 256u },
  { "QMFC2        rt,vfs", 4292872191u, 1210056704u, 256u },
  { "QMFC2.I      rt,vfs", 4292872191u, 1210056705u, 256u },
  { "QMTC2        rt,vfs", 4292872191u, 1218445312u, 256u },
  { "QMTC2.I      rt,vfs", 4292872191u, 1218445313u, 256u },
  { "SQC2         vft,simm(rs)", 4227858432u, 4160749568u, 256u },
  { "VABS&dest    vft,vfs", 4261414911u, 1241514493u, 256u },
  { "VADD&dest    vfd,vfs,vft", 4261412927u, 1241514024u, 256u },
  { "VADDI&dest   vfd,vfs,I", 4263444543u, 1241514018u, 256u },
  { "VADDQ&dest   vfd,vfs,Q", 4263444543u, 1241514016u, 256u },
  { "VADD*&dest   vfd,vfs,vft", 4261412924u, 1241513984u, 256u },
  { "VADDA&dest   ACC,vft,vfs", 4261414911u, 1241514684u, 256u },
  { "VADDAI&dest  ACC,vfs,I", 4263446527u, 1241514558u, 256u },
  { "VADDAQ&dest  ACC,vfs,Q", 4263446527u, 1241514556u, 256u },
  { "VADDA*&dest  ACC,vfs,vft", 4261414908u, 1241514044u, 256u },
  { "VCALLMS      callms", 4292870207u, 1241514040u, 256u },
  { "VCALLMSR     vis", 4294967295u, 1241569337u, 256u },
  { "VCLIP*&dest  vfs,vft", 4261414911u, 1241514495u, 256u },
  { "VDIV         Q,vfs&fsf,vft&ftf", 4261414911u, 1241514940u, 256u },
  { "VFTOI0&dest  vft,vfs", 4261414911u, 1241514364u, 256u },
  { "VFTOI4&dest  vft,vfs", 4261414911u, 1241514365u, 256u },
  { "VFTOI12&dest vft,vfs", 4261414911u, 1241514366u, 256u },
  { "VFTOI15&dest vft,vfs", 4261414911u, 1241514367u, 256u },
  { "VIADD        vid,vis,vit", 4292870207u, 1241514032u, 256u },
  { "VIADDI       vit,vis,iaddi", 4292870207u, 1241514034u, 256u },
  { "VIAND        vid,vis,vit", 4292870207u, 1241514036u, 256u },
  { "VILWR&dest   vit,(vis)", 4261414911u, 1241515006u, 256u },
  { "VIOR         vid,vis,vit", 4292870207u, 1241514037u, 256u },
  { "VISUB        vid,vis,vit", 4292870207u, 1241514033u, 256u },
  { "VISWR&dest   vit,(vis)", 4261414911u, 1241515007u, 256u },
  { "VITOF0&dest  vft,vfs", 4261414911u, 1241514300u, 256u },
  { "VITOF4&dest  vft,vfs", 4261414911u, 1241514301u, 256u },
  { "VITOF12&dest vft,vfs", 4261414911u, 1241514302u, 256u },
  { "VITOF15&dest vft,vfs", 4261414911u, 1241514303u, 256u },
  { "VLQD&dest    vft,(--vis)", 4261414911u, 1241514878u, 256u },
  { "VLQI&dest    vft,(vis++)", 4261414911u, 1241514876u, 256u },
  { "VMADD&dest   vfd,vfs,vft", 4261412927u, 1241514025u, 256u },
  { "VMADDI&dest  vfd,vfs,I", 4263444543u, 1241514019u, 256u },
  { "VMADDQ&dest  vfd,vfs,Q", 4263444543u, 1241514017u, 256u },
  { "VMADD*&dest  vfd,vfs,vft", 4261412924u, 1241513992u, 256u },
  { "VMADDA&dest  ACC,vfs,vft", 4261414911u, 1241514685u, 256u },
  { "VMADDAI&dest ACC,vfs,I", 4263446527u, 1241514559u, 256u },
  { "VMADDAQ&dest ACC,vfs,Q", 4263446527u, 1241514557u, 256u },
  { "VMADDA*&dest ACC,vfs,vft", 4261414908u, 1241514172u, 256u },
  { "VMAX&dest    vfd,vfs,vft", 4261412927u, 1241514027u, 256u },
  { "VMAXI&dest   vfd,vfs,I", 4263444543u, 1241514013u, 256u },
  { "VMAX*&dest   vfd,vfs,vft", 4261412924u, 1241514000u, 256u },
  { "VMFIR&dest   vft,vis", 4261414911u, 1241515005u, 256u },
  { "VMINI&dest   vfd,vfs,vft", 4261412927u, 1241514031u, 256u },
  { "VMINII&dest  vfd,vfs,I", 4263444543u, 1241514015u, 256u },
  { "VMINI*&dest  vfd,vfs,vft", 4261412924u, 1241514004u, 256u },
  { "VMOVE&dest   vft,vfs", 4261414911u, 1241514812u, 256u },
  { "VMR32&dest   vft,vfs", 4261414911u, 1241514813u, 256u },
  { "VMSUB&dest   vfd,vfs,vft", 4261412927u, 1241514029u, 256u },
  { "VMSUBI&dest  vfd,vfs,I", 4263444543u, 1241514023u, 256u },
  { "VMSUBQ&dest  vfd,vfs,Q", 4263444543u, 1241514021u, 256u },
  { "VMSUB*&dest  vfd,vfs,vft", 4261412924u, 1241513996u, 256u },
  { "VMSUBA&dest  ACC,vft,vfs", 4261414911u, 1241514749u, 256u },
  { "VMSUBAI&dest ACC,vfs,I", 4263446527u, 1241514623u, 256u },
  { "VMSUBAQ&dest ACC,vfs,Q", 4263446527u, 1241514621u, 256u },
  { "VMSUBA*&dest ACC,vfs,vft", 4261414908u, 1241514236u, 256u },
  { "VMTIR        vit,vfs&fsf", 4286580735u, 1241515004u, 256u },
  { "VMUL&dest    vfd,vfs,vft", 4261412927u, 1241514026u, 256u },
  { "VMULI&dest   vfd,vfs,I", 4263444543u, 1241514014u, 256u },
  { "VMULQ&dest   vfd,vfs,Q", 4263444543u, 1241514012u, 256u },
  { "VMUL*&dest   vfd,vfs,vft", 4261412924u, 1241514008u, 256u },
  { "VMULA&dest   ACC,vfs,vft", 4261414911u, 1241514686u, 256u },
  { "VMULAI&dest  ACC,vfs,I", 4263446527u, 1241514494u, 256u },
  { "VMULAQ&dest  ACC,vfs,Q", 4263446527u, 1241514492u, 256u },
  { "VMULA*&dest  ACC,vfs,vft", 4261414908u, 1241514428u, 256u },
  { "VNOP", 4294967295u, 1241514751u, 256u },
  { "VOPMSUB&dest vfd,vfs,vft", 4292870207u, 1270874158u, 256u },
  { "VOPMULA&dest ACC,vfs,vft", 4292872191u, 1270874878u, 256u },
  { "VRGET&dest   vft,R", 4261478399u, 1241515069u, 256u },
  { "VRINIT       R,vfs&fsf", 4288612351u, 1241515070u, 256u },
  { "VRNEXT&dest  vft,R", 4261478399u, 1241515068u, 256u },
  { "VRSQRT       Q,vfs&fsf,vft&ftf", 4261414911u, 1241514942u, 256u },
  { "VRXOR        R,vfs&fsf", 4288612351u, 1241515071u, 256u },
  { "VSQD&dest    vfs,(--vit)", 4261414911u, 1241514879u, 256u },
  { "VSQI&dest    vfs,(vit++)", 4261414911u, 1241514877u, 256u },
  { "VSQRT        Q,vft&ftf", 4261478399u, 1241514941u, 256u },
  { "VSUB&dest    vfd,vfs,vft", 4261412927u, 1241514028u, 256u },
  { "VSUBI&dest   vfd,vfs,I", 4263444543u, 1241514022u, 256u },
  { "VSUBQ&dest   vfd,vfs,Q", 4263444543u, 1241514020u, 256u },
  { "VSUB*&dest   vfd,vfs,vft", 4261412924u, 1241513988u, 256u },
  { "VSUBA&dest   ACC,vfs,vft", 4261414911u, 1241514748u, 256u },
  { "VSUBAI&dest  ACC,vfs,I", 4263446527u, 1241514622u, 256u },
  { "VSUBAQ&dest  ACC,vfs,Q", 4263446527u, 1241514620u, 256u },
  { "VSUBA*&dest  ACC,vfs,vft", 4261414908u, 1241514108u, 256u },
  { "VWAITQ", 4294967295u, 1241514943u, 256u },
  { "CFC0    rt,cd", 4292872191u, 1077936128u, 1u },
  { "CFC1    rt,cd", 4292872191u, 1145044992u, 1u },
  { "CFC2    rt,cd", 4292872191u, 1212153856u, 1u },
  { "CFC3    rt,cd", 4292872191u, 1279262720u, 1u },
  { "CTC0    rt,cd", 4292872191u, 1086324736u, 1u },
  { "CTC1    rt,cd", 4292872191u, 1153433600u, 1u },
  { "CTC2    rt,cd", 4292872191u, 1220542464u, 1u },
  { "CTC3    rt,cd", 4292872191u, 1287651328u, 1u },
  { "MFC0    rt,zd", 4292872191u, 1073741824u, 1u },
  { "MFC1    rt,zd", 4292872191u, 1140850688u, 1u },
  { "MFC2    rt,zd", 4292872191u, 1207959552u, 1u },
  { "MFC3    rt,zd", 4292872191u, 1275068416u, 1u },
  { "MTC0    rt,zd", 4292872191u, 1082130432u, 1u },
  { "MTC1    rt,zd", 4292872191u, 1149239296u, 1u },
  { "MTC2    rt,zd", 4292872191u, 1216348160u, 1u },
  { "MTC3    rt,zd", 4292872191u, 1283457024u, 1u },
  { "LWC0    zt,simm(rs)", 4227858432u, 3221225472u, 1u },
  { "LWC1    zt,simm(rs)", 4227858432u, 3288334336u, 1u },
  { "LWC2    zt,simm(rs)", 4227858432u, 3355443200u, 1u },
  { "LWC3    zt,simm(rs)", 4227858432u, 3422552064u, 1u },
  { "SWC0    zt,simm(rs)", 4227858432u, 3758096384u, 1u },
  { "SWC1    zt,simm(rs)", 4227858432u, 3825205248u, 1u },
  { "SWC2    zt,simm(rs)", 4227858432u, 3892314112u, 1u },
  { "SWC3    zt,simm(rs)", 4227858432u, 3959422976u, 1u },
  { "COP0    cofun", 4261412864u, 1107296256u, 1u },
  { "COP1    cofun", 4261412864u, 1174405120u, 1u },
  { "COP2    cofun", 4261412864u, 1241513984u, 1u },
  { "COP3    cofun", 4261412864u, 1308622848u, 1u },
  { NULL, 0u, 0u, 0u }
};

static char *__cdecl detect_load_32bits(char *bp, unsigned int adr, unsigned int inst, unsigned int next);
static int __cdecl tabs(char *buf, char *bp);
static char *__cdecl func_and_line(char *bp, unsigned int adr, int kind);
static char *__cdecl source_line(unsigned int adr, char **pfile, int *pline);
static char *__cdecl disasm(
        char *buf,
        unsigned int adr,
        unsigned int prev,
        unsigned int inst,
        unsigned int next,
        int f_adr,
        int f_iw,
        int f_ba,
        int f_ma);
static void __cdecl display_symbol_once(char *old, unsigned int adr, int f_iw, int f_adr);
static int __cdecl name_regno(int kind, int z, char *name, int *pval);
static int __cdecl assemble_pseudo_instruction(char *name, char *bp, unsigned int *ovals, int olen);
static int __cdecl assemble(char *buf, unsigned int *ovals, int olen, int f_ma);
static void __cdecl clear_bts();
static BT *__cdecl append_bt(unsigned int adr, unsigned int sp);
static int __cdecl bt_fetch_word(unsigned int adr, unsigned int *pval);
static int __cdecl backtrace_to_end_of_prologue(unsigned __int8 *ins_buf, unsigned int adr0, BT_REG *br);
static int __cdecl backtrace_to_top_of_function(unsigned __int8 *ins_buf, unsigned int adr0, BT_REG *br, BT *bt);
static int __cdecl backtrace_to_prev_function(BT_REG *br);

int __cdecl is_branch_instruction(unsigned int adr, unsigned int ins, unsigned int *ptadr)
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

static char *__cdecl detect_load_32bits(char *bp, unsigned int adr, unsigned int inst, unsigned int next)
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

static int __cdecl tabs(char *buf, char *bp)
{
  return ds_sprintf(bp, "%*s", 4 - (bp - buf) % 4, "");
}

static char *__cdecl func_and_line(char *bp, unsigned int adr, int kind)
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

static char *__cdecl source_line(unsigned int adr, char **pfile, int *pline)
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

static char *__cdecl disasm(
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
  int si; // [esp+24h] [ebp-420h]
  int cm; // [esp+28h] [ebp-41Ch]
  signed int v64; // [esp+2Ch] [ebp-418h]
  int v; // [esp+2Ch] [ebp-418h]
  unsigned int v66; // [esp+2Ch] [ebp-418h]
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
        v23 = ds_sprintf(bp, "$%s", (&cpr_name[32 * ((inst >> 26) & 7)])[BYTE2(inst) & 0x1F]);
        bp += v23;
      }
      else if ( !strcmp("zd", opr) )
      {
        v24 = ds_sprintf(bp, "$%s", (&cpr_name[32 * ((inst >> 26) & 7)])[(unsigned __int16)inst >> 11]);
        bp += v24;
      }
      else if ( !strcmp("cd", opr) )
      {
        v25 = ds_sprintf(bp, "$%s", (&ccr_name[32 * ((inst >> 26) & 7)])[(unsigned __int16)inst >> 11]);
        bp += v25;
      }
      else if ( !strcmp("simm", opr) )
      {
        if ( (inst & 0x8000u) == 0 )
          v26 = (unsigned __int16)inst;
        else
          v26 = inst | 0xFFFF0000;
        v64 = v26;
        if ( v26 < 0 )
        {
          *bp++ = 45;
          v64 = -v26;
        }
        if ( v64 < 0 || v64 > 9 )
          v27 = "0x";
        else
          v27 = "";
        v28 = ds_sprintf(bp, "%s%x", v27, v64);
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
          v45 = ds_sprintf(bp, "$%s", fpr_name[BYTE2(inst) & 0x1F]);
          bp += v45;
        }
        else if ( !strcmp("fs", opr) )
        {
          v46 = ds_sprintf(bp, "$%s", fpr_name[(unsigned __int16)inst >> 11]);
          bp += v46;
        }
        else if ( !strcmp("fd", opr) )
        {
          v47 = ds_sprintf(bp, "$%s", fpr_name[(inst >> 6) & 0x1F]);
          bp += v47;
        }
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
          v50 = ds_sprintf(bp, "%s", vf_name[BYTE2(inst) & 0x1F]);
          bp += v50;
        }
        else if ( !strcmp("vfs", opr) )
        {
          v51 = ds_sprintf(bp, "%s", vf_name[(unsigned __int16)inst >> 11]);
          bp += v51;
        }
        else if ( !strcmp("vfd", opr) )
        {
          v52 = ds_sprintf(bp, "%s", vf_name[(inst >> 6) & 0x1F]);
          bp += v52;
        }
        else if ( !strcmp("vit", opr) )
        {
          v53 = ds_sprintf(bp, "%s", vi_name[BYTE2(inst) & 0x1F]);
          bp += v53;
        }
        else if ( !strcmp("vis", opr) )
        {
          v54 = ds_sprintf(bp, "%s", vi_name[(unsigned __int16)inst >> 11]);
          bp += v54;
        }
        else if ( !strcmp("vid", opr) )
        {
          v55 = ds_sprintf(bp, "%s", vi_name[(inst >> 6) & 0x1F]);
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
    v61 = tabs(buf, bp);
    bp += v61;
    *bp++ = 35;
    sp_1 = bp;
    *bp = 0;
  }
  bp = detect_load_32bits(bp, adr, inst, next);
  if ( bp == sp_1 )
    *dp_1 = 0;
  return buf;
}

static void __cdecl display_symbol_once(char *old, unsigned int adr, int f_iw, int f_adr)
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
    ds_free_mem_low(sl, "opc.c", "display_symbol_once");
  }
LABEL_16:
  if ( !*old || strcmp(old, v9) )
    ds_printf("   <%s>:\n", buf);
  strcpy(old, v9);
}

int __cdecl di_cmd(int ac, char **av)
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

  mark = 0;
  v13 = 0;
  di_format(&f_adr, &f_iw, &f_ba, &f_ma);
  if ( ac > 0 )
    v2 = 20;
  else
    v2 = last_cnt_16;
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
  last_cnt_16 = cnt;
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
        ds_free_mem_low(sl, "opc.c", "di_cmd");
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

static int __cdecl name_regno(int kind, int z, char *name, int *pval)
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
      if ( !strcmp(s2, (&cpr_name[32 * z])[val]) )
        break;
    }
    else if ( kind == 3 && !strcmp(s2, (&ccr_name[32 * z])[val]) )
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

static int __cdecl assemble_pseudo_instruction(char *name, char *bp, unsigned int *ovals, int olen)
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

static int __cdecl assemble(char *buf, unsigned int *ovals, int olen, int f_ma)
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
  int v23; // eax
  int v24; // eax
  char v25; // al
  char *v26; // eax
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
        goto LABEL_350;
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
    else if ( !strcmp("fd", opr) )
    {
      if ( name_regno(2, 1, tok, &v) )
        return ds_error("invalid fd - %s", tok);
      inst |= (v & 0x1F) << 6;
    }
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
  }
  while ( *fmt );
  while ( 1 )
  {
LABEL_350:
    while ( *bp )
    {
      _c_7 = *bp;
      v24 = 0;
      if ( *bp == 32 || _c_7 == 12 || _c_7 == 10 || _c_7 == 13 || _c_7 == 9 || _c_7 == 11 )
        v24 = 1;
      if ( !v24 )
        break;
      ++bp;
    }
    ch = *bp;
    if ( ch != 91 && ch != 60 && ch != 123 )
      break;
    if ( ch == 91 )
    {
      v25 = 93;
    }
    else if ( ch == 60 )
    {
      v25 = 62;
    }
    else
    {
      v25 = 125;
    }
    ch = v25;
    ++bp;
    while ( *bp && ch != *bp )
      ++bp;
    v26 = bp++;
    if ( ch != *v26 )
      return ds_error("unblanced - '%c'", ch);
  }
  if ( *bp && *bp != 59 && *bp != 35 )
    return ds_error("unexpected extra characters - %s", bp);
  if ( (!ovals || (unsigned int)olen <= 3 || inst != *ovals) && ds_rdwr_mem_align(10, 2, 0, 0, dot, &inst, 4) )
    return -1;
  dot += 4;
  return 4;
}

int __cdecl as_cmd(int ac, char **av)
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

static void __cdecl clear_bts()
{
  BT *q; // [esp+0h] [ebp-8h]
  BT *p; // [esp+4h] [ebp-4h]

  for ( p = bts.head; p; p = q )
  {
    q = p->forw;
    ds_free_mem_low(p, "opc.c", "clear_bts");
  }
  bts.tail = 0;
  bts.head = 0;
  bt_no = 0;
}

static BT *__cdecl append_bt(unsigned int adr, unsigned int sp)
{
  BT *tail; // edx
  BT *p; // [esp+4h] [ebp-4h]

  p = (BT *)ds_alloc_mem_low("opc.c", "append_bt", sizeof(BT));
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

int __cdecl eval_bt_reg(char *name, unsigned int *padr)
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

int __cdecl eval_sfa_reg(char *name, unsigned int *padr)
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

int __cdecl eval_sfs_reg(char *name, unsigned int *padr)
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

static int __cdecl bt_fetch_word(unsigned int adr, unsigned int *pval)
{
  unsigned int val; // [esp+0h] [ebp-4h] BYREF

  if ( ds_rdwr_mem_align(8, 2, 0, 0, adr, &val, 4) )
    return -1;
  *pval = val;
  return 0;
}

static int __cdecl backtrace_to_end_of_prologue(unsigned __int8 *ins_buf, unsigned int adr0, BT_REG *br)
{
  unsigned int adr; // [esp+4h] [ebp-4h]

  for ( adr = adr0; br->pc > adr && !is_branch_instruction(adr, *(_DWORD *)&ins_buf[adr - adr0], 0); adr += 4 )
    ;
  br->pc = adr;
  return 0;
}

static int __cdecl backtrace_to_top_of_function(unsigned __int8 *ins_buf, unsigned int adr0, BT_REG *br, BT *bt)
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

static int __cdecl backtrace_to_prev_function(BT_REG *br)
{
  br->pc = br->ra - 8;
  return 0;
}

void __cdecl disp_bt(BT_REG *br, int cnt)
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
      || (n = br->pc - adr0, (ins_buf = (unsigned __int8 *)ds_alloc_mem_low("opc.c", "disp_bt", n)) == 0)
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
    ins_buf = (unsigned __int8 *)ds_free_mem_low(ins_buf, "opc.c", "disp_bt");
  }
  ds_free_mem_low(ins_buf, "opc.c", "disp_bt");
}

int __cdecl bt_cmd(int ac, char **av)
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

