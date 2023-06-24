import cs50
import csv

from flask import Flask, jsonify, redirect, render_template, request

# Configure application
app = Flask(__name__)

# Reload templates when they are changed
app.config["TEMPLATES_AUTO_RELOAD"] = True


@app.after_request
def after_request(response):
    """Disable caching"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET"])
def get_index():
    return redirect("/form")


@app.route("/form", methods=["GET"])
def get_form():
    return render_template("form.html")


@app.route("/form", methods=["POST"])
def post_form():
    # Validate all fields to be filled/checked except "visit", as it's not mandatory
    if not request.form.get("fname"):
        return render_template("error.html", message="Please enter your First name")
    elif not request.form.get("sname"):
        return render_template("error.html", message="Please enter your Second name")
    elif not request.form.get("pname"):
        return render_template("error.html", message="Please enter your Pet's name")
    elif not request.form.get("age"):
        return render_template("error.html", message="Please enter your Pet's age")
    elif not request.form.get("pet"):
        return render_template("error.html", message="Please choose your pet's type")
    elif not request.form.get("purpose"):
        return render_template("error.html", message="Please choose purpose of the visit")

    # Check if form "visit" ticked
    if not request.form.get("visit"):
        visit = "First time"
    else:
        visit = "Client"
    # Open csv file
    with open('survey.csv', 'a') as file:
        writer = csv.writer(file)
        # Append data from form.html to csv file
        writer.writerow((request.form.get("fname"), request.form.get("sname"), request.form.get("pname"), request.form.get("age"), request.form.get("pet"),
            request.form.get("purpose"), visit))
    # Redirect user to the table sheet.html
    return redirect ("/sheet")

@app.route("/sheet", methods=["GET"])
def get_sheet():
    # Open csv file
    with open ('survey.csv', 'r') as file:
        reader = csv.reader(file)
        visitors = list(reader)
    return render_template("sheet.html", visitors = visitors)
