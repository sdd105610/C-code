/* 2.	Write C program to construct SLR predictive parsing table
    Note:  the input to the program  is LR(o) items  and FOLLOW of nonterminal. The program    will create the table.
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_ITEMS 50
#define MAX_SYMBOLS 26
#define MAX_STATES 50
#define MAX_LEN 20

// Structure for LR(0) items
struct Item {
    char lhs;
    char rhs[MAX_LEN];
    int dot_pos;
};

// Structure for grammar productions
struct Production {
    char lhs;
    char rhs[MAX_LEN];
};

// Global variables
struct Item items[MAX_ITEMS];
struct Production prods[MAX_ITEMS];
char follow_sets[MAX_SYMBOLS][MAX_LEN];
char terminals[MAX_LEN];
char non_terminals[MAX_LEN];
int num_items = 0;
int num_prods = 0;

// Parsing table - Action and Goto
char action[MAX_STATES][MAX_SYMBOLS][3];  // [state][symbol][action type + number]
int goto_table[MAX_STATES][MAX_SYMBOLS];

// Function to initialize tables
void initializeTables() {
    int i, j;
    for(i = 0; i < MAX_STATES; i++) {
        for(j = 0; j < MAX_SYMBOLS; j++) {
            action[i][j][0] = ' ';
            action[i][j][1] = '0';
            action[i][j][2] = '\0';
            goto_table[i][j] = -1;
        }
    }
}

// Function to add shift actions
void addShiftAction(int state, char symbol, int next_state) {
    int idx = symbol - 'a';
    action[state][idx][0] = 's';
    action[state][idx][1] = next_state + '0';
}

// Function to add reduce actions
void addReduceAction(int state, char symbol, int prod_num) {
    int idx = symbol - 'a';
    action[state][idx][0] = 'r';
    action[state][idx][1] = prod_num + '0';
}

// Function to add goto entries
void addGotoEntry(int state, char non_term, int next_state) {
    int idx = non_term - 'A';
    goto_table[state][idx] = next_state;
}

// Function to construct SLR parsing table
void constructSLRTable() {
    int i, j;
    initializeTables();
    
    // Process each item
    for(i = 0; i < num_items; i++) {
        char symbol_after_dot = items[i].rhs[items[i].dot_pos];
        
        if(symbol_after_dot != '\0') {
            // Shift action
            if(islower(symbol_after_dot)) {
                addShiftAction(i, symbol_after_dot, i+1);
            }
            // Goto action
            else if(isupper(symbol_after_dot)) {
                addGotoEntry(i, symbol_after_dot, i+1);
            }
        } else {
            // Reduce action - add for all symbols in FOLLOW set
            for(j = 0; j < strlen(follow_sets[items[i].lhs - 'A']); j++) {
                char follow_symbol = follow_sets[items[i].lhs - 'A'][j];
                addReduceAction(i, follow_symbol, i);
            }
        }
    }
}

int main() {
    int i, j;
    
    // Input LR(0) items
    printf("Enter number of LR(0) items: ");
    scanf("%d", &num_items);
    
    printf("Enter LR(0) items (format: A->aB., where dot is represented by .)\n");
    for(i = 0; i < num_items; i++) {
        char temp[MAX_LEN];
        scanf("%s", temp);
        items[i].lhs = temp[0];
        strcpy(items[i].rhs, &temp[3]);
        // Find dot position
        char *dot = strchr(temp, '.');
        items[i].dot_pos = dot - temp - 3;
    }
    
    // Input FOLLOW sets
    printf("Enter FOLLOW sets for non-terminals:\n");
    for(i = 0; i < strlen(non_terminals); i++) {
        printf("FOLLOW(%c): ", non_terminals[i]);
        scanf("%s", follow_sets[i]);
    }
    
    // Construct SLR parsing table
    constructSLRTable();
    
    // Print parsing table
    printf("\nSLR Parsing Table:\n\n");
    printf("State\t\tAction\t\t\tGoto\n");
    printf("\t");
    
    // Print terminal symbols
    for(i = 0; i < strlen(terminals); i++)
        printf("%c\t", terminals[i]);
    
    // Print non-terminal symbols
    for(i = 0; i < strlen(non_terminals); i++)
        printf("%c\t", non_terminals[i]);
    printf("\n");
    
    // Print table contents
    for(i = 0; i < num_items; i++) {
        printf("%d\t", i);
        
        // Print actions
        for(j = 0; j < strlen(terminals); j++)
            printf("%s\t", action[i][j]);
        
        // Print goto entries
        for(j = 0; j < strlen(non_terminals); j++)
            printf("%d\t", goto_table[i][j]);
        printf("\n");
    }
    
    return 0;
}