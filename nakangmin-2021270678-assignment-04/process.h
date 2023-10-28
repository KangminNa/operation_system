#pragma once
#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <vector>
#include <tchar.h> // TCHAR를 사용하기 위한 헤더
#include <wtypes.h> // DWORD를 사용하기 위한 헤더

class Process {
    TCHAR* name; // 프로세스의 이름
    DWORD pid, ppid; // 프로세스 ID, 부모 프로세스 ID
    DWORD newPpid; // 부모 프로세스가 종료될 경우, 이 프로세스를 루트 프로세스의 자식으로 설정합니다.
    // 이러한 경우에 메모리 낭비를 줄이기 위해 포인터 대신 값을 사용합니다.
    std::vector<Process> children; // 자식 프로세스를 저장하는 벡터

public:
    /// 생성자 함수 ///
    Process(TCHAR name[], DWORD pid, DWORD ppid);

    /// 프로세스 정보 얻는 함수들 ///
    Process* getChild(int i); // i번째 자식 프로세스의 포인터를 반환합니다.
    int getSizeOfChildren(); // 자식 프로세스 벡터의 크기를 반환합니다.
    DWORD getPID(); // 현재 프로세스의 프로세스 ID를 반환합니다.
    DWORD getPPID(); // 현재 프로세스의 부모 프로세스 ID를 반환합니다.

    /// 프로세스 정보 세팅 함수들 ///
    void setNewPPID(DWORD newPpid); // 새로운 부모 프로세스 ID를 설정합니다.
    void addChild(Process p); // 자식 프로세스를 추가합니다.

    // 프로세스가 자식 프로세스를 가지고 있는지 확인합니다.
    bool hasChild();

    // 프로세스 정보 출력
    void printProcInfo();
};

#endif
