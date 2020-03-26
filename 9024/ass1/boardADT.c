//
// COMP9024 - Assignment 1
//
// definition of board structure
// and functions about board 
// 
// ZID: z5230310
// Name: Tian Liu
// Written: 24/06/2019
//

#include "boardADT.h"

/**
 * define board structure
 */
struct board_struct {
    // pointer
    int *numbers;
    // total number in board
    int total_number;
 };

/**
 * in order to bulid a board structure by stdin input
 */
board_type build_board(char *state) {
    // allocate memory for board structure
    board_type board = malloc(sizeof(struct board_struct));
    if (board != NULL) {
        // initialization
        board->total_number = 0;
        board->numbers = NULL;
        board->numbers = malloc(sizeof(int) * MAX);
        if (state != NULL) {
            // split number
            char *split_num = strtok(state," ");
            while (split_num != NULL) {
                // define copychar to solve first line is "b"
                char *copy_char;
                copy_char = malloc(sizeof(char));
                strncpy(copy_char, split_num,1);
                if (strcmp(copy_char, "b") != 0) {
                    if (atoi(split_num)) {
                        *(board->numbers + board->total_number) = atoi(split_num);
                    }
                    else {
                        *(board->numbers + board->total_number) = -1;
                    }
                }
                else {
                    *(board->numbers + board->total_number) = 0;
                }
                split_num = strtok(NULL, " ");
                board->total_number += 1;
                free(copy_char);
            }
        }
    }
    return board;
}

/**
 * calculate the size of the board by using total_number
 */
int n_size (board_type board) {
    int size_board;
    size_board = sqrt(board->total_number);
    return size_board;
}

/**
 * check whether the board is valid,
 * if valid return 1; else return 0;
 */
int is_valid_board (board_type board) {
    // judge board and numbers are not NULL, 
    if (board == NULL || board->numbers == NULL || board->total_number == 0) {
        return 0;
    }
    //judge whether size_board square equal to total_number
    int size_board = n_size (board);
    if (size_board * size_board != board->total_number) {
        return 0;
    }
    // judge every number are different from 0 to (size_board ** 2)
    for (int i = 0; i < board->total_number; i++) {
        // use flag to check
        int flag = 0;
        for (int j = 0; j < board->total_number; j++) {
            if (*(board->numbers + j) == i) {
                flag++;
            }
        }
        if (flag != 1) {
            return 0;
        }
    }
    return 1;
}

/** 
 * compute the number of disorder and return
 */
int compute_disorder (board_type board) {
    // define count as a counter
    int count = 0;
    // get size of board
    int size = n_size(board);
    if (board != NULL) {
        // loop
        for (int i = 0; i < board->total_number; i++) {
            if (*(board->numbers + i) == 0) {
                // first check the size of board(even or odd)
                if (size % 2 == 0) {
                    // if even, plus the row number of the blank
                    count += i / size + 1;
                }
            }
            else {
                // use for loop to count disorder
                for (int j = i + 1; j < board->total_number; j++) {
                    if (*(board->numbers + j) != 0 && (*(board->numbers + i) > *(board->numbers + j))) {
                        count++;
                    }
                }
            }
        }
    }
    return count;
}

/**
 * check the board is solvable or unsolvable
 * solvable return 1, else return 0
 */
int result(board_type start, board_type goal) {
    // check disorder are both even or both odd
    if ((compute_disorder(start) + compute_disorder(goal)) % 2 == 0) {
        return 1;
    }
    else {
        return 0;
    }

}

/**
 * free memory
 */
void free_mem(board_type board) {
    if (board != NULL) {
        if (board->numbers != NULL) {
            free(board->numbers);
        }
        free(board);
    }
}
