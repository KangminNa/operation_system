#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <time.h>

const int MAX = 100;


void print_less_ten(int n) {
    if (n < 10) {
        printf("0");
    }
}

void print_time() {
    time_t t = time(NULL);
    struct tm* now = localtime(&t);

    printf("%d_", now->tm_year + 1900);
    print_less_ten(now->tm_mon + 1);
    printf("%d_", now->tm_mon + 1);
    print_less_ten(now->tm_mday);
    printf("%d : ", now->tm_mday);
    print_less_ten(now->tm_hour);
    printf("%d_", now->tm_hour);
    print_less_ten(now->tm_min);
    printf("%d_", now->tm_min);
    print_less_ten(now->tm_sec + 1);
    printf("%d : ", now->tm_sec + 1);
}

void print_avg_usage(int index, double my_array[]) {
    double sum = 0;
    double avg = 0;

    if (index >= 4) {
        for (int i = index - 4; i <= index; i++) {
            sum += my_array[i];
        }
        avg = sum / 5;
        printf("[5 sec avg : %.2lf]", avg);

        if (index >= 9) {
            sum = 0;
            for (int i = index - 9; i <= index; i++) {
                sum += my_array[i];
            }
            avg = sum / 10;
            printf("[10 sec avg : %.2lf]", avg);

            if (index >= 14) {
                sum = 0;
                for (int i = index - 14; i <= index; i++) {
                    sum += my_array[i];
                }
                avg = sum / 15;
                printf("[15 sec avg : %.2lf]\n", avg);
            }
            else {
                printf("\n");
            }
        }
        else {
            printf("\n");
        }
    }
    else {
        printf("\n");
    }
}

int main() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    int numCores = sysInfo.dwNumberOfProcessors; // CPU 코어 수

    printf("CPU Cores: %d\n", numCores);

    double my_array[MAX];
    int index = 0;

 

    while (1) {
        FILETIME idleTime, kernelTime, userTime;
        BOOL res = GetSystemTimes(&idleTime, &kernelTime, &userTime);

        if (!res) {
            return 0;
        }

        DWORD idle_Low = idleTime.dwLowDateTime;
        DWORD kernel_Low = kernelTime.dwLowDateTime;
        DWORD user_Low = userTime.dwLowDateTime;

        Sleep(1000);

        res = GetSystemTimes(&idleTime, &kernelTime, &userTime);

        if (!res) {
            return 0;
        }

        DWORD idle_Low_2 = idleTime.dwLowDateTime;
        DWORD kernel_Low_2 = kernelTime.dwLowDateTime;
        DWORD user_Low_2 = userTime.dwLowDateTime;

        double idle = idle_Low_2 - idle_Low;
        double kernel = kernel_Low_2 - kernel_Low;
        double user = user_Low_2 - user_Low;

        my_array[index] = ((user + kernel - idle) * 100 / (user + kernel));

        printf("%2d ", index);
        print_time();
        printf("[CPU Load : %.2lf]", my_array[index]);

        print_avg_usage(index, my_array);

        index++;

        if (index > MAX) {
            return 0;
        }
    }

    return 0;
}
