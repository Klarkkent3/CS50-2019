from nltk.tokenize import sent_tokenize

def lines(a, b):
    """Return lines in both a and b"""
    # using to split text into the list by lines
    line_a = a.split("\n")
    line_b = b.split("\n")

    # create empty set for similar words (set - because we should not have duplicates)
    similar = set()

    # run loop and compare each word from line_a with line_b
    for i in range(len(line_a)):
        if line_a[i] in line_b:
            # if duplicated word found - add this to set
            similar.add(line_a[i])

    # convert set to list
    return list(similar)


def sentences(a, b):
    """Return sentences in both a and b"""
    # using to split text into the sentences
    sentence_a = sent_tokenize(a)
    sentence_b = sent_tokenize(b)

    # create empty set for similar sentences
    similar = set()

    # run loop and compare each sentence from sentence_a with sentence_b
    for i in range(len(sentence_a)):
        if sentence_a[i] in sentence_b:
            # if duplicated sentence found - add this to set
            similar.add(sentence_a[i])

    # convert set to list
    return list(similar)

# create a function to tokenaze substruings
def token (string, n):

    # create empty set
    token = set()
    i = 0
    # create loop to find all substrings and if find - add them to set
    while i < (len(string) - n + 1):
        token.add(string[i:(i+n)])
        i+=1

    return list(token)

def substrings(a, b, n):
    """Return substrings of length n in both a and b"""

    # tokenize substrings
    substring_a = token(a, n)
    substring_b = token(b, n)

    # create empty set for similar substrings
    similar = set()
    # run loop and compare each substring from substring_a with substring_b
    for i in range(len(substring_a)):
        if substring_a[i] in substring_b:
            similar.add(substring_a[i])

    # convert set to list
    return list(similar)
