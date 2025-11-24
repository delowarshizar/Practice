#include <stdio.h>
#include <string.h>

#define NUM_STATES 7
#define NUM_INPUTS 3

// DFA states
enum { D0, D1, D2, D3, D4, D5, DEAD };

// Input mapping: a -> 0, b -> 1, other -> 2
int get_input(char c) {
    if (c == 'a') return 0;
    if (c == 'b') return 1;
    return 2; // invalid input
}

// Accepting states with token labels
const char* accepting_tokens[NUM_STATES] = {
    "",        // D0 ->  accepting
    "a*",        // D1
    "a*b+",      // D2
    "a*",      // D3
    "a*b+", // D4
    "a*b+,abb",
    NULL         // DEAD
};

int main() {
    // DFA transition table: next_state[current_state][input_symbol]
    int next_state[NUM_STATES][NUM_INPUTS] = {
        /* D0 */ { D1, D2, DEAD },
        /* D1 */ { D3, D4, DEAD },
        /* D2 */ { DEAD, D2, DEAD },
        /* D3 */ { D3, D2, DEAD },
        /* D4 */ { DEAD, D5, DEAD },
        /* D5 */ { DEAD, D2, DEAD },
        /* DEAD */{ DEAD, DEAD, DEAD }
    };

    char str[100];
    printf("Enter a string: ");
    scanf("%s", str);

    int state = D0;
    for (int i = 0; str[i] != '\0'; i++) {
        int input = get_input(str[i]);
        state = next_state[state][input];
        if (state == DEAD) break;
    }

    // Check if the final state is accepting
    if (accepting_tokens[state] != NULL) {
        printf("%s Accepted by DFA: %s\n", str, accepting_tokens[state]);
    } else {
        printf("%s Rejected by all patterns\n", str);
    }

    return 0;
}
