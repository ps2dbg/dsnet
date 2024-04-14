
#include "dsnet_prototypes.h"

// root/dsnet/dbg/dbg.c

#ifdef DSNET_COMPILING_E
extern int get_handlerlist(DBGP_HDR *phdr);
extern int get_tcb(DBGP_HDR *phdr, int tid);
extern int get_thread_list(DBGP_HDR *phdr);
extern int get_semablock(DBGP_HDR *phdr, int sid);
extern int load_quad_registers(unsigned int *masks, quad *pv, int n);
extern int store_quad_registers(unsigned int *masks, quad *pv, int n);
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
extern int load_word_registers(unsigned int *masks, unsigned int *pv, int n);
extern int store_word_registers(unsigned int *masks, unsigned int *pv, int n);
#endif /* DSNET_COMPILING_I */
extern int cont_and_wait_halt(int code, int cnt);
extern int run_and_wait_halt(unsigned int entry_point, unsigned int gp_value, int argc, char *args, int args_len);
extern int break_and_wait_halt();
extern int wait_halt();
extern int get_brkpt(DBGP_BRKPT_DATA *bps, int *pn);
extern int put_brkpt(DBGP_BRKPT_DATA *bps, int n);
extern int is_target_is_running();
extern int send_iload_and_wait(int cmd, int action, unsigned int id, void *ptr, int len, int nodisp);
extern int is_target_exec_ctl();
extern int is_automatic_prefix_breakpoint();
extern int is_describe_ub_all();
extern int is_lstep_stop_at_no_line();
extern void di_format(int *f_adr, int *f_iw, int *f_ba, int *f_ma);
extern char *dr_format_str();
extern char *source_directories_str();
extern int get_help_lang();
extern char *get_help_pager();
extern void dr_default_di();
extern void ex_default_dr();
#ifdef DSNET_COMPILING_E
extern void dr0_default_di();
extern void ex0_default_dr();
extern void dr1_default_di();
extern void ex1_default_dr();
#endif /* DSNET_COMPILING_E */
extern void lstep_default_list();
extern int main(int ac, char **av);
extern int IsSupported(int MajorVersion, int MinorVersion);

// root/dsnet/dbg/mem.c

extern int dmem_cmd(int ac, char **av);
extern // local variable allocation has failed, the output may be wrong!
int smem_cmd(int ac, char **av);
extern int inp_cmd(int ac, char **av);
extern // local variable allocation has failed, the output may be wrong!
int out_cmd(int ac, char **av);
extern int bload_cmd(int ac, char **av);
extern int bsave_cmd(int ac, char **av);
extern int pload_cmd(int ac, char **av);

#ifdef DSNET_COMPILING_E
// root/dsnet/dbg/ereg.c
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
// root/dsnet/dbg/ireg.c
#endif /* DSNET_COMPILING_I */

extern int check_reserved_name(char *name);
#ifdef DSNET_COMPILING_I
extern int load_word_reg(char *name, unsigned int *pv);
extern int store_word_reg(char *name, unsigned int val);
#endif /* DSNET_COMPILING_I */
extern int load_quad_reg(char *name, quad *pv);
extern int store_quad_reg(char *name, quad val);
extern int dreg_cmd(int ac, char **av);
extern int sreg_cmd(int ac, char **av);
extern int hbp_cmd(int ac, char **av);
extern int rload_cmd(int ac, char **av);
extern int rsave_cmd(int ac, char **av);

// root/dsnet/dbg/exe.c

extern int store_user_breakpoints();
extern int restore_user_breakpoints();
extern int eval_bp_reg(char *name, unsigned int *padr);
extern int remove_breakpoints();
extern int bp_cmd(int ac, char **av);
extern int ub_cmd(int ac, char **av);
extern int be_cmd(int ac, char **av);
extern int bd_cmd(int ac, char **av);
extern int break_cmd(int ac, char **av);
extern int wait_cmd(int ac, char **av);
extern void display_current_informations(int result);
#ifdef DSNET_COMPILING_E
extern int set_runarg(int ac, char **av);
extern int run_cmd(int ac, char **av);
#endif /* DSNET_COMPILING_E */
extern int cont_cmd(int ac, char **av);
extern int until_cmd(int ac, char **av);
extern int step_cmd(int ac, char **av);
extern int next_cmd(int ac, char **av);
extern int lstep_cmd(int ac, char **av);
extern int lnext_cmd(int ac, char **av);
extern int luntil_cmd(int ac, char **av);

