/* 4.	Write C program to construct  LR(0) set of items.
Note:  the input to the program  is  augmented grammar and you have to implement  closer, goto and set of  item algorithm.
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_PROD 20
#define MAX_LEN 50
#define MAX_ITEMS 100

// Structure for grammar productions
struct Production {
    char lhs;
    char rhs[MAX_LEN];
};

// Structure for LR(0) items
struct Item {
    char lhs;
    char rhs[MAX_LEN];
    int dot_pos;
};

// Global variables
struct Production grammar[MAX_PROD];
struct Item item_sets[MAX_ITEMS][MAX_ITEMS];
int num_prods = 0;
int num_sets = 0;
int items_in_set[MAX_ITEMS] = {0};

// Function to add item to set if not already present
int addToSet(struct Item item, struct Item set[], int *count) {
    for(int i = 0; i < *count; i++) {
        if(set[i].lhs == item.lhs && 
           strcmp(set[i].rhs, item.rhs) == 0 && 
           set[i].dot_pos == item.dot_pos)
            return 0;
    }
    set[*count] = item;
    (*count)++;
    return 1;
}

// Function to compute closure of item set
void closure(struct Item set[], int *count) {
    int added;
    do {
        added = 0;
        for(int i = 0; i < *count; i++) {
            char next = set[i].rhs[set[i].dot_pos];
            if(isupper(next)) {  // Non-terminal after dot
                for(int j = 0; j < num_prods; j++) {
                    if(grammar[j].lhs == next) {
                        struct Item new_item;
                        new_item.lhs = grammar[j].lhs;
                        strcpy(new_item.rhs, grammar[j].rhs);
                        new_item.dot_pos = 0;
                        if(addToSet(new_item, set, count))
                            added = 1;
                    }
                }
            }
        }
    } while(added);
}

// Function to compute GOTO
void computeGoto(struct Item from_set[], int from_count, 
                char symbol, struct Item to_set[], int *to_count) {
    *to_count = 0;
    
    // Find items where dot is before symbol
    for(int i = 0; i < from_count; i++) {
        if(from_set[i].rhs[from_set[i].dot_pos] == symbol) {
            struct Item new_item = from_set[i];
            new_item.dot_pos++;
            addToSet(new_item, to_set, to_count);
        }
    }
    
    // Compute closure of resulting set
    closure(to_set, to_count);
}

// Function to print an item
void printItem(struct Item item) {
    printf("%c -> ", item.lhs);
    for(int i = 0; i < strlen(item.rhs); i++) {
        if(i == item.dot_pos)
            printf(".");
        printf("%c", item.rhs[i]);
    }
    if(item.dot_pos == strlen(item.rhs))
        printf(".");
    printf("\n");
}

int main() {
    printf("Enter number of productions: ");
    scanf("%d", &num_prods);
    
    printf("Enter productions (format: E->aB):\n");
    for(int i = 0; i < num_prods; i++) {
        char temp[MAX_LEN];
        scanf("%s", temp);
        grammar[i].lhs = temp[0];
        strcpy(grammar[i].rhs, &temp[3]);
    }
    
    // Create initial item
    struct Item initial_item;
    initial_item.lhs = grammar[0].lhs;
    strcpy(initial_item.rhs, grammar[0].rhs);
    initial_item.dot_pos = 0;
    
    // Create first set with initial item
    addToSet(initial_item, item_sets[0], &items_in_set[0]);
    closure(item_sets[0], &items_in_set[0]);
    num_sets = 1;
    
    // Compute all item sets
    int processed = 0;
    while(processed < num_sets) {
        // Try GOTO with all symbols
        for(char sym = 'A'; sym <= 'z'; sym++) {
            struct Item new_set[MAX_ITEMS];
            int new_count = 0;
            
            computeGoto(item_sets[processed], items_in_set[processed], 
                       sym, new_set, &new_count);
            
            if(new_count > 0) {
                // Check if this set already exists
                int exists = 0;
                for(int i = 0; i < num_sets; i++) {
                    if(items_in_set[i] == new_count) {
                        int same = 1;
                        for(int j = 0; j < new_count; j++) {
                            if(new_set[j].lhs != item_sets[i][j].lhs || 
                               strcmp(new_set[j].rhs, item_sets[i][j].rhs) != 0 ||
                               new_set[j].dot_pos != item_sets[i][j].dot_pos) {
                                same = 0;
                                break;
                            }
                        }
                        if(same) {
                            exists = 1;
                            break;
                        }
                    }
                }
                
                if(!exists) {
                    // Add new set
                    memcpy(item_sets[num_sets], new_set, 
                           sizeof(struct Item) * new_count);
                    items_in_set[num_sets] = new_count;
                    num_sets++;
                }
            }
        }
        processed++;
    }
    
    // Print all item sets
    printf("\nLR(0) Item Sets:\n");
    for(int i = 0; i < num_sets; i++) {
        printf("\nI%d:\n", i);
        for(int j = 0; j < items_in_set[i]; j++) {
            printItem(item_sets[i][j]);
        }
    }
    
    return 0;
}