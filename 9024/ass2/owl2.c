// #include <stdio.h>
// #include <stdlib.h>
// #include <stdbool.h>
// #include <string.h>
// #include <math.h>
// #include "Graph.h"
// #include "Quack.h"
// #define MAX_WORD_LENGTH 20
// #define MAX_NODES 1000

// int current = 0;
// int count_size = 0;
// int visited[MAX_NODES];
// int getsize(char *word) {
//     int i = 0;
//     while (*word != '\0') {
//         word++;
//         i++;
//     }
//     return i;
// }

// bool differByOne(char *word_a, char *word_b) {
//     int size_a = getsize(word_a);
//     int size_b = getsize(word_b);
//     // printf("%d, %d\n", size_a, size_b);
//     int flag = 0;
//     if(size_a == size_b) {
//         for (int i = 0; i < size_a; i++) {
//             if (*(word_a + i) != *(word_b + i)) {
//                 flag++;
//             }
//         }
//     }
//     else if (abs(size_a - size_b) == 1) {
//         int i = 0;
//         if (size_a > size_b) {
//             while(i < size_a) {
//                 if(*word_a == *word_b) {
//                     word_a++;
//                     word_b++;
//                 }
//                 else {
//                     word_a++;
//                     flag++;
//                 }
//                 i++;
//             }
//         }
//         else {
//             while(i < size_b) {
//                 if(*word_a == *word_b) {
//                     word_a++;
//                     word_b++;
//                 }
//                 else {
//                     word_b++;
//                     flag++;
//                 }
//                 i++;
//             }
//         }
//     }
//     if (flag == 1) {
//         return true;
//     }
//     else {
//         return false;
//     }
// }

// int findMaxlength(Graph g, int numV) {
//     int distance[MAX_NODES];
//     int max = 0;
//     for (int i = 0; i < numV; i++) {
//         distance[i] = 0;
//     }
//     for (int i = 0; i < numV; i++) {
//         for (int j = i; j < numV; j++) {
//             if (isEdge(newEdge(i, j), g) && distance[j] <= distance[i]) {
//                 distance[j] = distance[i] + 1;
//             }
//         }
//     }
//     for (int i = 0; i < numV; i++) {
//         if (distance[i] > max) {
//             max = distance[i];
//         }
//     }
    
//     int max_length = max + 1;
//     return max_length;
// }

// void findPath(Graph g, int rootv, int numV, int length, int max_length, int *path, char **word) {
//     visited[rootv] = 1;
//     path[current] = rootv;
//     current++;
//     for (int i = 0; i < numV; i++) {
//         if(isEdge(newEdge(rootv, i), g) && visited[i] == -1 && rootv < i) {
//             findPath(g, i, numV, length + 1, max_length, path, word);
//         }
//     }
//     if (length == max_length) {
//         count_size++;
//         printf("%d: ",count_size);
//         for(int i = 0; i < current - 1; i++) {
//             printf("%s -> ",word[path[i]]);
//         }
//         printf("%s\n", word[path[current - 1]]);
        
//     }
//     length--;
//     visited[rootv] = -1;
//     current--;

// }

// void findPathdfs(Graph g, int rootv, int numV, int max_length, char **word){
//     int path[MAX_NODES];
//     for (int v = 0; v < numV; v++) {
//         visited[v] = -1;
//     }
//     visited[rootv] = rootv;
//     findPath(g, rootv, numV, 1, max_length, path, word);
// }

