import sys
from cs50 import get_string

if len(sys.argv) != 2:
    print("Usage error")
    sys.exit(1)

key = int(sys.argv[1])

plaintext = get_string("plaintext: ")

print("ciphertext: ", end="")

for i in plaintext:
    if i.isalpha():
        if i.isupper():
            temp = chr((ord(i) - 65 + key)%26 + 65)
            print(temp, end="")
        if i.islower():
            temp = chr((ord(i) - 97 + key)%26 + 97)
            print(temp, end="")
    else:
        print(i, end="")

print()