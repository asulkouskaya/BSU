#include <windows.h>
#include <conio.h>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    int size = atoi(argv[0]);
    int* arr = new int[size];
    for (int i = 1; i < size + 1; i++) {
        arr[i - 1] = atoi(argv[i]);
    }
    int k = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] < 0 && abs(arr[i]) % 6 == 0) {
            k++;
            cout << arr[i] << ' ';
        }
        Sleep(5);
    }
    cout << endl << "Count of element which < 0 and % 6 == 0 is " << k << endl;

    cout << "Press any key to continue";
    _getch();
    return 0;
}