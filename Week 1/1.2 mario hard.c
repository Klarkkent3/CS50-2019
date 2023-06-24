#include <stdio.h>
#include <cs50.h>

void program(int n);

int main (void)
{
    int Height = get_int("Height:\n");
    if (Height < 1 || Height > 8)
    {
        main();
    }
    else
    {
         program(Height);
    }
}
void program(int n)
{
    for (int i=1; i<=n; i++)
    {
        for (int a=0; a<(n-i); a++)
        {
            printf(" ");
        }
        for (int b=0; b<i; b++)
        {
            printf("#");
        }
        printf("  ");
        for (int c=i; c>0; c--)
        {
            printf("#");
        }
        printf("\n");
    }  
}
