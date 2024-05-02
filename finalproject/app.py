import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
import random

# Configure application
app = Flask(__name__)

# Initializes a board, takes parameter to determine who your playing as (0 for black, any other number for white)
def newBoard():
    board = [[" " for i in range(3)] for j in range(3)]
    return board

# CheckWin: checks the board to see if either side has won, returns their number if so, if not then zero or negative one for tie
def checkWin(board):

    for i in range(3):
        # Check Rows
        if board[i][0] == board[i][1] and board[i][1] == board[i][2] and board[i][0] != " ":
            return board[i][0]
        # Check Columns
        if board[0][i] == board[1][i] and board[1][i] == board[2][i] and board[0][i] != " ":
            return board[0][i]
    # Check Diagonals
    if board[0][0] == board[1][1] and board[1][1] == board[2][2] and board[0][0] != " ":
        return board[0][0]
    if board[0][2] == board[1][1] and board[1][1] == board[2][0] and board[0][2] != " ":
        return board[0][0]

    # Check for tie
    zeros = False
    for i in range(3):
        for j in range(3):
            if board[i][j] == " ":
                zeros = True
    if not zeros:
        return "No One! It's a Tie!"

    return 0

# Initialize a board
board = newBoard()

# Set current player
currentPlayer = "X"

# Global winner variable instantiation
winner = " "

# Set indexes
indRow = ["T", "M", "B"]
indCol = ["L", "C", "R"]

# Render stuff
@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        # Defining currentPlayer as global because of semantics
        global currentPlayer
        global board
        global winner
        # Get the index that the user inputted
        index = request.form.get("spot")
        # Log the user's input
        board[int(index[0])][int(index[1])] = currentPlayer
        # Check if there is a win
        winner = checkWin(board)
        if winner:
            return redirect("/win")
        # Change the current player
        if currentPlayer == "X":
            currentPlayer = "O"
        else:
            currentPlayer = "X"
        return redirect("/")
    else:
        # How to make it so user can't place where another has placed already?
        # My possible solution, remove the buttons corresponding to the spots that are already filled (should be able to use if statements in jinja)
        return render_template("game.html", board = board, indRow = indRow, indCol = indCol)

@app.route("/win", methods=["GET", "POST"])
def win():
    if request.method == "POST":
        return redirect("/")
    else:
        global board
        global winner
        board = newBoard()
        return render_template("win.html", winner = winner)