// int main(int argc, char *agrv[]) {
//     // word = malloc(sizeof(char) * MAX);
//     char w;
//     char **word;
//     word = malloc(sizeof(char *) * MAX_NODES);
//     word[0] = malloc(sizeof(char) * MAX_WORD_LENGTH);
//     char *p = malloc(sizeof(char) * MAX_WORD_LENGTH);
//     int len = 0;
//     int count = 0;
//     int flag = 1;
//     while((w = getchar()) != EOF) {
//         if (w != ' ' && w != '\t' && w != '\n') {
//             *(p + len) = w;
//             len++;
//             flag++;
//         }
//         else {
//             if (flag > 1){
//                 *(p + len) = '\0';
//                 word[count] = p;
//                 len = 0;
//                 p = NULL;
//                 p = malloc(sizeof(char) * MAX_WORD_LENGTH);
//                 count++;
//                 word[count] = malloc(sizeof(char) * MAX_WORD_LENGTH);
//                 flag = 1;
//             }
            
//         }
//     }
//     if (flag > 1){
//         *(p + len) = '\0';
//         word[count++] = p;
//     }
//     // free(p);
    
//     // delete duplicate word
//     for (int i = 0; i < count; i++) {
//         for (int j = i + 1; j < count; j++) {
//             if (strcmp(word[i], word[j]) == 0) {
//                 for (int k = j; k < count; k++) {
//                     word[k] = word[k + 1];
//                 }
//                 count--;
//             }
//         }
//     }
    
//     int word_length = count;
//     // print dictionary
//     printf("Dictionary\n");
//     for(int i = 0; i< count; i++) {
//         printf("%d: %s\n", i, word[i]);
//     }
//     // printf("%d",count);
//     Graph graph = newGraph(count);
//     printf("Ordered Word Ladder Graph\n");
//     // printf("%s %s\n",word[0], word[1]);
//     for(int i = 0; i < count; i++) {
//         for (int j = 0; j < count; j++) {
//             if (differByOne(word[i], word[j]) == 1) {
//                 Edge edge = newEdge(i, j);
//                 insertEdge(edge, graph);
//             }
//         }
//     }
//     showGraph(graph);
//     // dfsQuack(graph,0,word_length);
//     // for (int i = 0;)
//     int max = findMaxlength(graph, word_length);
//     // printf("%d, %d\n", max, word_length);
//     printf("Longest ladder length: %d\n", max);
//     printf("Longest ladders:\n");
//     for (int i = 0; i < word_length; i++) {
//         findPathdfs(graph, i, word_length, max, word);
//     }

   
    
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Graph.h"

#define TRUE 1
#define FALSE 0
#define NSIZE 1000
#define WSIZE 20

int maxPath[1000];
int visited[1000];
int path[1000];
int maxlength = 0;
int count = 0;
int num = 0;

// create a bool function
bool differByOne(char *a, char *b) {
    int len_a = strlen(a);
    int len_b = strlen(b);
    // changing one letter
    if (len_a == len_b) {
        int nums = 0;
        for (int i = 0; i < len_a; i++) {
            if (a[i] == b[i]) {
                nums++;
            }
        }
        if (nums == len_a - 1) {
            return TRUE;
        }
        else {
            return FALSE;
        }
    }
        // adding or removing one letter
    else if (len_a - len_b == 1 || len_b - len_a == 1) {
        int counts = 0;
        int len;
        char *longer;
        char *shorter;
        if (len_a - len_b > 0) {
            len = strlen(a);
            longer = a;
            shorter = b;
        }
        else {
            len = strlen(b);
            longer = b;
            shorter = a;
        }
        for (int i = 0; i < len; i++) {
            // first
            int check = 0;
            if (counts == 0) {
                for (int j = 0; j < len - 1; j++) {
                    if (shorter[j] == longer[j + 1]) {
                        check++;
                    }
                }
                if (check == len - 1) {
                    return TRUE;
                }
            }
            else if (counts == len - 1) {
                for (int j = 0; j < counts; j++) {
                    if (shorter[j] == longer[j]) {
                        check++;
                    }
                }
                if (check == len - 1) {
                    return TRUE;
                }
            }
            else {
                for (int j = 0; j < counts; j++) {
                    if (shorter[j] == longer[j]) {
                        check++;
                    }
                }
                for (int j = counts; j < len - 1; j++) {
                    if (shorter[j] == longer[j + 1]) {
                        check++;
                    }
                }
                if (check == len - 1) {
                    return TRUE;
                }
            }
            counts++;
        }
        return FALSE;
    }
        // not meet the rules
    else {
        return FALSE;
    }
}

