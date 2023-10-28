#include <iostream>
#include <wtypes.h> // DWORD를 사용하기 위함
#include <tchar.h> // TCHAR를 사용하기 위함
#include "process.h"
#include "pstree.h"

// 생성자 함수
// 루트 프로세스 초기화
PSTree::PSTree(TCHAR nameOfRoot[], DWORD pidOfRoot, DWORD ppidOfRoot) {
	root = new Process(nameOfRoot, pidOfRoot, ppidOfRoot);
}

// 입력: 현재 프로세스의 주소, 부모 프로세스의 PID
// 출력: 부모 프로세스의 주소를 반환
//        부모 프로세스를 찾을 수 없는 경우(부모 프로세스가 종료된 경우) null을 반환
Process* PSTree::findParentProcess(Process* currentProcess, DWORD ppid) {
	// 만약 현재 프로세스가 찾아야 하는 부모 프로세스라면 현재 프로세스의 주소를 반환
	if (currentProcess->getPID() == ppid) {
		return currentProcess;
	}

	// 현재 프로세스에 자식 프로세스가 있는 경우
	if (currentProcess->hasChild()) {
		Process* child = NULL;
		Process* parent = NULL;

		// 현재 프로세스의 자식 프로세스들을 검색하여 찾아야 하는 부모 프로세스를 찾음
		for (std::vector<Process>::size_type iOfChild = 0; iOfChild < currentProcess->getSizeOfChildren(); iOfChild++) {
			child = currentProcess->getChild(iOfChild);
			if (parent = findParentProcess(child, ppid)) // 찾아야 하는 부모 프로세스를 찾으면 해당 프로세스 반환
				return parent;
		}
		// 현재 프로세스의 모든 자식 프로세스를 검색해도 부모 프로세스를 찾지 못하면 null 반환
		return NULL;
	}
	// 현재 프로세스가 찾아야 하는 부모 프로세스가 아니거나 자식이 없는 경우 null 반환
	else {
		return NULL;
	}
}

// 새로운 프로세스를 프로세스 트리에 추가
// 입력: 새 프로세스의 정보
void PSTree::addProc(TCHAR name[], DWORD pid, DWORD ppid) {
	Process newProc(name, pid, ppid);

	// 새 프로세스의 부모 프로세스를 찾음
	Process* parent = findParentProcess(root, newProc.getPPID());
	// 부모 프로세스를 찾을 수 없다면 루트 프로세스를 새 프로세스의 부모로 설정
	if (parent == NULL) {
		newProc.setNewPPID(0);
		root->addChild(newProc);
	}
	// 부모 프로세스를 찾았다면 새 프로세스를 해당 부모 프로세스의 자식으로 추가
	else {
		newProc.setNewPPID(parent->getPID());
		parent->addChild(newProc);
	}
}

// 입력: 현재 프로세스, 출력될 정보의 들여쓰기 수, 프로세스 정보의 인덱스
// 출력: 현재 프로세스의 정보를 출력
void PSTree::printProcess(Process* currentProc, int indent, int* iOfProc) {
	// 현재 프로세스에 자식 프로세스가 있는 경우, 해당 자식 프로세스들의 정보 출력
	Process* child;
	for (std::vector<Process>::size_type iOfChild = 0; iOfChild < currentProc->getSizeOfChildren(); iOfChild++) {
		printf("%03d ", (*iOfProc)++);
		for (int j = 0; j <= indent; j++) {
			printf(":\t");
		}
		child = currentProc->getChild(iOfChild);
		child->printProcInfo();

		// 자식 프로세스가 또 다른 자식 프로세스를 가지고 있다면 그것들도 출력
		if (child->hasChild()) {
			printProcess(child, indent + 1, iOfProc);
		}
	}
	indent++;
}

// 프로세스 트리 출력
void PSTree::printPSTree() {
	int iOfProc = 1; // 출력할 프로세스의 인덱스

	// 먼저 루트 프로세스의 정보 출력
	printf("%03d ", iOfProc++);
	root->printProcInfo();
	// 그런 다음 모든 프로세스 정보 출력
	printProcess(root, 0, &iOfProc);
}