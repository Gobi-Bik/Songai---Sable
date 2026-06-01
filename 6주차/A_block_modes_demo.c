#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define MAX 256
#define BLOCK_SIZE 4

void strip_newline(char *s)
{
    size_t len = strlen(s);

    if (len > 0 && s[len - 1] == '\n')
        s[len - 1] = '\0';
}

/* 학습용 블록 암호: 실제 보안용 아님 */
void toy_encrypt_block(const uint8_t input[], uint8_t output[], const uint8_t key[])
{
    for (int i = 0; i < BLOCK_SIZE; i++)
        output[i] = input[i] ^ key[i];
}

/* PKCS#7 방식처럼 부족한 부분을 패딩값으로 채움 */
size_t add_padding(const char *plain, uint8_t padded[])
{
    size_t len = strlen(plain);
    size_t pad = BLOCK_SIZE - (len % BLOCK_SIZE);

    if (pad == 0)
        pad = BLOCK_SIZE;

    memcpy(padded, plain, len);

    for (size_t i = 0; i < pad; i++)
        padded[len + i] = (uint8_t)pad;

    return len + pad;
}

void print_hex(const char *title, const uint8_t data[], size_t len)
{
    printf("%s", title);

    for (size_t i = 0; i < len; i++)
    {
        printf("%02X ", data[i]);

        if ((i + 1) % BLOCK_SIZE == 0)
            printf("| ");
    }

    printf("\n");
}

/* ECB: 각 블록을 독립적으로 암호화 */
void encrypt_ecb(const uint8_t plain[], uint8_t cipher[],
                 size_t len, const uint8_t key[])
{
    for (size_t i = 0; i < len; i += BLOCK_SIZE)
        toy_encrypt_block(&plain[i], &cipher[i], key);
}

/* CBC: 이전 암호문 블록을 다음 평문 블록에 섞음 */
void encrypt_cbc(const uint8_t plain[], uint8_t cipher[],
                 size_t len, const uint8_t key[], const uint8_t iv[])
{
    uint8_t mixed[BLOCK_SIZE];
    uint8_t prev[BLOCK_SIZE];

    memcpy(prev, iv, BLOCK_SIZE);

    for (size_t i = 0; i < len; i += BLOCK_SIZE)
    {
        for (int j = 0; j < BLOCK_SIZE; j++)
            mixed[j] = plain[i + j] ^ prev[j];

        toy_encrypt_block(mixed, &cipher[i], key);

        memcpy(prev, &cipher[i], BLOCK_SIZE);
    }
}

/* CTR: 카운터 값을 암호화해서 만든 값과 평문을 XOR */
void encrypt_ctr(const uint8_t plain[], uint8_t cipher[],
                 size_t len, const uint8_t key[], const uint8_t iv[])
{
    uint8_t counter_block[BLOCK_SIZE];
    uint8_t key_stream[BLOCK_SIZE];

    memcpy(counter_block, iv, BLOCK_SIZE);

    for (size_t i = 0; i < len; i += BLOCK_SIZE)
    {
        toy_encrypt_block(counter_block, key_stream, key);

        for (int j = 0; j < BLOCK_SIZE; j++)
            cipher[i + j] = plain[i + j] ^ key_stream[j];

        counter_block[BLOCK_SIZE - 1]++;
    }
}

int main(void)
{
    char plain[MAX];
    char key_input[MAX];
    char iv_input[MAX];

    uint8_t key[BLOCK_SIZE];
    uint8_t iv[BLOCK_SIZE];

    uint8_t padded[MAX];
    uint8_t ecb[MAX];
    uint8_t cbc[MAX];
    uint8_t ctr[MAX];

    uint8_t different_iv[BLOCK_SIZE] = {'I', 'V', '0', '2'};
    uint8_t cbc_with_different_iv[MAX];

    size_t padded_len;

    printf("평문 입력: ");
    fgets(plain, sizeof(plain), stdin);
    strip_newline(plain);

    printf("키 입력(4글자 이상): ");
    fgets(key_input, sizeof(key_input), stdin);
    strip_newline(key_input);

    printf("IV 입력(4글자 이상): ");
    fgets(iv_input, sizeof(iv_input), stdin);
    strip_newline(iv_input);

    if (strlen(key_input) < BLOCK_SIZE || strlen(iv_input) < BLOCK_SIZE)
    {
        printf("키와 IV는 최소 4글자 이상 입력해야 해.\n");
        return 1;
    }

    memcpy(key, key_input, BLOCK_SIZE);
    memcpy(iv, iv_input, BLOCK_SIZE);

    padded_len = add_padding(plain, padded);

    encrypt_ecb(padded, ecb, padded_len, key);
    encrypt_cbc(padded, cbc, padded_len, key, iv);
    encrypt_ctr(padded, ctr, padded_len, key, iv);
    encrypt_cbc(padded, cbc_with_different_iv, padded_len, key, different_iv);

    printf("\n=== 블록 단위 암호화 예시 ===\n");
    printf("블록 크기: %d바이트\n", BLOCK_SIZE);
    print_hex("패딩된 평문 : ", padded, padded_len);

    printf("\n=== ECB 모드 ===\n");
    print_hex("ECB 암호문  : ", ecb, padded_len);
    printf("설명: 각 블록을 따로 암호화하므로 같은 평문 블록은 같은 암호문 블록이 될 수 있음\n");

    printf("\n=== CBC 모드 ===\n");
    print_hex("CBC 암호문  : ", cbc, padded_len);
    printf("설명: 이전 암호문 블록을 다음 블록에 섞어서 암호화함\n");

    printf("\n=== CTR 모드 ===\n");
    print_hex("CTR 암호문  : ", ctr, padded_len);
    printf("설명: IV와 카운터를 이용해 만든 값과 평문을 XOR함\n");

    printf("\n=== IV 변경 비교(CBC) ===\n");
    print_hex("기존 IV 결과: ", cbc, padded_len);
    print_hex("다른 IV 결과: ", cbc_with_different_iv, padded_len);
    printf("설명: 같은 평문과 같은 키라도 IV가 달라지면 암호문이 달라질 수 있음\n");

    printf("\n주의: 이 코드는 블록 암호 모드의 구조를 보여주는 학습용 예시야.\n");
    printf("실제 보안에서는 AES 같은 검증된 알고리즘과 안전한 라이브러리를 사용해야 해.\n");

    return 0;
}

// 이 코드는 평문을 4바이트 블록으로 나눈 뒤, 같은 toy_encrypt_block 함수를 ECB, CBC, CTR 방식으로 다르게 사용한다.
// 즉, 암호 알고리즘이 같아도 운영 모드에 따라 암호문이 달라진다는 점을 보여준다.
// 또한 CBC에서 IV를 바꾸면 같은 평문과 같은 키를 사용해도 결과가 달라지므로, IV가 반복 패턴을 줄이는 데 필요하다는 점을 확인할 수 있다.