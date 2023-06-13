#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <chrono>
using namespace std;
using namespace std::chrono;

// Structure to represent an edge in the graph
struct Edge {
    int destination;
    int capacity;
    int flow;
    int reverseEdge;

    Edge(int d, int c)
    {
        destination = d;
        capacity = c;
        flow = 0;
        reverseEdge = -1;
    }
};

// Class to represent the graph
class Graph {
    int numVertices;
    vector<vector<Edge>> adjacencyList;
    vector<int> level;

public:
    Graph(int V)
    {
        numVertices = V;
        adjacencyList.resize(numVertices);
        level.resize(numVertices);
    }

    // Function to add an edge to the graph
    void addEdge(int source, int destination, int capacity)
    {
        adjacencyList[source].push_back(Edge(destination, capacity));
        adjacencyList[destination].push_back(Edge(source, 0));

        int sourceSize = adjacencyList[source].size();
        int destinationSize = adjacencyList[destination].size();

        adjacencyList[source][sourceSize - 1].reverseEdge = destinationSize - 1;
        adjacencyList[destination][destinationSize - 1].reverseEdge = sourceSize - 1;
    }

    // Function to perform Breadth-First Search to compute levels of nodes
    bool bfs(int source, int sink)
    {
        fill(level.begin(), level.end(), -1);
        level[source] = 0;

        queue<int> q;
        q.push(source);

        while (!q.empty()) {
            int currentVertex = q.front();
            q.pop();

            for (const Edge& edge : adjacencyList[currentVertex]) {
                if (level[edge.destination] < 0 && edge.flow < edge.capacity) {
                    level[edge.destination] = level[currentVertex] + 1;
                    q.push(edge.destination);
                }
            }
        }

        return level[sink] >= 0;
    }

    // Function to perform Depth-First Search to find augmenting paths
    int dfs(int currentVertex, int sink, int currentFlow)
    {
        if (currentVertex == sink)
            return currentFlow;

        for (Edge& edge : adjacencyList[currentVertex]) {
            if (level[edge.destination] == level[currentVertex] + 1 && edge.flow < edge.capacity) {
                int minFlow = min(currentFlow, edge.capacity - edge.flow);
                int bottleneckFlow = dfs(edge.destination, sink, minFlow);

                if (bottleneckFlow > 0) {
                    edge.flow += bottleneckFlow;
                    adjacencyList[edge.destination][edge.reverseEdge].flow -= bottleneckFlow;
                    return bottleneckFlow;
                }
            }
        }

        return 0;
    }

    // Function to compute maximum flow using Dinic's algorithm
    int dinicMaxFlow(int source, int sink)
    {
        int maxFlow = 0;

        high_resolution_clock::time_point startTime = high_resolution_clock::now();

        while (bfs(source, sink)) {
            int currentFlow;
            while ((currentFlow = dfs(source, sink, numeric_limits<int>::max())) > 0) {
                maxFlow += currentFlow;
            }
        }

        high_resolution_clock::time_point endTime = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(endTime - startTime).count();

        cout << "Runtime: " << duration << " microseconds" << endl;

        return maxFlow;
    }
};

int main()
{
    int numVertices = 6;
    Graph graph(numVertices);

    // Adding edges to the graph
    graph.addEdge(0, 1, 16);
    graph.addEdge(0, 2, 13);
    graph.addEdge(1, 2, 10);
    graph.addEdge(1, 3, 12);
    graph.addEdge(2, 1, 4);
    graph.addEdge(2, 4, 14);
    graph.addEdge(3, 2, 9);
    graph.addEdge(3, 5, 20);
    graph.addEdge(4, 3, 7);
    graph.addEdge(4, 5, 4);

    int source = 0;
    int sink = 5;

    int maxFlow = graph.dinicMaxFlow(source, sink);
    cout << "Maximum flow: " << maxFlow << endl;

    return 0;
}

