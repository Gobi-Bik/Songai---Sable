#include <stdio.h>
#include <string.h>

#define MAX_TEXT 256
#define MAX_KEY 64

void strip_newline(char *s)
{
    size_t len = strlen(s);

    if (len > 0 && s[len - 1] == '\n')
        s[len - 1] = '\0';
}

/* 거듭제곱을 빠르게 계산하고 mod로 나누는 함수 */
unsigned long long mod_pow(unsigned long long base,
                           unsigned long long exp,
                           unsigned long long mod)
{
    unsigned long long result = 1;
    base %= mod;

    while (exp > 0)
    {
        if (exp % 2 == 1)
            result = (result * base) % mod;

        base = (base * base) % mod;
        exp /= 2;
    }

    return result;
}

/* 대칭키 암호 역할: 같은 키로 암호화/복호화 */
void xor_crypt(const unsigned char *input,
               unsigned char *output,
               size_t len,
               const char *key)
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
    char message[MAX_TEXT];
    char session_key[MAX_KEY];
    char recovered_key[MAX_KEY];

    unsigned char cipher[MAX_TEXT];
    unsigned char decrypted[MAX_TEXT];

    unsigned long long wrapped_key[MAX_KEY];

    size_t message_len;
    size_t key_len;

    /* 학습용 RSA 작은 값: 실제 보안용 절대 아님 */
    const unsigned long long public_e = 17;
    const unsigned long long private_d = 2753;
    const unsigned long long n = 3233;

    printf("메시지 입력: ");
    fgets(message, sizeof(message), stdin);
    strip_newline(message);

    printf("대칭키 입력: ");
    fgets(session_key, sizeof(session_key), stdin);
    strip_newline(session_key);

    if (strlen(message) == 0 || strlen(session_key) == 0)
    {
        printf("메시지와 대칭키는 비어 있으면 안 돼.\n");
        return 1;
    }

    message_len = strlen(message);
    key_len = strlen(session_key);

    printf("\n=== 1. 공개키로 대칭키 보호 ===\n");
    printf("공개키(e, n): (%llu, %llu)\n", public_e, n);
    printf("개인키(d, n): (%llu, %llu)\n", private_d, n);

    printf("\n전달할 대칭키: %s\n", session_key);
    printf("공개키로 감싼 대칭키: ");

    for (size_t i = 0; i < key_len; i++)
    {
        unsigned char ch = (unsigned char)session_key[i];
        wrapped_key[i] = mod_pow(ch, public_e, n);
        printf("%llu ", wrapped_key[i]);
    }

    printf("\n");

    printf("\n=== 2. 개인키로 대칭키 복구 ===\n");

    for (size_t i = 0; i < key_len; i++)
    {
        recovered_key[i] = (char)mod_pow(wrapped_key[i], private_d, n);
    }

    recovered_key[key_len] = '\0';

    printf("복구된 대칭키: %s\n", recovered_key);

    printf("\n=== 3. 복구한 대칭키로 메시지 암호화 ===\n");

    xor_crypt((unsigned char *)message, cipher, message_len, recovered_key);

    printf("원문 메시지: %s\n", message);
    printf("암호문(HEX): ");
    print_hex(cipher, message_len);

    printf("\n=== 4. 같은 대칭키로 메시지 복호화 ===\n");

    xor_crypt(cipher, decrypted, message_len, recovered_key);
    decrypted[message_len] = '\0';

    printf("복호문: %s\n", decrypted);

    printf("\n=== 개념 정리 ===\n");
    printf("공개키 암호: 대칭키를 안전하게 전달하는 역할\n");
    printf("대칭키 암호: 실제 메시지를 빠르게 암호화하는 역할\n");
    printf("키 분배 문제: 대칭키를 상대에게 안전하게 전달해야 하는 문제\n");

    printf("\n주의: 이 코드는 개념 설명용이야.\n");
    printf("실제 보안에서는 직접 구현하지 말고 검증된 암호 라이브러리를 사용해야 해.\n");

    return 0;
}

// 1. 사용자가 메시지와 대칭키를 입력한다.
// 2. 공개키 암호 역할로 대칭키를 숫자 형태로 감싼다.
// 3. 받는 쪽은 개인키로 대칭키를 복구한다.
// 4. 복구한 대칭키로 실제 메시지를 XOR 암호화/복호화한다.