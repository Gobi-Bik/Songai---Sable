#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 256

void strip_newline(char *s)
{
    size_t len = strlen(s);

    if (len > 0 && s[len - 1] == '\n')
        s[len - 1] = '\0';
}

char caesar_char(char ch, int key)
{
    if (isupper((unsigned char)ch))
        return (ch - 'A' + key + 26) % 26 + 'A';

    if (islower((unsigned char)ch))
        return (ch - 'a' + key + 26) % 26 + 'a';

    return ch;
}

void caesar_convert(const char *input, char *output, int key)
{
    int i;

    for (i = 0; input[i] != '\0'; i++)
        output[i] = caesar_char(input[i], key);

    output[i] = '\0';
}

void print_ascii_encoding(const char *text)
{
    for (int i = 0; text[i] != '\0'; i++)
        printf("%u ", (unsigned char)text[i]);

    printf("\n");
}

unsigned long simple_hash(const char *text)
{
    unsigned long hash = 5381;

    for (int i = 0; text[i] != '\0'; i++)
        hash = hash * 33 + (unsigned char)text[i];

    return hash;
}

int main(void)
{
    char text[MAX];
    char encrypted[MAX];
    char decrypted[MAX];
    int key;

    printf("문자열 입력: ");
    fgets(text, sizeof(text), stdin);
    strip_newline(text);

    printf("암호화 키 입력: ");
    scanf("%d", &key);

    key %= 26;

    caesar_convert(text, encrypted, key);
    caesar_convert(encrypted, decrypted, -key);

    printf("\n=== 1. 암호화 ===\n");
    printf("원문   : %s\n", text);
    printf("암호문 : %s\n", encrypted);
    printf("복호문 : %s\n", decrypted);
    printf("설명   : 키가 있으면 원래 문장으로 되돌릴 수 있음\n");

    printf("\n=== 2. 인코딩 ===\n");
    printf("ASCII 인코딩: ");
    print_ascii_encoding(text);
    printf("설명         : 문자를 숫자 표현으로 바꾼 것일 뿐, 숨긴 것이 아님\n");

    printf("\n=== 3. 해시 ===\n");
    printf("해시값: %lu\n", simple_hash(text));
    printf("설명  : 원문을 고정된 형태의 값으로 요약하며, 원래 문장으로 되돌릴 수 없음\n");

    printf("\n=== 개념 정리 ===\n");
    printf("암호화 : 내용을 숨기기 위한 변환, 복호화 가능\n");
    printf("인코딩 : 표현 형식을 바꾸는 변환, 보안 목적 아님\n");
    printf("해시   : 검증을 위한 일방향 변환, 복원 불가\n");

    return 0;
}