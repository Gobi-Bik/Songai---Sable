#include <stdio.h>

int main(void)
{
    int charset_size;
    int key_length;
    unsigned long long key_space = 1;

    printf("문자 종류 수 입력: ");
    scanf("%d", &charset_size);

    printf("키 길이 입력: ");
    scanf("%d", &key_length);

    for (int i = 0; i < key_length; i++)
    {
        key_space *= charset_size;
    }

    printf("\n키 공간 크기: %llu\n", key_space);

    return 0;
}

// 입력 예시 : 10, 4
// 설명 : 숫자는 0부터 9까지 총 10종류가 있다.
// 4자리 키라면 각 자리마다 10가지 선택지가 있으므로
// 10 × 10 × 10 × 10 = 10000개의 키 후보가 생긴다.
// 이 후보의 전체 개수가 키 공간이다.