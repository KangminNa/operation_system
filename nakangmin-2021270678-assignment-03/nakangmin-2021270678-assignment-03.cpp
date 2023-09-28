// 필요한 헤더 파일 포함
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <time.h>

const int MAX = 100; // 배열의 최대 크기를 정의

// 숫자가 10보다 작으면 '0'을 앞에 출력하는 함수
void print_less_ten(int n) {
    if (n < 10) {
        printf("0");
    }
}

// 현재 시간을 출력하는 함수
void print_time() {
    time_t t = time(NULL); // 현재 시간 가져오기
    struct tm* now = localtime(&t); // 시간 구조체 생성

    // 년, 월, 일, 시, 분, 초를 출력
    printf("%d.", now->tm_year + 1900);
    print_less_ten(now->tm_mon + 1);
    printf("%d.", now->tm_mon + 1);
    print_less_ten(now->tm_mday);
    printf("%d ", now->tm_mday);
    print_less_ten(now->tm_hour);
    printf("%d:", now->tm_hour);
    print_less_ten(now->tm_min);
    printf("%d:", now->tm_min);
    print_less_ten(now->tm_sec + 1);
    printf("%d : ", now->tm_sec + 1);
}

// CPU 사용량의 평균을 출력하는 함수
void print_avg_usage(int index, double my_array[]) {
    double sum = 0;
    double avg = 0;

    if (index >= 4) { // 최소 5번의 데이터가 필요함
        for (int i = index - 4; i <= index; i++) {
            sum += my_array[i]; // 최근 5번의 데이터를 합산
        }
        avg = sum / 5; // 평균 계산
        printf("[5 sec avg : %.2lf]", avg);

        if (index >= 9) { // 최소 10번의 데이터가 필요함
            sum = 0;
            for (int i = index - 9; i <= index; i++) {
                sum += my_array[i]; // 최근 10번의 데이터를 합산
            }
            avg = sum / 10; // 평균 계산
            printf("[10 sec avg : %.2lf]", avg);

            if (index >= 14) { // 최소 15번의 데이터가 필요함
                sum = 0;
                for (int i = index - 14; i <= index; i++) {
                    sum += my_array[i]; // 최근 15번의 데이터를 합산
                }
                avg = sum / 15; // 평균 계산
                printf("[15 sec avg : %.2lf]\n", avg);
            }
            else {
                printf("\n"); // 15번 미만의 데이터는 줄바꿈만 출력
            }
        }
        else {
            printf("\n"); // 10번 미만의 데이터는 줄바꿈만 출력
        }
    }
    else {
        printf("\n"); // 5번 미만의 데이터는 줄바꿈만 출력
    }
}

int main() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    int numCores = sysInfo.dwNumberOfProcessors; // 시스템의 CPU 코어 수 가져오기

    printf("Number of CPU : %d\n", numCores); // CPU 코어 수 출력

    double my_array[MAX]; // CPU 사용량을 저장할 배열
    int index = 0; // 배열 인덱스 초기화

    while (1) {
        FILETIME idleTime, kernelTime, userTime;
        BOOL res = GetSystemTimes(&idleTime, &kernelTime, &userTime); // 시스템 시간 정보 가져오기

        if (!res) {
            return 0; // 시스템 시간 정보를 가져오지 못하면 프로그램 종료
        }

        DWORD idle_Low = idleTime.dwLowDateTime;
        DWORD kernel_Low = kernelTime.dwLowDateTime;
        DWORD user_Low = userTime.dwLowDateTime;

        Sleep(1000); // 1초 대기

        res = GetSystemTimes(&idleTime, &kernelTime, &userTime); // 다시 시스템 시간 정보 가져오기

        if (!res) {
            return 0; // 시스템 시간 정보를 가져오지 못하면 프로그램 종료
        }

        DWORD idle_Low_2 = idleTime.dwLowDateTime;
        DWORD kernel_Low_2 = kernelTime.dwLowDateTime;
        DWORD user_Low_2 = userTime.dwLowDateTime;

        double idle = idle_Low_2 - idle_Low;
        double kernel = kernel_Low_2 - kernel_Low;
        double user = user_Low_2 - user_Low;

        my_array[index] = ((user + kernel - idle) * 100 / (user + kernel)); // CPU 사용량 계산

        printf("%2d ", index); // 인덱스 출력
        print_time(); // 현재 시간 출력
        printf("[CPU Load : %.2lf]", my_array[index]); // CPU 사용량 출력

        print_avg_usage(index, my_array); // CPU 사용량의 평균 출력

        index++; // 배열 인덱스 증가

        if (index > MAX) {
            return 0; // 배열 인덱스가 최대 크기를 넘으면 프로그램 종료
        }
    }

    return 0;
}
