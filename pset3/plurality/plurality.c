#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
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
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    //Iterate through each candidate
    for (int i = 0; i < candidate_count; i++) {
        //Check if the name of the vote is the name of the candidate
        if (strcmp(candidates[i].name, name) == 0) {
            //Add to the candidate's votes if its true
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    //Declare variables to track winners
    int winner_indicies[candidate_count];
    int winner_count = 0;

    //Iterate through each candidate
    for (int i = 0, max = 0; i < candidate_count; i++) {
        //Check if the candidate's vote is greater than the largest amount of votes
        if (candidates[i].votes > max) {
            //Set the winner count to one, set the index of that candidate on the winners array, set new max
            winner_count = 1;
            winner_indicies[0] = i;
            max = candidates[i].votes;
        }
        //Check if the candidate has the same number of votes as the largest
        else if (candidates[i].votes == max) {
            //Add one to the winner count and put the index of this candidate on the next index of the winners array
            winner_count++;
            winner_indicies[winner_count - 1] = i;
        }
    }
    //Print all of the winners
    for (int i = 0; i < winner_count; i++) {
        printf("%s\n", candidates[winner_indicies[i]].name);
    }

    return;
}