#include "dsnet_prototypes.h"

static STRS SN_strs[3] =
{
  { "EE", "E", 69 },
  { "HOST", "H", 72 },
  { "IOP", "I", 73 }
};

static STRS SP_strs[42] =
{
  { "ANY", NULL, 0 },
  { "DCMP", NULL, 1 },
  { "I0TTYP", NULL, 272 },
  { "I1TTYP", NULL, 273 },
  { "I2TTYP", NULL, 274 },
  { "I3TTYP", NULL, 275 },
  { "I4TTYP", NULL, 276 },
  { "I5TTYP", NULL, 277 },
  { "I6TTYP", NULL, 278 },
  { "I7TTYP", NULL, 279 },
  { "I8TTYP", NULL, 280 },
  { "I9TTYP", NULL, 281 },
  { "IKTTYP", NULL, 287 },
  { "DRFP0", NULL, 288 },
  { "DRFP1", NULL, 289 },
  { "DRFP2", NULL, 290 },
  { "DRFP3", NULL, 291 },
  { "DRFP4", NULL, 292 },
  { "ISDBGP", NULL, 304 },
  { "ITDBGP", NULL, 320 },
  { "ILOADP", NULL, 336 },
  { "E0TTYP", NULL, 528 },
  { "E1TTYP", NULL, 529 },
  { "E2TTYP", NULL, 530 },
  { "E3TTYP", NULL, 531 },
  { "E4TTYP", NULL, 532 },
  { "E5TTYP", NULL, 533 },
  { "E6TTYP", NULL, 534 },
  { "E7TTYP", NULL, 535 },
  { "E8TTYP", NULL, 536 },
  { "E9TTYP", NULL, 537 },
  { "EKTTYP", NULL, 543 },
  { "ESDBGP", NULL, 560 },
  { "ETDBGP", NULL, 576 },
  { "ELOADP", NULL, 592 },
  { "NETMP", NULL, 1024 },
  { "NTTYP", NULL, 1040 },
  { "DTTYP", NULL, 1041 },
  { "CTTYP", NULL, 1042 },
  { "STTYP", NULL, 1043 },
  { "SYSTEMP", NULL, 1280 },
  { "IP", NULL, 1296 }
};

static STRS CT_strs[4] =
{
  { "CONNECT", "CO", 0 },
  { "ECHO", "EC", 1 },
  { "STATUS", "ST", 2 },
  { "ERROR", "ER", 3 }
};

static STRS CC_CO_strs[4] =
{
  { "CONNECT ", "C", 0 },
  { "CONNECTR", "CR", 1 },
  { "DISCONNECT ", "D", 2 },
  { "DISCONNECTR", "DR", 3 }
};

static STRS CE_CO_strs[4] =
{
  { "GOOD", "G", 0 },
  { "INVALDEST", "I", 1 },
  { "ALREADYCONN", "A", 2 },
  { "NOTCONNECT", "N", 3 }
};

static STRS CC_EC_strs[2] =
{
  { "ECHO", "E", 0 },
  { "ECHOR", "ER", 1 }
};

static STRS CC_ST_strs[5] =
{
  { "CONNECTED", "C", 0 },
  { "PROTO", "P", 1 },
  { "UNLOCKED", "L", 2 },
  { "SPACE", "S", 3 },
  { "ROUTE", "R", 4 }
};

static STRS CC_ER_strs[6] =
{
  { "NOROUTE", "R", 0 },
  { "NOPROTO", "P", 1 },
  { "LOCKED", "L", 2 },
  { "NOSPACE", "S", 3 },
  { "INVALHEAD", "H", 4 },
  { "NOCONNECT", "C", 5 }
};

static STRS CC_ER_LONG_strs[6] =
{
  { "No Route", NULL, 0 },
  { "Protocol Unreachable", NULL, 1 },
  { "Locked", NULL, 2 },
  { "No Space", NULL, 3 },
  { "Invalid Header", NULL, 4 },
  { "No Connect", NULL, 5 }
};

