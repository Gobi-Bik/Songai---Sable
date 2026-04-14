#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_TEXT 1024
#define MAX_CIPHER_HEX (MAX_TEXT * 2 + 1)

void strip_newline(char *s)
{
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n')
    {
        s[len - 1] = '\0';
    }
}

/* =========================
 * 1. 기밀성: XOR 암호화/복호화
 * ========================= */
void xor_encrypt(const unsigned char *plaintext, const char *key,
                 unsigned char *cipher, size_t len)
{
    size_t key_len = strlen(key);

    for (size_t i = 0; i < len; i++)
    {
        cipher[i] = plaintext[i] ^ (unsigned char)key[i % key_len];
    }
}

void bytes_to_hex(const unsigned char *bytes, size_t len, char *hex_out)
{
    const char *table = "0123456789ABCDEF";

    for (size_t i = 0; i < len; i++)
    {
        hex_out[i * 2] = table[(bytes[i] >> 4) & 0x0F];
        hex_out[i * 2 + 1] = table[bytes[i] & 0x0F];
    }
    hex_out[len * 2] = '\0';
}

int hex_value(char c)
{
    if ('0' <= c && c <= '9')
        return c - '0';
    if ('A' <= c && c <= 'F')
        return c - 'A' + 10;
    if ('a' <= c && c <= 'f')
        return c - 'a' + 10;
    return -1;
}

int hex_to_bytes(const char *hex, unsigned char *bytes_out, size_t *out_len)
{
    size_t len = strlen(hex);

    if (len % 2 != 0)
        return 0;

    for (size_t i = 0; i < len; i += 2)
    {
        int high = hex_value(hex[i]);
        int low = hex_value(hex[i + 1]);

        if (high < 0 || low < 0)
            return 0;
        bytes_out[i / 2] = (unsigned char)((high << 4) | low);
    }

    *out_len = len / 2;
    return 1;
}

/* =========================
 * 2. 무결성 + 인증: 교육용 MAC
 * =========================
 * 진짜 HMAC이 아니라 개념 시연용이야.
 * "공유 비밀키를 가진 쪽이 만들었는가"를 흉내냄.
 */
uint32_t toy_mac(const char *message, const char *key)
{
    uint32_t hash = 2166136261u;
    const unsigned char *p;

    for (p = (const unsigned char *)key; *p; p++)
    {
        hash ^= *p;
        hash *= 16777619u;
    }

    hash ^= (unsigned char)'|';
    hash *= 16777619u;

    for (p = (const unsigned char *)message; *p; p++)
    {
        hash ^= *p;
        hash *= 16777619u;
    }

    return hash;
}

int verify_toy_mac(const char *message, const char *key, uint32_t tag)
{
    return toy_mac(message, key) == tag;
}

/* =========================
 * 3. 부인방지: 교육용 RSA 전자서명
 * =========================
 * 아주 작은 값 사용 -> 실사용 절대 금지
 */
