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
    std::vector<int> depth; // вектор для глубин вершин, считаются отдельно для каждого бфс; значение для исходной вершины = 0
    std::vector<int> parents; // вектор для родителей вершин (для первого родителя, который достиг текущей вершины)
    std::vector<bool> visited; // вектор-маркер для посещённых вершин
    int bfs(int v);
};

int MinCycle::bfs(int v) { // во время обходов считаем для каждой вершины глубину и родителя
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
        minCycles[v] = bfs(v); // обходим граф от каждой вершины и в каждом бфс считаем сумму глубин для вершины первого найденного цикла
    } // (при таком алгоритме мы хоть раз запустимся от вершины в минимальном цикле, тогда сумма глубин для некоторой другой вершины этого цикла и есть искомая длина)
    int minReturn = graph.VerticesCount() + 1;
    for (int i = 0; i < graph.VerticesCount(); ++i) { // выбор минимальной длины цикла
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
