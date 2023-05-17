
#define TARGET_EE
#include "dsnet_prototypes.h"

// root/dsnet/dbg/dbg.c

extern int __cdecl get_handlerlist(DBGP_HDR *phdr);
extern int __cdecl get_tcb(DBGP_HDR *phdr, int tid);
extern int __cdecl get_thread_list(DBGP_HDR *phdr);
extern int __cdecl get_semablock(DBGP_HDR *phdr, int sid);
extern int __cdecl load_quad_registers(unsigned int *masks, quad *pv, int n);
extern int __cdecl store_quad_registers(unsigned int *masks, quad *pv, int n);
extern int __cdecl cont_and_wait_halt(int code, int cnt);
extern int __cdecl run_and_wait_halt(unsigned int entry_point, unsigned int gp_value, int argc, char *args, int args_len);
extern int __cdecl break_and_wait_halt();
extern int __cdecl wait_halt();
extern int __cdecl get_brkpt(DBGP_BRKPT_DATA *bps, int *pn);
extern int __cdecl put_brkpt(DBGP_BRKPT_DATA *bps, int n);
extern int __cdecl is_target_is_running();
extern int __cdecl send_iload_and_wait(int cmd, int action, unsigned int id, void *ptr, int len, int nodisp);
extern int __cdecl is_target_exec_ctl();
extern int __cdecl is_automatic_prefix_breakpoint();
extern int __cdecl is_describe_ub_all();
extern int __cdecl is_lstep_stop_at_no_line();
extern void __cdecl di_format(int *f_adr, int *f_iw, int *f_ba, int *f_ma);
extern char *__cdecl dr_format_str();
extern char *__cdecl source_directories_str();
extern int __cdecl get_help_lang();
extern char *__cdecl get_help_pager();
extern void __cdecl dr_default_di();
extern void __cdecl ex_default_dr();
extern void __cdecl dr0_default_di();
extern void __cdecl ex0_default_dr();
extern void __cdecl dr1_default_di();
extern void __cdecl ex1_default_dr();
extern void __cdecl lstep_default_list();
extern int __cdecl main(int ac, char **av);
extern int __cdecl IsSupported(int MajorVersion, int MinorVersion);

// root/dsnet/dbg/mem.c

extern int __cdecl dmem_cmd(int ac, char **av);
extern // local variable allocation has failed, the output may be wrong!
int __cdecl smem_cmd(int ac, char **av);
extern int __cdecl inp_cmd(int ac, char **av);
extern // local variable allocation has failed, the output may be wrong!
int __cdecl out_cmd(int ac, char **av);
extern int __cdecl bload_cmd(int ac, char **av);
extern int __cdecl bsave_cmd(int ac, char **av);
extern int __cdecl pload_cmd(int ac, char **av);

// root/dsnet/dbg/ereg.c

extern int __cdecl check_reserved_name(char *name);
extern int __cdecl load_quad_reg(char *name, quad *pv);
extern int __cdecl store_quad_reg(char *name, quad val);
extern int __cdecl dreg_cmd(int ac, char **av);
extern int __cdecl sreg_cmd(int ac, char **av);
extern int __cdecl hbp_cmd(int ac, char **av);
extern int __cdecl rload_cmd(int ac, char **av);
extern int __cdecl rsave_cmd(int ac, char **av);

// root/dsnet/dbg/exe.c

extern int __cdecl store_user_breakpoints();
extern int __cdecl restore_user_breakpoints();
extern int __cdecl eval_bp_reg(char *name, unsigned int *padr);
extern int __cdecl remove_breakpoints();
extern int __cdecl bp_cmd(int ac, char **av);
extern int __cdecl ub_cmd(int ac, char **av);
extern int __cdecl be_cmd(int ac, char **av);
extern int __cdecl bd_cmd(int ac, char **av);
extern int __cdecl break_cmd(int ac, char **av);
extern int __cdecl wait_cmd(int ac, char **av);
extern void __cdecl display_current_informations(int result);
extern int __cdecl set_runarg(int ac, char **av);
extern int __cdecl run_cmd(int ac, char **av);
extern int __cdecl cont_cmd(int ac, char **av);
extern int __cdecl until_cmd(int ac, char **av);
extern int __cdecl step_cmd(int ac, char **av);
extern int __cdecl next_cmd(int ac, char **av);
extern int __cdecl lstep_cmd(int ac, char **av);
extern int __cdecl lnext_cmd(int ac, char **av);
extern int __cdecl luntil_cmd(int ac, char **av);

