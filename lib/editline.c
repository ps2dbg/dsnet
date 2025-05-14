#include "dsnet_prototypes.h"

static char isearch_buf[1024];
static char *isearch_ptr;
static char cbuf[PATH_MAX + 1];

char *ds_additional_prompt_str = "";
static int prompt_len = 0;
static int isearch_dir = 0;
static int cbuf_len = 0;

static void flush_char();
static int ds_load_history();
static void replace(DS_HISTBUF *hb, char *buf, int off);
static char *isearch(DS_HISTBUF *hb, int f_force, int *ppos);
static void print_char(int ch);

static void flush_char()
{
    if (cbuf_len > 0) {
        cbuf[cbuf_len] = 0;
        ds_printf("%s", cbuf);
    }
    cbuf_len = 0;
}

int ds_prompt()
{
    char *v0; // eax

    flush_char();
    ds_printf("\r");
    if (isearch_dir) {
        if (isearch_dir >= 0)
            v0 = "";
        else
            v0 = "reverse-";
        prompt_len = ds_printf("(%si-search)", v0);
        prompt_len += ds_printf(" `%s'", isearch_buf);
        prompt_len += ds_printf(": ");
        return prompt_len;
    } else {
        prompt_len = ds_printf("%s %s> ", ds_program_name, ds_additional_prompt_str);
        return prompt_len;
    }
}

int ds_save_history()
{
    int v1;      // eax
    int n;       // [esp+0h] [ebp-10h]
    int i;       // [esp+4h] [ebp-Ch]
    void *fp;    // [esp+8h] [ebp-8h]
    DS_HIST *hp; // [esp+Ch] [ebp-4h]

    if (!ds_opt_histfile || !*ds_opt_histfile->str || !ds_opt_histfilesize)
        return 0;

    n = ds_opt_histfilesize->val;
    hp = ds_histbuf.tail;
    if (!ds_histbuf.tail)
        return 0;

    fp = ds_fopen(ds_opt_histfile->str, "w");
    if (!fp)
        return -1;

    for (i = 1; n >= 0 && n > i && hp->back; ++i)
        hp = hp->back;

    while (hp) {
        v1 = strlen(hp->buf);
        if (ds_fwrite(hp->buf, v1, 1, fp) != 1)
            return -1;
        if (ds_fwrite("\n", 1, 1, fp) != 1)
            return -1;
        hp = hp->forw;
    }
    ds_fclose(fp);
    return 0;
}

static int ds_load_history()
{
    char *v1;                // eax
    DS_HISTBUF *v2;          // edx
    DS_HIST *v3;             // ebx
    DS_HIST *tail;           // ecx
    char path[PATH_MAX + 1]; // [esp+8h] [ebp-428h] BYREF
    int v6;                  // [esp+40Ch] [ebp-24h]
    int siz;                 // [esp+410h] [ebp-20h] BYREF
    char *q;                 // [esp+414h] [ebp-1Ch]
    char *pe;                // [esp+418h] [ebp-18h]
    char *p;                 // [esp+41Ch] [ebp-14h]
    char *buf;               // [esp+420h] [ebp-10h]
    void *fp;                // [esp+424h] [ebp-Ch]
    DS_HIST *hp;             // [esp+428h] [ebp-8h]
    DS_HISTBUF *hb;          // [esp+42Ch] [ebp-4h]

    hb = &ds_histbuf;
    if (!ds_opt_histfile || !*ds_opt_histfile->str)
        return 0;
    ds_tilde_expand(path, ds_opt_histfile->str);
    if (access(path, 4))
        return 0;
    fp = ds_fopen(path, "r");
    if (!fp)
        return -1;
    if (ds_fsize(path, &siz) < 0) {
        ds_fclose(fp);
        return -1;
    }

    buf = ds_fload(fp, 0, 0, 1, siz);
    ds_fclose(fp);

    if (!buf) {
        return -1;
    }
    p = buf;
    pe = &buf[siz];
    while (pe > p) {
        while (pe > p && *p != '\n' && (isspace(*p)) != 0)
            ++p;
        for (q = p; pe > q && *q != '\n'; ++q)
            ;
        v1 = q++;
        if (p != v1) {
            *(q - 1) = 0;
            v6 = strlen(p);
            hp = ds_alloc(v6 + sizeof(DS_HIST) + 5);
            if (!hp)
                return -1;
            v2 = hb;
            hp->no = hb->no;
            ++v2->no;
            strcpy(hp->buf, p);
            v3 = hp;
            tail = hb->tail;
            hp->back = tail;
            if (tail)
                hp->back->forw = v3;
            else
                hb->head = v3;
            hp->forw = 0;
            hb->tail = hp;
            hb->curr = hp;
        }
        p = q;
    }

    ds_free(buf);
    return 0;
}

