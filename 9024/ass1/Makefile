#
# Zid: z5230310
# Name: Tian Liu
# Date: 26/06/2019
#
# Makefile to compile puzzle.c
#
#

CC = gcc


puzzle: puzzle.c boardADT.o
	@$(CC) puzzle.c boardADT.o -o puzzle

boardADT.o: boardADT.c
	@$(CC) -c  boardADT.c

clean:
	@rm *.o puzzle