#include "dsnet_definitions.h"

// Common definitions
extern void (*ds_drfp_err_func)(DSP_BUF *db);
// (Just going to put the following in cmds.c)
extern struct _GIF_OPT gif;
extern struct _GIF_OPT *pgif;
extern struct _SMEM_OPT smem;
extern struct _SMEM_OPT *psmem;
extern int m_result;
extern unsigned int m_sdram;
extern unsigned int m_init;

// root/dsnet/lib/desc.c

extern DS_DESC_LIST ds_select_list;

extern DS_DESC *ds_add_select_list(
        int type,
        int fd,
        char *name,
        int (*accept_func)(DS_DESC *desc),
        DS_RECV_FUNC *recv_func);
extern DS_RECV_FUNC_DESC *ds_add_recv_func(DS_DESC *desc, int proto, int type, int code, DS_RECV_FUNC *func);
extern DS_RECV_FUNC_DESC *ds_del_recv_func(DS_DESC *desc, DS_RECV_FUNC_DESC *rf);
extern DS_DESC *ds_close_desc(DS_DESC *desc);
extern int ds_reset_info(DS_DESC *desc);
extern int ds_select_desc(int sec, int usec);
extern DS_DESC *ds_desc_by_proto(int proto);
extern int ds_isbusy_desc(int pri, int proto);
extern DSP_BUF *ds_send_net(int proto, DSP_BUF *db);
extern DSP_BUF *ds_send_desc(DS_DESC *desc, DSP_BUF *db);
extern void ds_flush_desc_by_proto(int proto);
extern void ds_status_desc();

// root/dsnet/lib/netmp.c

extern int ds_send_netmp(DS_DESC *desc, int code, int result, void *ptr, int len);
extern int ds_send_netmp1(DS_DESC *desc, int code, int result, int pri, int proto);
extern int ds_reset(DS_DESC *desc, void *ptr, int len);
extern int ds_poweroff(DS_DESC *desc);
extern int ds_send_netmp_msg(DS_DESC *desc, char *msg);
extern int ds_send_netmp_default_msg(DS_DESC *desc);
extern int ds_send_netmp_status_reply(DS_DESC *desc);

// root/dsnet/lib/deci2.c

extern unsigned char ds_sid;

extern DSP_BUF *ds_alloc_buf(int proto, int did, void *ptr, int len);
extern DSP_BUF *ds_free_buf(DSP_BUF *db);
extern DSP_BUF *ds_dup_buf(DSP_BUF *db);

// root/dsnet/lib/file.c

extern int ds_ioctl(int fd, int cmd, void *arg);
extern void *ds_fopen(char *fname, char *mode);
extern void *ds_fopen_low(char *fname, char *mode);
extern int ds_fclose(void *stream);
extern int ds_fread(void *ptr, int size, int n, void *stream, int f_allow_eof);
extern int ds_fwrite(void *ptr, int size, int n, void *stream);
extern int ds_fseek(void *stream, int offset, int whence);
extern void *ds_fload(void *stream, int whence, int off, int siz, int cnt);
extern int ds_fsize(char *fname, int *psize);
extern void *ds_popen(char *cmd, char *type);
extern int ds_pclose(void *stream);
extern int ds_access(char *fname, int rmode);

// root/dsnet/lib/kbd.c

extern int ds_raw_kbd();
extern int ds_resume_kbd();
extern DS_DESC *ds_open_kbd(DS_RECV_FUNC *recv_func, int f_prompt);

// root/dsnet/lib/log.c

extern void ds_add_log(DS_DESC *desc, char *msg, DECI2_HDR *dh);
extern int ds_log_cmd(int ac, char **av);

// root/dsnet/lib/mem.c

#define ds_alloc(_s) (ds_alloc_mem_low(__FILE__, __FUNCTION__, (_s)))
#define ds_free(_p) (ds_free_mem_low((_p), __FILE__, __FUNCTION__))

extern void ds_status_mem();
extern void *ds_alloc_mem_low(const char *file, const char *func, int size);
extern void *ds_free_mem_low(void *ptr, const char *file, const char *func);

// root/dsnet/lib/misc.c

extern char *ds_program_name;
extern int (*ds_rdwr_mem_align_func)(int code, int align, int cpuid, int space, unsigned int adr, void *ptr, int len);
extern int (*ds_load_mem_func)(unsigned int adr, void *ptr, int len);
extern int (*ds_store_mem_func)(unsigned int adr, void *ptr, int len);
extern int (*ds_load_quad_reg_func)(char *name, quad *pq);
extern int (*ds_store_quad_reg_func)(char *name, quad qv);
extern int (*ds_symbol_to_value_func)(char *name, unsigned int *pv);
extern int (*ds_symbol_completion_func)(DS_HISTBUF *hb, char *name);
extern int (*ds_help_completion_func)(DS_HISTBUF *hb, char *str);
extern void (*ds_idle_func)();
extern int (*ds_check_reserved_name_func)(char *name);

