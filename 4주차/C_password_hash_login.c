#include <stdio.h>
#include <string.h>

#define MAX 256
#define SALT "study_salt"

void strip_newline(char *s)
{
    size_t len = strlen(s);

    if (len > 0 && s[len - 1] == '\n')
        s[len - 1] = '\0';
}

unsigned long simple_hash(const char *password, const char *salt)
{
    unsigned long hash = 5381;

    for (int i = 0; salt[i] != '\0'; i++)
        hash = hash * 33 + (unsigned char)salt[i];

    hash = hash * 33 + '|';

    for (int i = 0; password[i] != '\0'; i++)
        hash = hash * 33 + (unsigned char)password[i];

    return hash;
}

int main(void)
{
    char signup_password[MAX];
    char login_password[MAX];

    unsigned long stored_hash;
    unsigned long login_hash;

    printf("=== 회원가입 단계 ===\n");
    printf("새 비밀번호 입력: ");
    fgets(signup_password, sizeof(signup_password), stdin);
    strip_newline(signup_password);

    stored_hash = simple_hash(signup_password, SALT);

    printf("\n[DB에 저장되는 값]\n");
    printf("salt    : %s\n", SALT);
    printf("hash    : %lu\n", stored_hash);
    printf("원문 비밀번호는 저장하지 않음\n");

    printf("\n=== 로그인 단계 ===\n");
    printf("비밀번호 입력: ");
    fgets(login_password, sizeof(login_password), stdin);
    strip_newline(login_password);

    login_hash = simple_hash(login_password, SALT);

    printf("\n[검증 과정]\n");
    printf("저장된 해시값 : %lu\n", stored_hash);
    printf("입력값 해시값 : %lu\n", login_hash);

    if (stored_hash == login_hash)
        printf("로그인 성공\n");
    else
        printf("로그인 실패\n");

    printf("\n=== 개념 정리 ===\n");
    printf("비밀번호는 복호화하지 않음\n");
    printf("입력한 비밀번호를 다시 해시해서 저장된 해시값과 비교함\n");
    printf("서버는 원문 비밀번호를 알 필요가 없음\n");

    return 0;
}