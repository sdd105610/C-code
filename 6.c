// 6.	Write C program to parse using  non recursive predictive parsing table.(implement non recursive predictive parsing algorithm. )
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_STACK 100
#define MAX_INPUT 100
#define MAX_TABLE 26
#define MAX_PROD 20

// Structure for parsing table entries
struct TableEntry {
    char production[20];
    int valid;
};

// Global variables
struct TableEntry parsing_table[MAX_TABLE][MAX_TABLE];
char stack[MAX_STACK];
int top = -1;
char input[MAX_INPUT];
int input_pos = 0;

// Stack operations
void push(char symbol) {
    if (top < MAX_STACK - 1) {
        stack[++top] = symbol;
    }
}

char pop() {
    if (top >= 0) {
        return stack[top--];
    }
    return '\0';
}

// Initialize parsing table
void initParsingTable() {
    // Initialize all entries as invalid
    for(int i = 0; i < MAX_TABLE; i++) {
        for(int j = 0; j < MAX_TABLE; j++) {
            parsing_table[i][j].valid = 0;
        }
    }
    
    // Add productions to table
    // Example: E -> TE'
    parsing_table[0][0].valid = 1;  // E,a
    strcpy(parsing_table[0][0].production, "TE'");
    
    // Add more productions based on your grammar
}

// Function to parse input
int parse() {
    // Initialize stack with start symbol and $
    push('$');
    push('E');  // Start symbol
    
    printf("\nParsing Steps:\n");
    printf("Stack\tInput\tAction\n");
    
    while(top >= 0) {
        // Print current configuration
        printf("%-8s\t%-8s\t", stack, &input[input_pos]);
        
        char stack_top = stack[top];
        char curr_input = input[input_pos];
        
        // If stack top matches current input
        if(stack_top == curr_input) {
            printf("Match %c\n", curr_input);
            pop();
            input_pos++;
            continue;
        }
        
        // If stack top is non-terminal
        if(isupper(stack_top)) {
            int row = stack_top - 'A';
            int col = curr_input - 'a';
            
            if(parsing_table[row][col].valid) {
                // Get production and apply it
                char *prod = parsing_table[row][col].production;
                pop();
                
                // Push production in reverse order
                for(int i = strlen(prod) - 1; i >= 0; i--) {
                    push(prod[i]);
                }
                
                printf("Apply %c->%s\n", stack_top, prod);
            } else {
                printf("Error: No production found\n");
                return 0;
            }
        }
        
        // If stack top is $ and input is finished
        else if(stack_top == '$' && curr_input == '$') {
            printf("Accept\n");
            return 1;
        }
        
        // Error case
        else {
            printf("Error: Invalid symbol\n");
            return 0;
        }
    }
    
    return 0;
}

int main() {
    // Initialize parsing table
    initParsingTable();
    
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