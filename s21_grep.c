#define _GNU_SOURCE
#include <stdio.h>
#include <getopt.h>
#include "parse.h"
#include <regex.h>
#include <string.h>
#include "grep.h"


int main(int argc, char** argv) {

    flags flag= {0};
    char *short_option = "e:ivclnhsf:o";
    int err = parse_params(argc,argv,short_option,&flag);
    if (flag.v && flag.o) flag.o = 0;
    FILE *file;



    char *pattern = flag.e  ? flag.str : flag.f ? flag.str_e : argv[optind];


    for (int i = flag.e || flag.f  ? optind : optind+1;i < argc; ++i) {
        char *file_name = argv[i];
        file = fopen(file_name,"r");

        if (file == NULL) {
            if (!flag.s) {
                printf("%s: no such file or directory\n", file_name);
            }
        } else {
            grep(file, flag, pattern,argc,argv[i]);
            fclose(file);
        }
    }
    free_flags(&flag);

    return err;
}




