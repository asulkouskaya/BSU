#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {

    HANDLE small_event;
    small_event = CreateEvent(NULL, FALSE, FALSE, "small_event");
    HANDLE hReadPipe, hWritePipe;
    int k;
    sscanf(argv[0], "%d", &hWritePipe);
    sscanf(argv[1], "%d", &hReadPipe);
    sscanf(argv[2], "%d", &k);

    WaitForSingleObject(small_event, INFINITY);
    LPDWORD dwBytesWrite = 0, dwBytesRead = 0;
    int arl;

    if (!ReadFile(hReadPipe, &arl, sizeof(int), dwBytesRead, NULL)) {
        cout << "array  read is failed " << endl;
        return GetLastError();
    }

    int* a = new int[arl];
    for (int i = 0; i < arl; i++) {
        if (!ReadFile(hReadPipe, &a[i], sizeof(int), dwBytesRead, NULL))
        {
            cout << "array  read is failed " << endl;
            return GetLastError();
        }
    }
    
    
    int new_size = 0;
    vector<int> res;

    for (int i = 0; i < arl; i++) {
        if (a[i] > 0 && a[i] < k) {
            res.push_back(a[i]);
            new_size++;
        }
    }


    if (!WriteFile(hWritePipe, &new_size, sizeof(int), dwBytesWrite, NULL)) {
        cout << "Write of new size failed\n";
        return GetLastError();
    }

    for (int i = 0; i < new_size; i++) {
        if (!WriteFile(hWritePipe, &res[i], sizeof(int), dwBytesWrite, NULL)) {
            cout << "Write of array failed\n";
            return GetLastError();
        }
    }

    cout << "New array: " << endl;
    for (int i = 0; i < new_size; i++) {
        cout << a[i] << " ";
    }

    _cprintf("\nTo exit press any key ");
    char t = _getch();

    return 0;
}