```
#include <stdio.h>
#include <stdlib.h>

// 서브 테이블을 출력하는 함수
// start: 시작하는 단
// end: 끝나는 단
// num: 총 단 수
void print_subtable(int start, int end, int num) {
    // 구구단 가로 버전 출력
    for (int i = 1; i <= num; i++) {
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

int main(int argc, char* argv[]) {
    // 명령줄 인자 개수 확인
    if (argc != 2) {
        printf("Usage: %s <n>\n", argv[0]);
        return 1;
    }

    // 명령줄 인자에서 n 값을 읽어옴
    int n = atoi(argv[1]);

    // n이 0 이하인 경우 에러 메시지 출력 후 종료
    if (n <= 0) {
        printf("Please provide a positive integer as the 'n' argument.\n");
        return 1;
    }

    // n을 5로 나눈 횟수만큼 서브 테이블을 출력
    for (int i = 1; i <= n; i += 5) {
        int start = i;
        int end = i + 4;

        // 마지막 서브 테이블은 n을 초과하지 않도록 설정
        if (end > n) {
            end = n;
        }

        // 서브 테이블 출력
        print_subtable(start, end, n);

        // 서브 테이블 사이에 공백 줄 추가
        if (end != n) {
            printf("\n");
        }
    }

    return 0;
}
```
