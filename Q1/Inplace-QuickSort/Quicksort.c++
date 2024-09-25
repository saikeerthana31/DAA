#include <iostream>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <vector>
#include <fstream>

using namespace std;
using namespace std::chrono;

// Used for evaluation metrics
int cmp = 0, swaps = 0, basic_operations = 0;

// helper fucntion
void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
    swaps++;
}

// Splits the array based on a piviot point
int partition(vector<int> &arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        cmp++;
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
            basic_operations++;
        }
    }
    swap(arr[i + 1], arr[high]);
    basic_operations++;
    return i + 1;
}
// Recursive inplace-Quicksort
void quickSort(vector<int> &arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Used for input of average case 
void generateRandomNumbers(vector<int> &arr, int n) {
    arr[0] = 500;
    arr[1] = 1000;

    for (int i = 2; i < n; i++) {
        arr[i] = rand() % (901) + 100;
        basic_operations++;
    }

    std::random_device rd;
    std::mt19937 eng(rd());
    std::shuffle(arr.begin(), arr.end(), eng);
    basic_operations++;
}

// Used for input of best/worst case files
void readInputFromFile(vector<int> &arr, const string &filename) {
    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error: Could not open the file!" << endl;
        exit(1);
    }
    int number;
    while (inputFile >> number) {
        arr.push_back(number);
        basic_operations++;
    }
    inputFile.close();
}
// Main
int main() {
    int choice;
    cout << "Choose input method: \n1. Generate Random Numbers(Average case)\n2. Read from Input File (QuickSort_worst_case_input.txt or QuickSort_best_case_input.txt)\n";
    cin >> choice;

    vector<int> arr;

    if (choice == 1) {
        const int n = 10; // Size of the array
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

    cout << "Unsorted array: ";
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    auto start = high_resolution_clock::now();
    quickSort(arr, 0, arr.size() - 1);
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Sorted array: ";
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    cout << "Comparisons: " << cmp << endl;
    cout << "Swaps: " << swaps << endl;
    cout << "Basic operations: " << basic_operations << endl;
    cout << "Execution time: " << duration.count() << " ms" << endl;
    cout << "Memory usage: " << sizeof(int) * arr.size() << " bytes" << endl;

    return 0;
}
