#include<iostream>
#include "ListGraph.h"
#include "MatrixGraph.h"
#include "ArcGraph.h"

int main() {
    ListGraph FirstGraph(6);
    FirstGraph.AddEdge(0, 1);
    FirstGraph.AddEdge(0, 2);
    FirstGraph.AddEdge(0, 3);
    FirstGraph.AddEdge(2, 4);
    FirstGraph.AddEdge(3, 4);
    FirstGraph.AddEdge(4, 1);
    FirstGraph.AddEdge(5, 4);
    FirstGraph.AddEdge(5, 2);
    std::vector<int> nextVertices;
    FirstGraph.GetNextVertices(0, nextVertices);
    for (int i = 0; i < nextVertices.size(); ++i) {
        std::cout << nextVertices[i] << std::endl;
    }
    const IGraph* CurrGraph = &FirstGraph;
    MatrixGraph SecondGraph(CurrGraph);
    std::vector<int> prevVertices;
    SecondGraph.GetPrevVertices(4, prevVertices);
    for (int i = 0; i < prevVertices.size(); ++i) {
        std::cout << prevVertices[i] << std::endl;
    }
    return 0;
}
