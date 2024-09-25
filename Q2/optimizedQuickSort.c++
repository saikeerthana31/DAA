#include <iostream>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <vector>
#include <fstream>

using namespace std;
using namespace std::chrono;

// Evaluation metrics for Normal QuickSort
int cmp_normal = 0, swaps_normal = 0, basic_operations_normal = 0;

// Evaluation metrics for Optimized QuickSort
int cmp_optimized = 0, swaps_optimized = 0, basic_operations_optimized = 0;

// Swap function
void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
    swaps_normal++;
}

// Swap function for optimized version
void swap_optimized(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
    swaps_optimized++;
}

// Normal partition function
int partition(vector<int> &arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        cmp_normal++;
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
            basic_operations_normal++;
        }
    }
    swap(arr[i + 1], arr[high]);
    basic_operations_normal++;
    return i + 1;
}

// Recursive normal QuickSort
void quickSort(vector<int> &arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Insertion Sort for small subarrays in optimized QuickSort
void insertionSort(vector<int> &arr, int low, int high) {
    for (int i = low + 1; i <= high; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= low && arr[j] > key) {
            cmp_optimized++;
            arr[j + 1] = arr[j];
            j--;
            basic_operations_optimized++;
        }
        arr[j + 1] = key;
        basic_operations_optimized++;
    }
}

// Median-of-Three pivot selection for optimized QuickSort
int medianOfThree(vector<int> &arr, int low, int high) {
    int mid = low + (high - low) / 2;
    if (arr[high] < arr[low])
        swap_optimized(arr[low], arr[high]);
    if (arr[mid] < arr[low])
        swap_optimized(arr[low], arr[mid]);
    if (arr[high] < arr[mid])
        swap_optimized(arr[mid], arr[high]);
    return mid;
}

// Partition function for optimized QuickSort
int partition_optimized(vector<int> &arr, int low, int high) {
    int pivotIndex = medianOfThree(arr, low, high);
    int pivot = arr[pivotIndex];
    swap_optimized(arr[pivotIndex], arr[high]); // Move pivot to end
    int i = low - 1;
    for (int j = low; j < high; j++) {
        cmp_optimized++;
        if (arr[j] <= pivot) {
            i++;
            swap_optimized(arr[i], arr[j]);
            basic_operations_optimized++;
        }
    }
    swap_optimized(arr[i + 1], arr[high]); // Move pivot to its final place
    basic_operations_optimized++;
    return i + 1;
}

// Recursive optimized QuickSort
void optimizedQuickSort(vector<int> &arr, int low, int high) {
    while (low < high) {
        if (high - low + 1 < 10) { // Use insertion sort for small subarrays
            insertionSort(arr, low, high);
            break;
        } else {
            int pi = partition_optimized(arr, low, high);
            if (pi - low < high - pi) {
                optimizedQuickSort(arr, low, pi - 1);
                low = pi + 1;
            } else {
                optimizedQuickSort(arr, pi + 1, high);
                high = pi - 1;
            }
        }
    }
}

// Function to generate random numbers for testing
void generateRandomNumbers(vector<int> &arr, int n) {
    arr[0] = 500;
    arr[1] = 1000;

    for (int i = 2; i < n; i++) {
        arr[i] = rand() % (901) + 100;
        basic_operations_normal++;
    }

    std::random_device rd;
    std::mt19937 eng(rd());
    std::shuffle(arr.begin(), arr.end(), eng);
    basic_operations_normal++;
}

// Function to read input from file
void readInputFromFile(vector<int> &arr, const string &filename) {
    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error: Could not open the file!" << endl;
        exit(1);
    }
    int number;
    while (inputFile >> number) {
        arr.push_back(number);
        basic_operations_normal++;
    }
    inputFile.close();
}

// Function to reset the evaluation metrics
void resetMetrics() {
    cmp_normal = swaps_normal = basic_operations_normal = 0;
    cmp_optimized = swaps_optimized = basic_operations_optimized = 0;
}

// Main function
int main() {
    int choice;
    cout << "Choose input method: \n1. Generate Random Numbers (Average case)\n2. Read from Input File (worst_case.txt)\n";
    cin >> choice;

    vector<int> arr;

    if (choice == 1) {
        const int n = 10000; // Size of the array
        arr.resize(n);
        generateRandomNumbers(arr, n);
    } else if (choice == 2) {
        string filename;
        cout << "Enter the input file name: ";
        cin >> filename;
        readInputFromFile(arr, filename);
    } else {
        cout << "Invalid choice!" << endl;
        return 1;
    }

    vector<int> arr_copy = arr; // Copy the array for optimized QuickSort

    // Run Normal QuickSort
    resetMetrics();
    auto start = high_resolution_clock::now();
    quickSort(arr, 0, arr.size() - 1);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    cout << "\nNormal QuickSort Results:" << endl;
    cout << "Comparisons: " << cmp_normal << endl;
    cout << "Swaps: " << swaps_normal << endl;
    cout << "Basic operations: " << basic_operations_normal << endl;
    cout << "Execution time: " << duration.count() << " ms" << endl;
    cout << "Memory usage: " << sizeof(int) * arr.size() << " bytes" << endl;

    // Run Optimized QuickSort
    resetMetrics();
    start = high_resolution_clock::now();
    optimizedQuickSort(arr_copy, 0, arr_copy.size() - 1);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);

    cout << "\nOptimized QuickSort Results:" << endl;
    cout << "Comparisons: " << cmp_optimized << endl;
    cout << "Swaps: " << swaps_optimized << endl;
    cout << "Basic operations: " << basic_operations_optimized << endl;
    cout << "Execution time: " << duration.count() << " ms" << endl;
    cout << "Memory usage: " << sizeof(int) * arr_copy.size() << " bytes" << endl;

    return 0;
}