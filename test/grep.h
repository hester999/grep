#ifndef GREP_PRINT_H
#define GREP_PRINT_H
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"

typedef struct {
    int count_matching_lines;
    int invert_lines;
    int number;
    int matched_file;
    int matched_file_invert;
} counter;

void grep(FILE *new_f, char *sample, flags all_flag, int argc, char *argv);
void print_name_file(int argc, char *argv, flags all_flag);
void add_line_break(char *line);
void print_line_and_line_break(char flag, char *line, int number);
void grep_no_flags(flags all_flag, int argc, char *argv, char *line);
void flag_n(flags all_flag, int argc, char *argv, char *line, counter counts);
int flag_o(flags all_flag, int argc, char *argv, int *printed, int *printed_no,
           int start, int end, char *line, int number);
void flag_h(flags all_flag, char *line, counter counts);
void flag_c(flags all_flag, int argc, char *argv, counter counts);
void flag_v(flags all_flag, int argc, char *argv, char *line, int number);

#endif