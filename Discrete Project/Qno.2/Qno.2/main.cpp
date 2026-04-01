#include <iostream>
using namespace std;

class Graph {
private:
    int adjMatrix[100][100] = { 0 };
    int degrees[100] = { 0 };

public:
    void addEdge(int u, int v) {
        adjMatrix[u][v] = 1;
        adjMatrix[v][u] = 1;
        degrees[u]++;
        degrees[v]++;
    }

    void displayDegrees(int vertices) {
        cout << "Vertex Degrees:" << endl;
        for (int i = 0; i < vertices; ++i) {
            cout << "Vertex " << i << ": " << degrees[i] << " degree(s)" << endl;
        }
        cout << endl;
    }

    bool isClosedWalk(int path[], int size) {
        return path[0] == path[size - 1];
    }

    bool isCircuit(int path[], int size) {
        return isClosedWalk(path, size) && size > 2;
    }

    bool isSimpleCircuit(int path[], int size) {
        bool visited[100] = { false };
        for (int i = 0; i < size; ++i) {
            int vertex = path[i];
            if (visited[vertex]) {
                return false;
            }
            visited[vertex] = true;
        }
        return isCircuit(path, size);
    }

    bool isPath(int path[], int size) {
        bool visited[100] = { false };
        for (int i = 0; i < size; ++i) {
            int vertex = path[i];
            if (visited[vertex]) {
                return false;
            }
            visited[vertex] = true;
        }
        return true;
    }

    bool isSimplePath(int path[], int size) {
        return isPath(path, size);
    }
};

int main() {
    using namespace std;

    int vertices, edges;
    cout << "Enter the number of vertices: ";
    cin >> vertices;

    cout << "Enter the number of edges: ";
    cin >> edges;

    Graph graph;

    cout << "Enter the edges (format: u v):" << endl;
    for (int i = 0; i < edges; ++i) {
        int u, v;
        cin >> u >> v;
        graph.addEdge(u, v);
    }

    graph.displayDegrees(vertices);

    cout << "Enter the size of the path: ";
    int size;
    cin >> size;

    int path[100];
    cout << "Enter the path (space-separated vertices): ";
    for (int i = 0; i < size; ++i) {
        cin >> path[i];
    }

    if (graph.isSimplePath(path, size)) {
        cout << "It is a simple path." << endl;
    }
    else if (graph.isPath(path, size)) {
        cout << "It is a path." << endl;
    }
    else if (graph.isSimpleCircuit(path, size)) {
        cout << "It is a simple circuit." << endl;
    }
    else if (graph.isCircuit(path, size)) {
        cout << "It is a circuit." << endl;
    }
    else if (graph.isClosedWalk(path, size)) {
        cout << "It is a closed walk." << endl;
    }
    else {
        cout << "It is a walk." << endl;
    }

    return 0;
}
