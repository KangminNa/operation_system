#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996) // Disable warning for unsafe functions

int main(int argc, char* argv[]) {
    // 변수 및 배열 선언
    char buffer1[100], buffer2[100];
    int c = 0;
    int res = 0;
    int process = 0, cnt = 0;
    int allocation[16][11], max[16][11], Available[11], need[16][11], work[11];
    bool fin[16];
    int safeseq[16] = { 0, }, seq = 0;

    // 파일 읽기
    FILE* fp = fopen(argv[1], "r");
    fgets(buffer1, 100, fp);
    strcpy(buffer2, buffer1);
    int len = strlen(buffer2);

    // 첫 번째 라인에서 리소스 및 프로세스 수 읽기
    for (int i = 0; i < len; i++) {
        if (buffer2[i] != ' ' && res == 0) {
            res = atoi(&buffer2[i]);
        }
        else if (buffer2[i] != ' ' && process == 0) {
            process = atoi(&buffer2[i]);
        }
    }
    printf("\t%d\t%d \n\n", res, process);

    // Allocation, Max, Available 배열에 값을 입력하고 출력하기
    printf("Allocation\t");
    for (int i = 0; i < res; i++) {
        printf("%c\t", i + 65); // 리소스 레이블 (A, B, C, ...)
    }
    printf("\n");

    // Allocation 입력
    fgets(buffer1, sizeof(buffer1), fp);
    fgets(buffer1, sizeof(buffer1), fp);

    for (int i = 0; i < process; i++) {
        fgets(buffer1, sizeof(buffer1), fp);
        strcpy(buffer2, buffer1);
        len = strlen(buffer2);

        for (int j = 4; j < len; j++) {
            if (buffer2[j] != ' ') {
                allocation[i][c] = atoi(&buffer2[j]);
                c++;
            }
        }
        c = 0;

        printf("P_%02d\t", i);
        for (int k = 0; k < res; k++) {
            printf("\t%d", allocation[i][k]);
        }
        printf("\n");
    }

    // Max 입력 및 출력
    fgets(buffer1, sizeof(buffer1), fp);
    fgets(buffer1, sizeof(buffer1), fp);
    printf("\n");
    printf("MAX\t\t");
    for (int i = 0; i < res; i++) {
        printf("%c\t", i + 65);
    }
    printf("\n");
    for (int j = 0; j < process; j++) {
        fgets(buffer1, sizeof(buffer1), fp);
        strcpy(buffer2, buffer1);
        len = strlen(buffer2);

        for (int i = 4; i < len; i++) {
            if (buffer2[i] != ' ') {
                max[j][c] = atoi(&buffer2[i]);
                c++;
            }
        }
        c = 0;
        printf("P_%02d\t", j);

        for (int k = 0; k < res; k++) {
            printf("\t%d", max[j][k]);
        }
        printf("\n");
    }

    // Available 입력 및 출력
    fgets(buffer1, sizeof(buffer1), fp);
    fgets(buffer1, sizeof(buffer1), fp);
    printf("\n");
    printf("Available\t");
    for (int i = 0; i < res; i++) {
        printf("%c\t", i + 65);
    }
    printf("\n\t");

    fgets(buffer1, sizeof(buffer1), fp);
    strcpy(buffer2, buffer1);
    len = strlen(buffer2);
    for (int i = 0; i < len; i++) {
        if (buffer2[i] != ' ') {
            Available[c] = atoi(&buffer2[i]);
            work[c] = Available[c];
            c++;
        }
    }
    for (int i = 0; i < res; i++) {
        printf("\t%d", Available[i]);
    }

    // Finish 배열 초기화
    for (int i = 0; i < process; i++) {
        fin[i] = false;
    }

    // Need 배열 계산
    for (int i = 0; i < process; i++) {
        for (int j = 0; j < res; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    // Safety 알고리즘 실행
    printf("\n----------------------------------------------------------------------\n");
    cnt = 0;
    int k = 0;
    int i = 0;
    int j = 0;
    int exit = 0;
    int loopCnt = 0;

    for (i = 0; i < process; i++) {
        j = 0;
        cnt = 0;
        exit = 0;
        loopCnt = 0;

        while (!exit) {
            if (fin[j] == false) {
                for (k = 0; k < res; k++) {
                    if (work[k] >= need[j][k])
                        cnt++;
                }

                if (cnt == res) {
                    fin[j] = true;

                    for (k = 0; k < res; k++)
                        work[k] += allocation[j][k];

                    safeseq[seq] = j;
                    seq++;
                    exit = 1;
                }
            }
            j++;
            cnt = 0;
            loopCnt++;

            if (loopCnt == process)
                exit = 1;
        }
    }

    // 결과 출력
    cnt = 0;
    for (j = 0; j < process; j++) {
        if (fin[j] == true) cnt++;
    }

    printf("Result: ");
    if (cnt == process) {
        printf("Safe --> ");
        for (i = 0; i < process; i++)
            printf(" P_%02d ", safeseq[i]);
    }
    else {
        printf("UnSafe --> ");
        for (j = 0; j < process; j++) {
            if (fin[j] == false)
                printf(" P_%02d ", j);
        }
    }
    printf("\n----------------------------------------------------------------------\n");


    fclose(fp); // 파일 닫기
    return 0;
}
