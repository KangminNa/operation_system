#include <stdio.h> // 파일 입출력을 위한 라이브러리
#include <Windows.h> // Windows API 사용을 위한 라이브러리
#include <stdlib.h>
#include <string.h>
#include <process.h> // 다중 스레드를 생성하기 위한 라이브러리
#include <time.h>
#include <xutility>
#pragma warning(disable: 4996) // 컴파일러 경고 #4996을 무시하기 위한 지시문

#define MAX_ROW 100 // Matrix 최대 Row 100라인까지
#define MAX_COL 100 // Matrix 최대 Col 100라인까지
#define MAX_LINE 1000
#define MAX_THREAD 8 // 최대 스레드 수

int A[MAX_ROW][MAX_COL];    // 첫 번째 행렬
int Ar, Ac;                  // 첫 번째 행렬의 행(row) 및 열(column)
int B[MAX_ROW][MAX_COL];    // 두 번째 행렬
int Br, Bc;                  // 두 번째 행렬의 행(row) 및 열(column)
int AB[MAX_ROW][MAX_COL];   // 결과 행렬
int ABr, ABc;                // 결과 행렬의 행(row) 및 열(column)
int C[MAX_ROW][MAX_COL];
int Cr[50] = { 0 };
int Cc[50] = { 0 };
int D[MAX_ROW][MAX_COL];  // 중간 결과 배열
int E[MAX_ROW][MAX_COL];  // 중간 결과 배열
int F[MAX_ROW][MAX_COL];  // 최종 결과 배열
clock_t start, end;          // 프로그램 시작 및 종료 시간 측정용 변수

// 스레드 핸들 배열
int i, j, k;
HANDLE pthread[MAX_ROW][MAX_COL] = { NULL };
DWORD dwthreadID = NULL;

// 스레드 함수
unsigned int WINAPI ThreadFunction(void* arg) {
    Sleep(1000); // Sleep for 1 second
    return 0;
}

// 행렬 읽기 함수
void readMatrix(int A[MAX_ROW][MAX_COL], int& Ar, int& Ac, char* filename) {
    char line[MAX_LINE], * tok; // 문자열 및 토큰 파싱을 위한 변수 선언
    FILE* fp; // 파일 포인터

    // 파일을 읽기 모드로 엽니다.
    if (!(fp = fopen(filename, "r"))) {
        printf("ERROR: file open\n"); // 파일 열기 실패 시 오류 메시지 출력
        exit(0); // 프로그램 종료
    }

    Ar = 0; // 행 수 초기화
    char* str2 = NULL;

    // 파일 이름에서 "x"로 분리하여 행(row) 및 열(column) 개수 추출
    char* temp = strtok_s(filename, "x", &str2);
    Ar = atoi(temp); // 문자열을 정수로 변환하여 행 수로 저장
    Ac = atoi(str2); // 문자열을 정수로 변환하여 열 수로 저장

    Ar = 0; // 행 인덱스 초기화

    // 파일에서 한 줄씩 읽어와 행렬 데이터를 읽음
    while (fgets(line, MAX_LINE, fp)) {
        tok = strtok(line, "  "); // 공백을 구분자로 첫 번째 토큰 파싱
        Ac = 0; // 열 인덱스 초기화

        // 한 행의 각 열 요소에 대해 반복
        do {
            A[Ar][Ac] = atoi(tok); // 문자열을 정수로 변환하여 행렬 배열에 저장
            Ac++; // 열 인덱스 증가
        } while (tok = strtok(NULL, "  ")); // 다음 열 요소 파싱

        Ar++; // 다음 행으로 이동
    }

    fclose(fp); // 파일 닫기
}

// 행렬 곱 함수
void matrixMultiply(int A[MAX_ROW][MAX_COL], int Ar, int Ac, int B[MAX_ROW][MAX_COL], int Br, int Bc, int result[MAX_ROW][MAX_COL]) {
    // 이중 반복문을 사용하여 결과 행렬의 각 원소를 계산합니다.
    for (int i = 0; i < Ar; i++) { // 결과 행렬의 행에 대한 루프
        for (int j = 0; j < Bc; j++) { // 결과 행렬의 열에 대한 루프
            result[i][j] = 0; // 결과 행렬의 해당 위치 초기화

            for (int n = 0; n < Ac; n++) { // 내적을 계산하기 위한 루프
                // 결과 행렬의 해당 위치에 두 행렬의 각 원소를 곱하고 더합니다.
                result[i][j] += A[i][n] * B[n][j];
                pthread[i][j] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, NULL, 0, (unsigned*)&dwthreadID);
            }
        }
    }
}

