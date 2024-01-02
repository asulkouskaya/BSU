#include <iostream>
#include <windows.h>
#include <vector>
#include <process.h>
#include <ctime>
#include <algorithm>

using namespace std;

CRITICAL_SECTION cs2;

HANDLE hWorkEvent;
HANDLE hWorkCout;
HANDLE count_element;
HANDLE work;

int t;
double x;
int result;
int n;
int* res;

DWORD WINAPI Work(LPVOID lpParam) {
	WaitForSingleObject(hWorkEvent, INFINITE);
	int beg = 0, end = n - 1;
	res = new int[n];
	for (int i = 0; i < n; i++) {
		if (((vector<int>*)lpParam)->at(i) < 0 && abs(((vector<int>*)lpParam)->at(i)) % 3 == 0) {
			res[beg] = ((vector<int>*)lpParam)->at(i);
			beg++;
		} else {
			res[end] = ((vector<int>*)lpParam)->at(i);
			end--;
		}
	}
	SetEvent(hWorkCout);
	LeaveCriticalSection(&cs2);
	return 0;
}

DWORD WINAPI CountElement(LPVOID lpParam) {
	EnterCriticalSection(&cs2);
	WaitForSingleObject(hWorkEvent, INFINITE);
	int count = 0;
	for (int i = 0; i < n; i++) {
		if (((vector<int>*)lpParam)->at(i) > 0 && ((vector<int>*)lpParam)->at(i) == x) {
			count++;
		}
	}
	result = count;
	LeaveCriticalSection(&cs2);
	return 0;
}

int main() {
	hWorkEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	hWorkCout = CreateEvent(NULL, FALSE, FALSE, NULL);
	InitializeCriticalSection(&cs2);
	if (hWorkEvent == NULL) {
		return GetLastError();
	}
	cout << "Enter size of array: ";
	cin >> n;

	vector<int> arr;
	int elem;
	cout << "Enter elements of array: ";
	for (int i = 0; i < n; i++) {
		cin >> elem; 
		arr.push_back(elem);
	}
	cout << "Enter time to sleep after processing one element (s): ";
	cin >> t;
	
	work = CreateThread(NULL, 0, Work, &arr, 0, NULL);
	count_element = CreateThread(NULL, 0, CountElement, &arr, 0, NULL);


	cout << "Enter element for searching: ";
	cin >> x;
	
	SetEvent(hWorkEvent);
	WaitForSingleObject(hWorkCout, INFINITE);
	for (int i = 0; i < n; i++) {
		cout << res[i] << " ";
		Sleep(t * 1000);
	}
	cout << endl;
	EnterCriticalSection(&cs2);
	cout << "Count of elements equals to X: " << result << '\n';
	LeaveCriticalSection(&cs2);
	DeleteCriticalSection(&cs2);
	return 0;
}