#include<iostream>
#include<vector>
#include<utility>

const int inf = 1e9;

class DirectedGraph {
public:
    DirectedGraph(int verticesNumber_) : verticesNumber(verticesNumber_), vertices(verticesNumber_, std::vector<double>(verticesNumber_, 0)) {}
    void AddEdge(int from, int to, double weight);
    int VerticesCount() const;
    void GetNextVertices(int vertex, std::vector<std::pair<double, int>>& nextVertices) const;
    void GetPrevVertices(int vertex, std::vector<std::pair<double, int>>& prevVertices) const;

private:
    int verticesNumber;
    std::vector<std::vector<double>> vertices;
};

void DirectedGraph::AddEdge(int from, int to, double weight) {
    vertices[from][to] = weight;
}

int DirectedGraph::VerticesCount() const {
    return verticesNumber;
}

void DirectedGraph::GetNextVertices(int vertex, std::vector<std::pair<double, int>>& nextVertices) const {
    for (int i = 0; i < verticesNumber; ++i) {
        if (vertices[vertex][i] != 0) {
            nextVertices.push_back(std::make_pair(vertices[vertex][i], i));
        }
    }
}

void DirectedGraph::GetPrevVertices(int vertex, std::vector<std::pair<double, int>>& prevVertices) const {
    for (int i = 0; i < verticesNumber; ++i) {
        if (vertices[i][vertex] != 0) {
            prevVertices.push_back(std::make_pair(vertices[vertex][i], i));
        }
    }
}

class Floyd {
public:
    Floyd(DirectedGraph* Graph) : graph(Graph) {}
    void FindMatrix(std::vector<std::vector<double>>& Matrix);

private:
    DirectedGraph* graph;
};

void Floyd::FindMatrix(std::vector<std::vector<double>>& Matrix) {
    for (int i = 0; i < graph -> VerticesCount(); ++i) { // initially we fill Matrix with ways which contains not more than 1 edge
        std::vector<std::pair<double, int>> nextGeneration;
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
    DirectedGraph Graph(n);
    for (int from = 0; from < n; ++from) {
        for (int to = 0; to < n; ++to) {
            double weight;
            std::cin >> weight;
            Graph.AddEdge(from, to, weight);
        }
    }
    Floyd SearchShortestWays(&Graph);
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
