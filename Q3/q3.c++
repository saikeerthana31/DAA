#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

// Topological sort question.

int minNumberOfSemesters(int n, vector<vector<int> >& relations, int k) {
    vector<int> inDegree(n + 1, 0);
    vector<vector<int> > graph(n + 1);

    // Build the graph and compute the in-degree for each course
    for (auto& relation : relations) {
        int u = relation[0], v = relation[1];
        graph[u].push_back(v);
        inDegree[v]++;
    }

    // Initialize the queue with courses that have no prerequisites
    queue<int> q;
    for (int i = 1; i <= n; ++i) {
        if (inDegree[i] == 0) {
            q.push(i);
        }
    }

    int semesters = 0;

    while (!q.empty()) {
        int courseCount = q.size();
        vector<int> availableCourses;

        // Select up to k courses from the queue
        for (int i = 0; i < min(k, courseCount); ++i) {
            int course = q.front();
            q.pop();
            availableCourses.push_back(course);
        }

        // Process the selected courses
        for (int course : availableCourses) {
            for (int nextCourse : graph[course]) {
                inDegree[nextCourse]--;
                if (inDegree[nextCourse] == 0) {
                    q.push(nextCourse);
                }
            }
        }

        // Increment semester count
        semesters++;
    }

    return semesters;
}

int main() {
    int n, k, r;
    
    cout << "Enter the number of courses (n): ";
    cin >> n;
    
    cout << "Enter the number of relations: ";
    cin >> r;
    
    vector<vector<int> > relations(r, vector<int>(2));
    
    cout << "Enter the relations (each relation as two space-separated integers):" << endl;
    for (int i = 0; i < r; ++i) {
        cin >> relations[i][0] >> relations[i][1];
    }
    
    cout << "Enter the maximum number of courses you can take in a semester (k): ";
    cin >> k;

    int result = minNumberOfSemesters(n, relations, k);
    cout << "Minimum number of semesters required: " << result << endl;

    return 0;
}