extern int ds_rdwr_mem_align(int code, int align, int cpuid, int space, unsigned int adr, void *ptr, int len);
extern int ds_load_mem(unsigned int adr, void *ptr, int len);
extern int ds_store_mem(unsigned int adr, void *ptr, int len);
extern int ds_load_quad_reg(char *name, quad *pq);
extern int ds_store_quad_reg(char *name, quad qv);
extern int ds_symbol_to_value(char *name, unsigned int *pv);
extern int ds_exit(int code);
extern int ds_exit_invalid_version(char *manager_version, int n, char *client_version);
extern int ds_fork();
extern int ds_cmd_execution_for_filesv(char *cmd, int *pstatus);
extern int ds_read(int fd, void *ptr, int n);
extern int ds_write(int fd, void *ptr, int n);
extern char *ds_basename(char *name);
extern char *ds_getenv(char *env);
extern void ds_bzero(void *ptr, int len);
extern int ds_gettime(int64_t *psec, int64_t *pusec);
extern char *ds_strchr(char *str, int ch);
extern char *ds_strcat(char *dst, char *src);
extern int ds_strncmp(char *s1, char *s2, int n);
extern char *ds_tilde_expand(char *buf, char *str);
extern int ds_gethostname(char *hostname, int n);
extern char *ds_getcwd(char *buf, int size);
extern char *ds_abs_path(char *buf, int len, char *fname);
extern DS_DESC *ds_open_comport(char *name, DS_RECV_FUNC *recv_func);
extern DS_DESC *ds_open_netdev(char *name, DS_RECV_FUNC *recv_func);
extern int ds_comp_main(char *device, int escape);

extern int ds_errno;

// root/dsnet/lib/net.c

extern DS_DESC *ds_connect_net(char *targetp, DS_RECV_FUNC *recv_func);
extern DS_DESC *ds_listen_net(char *portp, int (*accept_func)(DS_DESC *desc));
extern DS_DESC *ds_accept(int s, int (*accept_func)(DS_DESC *desc));

// root/dsnet/lib/printf.c

extern DS_DESC *ds_disp_desc;
extern int ds_disp_proto;

extern int ds_printf(char *fmt, ...);
extern int ds_sprintf(char *buf, char *fmt, ...);
extern int ds_error(char *fmt, ...);
extern int ds_printf_cmd(int ac, char **av);
extern int ds_record_cmd(int ac, char **av);

// root/dsnet/lib/psnet.c

extern void ds_free_psnet(DS_DESC *desc);
extern int ds_read_psnet(DS_DESC *desc, char *buf, int len);
extern int ds_write_psnet(DS_DESC *desc, char *buf, int len);

// root/dsnet/lib/quad.c

extern quad ds_qzero;

extern twin ds_cwt(unsigned int v);
extern unsigned int ds_ctw(twin x);
extern quad *ds_cwq(quad *retstr, unsigned int v);
extern unsigned int ds_cqw(quad x);
extern quad *ds_ctq(quad *retstr, twin v);
extern twin ds_cqt(quad x);
extern quad *ds_qneg(quad *retstr, quad x);
extern quad *ds_qor(quad *retstr, quad x, quad y);
extern quad *ds_qxor(quad *retstr, quad x, quad y);
extern quad *ds_qand(quad *retstr, quad x, quad y);
extern int ds_qblt(quad x, quad y);
extern int ds_qble(quad x, quad y);
extern int ds_qbgt(quad x, quad y);
extern int ds_qbge(quad x, quad y);
extern int ds_qbeq(quad x, quad y);
extern quad *ds_qshr(quad *retstr, quad x, quad y);
extern quad *ds_qshl(quad *retstr, quad x, quad y);
extern quad *ds_qadd(quad *retstr, quad x, quad y);
extern quad *ds_qsub(quad *retstr, quad x, quad y);
extern quad *ds_qmul(quad *retstr, quad x, quad y);
extern quad *ds_qdiv(quad *retstr, quad x, quad y);
extern quad *ds_qmod(quad *retstr, quad x, quad y);

// root/dsnet/lib/scans.c

extern int ds_scan_proto(char *str, int *pv);
extern int ds_scan_nid(char *str, int *pv);
extern int ds_scan_string(char *buf, char *str);
extern char *ds_decode_args(int ac, char **av);
extern int ds_scan_digit_word(char *s, unsigned int *pv);
extern int ds_scan_digit_quad(char *s, quad *pv);
extern int ds_scan_hex_quad(char *str, quad *pv);
extern int ds_scan_hex_word(char *str, unsigned int *pv);
extern int ds_scan_symbol_or_hex_quad(char *str, quad *pq);
extern int ds_scan_esc(char *p, int *pv);
extern char *ds_scan_args(char *s, int *pac, char **av, int max_args);

// root/dsnet/lib/cmds.c

