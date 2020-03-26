// COMP9024 - Assignment 2
// ZID: z5230310
// Name: Tian Liu
// Phase1: bool differByOne to compare two strings
// Phase2: read input from stdin and use Graph ADT to build graph and show graph
// Phase3: find the length of longest owl in the graph
//
// Written: 01/08/2019
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "Graph.h"
#include "Quack.h"
#define MAX_WORD_LENGTH 20
#define MAX_NODES 1000

int current = 0;
int count_size = 0;
int visited[MAX_NODES];

// compare two strings whether differ by one
bool differByOne(char *word_a, char *word_b) {
    int size_a = strlen(word_a);
    int size_b = strlen(word_b);
    int flag = 0;
    if(size_a == size_b) {
        for (int i = 0; i < size_a; i++) {
            if (*(word_a + i) != *(word_b + i)) {
                flag++;
            }
        }
    }
    else if (abs(size_a - size_b) == 1) {
        int i = 0;
        if (size_a > size_b) {
            while(i < size_a) {
                if(*word_a == *word_b) {
                    word_a++;
                    word_b++;
                }
                else {
                    word_a++;
                    flag++;
                }
                i++;
            }
        }
        else {
            while(i < size_b) {
                if(*word_a == *word_b) {
                    word_a++;
                    word_b++;
                }
                else {
                    word_b++;
                    flag++;
                }
                i++;
            }
        }
    }
    if (flag == 1) {
        return true;
    }
    else {
        return false;
    }
}

// find the max length of all words
int findMaxlength(Graph g, int numV) {
    int distance[MAX_NODES];
    int max = 0;
    for (int i = 0; i < numV; i++) {
        distance[i] = 0;
    }
    for (int i = 0; i < numV; i++) {
        for (int j = i; j < numV; j++) {
            if (isEdge(newEdge(i, j), g) && distance[j] <= distance[i]) {
                distance[j] = distance[i] + 1;
            }
        }
    }
    for (int i = 0; i < numV; i++) {
        if (distance[i] > max) {
            max = distance[i];
        }
    }
    if (max == 0) {
        return max;
    }
    else {
        return max + 1;
    }
}

// find path
void findPath(Graph g, int rootv, int numV, int length, int max_length, int *path, char **word) {
    visited[rootv] = 1;
    path[current] = rootv;
    current++;
    for (int i = 0; i < numV; i++) {
        if(isEdge(newEdge(rootv, i), g) && visited[i] == -1 && rootv < i) {
            findPath(g, i, numV, length + 1, max_length, path, word);
        }
    }
    if (length == max_length) {
        count_size++;
        printf("%2d: ",count_size);
        for(int i = 0; i < current - 1; i++) {
            printf("%s -> ",word[path[i]]);
        }
        printf("%s\n", word[path[current - 1]]);
        
    }
    length--;
    visited[rootv] = -1;
    current--;
}

// DFS to find the path
void findPathdfs(Graph g, int rootv, int numV, int max_length, char **word){
    int path[MAX_NODES];
    for (int v = 0; v < numV; v++) {
        visited[v] = -1;
    }
    visited[rootv] = rootv;
    findPath(g, rootv, numV, 1, max_length, path, word);
}

int main(int argc, char *agrv[]) {
    char w;
    char **word;
    word = malloc(sizeof(char *) * MAX_NODES);
    word[0] = malloc(sizeof(char) * MAX_WORD_LENGTH);
    char *p = malloc(sizeof(char) * MAX_WORD_LENGTH);
    int len = 0;
    int count = 0;

    // read words from stdin
    while((w = getchar()) != EOF) {
        if (w >= 'a' && w <= 'z') {
            *(p + len) = w;
            len++;
        }
        else {
            if (len > 0){
                *(p + len) = '\0';
                word[count] = p;
                len = 0;
                p = malloc(sizeof(char) * MAX_WORD_LENGTH);
                count++;
                word[count] = malloc(sizeof(char) * MAX_WORD_LENGTH);
            }
            
        }
    }
    if (len > 0){
        *(p + len) = '\0';
        word[count++] = p;
    }

    // delete duplicate words
    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(word[i], word[j]) == 0) {
                for (int k = j; k < count; k++) {
                    word[k] = word[k + 1];
                }
                count--;
            }
        }
    }
    int word_length = count;

    // print dictionary
    printf("Dictionary\n");
    for(int i = 0; i< word_length; i++) {
        printf("%d: %s\n", i, word[i]);
    }

    // create and print graph
    Graph graph = newGraph(word_length);
    printf("Ordered Word Ladder Graph\n");
    for (int i = 0; i < word_length; i++) {
        for (int j = 0; j < word_length; j++) {
            if (differByOne(word[i], word[j]) == 1) {
                Edge edge = newEdge(i, j);
                insertEdge(edge, graph);
            }
        }
    }
    showGraph(graph);

    int max = findMaxlength(graph, word_length);
    printf("Longest ladder length: %d\n", max);
    printf("Longest ladders:\n");

    // use DFS find the longest owl
    for (int i = 0; i < word_length; i++) {
        findPathdfs(graph, i, word_length, max, word);
    }

    // free memory
    for (int i = 0; i < word_length; i++) {
        free(word[i]);
    }
    free(word);
}