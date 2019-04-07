#include<iostream>
#include<vector>
#include<list>
#include<set>
#include<utility>

const int inf = 1e9; // inf > max(n) * max(lenOfWay)

class UndirectedGraph {
public:
    UndirectedGraph(int verticesNumber_) : verticesNumber(verticesNumber_), vertices(verticesNumber_) {}
    void AddEdge(int from, int to, double weight);
    int VerticesCount() const;
    void GetNextVertices(int vertex, std::vector<std::pair<double, int>>& nextVertices) const;
    void GetPrevVertices(int vertex, std::vector<std::pair<double, int>>& prevVertices) const;

private:
    int verticesNumber;
    std::vector<std::list<std::pair<double, int>>> vertices;
};

void UndirectedGraph::AddEdge(int from, int to, double weight) {
    vertices[from].push_back(std::make_pair(weight, to));
    vertices[to].push_back(std::make_pair(weight, from));
}

int UndirectedGraph::VerticesCount() const {
    return verticesNumber;
}

void UndirectedGraph::GetNextVertices(int vertex, std::vector<std::pair<double, int>>& nextVertices) const {
    for (auto v : vertices[vertex]) {
        nextVertices.push_back(v);
    }
}

void UndirectedGraph::GetPrevVertices(int vertex, std::vector<std::pair<double, int>>& prevVertices) const {
    for (int v = 0; v < verticesNumber; ++v) {
        for (auto w : vertices[v]) {
            if (w.second == vertex) {
                prevVertices.push_back(std::make_pair(w.first, v));
            }
        }
    }
}

class Dijkstra {
public:
    Dijkstra(UndirectedGraph* Graph) : graph(Graph) {}
    double Find(int from, int to);

private:
    UndirectedGraph* graph;
    int from;
    int to;
    std::set<std::pair<double, int>> nearestVertices; // we use set (priority queue) to check all the vertices
    std::vector<double> minWays;
    std::vector<bool> visited;
};

double Dijkstra::Find(int from, int to) {
    for (int i = 0; i < graph -> VerticesCount(); ++i) {
        minWays.push_back(inf);
        visited.push_back(false);
    }
    minWays[from] = 0;
    nearestVertices.insert(std::make_pair(0, from));
    while (nearestVertices.empty() == false) {
        std::pair<double, int> currVertex = *nearestVertices.begin();
        nearestVertices.erase(nearestVertices.begin());
        visited[currVertex.second] = true;
        std::vector<std::pair<double, int>> nextGeneration;
        graph -> GetNextVertices(currVertex.second, nextGeneration); // get all offspring of a current vertex
        for (int i = 0; i < nextGeneration.size(); ++i) {
            if (visited[nextGeneration[i].second] == false) {
                if (minWays[nextGeneration[i].second] > minWays[currVertex.second] + nextGeneration[i].first) {
                    if (minWays[nextGeneration[i].second] != inf) {
                        nearestVertices.erase(std::make_pair(nextGeneration[i].second, minWays[nextGeneration[i].second]));
                    }
                    minWays[nextGeneration[i].second] = minWays[currVertex.second] + nextGeneration[i].first;
                    nearestVertices.insert(std::make_pair(minWays[nextGeneration[i].second], nextGeneration[i].second)); // take next vertex from set or decrease key for old vertices
                }
            }
        }
    }
    double answer = (minWays[to] < inf ? minWays[to] : -1.0);
    nearestVertices.clear();
    minWays.clear();
    visited.clear();
    return answer;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    UndirectedGraph Graph(n);
    for (int i = 0; i < m; ++i) {
        int from, to;
        double weight;
        std::cin >> from >> to >> weight;
        Graph.AddEdge(from, to, weight);
    }
    int from, to;
    std::cin >> from >> to;
    Dijkstra FindMinWay(&Graph);
    double answer = FindMinWay.Find(from, to);
    std::cout << answer;
    return 0;
}
