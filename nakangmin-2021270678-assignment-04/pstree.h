#pragma once
#ifndef _PSTREE_H_
#define _PSTREE_H_

#include <wtypes.h> // 실행 중인 프로세스 수를 가져오기 위해 필요
#include <tchar.h> // tchar를 사용하기 위해 필요
#include "process.h"

class PSTree {
    Process* root; // 루트 프로세스(초기 프로세스)
    // 동적 할당(Process의 매개변수 있는 생성자 사용을 위해 포인터로 선언

public:
    // 생성자 함수
    // 루트 프로세스 초기화
    PSTree(TCHAR nameOfRoot[], DWORD pidOfRoot, DWORD ppidOfRoot);

    // 입력: 현재 프로세스의 주소, 부모 프로세스의 PID
    // 출력: 부모 프로세스의 주소 반환
    //        부모 프로세스를 찾을 수 없는 경우(부모 프로세스가 종료된 경우) null 반환
    Process* findParentProcess(Process* currentProcess, DWORD ppid);

    // 프로세스 트리에 새로운 프로세스 추가
    // 입력: 새 프로세스의 정보
    void addProc(TCHAR name[], DWORD pid, DWORD ppid);

    // 입력: 현재 프로세스, 프로세스 정보를 출력할 들여쓰기 수, 프로세스 정보의 인덱스
    // 출력: 현재 프로세스의 정보 출력
    void printProcess(Process* currentProc, int indent, int* iOfProc);

    // 프로세스 트리 출력
    void printPSTree();
};

#endif