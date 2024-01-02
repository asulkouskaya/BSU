#include <iostream>
#include <windows.h>
#include <conio.h>

CRITICAL_SECTION A;
using namespace std;

int main() {
	HANDLE semaphore = CreateSemaphore(NULL, 5, 5, "semaphore");
	if (semaphore == NULL) {
		cout << "Create semafor failed." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();
		return GetLastError();
	}
	HANDLE eventmasCD[2];
	eventmasCD[0] = CreateEvent(NULL, FALSE, FALSE, "Event_C");
	eventmasCD[1] = CreateEvent(NULL, FALSE, FALSE, "Event_D");

	HANDLE endOfPr = CreateEvent(NULL, FALSE, FALSE, "End");

	if (eventmasCD[0] == NULL) {
		return GetLastError();
	}
	if (eventmasCD[1] == NULL) {
		return GetLastError();
	}
	cout << "Press any key to CONTINUE." << endl;
	_getch();

	WaitForSingleObject(semaphore, INFINITE);
	while (true) {
		DWORD y = WaitForMultipleObjects(2, eventmasCD, false, INFINITE);
			cout << "WaitForMultipleObjects is " << (int)(y - WAIT_OBJECT_0) << endl;
			_getch();
			if (y == WAIT_OBJECT_0) {
				cout << "Boss process shows massege C" << endl;
				_getch();
				break;
			}
			if (y == WAIT_OBJECT_0 + 1) {
				cout << "Boss process shows massege D" << endl;
				_getch();
				break;
			}
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
