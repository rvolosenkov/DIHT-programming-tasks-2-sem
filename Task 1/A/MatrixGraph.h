#ifndef MATRIXGRAPH_H_INCLUDED
#define MATRIXGRAPH_H_INCLUDED

#include "IGraph.h"

class MatrixGraph : public IGraph {
public:
    MatrixGraph(int verticesNumber_) : verticesNumber(verticesNumber_), vertices(verticesNumber_, std::vector<bool>(verticesNumber_, false)) {}
    MatrixGraph(const IGraph * Graph);
    void AddEdge(int from, int to);
    int VerticesCount() const;
    void GetNextVertices(int vertex, std::vector<int>& nextVertices) const;
    void GetPrevVertices(int vertex, std::vector<int>& prevVertices) const;

private:
    int verticesNumber;
    std::vector<std::vector<bool>> vertices;
};

#endif // MATRIXGRAPH_H_INCLUDED
