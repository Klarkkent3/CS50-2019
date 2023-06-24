from cs50 import get_string
from sys import argv


def main():
    # checking for number of arguments
    if len(argv) != 2:
        print ("Usage: python bleep.py dictionary")
        sys.exit(1)

    # declaring set
    words = set()

    # open file
    file = open (argv[1], "r")

    # removing whitespaces from set
    for i in file:
        words.add(i.strip())

    # getting text from user
    text = get_string("What message would you like to censor?\n")
    # splitting text to list
    text = text.split()

    # ittirating through words in user's text
    for i in text:

        # counter for finded 'bad' words
        count = 0
        # ittirating throug set of 'bad' words, and comparing with words from text
        for j in words:
            if i.lower() in j:
                count += 1
        # if we found 'bad' word print "*" instead of it
        if count == 1:
            print(len(i) * "*", end=" ")
        # or just print word if it's not 'bad'
        else:
            print(i, end=" ")

    print()

if __name__ == "__main__":
    main()
