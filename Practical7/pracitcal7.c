#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// LALR Parsing Table for grammar:
// E -> E + T | T
// T -> T * F | F
// F -> ( E ) | id

#define STATES 12
#define TERMS  6
#define NTERMS 3

// Terminals: id(0) +(1) *(2) )(3) ((4) $(5)
// Non-terminals: E(0) T(1) F(2)

char action[12][6][5] = {
    {"s5", "err","err","err","s4","err"},  // state 0
    {"err","s6", "err","err","err","acc"}, // state 1
    {"err","r2", "s7", "err","err","r2"},  // state 2
    {"err","r4", "r4", "err","err","r4"},  // state 3
    {"s5", "err","err","err","s4","err"},  // state 4
    {"err","r6", "r6", "err","err","r6"},  // state 5
    {"s5", "err","err","err","s4","err"},  // state 6
    {"s5", "err","err","err","s4","err"},  // state 7
    {"err","s6", "err","s11","err","err"}, // state 8
    {"err","r1", "s7", "err","err","r1"},  // state 9
    {"err","r3", "r3", "err","err","r3"},  // state 10
    {"err","r5", "r5", "err","err","r5"}   // state 11
};

int go_to[12][3] = {
    {1,  2,  3 },  // state 0
    {-1, -1, -1},  // state 1
    {-1, -1, -1},  // state 2
    {-1, -1, -1},  // state 3
    {8,  2,  3 },  // state 4
    {-1, -1, -1},  // state 5
    {-1, 9,  3 },  // state 6
    {-1, -1, 10},  // state 7
    {-1, -1, -1},  // state 8
    {-1, -1, -1},  // state 9
    {-1, -1, -1},  // state 10
    {-1, -1, -1}   // state 11
};

// Grammar rules: {lhs, rhs_length}
// 1: E->E+T, 2: E->T, 3: T->T*F, 4: T->F, 5: F->(E), 6: F->id
int lhs[7]  = {0, 0, 0, 1, 1, 2, 2}; // 0=E,1=T,2=F
int rhs_len[7] = {0, 3, 1, 3, 1, 3, 1};

int state_stack[100];
char sym_stack[100];
int top = -1;

void push_state(int s) { state_stack[++top] = s; }
int  pop_state()       { return state_stack[top--]; }
int  peek_state()      { return state_stack[top]; }

int get_terminal(char c) {
    if (c == 'i') return 0;
    if (c == '+') return 1;
    if (c == '*') return 2;
    if (c == ')') return 3;
    if (c == '(') return 4;
    if (c == '$') return 5;
    return -1;
}

void print_stack() {
    printf("Stack: ");
    for (int i = 0; i <= top; i++)
        printf("%d ", state_stack[i]);
    printf("\t");
}

int main() {
    char input[50];
    printf("Enter input string (use 'i' for id, end without $): ");
    scanf("%s", input);

    int len = strlen(input);
    input[len] = '$';
    input[len+1] = '\0';

    push_state(0);
    int ip = 0;

    printf("\n%-20s %-20s %s\n", "Stack", "Input", "Action");
    printf("-------------------------------------------------------\n");

    while (1) {
        int s = peek_state();
        int a = get_terminal(input[ip]);

        if (a == -1) {
            printf("Invalid character '%c'\n", input[ip]);
            exit(1);
        }

        print_stack();
        printf("%-20s ", input + ip);

        if (strcmp(action[s][a], "acc") == 0) {
            printf("Accept\n");
            break;
        } else if (action[s][a][0] == 's') {
            int next = atoi(&action[s][a][1]);
            push_state(next);
            printf("Shift to state %d\n", next);
            ip++;
        } else if (action[s][a][0] == 'r') {
            int rule = atoi(&action[s][a][1]);
            printf("Reduce by rule %d\n", rule);
            for (int i = 0; i < rhs_len[rule]; i++)
                pop_state();
            int t = peek_state();
            int nt = lhs[rule];
            if (go_to[t][nt] == -1) {
                printf("Error in goto\n");
                exit(1);
            }
            push_state(go_to[t][nt]);
        } else {
            printf("Error! Unexpected symbol '%c'\n", input[ip]);
            exit(1);
        }
    }
    return 0;
}