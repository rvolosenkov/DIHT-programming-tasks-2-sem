#include "SetGraph.h"

SetGraph::SetGraph(const IGraph * Graph) : verticesNumber_(Graph -> VerticesCount()), vertices_(Graph -> VerticesCount()) {
    for (int v = 0; v < verticesNumber_; ++v) {
        std::vector<int> currVertices;
        Graph -> GetNextVertices(v, currVertices);
        for (int i = 0; i < currVertices.size(); ++i) {
            AddEdge(v, currVertices[i]);
        }
    }
}

void SetGraph::AddEdge(int from, int to) {
    vertices_[from].insert(to);
}

int SetGraph::VerticesCount() const {
    return verticesNumber_;
}

void SetGraph::GetNextVertices(int vertex, std::vector<int>& vertices) const {
    for (auto v : vertices_[vertex]) {
        vertices.push_back(v);
    }
}

void SetGraph::GetPrevVertices(int vertex, std::vector<int>& vertices) const {
    for (int v = 0; v < verticesNumber_; ++v) {
        if (vertices_[v].find(vertex) != vertices_[v].end())
            vertices.push_back(v);
    }
}
