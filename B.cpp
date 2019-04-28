#include<iostream>
#include<vector>
#include<utility>
#include<queue>

const int inf = 1e9;

class DirectedGraph {
public:
    DirectedGraph(int verticesNumber_) : verticesNumber(verticesNumber_), vertices(verticesNumber_, std::vector<int>(verticesNumber_, 0)) {}
    void AddEdge(int from, int to, int weight);
    void ChangeEdge(int from, int to, int weight);
    int VerticesCount() const;
    void GetNextVertices(int vertex, std::vector<std::pair<int, int>>& nextVertices) const;
    void GetPrevVertices(int vertex, std::vector<std::pair<int, int>>& prevVertices) const;

private:
    int verticesNumber;
    std::vector<std::vector<int>> vertices;
};

void DirectedGraph::AddEdge(int from, int to, int weight) {
    vertices[from][to] = weight;
}

void DirectedGraph::ChangeEdge(int from, int to, int weight) {
    vertices[from][to] += weight;
}

int DirectedGraph::VerticesCount() const {
    return verticesNumber;
}

void DirectedGraph::GetNextVertices(int vertex, std::vector<std::pair<int, int>>& nextVertices) const {
    for (int i = 0; i < verticesNumber; ++i) {
        if (vertices[vertex][i] != 0) {
            nextVertices.push_back(std::make_pair(i, vertices[vertex][i]));
        }
    }
}

void DirectedGraph::GetPrevVertices(int vertex, std::vector<std::pair<int, int>>& prevVertices) const {
    for (int i = 0; i < verticesNumber; ++i) {
        if (vertices[i][vertex] != 0) {
            prevVertices.push_back(std::make_pair(i, vertices[i][vertex]));
        }
    }
}

class Dinitz {
public:
    Dinitz(DirectedGraph Graph) : network(Graph) {}
    int FindMaxFlow(int source, int sink);

private:
    DirectedGraph network;
    int dfs(DirectedGraph& net, int maxFlow, std::vector<bool>& visited, int currVert, int sink);
    int searchBlockingFlow(DirectedGraph& net, int source, int sink);
    bool buildLayerNet(DirectedGraph& net, int source, int sink);
};

int Dinitz::dfs(DirectedGraph& net, int maxFlow, std::vector<bool>& visited, int currVert, int sink) {
    std::vector<std::pair<int, int>> nextGen;
    net.GetNextVertices(currVert, nextGen);
    visited[currVert] = true;
    for (int i = 0; i < nextGen.size(); ++i) {
        if (visited[nextGen[i].first] == true) {
            continue;
        }
        if (nextGen[i].first == sink) {
            network.ChangeEdge(currVert, nextGen[i].first, std::min(maxFlow, nextGen[i].second) * (-1));
            network.ChangeEdge(nextGen[i].first, currVert, std::min(maxFlow, nextGen[i].second));
            net.ChangeEdge(currVert, nextGen[i].first, std::min(maxFlow, nextGen[i].second) * (-1));
            nextGen.clear();
            return (maxFlow < nextGen[i].second ? maxFlow : nextGen[i].second);
        }
        if (nextGen[i].second != 0) {
            int newFlow = dfs(net, std::min(maxFlow, nextGen[i].second), visited, nextGen[i].first, sink);
            if (newFlow != 0) {
                network.ChangeEdge(currVert, nextGen[i].first, newFlow * (-1));
                network.ChangeEdge(nextGen[i].first, currVert, newFlow);
                net.ChangeEdge(currVert, nextGen[i].first, newFlow * (-1));
                nextGen.clear();
                return newFlow;
            }
            net.ChangeEdge(currVert, nextGen[i].first, nextGen[i].second * (-1));
        }
    }
    return 0;
}

int Dinitz::searchBlockingFlow(DirectedGraph& net, int source, int sink) {
    int maxFlow = 0;
    while (true) {
        std::vector<bool> visited(network.VerticesCount(), false);
        int currFlow = dfs(net, inf, visited, source, sink);
        visited.clear();
        if (currFlow == 0) { // when there are no increasing flows any more, we have found a blocking flow
            return maxFlow;
        }
        maxFlow += currFlow;
    }
}

bool Dinitz::buildLayerNet(DirectedGraph& net, int source, int sink) {
    bool foundSink  = false;
    std::vector<int> layers(network.VerticesCount(), 0);
    std::queue<int> vert;
    vert.push(source);
    while (vert.empty() == false) {
        int currVert = vert.front();
        vert.pop();
        std::vector<std::pair<int, int>> nextGen;
        network.GetNextVertices(currVert, nextGen);
        for (int i = 0; i < nextGen.size(); ++i) {
            if (layers[nextGen[i].first] != 0 && layers[nextGen[i].first] != layers[currVert] + 1) {
                continue;
            }
            if (nextGen[i].first == sink) {
                foundSink = true;
            }
            layers[nextGen[i].first] = layers[currVert] + 1;
            vert.push(nextGen[i].first);
            net.AddEdge(currVert, nextGen[i].first, nextGen[i].second);
        }
    }
    layers.clear();
    return foundSink;
}

int Dinitz::FindMaxFlow(int source, int sink) {
    int flow = 0;
    while (true) {
        DirectedGraph net(network.VerticesCount()); // layer net
        if (buildLayerNet(net, source, sink) == false) { // if we cannot reach sink from source, algorithm finishes
            return flow;
        }
        flow += searchBlockingFlow(net, source, sink);
    }
}

int main() {
    int n, m;
    std::cin >> n >> m;
    DirectedGraph Graph(n);
    for (int i = 0; i < m; ++i) {
        int from, to, weight;
        std::cin >> from >> to >> weight;
        Graph.AddEdge(from - 1, to - 1, weight);
    }
    Dinitz MaxFlow(Graph);
    int answer = MaxFlow.FindMaxFlow(0, n - 1);
    std::cout << answer;
    return 0;
}
