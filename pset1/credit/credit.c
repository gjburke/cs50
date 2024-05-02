#include <cs50.h>
#include <stdio.h>
#include <math.h>

int addDigits(int n);

int main(void) {

    //Setting up a boolean that is set to false anytime it fails a check
    bool isCard = true;

    //Getting number from user
    long number = get_long("Number: ");

    //Checklength
    long temp = number;
    int length = 0;
    while (temp > 0) {
        temp /= 10;
        length++; //Incrementing length for each digit
    }
    //Check if length is valid, if not set the boolean to false
    if (!(length == 13 || length == 15 || length == 16)) {
        isCard = false;
    }

    //Checksum
    //Sum 1 is every other digit from the last digit being added together
    //Sum 2 is made from multiplying every other digit starting from second to last digit by two and then adding all of their digits together
    int sum1 = 0, sum2 = 0;
    temp = number;
    while (temp > 0) {
        sum1 += temp % 10;
        temp /= 10;
        sum2 += addDigits((temp % 10) * 2);
        temp /= 10;
    }
    //If the sum of sum1 and sum2 doesn't end in zero, the credit card is not valid
    if ((sum1 + sum2) % 10 != 0) {
        isCard = false;
    }

    //Checktype
    temp = number;
    //Using the length variable to get the first and first two digits of the card number
    int firstDigit = number / pow(10, length - 1);
    int firstTwoDigits = number / pow(10, length - 2);
    //Using if statements to evaluate which card it is depending on the first or first two digits
    if (isCard) {
        if (firstDigit == 4) {
            printf("VISA\n");
        }
        else if (firstTwoDigits == 34 || firstTwoDigits == 37) {
            printf("AMEX\n");
        }
        else if (firstTwoDigits == 51 || firstTwoDigits == 52 || firstTwoDigits == 53 || firstTwoDigits == 54 || firstTwoDigits == 55) {
            printf("MASTERCARD\n");
        }
        else {
            printf("INVALID\n");
        }
    }
    else {
        printf("INVALID\n");
    }
}

//This method takes in a number, adds their digits, and spits out the sum
int addDigits(int n) {
    int sum = 0;
    while(n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}