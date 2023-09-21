#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <tchar.h>
#include <psapi.h>
#include <stdio.h>
#include <time.h>

// 두 자리 숫자로 출력하기 위한 함수
void print_less_ten(int n) {
    if (n < 10) {
        printf("0");
    }
}

int main(void) {
    DWORD aProcesses[1024];
    DWORD cbNeeded;
    DWORD cProcesses;

    // 현재 시간 구하기
    time_t t = time(0);
    struct tm* now = localtime(&t);

    while (1) {
        // 프로세스 목록 열거
        if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
            return 1;
        }

        cProcesses = cbNeeded / sizeof(DWORD);

        // 현재 시간 다시 가져오기
        t = time(0);
        now = localtime(&t);

        // 현재 날짜와 시간을 지정된 형식으로 출력
        printf("%d.", (now->tm_year + 1900)); // 년
        print_less_ten(now->tm_mon + 1); // 월
        printf("%d.", (now->tm_mon + 1));
        print_less_ten(now->tm_mday); // 일
        printf("%d ", now->tm_mday);
        print_less_ten(now->tm_hour); // 시
        printf("%d:", now->tm_hour);
        print_less_ten(now->tm_min); // 분
        printf("%d:", now->tm_min);
        print_less_ten((now->tm_sec) + 1); // 초
        printf("%d : %lu\n", (now->tm_sec + 1), cProcesses); // 프로세스 수

        Sleep(1000); // 1초 대기
    }

    return 0;
}

