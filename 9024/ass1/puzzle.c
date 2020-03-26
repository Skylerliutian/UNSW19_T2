// COMP9024 - Assignment 1
// ZID: z5230310
// Name: Tian Liu
//
// main program to  
//
// Written: 24/06/2019
//

#include "boardADT.h"

int main(int argc, char *argv[]) {
    // use two pointers to get the first and second line from the file
    char *start_state = NULL;  
    char *goal_state = NULL;
    start_state = malloc(sizeof(char) * MAX);
    goal_state = malloc(sizeof(char) * MAX);
    fgets(start_state, MAX, stdin);
    fgets(goal_state, MAX, stdin);
    printf("start: %s", start_state);
    printf("goal: %s\n", goal_state);

    //build two board use function build_board
    board_type start = build_board(start_state);
    board_type goal = build_board(goal_state);

    //printf("%d,%d",is_valid_board(start),is_valid_board(goal));
    printf("%d,%d", compute_disorder(start),compute_disorder(goal));

    // check valid or not
    if (is_valid_board(start) != 1 || is_valid_board(goal) != 1) {
        printf("Invalid Input!\n");
        return EXIT_FAILURE;
    }
    // check start size and goal size are same
    else if (n_size(start) != n_size(goal)) {
        printf("Size Error!\n");
        return EXIT_FAILURE;
    }
    // check solvable or not
    else {
        if (result(start, goal) == 1) {
            printf("solvable\n");
        }
        else {
            printf("unsolvable\n");
        }
    }
    //free memory
    free_mem(start);
    free_mem(goal);
    free(start_state);
    free(goal_state);

    return EXIT_SUCCESS;

}