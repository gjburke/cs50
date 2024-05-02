from cs50 import get_int

# Get number
number = get_int("Number: ")

# Check length
length = len(str(number))
if not (length == 13 or length == 15 or length == 16):
    print("INVALID\n")
    exit()

# Check sum
# Sum 1 is every other digit from the last digit being added together
# Sum 2 is made from multiplying every other digit starting from second to last digit by two and then adding all of their digits together
sum1 = 0
sum2 = 0
count = 0
for i in reversed(str(number)):
    if count % 2 == 0:
        sum1 += int(i)
    if count % 2 == 1:
        num = int(i) * 2
        while num > 0:
            sum2 += num % 10
            num = int(num / 10)
    count += 1

# If the sumer of sum1 and sum2 doesn't end in zero, it's invalid
if not ((sum1 + sum2) % 10 == 0):
    print("INVALID\n")
    exit()

# Check type based on first numbers
firstDigit = int(str(number)[:1])
firstTwoDigits = int(str(number)[:2])

if (firstDigit == 4):
    print("VISA\n")
elif (firstTwoDigits == 34 or firstTwoDigits == 37):
    print("AMEX\n")
elif (firstTwoDigits == 51 or firstTwoDigits == 52 or firstTwoDigits == 53 or firstTwoDigits == 54 or firstTwoDigits == 55):
    print("MASTERCARD\n")
else:
    print("INVALID\n")