static STRS FC_strs[10] =
{
  { "OPEN ", "O ", 0 },
  { "OPENR", "OR", 1 },
  { "CLOSE ", "C ", 2 },
  { "CLOSER", "CR", 3 },
  { "READ ", "R ", 4 },
  { "READR", "RR", 5 },
  { "WRITE ", "W ", 6 },
  { "WRITER", "WR", 7 },
  { "SEEK ", "S ", 8 },
  { "SEEKR", "SR", 9 }
};

static STRS MC_strs[16] =
{
  { "CONNECT ", "C ", 0 },
  { "CONNECTR", "CR", 1 },
  { "RESET ", "R ", 2 },
  { "RESETR", "DR", 3 },
  { "MESSAGE ", "M ", 4 },
  { "MESSAGER", "MR", 5 },
  { "STATUS ", "S ", 6 },
  { "STATUSR", "SR", 7 },
  { "KILL ", "K ", 8 },
  { "KILLR", "KR", 9 },
  { "VERSION ", "V ", 10 },
  { "VERSIONR", "VR", 11 },
  { "POWEROFF ", "P ", 12 },
  { "POWEROFFR", "PR", 13 },
  { "COMPORT ", "T ", 14 },
  { "COMPORTR", "TR", 15 }
};

static STRS ME_strs[7] =
{
  { "OK", "O", 0 },
  { "INVAL", "I", 1 },
  { "BUSY", "B", 2 },
  { "NOTCONN", "N", 3 },
  { "ALREADYCONN", "A", 4 },
  { "NOMEM", "M", 5 },
  { "NOPROTO", "P", 6 }
};

static STRS DI_strs[3] =
{
  { "CPU", NULL, 0 },
  { "VU0", NULL, 1 },
  { "VU1", NULL, 2 }
};

static STRS DT_strs[35] =
{
  { "GETCONF ", "GC ", 0 },
  { "GETCONFR", "GCR", 1 },
  { "GETREG ", "GR ", 4 },
  { "GETREGR", "GRR", 5 },
  { "PUTREG ", "PR ", 6 },
  { "PUTREGR", "PRR", 7 },
  { "RDMEM ", "RM ", 8 },
  { "RDMEMR", "RMR", 9 },
  { "WRMEM ", "WM ", 10 },
  { "WRMEMR", "WMR", 11 },
  { "GETBRKPT ", "GB ", 16 },
  { "GETBRKPTR", "GBR", 17 },
  { "PUTBRKPT ", "PB ", 18 },
  { "PUTBRKPTR", "PBR", 19 },
  { "BREAK ", "BR ", 20 },
  { "BREAKR", "BRR", 21 },
  { "CONTINUE ", "CO ", 22 },
  { "CONTINUER", "COR", 23 },
  { "RUN ", "RU ", 24 },
  { "RUNR", "RUR", 25 },
  { "XGKTCTL ", "XC ", 32 },
  { "XGKTCTLR", "XCR", 33 },
  { "XGKTDATAR", "XDR", 35 },
  { "DBGCTL ", "DC ", 36 },
  { "DBGCTLR", "DCR", 37 },
  { "RDIMG ", "RI ", 40 },
  { "RDIMGR", "RIR", 41 },
  { "SETBPFUNC ", "SF ", 46 },
  { "SETBPFUNCR", "SFR", 47 },
  { "LIST", "LI ", 64 },
  { "LISTR", "LIR", 65 },
  { "INFO", "IF ", 66 },
  { "INFOR", "IFR", 67 },
  { "MEMLIST", "ML ", 68 },
  { "MEMLISTR", "MLR", 69 }
};

static STRS DC_C_strs[4] =
{
  { "CONT", "C", 0 },
  { "STEP", "S", 1 },
  { "NEXT", "N", 2 },
  { "OTHER", "O", 255 }
};

static STRS DR_strs[17] =
{
  { "GOOD", NULL, 0 },
  { "INVALREQ", NULL, 1 },
  { "UNIMPREQ", NULL, 2 },
  { "ERROR", NULL, 3 },
  { "INVALCONT", NULL, 4 },
  { "TLBERR", NULL, 16 },
  { "ADRERR", NULL, 17 },
  { "BUSERR", NULL, 18 },
  { "INVALSTATE", NULL, 32 },
  { "BREAKED", NULL, 33 },
  { "BRKPT", NULL, 34 },
  { "STEPNEXT", NULL, 35 },
  { "EXCEPTION", NULL, 36 },
  { "PROGEND", NULL, 37 },
  { "BUSYSTATE", NULL, 38 },
  { "DEBUG_EXCEPTION", NULL, 39 },
  { "TIMEOUT", NULL, 40 }
};

