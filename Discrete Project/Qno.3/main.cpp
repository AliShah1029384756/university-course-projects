#include <iostream>
#include <queue>

using namespace std;

class Graph {
public:
    int V;
    int** adjMatrix;

    Graph(int vertices) {
        V = vertices;
        adjMatrix = new int* [V];
        for (int i = 0; i < V; ++i) {
            adjMatrix[i] = new int[V];
            for (int j = 0; j < V; ++j) {
                adjMatrix[i][j] = 0;
            }
        }
    }

    void addEdge(int u, int v) {
        adjMatrix[u][v] = 1;
        adjMatrix[v][u] = 1;
    }

    bool isBipartite() {
        int* color = new int[V];
        for (int i = 0; i < V; ++i) {
            color[i] = -1;
        }

        int* conflictingVertices = new int[V];
        int conflictingVerticesCount = 0;

        for (int i = 0; i < V; ++i) {
            if (color[i] == -1) {
                if (!dfs(i, 0, color, conflictingVertices, conflictingVerticesCount)) {
                    displayConflictingVertices(conflictingVertices, conflictingVerticesCount);
                    delete[] color;
                    delete[] conflictingVertices;
                    return false;
                }
            }
        }

        delete[] color;
        delete[] conflictingVertices;
        return true;
    }

private:
    bool dfs(int node, int currentColor, int* color, int* conflictingVertices, int& count) {
        color[node] = currentColor;

        for (int neighbor = 0; neighbor < V; ++neighbor) {
            if (adjMatrix[node][neighbor] == 1) {
                if (color[neighbor] == -1) {
                    if (!dfs(neighbor, 1 - currentColor, color, conflictingVertices, count)) {
                        conflictingVertices[count++] = node;
                        conflictingVertices[count++] = neighbor;
                        return false;
                    }
                }
                else if (color[neighbor] == currentColor) {
                    conflictingVertices[count++] = node;
                    conflictingVertices[count++] = neighbor;
                    return false;
                }
            }
        }

        return true;
    }

    void displayConflictingVertices(const int* conflictingVertices, int count) {
        cout << "Graph is not bipartite. Conflicting vertices are:";
        for (int i = 0; i < count; ++i) {
            cout << " " << conflictingVertices[i];
        }
        cout << endl;
    }
};

int main() {
    int V, E;
    cout << "Enter the number of vertices and edges: ";
    cin >> V >> E;

    Graph graph(V);

    cout << "Enter the edges (vertex pairs):" << endl;
    for (int i = 0; i < E; ++i) {
        int u, v;
        cin >> u >> v;
        graph.addEdge(u, v);
    }

    if (graph.isBipartite()) {
        cout << "Graph is bipartite." << endl;
    }

    return 0;
}
