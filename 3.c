/* 3.	Write C program to construct  Canonical LR predictive parsing table
    Note:  the input to the program  is LR(1) items. The program    will create the table.
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_ITEMS 100
#define MAX_SYMBOLS 26
#define MAX_STATES 100
#define MAX_LEN 20

// Structure for LR(1) items
struct LR1Item {
    char lhs;               // Left-hand side
    char rhs[MAX_LEN];     // Right-hand side
    int dot_pos;           // Position of dot
    char lookahead;        // Lookahead symbol
};

// Structure for state
struct State {
    struct LR1Item items[MAX_ITEMS];
    int num_items;
};

// Global variables
struct State states[MAX_STATES];
int num_states = 0;
char terminals[MAX_LEN];
char non_terminals[MAX_LEN];
char parsing_table[MAX_STATES][MAX_SYMBOLS][3];  // Action/Goto table

// Initialize parsing table with empty entries
void initParsingTable() {
    for(int i = 0; i < MAX_STATES; i++) {
        for(int j = 0; j < MAX_SYMBOLS; j++) {
            parsing_table[i][j][0] = ' ';
            parsing_table[i][j][1] = ' ';
            parsing_table[i][j][2] = '\0';
        }
    }
}

// Add shift action to parsing table
void addShiftAction(int state, char symbol, int next_state) {
    int col = (islower(symbol) ? symbol - 'a' : symbol - 'A');
    parsing_table[state][col][0] = 's';
    parsing_table[state][col][1] = next_state + '0';
}

// Add reduce action to parsing table
void addReduceAction(int state, char symbol, int prod_num) {
    int col = (islower(symbol) ? symbol - 'a' : symbol - 'A');
    parsing_table[state][col][0] = 'r';
    parsing_table[state][col][1] = prod_num + '0';
}

// Add goto action to parsing table
void addGotoAction(int state, char symbol, int next_state) {
    int col = symbol - 'A';
    parsing_table[state][col][0] = 'g';
    parsing_table[state][col][1] = next_state + '0';
}

// Construct CLR parsing table
void constructCLRTable() {
    initParsingTable();
    
    // Process each state
    for(int i = 0; i < num_states; i++) {
        // Process each item in the state
        for(int j = 0; j < states[i].num_items; j++) {
            struct LR1Item item = states[i].items[j];
            char next_symbol = item.rhs[item.dot_pos];
            
            if(next_symbol != '\0') {  // Dot not at end
                // Add shift action for terminals
                if(islower(next_symbol)) {
                    addShiftAction(i, next_symbol, i + 1);
                }
                // Add goto action for non-terminals
                else if(isupper(next_symbol)) {
                    addGotoAction(i, next_symbol, i + 1);
                }
            }
            else {  // Dot at end - reduce action
                addReduceAction(i, item.lookahead, j);
            }
        }
    }
}

int main() {
    int i, j;
    printf("Enter number of states: ");
    scanf("%d", &num_states);
    
    // Input states and their items
    for(i = 0; i < num_states; i++) {
        printf("\nState %d\n", i);
        printf("Enter number of items in state: ");
        scanf("%d", &states[i].num_items);
        
        printf("Enter items (format: A->aB.,c where c is lookahead):\n");
        for(j = 0; j < states[i].num_items; j++) {
            char temp[MAX_LEN];
            scanf("%s", temp);
            
            // Parse the input
            states[i].items[j].lhs = temp[0];
            int k = 0;
            while(temp[k+3] != ',' && temp[k+3] != '\0') {
                states[i].items[j].rhs[k] = temp[k+3];
                k++;
            }
            states[i].items[j].rhs[k] = '\0';
            states[i].items[j].dot_pos = strchr(temp, '.') - temp - 3;
            states[i].items[j].lookahead = temp[k+4];
        }
    }
    
    // Construct CLR parsing table
    constructCLRTable();
    
    // Print parsing table
    printf("\nCLR Parsing Table:\n\n");
    printf("State\t");
    
    // Print terminal symbols
    for(i = 0; i < strlen(terminals); i++)
        printf("%c\t", terminals[i]);
    
    // Print non-terminal symbols
    for(i = 0; i < strlen(non_terminals); i++)
        printf("%c\t", non_terminals[i]);
    printf("\n");
    
    // Print table contents
    for(i = 0; i < num_states; i++) {
        printf("%d\t", i);
        
        // Print actions for terminals
        for(j = 0; j < strlen(terminals); j++)
            printf("%s\t", parsing_table[i][j]);
        
        // Print goto actions for non-terminals
        for(j = 0; j < strlen(non_terminals); j++)
            printf("%s\t", parsing_table[i][strlen(terminals) + j]);
        printf("\n");
    }
    
    return 0;
}