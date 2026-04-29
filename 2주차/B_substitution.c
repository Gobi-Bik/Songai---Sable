// ABCDEFGHIJKLMNOPQRSTUVWXYZ
// QWERTYUIOPASDFGHJKLZXCVBNM

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 256

const char KEY[] = "QWERTYUIOPASDFGHJKLZXCVBNM";

char encrypt_char(char ch)
{
    if (isupper((unsigned char)ch))
        return KEY[ch - 'A'];

    if (islower((unsigned char)ch))
        return tolower(KEY[ch - 'a']);

    return ch;
}

char decrypt_char(char ch)
{
    char upper = toupper((unsigned char)ch);

    if (isalpha((unsigned char)ch))
    {
        for (int i = 0; i < 26; i++)
        {
            if (KEY[i] == upper)
            {
                if (isupper((unsigned char)ch))
                    return 'A' + i;
                else
                    return 'a' + i;
            }
        }
    }

    return ch;
}

void convert(const char *input, char *output, char (*func)(char))
{
    int i;

    for (i = 0; input[i] != '\0'; i++)
        output[i] = func(input[i]);

    output[i] = '\0';
}

int main(void)
{
    char plain[MAX];
    char cipher[MAX];
    char decrypted[MAX];

    printf("평문 입력: ");
    fgets(plain, sizeof(plain), stdin);
    plain[strcspn(plain, "\n")] = '\0';

    convert(plain, cipher, encrypt_char);
    convert(cipher, decrypted, decrypt_char);

    printf("\n치환 키: %s\n", KEY);
    printf("평문   : %s\n", plain);
    printf("암호문 : %s\n", cipher);
    printf("복호문 : %s\n", decrypted);

    return 0;
}