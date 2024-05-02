import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    # Read database file into a variable
    f1 = open(sys.argv[1])
    data = csv.DictReader(f1)

    # Read DNA sequence file into a variable
    f2 = open(sys.argv[2])
    sequence = f2.read()

    # Find longest match of each STR in DNA sequence
    longest = []
    keys = data.fieldnames
    for i in range(1, len(keys)):
        longest.append(longest_match(sequence, keys[i]))

    # Check database for matching profiles
    matches = []
    # Iterate over every person
    for person in data:
        match = True
        # Check over every longest match, if any are not equal break and go to next person
        for i in range(len(keys) - 1):
            if int(person[keys[i + 1]]) != int(longest[i]):
                match = False
                break
        # if match is never set to false, then it must be true that it matches
        if match == True:
            matches.append(person[keys[0]])

    # Print the names
    if len(matches) == 0:
        print("No match")
    else:
        for people in matches:
            print(people)

    # Close files
    f1.close()
    f2.close()

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
