// nakangmin-2021270678-.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//



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

    printf("Enter the value of n: ");
    scanf_s("%d", &n);

    if (n <= 0) {
        printf("Please provide a positive integer as the 'n' argument.\n");
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













// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
