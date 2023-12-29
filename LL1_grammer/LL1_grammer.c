#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_RULES 10
#define MAX_SYMBOLS 10

typedef struct {
    char nonTerminal;
    char rule[MAX_SYMBOLS];
} ProductionRule;

typedef struct {
    ProductionRule rules[MAX_RULES];
    int count;
} Grammar;

int isNonTerminal(char symbol) {
    return (symbol >= 'A' && symbol <= 'Z');
}

int isTerminal(char symbol) {
    return !isNonTerminal(symbol);
}

void addSymbol(char array[], char symbol) {
    if (strchr(array, symbol) == NULL) {
        int length = strlen(array);
        array[length] = symbol;
        array[length + 1] = '\0';
    }
}

void addSet(char array[], char set[]) {
    int i;
    for (i = 0; set[i] != '\0'; ++i) {
        addSymbol(array, set[i]);
    }
}

int isNullable(char symbol) {
    return symbol == 'e';
}

int isEpsilon(char symbol) {
    return symbol == 'e';
}


int isLL1Grammar(Grammar grammar) {
    int i, j, k;
    char firstSets[MAX_RULES][MAX_SYMBOLS] = {0};  // Array to store first sets of each non-terminal
    char followSets[MAX_RULES][MAX_SYMBOLS] = {0}; // Array to store follow sets of each non-terminal

    // Initialize follow sets with '$' for start symbol
    addSymbol(followSets[grammar.rules[0].nonTerminal - 'A'], '$');

    // Calculate first sets
// Calculate first sets
for (i = 0; i < grammar.count; ++i) {
    char nonTerminal = grammar.rules[i].nonTerminal;
    char rule[MAX_SYMBOLS];
    strcpy(rule, grammar.rules[i].rule);

    j = 0;

    while (rule[j] != '\0') {
        if (isNonTerminal(rule[j])) {
            int k = j;
            int nullable = 1;
            while (isNullable(rule[k]) && rule[k] != '\0') {
                addSet(firstSets[nonTerminal - 'A'], firstSets[rule[k] - 'A']);
                ++k;
                nullable = 0;
            }
            if (isTerminal(rule[k]) || isEpsilon(rule[k])) {
                addSymbol(firstSets[nonTerminal - 'A'], rule[k]);
            }
            if (nullable) {
                addSymbol(firstSets[nonTerminal - 'A'], 'e');
            }
        } else if (isTerminal(rule[j]) || isEpsilon(rule[j])) {
            addSymbol(firstSets[nonTerminal - 'A'], rule[j]);
            break;
        }
        ++j;
    }
}



    // Calculate follow sets
    int changes;
    do {
        changes = 0;

        for (i = 0; i < grammar.count; ++i) {
            char nonTerminal = grammar.rules[i].nonTerminal;
            char rule[MAX_SYMBOLS];
            strcpy(rule, grammar.rules[i].rule);

            j = 0;

            while (rule[j] != '\0') {
                if (isNonTerminal(rule[j])) {
                    int k = j + 1;
                    while (rule[k] != '\0') {
                        if (isNonTerminal(rule[k])) {
                            int before = strlen(followSets[rule[j] - 'A']);
                            addSet(followSets[rule[j] - 'A'], firstSets[rule[k] - 'A']);
                            int after = strlen(followSets[rule[j] - 'A']);
                            if (before != after) {
                                changes = 1;
                            }
                            if (!isEpsilon(firstSets[rule[k] - 'A'])) {
                                break;
                            }
                        } else if (isTerminal(rule[k]) || isEpsilon(rule[k])) {
                            int before = strlen(followSets[rule[j] - 'A']);
                            addSymbol(followSets[rule[j] - 'A'], rule[k]);
                            int after = strlen(followSets[rule[j] - 'A']);
                            if (before != after) {
                                changes = 1;
                            }
                            if (!isEpsilon(rule[k])) {
                                break;
                            }
                        }
                        ++k;
                    }
                }
                ++j;
            }
        }
    } while (changes);

    // Check if first sets and follow sets are disjoint for each non-terminal
    for (i = 0; i < grammar.count; ++i) {
        char nonTerminal = grammar.rules[i].nonTerminal;
        for (j = 0; firstSets[nonTerminal - 'A'][j] != '\0'; ++j) {
            if (strchr(followSets[nonTerminal - 'A'], firstSets[nonTerminal - 'A'][j]) != NULL) {
                return 0;
            }
        }
    }

    return 1;
}

void printGrammar(Grammar grammar) {
    printf("Grammar Rules:\n");
    for (int i = 0; i < grammar.count; ++i) {
        printf("%c -> %s\n", grammar.rules[i].nonTerminal, grammar.rules[i].rule);
    }
}

int main() {
    Grammar grammar;
    int i;

    printf("Enter the number of production rules: ");
    scanf("%d", &grammar.count);

    printf("Enter the production rules (e.g., A->aB): \n");
    for (i = 0; i < grammar.count; ++i) {
        printf("Rule %d: ", i + 1);
        scanf("%s", grammar.rules[i].rule);
        grammar.rules[i].nonTerminal = grammar.rules[i].rule[0];
    }

    printf("\n");

    printGrammar(grammar);

    if (isLL1Grammar(grammar)) {
        printf("\nThe given grammar is LL(1).\n");
    } else {
        printf("\nThe given grammar is not LL(1).\n");
    }

    return 0;
}
