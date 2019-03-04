#ifndef LISTGRAPH_H_INCLUDED
#define LISTGRAPH_H_INCLUDED

#include "IGraph.h"
#include<list>
#include<algorithm>

class ListGraph : public IGraph {
public:
    ListGraph(int verticesNumber_) : verticesNumber(verticesNumber_), vertices(verticesNumber_) {}
    ListGraph(const IGraph * Graph);
    void AddEdge(int from, int to);
    int VerticesCount() const;
    void GetNextVertices(int vertex, std::vector<int>& nextVertices) const;
    void GetPrevVertices(int vertex, std::vector<int>& prevVertices) const;

private:
    int verticesNumber;
    std::vector<std::list<int>> vertices;
};

#endif // LISTGRAPH_H_INCLUDED
