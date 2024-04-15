#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono> // for std::chrono_literals

using namespace std;

// Function declarations
void startThreads();
void Thread1();
void Thread2();
void waitForOtherThread(bool& otherThreadWantsToEnter);
void favourThread(int thread);

// Global variables
int favouredThread = 1;
bool thread1WantsToEnter = false;
bool thread2WantsToEnter = false;
bool stopExecution = false;
mutex mtx;
condition_variable cv;

// Function to start threads
void startThreads() {
    thread t1(Thread1);
    thread t2(Thread2);
    this_thread::sleep_for(chrono::seconds(5)); // Run threads for 5 seconds
    stopExecution = true; // Set flag to stop execution
    cv.notify_all(); // Notify threads to stop
    t1.join();
    t2.join();
}

// Function for Thread 1
void Thread1() {
    while (!stopExecution) {
        thread1WantsToEnter = true;

        // Entry section
        waitForOtherThread(thread2WantsToEnter);

        // Critical section
        {
            lock_guard<mutex> lock(mtx);
            cout << "Thread 1: In critical section" << endl;
        }

        // Exit section
        thread1WantsToEnter = false;

        // Remainder section
        favourThread(2);
    }
}

// Function for Thread 2
void Thread2() {
    while (!stopExecution) {
        thread2WantsToEnter = true;

        // Entry section
        waitForOtherThread(thread1WantsToEnter);

        // Critical section
        {
            lock_guard<mutex> lock(mtx);
            cout << "Thread 2: In critical section" << endl;
        }

        // Exit section
        thread2WantsToEnter = false;

        // Remainder section
        favourThread(1);
    }
}

// Function to wait until the other thread wants to enter its critical section
void waitForOtherThread(bool& otherThreadWantsToEnter) {
    unique_lock<mutex> lock(mtx);
    cv.wait(lock, [&]() { return !otherThreadWantsToEnter || stopExecution; });
}

// Function to favour a specific thread
void favourThread(int thread) {
    favouredThread = thread;
    cv.notify_all();
}

int main() {
    startThreads();
    return 0;
}
