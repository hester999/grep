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

void print_line_and_line_break(char flag, char *line, int number) {
    switch (flag) {
        case '0':
            printf("%s", line);
            add_line_break(line);
            break;
        case 'n':
            printf("%d:%s", number, line);
            add_line_break(line);
            break;
        default:
            break;
    }
}

void grep(FILE *new_f, char *sample, flags flag, int argc, char *argv) {
    char *line = NULL;
    size_t len = 0;
    regex_t preg;
    regmatch_t match;
    int res = 0;
    int err = regcomp(&preg, sample,
                      REG_EXTENDED | (flag.i ? REG_ICASE : 0) | REG_NEWLINE);
    if (err != 0)
        fprintf(stderr, "Error: compilation error");
    else {
        counter counts = {0};
        while ((res = getline(&line, &len, new_f)) != -1) {
            int printed = 0, printed_no = 0;
            counts.number = counts.number + 1;
            if ((regexec(&preg, line, 1, &match, 0) == 0) && !flag.v) {
                counts.count_matching_lines++;
                counts.matched_file = 1;
                if (flag.c || flag.l) continue;
                if (flag.h && !flag.o) {
                    flag_h(flag, line, counts);
                    continue;
                }
                if (flag.n && !flag.o)
                    flag_n(flag, argc, argv, line, counts);
                if ((!flag.o && !flag.n && !flag.h))
                    grep_no_flags(flag, argc, argv, line);
            } else if ((regexec(&preg, line, 1, &match, 0) != 0) && flag.v) {
                counts.invert_lines = counts.invert_lines + 1;
                counts.matched_file_invert = 1;
                if (flag.c || flag.l) continue;
                flag_v(flag, argc, argv, line, counts.number);
            }
            if (flag.o) {
                int add = 0;
                while (regexec(&preg, line + add, 1, &match, 0) == 0) {
                    int start = match.rm_so + add, end = match.rm_eo + add;
                    add = flag_o(flag, argc, argv, &printed, &printed_no, start, end,
                                 line, counts.number);
                }
            }
        }
        if (flag.c) flag_c(flag, argc, argv, counts);
        if ((flag.l && flag.v && counts.invert_lines > 0) ||
            (flag.l && counts.count_matching_lines > 0))
            printf("%s\n", argv);
        free(line);
        regfree(&preg);
    }
}

void flag_n(flags flag, int argc, char *argv, char *line, counter counts) {
    print_name_file(argc, argv, flag);
    print_line_and_line_break('n', line, counts.number);
}

void flag_h(flags flag, char *line, counter counts) {
    if (flag.n) printf("%d:", counts.number);
    print_line_and_line_break('0', line, counts.number);
}

void grep_no_flags(flags flag, int argc, char *argv, char *line) {
    print_name_file(argc, argv, flag);
    printf("%s", line);
    add_line_break(line);
}

void flag_v(flags flag, int argc, char *argv, char *line, int number) {
    print_name_file(argc, argv, flag);
    if (flag.n && !flag.o)
        print_line_and_line_break('n', line, number);
    else
        print_line_and_line_break('0', line, number);
}

void flag_c(flags flag, int argc, char *argv, counter counts) {
    print_name_file(argc, argv, flag);
    if (flag.l && flag.v) {
        ((counts.matched_file_invert == 1) ? printf("1\n") : printf("0\n"));
    } else if (flag.l) {
        ((counts.matched_file == 1) ? printf("1\n") : printf("0\n"));
    } else if (flag.v) {
        printf("%d\n", counts.invert_lines);
    } else {
        printf("%d\n", counts.count_matching_lines);
    }
}

int flag_o(flags flag, int argc, char *argv, int *printed, int *printed_no,
           int start, int end, char *line, int number) {
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
        printf("%d:", number);
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
