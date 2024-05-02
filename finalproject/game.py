import random

# Main
def main():
    # Initialize a board
    board = newBoard()
    # Show players
    print("Player 1: Xs")
    print("Player 2: Os")
    # Pick random person to start
    currentPlayer = random.randint(1, 2)
    #Play the game until there is a winner
    winner = 0
    while not winner:
        board = move(currentPlayer, board)
        display(board)
        winner = checkWin(board)
        
    # Show tie if tie
    if winner == -1:
        print("It's a tie!")
    # Show the winner
    else:
        print("The winner is player " + str(winner) + "!")

    return

# Move: takes the player number as input and prompts them to make a move, takes valid input and modifies the board for that move
def move(player, board):
    while True:
        row = int(input("Player " + str(player) + " row: "))
        column = int(input("Player " + str(player) + " column: "))
        if (board[row][column] == 0):
            board[row][column] = player
            break
        else:
            print("Space not available")
    return board

# CheckWin: checks the board to see if either side has won, returns their number if so, if not then zero or negative one for tie
def checkWin(board):

    for i in range(3):
        # Check Rows
        if board[i][0] == board[i][1] and board[i][1] == board[i][2]:
            return board[i][0]
        # Check Columns
        if board[0][i] == board[1][i] and board[1][i] == board[2][i]:
            return board[0][i]
    # Check Diagonals
    if board[0][0] == board[1][1] and board[1][1] == board[2][2]:
        return board[0][0]
    if board[0][2] == board[1][1] and board[1][1] == board[2][0]:
        return board[0][0]

    # Check for tie
    zeros = False
    for i in range(3):
        for j in range(3):
            if board[i][j] == 0:
                zeros = True
    if not zeros:
        return -1

    return 0

# Displays the board state that is fed in as a parameter
def display(board):
    # Iterates through each row, then each element in the row
    for row in board:
        print(" ", end = "")
        for element in row:
            # Displays the stored values of 1 and 2 for Xs and Os as actual Xs and Os
            if element != 1 and element != 2:
                spot = " "
            else:
                if element == 1:
                    spot = "X"
                else:
                    spot = "O"
            print(str(spot) + " | ", end = "")
        print("\n------------")
    return

# Initializes a board, takes parameter to determine who your playing as (0 for black, any other number for white)
def newBoard():
    board = [[0 for i in range(3)] for j in range(3)]
    return board

if __name__ == "__main__":
    main()