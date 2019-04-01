#include "ArcGraph.h"

ArcGraph::ArcGraph(const IGraph* Graph) : verticesNumber(Graph -> VerticesCount()) {
    for (int v = 0; v < verticesNumber; ++v) {
        std::vector<int> currVertices;
        Graph -> GetNextVertices(v, currVertices);
        for (int i = 0; i < currVertices.size(); ++i) {
            AddEdge(v, currVertices[i]);
        }
    }
}

void ArcGraph::AddEdge(int from, int to) {
    vertices.push_back(std::make_pair(from, to));
}

int ArcGraph::VerticesCount() const {
    return verticesNumber;
}

void ArcGraph::GetNextVertices(int vertex, std::vector<int>& nextVertices) const {
    for (auto v : vertices) {
        if (v.first == vertex) {
            nextVertices.push_back(v.second);
        }
    }
}

void ArcGraph::GetPrevVertices(int vertex, std::vector<int>& prevVertices) const {
    for (auto v : vertices) {
        if (v.second == vertex) {
            prevVertices.push_back(v.first);
        }
    }
}
