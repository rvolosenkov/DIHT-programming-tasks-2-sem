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

class BipartiteCheck {
public:
    BipartiteCheck(UndirectedGraph Graph) : graph(Graph) {
        part.reserve(graph.VerticesCount());
        for (int i = 0; i < graph.VerticesCount(); ++i) {
            part[i] = -1;
        }
    }
    bool CheckBigraph();

private:
    UndirectedGraph graph;
    std::vector<int> part; // markers of vertices to differentiate parts of a bigraph
    bool bfs(int v);
};

bool BipartiteCheck::bfs(int v) {
    std::queue<int> vertices;
    vertices.push(v);
    part[v] = 1;
    while(vertices.empty() == false) {
        int currVertex = vertices.front();
        vertices.pop();
        std::vector<int> nextGeneration;
        graph.GetNextVertices(currVertex, nextGeneration);
        for (auto w : nextGeneration) {
            if (part[w] == part[currVertex]) { // if there is a conflict (neighboring vertices are from one part), this graph is not bipartite
                return false;
            }
            if (part[w] == -1) {
                vertices.push(w);
            }
            part[w] = 1 - part[currVertex];
        }
    }
    return true;
}

bool BipartiteCheck::CheckBigraph() {
    return bfs(0);
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
    BipartiteCheck IsBipartite(Graph);
    if (IsBipartite.CheckBigraph() == true) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
    return 0;
}
