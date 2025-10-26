#include <stdio.h>
#include <string.h>

#define NUM_STATES 10
#define NUM_INPUTS 4

// DFA states (index order matters for the tables below)
enum { D0, D1, D2, D3, D4, D5, D6, D7, D8, DEAD };

// Map input char -> column in transition table
// a -> 0, b -> 1, c -> 2, other -> 3 (invalid)
int get_input(char c) {
    if (c == 'a') return 0;
    if (c == 'b') return 1;
    if (c == 'c') return 2;
    return 3;
}

// Accepting states (NULL => not accepting)
// Based on your construction: accepting = D1, D2, D5, D6, D7, D8
// Labels show which pattern(s) they satisfy.
const char* accepting_tokens[NUM_STATES] = {
    /* D0   */ NULL,
    /* D1   */ NULL,
    /* D2   */ "a*b+ | c*a*b+",
    /* D3   */ NULL,
    /* D4   */ NULL,
    /* D5   */ "a*b+ | c*a*b+",
    /* D6   */ "ab*c+",
    /* D7   */ "c*a*b+",
    /* D8   */ "ab*c+",
    /* DEAD */ NULL
};

int main(void) {
    // next_state[current_state][input]  (input: a, b, c, invalid)
    int next_state[NUM_STATES][NUM_INPUTS] = {
        /* D0   */ { D1,  D2,  D3,  DEAD },
        /* D1   */ { D4,  D5,  D6,  DEAD },
        /* D2   */ { DEAD,D2,  DEAD,DEAD },
        /* D3   */ { D3,  D7,  D3,  DEAD },
        /* D4   */ { D4,  D2,  D3,  DEAD },
        /* D5   */ { DEAD,D5,  D8,  DEAD },
        /* D6   */ { D3,  D7,  D6,  DEAD },
        /* D7   */ { DEAD,D7,  D3,  DEAD },
        /* D8   */ { DEAD,DEAD,D8,  DEAD },
        /* DEAD */ { DEAD,DEAD,DEAD,DEAD }
    };

    char str[256];
    printf("Enter a string: ");
    if (scanf("%255s", str) != 1) return 0;

    int state = D0;
    for (int i = 0; str[i] != '\0'; i++) {
        int input = get_input(str[i]);
        state = next_state[state][input];
        if (state == DEAD) break;
    }

    if (accepting_tokens[state] != NULL) {
        printf("Accepted: %s\nMatched pattern(s): %s\n", str, accepting_tokens[state]);
    } else {
        printf("Rejected: %s\n", str);
    }
    return 0;
}

