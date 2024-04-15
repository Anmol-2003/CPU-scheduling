#include <iostream>
using namespace std;

int main()
{
    // Number of processes and resources
    int numProcesses = 5;
    int numResources = 3;

    // Allocation, Maximum, and Available matrices
    int allocation[5][3] = { {0, 1, 0},
                             {2, 0, 0},
                             {3, 0, 2},
                             {2, 1, 1},
                             {0, 0, 2} };

    int maximum[5][3] = { {7, 5, 3},
                          {3, 2, 2},
                          {9, 0, 2},
                          {2, 2, 2},
                          {4, 3, 3} };

    int available[3] = {3, 3, 2};

    // Arrays to track process status and safe sequence
    int finish[numProcesses] = {0};
    int safeSequence[numProcesses], safeCount = 0;

    // Calculate the need matrix
    int need[numProcesses][numResources];
    for (int i = 0; i < numProcesses; ++i) {
        for (int j = 0; j < numResources; ++j) {
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }

    // Iterate through processes to find safe sequence
    for (int count = 0; count < numProcesses; ++count) {
        for (int i = 0; i < numProcesses; ++i) {
            if (!finish[i]) {
                bool canExecute = true;
                for (int j = 0; j < numResources; ++j) {
                    if (need[i][j] > available[j]) {
                        canExecute = false;
                        break;
                    }
                }
                if (canExecute) {
                    safeSequence[safeCount++] = i;
                    for (int j = 0; j < numResources; ++j) {
                        available[j] += allocation[i][j];
                    }
                    finish[i] = 1;
                }
            }
        }
    }

    // Check if the safe sequence is found
    bool isSafe = true;
    for (int i = 0; i < numProcesses; ++i) {
        if (!finish[i]) {
            isSafe = false;
            break;
        }
    }

    // Display the safe sequence if found, else indicate that no safe sequence exists
    if (isSafe) {
        cout << "Safe Sequence:";
        for (int i = 0; i < safeCount; ++i) {
            cout << " P" << safeSequence[i];
            if (i != safeCount - 1) {
                cout << " ->";
            }
        }
        cout << endl;
    } else {
        cout << "No safe sequence exists." << endl;
    }

    return 0;
}
