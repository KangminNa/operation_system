#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 500
#define DELIM " \r\t'P''ARRIVAL'':''CPU''I/O'"
#define MAX_PROCESSES 10
#define MAX_COLUMNS 100
#define CPU_BURST_INDEX_OFFSET 2

// 각 프로세스의 정보를 저장하는 배열들
int ProcessID[MAX_PROCESSES] = { 0 };         // 각 0~9번의 프로세스들의 Process ID
int arrivalT[MAX_PROCESSES] = { 0 };          // 각 0~9번의 프로세스들의 Arrival Time
int allcpuburst[MAX_PROCESSES] = { 0 };       // 각 0~9번의 프로세스들 각각의 모든 CPU Burst 시간을 합한 값
int allcpuburst2[MAX_PROCESSES] = { 0 };      // 각 0~9번의 프로세스들 각각의 모든 CPU Burst 시간을 합한 값
int ProcessFT[MAX_PROCESSES] = { 0 };         // 각 0~9번의 프로세스들의 Finish Time (CPU Burst를 마치는 시간)
int totalcpuburst = 0;                      // 모든 프로세스의 모든 cpuburst를 더한값 (time slice의 범위를 위해 선언)

// 함수 선언
void printErrorAndExit(const char* errorMessage);
void readProcessesFromFile(char* fileName, int processInfo[MAX_PROCESSES][MAX_COLUMNS], int* numProcesses, int* maxColumns);
void extractProcessInfo(int processInfo[MAX_PROCESSES][MAX_COLUMNS], int numProcesses, int maxColumns);
void calculateTotalCpuBurst(int processInfo[MAX_PROCESSES][MAX_COLUMNS], int numProcesses);
void processScheduling(int processInfo[MAX_PROCESSES][MAX_COLUMNS], int numProcesses, int maxColumns);
void printWaitingTime(int processInfo[MAX_PROCESSES][MAX_COLUMNS], int numProcesses);

int main(int argc, char** argv)
{
    // 명령행 인수가 3개 이상인 경우 에러 출력 및 종료
    if (argc >= 3)
    {
        printErrorAndExit("파일을 하나만 입력하세요");
    }

    // 파일 읽기용 변수 및 배열 초기화
    char str[MAX];
    int processInfo[MAX_PROCESSES][MAX_COLUMNS] = { 0 };
    int numProcesses = 0, maxColumns = 0;

    // 첫 번째 파일 읽기
    readProcessesFromFile(argv[1], processInfo, &numProcesses, &maxColumns);

    // 각 프로세스의 정보 추출
    extractProcessInfo(processInfo, numProcesses, maxColumns);

    // 모든 프로세스들의 모든 CPU burst를 합산
    calculateTotalCpuBurst(processInfo, numProcesses);

    // 프로세스 스케줄링 및 대기 시간 계산
    processScheduling(processInfo, numProcesses, maxColumns);

    // Waiting Time 출력
    printWaitingTime(processInfo, numProcesses);

    return 0;
}

// 에러 출력 후 종료
void printErrorAndExit(const char* errorMessage)
{
    printf("ERROR: %s\n", errorMessage);
    exit(0);
}

// 파일에서 프로세스 정보 읽기
void readProcessesFromFile(char* fileName, int processInfo[MAX_PROCESSES][MAX_COLUMNS], int* numProcesses, int* maxColumns)
{
    char str[MAX], * tok;
    FILE* fp;

    // 각 cpu burst들 중 최솟값을 구하기 위해 초기값 설정
    int i;
    int minCpuBurst[100];
    for (i = 0; i < 100; i++)
        minCpuBurst[i] = 100;

    fp = fopen(fileName, "r");
    // 파일 열기 실패 시 에러 출력 및 종료
    if (!fp)
    {
        printErrorAndExit("파일 열기 실패");
    }

    // 파일에서 한 줄씩 읽어서 배열에 저장
    int Ar = 0, Ac = 0;
    while (fgets(str, MAX, fp))
    {
        // 첫 행을 기록하고 나서는
        if (Ar > 0)
        {
            // 가장 최고의 column 길이를 찾아서 maxc에 저장
            if (Ac > *maxColumns)
                *maxColumns = Ac;
        }

        printf("%s", str);

        // 토큰 분리 및 배열에 저장
        tok = strtok(str, DELIM);
        Ac = 0;
        do
        {
            processInfo[Ar][Ac++] = atoi(tok);
        } while (tok = strtok(NULL, DELIM));

        Ar++;
    }

    fclose(fp);
    *numProcesses = Ar;
}

