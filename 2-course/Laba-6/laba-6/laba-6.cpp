#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <future>
#include <vector>
#include <cstdlib>
#include <ctime>

const int MAX_SIZE = 100;

int array[MAX_SIZE];
int newArray[MAX_SIZE];
int arraySize;
int interval;
int X;
int count = 0;
std::condition_variable_any cv1, cv2;
std::mutex mutex;

void WorkThread() {
    std::unique_lock<std::mutex> lock(mutex);
    cv1.wait(lock);

    int negativeMultipleOfThree[MAX_SIZE];
    int positiveElements[MAX_SIZE];
    int negCount = 0;
    int posCount = 0;

    for (int i = 0; i < arraySize; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));

        if (array[i] < 0 && array[i] % 3 == 0) {
            negativeMultipleOfThree[negCount++] = array[i];
        }
        else {
            positiveElements[posCount++] = array[i];
        }
    }

    for (int i = 0; i < negCount; ++i) {
        newArray[i] = negativeMultipleOfThree[i];
    }

    for (int i = 0; i < posCount; ++i) {
        newArray[negCount + i] = positiveElements[i];
    }

    cv2.notify_one();
}

void CountElementThread() {
    std::unique_lock<std::mutex> lock(mutex);
    cv1.wait(lock);

    for (int i = 0; i < arraySize; ++i) {
        if (array[i] >= 0 && array[i] == X) {
            ++count;
        }
    }

  //  cv2.notify_one();
}

int main() {
    std::vector<std::thread> threads;

    std::cout << "Enter array size: ";
    std::cin >> arraySize;

    srand(static_cast<unsigned int>(time(0)));

    std::cout << "Enter elements of array:" << std::endl;
    for (int i = 0; i < arraySize; ++i) {
        std::cin >> array[i];
    }
    std::cout << std::endl;

    std::cout << "Enter the interval for element processing (in milliseconds): ";
    std::cin >> interval;

    std::thread workThread(WorkThread);
    std::thread countThread(CountElementThread);

    std::cout << "Enter number X: ";
    std::cin >> X;

    cv1.notify_all();

    {
        std::unique_lock<std::mutex> lock(mutex);
        cv2.wait(lock);
    }

    std::cout << "Result from work thread: ";
    for (int i = 0; i < arraySize; ++i) {
        std::cout << newArray[i] << " ";
    }
    std::cout << std::endl;


    std::cout << "Number of elements which is equal to X: " << count << std::endl;


    workThread.join();
    countThread.join();

    return 0;
}
