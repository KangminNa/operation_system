#include <iostream>
#include <wtypes.h> // 프로세스 개수를 얻기 위한 헤더
#include <Psapi.h> // 프로세스 정보를 얻기 위한 헤더
#include <TlHelp32.h> // 프로세스 스냅샷을 생성하기 위한 헤더
#include "pstree.h" // PSTree 클래스 헤더

int main() {

    /// 식별자 선언 ///
    HANDLE hProcessSnap; // 프로세스 스냅샷 핸들
    PROCESSENTRY32 pe32; // 프로세스 정보를 담는 구조체
    int numOfProc = 0; // 실행 중인 프로세스 수

    /// 프로세스 스냅샷 생성 ///
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        puts("CreateToolhelp32Snapchot 오류");
        exit(EXIT_FAILURE);
    }

    /// 첫 번째 프로세스(초기화 프로세스) 얻기 ///
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hProcessSnap, &pe32)) {
        puts("Process32First 오류!");
        CloseHandle(hProcessSnap);
        exit(EXIT_FAILURE);
    }
    numOfProc++;

    /// 모든 프로세스 얻기 ///
    PSTree pstree(pe32.szExeFile, pe32.th32ProcessID, pe32.th32ParentProcessID); // 초기 프로세스를 pstree의 루트 프로세스로 설정
    // 다음 프로세스들의 정보를 받아 트리를 만듭니다.
    while (Process32Next(hProcessSnap, &pe32)) {
        pstree.addProc(pe32.szExeFile, pe32.th32ProcessID, pe32.th32ParentProcessID);
        numOfProc++;
    }

    /// 출력 ///
    puts("############### Process Tree ################");
    printf("Number of Running Processes = %d\n", numOfProc);
    pstree.printPSTree();

}
