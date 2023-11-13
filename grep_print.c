#include "grep_print.h"

void add_line_break(char *line) {
  if (strstr(line, "\n") == 0) {
    putchar('\n');
  }
}

void print_name_file(int argc, char *argv, flags all_flag) {
  if (!all_flag.h) {
    if (all_flag.e || all_flag.f) {
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

void grep(FILE *new_f, char *sample, flags all_flag, int argc, char *argv) {
  char *line = NULL;
  size_t len = 0;
  regex_t preg;
  regmatch_t match;
  int res = 0;
  int err = regcomp(&preg, sample,
                    REG_EXTENDED | (all_flag.i ? REG_ICASE : 0) | REG_NEWLINE);
  if (err != 0)
    fprintf(stderr, "Error: compilation error");
  else {
    counter counts = {0};
    while ((res = getline(&line, &len, new_f)) != -1) {
      int printed = 0, printed_no = 0;
      counts.number = counts.number + 1;
      if ((regexec(&preg, line, 1, &match, 0) == 0) && !all_flag.v) {
        counts.count_matching_lines++;
        counts.matched_file = 1;
        if (all_flag.c || all_flag.l) continue;
        if (all_flag.h && !all_flag.o) {
          flag_h(all_flag, line, counts);
          continue;
        }
        if (all_flag.n && !all_flag.o)
          flag_n(all_flag, argc, argv, line, counts);
        if ((!all_flag.o && !all_flag.n && !all_flag.h))
          grep_no_flags(all_flag, argc, argv, line);
      } else if ((regexec(&preg, line, 1, &match, 0) != 0) && all_flag.v) {
        counts.invert_lines = counts.invert_lines + 1;
        counts.matched_file_invert = 1;
        if (all_flag.c || all_flag.l) continue;
        flag_v(all_flag, argc, argv, line, counts.number);
      }
      if (all_flag.o) {
        int add = 0;
        while (regexec(&preg, line + add, 1, &match, 0) == 0) {
          int start = match.rm_so + add, end = match.rm_eo + add;
          add = flag_o(all_flag, argc, argv, &printed, &printed_no, start, end,
                       line, counts.number);
        }
      }
    }
    if (all_flag.c) flag_c(all_flag, argc, argv, counts);
    if ((all_flag.l && all_flag.v && counts.invert_lines > 0) ||
        (all_flag.l && counts.count_matching_lines > 0))
      printf("%s\n", argv);
    free(line);
    regfree(&preg);
  }
}

void flag_n(flags all_flag, int argc, char *argv, char *line, counter counts) {
  print_name_file(argc, argv, all_flag);
  print_line_and_line_break('n', line, counts.number);
}

void flag_h(flags all_flag, char *line, counter counts) {
  if (all_flag.n) printf("%d:", counts.number);
  print_line_and_line_break('0', line, counts.number);
}

void grep_no_flags(flags all_flag, int argc, char *argv, char *line) {
  print_name_file(argc, argv, all_flag);
  printf("%s", line);
  add_line_break(line);
}

void flag_v(flags all_flag, int argc, char *argv, char *line, int number) {
  print_name_file(argc, argv, all_flag);
  if (all_flag.n && !all_flag.o)
    print_line_and_line_break('n', line, number);
  else
    print_line_and_line_break('0', line, number);
}

void flag_c(flags all_flag, int argc, char *argv, counter counts) {
  print_name_file(argc, argv, all_flag);
  if (all_flag.l && all_flag.v) {
    ((counts.matched_file_invert == 1) ? printf("1\n") : printf("0\n"));
  } else if (all_flag.l) {
    ((counts.matched_file == 1) ? printf("1\n") : printf("0\n"));
  } else if (all_flag.v) {
    printf("%d\n", counts.invert_lines);
  } else {
    printf("%d\n", counts.count_matching_lines);
  }
}

int flag_o(flags all_flag, int argc, char *argv, int *printed, int *printed_no,
           int start, int end, char *line, int number) {
  if (all_flag.h)
    ;
  else if (all_flag.e) {
    if ((argc - optind > 1) && !(*printed)) {
      printf("%s:", argv);
      (*printed) = 1;
    }
  } else if ((argc - optind > 2) && !(*printed)) {
    printf("%s:", argv);
    (*printed) = 1;
  }

  if (all_flag.n && !(*printed_no)) {
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
