#include "MatrixGraph.h"
#include<iostream>

MatrixGraph::MatrixGraph(const IGraph* Graph) : verticesNumber(Graph -> VerticesCount()), vertices(Graph -> VerticesCount(), std::vector<bool>(Graph -> VerticesCount(), false)) {
    for (int v = 0; v < verticesNumber; ++v) {
        std::vector<int> currVertices;

        Graph -> GetNextVertices(v, currVertices);
        for (int i = 0; i < currVertices.size(); ++i) {
            AddEdge(v, currVertices[i]);
        }
    }
}

void MatrixGraph::AddEdge(int from, int to) {
    vertices[from][to] = true;
}

int MatrixGraph::VerticesCount() const {
    return verticesNumber;
}

void MatrixGraph::GetNextVertices(int vertex, std::vector<int>& nextVertices) const {
    for (int i = 0; i < verticesNumber; ++i) {
        if (vertices[vertex][i] == true) {
            nextVertices.push_back(i);
        }
    }
}

void MatrixGraph::GetPrevVertices(int vertex, std::vector<int>& prevVertices) const {
    for (int i = 0; i < verticesNumber; ++i) {
        if (vertices[i][vertex] == true) {
            prevVertices.push_back(i);
        }
    }
}
