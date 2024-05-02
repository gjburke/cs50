#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Candidate count
#define candidate_count 4

// locked[i][j] means i is locked in over j
bool locked[candidate_count][candidate_count];

// Function prototypes
bool check_pointing(int i);
int check_loops(int i, int count);

int main(int argc, string argv[]) {
    for (int i = 0; i < candidate_count; i++) {
        for (int j = 0; j < candidate_count; j++) {
            locked[i][j] = false;
        }
    }
    locked[0][1] = true;
    locked[0][2] = true;
    locked[2][3] = true;
    locked[0][3] = true;
    locked[3][1] = true;

    bool work = check_loops(0, 0);
    printf("Loops: %i\n", work);
}

//Checks if candidate i has preference over any candidate
//Takes candidate index i for input
//returns true if the candidate does have preferences over others, false is it doesnt have preferences over others
bool check_pointing(int i) {
    bool pointing = false;
    for (int j = 0; j < candidate_count; j++) {
        if (locked[i][j]) {
            pointing = true;
        }
    }
    return pointing;
}

//Check the lock_pairs array for a loop:
//takes index i for the node on the graph lock_pairs and integer count of arrows from one node to another
//returns 0 if there are no loops, nonzero integer if there are
int check_loops(int i, int count) {
    int loop = 0;
    if (count > candidate_count - 1) {
        loop += 1;
    }
    else if (check_pointing(i)) {
        count += 1;
        for (int j = 0; j < candidate_count; j++) {
            if (locked[i][j]) {
                loop += check_loops(j, count);
            }
        }
    }
    return loop;
}