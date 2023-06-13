#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
using namespace std;
using namespace std::chrono;

// Structure to represent an edge in the graph
struct Edge {
    int source, destination, capacity, flow;

    Edge(int s, int d, int c)
    {
        source = s;
        destination = d;
        capacity = c;
        flow = 0;
    }
};

// Class to represent the graph
class Graph {
    int numVertices;
    vector<Edge> edges;
    vector<int> heights, excessFlows, labels;
    vector<vector<int>> residualCapacity;

public:
    Graph(int V)
    {
        numVertices = V;
        heights.resize(numVertices, 0);
        excessFlows.resize(numVertices, 0);
        labels.resize(numVertices, 0);
        residualCapacity.resize(numVertices, vector<int>(numVertices, 0));
    }

    // Function to add an edge to the graph
    void addEdge(int source, int destination, int capacity)
    {
        // Adding forward edge
        edges.push_back(Edge(source, destination, capacity));
        residualCapacity[source][destination] = capacity;

        // Adding backward edge
        edges.push_back(Edge(destination, source, 0));
        residualCapacity[destination][source] = 0;
    }

    // Function to initialize preflow
    void initializePreflow(int source)
    {
        heights[source] = numVertices;

        for (int i = 0; i < numVertices; i++) {
            if (i != source) {
                excessFlows[i] = 0;
                labels[i] = 0;
            }
        }

        for (auto& edge : edges) {
            if (edge.source == source) {
                edge.flow = edge.capacity;
                residualCapacity[edge.source][edge.destination] = 0;
                residualCapacity[edge.destination][edge.source] = edge.capacity;
                excessFlows[edge.destination] = edge.capacity;
            }
        }
    }

    // Function to relabel a vertex
    void relabelVertex(int vertex)
    {
        int minHeight = INT_MAX;

        for (int i = 0; i < numVertices; i++) {
            if (residualCapacity[vertex][i] > 0)
                minHeight = min(minHeight, heights[i]);
        }

        heights[vertex] = minHeight + 1;
    }

    // Function to discharge excess flow from a vertex
    void discharge(int vertex)
    {
        while (excessFlows[vertex] > 0) {
            if (labels[vertex] < numVertices) {
                int nextVertex = labels[vertex];
                if (residualCapacity[vertex][nextVertex] > 0 &&
                    heights[vertex] == heights[nextVertex] + 1) {
                    int flow = min(excessFlows[vertex], residualCapacity[vertex][nextVertex]);
                    excessFlows[vertex] -= flow;
                    excessFlows[nextVertex] += flow;
                    residualCapacity[vertex][nextVertex] -= flow;
                    residualCapacity[nextVertex][vertex] += flow;
                }
                else {
                    labels[vertex]++;
                }
            }
            else {
                relabelVertex(vertex);
                labels[vertex] = 0;
            }
        }
    }

    // Function to compute maximum flow using Push-Relabel algorithm
    int pushRelabelMaxFlow(int source, int sink)
    {
        initializePreflow(source);

        queue<int> activeVertices;
        for (int i = 0; i < numVertices; i++) {
            if (i != source && i != sink)
                activeVertices.push(i);
        }

        high_resolution_clock::time_point startTime = high_resolution_clock::now();

        while (!activeVertices.empty()) {
            int vertex = activeVertices.front();
            int oldHeight = heights[vertex];
            discharge(vertex);
            if (heights[vertex] > oldHeight)
                activeVertices.push(vertex);
            activeVertices.pop();
        }

        high_resolution_clock::time_point endTime = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(endTime - startTime).count();

        cout << "Runtime: " << duration << " microseconds" << endl;

        return excessFlows[sink];
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

    int maxFlow = graph.pushRelabelMaxFlow(source, sink);
    cout << "Maximum flow: " << maxFlow << endl;

    return 0;
}