static STRS DR_LONG_strs[17] =
{
  { "Good", NULL, 0 },
  { "Invalid Request", NULL, 1 },
  { "Unimplemented Request", NULL, 2 },
  { "Error", NULL, 3 },
  { "Invalid Continue", NULL, 4 },
  { "TLB Error", NULL, 16 },
  { "Address Error", NULL, 17 },
  { "Bus Error", NULL, 18 },
  { "Invalid State", NULL, 32 },
  { "Breaked", NULL, 33 },
  { "Breakpoint", NULL, 34 },
  { "Step or Next", NULL, 35 },
  { "Exception", NULL, 36 },
  { "End of Program", NULL, 37 },
  { "Busy State", NULL, 38 },
  { "Debug Exception", NULL, 39 },
  { "Time Out", NULL, 40 }
};

static STRS DS_strs[4] =
{
  { "MAIN_MEM", NULL, 0 },
  { "VU_MEM", NULL, 1 },
  { "VU_UMEM", NULL, 2 },
  { "GS", NULL, 3 }
};

static STRS DA_strs[6] =
{
  { "BYTE", NULL, 0 },
  { "HALF", NULL, 1 },
  { "WORD", NULL, 2 },
  { "TWIN", NULL, 3 },
  { "QUAD", NULL, 4 },
  { "1KB", NULL, 10 }
};

static STRS DK_I_strs[10] =
{
  { "RSV", NULL, 0 },
  { "HL", NULL, 1 },
  { "GPR", NULL, 2 },
  { "SCC", NULL, 3 },
  { "C1R", NULL, 4 },
  { "C1C", NULL, 5 },
  { "C2R", NULL, 6 },
  { "C2C", NULL, 7 },
  { "C3R", NULL, 8 },
  { "C3C", NULL, 9 }
};

static STRS DK_E_strs[11] =
{
  { "GPR", NULL, 0 },
  { "HLS", NULL, 1 },
  { "SCR", NULL, 2 },
  { "PCR", NULL, 3 },
  { "HDR", NULL, 4 },
  { "FPR", NULL, 5 },
  { "FPC", NULL, 6 },
  { "V0F", NULL, 7 },
  { "V0I", NULL, 8 },
  { "V1F", NULL, 9 },
  { "V1I", NULL, 10 }
};

static STRS IC_strs[13] =
{
  { "START", NULL, 0 },
  { "STARTR", NULL, 1 },
  { "REMOVE", NULL, 2 },
  { "REMOVER", NULL, 3 },
  { "LIST", NULL, 4 },
  { "LISTR", NULL, 5 },
  { "INFO", NULL, 6 },
  { "INFOR", NULL, 7 },
  { "WATCH", NULL, 8 },
  { "WATCHR", NULL, 9 },
  { "MEMLIST", NULL, 10 },
  { "MEMLISTR", NULL, 11 },
  { "REPORT", NULL, 16 }
};

static STRS IR_N_strs[10] =
{
  { "OK", NULL, 0 },
  { "LINKERR", NULL, 1 },
  { "NOMEM", NULL, 2 },
  { "OBJERR", NULL, 3 },
  { "NOMOD", NULL, 4 },
  { "ILLCMD", NULL, 5 },
  { "BUSY", NULL, 6 },
  { "NOFILE", NULL, 7 },
  { "FILERR", NULL, 8 },
  { "MEMINUSE", NULL, 9 }
};

static STRS IR_N_LONG_strs[10] =
{
  { "Ok", NULL, 0 },
  { "Link Failed", NULL, 1 },
  { "No Memory", NULL, 2 },
  { "Invalid Object Format", NULL, 3 },
  { "Module not found", NULL, 4 },
  { "Invalid Command", NULL, 5 },
  { "Manager Busy", NULL, 6 },
  { "File not found", NULL, 7 },
  { "File read error", NULL, 8 },
  { "Memory in use", NULL, 9 }
};

