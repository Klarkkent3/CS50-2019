import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to u_e SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Take cash from users table
    t_cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
    t_cash = t_cash[0]["cash"]

    # Check each row from Profile which belongs to user
    rows = db.execute("SELECT * FROM Profile WHERE id = :id", id=session["user_id"])

    # Variable for total balance
    t_balance = t_cash

    # Itirate throud each row of user's Profile
    for row in rows:

        # By symbol in profile we found company name and current price for shares
        share = lookup(row["symbol"])
        row["name"] = share["name"]
        row["price"] = usd(share["price"])
        row["sum"] = share["price"] * row["share"]

        t_balance += row["sum"]


    return render_template("index.html", rows=rows, t_cash=usd(t_cash), t_balance=usd(t_balance))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure share's symbol and amount were submitted
        if not request.form.get("symbol"):
            return apology("must provide share's symbol", 400)
        if not request.form.get("shares"):
            return apology("must provide share's amount", 400)

        # Ensure that user enter not partial shares
        user_shares = request.form.get("shares")
        if not user_shares.isdigit():
            return apology("you cannot provide partial shares", 400)

        # Check symbol and quote share with lookup function
        shares = lookup(request.form.get("symbol"))
        if shares is None:
            return apology("check share's symbol", 400)

        # Check quote for shares that user want to buy
        quote = shares["price"] * int(request.form.get("shares"))

        # Check user's balance
        balance = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])
        balance = balance[0]["cash"]

        # Find user's balance after transaction and check if transaction can be performed
        post_balance = balance - quote
        if post_balance < 0:
            return apology("you have insufficient funds to perform transaction", 400)

        # Update user's cash balance after transaction
        db.execute("UPDATE users SET cash = :post_balance WHERE id = :id",
                    post_balance=post_balance, id=session["user_id"])

        # Check for these shares in user's profile
        row = db.execute("SELECT * FROM Profile WHERE id = :id AND symbol = :symbol",
                            id=session["user_id"], symbol=shares["symbol"])

        # If user don't have these shares - create a new row in profile
        if len(row) != 1:
            db.execute("INSERT INTO Profile (id, symbol, share) VALUES (:id, :symbol, :share)",
                        id=session["user_id"], symbol=shares["symbol"], share=request.form.get("shares"))

        # If user already have these shares - update quantity of shares
        else:
            # Check shares before transaction
            bef_share = db.execute("SELECT share FROM Profile WHERE id = :id AND symbol = :symbol",
                                    id=session["user_id"], symbol=shares["symbol"])
            bef_share = bef_share[0]["share"]

            # Calculate new amount of shares
            new_share = bef_share + int(request.form.get("shares"))

            # Update Profile
            db.execute("UPDATE Profile SET share = :new_share WHERE id = :id AND symbol = :symbol",
                        new_share=new_share, id=session["user_id"], symbol=shares["symbol"])

        # Update history of transactions
        db.execute("INSERT INTO Trans_history (id, symbol, share, price, operation) VALUES (:id, :symbol, :share, :price, 'buy')",
                    id=session["user_id"], symbol=shares["symbol"], share=request.form.get("shares"), price=quote)

        # Redirect user to home page after successful operation
        return redirect("/")

    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Check each row from History which belongs to user
    rows = db.execute("SELECT * FROM Trans_history WHERE id = :id", id=session["user_id"])

    # By symbol in History we found company name and
    for row in rows:
        share = lookup(row["symbol"])
        row["name"] = share["name"]
        row["price"]=usd(row["price"])

    return render_template("history.html", rows=rows)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

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

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure share's symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide share's symbol", 400)

        # Check symbol with lookup function
        shares = lookup(request.form.get("symbol"))

        if shares is None:
            return apology("check share's symbol", 400)

        # Redirect user to quoted form
        return render_template("quoted.html", shares = shares)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Create variables
        username = request.form.get("username")
        hash = generate_password_hash(request.form.get("password"))

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username is not existed in database
        if len(rows) != 0:
            return apology("Username already existed, choose another one", 400)

        # Ensure password and password confirmation is matching
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("Password is not matching, please recheck", 400)

        # Add registered user to database
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
                   username=username, hash = hash)

        # Redirect user to login form
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # Check which shares we own and copy them in order to display it in options in sell.html
    symbols = db.execute(" SELECT symbol FROM Profile WHERE id = :id", id=session["user_id"])

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure share's symbol and amount were submitted
        if not request.form.get("symbol"):
            return apology("must provide share's symbol", 400)
        if not request.form.get("shares"):
            return apology("must provide share's amount", 400)

        # Check symbol and quote share with lookup function
        shares = lookup(request.form.get("symbol"))

        # Check amount of shares that user have and if it's sufficient for operation
        row = db.execute("SELECT * FROM Profile WHERE id = :id AND symbol = :symbol",
                                    id=session["user_id"], symbol=shares["symbol"])
        user_shares = row[0]["share"]
        if user_shares < int(request.form.get("shares")):
            return apology("you have insufficient shares", 400)

        # Check quote for shares that user want to buy
        quote = shares["price"] * int(request.form.get("shares"))

        # Check user's balance
        balance = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])
        balance = balance[0]["cash"]

        # Find user's balance after transaction and update user's cash
        post_balance = balance + quote
        db.execute("UPDATE users SET cash = :post_balance WHERE id = :id",
                    post_balance=post_balance, id=session["user_id"])

        # Find amount of shares that user will have after transactions
        new_shares = user_shares - int(request.form.get("shares"))

        # If shares will remain more than 0 - update table
        if new_shares > 0:
            db.execute("UPDATE Profile SET share = :new_shares WHERE id = :id AND symbol = :symbol",
                        new_shares=new_shares, id=session["user_id"], symbol=shares["symbol"])

        # If shares will remain 0 - delete row in table
        else:
            db.execute("DELETE FROM Profile WHERE id = :id AND symbol = :symbol",
                        id=session["user_id"], symbol=shares["symbol"])

        # Update history of transactions
        db.execute("INSERT INTO Trans_history (id, symbol, share, price, operation) VALUES (:id, :symbol, :share, :price, 'sell')",
                    id=session["user_id"], symbol=shares["symbol"], share=request.form.get("shares"), price=quote)

        # Redirect user to home page after successful operation
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("sell.html", symbols=symbols)

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
