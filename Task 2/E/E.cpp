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
class Floyd {
public:
    Floyd(DirectedGraph<T>* Graph) : graph(Graph) {}
    void FindMatrix(std::vector<std::vector<T>>& Matrix);

private:
    DirectedGraph<T>* graph;
};

template<typename T>
void Floyd<T>::FindMatrix(std::vector<std::vector<T>>& Matrix) {
    for (int i = 0; i < graph -> VerticesCount(); ++i) { // initially we fill Matrix with ways which contains not more than 1 edge
        std::vector<std::pair<T, int>> nextGeneration;
        graph -> GetNextVertices(i, nextGeneration);
        for (int j = 0; j < nextGeneration.size(); ++j) {
            Matrix[i][nextGeneration[j].second] = nextGeneration[j].first;
        }
    }
    for (int k = 0; k < graph -> VerticesCount(); ++k) { // look through all possible ways with not more than k+1 edge
        for (int i = 0; i < graph -> VerticesCount(); ++i) {
            for (int j = 0; j < graph -> VerticesCount(); ++j) {
                Matrix[i][j] = std::min(Matrix[i][j], Matrix[i][k] + Matrix[k][j]); // change a way length to a better one
            }
        }
    }
}

int main() {
    int n;
    std::cin >> n;
    DirectedGraph<double> Graph(n);
    for (int from = 0; from < n; ++from) {
        for (int to = 0; to < n; ++to) {
            double weight;
            std::cin >> weight;
            Graph.AddEdge(from, to, weight);
        }
    }
    Floyd<double> SearchShortestWays(&Graph);
    std::vector<std::vector<double>> MatrixOfWays(n, std::vector<double>(n, 0));
    SearchShortestWays.FindMatrix(MatrixOfWays);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << MatrixOfWays[i][j] << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
