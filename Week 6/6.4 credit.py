from cs50 import get_int



def main ():

    while True:
        Card = get_int ("Number: ")
        if Card > 0:
            break

    if Moon(Card):
        CardType(Card)
    else:
        print("INVALID")



def Moon (number):
    sum = 0

    for i in range (len(str(number))):
        if (i%2 == 0):
            sum += number%10
        else:
            digit = (number %10) * 2
            sum += digit//10 + digit%10
        number //= 10

    if sum%10 == 0:
        return True
    else:
        return False



def CardType(n):

    digits = int(str(n)[0:2])

    if (digits == 34 or digits == 37) and len(str(n)) == 15:
        print("AMEX")
    elif (digits > 50 and digits < 56) and len(str(n)) == 16:
        print("MASTERCARD")
    elif (digits > 39 and digits < 50) and (len(str(n)) == 13 or len(str(n)) == 16):
        print("VISA")
    else:
        print("INVALID")

if __name__ == "__main__":
    main()
