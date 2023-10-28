#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <tchar.h> // TCHAR를 사용하기 위한 헤더
#include <wtypes.h> // DWORD를 사용하기 위한 헤더
#include "process.h"

/// 생성자 함수 ///
Process::Process(TCHAR name[], DWORD pid, DWORD ppid) {
    // 프로세스 객체의 생성자 함수입니다.

    // 이름 문자열의 길이를 계산합니다.
    int nameLength = _tcslen(name);

    // 이름을 저장할 메모리를 동적으로 할당합니다.
    this->name = new TCHAR[nameLength + 1];

    // 이름을 복사합니다.
    _tcscpy(this->name, name);

    // 프로세스 ID를 설정합니다.
    this->pid = pid;

    // 부모 프로세스 ID를 설정합니다.
    this->ppid = ppid;

    // 새로운 부모 프로세스 ID를 현재 부모 프로세스 ID로 초기화합니다.
    newPpid = ppid;
}

/// 프로세스 정보 가져오는 함수들 ///
Process* Process::getChild(int i) {
    // i번째 자식 프로세스의 포인터를 반환합니다.
    return &children[i];
}

int Process::getSizeOfChildren() {
    // 자식 프로세스 벡터의 크기를 반환합니다.
    return children.size();
}

DWORD Process::getPID() {
    // 현재 프로세스의 프로세스 ID를 반환합니다.
    return pid;
}

DWORD Process::getPPID() {
    // 현재 프로세스의 부모 프로세스 ID를 반환합니다.
    return ppid;
}

/// 프로세스 정보 설정 함수들 ///
void Process::setNewPPID(DWORD newPpid) {
    // 새로운 부모 프로세스 ID를 설정합니다.
    this->newPpid = newPpid;
}

void Process::addChild(Process p) {
    // 자식 프로세스를 추가합니다.
    children.push_back(p);
}

// 프로세스가 자식 프로세스를 가지고 있는지 확인합니다.
bool Process::hasChild() {
    if (children.size()) return true;
    return false;
}

// 프로세스 정보 출력
void Process::printProcInfo() {
    // 프로세스 정보를 출력합니다.
    _tprintf(_T("+-%s<%lu><%lu:%lu>\n"), name, pid, newPpid, ppid);
}