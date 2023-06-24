#include <cs50.h>
#include <stdio.h>
#include <math.h>

void calculation (float n);

int main (void)
{
    float change = get_float("Change:\n");
    if (change <= 0)
    {
        main();
    }
    else 
    {
        calculation(change);
    }
}
void calculation (float n)
{
int coins = round(n * 100);
int cash = 0;
while (coins >= 25)
{
    cash++;
    coins = coins - 25;
}
while (coins >= 10)
{
    cash++;
    coins = coins - 10;
}
while (coins >= 5)
{
    cash++;
    coins = coins - 5;
}
while (coins >= 1)
{
    cash++;
    coins = coins -1;
}
printf("%d\n", cash);
}

