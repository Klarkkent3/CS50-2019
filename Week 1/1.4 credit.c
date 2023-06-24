#include <cs50.h>
#include <stdio.h>

void CardType (long n);

int main (void)
{
    long Card = get_long ("Enter your Credit Card Number:");
    long a = 10, b = 1;
    int sum1 = 0, sum2 = 0, lun1 = 0, lun2 = 0, i = 0;

    while ( i < 8)
    {
        lun1 = (Card / a)%10;
        a *= 100;
        lun2 = (Card / b)%10;
        b *= 100;

        sum2 += lun2;

        if ((lun1 * 2) > lun1%10)
        { 
            sum1 += ((lun1 * 2)/10)%10;
            sum1 += (lun1 * 2)%10;
        }
        else 
        {
            sum1 += lun1*2;
        }
        i++;
    }
    if ((sum1 + sum2)%10 != 0 || (Card / 10000000000000) == 0)
    {
        printf("INVALID\n");
    }
    else
    {
        CardType(Card);
    }
}
void CardType(long n)
{
    while ( n > 100)
    {
        n /= 10;
    }
    if (n == 34 || n == 37)
    {
        printf("AMEX\n");
    }
    else if (n > 50 && n < 56)
    {
        printf("MASTERCARD\n");
    }
    else if (n/10 == 4)
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }   
}