extern DS_OPTION *ds_opt_hex_radix;
extern DS_OPTION *ds_opt_log_total_size;
extern DS_OPTION *ds_opt_log_packet_size;
extern DS_OPTION *ds_opt_histfile;
extern DS_OPTION *ds_opt_histfilesize;
extern DS_OPTION *ds_opt_histsize;
extern DS_OPTION *ds_opt_tty_max_size;
extern int ds_abort_input;
extern int ds_cmd_executing;

extern int ds_shell_cmd(int ac, char **av);
extern char *ds_ref_option_str(char *name);
extern int ds_ref_user_variable(char *name, unsigned int *wp);
extern DS_OPTION *ds_set_option(char *name, int type, char *str, int val, int f_def);
extern int ds_set_cmd(int ac, char **av);
extern int ds_read_option_file();
extern int ds_read_startup_file();
extern int ds_source_cmd(int ac, char **av);
extern int ds_cd_cmd(int ac, char **av);
extern int ds_if_cmd(int ac, char **av);
extern int ds_repeat_cmd(int ac, char **av);
extern int ds_add_show_arg(char *name, int (*func)(int ac, char **av));
extern int ds_show_cmd(int ac, char **av);
extern int ds_quit_cmd(int ac, char **av);
extern int ds_add_help(char *name, int (*func)(char *name));
extern int ds_help_cmd(int ac, char **av);
extern void ds_cmd_install(char *name, char *arg, char *help, int (*func)(int ac, char **av));
extern void ds_cmd_control(char *name, int f_enable);
extern void ds_opt_standard_init();
extern void ds_cmd_standard_install(int f_shell);
extern void ds_cmd_xxxable_install();
extern int ds_alias_cmd(int ac, char **av);
extern int ds_cmd_call(int ac, char **av, int f_repeat);
extern int ds_cmd_execute(char *s, int f_repeat);
extern int ds_cmd_completion(DS_HISTBUF *hb, char *s);
extern DSP_BUF *ds_cmd_input(DS_DESC *desc, DSP_BUF *db);

extern DS_HISTBUF ds_histbuf;

// root/dsnet/lib/dump.c

extern void ds_dump(char *str, DECI2_HDR *dh, int n);

// root/dsnet/lib/editline.c

extern char *ds_additional_prompt_str;

extern int ds_prompt();
extern int ds_save_history();
extern int ds_editline(DS_HISTBUF *hb, int ch, char *rp);

// root/dsnet/lib/expr.c

extern int ds_eval_quad(char *str, quad *pv);
extern int ds_eval_twin(char *str, twin *pv);
extern int ds_eval_word(char *str, unsigned int *pv);
extern int ds_eval_word_ex(char *str, unsigned int *pv);
extern int ds_eval_float(char *str, float *pv);

// root/dsnet/lib/subst.c

extern char *ds_subst(char *dst, int dst_siz, char *src);
extern int ds_subst_cmd(int ac, char **av);

// root/dsnet/lib/dcmp.c (dsedb, dsidb, **dsnetm**, dssim, pacons)

extern int ds_send_dcmp_connect(DS_DESC *desc, int did, int result, unsigned int *param);
extern int ds_send_dcmp_status(DS_DESC *desc, int nid, int code, int proto);
extern int ds_send_dcmp_status_net(int code, int proto);
extern DSP_BUF *ds_send_dcmp_error(DS_DESC *desc, DSP_BUF *odb, int code);

// root/dsnet/lib/drfp.c (dsedb, dsefilesv, dsestart, dsidb, dsifilesv, dsistart, **dsnetm**, pacons)

extern int ds_drfp_opened_files;
extern void (*ds_drfp_err_func)(DSP_BUF *db);

extern DSP_BUF *ds_recv_drfp(DS_DESC *desc, DSP_BUF *db);

// fmt.c

extern FMT_STRS fmt_strs[];

// root/dsnet/lib/pa.c (dsnetm)

extern unsigned int ds_open_sam();
extern void set_dma(unsigned int phyAdr, unsigned int adrs, unsigned int bytes);
extern unsigned int ds_close_sam(unsigned int sam_fd);
extern unsigned int GetIOPAddr(unsigned int ras, unsigned int cas);
extern void ConvertIOP(unsigned __int8 *dst, unsigned __int8 *src, int linesize);
extern DSP_BUF *ds_recv_pamp(DS_DESC *desc, DSP_BUF *db);

// root/dsnet/lib/check_pfm.c (pacons)

extern int check_pfm(VERIFY_PFM *verify, char *p, int mode, int size, char *checkStr);
extern int reset_pfm(VERIFY_PFM *verify, char *checkStr);
extern int result_pfm(VERIFY_PFM *verify, int mode, char *checkStr);

// root/dsnet/lib/check_gif.c (pacons)

extern int DataFileRead1();
extern int DataFileRead2();
extern int MemoryFree1();
extern int MemoryFree2();
extern int check_gif(char *p, int mode, int size, char *checkStr);

// root/dsnet/lib/check_sss.c (pacons)

extern int SetBusType(VERIFY_SSS *verify, char **av);
extern int check_sss(VERIFY_SSS *verify, char *p, int size, char **av);
extern int result_sss(VERIFY_SSS *verify);
