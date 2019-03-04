#include<iostream>
#include<vector>
#include<queue>
#include<utility>
#include<algorithm>

class DirectedGraph {
public:
    DirectedGraph(int verticesNumber_) : verticesNumber(verticesNumber_) {}
    void AddEdge(int from, int to);
    int VerticesCount() const;
    void GetNextVertices(int vertex, std::vector<int>& nextVertices) const;
    void GetPrevVertices(int vertex, std::vector<int>& prevVertices) const;

private:
    int verticesNumber;
    std::vector<std::pair<int, int>> vertices;
};

void DirectedGraph::AddEdge(int from, int to) {
    vertices.push_back(std::make_pair(from, to));
}

int DirectedGraph::VerticesCount() const {
    return verticesNumber;
}

void DirectedGraph::GetNextVertices(int vertex, std::vector<int>& nextVertices) const {
    for (auto v : vertices) {
        if (v.first == vertex) {
            nextVertices.push_back(v.second);
        }
    }
}

void DirectedGraph::GetPrevVertices(int vertex, std::vector<int>& prevVertices) const {
    for (auto v : vertices) {
        if (v.second == vertex) {
            prevVertices.push_back(v.first);
        }
    }
}

class StronglyConnected {
public:
    StronglyConnected(DirectedGraph Graph) : graph(Graph), leave(Graph.VerticesCount()),
visited(Graph.VerticesCount(), false), sccnumbers(Graph.VerticesCount(), -1) {}
    int CountEdges();

private:
    DirectedGraph graph;
    std::vector<bool> visited;
    std::vector<int> leave;
    std::vector<int> sccnumbers;
    void maindfsinverted(DirectedGraph& currGraph, int& time); // all these functions are for Kosaraju's algorithm
    void dfsinverted(int vertex, DirectedGraph& currGraph, int& time); // during dfs of inverted graph we fill in vector 'leave'
    void maindfs(int& sccquantity);
    void dfs(int vertex, int& sccquantity); // in dfs we reach vertices in descneding order of 'leave' and fill in vector 'sccnumbers' (a number of strongly connected component for each vertex)
    void invert(DirectedGraph& newGraph);
};

void StronglyConnected::maindfsinverted(DirectedGraph& currGraph, int& time) {
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        if (visited[i] == false) {
            dfsinverted(i, currGraph, time);
        }
    }
}

void StronglyConnected::dfsinverted(int vertex, DirectedGraph& currGraph, int& time) {
    std::vector<int> nextVert;
    currGraph.GetNextVertices(vertex, nextVert);
    visited[vertex] = true;
    for (int i = 0; i < nextVert.size(); ++i) {
        if (visited[nextVert[i]] == false) {
            visited[nextVert[i]] = true;
            dfsinverted(nextVert[i], currGraph, time);
        }
    }
    leave[time++] = vertex;
}

void StronglyConnected::maindfs(int& sccquantity) {
    for (int i = graph.VerticesCount() - 1; i >= 0; --i) {
        if (visited[leave[i]] == false) {
            dfs(leave[i], sccquantity);
            ++sccquantity;
        }
    }
}

void StronglyConnected::dfs(int vertex, int& sccquantity) {
    visited[vertex] = true;
    sccnumbers[vertex] = sccquantity;
    std::vector<int> nextVert;
    graph.GetNextVertices(vertex, nextVert);
    for (int j = 0; j < nextVert.size(); ++j) {
        if (visited[nextVert[j]] == false) {
            dfs(nextVert[j], sccquantity);
        }
    }
}

void StronglyConnected::invert(DirectedGraph& newGraph) {
    for (int i = 0; i < graph.VerticesCount(); i++) {
        std::vector<int> nextVert;
        graph.GetNextVertices(i, nextVert);
        for (int j = 0; j < nextVert.size(); ++j) {
            newGraph.AddEdge(nextVert[j], i);
        }
    }
}

int StronglyConnected::CountEdges() {
    DirectedGraph invertedGraph(graph.VerticesCount());
    invert(invertedGraph);
    int time = 0;
    maindfsinverted(invertedGraph, time);
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        visited[i] = false;
    }
    int SCCquantity = 0;
    maindfs(SCCquantity);
    DirectedGraph CondensationGraph(SCCquantity);
    for (int i = 0; i < graph.VerticesCount(); ++i) { // building a condensation graph
        std::vector<int> nextVert;
        graph.GetNextVertices(i, nextVert);
        for (int j = 0; j < nextVert.size(); ++j) {
            if (sccnumbers[i] != sccnumbers[nextVert[j]]) {
                CondensationGraph.AddEdge(sccnumbers[i], sccnumbers[nextVert[j]]);
            }
        }
    }
    int noentry = 0, noexit = 0;
    for (int i = 0; i < CondensationGraph.VerticesCount(); ++i) { // the answer is max from numbers of vertices with no 'entry' edges and vertices with no 'exit' edges
        std::vector<int> nextVert, prevVert;
        CondensationGraph.GetNextVertices(i, nextVert);
        CondensationGraph.GetPrevVertices(i, prevVert);
        if (nextVert.empty()) {
            ++noexit;
        }
        if (prevVert.empty()) {
            ++noentry;
        }
    }
    return CondensationGraph.VerticesCount() != 1 ? std::max(noentry, noexit) : 0; // if the current graph comprises the only SCC, we don't need to add edges
}

int main() {
    int v, e;
    std::cin >> v >> e;
    DirectedGraph Graph(v);
    for (int i = 0; i < e; ++i) {
        int from, to;
        std::cin >> from >> to;
        Graph.AddEdge(from - 1, to - 1);
    }
    StronglyConnected ModifyGraph(Graph);
    int answer = ModifyGraph.CountEdges();
    std::cout << answer;
    return 0;
}
