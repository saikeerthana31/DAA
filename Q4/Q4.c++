#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Utility function to perform BFS for finding islands
void bfsIsland(vector<vector<int> >& grid, int i, int j, vector<vector<bool> >& visited) {
    int m = grid.size(); 
    int n = grid[0].size();
    queue<pair<int, int> > q;
    q.push(make_pair(i, j));
    visited[i][j] = true;

    int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    while (!q.empty()) {
        pair<int, int> cell = q.front();
        int x = cell.first;
        int y = cell.second;
        q.pop();

        for (int d = 0; d < 4; ++d) {
            int nx = x + dirs[d][0];
            int ny = y + dirs[d][1];

            if (nx >= 0 && nx < m && ny >= 0 && ny < n && grid[nx][ny] == 1 && !visited[nx][ny]) {
                visited[nx][ny] = true;
                q.push(make_pair(nx, ny));
            }
        }
    }
}

// Function to find the total number of islands
int numIslands(vector<vector<int> >& grid) {
    int m = grid.size();
    int n = grid[0].size();
    vector<vector<bool> > visited(m, vector<bool>(n, false));
    int count = 0;

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (grid[i][j] == 1 && !visited[i][j]) {
                bfsIsland(grid, i, j, visited);
                count++;
            }
        }
    }
    return count;
}

// Function to check if there is exactly one path from source to destination
bool hasUniquePath(vector<vector<int> >& grid) {
    int m = grid.size();
    int n = grid[0].size();

    if (grid[0][0] == 0 || grid[m-1][n-1] == 0) return false;

    vector<vector<int> > dp(m, vector<int>(n, 0));
    dp[0][0] = 1;

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (grid[i][j] == 0) continue;

            if (i > 0) dp[i][j] += dp[i-1][j];
            if (j > 0) dp[i][j] += dp[i][j-1];

            if (dp[i][j] > 1) return false;
        }
    }

    return dp[m-1][n-1] == 1;
}

// Function to count the number of dead ends
int countDeadEnds(vector<vector<int> >& grid, int k) {
    int m = grid.size();
    int n = grid[0].size();
    int deadEnds = 0;

    int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (grid[i][j] == 1) {
                int pathCount = 0;

                for (int d = 0; d < 4; ++d) {
                    int ni = i + dirs[d][0];
                    int nj = j + dirs[d][1];

                    // Check if the neighbor is within bounds and is a valid path
                    if (ni >= 0 && ni < m && nj >= 0 && nj < n && grid[ni][nj] == 1) {
                        pathCount++;
                    }
                }

                // A dead end is defined as a cell with exactly one valid path
                if (pathCount == 1) {
                    deadEnds++;
                }
            }
        }
    }

    return deadEnds == k;
}

int main() {
    int m, n, k;
    cout << "Enter grid dimensions (m and n): ";
    cin >> m >> n;

    vector<vector<int> > grid(m, vector<int>(n));
    cout << "Enter grid elements (0 or 1):" << endl;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> grid[i][j];
        }
    }

    // Task 1: Count the number of islands
    int islands = numIslands(grid);
    cout << "Number of islands: " << islands << endl;

    // Task 2: Check if there is exactly one path from (1,1) to (m,n)
    bool uniquePath = hasUniquePath(grid);
    cout << "Exactly one path from (1,1) to (" << m << "," << n << "): " << (uniquePath ? "Yes" : "No") << endl;

    // Task 3: Check if there are exactly 'k' dead ends
    cout << "Enter the number of dead ends (k) to check: ";
    cin >> k;

    bool exactDeadEnds = countDeadEnds(grid, k);
    cout << "Exactly " << k << " dead ends: " << (exactDeadEnds ? "Yes" : "No") << endl;

    return 0;
}