// root/dsnet/dbg/opc.c

extern int __cdecl is_branch_instruction(unsigned int adr, unsigned int ins, unsigned int *ptadr);
extern int __cdecl di_cmd(int ac, char **av);
extern int __cdecl as_cmd(int ac, char **av);
extern int __cdecl eval_bt_reg(char *name, unsigned int *padr);
extern int __cdecl eval_sfa_reg(char *name, unsigned int *padr);
extern int __cdecl eval_sfs_reg(char *name, unsigned int *padr);
extern void __cdecl disp_bt(BT_REG *br, int cnt);
extern int __cdecl bt_cmd(int ac, char **av);

// root/dsnet/dbg/sym.c

extern void __cdecl clear_module_symbol();
extern void __cdecl clear_module_symbol_with_name(char *name);
extern void __cdecl add_module_symbol(char *name, int adr, int siz, int id);
extern void __cdecl clear_symbol();
extern void __cdecl clear_symbol_with_id(int id);
extern int __cdecl match_symbol(char *str, int nstr, char *name);
extern int __cdecl module_base(int id, int base, int shndx, int info);
extern int __cdecl show_symbol(int ac, char **av);
extern int __cdecl look_eemod(
        void *stream,
        DS_ELF_EHDR *ehdr,
        DS_ELF_SHDR *shdr,
        int id,
        int base,
        void (__cdecl *clear_func)());
extern int __cdecl load_symbol(void *stream, DS_ELF_EHDR *ehdr, DS_ELF_SHDR *shdr, int symndx, int strndx, int id, int base);
extern int __cdecl address_to_func(unsigned int adr, unsigned int *padr0, unsigned int *padr1);
extern int __cdecl address_to_symbol(char *buf, unsigned int adr, int force_delta);
extern int __cdecl symbol_to_value(char *name, unsigned int *pv);
extern int __cdecl symbol_completion(DS_HISTBUF *hb, char *name);

// root/dsnet/dbg/mod.c

extern int __cdecl mod_id_by_name(int id, char *name, int ver);
extern int __cdecl mod_fetch_id(int id);
extern unsigned int __cdecl mod_address_by_id(int id);
extern void __cdecl mod_set_vaddr(int id, unsigned int vaddr);
extern void __cdecl clear_mod_list();
extern int __cdecl iload_callback(unsigned int id, int cmd, void *ptr, int len);
extern int __cdecl mload_cmd(int ac, char **av);
extern int __cdecl mstart_cmd(int ac, char **av);
extern int __cdecl mremove_cmd(int ac, char **av);
extern int __cdecl mlist_cmd(int ac, char **av);
extern int __cdecl memlist_cmd(int ac, char **av);

// root/dsnet/dbg/vopc.c

extern int __cdecl vdi_cmd(int ac, char **av);

// root/dsnet/dbg/help.c

extern int __cdecl dbg_help(char *name);
extern int __cdecl dbg_help_completion(DS_HISTBUF *hb, char *name);

// root/dsnet/dbg/mdebug.c

extern void __cdecl clear_mdebug();
extern void __cdecl clear_mdebug_with_id(int id);
extern int __cdecl show_mdebug(int ac, char **av);
extern int __cdecl load_mdebug(
        void *stream,
        DS_ELF_EHDR *elf_header,
        DS_ELF_SHDR *section_header,
        int mindex,
        int id,
        int base,
        char *path);
extern unsigned int __cdecl file_and_line_to_address(int line, char *path);
extern char *__cdecl address_to_file_and_line(unsigned int loc, int *pline, int *f_has, int *pdelta, char **ppath);
extern int __cdecl symbol_to_value_by_mdebug(char *file, char *name, unsigned int *pv);
extern char *__cdecl string_by_file_and_line(char *fname, int line, char *obj_path);
extern int __cdecl list_cmd(int ac, char **av);

// root/dsnet/dbg/ethread.c

extern int __cdecl dt_cmd(int ac, char **av);
extern void __cdecl disp_tcb(DBGP_HDR *hdr, int fmode);
extern void __cdecl disp_thread_list_all(DBGP_HDR *hdr);
extern void __cdecl disp_thread_list(DBGP_EE_THREADLIST_DATA *pdata);
extern int __cdecl ds_cmd(int ac, char **av);
extern int __cdecl intr_cmd(int ac, char **av);
