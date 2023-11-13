#define _GNU_SOURCE
#include <regex.h>
#include <stdio.h>
#include "parse.h"
#include "grep.h"
#include <stdlib.h>
#include <string.h>


void add_line_break(char *line) {
    if (strstr(line, "\n") == 0) {
        putchar('\n');
    }
}



void print_name_file(int argc, char *argv, flags flag) {
    if (!flag.h) {
        if (flag.e || flag.f) {
            if ((argc - optind > 1)) printf("%s:", argv);
        } else {
            if ((argc - optind > 2)) printf("%s:", argv);
        }
    }
}



void print_line_and_line_break(char flag, char *line, value v) {
    switch (flag) {
        case '0':
            printf("%s", line);
            add_line_break(line);
            break;
        case 'n':
            printf("%d:%s", v.number, line);
            add_line_break(line);
            break;
        default:
            break;
    }
}


void grep(FILE *file, flags flag, char* pattern,int argc, char *argv) {

    char *line = NULL;
    size_t len = 0;
    regex_t reg;
    int err;

    regmatch_t pmatch;

    err = regcomp(&reg, pattern, REG_EXTENDED | (flag.i ? REG_ICASE : 0) | REG_NEWLINE);
    if (err != 0) {
        fprintf(stderr, "Regex compilation failed");
        return;
    }
    value v = {0};
    while (getline(&line, &len, file) != -1) {
        int printed = 0, printed_no = 0;
        (v.number)++;
        if ((regexec(&reg, line, 0, &pmatch, 0) == 0) && !flag.v) {
            (v.match_lines)++;
            (v.match_files) = 1;
            if (flag.c || flag.l) continue;
            if (flag.h && (!flag.o)) {
                flag_h(flag, line, v);
                continue;
            }
            if (flag.n && !flag.o) {
                flag_n(flag, argc, argv, line, v);
            }
            if ((!flag.o && !flag.n && !flag.h)) {
                grep_no_flags(flag, argc, argv, line);
            }
        } else if ((regexec(&reg, line, 1, &pmatch, 0) != 0) && flag.v) {
            v.invert_line++;
            v.match_files_invert = 1;
            if (flag.c || flag.l) {
                continue;
            }
            flag_v(flag, argc, argv, line, v);
        }
        if (flag.o) {
            int add = 0;
            while (regexec(&reg, line + add, 1, &pmatch, 0) == 0) {
                int start = pmatch.rm_so + add, end = pmatch.rm_eo + add;
                add = flag_o(flag, argc, argv, &printed, &printed_no, start, end, line, v);
            }
        }
    }
    if (flag.c) flag_c(flag, argc, argv,v);
    if ((flag.l && flag.v &&  v.invert_line > 0) ||
        (flag.l && v.match_lines > 0))
        printf("%s\n", argv);
    free(line);
    regfree(&reg);
}




void flag_n(flags flag, int argc, char *argv, char *line, value v) {
    print_name_file(argc, argv, flag);
    print_line_and_line_break('n', line, v);
}



void flag_h(flags flag,char* line,value v){
    if (flag.n) printf("%d:", v.number);
    print_line_and_line_break('0',line,v);
}


void grep_no_flags(flags flag, int argc, char *argv, char *line) {
    print_name_file(argc, argv, flag);
    printf("%s", line);
    add_line_break(line);
}


void flag_v(flags flag, int argc, char* argv, char* line, value v){
    print_name_file(argc,argv,flag);
    if(flag.n && (!flag.o)){
        print_line_and_line_break('n', line,v);
    }else{
        print_line_and_line_break('0', line,v);
    }
}

void flag_c(flags flag, int argc, char *argv, value v) {
    print_name_file(argc, argv, flag);
    if (flag.l && flag.v) {
        ((v.match_files_invert == 1) ? printf("1\n") : printf("0\n"));
    } else if (flag.l) {
        ((v.match_files == 1) ? printf("1\n") : printf("0\n"));
    } else if (flag.v) {
        printf("%d\n", v.invert_line);
    } else {
        printf("%d\n", v.match_lines);
    }
}




int flag_o(flags flag, int argc, char *argv, int *printed, int *printed_no,
           int start, int end, char *line, value v) {
    if (flag.h)
        ;
    else if (flag.e) {
        if ((argc - optind > 1) && !(*printed)) {
            printf("%s:", argv);
            (*printed) = 1;
        }
    } else if ((argc - optind > 2) && !(*printed)) {
        printf("%s:", argv);
        (*printed) = 1;
    }

    if (flag.n && !(*printed_no)) {
        (*printed_no) = 1;
        printf("%d:", v.number);
        for (int i = start; i < end; i++) {
            printf("%c", line[i]);
        }
    } else {
        for (int i = start; i < end; i++) {
            printf("%c", line[i]);
        }
    }
    printf("\n");
    return end;
}