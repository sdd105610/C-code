// 7.	Write C program to implement LR parsing algorithm

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_STACK 100
#define MAX_INPUT 100
#define MAX_STATES 50
#define MAX_SYMBOLS 26

// Structure for Action table entries
struct Action {
    char type;  // 's' for shift, 'r' for reduce, 'a' for accept
    int value;  // state number for shift, production number for reduce
};

// Structure for grammar productions
struct Production {
    char lhs;
    char rhs[10];
    int len;
};

// Global variables
struct Action action[MAX_STATES][MAX_SYMBOLS];
int goto_table[MAX_STATES][MAX_SYMBOLS];
struct Production productions[MAX_SYMBOLS];
int num_prods = 0;
int stack[MAX_STACK];
char symbols[MAX_STACK];
int top = -1;
char input[MAX_INPUT];
int input_pos = 0;

// Stack operations
void push(int state, char symbol) {
    if (top < MAX_STACK - 2) {
        symbols[++top] = symbol;
        stack[top] = state;
    }
}

void pop(int count) {
    top -= count;
}

// Initialize parsing tables
void initTables() {
    // Initialize action table with error entries
    for(int i = 0; i < MAX_STATES; i++) {
        for(int j = 0; j < MAX_SYMBOLS; j++) {
            action[i][j].type = 'e';  // error
            action[i][j].value = 0;
            goto_table[i][j] = -1;
        }
    }

    // Add shift actions
    action[0]['i'-'a'].type = 's';
    action[0]['i'-'a'].value = 5;
    // Add more actions based on your grammar

    // Add reduce actions
    action[1]['+'-'a'].type = 'r';
    action[1]['+'-'a'].value = 2;
    // Add more actions

    // Add goto entries
    goto_table[0]['E'-'A'] = 1;
    goto_table[0]['T'-'A'] = 2;
    // Add more goto entries

    // Add productions
    productions[0].lhs = 'E';
    strcpy(productions[0].rhs, "E+T");
    productions[0].len = 3;
    // Add more productions
}

// Function to parse input
int parse() {
    // Initialize stack with state 0
    push(0, '$');
    
    printf("\nParsing Steps:\n");
    printf("Stack\tInput\tAction\n");
    
    while (1) {
        // Print current configuration
        printf("Stack: ");
        for(int i = 0; i <= top; i++)
            printf("%c%d", symbols[i], stack[i]);
        printf("\tInput: %s\tAction: ", &input[input_pos]);
        
        int current_state = stack[top];
        char current_input = input[input_pos];
        struct Action act = action[current_state][current_input - 'a'];
        
        if (act.type == 's') {
            // Shift action
            push(act.value, current_input);
            input_pos++;
            printf("Shift %d\n", act.value);
        }
        else if (act.type == 'r') {
            // Reduce action
            struct Production prod = productions[act.value];
            pop(prod.len);
            int goto_state = goto_table[stack[top]][prod.lhs - 'A'];
            push(goto_state, prod.lhs);
            printf("Reduce by %c->%s\n", prod.lhs, prod.rhs);
        }
        else if (act.type == 'a') {
            // Accept
            printf("Accept\n");
            return 1;
        }
        else {
            // Error
            printf("Error\n");
            return 0;
        }
    }
}

int main() {
    // Initialize parsing tables
    initTables();
    
    // Get input string
    printf("Enter input string (end with $): ");
    scanf("%s", input);
    
    // Parse input
    if(parse()) {
        printf("\nInput string accepted!\n");
    } else {
        printf("\nInput string rejected!\n");
    }
    
    return 0;
}