static void replace(DS_HISTBUF *hb, char *buf, int off)
{
    int n;   // [esp+8h] [ebp-8h]
    char *p; // [esp+Ch] [ebp-4h]

    if (!hb->ptr) {
        hb->ptr = hb->buf;
        hb->buf[0] = 0;
    }

    for (p = hb->ptr; p > hb->buf; --p)
        print_char(8);

    n = prompt_len;
    while (0 < n) {
        print_char(8);
        n--;
    }

    n = prompt_len;
    while (0 < n) {
        print_char(32);
        n--;
    }

    while (*p) {
        print_char(32);
        p++;
    }

    ds_prompt();

    if (buf) {
        n = strlen(buf);
        p = memmove(hb->buf, buf, n);
        p[n + 1] = '\0';
        hb->ptr = p + off;
    }

    for (p = hb->buf; *p; ++p)
        print_char(*p);

    while (p > hb->ptr) {
        print_char(8);
        --p;
    }
}

static char *isearch(DS_HISTBUF *hb, int f_force, int *ppos)
{
    char *q;     // [esp+4h] [ebp-14h]
    char *q_1;   // [esp+4h] [ebp-14h]
    int dir;     // [esp+Ch] [ebp-Ch]
    int n;       // [esp+10h] [ebp-8h]
    DS_HIST *hp; // [esp+14h] [ebp-4h]

    n = strlen(isearch_buf);
    dir = isearch_dir;
    if (!f_force && !strncmp(isearch_buf, hb->ptr, n)) {
        *ppos = hb->ptr - hb->buf;
        return hb->buf;
    }

    if (n <= 0)
        return 0;

    if (f_force < 0)
        dir = -dir;

    hp = hb->curr;
    if (!hp) {
        if (dir <= 0) {
            hp = hb->tail;
            if (!hp)
                return 0;
        } else {
            hp = hb->head;
            if (!hb->head)
                return 0;
        }
    }

    q = &hp->buf[hb->ptr - hb->buf];
    if (dir <= 0) {
        if (!hb->buf[0])
            q = &hp->buf[strlen(hp->buf)];
        q_1 = q - 1;
        if (q_1 < hp->buf)
            goto LABEL_37;
        while (hp) {
            while (q_1 >= hp->buf && strncmp(isearch_buf, q_1, n))
                --q_1;
            if (q_1 >= hp->buf)
                break;
        LABEL_37:
            if (!hp)
                return 0;
            hp = hp->back;
            if (!hp)
                return 0;
            q_1 = &hp->buf[strlen(hp->buf) - 1];
        }
    } else {
        q_1 = q + 1;
        if (!hp->buf[hb->ptr - hb->buf])
            goto LABEL_23;
        while (hp) {
            while (*q_1 && strncmp(isearch_buf, q_1, n))
                ++q_1;
            if (*q_1)
                break;
        LABEL_23:
            if (!hp)
                return 0;
            hp = hp->forw;
            if (!hp)
                return 0;
            q_1 = hp->buf;
        }
    }

    hb->curr = hp;
    *ppos = q_1 - hp->buf;
    return hp->buf;
}

