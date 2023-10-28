#include <iostream>
#include <wtypes.h> // DWORD�� ����ϱ� ����
#include <tchar.h> // TCHAR�� ����ϱ� ����
#include "process.h"
#include "pstree.h"

// ������ �Լ�
// ��Ʈ ���μ��� �ʱ�ȭ
PSTree::PSTree(TCHAR nameOfRoot[], DWORD pidOfRoot, DWORD ppidOfRoot) {
	root = new Process(nameOfRoot, pidOfRoot, ppidOfRoot);
}

// �Է�: ���� ���μ����� �ּ�, �θ� ���μ����� PID
// ���: �θ� ���μ����� �ּҸ� ��ȯ
//        �θ� ���μ����� ã�� �� ���� ���(�θ� ���μ����� ����� ���) null�� ��ȯ
Process* PSTree::findParentProcess(Process* currentProcess, DWORD ppid) {
	// ���� ���� ���μ����� ã�ƾ� �ϴ� �θ� ���μ������ ���� ���μ����� �ּҸ� ��ȯ
	if (currentProcess->getPID() == ppid) {
		return currentProcess;
	}

	// ���� ���μ����� �ڽ� ���μ����� �ִ� ���
	if (currentProcess->hasChild()) {
		Process* child = NULL;
		Process* parent = NULL;

		// ���� ���μ����� �ڽ� ���μ������� �˻��Ͽ� ã�ƾ� �ϴ� �θ� ���μ����� ã��
		for (std::vector<Process>::size_type iOfChild = 0; iOfChild < currentProcess->getSizeOfChildren(); iOfChild++) {
			child = currentProcess->getChild(iOfChild);
			if (parent = findParentProcess(child, ppid)) // ã�ƾ� �ϴ� �θ� ���μ����� ã���� �ش� ���μ��� ��ȯ
				return parent;
		}
		// ���� ���μ����� ��� �ڽ� ���μ����� �˻��ص� �θ� ���μ����� ã�� ���ϸ� null ��ȯ
		return NULL;
	}
	// ���� ���μ����� ã�ƾ� �ϴ� �θ� ���μ����� �ƴϰų� �ڽ��� ���� ��� null ��ȯ
	else {
		return NULL;
	}
}

// ���ο� ���μ����� ���μ��� Ʈ���� �߰�
// �Է�: �� ���μ����� ����
void PSTree::addProc(TCHAR name[], DWORD pid, DWORD ppid) {
	Process newProc(name, pid, ppid);

	// �� ���μ����� �θ� ���μ����� ã��
	Process* parent = findParentProcess(root, newProc.getPPID());
	// �θ� ���μ����� ã�� �� ���ٸ� ��Ʈ ���μ����� �� ���μ����� �θ�� ����
	if (parent == NULL) {
		newProc.setNewPPID(0);
		root->addChild(newProc);
	}
	// �θ� ���μ����� ã�Ҵٸ� �� ���μ����� �ش� �θ� ���μ����� �ڽ����� �߰�
	else {
		newProc.setNewPPID(parent->getPID());
		parent->addChild(newProc);
	}
}

// �Է�: ���� ���μ���, ��µ� ������ �鿩���� ��, ���μ��� ������ �ε���
// ���: ���� ���μ����� ������ ���
void PSTree::printProcess(Process* currentProc, int indent, int* iOfProc) {
	// ���� ���μ����� �ڽ� ���μ����� �ִ� ���, �ش� �ڽ� ���μ������� ���� ���
	Process* child;
	for (std::vector<Process>::size_type iOfChild = 0; iOfChild < currentProc->getSizeOfChildren(); iOfChild++) {
		printf("%03d ", (*iOfProc)++);
		for (int j = 0; j <= indent; j++) {
			printf(":\t");
		}
		child = currentProc->getChild(iOfChild);
		child->printProcInfo();

		// �ڽ� ���μ����� �� �ٸ� �ڽ� ���μ����� ������ �ִٸ� �װ͵鵵 ���
		if (child->hasChild()) {
			printProcess(child, indent + 1, iOfProc);
		}
	}
	indent++;
}

// ���μ��� Ʈ�� ���
void PSTree::printPSTree() {
	int iOfProc = 1; // ����� ���μ����� �ε���

	// ���� ��Ʈ ���μ����� ���� ���
	printf("%03d ", iOfProc++);
	root->printProcInfo();
	// �׷� ���� ��� ���μ��� ���� ���
	printProcess(root, 0, &iOfProc);
}