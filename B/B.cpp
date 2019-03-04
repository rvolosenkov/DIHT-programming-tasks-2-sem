#include<iostream>
#include<vector>
#include<unordered_set>
#include<queue>

class UndirectedGraph {
public:
    UndirectedGraph(int verticesNumber_) : verticesNumber(verticesNumber_), vertices(verticesNumber_) {}
    void AddEdge(int from, int to);
    int VerticesCount() const;
    void GetNextVertices(int vertex, std::vector<int>& nextVertices) const;

private:
    int verticesNumber;
    std::vector<std::unordered_set<int>> vertices;
};

void UndirectedGraph::AddEdge(int from, int to) {
    vertices[from].insert(to);
    vertices[to].insert(from);
}

int UndirectedGraph::VerticesCount() const {
    return verticesNumber;
}

void UndirectedGraph::GetNextVertices(int vertex, std::vector<int>& nextVertices) const {
    for (auto v : vertices[vertex]) {
        nextVertices.push_back(v);
    }
}

class MinCycle {
public:
    MinCycle(UndirectedGraph Graph) : graph(Graph) {
        depth.reserve(graph.VerticesCount());
        parents.reserve(graph.VerticesCount());
        visited.reserve(graph.VerticesCount());
    }
    int FindMin();

private:
    UndirectedGraph graph;
    std::vector<int> depth; // vector for depths of vertices (for initial vertex = 0)
    std::vector<int> parents; // vector for parents (the first parent to have reached the current vertex)
    std::vector<bool> visited; // vector-marker for visited vertices
    int bfs(int v);
};

int MinCycle::bfs(int v) { // during bfs: count a depth and a parent for each vertex
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        parents[i] = -1;
        visited[i] = false;
    }
    depth[v] = 0;
    parents[v] = -2;
    std::queue<int> vertices;
    vertices.push(v);
    while (vertices.empty() == false) {
        int currVertex = vertices.front();
        vertices.pop();
        visited[currVertex] = true;
        std::vector<int> nextGeneration;
        graph.GetNextVertices(currVertex, nextGeneration);
        for (auto w : nextGeneration) {
            if (visited[w] == false) {
                vertices.push(w);
                depth[w] = depth[currVertex] + 1;
                parents[w] = currVertex;
            } else {
                if (parents[currVertex] != w) {
                    return depth[currVertex] + depth[w] + 1;
                }
            }
        }
    }
    return graph.VerticesCount() + 1;
}

int MinCycle::FindMin() {
    std::vector<int> minCycles(graph.VerticesCount());
    for (int v = 0; v < graph.VerticesCount(); ++v) {
        minCycles[v] = bfs(v); // for each bfs: count a sum of depths for a vertex of the first found cycle
    } // (this algorithm guarantees us that we will launch with a vertex in min cycle and so the answer is a sum of two depths of another vertex in this cycle)
    int minReturn = graph.VerticesCount() + 1;
    for (int i = 0; i < graph.VerticesCount(); ++i) { // choosing min from cycles' lengths
        if (minCycles[i] < minReturn) {
            minReturn = minCycles[i];
        }
    }
    if (minReturn == graph.VerticesCount() + 1) {
        minReturn = -1;
    }
    return minReturn;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    UndirectedGraph Graph(n);
    for (int i = 0; i < m; ++i) {
        int from, to;
        std::cin >> from >> to;
        Graph.AddEdge(from, to);
    }
    MinCycle FindMinCycle(Graph);
    int answer = FindMinCycle.FindMin();
    std::cout << answer;
    return 0;
}
