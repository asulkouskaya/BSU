#include <windows.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
	HANDLE eventmasAB[2];
	eventmasAB[0] = CreateEvent(NULL, FALSE, FALSE, "Event_A");
	eventmasAB[1] = CreateEvent(NULL, FALSE, FALSE, "Event_B");
	HANDLE eventmasCD[2];
	eventmasCD[0] = CreateEvent(NULL, FALSE, FALSE, "Event_C");
	eventmasCD[1] = CreateEvent(NULL, FALSE, FALSE, "Event_D");

	HANDLE endOfPr = CreateEvent(NULL, FALSE, FALSE, "End");

	HANDLE semaphore = CreateSemaphore(NULL, 5, 5, "semaphore");
	//HANDLE sem = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, (LPCWSTR)"semaphore");
	if (semaphore == NULL)
	{
		cout << "kjshdf";
	}
	if (eventmasAB[0] == NULL) {
		return GetLastError();
	}
	if (eventmasAB[1] == NULL) {
		return GetLastError();
	}
	if (eventmasCD[0] == NULL) {
		return GetLastError();
	}
	if (eventmasCD[1] == NULL) {
		return GetLastError();
	}

	int parent, child;
	std::cout << "Enter count of parent process: ";
	std::cin >> parent;
	std::cout << "Enter count (<= 4) of child process: ";
	std::cin >> child;

	STARTUPINFO* si = new STARTUPINFO[parent];
	PROCESS_INFORMATION* piApp = new PROCESS_INFORMATION[parent];
	//HANDLE* a = new HANDLE[parent];
	STARTUPINFO* si1 = new STARTUPINFO[child];
	PROCESS_INFORMATION* piApp1 = new PROCESS_INFORMATION[child];
	//HANDLE* a1 = new HANDLE[child];
	for (int i = 0; i < parent; i++) {
		ZeroMemory(&si[i], sizeof(STARTUPINFO));
		si[i].cb = sizeof(STARTUPINFO);
		if (!CreateProcessA((LPCSTR)"F:\\BSU-FAMCs\\2-course\\OS\\laba-4\\Debug\\Parent.exe", nullptr, nullptr, nullptr, FALSE, CREATE_NEW_CONSOLE, nullptr, nullptr, (LPSTARTUPINFOA) & (si[i]), &(piApp[i]))) {
			cout << "The new Parent process is not created.\n";
			return 0;
		}
		//a[i] = piApp[i].hProcess;
		Sleep(100);
	}
	for (int i = 0; i < child; i++)
	{
		ZeroMemory(&si1[i], sizeof(STARTUPINFO));
		si1[i].cb = sizeof(STARTUPINFO);
		if (!CreateProcessA((LPCSTR)"F:\\BSU-FAMCs\\2-course\\OS\\laba-4\\Debug\\Child.exe", nullptr, nullptr, nullptr, FALSE, CREATE_NEW_CONSOLE, nullptr, nullptr, (LPSTARTUPINFOA) & (si1[i]), &(piApp1[i]))) {
			cout << "The new Child process is not created.\n";
			return 0;
		}
		//a1[i] = piApp1[i].hProcess;
		Sleep(100);
	}
	DWORD x;
	for (int i = 0; i < parent; ) {
		WaitForSingleObject(endOfPr, INFINITE);
		cout << "Blablabla" << endl;
		_getch();
		
		x = WaitForMultipleObjects(2, eventmasAB, false, INFINITE);
			cout << "WaitForMultipleObjects is " << (int)(x - WAIT_OBJECT_0) << endl;
			if (x == WAIT_OBJECT_0) {
				i++;
				cout << "Parent process shows massege A" << endl;
				SetEvent(eventmasCD[0]);

			}
			if (x == WAIT_OBJECT_0 + 1) {
				i++;
				cout << "Parent process shows massege B" << endl;
				SetEvent(eventmasCD[1]);
			}
	}
	return 0;
}