// root/dsnet/dbg/opc.c

extern int is_branch_instruction(unsigned int adr, unsigned int ins, unsigned int *ptadr);
extern int di_cmd(int ac, char **av);
extern int as_cmd(int ac, char **av);
extern int eval_bt_reg(char *name, unsigned int *padr);
extern int eval_sfa_reg(char *name, unsigned int *padr);
extern int eval_sfs_reg(char *name, unsigned int *padr);
extern void disp_bt(BT_REG *br, int cnt);
extern int bt_cmd(int ac, char **av);

// root/dsnet/dbg/sym.c

extern void clear_module_symbol();
extern void clear_module_symbol_with_name(char *name);
extern void add_module_symbol(char *name, int adr, int siz, int id);
extern void clear_symbol();
extern void clear_symbol_with_id(int id);
extern int match_symbol(char *str, int nstr, char *name);
extern int module_base(int id, int base, int shndx, int info);
extern int show_symbol(int ac, char **av);
#ifdef DSNET_COMPILING_E
extern int look_eemod(
#endif /* DSNET_COMPILING_E */
#ifdef DSNET_COMPILING_I
extern int look_iopmod(
#endif /* DSNET_COMPILING_I */
        void *stream,
        DS_ELF_EHDR *ehdr,
        DS_ELF_SHDR *shdr,
        int id,
        int base,
        void (*clear_func)());
extern int load_symbol(void *stream, DS_ELF_EHDR *ehdr, DS_ELF_SHDR *shdr, int symndx, int strndx, int id, int base);
extern int address_to_func(unsigned int adr, unsigned int *padr0, unsigned int *padr1);
extern int address_to_symbol(char *buf, unsigned int adr, int force_delta);
extern int symbol_to_value(char *name, unsigned int *pv);
extern int symbol_completion(DS_HISTBUF *hb, char *name);

// root/dsnet/dbg/mod.c

extern int mod_id_by_name(int id, char *name, int ver);
extern int mod_fetch_id(int id);
extern unsigned int mod_address_by_id(int id);
extern void mod_set_vaddr(int id, unsigned int vaddr);
extern void clear_mod_list();
extern int iload_callback(unsigned int id, int cmd, void *ptr, int len);
extern int mload_cmd(int ac, char **av);
extern int mstart_cmd(int ac, char **av);
extern int mremove_cmd(int ac, char **av);
extern int mlist_cmd(int ac, char **av);
extern int memlist_cmd(int ac, char **av);

#ifdef DSNET_COMPILING_E
// root/dsnet/dbg/vopc.c

extern int vdi_cmd(int ac, char **av);
#endif /* DSNET_COMPILING_E */

// root/dsnet/dbg/help.c

extern int dbg_help(char *name);
extern int dbg_help_completion(DS_HISTBUF *hb, char *name);

// root/dsnet/dbg/mdebug.c

extern void clear_mdebug();
extern void clear_mdebug_with_id(int id);
extern int show_mdebug(int ac, char **av);
extern int load_mdebug(
        void *stream,
        DS_ELF_EHDR *elf_header,
        DS_ELF_SHDR *section_header,
        int mindex,
        int id,
        int base,
        char *path);
extern unsigned int file_and_line_to_address(int line, char *path);
extern char *address_to_file_and_line(unsigned int loc, int *pline, int *f_has, int *pdelta, char **ppath);
extern int symbol_to_value_by_mdebug(char *file, char *name, unsigned int *pv);
extern char *string_by_file_and_line(char *fname, int line, char *obj_path);
extern int list_cmd(int ac, char **av);

#ifdef DSNET_COMPILING_E
// root/dsnet/dbg/ethread.c

extern int dt_cmd(int ac, char **av);
extern void disp_tcb(DBGP_HDR *hdr, int fmode);
extern void disp_thread_list_all(DBGP_HDR *hdr);
extern void disp_thread_list(DBGP_EE_THREADLIST_DATA *pdata);
extern int ds_cmd(int ac, char **av);
extern int intr_cmd(int ac, char **av);
#endif /* DSNET_COMPILING_E */
