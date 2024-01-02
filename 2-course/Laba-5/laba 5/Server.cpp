#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;

int main() {
    cout << "Enter the length of the array: ";
    int arrayLength;
    cin >> arrayLength;

    int* array = new int[arrayLength];

    cout << "Enter the array elements: ";
    int elem;
    for (int i = 0; i < arrayLength; ++i) {
        cin >> elem;
        array[i] = elem;
    }
    cout << "\nInputed array: ";
    for (int i = 0; i < arrayLength; ++i) {
        cout << array[i] << " ";
    }
    cout << endl;

    int k;
    cout << "N: "; cin >> k;


    HANDLE hWritePipe, hReadPipe;

    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = FALSE;
    saAttr.lpSecurityDescriptor = NULL;

    // Create pipes
    if (!CreatePipe(&hReadPipe, &hWritePipe, &saAttr, 0)) {
        cout << "Create pipe failed" << endl;
        return GetLastError();
    }

    HANDLE hInheritWritePipe;

    if (!DuplicateHandle(
        GetCurrentProcess(),
        hWritePipe,
        GetCurrentProcess(),
        &hInheritWritePipe,
        0,
        TRUE,
        DUPLICATE_SAME_ACCESS))
    {
        _cputs("Duplicate handle failed.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return GetLastError();
    }
    // закрываем ненужный дескриптор
    CloseHandle(hWritePipe);


    HANDLE hInheritReadPipe;

    if (!DuplicateHandle(
        GetCurrentProcess(),
        hReadPipe,
        GetCurrentProcess(),
        &hInheritReadPipe,
        0,
        TRUE,
        DUPLICATE_SAME_ACCESS))
    {
        _cputs("Duplicate handle failed.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return GetLastError();
    }
    // закрываем ненужный дескриптор
    CloseHandle(hReadPipe);

    HANDLE small_event;
    small_event = CreateEvent(NULL, FALSE, FALSE, "small_event");

    DWORD dwBytesWrite;
    // Write array data to the pipe

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    char lpszComLine[80];
    wsprintf(lpszComLine, (LPCSTR)"%d %d %d", (int)hInheritWritePipe, (int)hInheritReadPipe, k);

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    // Create Consume process
    if (!CreateProcessA("F:\\BSU-FAMCs\\2-course\\OS\\laba-5\\x64\\Release\\Small.exe", (LPSTR)lpszComLine,
        NULL, NULL, TRUE,
        CREATE_NEW_CONSOLE,
        NULL, NULL,
        (LPSTARTUPINFOA)&si, &pi))
    {
        cout << "Small.exe was not launched.\n";
        return GetLastError();
    }

    if (!WriteFile(hInheritWritePipe, &arrayLength, sizeof(int), &dwBytesWrite, NULL)) {
        cout << "Write of length failed\n" << GetLastError();
        return GetLastError();
    }
    for (int i = 0; i < arrayLength; i++) {
        if (!WriteFile(hInheritWritePipe, &array[i], sizeof(int), &dwBytesWrite, NULL)) {
            cout << "Write of array failed\n" << GetLastError();
            return GetLastError();
        }
    }
    

    WaitForSingleObject(pi.hProcess, INFINITE);
    
    int new_size;
    DWORD byteRwad;
    if (!ReadFile(hInheritReadPipe, &new_size, sizeof(int), &byteRwad, NULL))
    {
        cout << "New size read is failed " << endl;
        return GetLastError();
    }
    int* new_arr = new int[new_size];
    for (int i = 0; i < new_size; i++) {
        if (!ReadFile(hInheritReadPipe, &new_arr[i], sizeof(int), &byteRwad, NULL))
        {
            cout << "New array read is failed " << endl;
            return GetLastError();
        }
    }
    
    cout << "New array: " << endl;
    for (int i = 0; i < new_size; i++) {
        cout << new_arr[i] << " ";
    }

    CloseHandle(hInheritReadPipe);
    CloseHandle(hInheritWritePipe);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    // Release the dynamically allocated memory
    delete[] array;
    delete[] new_arr;

    return 0;
}