#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void) {
    //Get the text
    string input = get_string("Text: ");
    //Get the number of lettters, words, and sentences
    int numLetters = count_letters(input);
    int numWords = count_words(input);
    int numSentences = count_sentences(input);

    //Compute L (number of letters per 100 words) and S (number of sentences per 100 words)
    double L = ((double) numLetters / numWords) * 100;
    double S = ((double) numSentences / numWords) * 100;
    //Compute the index using L and S
    double index = 0.0588 * L - 0.296 * S - 15.8;
    //Round the index to get the grade
    int grade = (int) round(index);

    //Print the grade
    if (grade >= 1 && grade < 16) {
        printf("Grade %i\n", grade);
    }
    else if (grade >= 16) {
        printf("Grade 16+\n");
    }
    else {
        printf("Before Grade 1\n");
    }
}

int count_letters(string text) {
    //Find length and intantiate letter count
    int length = strlen(text);
    int letterCount = 0;
    //Iterate over each character
    for (int i = 0; i < length; i++) {
        //If a character is a letter, add to the letter count
        if (islower(text[i]) || isupper(text[i])) {
            letterCount++;
        }
    }
    return letterCount;
}

int count_words(string text) {
    //Find length and intantiate word count
    int length = strlen(text);
    int wordCount = 0;
    //Iterate over each character
    for (int i = 0; i < length; i++) {
        //If there is a space, we know the characters before it were a word
        if (text[i] == ' ') {
            wordCount++;
        }
    }
    //Add one in the end to account for the last word - no space after
    return wordCount + 1;
}

int count_sentences(string text) {
    //Find length and intantiate sentence count
    int length = strlen(text);
    int sentenceCount = 0;
    //Iterate over each character
    for (int i = 0; i < length; i++) {
        //If there is a . ! ? then it is ending a sentence, so we add to sentecne counter
        if (text[i] == '.' || text[i] == '!' || text[i] == '?') {
            sentenceCount++;
        }
    }
    return sentenceCount;
}
