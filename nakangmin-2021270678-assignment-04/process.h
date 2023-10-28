#pragma once
#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <vector>
#include <tchar.h> // TCHAR�� ����ϱ� ���� ���
#include <wtypes.h> // DWORD�� ����ϱ� ���� ���

class Process {
    TCHAR* name; // ���μ����� �̸�
    DWORD pid, ppid; // ���μ��� ID, �θ� ���μ��� ID
    DWORD newPpid; // �θ� ���μ����� ����� ���, �� ���μ����� ��Ʈ ���μ����� �ڽ����� �����մϴ�.
    // �̷��� ��쿡 �޸� ���� ���̱� ���� ������ ��� ���� ����մϴ�.
    std::vector<Process> children; // �ڽ� ���μ����� �����ϴ� ����

public:
    /// ������ �Լ� ///
    Process(TCHAR name[], DWORD pid, DWORD ppid);

    /// ���μ��� ���� ��� �Լ��� ///
    Process* getChild(int i); // i��° �ڽ� ���μ����� �����͸� ��ȯ�մϴ�.
    int getSizeOfChildren(); // �ڽ� ���μ��� ������ ũ�⸦ ��ȯ�մϴ�.
    DWORD getPID(); // ���� ���μ����� ���μ��� ID�� ��ȯ�մϴ�.
    DWORD getPPID(); // ���� ���μ����� �θ� ���μ��� ID�� ��ȯ�մϴ�.

    /// ���μ��� ���� ���� �Լ��� ///
    void setNewPPID(DWORD newPpid); // ���ο� �θ� ���μ��� ID�� �����մϴ�.
    void addChild(Process p); // �ڽ� ���μ����� �߰��մϴ�.

    // ���μ����� �ڽ� ���μ����� ������ �ִ��� Ȯ���մϴ�.
    bool hasChild();

    // ���μ��� ���� ���
    void printProcInfo();
};

#endif
