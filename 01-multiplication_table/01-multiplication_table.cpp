#include <stdio.h>

// 구구단을 출력하는 함수
void print_subtable(int start, int end) {
    // 구구단 가로 버전 출력
    for (int i = 1; i <= 9; i++) {
        for (int j = start; j <= end; j++) {
            // 출력되지 않는 빈 공간 유지
            if (j < i) {
                printf("                ");
            }
            else {
                printf("%3d x %d = %2d\t", j, i, (i * j));
            }
        }
        printf("\n");
    }
}

int main() {
    int n; // 사용자로부터 입력 받을 n을 선언합니다.

    printf("숫자를 입력하세요: ");
    scanf_s("%d", &n);

    if (n <= 0) {
        printf("1부터 숫자를 입력해 주세요.\n");
        return 1;
    }

    // n을 5로 나눈 횟수만큼 서브 테이블을 출력합니다.
    for (int i = 1; i <= n; i += 5) {
        int start = i;
        int end = i + 4;

        // 마지막 서브 테이블은 n을 초과하지 않도록 설정합니다.
        if (end > n) {
            end = n;
        }

        // 서브 테이블 출력
        print_subtable(start, end);

        // 서브 테이블 사이에 공백 줄 추가
        if (end != n) {
            printf("\n");
        }
    }

    return 0;
}