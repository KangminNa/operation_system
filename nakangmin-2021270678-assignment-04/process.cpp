#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <tchar.h> // TCHAR�� ����ϱ� ���� ���
#include <wtypes.h> // DWORD�� ����ϱ� ���� ���
#include "process.h"

/// ������ �Լ� ///
Process::Process(TCHAR name[], DWORD pid, DWORD ppid) {
    // ���μ��� ��ü�� ������ �Լ��Դϴ�.

    // �̸� ���ڿ��� ���̸� ����մϴ�.
    int nameLength = _tcslen(name);

    // �̸��� ������ �޸𸮸� �������� �Ҵ��մϴ�.
    this->name = new TCHAR[nameLength + 1];

    // �̸��� �����մϴ�.
    _tcscpy(this->name, name);

    // ���μ��� ID�� �����մϴ�.
    this->pid = pid;

    // �θ� ���μ��� ID�� �����մϴ�.
    this->ppid = ppid;

    // ���ο� �θ� ���μ��� ID�� ���� �θ� ���μ��� ID�� �ʱ�ȭ�մϴ�.
    newPpid = ppid;
}

/// ���μ��� ���� �������� �Լ��� ///
Process* Process::getChild(int i) {
    // i��° �ڽ� ���μ����� �����͸� ��ȯ�մϴ�.
    return &children[i];
}

int Process::getSizeOfChildren() {
    // �ڽ� ���μ��� ������ ũ�⸦ ��ȯ�մϴ�.
    return children.size();
}

DWORD Process::getPID() {
    // ���� ���μ����� ���μ��� ID�� ��ȯ�մϴ�.
    return pid;
}

DWORD Process::getPPID() {
    // ���� ���μ����� �θ� ���μ��� ID�� ��ȯ�մϴ�.
    return ppid;
}

/// ���μ��� ���� ���� �Լ��� ///
void Process::setNewPPID(DWORD newPpid) {
    // ���ο� �θ� ���μ��� ID�� �����մϴ�.
    this->newPpid = newPpid;
}

void Process::addChild(Process p) {
    // �ڽ� ���μ����� �߰��մϴ�.
    children.push_back(p);
}

// ���μ����� �ڽ� ���μ����� ������ �ִ��� Ȯ���մϴ�.
bool Process::hasChild() {
    if (children.size()) return true;
    return false;
}

// ���μ��� ���� ���
void Process::printProcInfo() {
    // ���μ��� ������ ����մϴ�.
    _tprintf(_T("+-%s<%lu><%lu:%lu>\n"), name, pid, newPpid, ppid);
}