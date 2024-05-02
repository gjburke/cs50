# Prompt for height (must be 1-8 inclusive)
height = 0
while height < 1 or height > 8:
    try:
        height = int(input("Height: "))
    except:
        height = -1

# Print out the pyramid
for i in range(height):
    for j in range(height-i-1):
        print(" ", end="")
    for k in range(i + 1):
        print("#", end="")
    print("  ", end="")
    for k in range(i + 1):
        print("#", end="")
    print()