// 결과 행렬 출력 함수
void printResult(int result[MAX_ROW][MAX_COL], int rows, int cols, const char* filename) {
    char file[15] = "result.txt"; // 결과를 파일에 출력하기 위한 파일 이름 (현재 하드코딩)
    FILE* fp = fopen(file, "w");   // 파일 열기 및 쓰기 모드로 열기

    if (!fp) {
        printf("ERROR: file open\n"); // 파일 열기 실패 시 에러 메시지 출력
        exit(0); // 프로그램 종료
    }

    printf("\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d\t", result[i][j]);   // 콘솔에 결과 행렬 값 출력
            fprintf(fp, " %5d", result[i][j]); // 파일에 결과 행렬 값 출력
        }
        printf("\n");      // 콘솔에서 행 종료
        fprintf(fp, "\n"); // 파일에서 행 종료
    }
    printf("\n");

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            CloseHandle(pthread[i][j]); // 스레드 핸들 닫기
        }
    }

    fclose(fp); // 파일 닫기
}



int main(int argc, char** argv) {
    if (argc < 3) {
        printf("더 많은 행렬 혹은 유효한 파일 이름 쓰기\n");
        return 0;
    }

    // 시간 체크 시작
    start = clock(); // 프로그램 시작 시간 측정
    printf("\n");

    // 메모리 초기화
    memset(A, 0, sizeof(int) * MAX_ROW * MAX_COL); // 첫 번째 행렬 초기화
    Ar = Ac = 0; // 첫 번째 행렬의 행(row) 및 열(column) 초기화
    memset(B, 0, sizeof(int) * MAX_ROW * MAX_COL); // 두 번째 행렬 초기화
    Br = Bc = 0; // 두 번째 행렬의 행(row) 및 열(column) 초기화

    // 첫 번째와 두 번째 행렬 읽기
    readMatrix(A, Ar, Ac, argv[1]); // 첫 번째 행렬을 파일에서 읽어옴
    readMatrix(B, Br, Bc, argv[2]); // 두 번째 행렬을 파일에서 읽어옴

    // A와 B의 행렬 곱하기
    ABr = ABc = 0; // 결과 행렬의 행(row) 및 열(column) 초기화
    matrixMultiply(A, Ar, Ac, B, Br, Bc, AB); // A와 B의 행렬 곱 수행

    Sleep(1000);
    printf(" ( %dx%d ) X ( %dx%d  ) = (%dx%d )\n ", Ar, Ac, Br, Bc, Ar, Bc);



    //A와 B의 곱한 행렬 result에 저장하기
    if (argc == 3) {
        // 결과를 출력할 파일 이름 지정
        char file[15] = { "result.txt" };
        FILE* fp = fopen(file, "w"); // 결과를 파일에 쓸 포인터 생성

        printf("\n");
        for (int i = 0; i < Ar; i++) {
            for (int j = 0; j < Bc; j++) {
                // AB 행렬의 요소 출력
                printf("%d\t", AB[i][j]);
                fprintf(fp, " %5d", AB[i][j]); // 결과 파일에 요소를 쓰기
            }
            printf("\n");
            fprintf(fp, "\n");
        }
        printf("\n");
        CloseHandle(pthread); // 스레드 핸들 닫기
        end = clock();

        printf("Processing Time\t: %.3f Sec\n", (double)(end - start) / CLOCKS_PER_SEC); // 처리 시간 출력
        fclose(fp); // 결과 파일 닫기
    }
    else if (argc >= 4)
    {
        int count;
        for (count = 3; count < argc; count++) {
            // 추가 행렬 C 읽기
            readMatrix(C, Cr[count - 3], Cc[count - 3], argv[count]);
            Sleep(1000);
            printf(" ( %dx%d ) X ( %dx%d ) = ( %dx%d )\n ", Ar, Cr[count - 3], Cr[count - 3], Cc[count - 3], Ar, Cc[count - 3]);

            if (count == 3) {
                // 첫 번째 행렬 곱셈
                for (int i = 0; i < Ar; i++) {
                    for (int j = 0; j < Cc[count - 3]; j++) {
                        D[i][j] = 0;
                        for (int n = 0; n < Cr[count - 3]; n++) {
                            // AB 행렬과 C 행렬의 곱셈 결과를 D 행렬에 저장
                            D[i][j] += AB[i][n] * C[n][j];
                            pthread[i][j] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, NULL, 0, (unsigned*)&dwthreadID);
                        }
                    }
                }
            }
            if (count == argc - 1) {
                char file[15] = { "result.txt" };
                FILE* fp = stdout;
                fp = fopen(file, "w");

                printf("\n");
                for (i = 0; i < Ar; i++) {
                    for (j = 0; j < Cc[count - 3]; j++) {
                        printf("%d\t", D[i][j]);
                        fprintf(fp, " %5d", D[i][j]);
                    }
                    printf("\n");
                    fprintf(fp, "\n");
                }
                printf("\n");
                CloseHandle(pthread);
                end = clock();

                printf("Processing Time\t: %.3f Sec\n", (double)(end - start) / CLOCKS_PER_SEC);
                fclose(fp);

            }
            else if (count == 4) {
                for (i = 0; i < Cr[count - 4]; i++) {
                    for (j = 0; j < Cc[count - 3]; j++) {
                        E[i][j] = 0;
                        for (int n = 0; n < Cr[count - 3]; n++) {
                            E[i][j] = E[i][j] + D[i][j] * C[n][j];
                            pthread[i][j] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, NULL, 0, (unsigned*)&dwthreadID);
                        }

                    }
                }
                if (count == argc - 1) {
                    char file[15] = { "result.txt" };
                    FILE* fp = stdout;
                    fp = fopen(file, "w");

                    printf("\n");
                    for (i = 0; i < Ar; i++) {
                        for (j = 0; j < Cc[count - 3]; j++) {
                            printf("%d\t", E[i][j]);
                            fprintf(fp, " %5d", E[i][j]);
                        }
                        printf("\n");
                        fprintf(fp, "\n");
                    }
                    printf("\n");
                    CloseHandle(pthread);
                    end = clock();

                    printf("Processing Time\t: %.3f Sec\n", (double)(end - start) / CLOCKS_PER_SEC);
                    fclose(fp);

                }
            }
            else if (count == 5) {
                for (i = 0; i < Cr[count - 5]; i++) {
                    for (j = 0; j < Cc[count - 4]; j++) {
                        F[i][j] = 0;
                        for (int n = 0; n < Cr[count - 4]; n++) {
                            F[i][j] = F[i][j] + E[i][j] * C[n][j];
                            pthread[i][j] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, NULL, 0, (unsigned*)&dwthreadID);
                        }

                    }
                }
                if (count == argc - 1) {
                    char file[15] = { "result.txt" };
                    FILE* fp = stdout;
                    fp = fopen(file, "w");

                    printf("\n");
                    for (i = 0; i < Ar; i++) {
                        for (j = 0; j < Cc[count - 3]; j++) {
                            printf("%d\t", F[i][j]);
                            fprintf(fp, " %5d", F[i][j]);
                        }
                        printf("\n");
                        fprintf(fp, "\n");
                    }
                    printf("\n");
                    CloseHandle(pthread);
                    end = clock();

                    printf("Processing Time\t: %.3f Sec\n", (double)(end - start) / CLOCKS_PER_SEC);
                    fclose(fp);

                }
            }
            else if (count == 6) {
                for (i = 0; i < Cr[count - 6]; i++) {
                    for (j = 0; j < Cc[count - 5]; j++) {
                        F[i][j] = 0;
                        for (int n = 0; n < Cr[count - 5]; n++) {
                            F[i][j] = F[i][j] + E[i][j] * C[n][j];
                            pthread[i][j] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, NULL, 0, (unsigned*)&dwthreadID);
                        }

                    }
                }
                if (count == argc - 1) {
                    char file[15] = { "result.txt" };
                    FILE* fp = stdout;
                    fp = fopen(file, "w");

                    printf("\n");
                    for (i = 0; i < Ar; i++) {
                        for (j = 0; j < Cc[count - 3]; j++) {
                            printf("%d\t", F[i][j]);
                            fprintf(fp, " %5d", F[i][j]);
                        }
                        printf("\n");
                        fprintf(fp, "\n");
                    }
                    printf("\n");
                    CloseHandle(pthread);
                    end = clock();

                    printf("Processing Time\t: %.3f Sec\n", (double)(end - start) / CLOCKS_PER_SEC);
                    fclose(fp);

                }

            }
        }
    }

    return 0;
}
