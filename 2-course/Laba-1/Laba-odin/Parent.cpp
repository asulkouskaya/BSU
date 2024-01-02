#include <Windows.h>
#include <process.h>
#include <iostream>
using namespace std;

struct Data {
	int count_;
	int* arr_;
	int size_;

	Data(int size) {
		size_ = size;
		arr_ = new int[size];
		cout << "Enter elements of array: ";
		for (int i = 0; i < size; i++) {
			cin >> arr_[i];
		}
		count_ = 0;
	}
};


void WINAPI task1(LPVOID data) {
	for (int i = 0; i < ((Data*)data)->size_; i++) {
		if (((Data*)data)->arr_[i] % 9 == 0) {	
			((Data*)data)->count_++;
		}
	}
	cout << "Count of elements in array which gives % 9 == 0 is " << ((Data*)data)->count_ << endl;

}

unsigned __stdcall task2(void* data) {
	for (int i = 0; i < ((Data*)data)->size_; i++) {
		if (((Data*)data)->arr_[i] % 9 == 0) {
			((Data*)data)->count_++;
		}
	}
	cout << "Count of elements in array which gives % 9 == 0 is " << ((Data*)data)->count_ << endl;

	return ((Data*)data)->count_;
}

int main() {
	HANDLE workerThread;
	DWORD IDWorkerThread;

	cout << "Enter size of array: ";
	int size; cin >> size;

	auto param = new Data(size);
	int time;
	cout << "Enter stop time (s): ";
	cin >> time;

	// workerThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)task1, (LPVOID)param, 0, &IDWorkerThread);
	workerThread = (HANDLE)_beginthreadex(NULL, 0, &task2, (void*)param, 0, (unsigned int*)&IDWorkerThread);

	if (workerThread == NULL) {
		return GetLastError();
	}
	SuspendThread(workerThread); // приостановить
	Sleep(time * 1000); // подождать
	ResumeThread(workerThread); // возобновить
	if (WaitForSingleObject(workerThread, INFINITE) != WAIT_OBJECT_0)
	{
		cout << "Wait for single object failed." << endl;
	}
	else {
		cout << "Result: " << param->count_ << endl;
	}
	CloseHandle(workerThread);
	return 0;
}