#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


int shift(char c);

int main (int argc, string argv[])


{
    if (argc == 2)
    {
        for (int i = 0; i < strlen(argv[1]); i++)
        {
            if (isalpha(argv[1][i]) == false)
            {
                printf("Usage: ./vigenere keyword\n");
                return 1;
            }
        }
    }
    else
    {
        printf("Usage: ./vigenere keyword\n");
        return 1;
    }
    string plaintext = get_string("plaintext: ");
    
    int index = 0;
    printf("ciphertext: ");

    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        int Key = shift(argv[1][index]);
        
        if (!isalpha(plaintext[i]))
        {
            printf ("%c", plaintext[i]);
        }
        else if (isupper(plaintext[i]))
        {
            plaintext[i] = (plaintext[i] - 65 + Key)%26 + 65;
            printf ("%c", plaintext[i]);
            index++;
        }
        else if (islower(plaintext[i]))
        {
            plaintext[i] = (plaintext[i] - 97 + Key)%26 + 97;
            printf ("%c", plaintext[i]);
            index++;
        }
        if (index == strlen(argv[1]))
        {
            index = 0;
        }
    }
    printf("\n");
}


int shift (char c)
{
    if (isupper(c))
    {
        int k = (int) c - 65;
        return k;
    }
    else
    {
        int k = (int) c - 97;
        return k;
    }
}
