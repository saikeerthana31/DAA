#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <fstream>
#include <sstream>  
#include <string> 

using namespace std;
using namespace std::chrono;

// Used for evaluation metrics
int cmp = 0, swaps = 0, basic_operations = 0;

// Structure to represent an element in the priority queue
struct Element {
    int value;           
    int arrayIndex;      
    int elementIndex;    

    // Overloading the > operator to use in the min-heap (priority queue)
    bool operator>(const Element &other) const {
        cmp++; 
        return value > other.value;
    }
};

// Function to perform k-way merge sort
vector<int> kWayMergeSort(const vector<vector<int>> &arrays, int k) {
    priority_queue<Element, vector<Element>, greater<Element>> minHeap;
    vector<int> mergedArray;

    // Initialize the min-heap with the first element of each array
    for (int i = 0; i < k; i++) {
        if (!arrays[i].empty()) {
            minHeap.push(Element{arrays[i][0], i, 0});
            basic_operations++; 
        }
    }

    // Extract the smallest element from the min-heap and push the next element from the same array
    while (!minHeap.empty()) {
        Element current = minHeap.top();
        minHeap.pop();

        mergedArray.push_back(current.value);
        swaps++; 
        basic_operations++; 

        if (current.elementIndex + 1 < arrays[current.arrayIndex].size()) {
            minHeap.push(Element{
                arrays[current.arrayIndex][current.elementIndex + 1],
                current.arrayIndex,
                current.elementIndex + 1});
            basic_operations++; 
        }
    }

    return mergedArray;
}

// Function to generate random arrays for k-way merge sort, used for 
void generateRandomArrays(vector<vector<int>> &arrays, int k, int n) {
    std::random_device rd;
    std::mt19937 eng(rd());

    // Generate k sorted arrays with n random elements each
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < n; j++) {
            int num = rand() % 901 + 100; // Random number between 100 and 1000
            arrays[i].push_back(num);
            basic_operations++; // Count the basic operation
        }
        std::sort(arrays[i].begin(), arrays[i].end()); // Ensure each array is sorted
    }
    
    // Shuffle the arrays slightly to randomize the sequence of numbers across arrays
    for (int i = 0; i < k; i++) {
        std::shuffle(arrays[i].begin(), arrays[i].end(), eng);
    }
}

// Function to read input arrays from a file
void readInputFromFile(vector<vector<int>> &arrays, int &k, const string &filename) {
    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error: Could not open the file!" << endl;
        exit(1);
    }

    string line;
    k = 0; // Initialize the number of arrays

    // Read arrays from the file line by line
    while (getline(inputFile, line)) {
        vector<int> array;
        int num;
        stringstream ss(line);
        while (ss >> num) {
            array.push_back(num);
            basic_operations++; // Count the basic operation
        }
        arrays.push_back(array);
        k++;
    }

    inputFile.close();
}

int main() {
    srand(static_cast<unsigned>(time(0))); // Seed for random number generation

    int k;  // Number of arrays
    int n = 5;  // Number of elements in each array (default if generating random arrays)

    vector<vector<int>> arr;

    int choice;
    cout << "Choose input method: \n1. Generate Random Numbers\n2. Read from Input File\n";
    cin >> choice;

    // Choose to generate random arrays or read arrays from a file
    if (choice == 1) {
        cout << "Enter the number of arrays (k): ";
        cin >> k;
        arr.resize(k);
        generateRandomArrays(arr, k, n);
    } else if (choice == 2) {
        string filename;
        cout << "Enter the input file name: ";
        cin >> filename;
        readInputFromFile(arr, k, filename);
    } else {
        cout << "Invalid choice!" << endl;
        return 1;
    }

    // Display the generated or read arrays
    cout << "Generated arrays:" << endl;
    for (int i = 0; i < k; i++) {
        for (int value : arr[i]) {
            cout << value << " ";
        }
        cout << endl;
    }

    // Measure the execution time of k-way merge sort
    auto start = high_resolution_clock::now();
    vector<int> result = kWayMergeSort(arr, k);
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(end - start);

    // Display the merged array and evaluation metrics
    cout << "Merged array: ";
    for (int value : result) {
        cout << value << " ";
    }
    cout << endl;
    
    cout << "Comparisons: " << cmp << endl;
    cout << "Swaps: " << swaps << endl;
    cout << "Basic operations: " << basic_operations << endl;
    cout << "Execution time: " << duration.count() << " ms" << endl;
    cout << "Memory usage: " << sizeof(int) * result.size() + sizeof(Element) * k << " bytes" << endl;

    return 0;
}
