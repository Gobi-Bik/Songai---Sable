#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 256

char decrypt_char(char ch, int key)
{
    if (isupper((unsigned char)ch))
        return (ch - 'A' - key + 26) % 26 + 'A';

    if (islower((unsigned char)ch))
        return (ch - 'a' - key + 26) % 26 + 'a';

    return ch;
}

void decrypt_text(const char *cipher, char *result, int key)
{
    int i;

    for (i = 0; cipher[i] != '\0'; i++)
        result[i] = decrypt_char(cipher[i], key);

    result[i] = '\0';
}

int main(void)
{
    char cipher[MAX];
    char result[MAX];

    printf("카이사르 암호문 입력: ");
    fgets(cipher, sizeof(cipher), stdin);
    cipher[strcspn(cipher, "\n")] = '\0';

    printf("\n=== 브루트포스 결과 ===\n");

    for (int key = 0; key < 26; key++)
    {
        decrypt_text(cipher, result, key);
        printf("Key %2d: %s\n", key, result);
    }

    return 0;
}

// 입력 예시 : Khoor Zruog
// 설명 : 카이사르 암호는 가능한 키가 26개뿐이기 때문에,
// 공격자가 모든 키를 하나씩 시도하면 원문을 찾을 수 있다.
// 이것이 브루트포스 공격이다.