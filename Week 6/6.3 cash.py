from cs50 import get_float

while True:
    Change = get_float ("Change owed: ")
    if Change >= 0:
        break

Coins = round(Change * 100)

Cash = Coins//25 + Coins%25//10 + Coins%25%10//5 + Coins%25%10%5

print(Cash)