unsigned long long mod_pow(unsigned long long base,
                           unsigned long long exp,
                           unsigned long long mod)
{
    unsigned long long result = 1;
    base %= mod;

    while (exp > 0)
    {
        if (exp & 1ULL)
        {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1ULL;
    }

    return result;
}

unsigned long long hash_to_int(const char *message, unsigned long long n)
{
    return (unsigned long long)(toy_mac(message, "rsa-demo")) % n;
}

unsigned long long sign_message(const char *message,
                                unsigned long long d,
                                unsigned long long n)
{
    unsigned long long h = hash_to_int(message, n);
    return mod_pow(h, d, n);
}

int verify_signature(const char *message,
                     unsigned long long signature,
                     unsigned long long e,
                     unsigned long long n)
{
    unsigned long long h = hash_to_int(message, n);
    unsigned long long recovered = mod_pow(signature, e, n);
    return h == recovered;
}

int main(void)
{
    char plaintext[MAX_TEXT];
    char enc_key[256];
    char mac_key[256];

    unsigned char cipher_bytes[MAX_TEXT];
    unsigned char decrypted_bytes[MAX_TEXT];
    unsigned char parsed_cipher[MAX_TEXT];

    char ciphertext_hex[MAX_CIPHER_HEX];
    char tampered_hex[MAX_CIPHER_HEX];

    size_t plain_len;
    size_t cipher_len;

    /* 교육용 RSA 키 */
    const unsigned long long e = 65537ULL;
    const unsigned long long d = 4922825ULL;
    const unsigned long long n = 9173503ULL;

    uint32_t mac_tag;
    unsigned long long signature;

    printf("=== 암호학 개념 시연 프로그램(C 버전) ===\n");
    printf("1) 평문 / 암호화 / 키\n");
    printf("2) 기밀성\n");
    printf("3) 무결성 / 인증\n");
    printf("4) 부인방지\n\n");

    printf("평문 입력: ");
    if (fgets(plaintext, sizeof(plaintext), stdin) == NULL)
        return 1;
    strip_newline(plaintext);

    printf("암호화 키 입력: ");
    if (fgets(enc_key, sizeof(enc_key), stdin) == NULL)
        return 1;
    strip_newline(enc_key);

    printf("무결성/인증 키 입력: ");
    if (fgets(mac_key, sizeof(mac_key), stdin) == NULL)
        return 1;
    strip_newline(mac_key);

    if (strlen(plaintext) == 0 || strlen(enc_key) == 0 || strlen(mac_key) == 0)
    {
        printf("입력값은 비어 있으면 안 돼.\n");
        return 1;
    }

    /* 1. 암호화 */
    plain_len = strlen(plaintext);
    xor_encrypt((unsigned char *)plaintext, enc_key, cipher_bytes, plain_len);
    bytes_to_hex(cipher_bytes, plain_len, ciphertext_hex);

    /* 2. 복호화 */
    if (!hex_to_bytes(ciphertext_hex, parsed_cipher, &cipher_len))
    {
        printf("암호문 파싱 실패\n");
        return 1;
    }

    xor_encrypt(parsed_cipher, enc_key, decrypted_bytes, cipher_len);
    decrypted_bytes[cipher_len] = '\0';

    /* 3. 무결성/인증용 MAC */
    mac_tag = toy_mac(ciphertext_hex, mac_key);

    /* 4. 부인방지용 전자서명 */
    signature = sign_message(ciphertext_hex, d, n);

    printf("\n=== 원본 데이터 ===\n");
    printf("평문: %s\n", plaintext);

    printf("\n=== 기밀성: 암호화 결과 ===\n");
    printf("암호문(HEX): %s\n", ciphertext_hex);

    printf("\n=== 복호화 결과 ===\n");
    printf("복호문: %s\n", decrypted_bytes);

    printf("\n=== 무결성 + 인증(교육용 MAC) ===\n");
    printf("MAC 태그: %u\n", mac_tag);
    printf("검증 결과: %s\n",
           verify_toy_mac(ciphertext_hex, mac_key, mac_tag) ? "성공" : "실패");

    printf("\n=== 부인방지(교육용 RSA 전자서명) ===\n");
    printf("공개키(e, n): (%llu, %llu)\n", e, n);
    printf("서명값: %llu\n", signature);
    printf("서명 검증 결과: %s\n",
           verify_signature(ciphertext_hex, signature, e, n) ? "성공" : "실패");

    /* 변조 시연 */
    strcpy(tampered_hex, ciphertext_hex);
    if (strlen(tampered_hex) > 0)
    {
        tampered_hex[0] = (tampered_hex[0] == 'A') ? 'B' : 'A';
    }

    printf("\n=== 변조 시연 ===\n");
    printf("원래 암호문: %s\n", ciphertext_hex);
    printf("변조 암호문: %s\n", tampered_hex);

    printf("\n[변조 후 검증 결과]\n");
    printf("MAC 검증: %s\n",
           verify_toy_mac(tampered_hex, mac_key, mac_tag) ? "성공" : "실패");
    printf("서명 검증: %s\n",
           verify_signature(tampered_hex, signature, e, n) ? "성공" : "실패");

    printf("\n=== 개념 연결 ===\n");
    printf("평문       : 암호화 전 원래 데이터\n");
    printf("암호화     : 평문을 암호문으로 바꾸는 과정\n");
    printf("키         : 암호화/검증을 제어하는 값\n");
    printf("기밀성     : 암호문만 보고는 원문을 바로 알기 어려움\n");
    printf("무결성     : 변조되면 검증이 실패함\n");
    printf("인증       : 올바른 키를 가진 쪽이 만들었는지 확인함\n");
    printf("부인방지   : 개인키 서명으로 나중에 부인하기 어렵게 함\n");

    return 0;
}