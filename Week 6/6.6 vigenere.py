import sys
from cs50 import get_string




def main():
    if len(sys.argv) == 2:
        for i in sys.argv[1]:
            if i.isalpha() == False:
                print("Usage error")
                sys.exit(1)
    else:
        print("Usage error1")
        sys.exit(1)


    plaintext = get_string("plaintext: ")
    print ("ciphertext: ", end="")

    index = 0

    for i in plaintext:
        Key = shift(sys.argv[1][index])

        if i.isalpha():
            if i.islower():
                temp = chr((ord(i) - 97 + Key)%26 + 97)
                print(temp, end="")
                index += 1
            if i.isupper():
                temp = chr((ord(i) - 65 + Key)%26 + 65)
                print(temp, end="")
                index += 1
            if index == len(sys.argv[1]):
                index = 0
        else:
            print(i, end="")

    print()

def shift(n):
    if n.islower():
        k = ord(n) - 97
        return k
    else:
        k = ord(n) - 65
        return k



if __name__ == "__main__":
    main()