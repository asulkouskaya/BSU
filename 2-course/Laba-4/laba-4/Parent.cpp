#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;

int main() {
	HANDLE semaphore = CreateSemaphore(NULL, 5, 5, "semaphore");
	HANDLE eventmasAB[2];
	eventmasAB[0] = CreateEvent(NULL, FALSE, FALSE, "Event_A");
	eventmasAB[1] = CreateEvent(NULL, FALSE, FALSE, "Event_B");

	HANDLE endOfPr = CreateEvent(NULL, FALSE, FALSE, "End");

	if (eventmasAB[0] == NULL) {
		return GetLastError();
	}
	if (eventmasAB[1] == NULL) {
		return GetLastError();
	}
	if (semaphore == NULL) {
		cout << "Create semafor failed." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();
		return GetLastError();
	}
	HANDLE message = CreateEvent(NULL, FALSE, FALSE, "Lovi soobschenie");
	WaitForSingleObject(semaphore, INFINITE);
	cout << "Enter the messege for Child window ('A' or 'B'): ";
	char d;
	cin >> d;
	if (d == 'A') {
		cout << "YOU PRESSED A!!";
		_getch();
		SetEvent(eventmasAB[0]);
		cout << "A";
		d = 'v';
	}
	if (d == 'B') {
		cout << "YOU PRESSED B!!";
		_getch();
		SetEvent(eventmasAB[1]);
		cout << "B";
		d = 'v';
	}
	cout << "Press any key to exit." << endl;
	_getch();
	ReleaseSemaphore(semaphore, 1, NULL);
	SetEvent(endOfPr);
	//CloseHandle(eventmas[0]);
	//CloseHandle(eventmas[1]);
	//CloseHandle(eventmas[2]);
	return 0;
}
