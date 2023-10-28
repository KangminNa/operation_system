#pragma once
#ifndef _PSTREE_H_
#define _PSTREE_H_

#include <wtypes.h> // ���� ���� ���μ��� ���� �������� ���� �ʿ�
#include <tchar.h> // tchar�� ����ϱ� ���� �ʿ�
#include "process.h"

class PSTree {
    Process* root; // ��Ʈ ���μ���(�ʱ� ���μ���)
    // ���� �Ҵ�(Process�� �Ű����� �ִ� ������ ����� ���� �����ͷ� ����

public:
    // ������ �Լ�
    // ��Ʈ ���μ��� �ʱ�ȭ
    PSTree(TCHAR nameOfRoot[], DWORD pidOfRoot, DWORD ppidOfRoot);

    // �Է�: ���� ���μ����� �ּ�, �θ� ���μ����� PID
    // ���: �θ� ���μ����� �ּ� ��ȯ
    //        �θ� ���μ����� ã�� �� ���� ���(�θ� ���μ����� ����� ���) null ��ȯ
    Process* findParentProcess(Process* currentProcess, DWORD ppid);

    // ���μ��� Ʈ���� ���ο� ���μ��� �߰�
    // �Է�: �� ���μ����� ����
    void addProc(TCHAR name[], DWORD pid, DWORD ppid);

    // �Է�: ���� ���μ���, ���μ��� ������ ����� �鿩���� ��, ���μ��� ������ �ε���
    // ���: ���� ���μ����� ���� ���
    void printProcess(Process* currentProc, int indent, int* iOfProc);

    // ���μ��� Ʈ�� ���
    void printPSTree();
};

#endif