#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <TlHelp32.h>

#define MAX_PATH 1000

struct Process {
    TCHAR name[MAX_PATH];
    DWORD pid, ppid;
    struct Process* children;
    int childCount;
};

struct PSTree {
    struct Process* root;
};

struct Process createProcess(TCHAR name[], DWORD pid, DWORD ppid) {
    struct Process proc;
    _tcscpy(proc.name, name);
    proc.pid = pid;
    proc.ppid = ppid;
    proc.children = NULL;
    proc.childCount = 0;
    return proc;
}

struct PSTree createPSTree(DWORD pidOfRoot) {
    struct PSTree pstree;
    pstree.root = (struct Process*)malloc(sizeof(struct Process));

    if (pstree.root != NULL) {
        _tcscpy(pstree.root->name, _T("System Idle Process"));
        pstree.root->pid = pidOfRoot;
        pstree.root->ppid = 0;
        pstree.root->children = NULL;
        pstree.root->childCount = 0;
    }
    else {
        _tprintf(_T("Memory allocation failed\n"));
    }
    return pstree;
}

void addChild(struct Process* parent, struct Process child) {
    if (parent != NULL) {
        parent->children = (struct Process*)realloc(parent->children, (parent->childCount + 1) * sizeof(struct Process));
        if (parent->children != NULL) {
            parent->children[parent->childCount] = child;
            parent->childCount++;
        }
    }
}

void addProc(struct PSTree* pstree, TCHAR name[], DWORD pid, DWORD ppid) {
    if (ppid == 0) {
        // 루트 프로세스의 경우
        _tcscpy(pstree->root->name, name);
        pstree->root->pid = pid;
        pstree->root->ppid = 0;
    }
    else {
        // 나머지 프로세스를 추가
        struct Process newProc = createProcess(name, pid, ppid);
        struct Process* parent = pstree->root;
        while (parent != NULL) {
            if (parent->pid == ppid) {
                addChild(parent, newProc);
                break;
            }
            if (parent->childCount == 0) {
                parent = NULL;
            }
            else {
                parent = &parent->children[parent->childCount - 1];
            }
        }
    }
}

void printProcess(struct Process* currentProc, int indent, int* iOfProc) {
    printf("%03d ", (*iOfProc)++);
    for (int j = 0; j < indent; j++) {
        printf(":\t");
    }
    _tprintf(_T("+-%s<%lu><%lu>\n"), currentProc->name, currentProc->pid, currentProc->ppid);

    for (int i = 0; i < currentProc->childCount; i++) {
        printProcess(&currentProc->children[i], indent + 1, iOfProc);
    }
}

void printPSTree(struct PSTree pstree) {
    int iOfProc = 1;
    printf("%03d ", iOfProc++);
    _tprintf(_T("+-%s<%lu><%lu>\n"), pstree.root->name, pstree.root->pid, pstree.root->ppid);
    for (int i = 0; i < pstree.root->childCount; i++) {
        printProcess(&pstree.root->children[i], 0, &iOfProc);
    }
}

int main() {
    struct PSTree pstree;
    DWORD rootPID = 0;
    DWORD numOfProc = 0;

    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    // Initialize pstree with the root process
    pstree = createPSTree(rootPID);

    if (hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) {
        if (Process32First(hProcessSnap, &pe32)) {
            addProc(&pstree, pe32.szExeFile, pe32.th32ProcessID, pe32.th32ParentProcessID);
            numOfProc++;
        }
    }
    else {
        puts("CreateToolhelp32Snapshot error");
        exit(EXIT_FAILURE);
    }

    // Get all processes and build the tree
    while (Process32Next(hProcessSnap, &pe32)) {
        addProc(&pstree, pe32.szExeFile, pe32.th32ProcessID, pe32.th32ParentProcessID);
        numOfProc++;
    }

    // Output the process tree
    puts("############### Process Tree ################");
    printf("Number of Running Processes = %lu\n", numOfProc);
    printPSTree(pstree);

    return 0;
}