static STRS IR_R_strs[4] =
{
  { "LOADED", NULL, 1 },
  { "RUNNING", NULL, 2 },
  { "REMOVED", NULL, 3 },
  { "RESIDENT", NULL, 4 }
};

static STRS EC_strs[11] =
{
  { "START", NULL, 0 },
  { "STARTR", NULL, 1 },
  { "REMOVE", NULL, 2 },
  { "REMOVER", NULL, 3 },
  { "LIST", NULL, 4 },
  { "LISTR", NULL, 5 },
  { "INFO", NULL, 6 },
  { "INFOR", NULL, 7 },
  { "WATCH", NULL, 8 },
  { "WATCHR", NULL, 9 },
  { "REPORT", NULL, 16 }
};

static STRS ER_N_strs[10] =
{
  { "OK", NULL, 0 },
  { "LINKERR", NULL, 1 },
  { "NOMEM", NULL, 2 },
  { "OBJERR", NULL, 3 },
  { "NOMOD", NULL, 4 },
  { "ILLCMD", NULL, 5 },
  { "BUSY", NULL, 6 },
  { "NOFILE", NULL, 7 },
  { "FILERR", NULL, 8 },
  { "MEMINUSE", NULL, 9 }
};

static STRS ER_N_LONG_strs[10] =
{
  { "Ok", NULL, 0 },
  { "Link Failed", NULL, 1 },
  { "No Memory", NULL, 2 },
  { "Invalid Object Format", NULL, 3 },
  { "Module not found", NULL, 4 },
  { "Invalid Command", NULL, 5 },
  { "Manager Busy", NULL, 6 },
  { "File not found", NULL, 7 },
  { "File read error", NULL, 8 },
  { "Memory in use", NULL, 9 }
};

static STRS ER_R_strs[4] =
{
  { "LOADED", NULL, 1 },
  { "RUNNING", NULL, 2 },
  { "REMOVED", NULL, 3 },
  { "RESIDENT", NULL, 4 }
};

FMT_STRS fmt_strs[] =
{
  { "0x%02x", "SN", SN_strs, 3 },
  { "0x%04x", "SP", SP_strs, 42 },
  { "0x%02x", "CT", CT_strs, 4 },
  { "0x%02x", "CC_CO", CC_CO_strs, 4 },
  { "0x%02x", "CE_CO", CE_CO_strs, 4 },
  { "0x%02x", "CC_EC", CC_EC_strs, 2 },
  { "0x%02x", "CC_ST", CC_ST_strs, 5 },
  { "0x%02x", "CC_ER", CC_ER_strs, 6 },
  { "0x%02x", "CC_ER_LONG", CC_ER_LONG_strs, 6 },
  { "0x%08x", "FC", FC_strs, 10 },
  { "0x%02x", "MC", MC_strs, 16 },
  { "0x%02x", "ME", ME_strs, 7 },
  { "0x%04x", "DI", DI_strs, 3 },
  { "0x%02x", "DT", DT_strs, 35 },
  { "0x%02x", "DC_C", DC_C_strs, 4 },
  { "0x%02x", "DR", DR_strs, 17 },
  { "0x%02x", "DR_LONG", DR_LONG_strs, 17 },
  { "0x%02x", "DS", DS_strs, 4 },
  { "0x%02x", "DA", DA_strs, 6 },
  { "0x%02x", "DK_I", DK_I_strs, 10 },
  { "0x%02x", "DK_E", DK_E_strs, 11 },
  { "0x%02x", "IC", IC_strs, 13 },
  { "0x%02x", "IR_N", IR_N_strs, 10 },
  { "0x%02x", "IR_N_LONG", IR_N_LONG_strs, 10 },
  { "0x%02x", "IR_R", IR_R_strs, 4 },
  { "0x%02x", "EC", EC_strs, 11 },
  { "0x%02x", "ER_N", ER_N_strs, 10 },
  { "0x%02x", "ER_N_LONG", ER_N_LONG_strs, 10 },
  { "0x%02x", "ER_R", ER_R_strs, 4 },
  { NULL, NULL, NULL, 0 }
};
