#include<iostream>
#include<vector>
#include<queue>

class UndirectedGraph {
public:
    UndirectedGraph(int verticesNumber_) : verticesNumber(verticesNumber_), vertices(verticesNumber_) {}
    void AddEdge(int from, int to);
    int VerticesCount() const;
    void GetNextVertices(int vertex, std::vector<int>& nextVertices) const;

private:
    int verticesNumber;
    std::vector<std::vector<int>> vertices;
};

void UndirectedGraph::AddEdge(int from, int to) {
    vertices[from].push_back(to);
    vertices[to].push_back(from);
}

int UndirectedGraph::VerticesCount() const {
    return verticesNumber;
}

void UndirectedGraph::GetNextVertices(int vertex, std::vector<int>& nextVertices) const {
    for (auto v : vertices[vertex]) {
        nextVertices.push_back(v);
    }
}

class ShortestWays {
public:
    ShortestWays(UndirectedGraph Graph) : graph(Graph) {
        ways.reserve(graph.VerticesCount());
        depth.reserve(graph.VerticesCount());
        for (int i = 0; i < graph.VerticesCount(); ++i) {
            ways[i] = 0;
            depth[i] = -1;
        }
    }
    int FindWays(int u, int w);

private:
    UndirectedGraph graph;
    std::vector<int> ways;
    std::vector<int> depth;
    void bfs(int u);
};

void ShortestWays::bfs(int u) { // for each generation: count a number of ways from the first main vertex (u) to the current vertex
    std::queue<int> vertices;
    vertices.push(u);
    depth[u] = 0;
    ways[u] = 1;
    while(vertices.empty() == false) {
        int currVertex = vertices.front();
        vertices.pop();
        std::vector<int> nextGeneration;
        graph.GetNextVertices(currVertex, nextGeneration);
        for (auto v : nextGeneration) {
            if (depth[v] == -1) {
                depth[v] = depth[currVertex] + 1;
                ways[v] += ways[currVertex];
                vertices.push(v);
            } else {
                if (depth[v] > depth[currVertex]) {
                    ways[v] += ways[currVertex];
                }
            }
        }
    }
}

int ShortestWays::FindWays(int u, int w) {
    bfs(u);
    return ways[w];
}

int main() {
    int v, n;
    std::cin >> v >> n;
    UndirectedGraph Graph(v);
    for (int i = 0; i < n; ++i) {
        int from, to;
        std::cin >> from >> to;
        Graph.AddEdge(from, to);
    }
    ShortestWays FindShortestWays(Graph);
    int u, w;
    std::cin >> u >> w;
    int answer = FindShortestWays.FindWays(u, w);
    std::cout << answer;
    return 0;
}
