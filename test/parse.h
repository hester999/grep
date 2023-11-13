#ifndef GREP_PARSE_FLAG_H
#define GREP_PARSE_FLAG_H
#define _GNU_SOURCE
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int v;
    int c;
    int l;
    int n;
    int h;
    int s;
    int f;
    int o;
    int e;
    int i;
    char *str;
    char *str_e;
} flags;

int parse_flags(int argc, char *argv[], char *short_flags, flags *all_flag,
                char *patterns);
void save_sample(char *argv, char *sample);
void save_sample_from_file(char *argv, char *sample);

#endif