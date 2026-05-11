#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;

public:

    // Constructor
    Graph(int vertices) {
        V = vertices;
        adj.resize(V);
    }

    // Add edge
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // Undirected graph
    }

    // Parallel BFS
    void parallelBFS(int start) {

        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "\nParallel BFS Traversal: ";

        while (!q.empty()) {

            int node = q.front();
            q.pop();

            cout << node << " ";

            #pragma omp parallel for
            for (int i = 0; i < adj[node].size(); i++) {

                int neighbor = adj[node][i];

                if (!visited[neighbor]) {

                    #pragma omp critical
                    {
                        if (!visited[neighbor]) {
                            visited[neighbor] = true;
                            q.push(neighbor);
                        }
                    }
                }
            }
        }
    }

    // DFS Utility Function
    void parallelDFSUtil(int node, vector<bool>& visited) {

        visited[node] = true;

        cout << node << " ";

        #pragma omp parallel for
        for (int i = 0; i < adj[node].size(); i++) {

            int neighbor = adj[node][i];

            if (!visited[neighbor]) {

                #pragma omp critical
                {
                    if (!visited[neighbor]) {
                        parallelDFSUtil(neighbor, visited);
                    }
                }
            }
        }
    }

    // Parallel DFS
    void parallelDFS(int start) {

        vector<bool> visited(V, false);

        cout << "\nParallel DFS Traversal: ";

        parallelDFSUtil(start, visited);
    }
};

int main() {

    Graph g(5);

    // Creating simple tree/graph
    g.addEdge(0,1);
    g.addEdge(0,2);
    g.addEdge(1,3);
    g.addEdge(1,4);

    /*
            0
           / \
          1   2
         / \
        3   4
    */

    g.parallelBFS(0);

    cout << endl;

    g.parallelDFS(0);

    return 0;
}