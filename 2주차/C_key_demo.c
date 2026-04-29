#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 256

char caesar(char ch, int key)
{
    if (isupper((unsigned char)ch))
        return (ch - 'A' + key + 26) % 26 + 'A';

    if (islower((unsigned char)ch))
        return (ch - 'a' + key + 26) % 26 + 'a';

    return ch;
}

void convert(const char *input, char *output, int key)
{
    int i;

    for (i = 0; input[i] != '\0'; i++)
        output[i] = caesar(input[i], key);

    output[i] = '\0';
}

int main(void)
{
    char plain[MAX];
    char cipher1[MAX], cipher2[MAX];
    char decrypt_correct[MAX], decrypt_wrong[MAX];
    int key1, key2;

    printf("평문 입력: ");
    fgets(plain, sizeof(plain), stdin);
    plain[strcspn(plain, "\n")] = '\0';

    printf("키 1 입력: ");
    scanf("%d", &key1);

    printf("키 2 입력: ");
    scanf("%d", &key2);

    key1 %= 26;
    key2 %= 26;

    convert(plain, cipher1, key1);
    convert(plain, cipher2, key2);

    convert(cipher1, decrypt_correct, -key1);
    convert(cipher1, decrypt_wrong, -key2);

    printf("\n=== 키에 따른 암호화 결과 ===\n");
    printf("평문        : %s\n", plain);
    printf("키 1(%d) 암호문 : %s\n", key1, cipher1);
    printf("키 2(%d) 암호문 : %s\n", key2, cipher2);

    printf("\n=== 복호화 결과 ===\n");
    printf("올바른 키로 복호화 : %s\n", decrypt_correct);
    printf("잘못된 키로 복호화 : %s\n", decrypt_wrong);

    return 0;
}