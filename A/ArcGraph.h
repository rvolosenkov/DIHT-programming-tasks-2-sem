#ifndef ARCGRAPH_H_INCLUDED
#define ARCGRAPH_H_INCLUDED

#include "IGraph.h"
#include<utility>

class ArcGraph : public IGraph {
public:
    ArcGraph(int verticesNumber_) : verticesNumber(verticesNumber_) {}
    ArcGraph(const IGraph * Graph);
    void AddEdge(int from, int to);
    int VerticesCount() const;
    void GetNextVertices(int vertex, std::vector<int>& nextVertices) const;
    void GetPrevVertices(int vertex, std::vector<int>& prevVertices) const;

private:
    int verticesNumber;
    std::vector<std::pair<int, int>> vertices;
};

#endif // ARCGRAPH_H_INCLUDED
