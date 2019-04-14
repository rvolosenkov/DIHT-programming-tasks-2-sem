#include<iostream>
#include<vector>
#include<utility>

const int inf = 1e9;

template<typename T>
class DirectedGraph {
public:
    DirectedGraph(int verticesNumber_) : verticesNumber(verticesNumber_), vertices(verticesNumber_, std::vector<T>(verticesNumber_, 0)) {}
    void AddEdge(int from, int to, T weight);
    int VerticesCount() const;
    void GetNextVertices(int vertex, std::vector<std::pair<T, int>>& nextVertices) const;
    void GetPrevVertices(int vertex, std::vector<std::pair<T, int>>& prevVertices) const;

private:
    int verticesNumber;
    std::vector<std::vector<T>> vertices;
};

template<typename T>
void DirectedGraph<T>::AddEdge(int from, int to, T weight) {
    vertices[from][to] = weight;
}

template<typename T>
int DirectedGraph<T>::VerticesCount() const {
    return verticesNumber;
}

template<typename T>
void DirectedGraph<T>::GetNextVertices(int vertex, std::vector<std::pair<T, int>>& nextVertices) const {
    for (int i = 0; i < verticesNumber; ++i) {
        if (vertices[vertex][i] != 0) {
            nextVertices.push_back(std::make_pair(vertices[vertex][i], i));
        }
    }
}

template<typename T>
void DirectedGraph<T>::GetPrevVertices(int vertex, std::vector<std::pair<T, int>>& prevVertices) const {
    for (int i = 0; i < verticesNumber; ++i) {
        if (vertices[i][vertex] != 0) {
            prevVertices.push_back(std::make_pair(vertices[i][vertex], i));
        }
    }
}

template<typename T>
class Bellman {
public:
    Bellman(DirectedGraph<T>* Graph) : graph(Graph), longestWays(Graph -> VerticesCount(), -1.0) {}
    bool FindGoodCycle();

private:
    DirectedGraph<T>* graph;
    std::vector<T> longestWays; // to count a longest way to each vertex from a current one
};

template<typename T>
bool Bellman<T>::FindGoodCycle() {
    for (int w = 0; w < graph -> VerticesCount(); ++w) { // look through all ways for each vertex
        longestWays[w] = 1.0;
        for (int k = 0; k < graph -> VerticesCount() - 1; ++k) {
            for (int u = 0; u < graph -> VerticesCount(); ++u) {
                std::vector<std::pair<T, int>> prevGeneration;
                graph -> GetPrevVertices(u, prevGeneration);
                for (auto v : prevGeneration) {
                    if (longestWays[u] != -1.0 && longestWays[v.second] < longestWays[u] * v.first) {
                        longestWays[v.second] = longestWays[u] * v.first;
                    }
                }
            }
        }
        for (int u = 0; u < graph -> VerticesCount(); ++u) { // if we are in the beginning vertex and change a length again, we found a suitable cycle
            std::vector<std::pair<T, int>> prevGeneration;
            graph -> GetPrevVertices(u, prevGeneration);
            for (auto v : prevGeneration) {
                if (longestWays[u] != -1.0 && longestWays[v.second] < longestWays[u] * v.first) {
                    longestWays.clear();
                    return true;
                }
            }
        }
        for (int i = 0; i < longestWays.size(); ++i) {
            longestWays[i] = -1.0;
        }
    }
    longestWays.clear();
    return false;
}

int main() {
    int n;
    std::cin >> n;
    DirectedGraph<double> Graph(n);
    for (int from = 0; from < n; ++from) {
        for (int to = 0; to < n; ++to) {
            if (from == to) {
                Graph.AddEdge(from, to, 1.0);
            } else {
                double weight;
                std::cin >> weight;
                Graph.AddEdge(from, to, weight);
            }
        }
    }
    Bellman<double> SearchBenefit(&Graph);
    bool answer = SearchBenefit.FindGoodCycle();
    std::cout << (answer == true ? "YES" : "NO");
    return 0;
}
