#include <getopt.h>

#ifndef GREP_H
#define GREP_H

typedef struct
{
    int c_count;
    int number;
    int match_lines;
    int match_files;
    int match_files_invert;
    int invert_line;
}value;

void grep(FILE *file, flags flag, char* pattern,int argc, char *argv);
void flag_h(flags flag,char* line,value v);
void print_line_and_line_break(char flag, char *line,value v);
void print_name_file(int argc, char *argv, flags flag);
void flag_n(flags flag, int argc, char *argv, char *line, value v);
void grep_no_flags(flags flag, int argc, char *argv, char *line);
void flag_v(flags flag, int argc, char* argv, char* line, value v);
int  flag_o(flags flag, int argc,char * argv, int *printed,int *printed_no,int start, int end, char *line,value v);
void flag_c(flags flag,int argc, char* argv, value v);


#endif