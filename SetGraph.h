#ifndef SETGRAPH_H_INCLUDED
#define SETGRAPH_H_INCLUDED

#include "IGraph.h"
#include<unordered_set>

class SetGraph : public IGraph {
public:
    SetGraph(int verticesNumber) : verticesNumber_(verticesNumber), vertices_(verticesNumber) {}
    SetGraph(const IGraph * Graph);
    void AddEdge(int from, int to);
    int VerticesCount() const;
    void GetNextVertices(int vertex, std::vector<int>& vertices) const;
    void GetPrevVertices(int vertex, std::vector<int>& vertices) const;

private:
    int verticesNumber_;
    std::vector<std::unordered_set<int>> vertices_;
};

#endif // SETGRAPH_H_INCLUDED