// find the length of longest path
int searchMax(Graph g, int total) {
    for (int i = 0; i < total; i++) {
        for (int j = i + 1; j < total; j++) {
            if (isEdge(newEdge(i, j), g) == 1 && maxPath[j] < maxPath[i] + 1) {
                maxPath[j] =  maxPath[i] + 1;
                if (maxPath[j] > maxlength) {
                    maxlength = maxPath[j];
                }
            }
        }
    }
    int max = maxlength + 1;
    printf("Longest ladder length: %d\n", max);
    return maxlength;
}

// find the correct path and print
void findPath(Graph g, char words[NSIZE][WSIZE], int w, int length, int total) {
    visited[w]= 1;
    path[count] = w;
    count++;
    for (int i = w; i < total; i++) {
        if (visited[i] == 0 && isEdge(newEdge(w, i), g) == 1) {
            findPath(g, words, i, length + 1, total);
        }
    }
    if (length == maxlength) {
        num++;
        printf("%d: ", num);
        for (int i = 0; i < count - 1; i++) {
            printf("%s -> ", words[path[i]]);
        }
        printf("%s\n", words[path[count - 1]]);
    }
    length--;
    visited[w] = 0;
    count--;
}

// Depth first search
void dfsMethod(Graph g, char words[NSIZE][WSIZE], int w, int total) {
    for (int v = 0; v < total; v++) {
        visited[v] = 0;
    }
    visited[w] = w;
    findPath(g, words, w, 0, total);
}

int main() {
    // read the file
    char words[NSIZE][WSIZE];
    char c;
    int len = 0, j = 0, flag = 0;
    while ((c = getchar()) != EOF) {
        if ((c == ' ' || c == '\n' || c == '\t')&& flag > 0) {
            // printf("%d\n", 1243);
            len++;
            j = 0;
            flag = 0;
        }
        else if (c != ' ' && c != '\n' && c != '\t'){
            // if (flag > 1) {
            //     words[len][j] = c;
            //     j++;
            //     flag = 1;
            // }
            words[len][j] = c;
            j++;
            flag = 1;
            
        }
    }
    if (flag != 1) {
        len--;
    }
    int total = len + 1;
    for (int a = 0; a < total; a++) {
        for (int b = a + 1; b < total; b++) {
            if (strcmp(words[a], words[b]) == 0) {
                for (int d = b; d < total; d++) {
                    int one = sizeof(words[d])/sizeof(words[d][0]);
                    int two = sizeof(words[d + 1])/sizeof(words[d + 1][0]);
                    int chang;
                    if (one > two) {
                        chang = one;
                    }
                    else {
                        chang = two;
                    }
                    for (int e = 0; e < chang; e++) {
                        words[d][e] = words[d + 1][e];
                    }
                }
                total--;
            }
        }
    }
    // print Phase 1
    printf("Dictionary\n");
    for (int i = 0; i < total; i++) {
        printf("%d: %s\n", i, words[i]);
    }
    // create a new graph
    Graph g = newGraph(total);
    // add the edges into graph
    printf("Ordered Word Ladder Graph\n");
    // compute nE and insert the edges
    for (int i = 0; i < total - 1; i++) {
        for (int k = i + 1; k < total; k++) {
            if (differByOne(words[i], words[k]) == 1) {
                Edge e = newEdge(i, k);
                insertEdge(e, g);
            }
        }
    }
    showGraph(g);
    // Phase 3
    searchMax(g, total);
    printf("Longest ladders:\n");
    for (int i = 0; i < total; i++) {
        dfsMethod(g, words, i, total);
    }
    freeGraph(g);
    return 0;
}
