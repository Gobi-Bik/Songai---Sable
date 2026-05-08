#include <stdio.h>
#include <ctype.h>

#define MAX 256

int main(void)
{
    char text[MAX];
    int count[26] = {0};

    printf("암호문 입력: ");
    fgets(text, sizeof(text), stdin);

    for (int i = 0; text[i] != '\0'; i++)
    {
        if (isalpha((unsigned char)text[i]))
        {
            int ch = toupper((unsigned char)text[i]);
            count[ch - 'A']++;
        }
    }

    printf("\n=== 문자 빈도 ===\n");

    for (int i = 0; i < 26; i++)
    {
        if (count[i] > 0)
            printf("%c : %d\n", 'A' + i, count[i]);
    }

    return 0;
}

// 입력 예시 : WKLV LV D VHFUHW PHVVDJH
// 설명 : 이 코드는 암호문에서 각 알파벳이 몇 번 등장했는지 센다.
// 단순 치환 암호는 글자가 다른 글자로 바뀌어도 등장 빈도 자체는
// 남기 때문에, 많이 나온 글자를 보고 원래 평문의 자주 쓰이는 글자를
// 추측할 수 있다.