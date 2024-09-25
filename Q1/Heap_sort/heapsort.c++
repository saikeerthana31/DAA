#include <iostream>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <random>

using namespace std;

int cmp = 0, swaps = 0, basic_operations = 0;

// Function to heapify a subtree rooted with node i
void heapify(int arr[], int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    cmp++;
    if (l < n && arr[l] > arr[largest])
        largest = l;

    cmp++;
    if (r < n && arr[r] > arr[largest])
        largest = r;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        swaps++;
        heapify(arr, n, largest);
    }
    basic_operations++;
}

// Function to perform heap sort
void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i >= 0; i--) {
        swap(arr[0], arr[i]);
        swaps++;
        heapify(arr, i, 0);
    }
    basic_operations++;
}

// Function to generate random numbers ensuring 500 and 1000 are included
void generateRandomNumbers(int arr[], int n) {
    arr[0] = 500;
    arr[1] = 1000;

    for (int i = 2; i < n; i++) {
        arr[i] = rand() % (901) + 100;
        basic_operations++;
    }

    std::random_device rd;
    std::mt19937 eng(rd());
    std::shuffle(arr, arr + n, eng);
    basic_operations++;
}

// Function to print the array
void printArray(int arr[], int n) {
    for (int i = 0; i < n; ++i)
        cout << arr[i] << " ";
    cout << "\n";
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    const int n = 10;
    int* arr = new int[n];

    generateRandomNumbers(arr, n);

    cout << "Generated array is: ";
    printArray(arr, n);

    auto start = chrono::high_resolution_clock::now();
    
    heapSort(arr, n);
    
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> execution_time = end - start;

    cout << "Sorted array: ";
    printArray(arr, n);

    cout << "Comparisons: " << cmp << endl;
    cout << "Swaps: " << swaps << endl;
    cout << "Basic operations: " << basic_operations << endl;
    cout << "Execution time: " << execution_time.count() << " milliseconds" << endl;

    cout << "Memory usage: " << sizeof(int) * n << " bytes" << endl;

    delete[] arr;

    return 0;
}
