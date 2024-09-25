#include <iostream>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <fstream>

using namespace std;

// used for assessing the algorithim
int cmp = 0, swaps = 0, basic_operations = 0;

// Finds the maximum number in the array and calculates the number of digits in this maximum value.
int findMax(int arr[], int n) {
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        cmp++;
        if (arr[i] > max)
            max = arr[i];
    }
    int cnt = 0;
    while (max > 0) {
        cnt++;
        max = max / 10;
        basic_operations++;
    }
    return cnt;
}

/*
The bucket sort algorithm, which sorts the array by distributing elements 
into buckets based on their digit values, then recombining them.
*/

void bucketSort(int arr[], int* bucket[], int n) {
    static int j[10], k, l, d = 1;

    int c = findMax(arr, n);

    for (int m = 0; m < c; m++) {
        for (int i = 0; i < 10; i++) {
            j[i] = 0;
            basic_operations++;
        }
        for (int i = 0; i < n; i++) {
            k = (arr[i] / d) % 10;
            bucket[k][j[k]] = arr[i];
            j[k]++;
            basic_operations++;
        }
        l = 0;
        for (int i = 0; i < 10; i++) {
            for (k = 0; k < j[i]; k++) {
                arr[l] = bucket[i][k];
                l++;
                swaps++;
                basic_operations++;
            }
        }
        d *= 10;
    }
}


// Used for input of average case
void generateRandomNumbers(int arr[], int n) {
    // enusres that the values 500 and 1000 are included in the random numbers
    arr[0] = 500; 
    arr[1] = 1000; 

    for (int i = 2; i < n; i++) {
        arr[i] = rand() % (901) + 100; // Random number between 100 and 1000
        basic_operations++;
    }

    std::random_device rd; // Obtain a random number from hardware
    std::mt19937 eng(rd()); // Seed the generator
    std::shuffle(arr, arr + n, eng); // Shuffle the array
    basic_operations++;
}

// Used for input of best/worst case files
void readInputFromFile(int arr[], int &n, const string &filename) {
    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error: Could not open the file!" << endl;
        exit(1);
    }
    int i = 0;
    while (inputFile >> arr[i]) {
        i++;
        basic_operations++;
    }
    n = i; // Set n to the number of elements read
    inputFile.close();
}

int main() {
    srand(static_cast<unsigned>(time(0))); // Seed for random number generation

    int choice;
    cout << "Choose input method: \n1. Generate Random Numbers\n2. Read from Input File(best_case_bucket_input.txt or worst_case_bucket_input.txt) \n";
    cin >> choice;

    int n = 10; // Default size, can be updated if reading from a file
    int* arr = new int[n];
    int* bucket[10];

    // Allocate buckets
    for (int i = 0; i < 10; i++) {
        bucket[i] = new int[n];
        basic_operations++;
    }

    if (choice == 1) {
        generateRandomNumbers(arr, n);
    } else if (choice == 2) {
        string filename;
        cout << "Enter the input file name: ";
        cin >> filename;
        readInputFromFile(arr, n, filename);
    } else {
        cout << "Invalid choice!" << endl;
        return 1;
    }

    cout << "Generated array is: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    // Measure execution time
    auto start = chrono::high_resolution_clock::now();
    bucketSort(arr, bucket, n);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> execution_time = end - start;

    cout << "Sorted array: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    // Output metrics
    cout << "Comparisons: " << cmp << endl;
    cout << "Swaps: " << swaps << endl;
    cout << "Basic operations: " << basic_operations << endl;
    cout << "Execution time: " << execution_time.count() << " milliseconds" << endl;
    cout << "Memory usage: " << sizeof(int) * n + sizeof(int*) * 10 << " bytes" << endl;

    // Free allocated memory
    delete[] arr;
    for (int i = 0; i < 10; i++)
        delete[] bucket[i];

    return 0;
}
