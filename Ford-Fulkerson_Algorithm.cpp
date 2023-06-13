#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <ctime>

using namespace std;

// Structure to represent an edge in the network
struct Edge {
    int source, destination, capacity, flow;
};

// Function to perform Breadth-First Search (BFS) on the residual graph
bool bfs(const vector<vector<int>>& residualGraph, int source, int sink, vector<int>& parent) {
    int numVertices = residualGraph.size();
    vector<bool> visited(numVertices, false);
    queue<int> q;

    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < numVertices; v++) {
            if (!visited[v] && residualGraph[u][v] > 0) {
                q.push(v);
                visited[v] = true;
                parent[v] = u;
            }
        }
    }

    return visited[sink];
}

// Function to calculate the maximum flow using the Ford-Fulkerson algorithm
int fordFulkerson(vector<vector<int>>& graph, int source, int sink) {
    int numVertices = graph.size();
    vector<vector<int>> residualGraph(numVertices, vector<int>(numVertices, 0));

    // Initialize the residual graph with the capacities of the original graph
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            residualGraph[i][j] = graph[i][j];
        }
    }

    // Initialize the parent array for the augmenting path
    vector<int> parent(numVertices, -1);

    // Initialize the maximum flow
    int maxFlow = 0;

    // Augment the flow while there is a path from source to sink
    while (bfs(residualGraph, source, sink, parent)) {
        // Find the bottleneck capacity along the augmenting path
        int minCapacity = numeric_limits<int>::max();
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            minCapacity = min(minCapacity, residualGraph[u][v]);
        }

        // Update the residual capacities and reverse edges along the augmenting path
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            residualGraph[u][v] -= minCapacity;
            residualGraph[v][u] += minCapacity;
        }

        // Add the bottleneck capacity to the maximum flow
        maxFlow += minCapacity;
    }

    return maxFlow;
}

// Function to calculate the runtime of a function
template<typename Func, typename... Args>
double calculateRuntime(Func func, Args&&... args) {
    clock_t startTime = clock();
    func(std::forward<Args>(args)...);
    clock_t endTime = clock();

    return static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC;
}

int main() {
    // Example usage
    int numVertices = 6;
    vector<vector<int>> graph(numVertices, vector<int>(numVertices, 0));

    // Create the graph with capacities of the edges
    graph[0][1] = 10;
    graph[0][2] = 10;
    graph[1][2] = 2;
    graph[1][3] = 4;
    graph[1][4] = 8;
    graph[2][4] = 9;
    graph[4][3] = 6;
    graph[3][5] = 10;
    graph[4][5] = 10;

    int source = 0;
    int sink = 5;

    // Calculate the maximum flow using the Ford-Fulkerson algorithm and measure the runtime
    double runtime = calculateRuntime(fordFulkerson, graph, source, sink);

    // Output the maximum flow and runtime
    cout << "Maximum Flow: " << fordFulkerson(graph, source, sink) << endl;
    cout << "Runtime: " << runtime << " seconds" << endl;

    return 0;
}
