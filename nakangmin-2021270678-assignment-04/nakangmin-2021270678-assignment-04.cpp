#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int currentProcessIndex = 1; // 현재 프로세스 인덱스를 나타내는 변수
int totalProcess = 0;

// TreeNode 구조체 정의
typedef struct TreeNode {
    DWORD pid;
    DWORD parentPid;
    char name[MAX_PATH];
    struct TreeNode* sibling;
    struct TreeNode* child;
} TreeNode;


// TreeNode 생성 함수
TreeNode* CreateTreeNode(DWORD pid, DWORD parentPid, const char* name) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->pid = pid;
    node->parentPid = parentPid;
    strncpy(node->name, name, MAX_PATH);
    node->sibling = NULL;
    node->child = NULL;
    return node;
}



// 자식 노드를 추가하는 함수
void InsertChild(TreeNode* parent, TreeNode* child) {
    if (parent->child == NULL) {
        parent->child = child;
    }
    else {
        TreeNode* sibling = parent->child;
        while (sibling->sibling != NULL) {
            sibling = sibling->sibling;
        }
        sibling->sibling = child;
    }
}

// 프로세스 트리를 출력하는 함수
void PrintProcessTree(TreeNode* node, int level) {
    
    char indentation[512] = ""; // 들여쓰기를 나타내는 문자열

    // 현재 레벨만큼 들여쓰기 공백 문자열 생성
    for (int i = 0; i < level; i++) {
        strcat(indentation, "  |");
    }

 

    // 출력 형태를 들여쓰기, 프로세스 이름, 자신의 PID, 부모 PID
    printf("%3d %s+-%s <%lu> <%lu>\n",currentProcessIndex, indentation, node->name, node->pid, node->parentPid);
    currentProcessIndex++;

    TreeNode* child = node->child;
    while (child != NULL) {
        PrintProcessTree(child, level + 1); // 재귀적으로 자식 노드 출력
        child = child->sibling;
    }
}

int main() {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    TreeNode* root = NULL; // 루트 노드 초기화

    if (Process32First(snapshot, &pe)) {
        do {
            // WCHAR*를 char*로 변환
            int len = WideCharToMultiByte(CP_UTF8, 0, pe.szExeFile, -1, NULL, 0, NULL, NULL);
            char* exeName = (char*)malloc(len);
            WideCharToMultiByte(CP_UTF8, 0, pe.szExeFile, -1, exeName, len, NULL, NULL);

            TreeNode* node = CreateTreeNode(pe.th32ProcessID, pe.th32ParentProcessID, exeName);
            free(exeName);

            if (pe.th32ParentProcessID == 0) {
                // 부모 PID가 0인 프로세스를 루트로 설정
                root = node;
            }
            else {
                // 부모 PID에 해당하는 노드를 찾아서 자식으로 추가
                TreeNode* parent = root;
                if (parent != NULL) {
                    InsertChild(parent, node);
                    totalProcess++;
                }
            }
        } while (Process32Next(snapshot, &pe));
    }

    CloseHandle(snapshot);

    if (root != NULL) {
        printf(" ############## Process Tree ##############\n");
        printf("Number of Running Processes = %d\n", totalProcess + 1);
        PrintProcessTree(root, 0);
    }

    return 0;
}


