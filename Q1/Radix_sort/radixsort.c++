#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <fstream>  // Include fstream for file operations

using namespace std;
using namespace std::chrono;

// Used for evaluation metrics
int cmp = 0, swaps = 0, basic_operations = 0;

// Function to get the maximum value in the array
int getMax(const vector<int> &arr) {
    return *max_element(arr.begin(), arr.end());
}

// Function to perform counting sort on the array based on the digit represented by exp
void countingSort(vector<int> &arr, int exp) {
    int n = arr.size();
    vector<int> output(n);
    int count[10] = {0}; // There are 10 possible digits (0-9)

    // Count the occurrences of each digit in the current place value
    for (int i = 0; i < n; i++) {
        count[(arr[i] / exp) % 10]++;
        basic_operations++; 
    }

    // Update count[i] so that it contains the actual position of the digit in the output array
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
        basic_operations++; 
    }

    // Build the output array by placing elements in their correct position
    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
        basic_operations++; 
        swaps++; 
    }

    // Copy the sorted elements from the output array back to the original array
    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
        basic_operations++; 
    }
}

// Function to perform radix sort on the array
void radixSort(vector<int> &arr) {
    int maxVal = getMax(arr); // Find the maximum value to determine the number of digits

    // Perform counting sort for every digit, starting from the least significant digit (LSD)
    for (int exp = 1; maxVal / exp > 0; exp *= 10) {
        countingSort(arr, exp);
    }
}

// Used for average case input
void generateRandomNumbers(vector<int> &arr, int n) {
    arr.resize(n);
    arr[0] = 500;  
    arr[1] = 1000;

    // Generate random numbers between 100 and 1000 for the rest of the array
    for (int i = 2; i < n; i++) {
        arr[i] = rand() % (901) + 100;
        basic_operations++; 
    }

    // Shuffle the array to randomize the order of elements
    random_device rd;
    mt19937 eng(rd());
    shuffle(arr.begin(), arr.end(), eng);
    basic_operations++; 
}

// Function to read input from a file
void readFromFile(vector<int> &arr, const string &filename) {
    ifstream infile(filename);
    int number;
    arr.clear();  // Clear the array before reading new values
    while (infile >> number) {
        arr.push_back(number);  // Dynamically add numbers to the array
    }
    infile.close();
}

// Main
int main() {
    vector<int> arr;
    int choice;
    cout << "Choose input method:\n";
    cout << "1. Generate random numbers\n";
    cout << "2. Read from a text file\n";
    cout << "Enter your choice (1 or 2): ";
    cin >> choice;

    if (choice == 1) {
        const int n = 10; 
        generateRandomNumbers(arr, n);
    } else if (choice == 2) {
        string filename;
        cout << "Enter the filename: ";
        cin >> filename;
        readFromFile(arr, filename);

        if (arr.empty()) {
            cout << "File is empty or not found. Exiting." << endl;
            return 1;
        }
    } else {
        cout << "Invalid choice!" << endl;
        return 1;
    }

    // Display the generated array
    cout << "Input array is: ";
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    // Measure the execution time of radix sort
    auto start = high_resolution_clock::now();
    radixSort(arr);
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(end - start);

    // Display the sorted array and evaluation metrics
    cout << "Sorted array: ";
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    cout << "Comparisons: " << cmp << endl; // Note: Radix Sort doesn't perform explicit comparisons
    cout << "Swaps: " << swaps << endl;
    cout << "Basic operations: " << basic_operations << endl;
    cout << "Execution time: " << duration.count() << " ms" << endl;
    cout << "Memory usage: " << sizeof(int) * arr.size() + sizeof(int) * 10 + sizeof(int) * arr.size() << " bytes" << endl;

    return 0;
}