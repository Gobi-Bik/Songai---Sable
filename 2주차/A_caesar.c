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

// caesar 함수 설명
//  1. 대문자 or 소문자 확인
//  2. 'A' 입력 가정
//      1. 'A' - 'A' = 0
//      2. 0 + 3 = 3
//      3. 3 % 26 = 3 (% 26은 'Z'를 넘어가면 'A'부터 시작되게 하는 역할)
//      4. 3 + 'A' = 'D'
//  3. 'D' 반환

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
    char cipher[MAX];
    char decrypted[MAX];
    int key;

    printf("평문 입력: ");
    fgets(plain, sizeof(plain), stdin);
    plain[strcspn(plain, "\n")] = '\0';

    printf("키 입력: ");
    scanf("%d", &key);

    key %= 26;

    convert(plain, cipher, key);
    convert(cipher, decrypted, -key);

    printf("\n평문   : %s\n", plain);
    printf("암호문 : %s\n", cipher);
    printf("복호문 : %s\n", decrypted);

    return 0;
}