//
// COMP9024 - Assignment 1
// 
// ZID: z5230310
// Name: Tian Liu
// Written: 24/06/2019
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// define the MAX memorey to allocate
#define MAX 1024

typedef struct board_struct *board_type;

board_type build_board(char *);

int n_size (board_type);

int is_valid_board (board_type);

int compute_disorder (board_type);

int result(board_type, board_type);

void free_mem(board_type);
