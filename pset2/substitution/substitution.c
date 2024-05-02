#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char convert(char text, string k);
bool checkKey(string k);

int main(int argc, string argv[]) {
    //Only execute the actual function if the key is 26 characters long and there are two arguments
    if (argc != 2) {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if (strlen(argv[1]) != 26) {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else if (!checkKey(argv[1])) {
        printf("Key cannot contain non-alphabetic or duplicate letters\n");
        return 1;
    }
    else {
        //Declaring the key as the second argument
        string key = argv[1];
        //Getting the user to input the plaintext and finding its length
        string plaintext = get_string("plaintext:  ");
        int length = strlen(plaintext);
        //Set ciphertext as plaintext, will be changed character by character later
        string ciphertext = plaintext;

        //Iterate over each character of the ciphertext, convert to cipher with key
        for (int i = 0; i < length; i++) {
            ciphertext[i] = convert(plaintext[i], key);
        }

        //Print ciphertet
        printf("ciphertext: %s\n", ciphertext);
    }
}

//Converts a singular character of plaintext to a charcter of ciphertext
char convert(char c, string k) {
    //Instantiate char for returning as original text
    char cipher = c;
    if (isupper(c)) {
        //Index at c - 65: converting ascii to index of key ('A' == 65)
        //Convert to upper because key could be in lower
        cipher = toupper(k[c - 65]);
    }
    else if (islower(c)) {
        //Index at c - 98: converting ascii to index of key ('a' == 98)
        //Convert to lower because key could be in upper
        cipher = tolower(k[c - 97]);
    }
    return cipher;
}

//Returns true if the key is valid
bool checkKey(string k) {
    //Iterating through each characetr of the key
    for (int i = 0, len = strlen(k); i < len; i++) {
        //defining char of the key that is at the current index i
        //Use toupper to standardize the case
        char c = toupper(k[i]);
        //check if it is an alphabetical letter
        if (!isalpha(c)) {
            return false;
        }
        //Iterate over every character in the key
        for (int j = 0, count = 0; j < len; j++) {
            //Compare the characters, with standardized upper
            if (c == toupper(k[j])) {
                count++;
            }
            //Check if the count is greater than one, then there is a duplicate
            //Can be one since it will count the original character
            if (count > 1) {
                return false;
            }
        }
    }
    return true;
}