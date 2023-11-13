#include "parse.h"

#include <stdlib.h>
#include <string.h>

int parse_flags(int argc, char *argv[], char *short_flags, flags *all_flag,
                char *patterns) {
    int res = 0;
    int flag_error = 0;

    while ((res = getopt_long(argc, argv, short_flags, NULL, NULL)) != -1) {
        switch (res) {
            case 'o':
                all_flag->o = 1;
                break;
            case 'e':
                save_sample(optarg, patterns);
                all_flag->e = 1;
                break;
            case 'i':
                all_flag->i = 1;
                break;
            case 'v':
                all_flag->v = 1;
                break;
            case 'c':
                all_flag->c = 1;
                break;
            case 'l':
                all_flag->l = 1;
                break;
            case 'n':
                all_flag->n = 1;
                break;
            case 'h':
                all_flag->h = 1;
                break;
            case 's':
                all_flag->s = 1;
                break;
            case 'f':
                all_flag->f = 1;
                save_sample_from_file(optarg, patterns);
                break;
            case '?':
            default:
                flag_error = 1;
                break;
        }
    }
    return flag_error;
}

void save_sample(char *argv, char *sample) {
    if (sample[0] == 0)
        strcpy(sample, argv);
    else {
        strcat(sample, "|");
        strcat(sample, argv);
    }
}

void save_sample_from_file(char *argv, char *sample) {
    FILE *new = fopen(argv, "r");
    int res = 0;
    if (new == NULL) {
        fprintf(stderr, "Error: file not found\n");
    } else {
        char *line = NULL;
        size_t len = 0;

        while ((res = getline(&line, &len, new)) != -1) {
            int number = strlen(line);
            if (strstr(line, "\n") != 0) {
                line[number - 1] = '\0';
            }
            if (sample[0] == 0) {
                strcpy(sample, line);
            } else {
                strcat(sample, "|");
                strcat(sample, line);
            }
        }
        free(line);
    }
    fclose(new);
}