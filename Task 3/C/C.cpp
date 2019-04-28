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
    void Print();

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

void DirectedGraph::Print() {
    std::cout << verticesNumber << std::endl;
    for (int i = 0; i < verticesNumber; ++i) {
        for (int j = 0; j < verticesNumber; ++j) {
            std::cout << vertices[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

class SearchMaxFlow { // Dinitz algorithm as in B
public:
    SearchMaxFlow(DirectedGraph Graph) : network(Graph) {}
    int FindMaxFlow(int source, int sink);

private:
    DirectedGraph network;
    int dfs(DirectedGraph& net, int maxFlow, std::vector<bool>& visited, int currVert, int sink);
    int searchBlockingFlow(DirectedGraph& net, int source, int sink);
    bool buildLayerNet(DirectedGraph& net, int source, int sink);
};

int SearchMaxFlow::dfs(DirectedGraph& net, int maxFlow, std::vector<bool>& visited, int currVert, int sink) {
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

int SearchMaxFlow::searchBlockingFlow(DirectedGraph& net, int source, int sink) {
    int maxFlow = 0;
    while (true) {
        std::vector<bool> visited(network.VerticesCount(), false);
        int currFlow = dfs(net, inf, visited, source, sink);
        visited.clear();
        if (currFlow == 0) {
            return maxFlow;
        }
        maxFlow += currFlow;
    }
}

bool SearchMaxFlow::buildLayerNet(DirectedGraph& net, int source, int sink) {
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

int SearchMaxFlow::FindMaxFlow(int source, int sink) {
    int flow = 0;
    while (true) {
        DirectedGraph net(network.VerticesCount());
        if (buildLayerNet(net, source, sink) == false) {
            return flow;
        }
        flow += searchBlockingFlow(net, source, sink);
    }
}

int CountDays(DirectedGraph Graph, int rightBorder, int employeesNumber) {
    int leftBorder = 0;
    int oldMid = 0, newMid = 0;
    while (leftBorder < rightBorder - 1) { // bin search (borders, mid = amounts of days)
        oldMid = newMid;
        newMid = (leftBorder + rightBorder) / 2;
        for (int i = 1; i <= employeesNumber; ++i) {
            Graph.ChangeEdge(0, i, newMid - oldMid); // edges 'source - employee' fill with a current number of days
        }
        SearchMaxFlow MaxFlow(Graph);
        int daysAmount = MaxFlow.FindMaxFlow(0, Graph.VerticesCount() - 1);
        if (daysAmount < employeesNumber * newMid) { // check whether all employees can drink tea for a current number of days (=newMid)
            rightBorder = newMid;
        } else {
            leftBorder = newMid;
        }
    }
    return leftBorder;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    DirectedGraph Graph(n + m + 2); // create a graph with fictitious source and sink (vertices from 1 to n are employees, vertices from n+1 to n+m are sorts of tea)
    int sumNumber = 0;
    for (int i = 1; i <= m; ++i) {
        int numberOfTeaBags;
        std::cin >> numberOfTeaBags;
        sumNumber += numberOfTeaBags; // count total amount of tea bags (it is needed for the right border of bin search)
        Graph.AddEdge(n + i, n + m + 1, numberOfTeaBags); // edges 'sort of tea - sink' fill with an amount of tea bags
    }
    for (int i = 1; i <= n; ++i) {
        Graph.AddEdge(0, i, 0); // edges 'source - employee' are needed for amounts of days
        int numberOfFavouriteSorts;
        std::cin >> numberOfFavouriteSorts;
        for (int j = 0; j < numberOfFavouriteSorts; ++j) {
            int favouriteSort;
            std::cin >> favouriteSort;
            Graph.AddEdge(i, n + favouriteSort, inf); // edges 'employee - sort of tea' fill with inf since we need them only to connect employees with their favourite sorts of tea
        }
    }
    int answer = CountDays(Graph, sumNumber / n + 1, n); // a number of days cannot exceed an amount of tea bags divided by a number of employees
    std::cout << answer;
    return 0;
}