// 각 프로세스의 정보 추출
void extractProcessInfo(int processInfo[MAX_PROCESSES][MAX_COLUMNS], int numProcesses, int maxColumns)
{
    int i, j;

    // 각 프로세스의 정보 추출
    for (i = 0; i < numProcesses; i++)
    {
        ProcessID[i] = processInfo[i][0];         // 각 프로세스의 ID값을 넣어준다.
        arrivalT[i] = processInfo[i][1];          // 각 프로세스의 도착시간을 넣어준다.
        for (j = CPU_BURST_INDEX_OFFSET; j < maxColumns; j = j + 2)
        {
            allcpuburst[i] += processInfo[i][j]; // 각 프로세스의 모든 CPU burst값을 합한 값을 넣어준다
            allcpuburst2[i] += processInfo[i][j];
        }
    }
}

// 모든 프로세스들의 모든 CPU burst를 합산
void calculateTotalCpuBurst(int processInfo[MAX_PROCESSES][MAX_COLUMNS], int numProcesses)
{
    int i;

    // 모든 프로세스들의 모든 CPU burst를 합산
    for (i = 0; i < numProcesses; i++)
    {
        totalcpuburst += allcpuburst[i];
    }
}

// 프로세스 스케줄링 및 대기 시간 계산
void processScheduling(int processInfo[MAX_PROCESSES][MAX_COLUMNS], int numProcesses, int maxColumns)
{
    int i, j;
    int minCpuBurst[100];
    int timeslice = arrivalT[0];

    // 각 cpu burst들 중 최솟값을 구하기 위해 초기값 설정
    for (i = 0; i < 100; i++)
        minCpuBurst[i] = 100;

    // Time Slice를 두어 CPU가 할당되는 과정
    while (timeslice <= totalcpuburst)
    {
        // 최소 CPU Burst를 구하는 과정
        for (i = 0; i < numProcesses; i++)
        {
            for (j = CPU_BURST_INDEX_OFFSET; j < maxColumns; j = j + 2)
            {
                // 프로세스가 도착했는지 확인
                if (processInfo[i][1] <= timeslice)
                {
                    // 최소 CPU Burst를 구하는 과정
                    if (minCpuBurst[timeslice] > processInfo[i][j] && processInfo[i][j] != 0)
                    {
                        minCpuBurst[timeslice] = processInfo[i][j];
                        break;
                    }
                }
            }
        }

        // 선택된 프로세스 처리
        for (i = 0; i < numProcesses; i++)
        {
            for (j = CPU_BURST_INDEX_OFFSET; j < maxColumns; j = j + 2)
            {
                // 선택된 경우
                if (minCpuBurst[timeslice] == processInfo[i][j] && processInfo[i][j] != 0)
                {
                    // 모든 CPU Burst 값 감소
                    allcpuburst[i]--;
                    // 해당 열의 CPU Burst도 감소
                    processInfo[i][j]--;
                    break;
                }
            }
        }

        // 모든 CPU Burst가 다 끝난 프로세스 처리
        for (i = 0; i < numProcesses; i++)
        {
            if (allcpuburst[i] == 0 && ProcessFT[i] == 0)
            {
                // Process Finish Time값 계산 및 저장
                ProcessFT[i] = timeslice + 1;
                break;
            }
        }

        // 시간 증가
        timeslice++;
    }
}

// Waiting Time 출력
void printWaitingTime(int processInfo[MAX_PROCESSES][MAX_COLUMNS], int numProcesses)
{
    int i;
    int average = 0;

    printf("\n");
    printf("프로세스 대기 시간\n");
    printf("---------------------------------\n");
    for (i = 0; i < numProcesses; i++)
    {
        printf("P%d\t: %d \n", processInfo[i][0], (ProcessFT[i] - arrivalT[i] - allcpuburst2[i] + 1));
        average += (ProcessFT[i] - arrivalT[i] - allcpuburst2[i] + 1);
    }
    printf("---------------------------------\n");
    printf("평균 대기 시간  = %0.2f\n", (average / (double)numProcesses));
}
