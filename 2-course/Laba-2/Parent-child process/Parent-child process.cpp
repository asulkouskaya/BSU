#include <windows.h>
#include <conio.h>
#include <iostream>
using namespace std;
#pragma warning(disable : 4996)

int main() {
  //  char lpszAppName[] = "F:\\BSU-FAMCs\\2-course\\OS\\Parent-child-process\\x64\\Debug\\Child.exe";
    char lpszAppName[] = "Child.exe";
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USEFILLATTRIBUTE;
    si.dwFillAttribute = BACKGROUND_RED;
    char title[] = "child title";
    si.lpTitle = (LPWSTR)title;

    int* arr;
    int n;
    cout << "Enter size of array: \n";
    cin >> n;
    arr = new int[n];
    cout << "Enter elements of array: \n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    char* a = new char[8];
    int radix = 10;

    char args[1000];
    strcpy_s(args, _itoa(n, a, radix));
    for (int i = 0; i < n; i++) {
        strcat_s(args, " ");
        strcat_s(args, _itoa(arr[i], a, radix));
    }

    if (!CreateProcessA((LPCSTR)lpszAppName, args, nullptr, nullptr, FALSE,
        CREATE_NEW_CONSOLE, nullptr, nullptr, (LPSTARTUPINFOA)&si, &pi))
    {
        _cputs("The new process is not created.\n");
        _cputs("Check a name of the process.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return 0;
    }
    _cputs("The new process is created.\n");
    WaitForSingleObject(pi.hProcess, INFINITE);
    // закрываем дескрипторы этого процесса в текущем процессе
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return 0;
}