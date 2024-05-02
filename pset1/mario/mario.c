#include <cs50.h>
#include <stdio.h>

void printNTimes(string str, int n);

int main(void)
{
    int height;
    // Ask for the height until the height is between 1 and 8
    do {
        height = get_int("Height: ");
    } while (!(height >= 1 && height <= 8));

    //Iterating from i = 1 to i = height, i is the amount of bricks in each row
    for (int i = 1; i <= height; i++) {
        //Print spaces, then brick, then two spaces, then the bricks again
        printNTimes(" ", height - i);
        printNTimes("#", i);
        printf("  ");
        printNTimes("#", i);
        //Move to new line to start next layer of bricks
        printf("\n");
    }
}

// Defining a functoin that prints a string N times
void printNTimes(string str, int n) {
    for (int i = 0; i < n; i++) {
        printf("%s", str);
    }
}