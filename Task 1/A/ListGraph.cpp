#include "ListGraph.h"

ListGraph::ListGraph(const IGraph * Graph) : verticesNumber(Graph -> VerticesCount()), vertices(Graph -> VerticesCount()) {
    for (int v = 0; v < verticesNumber; ++v) {
        std::vector<int> currVertices;
        Graph -> GetNextVertices(v, currVertices);
        for (int i = 0; i < currVertices.size(); ++i) {
            AddEdge(v, currVertices[i]);
        }
    }
}

void ListGraph::AddEdge(int from, int to) {
    vertices[from].push_back(to);
}

int ListGraph::VerticesCount() const {
    return verticesNumber;
}

void ListGraph::GetNextVertices(int vertex, std::vector<int>& nextVertices) const {
    for (auto v : vertices[vertex]) {
        nextVertices.push_back(v);
    }
}

void ListGraph::GetPrevVertices(int vertex, std::vector<int>& prevVertices) const {
    for (int v = 0; v < verticesNumber; ++v) {
        if (std::find(vertices[v].begin(), vertices[v].end(), vertex) != vertices[v].end())
            prevVertices.push_back(v);
    }
}
