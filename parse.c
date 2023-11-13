#define _GNU_SOURCE
#include "parse.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

char* concat( char *s1,  char *s2);
char* flag_f(char *optarg);

int parse_params(int argc, char **argv, char *short_options, flags *flag) {
    int res;
    int error = 0;
    char *result="";
    char *result1="";

    while ((res = getopt_long(argc, argv, short_options,NULL,NULL)) !=
           -1) {
        switch (res) {
            case 'e':
                flag->e = 1;
                flag->regextand = REG_EXTENDED;
                if (flag->str != NULL) {
                    result = concat(flag->str, optarg);
                    free(flag->str);
                } else {
                    result = strdup(optarg);
                }
                flag->str = result;

                break;
            case 'i':
                flag->i =1;
                flag->regextand += REG_ICASE;
                break;
            case 'v':
                flag->v= 1;
                break;
            case 'c':
                flag -> c =1;
                break;
            case 'l':
                flag-> l =1;
                break;
            case 'n':
                flag-> n =1;
                break;
            case 'h':
                flag -> h =1;
                break;
            case 's':
                flag-> s =1;
                break;
            case 'f':
                flag->f =1;
                flag->regextand = REG_EXTENDED;

                result1 = flag_f(optarg);
                char * tmp = concat(flag->str,result1);
                free(result1);
                if(flag->str != NULL){
                    free(flag->str);
                }
                flag->str = tmp;
                break;
            case 'o':
                flag -> o =1;
                break;
            case '?':
                error = 1;
                break;
            default:
                error = 1;
                break;
        }
    }

    return error;
}

void free_flags(flags *flag) {
    if (flag->str != NULL) {
        free(flag->str);
        flag->str = NULL;
    }
    if(flag->str_e !=NULL){
        free(flag->str_e);
        flag->str_e = NULL;
    }

}

char* concat(char *s1, char *s2) {

    if(s1 == NULL){
        return strdup(s2);
    }
    int len1  = strlen(s1);
    int len2 = strlen(s2);

    char *result = calloc(len1 + len2 + 2, sizeof(char));
    strcpy(result, s1);

    result[len1] = '|';  // Use '|' as the separator
    strcpy(result + len1 + 1, s2);

    return result;
}


char* flag_f(char *optarg){
    FILE *file;

    file = fopen(optarg, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }
    char s[100];

    char *res;
    char* temp=NULL;
    while (fgets(s, 100, file) != NULL) {
        int len = strlen(s);
        if (s[len - 1] == '\n') {
            s[len - 1] = '\0';
        }

        if (temp == NULL ) {
            temp = strdup(s);
        } else {
            res = concat(temp, s);
            free(temp);
            temp = res;

        }
    }

    fclose(file);
    return temp;
}