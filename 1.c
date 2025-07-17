/* 1.	Write C program to construct non recursive predictive parsing table.
        Note:  the input to the program  is a CFG with no left recursion and left factored grammar  the program will calculate FIST and follow set inside and also do the construction of parsing table.
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_PROD 10
#define MAX_LEN 20

// Structure to store production rules
struct Production {
    char lhs;
    char rhs[MAX_LEN];
};

// Structure to store FIRST and FOLLOW sets
struct Sets {
    char first[MAX_LEN];
    char follow[MAX_LEN];
};

struct Production prod[MAX_PROD];
struct Sets sets[26];  // For all uppercase letters
char terminals[MAX_LEN];
char non_terminals[MAX_LEN];
char parsing_table[26][26];
int num_prod = 0;

// Function to calculate FIRST set
void calculateFirst(char symbol) {
    int i, j;
    
    for (i = 0; i < num_prod; i++) {
        if (prod[i].lhs == symbol) {
            if (islower(prod[i].rhs[0]) || prod[i].rhs[0] == '$') {
                // Add terminal to FIRST set
                int len = strlen(sets[symbol - 'A'].first);
                sets[symbol - 'A'].first[len] = prod[i].rhs[0];
                sets[symbol - 'A'].first[len + 1] = '\0';
            } else {
                // Calculate FIRST of first symbol in RHS
                calculateFirst(prod[i].rhs[0]);
            }
        }
    }
}

// Function to calculate FOLLOW set
void calculateFollow() {
    int i, j;
    
    // Add $ to FOLLOW set of start symbol
    sets[prod[0].lhs - 'A'].follow[0] = '$';
    sets[prod[0].lhs - 'A'].follow[1] = '\0';
    
    for (i = 0; i < num_prod; i++) {
        for (j = 0; j < strlen(prod[i].rhs) - 1; j++) {
            if (isupper(prod[i].rhs[j])) {
                // If terminal follows, add it to FOLLOW
                if (islower(prod[i].rhs[j + 1])) {
                    int len = strlen(sets[prod[i].rhs[j] - 'A'].follow);
                    sets[prod[i].rhs[j] - 'A'].follow[len] = prod[i].rhs[j + 1];
                    sets[prod[i].rhs[j] - 'A'].follow[len + 1] = '\0';
                }
                // If non-terminal follows, add its FIRST set
                else if (isupper(prod[i].rhs[j + 1])) {
                    strcat(sets[prod[i].rhs[j] - 'A'].follow, 
                           sets[prod[i].rhs[j + 1] - 'A'].first);
                }
            }
        }
    }
}

// Function to construct parsing table
void constructParsingTable() {
    int i, j;
    
    // Initialize parsing table with empty entries
    for (i = 0; i < 26; i++)
        for (j = 0; j < 26; j++)
            parsing_table[i][j] = ' ';
            
    for (i = 0; i < num_prod; i++) {
        char non_term = prod[i].lhs;
        char first_sym = prod[i].rhs[0];
        
        if (islower(first_sym) || first_sym == '$') {
            int row = non_term - 'A';
            int col = first_sym - 'a';
            parsing_table[row][col] = i + '0';  // Store production number
        }
        // Handle non-terminals and epsilon cases here
    }
}

int main() {
    int i;
    printf("Enter number of productions: ");
    scanf("%d", &num_prod);
    
    printf("Enter productions (E->aB format):\n");
    for (i = 0; i < num_prod; i++) {
        char temp[MAX_LEN];
        scanf("%s", temp);
        prod[i].lhs = temp[0];
        strcpy(prod[i].rhs, &temp[3]);
    }
    
    // Calculate FIRST sets
    for (i = 0; i < num_prod; i++) {
        if (isupper(prod[i].lhs))
            calculateFirst(prod[i].lhs);
    }
    
    // Calculate FOLLOW sets
    calculateFollow();
    
    // Construct parsing table
    constructParsingTable();
    
    // Print parsing table
    printf("\nParsing Table:\n");
    printf("   ");
    for (i = 0; i < strlen(terminals); i++)
        printf("%c  ", terminals[i]);
    printf("\n");
    
    for (i = 0; i < strlen(non_terminals); i++) {
        printf("%c  ", non_terminals[i]);
        for (int j = 0; j < strlen(terminals); j++) {
            printf("%c  ", parsing_table[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}