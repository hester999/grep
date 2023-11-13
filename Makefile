C = gcc
FLAGS = -Wall -Werror -Wextra -std=c11

all: s21_grep

s21_grep: s21_grep.o parse.o grep.o
	$(C) $(FLAGS) s21_grep.o parse.o grep.o -o s21_grep
	rm -f *.o

s21_grep.o: s21_grep.c
	$(C) $(FLAGS) -c s21_grep.c

parse.o: parse.c
	$(C) $(FLAGS) -c parse.c

grep.o: grep.c
	$(C) $(FLAGS) -c grep.c
clean:
	rm -f *.o 
