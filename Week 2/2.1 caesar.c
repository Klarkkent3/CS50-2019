#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[])

{
    if (argc == 2)
    {
        for (int i = 0; i < strlen(argv[1]); i++)
        {
            if (isdigit(argv[1][i]) == false)
            {
                printf("Usage: ./caesar key\n");
                return 1;
            }
        }
    }
    else
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    
    int Key = atoi(argv[1]);

    string plaintext = get_string("Enter your text: ");

    printf("ciphertext: ");

    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        if (islower(plaintext[i]))
        {
            plaintext[i] = (plaintext[i] - 97 + Key)%26 + 97;
            printf("%c", plaintext[i]);
        }
        else if (isupper(plaintext[i]))
        {
            plaintext[i] = (plaintext[i] - 65 + Key)%26 + 65;
            printf("%c", plaintext[i]);
        }
        else
        {
            printf("%c", plaintext[i]);
        }
    }
    printf("\n");
}
