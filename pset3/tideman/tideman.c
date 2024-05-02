#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
bool check_pointing(int i);
int check_loops(int i, int count);
void lock_pairs(void);
int check_winning(int c);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    //Iterating through each candidate
    for (int i = 0; i < candidate_count; i++) {
        //Finding when the name matches the candidate
        if (strcmp(candidates[i], name) == 0) {
            //Setting that candidate's index to their place in the rank
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    //Iterate over each candidate
    for (int i = 0; i < candidate_count; i++) {
        //Iterate over every candidate after the 'i'th candidate
        for (int j = i + 1; j < candidate_count; j++) {
            //Add one to preferences[i,j], where i and j are the indexes of ranks (ranks[i], ranks[j])
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    //Iterate over all candidates i
    for (int i = 0; i < candidate_count; i++) {
        //Iterate over all candidates j
        for (int j = 0; j < candidate_count; j++) {
            //Check the voter preference one way or the other
            if (preferences[i][j] > preferences[j][i]) {
                //Creating a pair object and assigning the indicies of the winner and loser
                pair p;
                p.winner = i;
                p.loser = j;
                //Adding the pair to the pairs array and incrementing the pair count by one
                pairs[pair_count] = p;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    //Iterate over all elements in the array
    for (int i = 0; i < pair_count; i++) {
        //Instantiate variable for the index of the max
        int index_max = i;
        //Iterate over every pair after the compred pair i
        for (int j = i; j < pair_count; j++) {
            //instantiate strength variable at certain index
            int strength = preferences[pairs[j].winner][pairs[j].loser];
            //compare if the strength of the current pair is greater than the strength of the previous max pair
            if (strength > preferences[pairs[index_max].winner][pairs[index_max].loser]) {
                //assign the max index to the current index
                index_max = j;
            }
        }
        //Switch the pair at max_index with the pair at index i
        pair temp = pairs[i];
        pairs[i] = pairs[index_max];
        pairs[index_max] = temp;
    }
    return;
}

//Checks if candidate i has preference over any candidate
//Takes candidate index i for input
//returns true if the candidate does have preferences over others, false is it doesnt have preferences over others
bool check_pointing(int i) {
    //initalize return varaible as if the node doesn't point anywhere (the candidate doesn't win over anyone)
    bool pointing = false;
    for (int j = 0; j < candidate_count; j++) {
        if (locked[i][j]) {
            //change to true if the candidate does win over at least one person
            pointing = true;
        }
    }
    return pointing;
}

//Check the lock_pairs array for a loop:
//takes index i for the node on the graph lock_pairs and integer count of arrows from one node to another
//returns 0 if there are no loops, nonzero integer if there are
int check_loops(int i, int count) {
    //Instantiate loop variable for the loop count
    int loop = 0;
    //If, while iterating through different branches, the amount of times someone has won over someone
    //else is greater than the candidate count - 1, then you know there is a loop
    if (count > candidate_count - 1) {
        loop += 1;
    }
    //if there isn't a loop, check if that candidate wins over anybody
    else if (check_pointing(i)) {
        //increase the pointer count by one
        count += 1;
        //for all of the candidates that this candidate wins over, call this function on that candidate
        for (int j = 0; j < candidate_count; j++) {
            if (locked[i][j]) {
                //add the loop count to the original call
                loop += check_loops(j, count);
            }
        }
    }
    //return if there are any loops
    return loop;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    //iterate, starting from the first pair, through each pair
    for (int i = 0; i < pair_count; i++) {
        pair p = pairs[i];
        //lock the edge in to the locked graph
        locked[p.winner][p.loser] = true;
        //check if that locked edge creates any loops
        if (check_loops(p.winner, 0) != 0) {
            //remove the edge if it does create a loop
            locked[p.winner][p.loser] = false;
        }
    }
    return;
}

// Checks if the candidate has been won over
// Takes candidate index as input
// Outputs the number of candidates that win over it
int check_winning(int c) {
    int count = 0;
    for (int i = 0; i < candidate_count; i++) {
        if (locked[i][c]) {
            count++;
        }
    }
    return count;
}

// Print the winner of the election
void print_winner(void)
{
    int winning_index = 0;
    for (int i = 1; i < candidate_count; i++) {
        if (check_winning(i) == 0) {
            winning_index = i;
        }
    }
    printf("%s\n", candidates[winning_index]);
    return;
}