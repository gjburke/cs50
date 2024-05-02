import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

# Adding datetime for the datetime of the stock purchase
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Get a list of all the stocks the user owns, combining seperate purchases under one stock name
    dbStocks = db.execute("SELECT name, symbol, shares FROM portfolio WHERE user_id = ?", session["user_id"])

    # Get user's cash balance, total stock value
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    totalStockValue = 0

    for stock in dbStocks:
        stockPrice = lookup(stock["symbol"])["price"]
        totalStockValue += stockPrice * stock["shares"]

    # Append the stock's price to each dictionary that represents each stock in dbStocks
    stocks = []
    for stock in dbStocks:
        stock.update({"price": lookup(stock["symbol"])["price"]})
        stocks.append(stock)

    return render_template("index.html", stocks = stocks, cash = cash, totalStockValue = totalStockValue)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        # Get the data the user inputted
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Check if user inputted a symbol
        if not symbol:
            return apology("No symbol inputted")

        # Get the related stock
        stock = lookup(symbol)

        # Apologies for wrong inputs
        if not stock:
            return apology("No stock associated with symbol")
        try:
            shareCount = int(shares)
            if shareCount <= 0:
                return apology("Share count must be a positive integer")
        except ValueError:
            return apology("Share count must be a positive integer")

        # Look up how much cash the user has, check and see if they can actually purchas that much stock
        userCash = db.execute("SELECT cash FROM users where id = ?", session["user_id"])[0]["cash"]
        overallCost = stock["price"] * shareCount

        if (userCash < overallCost):
            return apology("Not enough cash to buy all that stock")

        # Add user's purcahse to the transactions database as a "buy"
        db.execute("INSERT INTO transactions (type, name, symbol, price, shares, time, user_id) VALUES (?, ?, ?, ?, ?, ?, ?)", "buy", stock["name"], stock["symbol"], stock["price"], shareCount, datetime.now(), session["user_id"])

        # Add user's purchase to their portfolio, making sure each user only has one row for each stock it has (no split rows for the same stock, just adds to the share count of the stock's row)
        userStocks = db.execute("SELECT * FROM portfolio WHERE user_id = ?", session["user_id"])
        inserted = False
        for userStock in userStocks:
            if userStock["name"] == stock["name"]:
                db.execute("UPDATE portfolio SET shares = shares + ? WHERE user_id = ? AND name = ?", shareCount, session["user_id"], stock["name"])
                inserted = True
                break
        if not inserted:
            db.execute("INSERT INTO portfolio (name, symbol, shares, user_id) VALUES (?, ?, ?, ?)", stock["name"], stock["symbol"], shareCount, session["user_id"])

        # Take money away from the user
        db.execute("UPDATE users SET cash = ? WHERE id = ?", userCash - overallCost, session["user_id"])

        return redirect("/")

    else:

        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    transactions = db.execute("SELECT * FROM transactions WHERE user_id = ?", session["user_id"])

    return render_template("history.html", transactions = transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":

        # Get the stock symbl submitted
        symbol = request.form.get("symbol")

        quote = lookup(symbol)

        if not quote:
            return apology("No stock associated with symbol")

        return render_template("quoted.html", name = quote["name"], price = quote["price"], symbol = quote["symbol"])

    else:

        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":

        # Get the inputted username, password, and confirmation
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Check if username is left blank
        if not username:
            return apology("Username was blank")

        # Check if username is already in database
        users = db.execute("SELECT * FROM users")
        for user in users:
            if username == user["username"]:
                return apology("Username already in use")

        # Check if passwords are left blank
        if not password or not confirmation:
            return apology("One or more password field left blank")

        # Check if passwords match
        if password != confirmation:
            return apology("Passwords did not match")

        # Insert user into database, with hashed password
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, generate_password_hash(password))

        return redirect("/login")

    else:

        return render_template("register.html")





@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":

        # Get the stock symbol and the shares
        symbol = request.form.get("symbol")
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("No shares entered in")

        # Check if user selected a stock
        if not symbol:
            return apology("No symbol selected")

        # Check if user owns the stock
        userShareCount = db.execute("SELECT shares FROM portfolio WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)[0]["shares"]
        if userShareCount == 0 or not userShareCount:
            return apology("No stocks owned")

        # Check if shares are a positive integer
        if shares < 1:
            return apology("Enter a positive integer for shares")

        # Check if user owns as much stock as the shares
        if userShareCount < shares:
            return apology("You don't own enough shares")

        # Log the selling to the transactions database
        stock = lookup(symbol)
        db.execute("INSERT INTO transactions (type, name, symbol, price, shares, time, user_id) VALUES (?, ?, ?, ?, ?, ?, ?)", "sell", stock["name"], symbol, stock["price"], shares, datetime.now(), session["user_id"])

        # Take away the stock amount from the portfolio, if the share count for the stock is below one, take away stock from the portfolio
        db.execute("UPDATE portfolio SET shares = shares - ? WHERE user_id = ? AND symbol = ?", shares, session["user_id"], symbol)
        userShareCount = db.execute("SELECT shares FROM portfolio WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)[0]["shares"]
        if userShareCount < 1:
            db.execute("DELETE FROM portfolio WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)

        # Add the money from selling the shares to the user's balance
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", stock["price"] * shares, session["user_id"])

        return redirect("/")
    else:
        stocks = db.execute("SELECT * FROM portfolio WHERE user_id = ?", session["user_id"])

        # Make this a list of symbols that the user owns
        symbols = []
        for stock in stocks:
            symbols.append(stock["symbol"])

        return render_template("sell.html", symbols = symbols)