int ds_editline(DS_HISTBUF *hb, char ch, char *rp)
{
    char v8;       // cl
    char v9;       // cl
    DS_HIST *v10;  // esi
    DS_HIST *tail; // ecx
    DS_HIST *back; // edi
    DS_HIST *forw; // [esp+10h] [ebp-28h]
    int pos;       // [esp+14h] [ebp-24h] BYREF
    int r;         // [esp+18h] [ebp-20h]
    char *ptr;     // [esp+1Ch] [ebp-1Ch]
    char *buf;     // [esp+20h] [ebp-18h]
    char *i;       // [esp+24h] [ebp-14h]
    char *p;       // [esp+28h] [ebp-10h]
    char v;        // [esp+2Fh] [ebp-9h]
    DS_HIST *hp;   // [esp+30h] [ebp-8h]

    static int once = 1;
    static int cmdmode = 0;

    hp = hb->curr;
    buf = hb->buf;
    ptr = hb->ptr;
    if (once) {
        ds_load_history();
        once = 0;
    }

    if (!ptr) {
        ptr = buf;
        *buf = 0;
    }

    if (rp)
        *rp = 0;

    if (!cmdmode) {
        if (ch == 27) {
            cmdmode = 1;
            return 0;
        }
    } else {
        if (ch == 'x' && ptr > buf && !*ptr) {
            print_char(8);
            --ptr;
        }

        switch (ch) {
            case '$':
                ch = 5;
                break;
            case '/':
                ch = 18;
                break;
            case '0':
                ch = 1;
                break;
            case '?':
                ch = 19;
                break;
            case 'D':
                ch = 11;
                break;
            case 'h':
                ch = 2;
                break;
            case 'j':
                ch = 14;
                break;
            case 'k':
                ch = 16;
                break;
            case 'l':
                ch = 6;
                break;
            case 'p':
                ch = 25;
                break;
            case 'x':
                ch = 4;
                break;
            default:
                cmdmode = 0;
                if (ch > 31 || ch != 127) {
                    cmdmode = 0;
                    return 0;
                }
                break;
        }
    }

    if (isearch_dir == 0) {
        if (ch == 19 || ch == 18) {
            isearch_dir = ch == 19 ? 1 : -1;
            isearch_ptr = isearch_buf;
            isearch_buf[0] = 0;
            replace(hb, "", 0);

            flush_char();
            return ch;
        }
    } else {
        if (ch == 19 || ch == 18) {
            isearch_dir = ch == 19 ? 1 : -1;
            buf = isearch(hb, 1, &pos);
            if (buf) {
                replace(hb, buf, pos);
            }

            flush_char();
            return ch;
        }

        if (ch == 8 || ch == 127) {
            if (isearch_ptr > isearch_buf) {
                --isearch_ptr;
                *isearch_ptr = 0;
                buf = isearch(hb, -1, &pos);
                if (!buf) {
                    replace(hb, "", 0);
                } else {
                    replace(hb, buf, pos);
                }
            }

            flush_char();
            return ch;
        }

        if (ch > 31 && ch != 127) {
            if (isearch_ptr < &isearch_buf[sizeof(isearch_buf) - 1]) {
                *isearch_ptr++ = ch;
                *isearch_ptr = 0;
                buf = isearch(hb, 0, &pos);
                if (!buf) {
                    --isearch_ptr;
                    *isearch_ptr = 0;
                } else {
                    replace(hb, buf, pos);
                }
            }

            flush_char();
            return ch;
        }

        isearch_dir = 0;
        replace(hb, NULL, 0);
        hp = hb->curr;
        buf = hb->buf;
        ptr = hb->ptr;
    }

    switch (ch) {
        case 1:
            while (ptr > buf) {
                print_char(8);
                --ptr;
            }
            break;
        case 2:
            if (ptr > buf) {
                print_char(8);
                --ptr;
            }
            break;
        case 3:
            ds_printf("^C");
            //[[fallthrough]];
        case 7:
            ptr = buf;
            *buf = 0;
            break;
        case 4:
            if (*ptr) {
                for (p = ptr;; ++p) {
                    v9 = p[1];
                    *p = v9;
                    if (!v9)
                        break;
                    print_char(*p);
                }
                print_char(32);
                while (p >= ptr) {
                    print_char(8);
                    --p;
                }
            }
            break;
        case 5:
            while (*ptr)
                print_char(*ptr++);
            break;
        case 6:
            if (*ptr)
                print_char(*ptr++);
            break;
        case 8:
        case 127:
            if (ptr > buf) {
                print_char(8);
                for (p = --ptr;; ++p) {
                    v8 = p[1];
                    *p = v8;
                    if (!v8)
                        break;
                    print_char(*p);
                }
                print_char(32);
                while (p >= ptr) {
                    print_char(8);
                    --p;
                }
            }
            break;
        case 9:
            if (rp)
                strcpy(rp, hb->buf);
            break;
        case 10:
        case 13:
            r = strlen(buf);
            if (r > 0) {
                if (!hb->tail || strcmp(buf, hb->tail->buf)) {
                    hp = ds_alloc(r + sizeof(DS_HIST) + 5);
                    if (!hp) {
                        return -1;
                    }

                    hp->no = hb->no++;
                    strcpy(hp->buf, hb->buf);
                    v10 = hp;
                    tail = hb->tail;
                    hp->back = tail;
                    if (tail)
                        hp->back->forw = v10;
                    else
                        hb->head = v10;
                    hp->forw = 0;
                    hb->tail = hp;
                    if (ds_opt_histsize) {
                        r = ds_opt_histsize->val;
                        if (r >= 0) {
                            do {
                                if (r > hb->tail->no - hb->head->no)
                                    break;
                                hp = hb->head;
                                back = hp->back;
                                if (hp->forw)
                                    hp->forw->back = back;
                                else
                                    hb->tail = back;
                                forw = hp->forw;
                                if (hp->back)
                                    hp->back->forw = forw;
                                else
                                    hb->head = forw;
                                ds_free(hp);
                            } while (hb->head);
                        }
                    }
                }
                if (rp) {
                    strcpy(rp, hb->buf);
                }
            }
            ptr = buf;
            *buf = 0;
            hp = 0;
            break;
        case 12:
            ds_prompt();
            for (p = buf; *p; ++p)
                print_char(*p);
            while (p > ptr) {
                print_char(8);
                --p;
            }
            break;
        case 14:
            while (ptr > buf) {
                --ptr;
                print_char(8);
            }
            for (ptr = buf; *ptr; ++ptr)
                print_char(32);
            while (ptr > buf) {
                print_char(8);
                --ptr;
            }
            *ptr = 0;
            if (hp) {
                hp = hp->forw;
                if (hp) {
                    p = hp->buf;
                    while (1) {
                        *ptr = *p;
                        if (!*p)
                            break;
                        print_char(*p++);
                        ++ptr;
                    }
                }
            }
            break;
        case 16:
            while (ptr > buf) {
                --ptr;
                print_char(8);
            }
            for (ptr = buf; *ptr; ++ptr)
                print_char(32);
            while (ptr > buf) {
                print_char(8);
                --ptr;
            }
            *ptr = 0;
            if (hp) {
                if (hp->back)
                    hp = hp->back;
            } else {
                hp = hb->tail;
            }
            if (hp) {
                p = hp->buf;
                while (1) {
                    *ptr = *p;
                    if (!*p)
                        break;
                    print_char(*p++);
                    ++ptr;
                }
            }
            break;
        case 21:
        case 24:
            while (ptr > buf) {
                print_char(8);
                --ptr;
            }
            //[[fallthrough]];
        case 11:
            strcpy(hb->yank, ptr);
            for (p = ptr; *p; ++p) {
                *p = 32;
                print_char(*p);
            }
            *ptr = 0;
            while (p > ptr) {
                print_char(8);
                --p;
            }
            break;
        case 25:
            for (i = hb->yank;; ++i) {
                ch = *i;
                if (!ch)
                    break;
                for (p = ptr++; *p; ++p) {
                    print_char(ch);
                    v = *p;
                    *p = ch;
                    ch = v;
                }
                if (p >= buf + 1023) {
                    ptr = p;
                } else {
                    print_char(ch);
                    *p++ = ch;
                }
                *p = 0;
                while (p > ptr) {
                    print_char(8);
                    --p;
                }
            }
            ch = 25;
            break;
        default:
            if (ch > 31 && ch != 127) {
                for (p = ptr++; *p; ++p) {
                    print_char(ch);
                    v = *p;
                    *p = ch;
                    ch = v;
                }
                if (p >= buf + 1023) {
                    ptr = p;
                } else {
                    print_char(ch);
                    *p++ = ch;
                }
                *p = 0;
                while (p > ptr) {
                    print_char(8);
                    --p;
                }
            }
            break;
    }

    hb->curr = hp;
    hb->ptr = ptr;

    flush_char();
    return ch;
}

static void print_char(int ch)
{
    if ((unsigned int)cbuf_len > (sizeof(cbuf) - 2))
        flush_char();
    cbuf[cbuf_len++] = ch;
}
