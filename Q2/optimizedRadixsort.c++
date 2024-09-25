#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <fstream>

using namespace std;
using namespace std::chrono;

// Evaluation metrics for Base-10 Radix Sort
int cmp_base10 = 0, swaps_base10 = 0, basic_operations_base10 = 0;

// Evaluation metrics for Base-16 Radix Sort
int cmp_base16 = 0, swaps_base16 = 0, basic_operations_base16 = 0;

// Function to get the maximum value in the array
int getMax(const vector<int> &arr) {
    return *max_element(arr.begin(), arr.end());
}

// Function to perform counting sort on the array based on the digit represented by exp (Base-10)
void countingSortBase10(vector<int> &arr, int exp) {
    int n = arr.size();
    vector<int> output(n);
    int count[10] = {0}; // There are 10 possible digits (0-9)

    // Count the occurrences of each digit in the current place value
    for (int i = 0; i < n; i++) {
        count[(arr[i] / exp) % 10]++;
        basic_operations_base10++;
    }

    // Update count[i] so that it contains the actual position of the digit in the output array
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
        basic_operations_base10++;
    }

    // Build the output array by placing elements in their correct position
    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
        basic_operations_base10++;
        swaps_base10++;
    }

    // Copy the sorted elements from the output array back to the original array
    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
        basic_operations_base10++;
    }
}

// Function to perform radix sort on the array (Base-10)
void radixSortBase10(vector<int> &arr) {
    int maxVal = getMax(arr); // Find the maximum value to determine the number of digits

    // Perform counting sort for every digit, starting from the least significant digit (LSD)
    for (int exp = 1; maxVal / exp > 0; exp *= 10) {
        countingSortBase10(arr, exp);
    }
}

// Function to perform counting sort on the array based on the digit represented by exp (Base-16)
void countingSortBase16(vector<int> &arr, int exp) {
    int n = arr.size();
    vector<int> output(n);
    int count[16] = {0}; // There are 16 possible digits (0-F)

    // Count the occurrences of each digit in the current place value
    for (int i = 0; i < n; i++) {
        count[(arr[i] / exp) % 16]++;
        basic_operations_base16++;
    }

    // Update count[i] so that it contains the actual position of the digit in the output array
    for (int i = 1; i < 16; i++) {
        count[i] += count[i - 1];
        basic_operations_base16++;
    }

    // Build the output array by placing elements in their correct position
    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 16] - 1] = arr[i];
        count[(arr[i] / exp) % 16]--;
        basic_operations_base16++;
        swaps_base16++;
    }

    // Copy the sorted elements from the output array back to the original array
    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
        basic_operations_base16++;
    }
}

// Function to perform radix sort on the array (Base-16)
void radixSortBase16(vector<int> &arr) {
    int maxVal = getMax(arr); // Find the maximum value to determine the number of digits

    // Perform counting sort for every digit, starting from the least significant digit (LSD)
    for (int exp = 1; maxVal / exp > 0; exp *= 16) {
        countingSortBase16(arr, exp);
    }
}

// Function to generate random numbers for testing
void generateLargeTestCase(vector<int> &arr, int n, int maxValue) {
    arr.resize(n);
    random_device rd;
    mt19937 eng(rd());
    uniform_int_distribution<> distr(1, maxValue);

    for (int i = 0; i < n; i++) {
        arr[i] = distr(eng);
    }
}

// Function to reset the evaluation metrics
void resetMetrics() {
    cmp_base10 = swaps_base10 = basic_operations_base10 = 0;
    cmp_base16 = swaps_base16 = basic_operations_base16 = 0;
}

// Main function
int main() {
    int choice;
    cout << "Choose input method: \n1. Generate Large Random Numbers (1000000 elements)\n2. Read from Input File\n";
    cin >> choice;

    vector<int> arr;

    if (choice == 1) {
        const int n = 1000000; // 1 million elements
        const int maxValue = 1000000; // Numbers range from 1 to 1,000,000
        generateLargeTestCase(arr, n, maxValue);
    } else if (choice == 2) {
        string filename;
        cout << "Enter the input file name: ";
        cin >> filename;
        ifstream infile(filename);
        int number;
        while (infile >> number) {
            arr.push_back(number);
        }
        infile.close();
    } else {
        cout << "Invalid choice!" << endl;
        return 1;
    }

    vector<int> arr_copy = arr; // Copy the array for Base-16 Radix Sort

    // Run Base-10 Radix Sort
    resetMetrics();
    auto start = high_resolution_clock::now();
    radixSortBase10(arr);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    cout << "\nBase-10 Radix Sort Results:" << endl;
    cout << "Comparisons: " << cmp_base10 << endl;
    cout << "Swaps: " << swaps_base10 << endl;
    cout << "Basic operations: " << basic_operations_base10 << endl;
    cout << "Execution time: " << duration.count() << " ms" << endl;
    cout << "Memory usage: " << sizeof(int) * arr.size() << " bytes" << endl;

    // Run Base-16 Radix Sort
    resetMetrics();
    start = high_resolution_clock::now();
    radixSortBase16(arr_copy);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);

    cout << "\nBase-16 Radix Sort Results:" << endl;
    cout << "Comparisons: " << cmp_base16 << endl;
    cout << "Swaps: " << swaps_base16 << endl;
    cout << "Basic operations: " << basic_operations_base16 << endl;
    cout << "Execution time: " << duration.count() << " ms" << endl;
    cout << "Memory usage: " << sizeof(int) * arr_copy.size() << " bytes" << endl;

    return 0;
}
