#include <stdio.h>
#include <string.h>

#define MAX 256

void strip_newline(char *s)
{
    size_t len = strlen(s);

    if (len > 0 && s[len - 1] == '\n')
        s[len - 1] = '\0';
}

void xor_crypt(const unsigned char *input, unsigned char *output, size_t len, const char *key)
{
    size_t key_len = strlen(key);

    for (size_t i = 0; i < len; i++)
        output[i] = input[i] ^ key[i % key_len];
}

void print_hex(const unsigned char *data, size_t len)
{
    for (size_t i = 0; i < len; i++)
        printf("%02X ", data[i]);

    printf("\n");
}

int main(void)
{
    char plain[MAX];
    char key[MAX];

    unsigned char cipher[MAX];
    unsigned char decrypted[MAX];

    size_t plain_len;

    printf("평문 입력: ");
    fgets(plain, sizeof(plain), stdin);
    strip_newline(plain);

    printf("키 입력: ");
    fgets(key, sizeof(key), stdin);
    strip_newline(key);

    if (strlen(key) == 0)
    {
        printf("키는 비어 있으면 안 돼.\n");
        return 1;
    }

    plain_len = strlen(plain);

    xor_crypt((unsigned char *)plain, cipher, plain_len, key);
    xor_crypt(cipher, decrypted, plain_len, key);

    decrypted[plain_len] = '\0';

    printf("\n=== 대칭키 암호 예시 ===\n");
    printf("평문   : %s\n", plain);

    printf("암호문 : ");
    print_hex(cipher, plain_len);

    printf("복호문 : %s\n", decrypted);

    printf("\n설명: 같은 키로 암호화하고, 같은 키로 다시 복호화함\n");

    return 0;
}

// 이 코드는 사용자가 입력한 평문과 키를 XOR 연산해서 암호문을 만든다.
// XOR은 같은 값을 두 번 적용하면 원래 값으로 돌아오는 성질이 있기 때문에, 암호문에 같은 키를 다시 적용하면 복호문이 된다.
// 따라서 이 코드는 대칭키 암호에서 암호화와 복호화에 같은 키를 사용한다는 핵심 개념을 보여준다.