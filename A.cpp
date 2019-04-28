#include<iostream>
#include<vector>
#include<list>
#include<set>
#include<queue>
#include<utility>
#include<algorithm>

const int inf = 1e9; // inf > max(n) * max(lenOfWay)

class UndirectedGraph {
public:
    UndirectedGraph(int verticesNumber_) : verticesNumber(verticesNumber_), vertices(verticesNumber_) {}
    void AddEdge(int from, int to, int weight);
    int VerticesCount() const;
    void GetNextVertices(int vertex, std::vector<std::pair<int, int>>& nextVertices) const;
    void GetPrevVertices(int vertex, std::vector<std::pair<int, int>>& prevVertices) const;

private:
    int verticesNumber;
    std::vector<std::list<std::pair<int, int>>> vertices;
};

void UndirectedGraph::AddEdge(int from, int to, int weight) {
    vertices[from].push_back(std::make_pair(to, weight));
    vertices[to].push_back(std::make_pair(from, weight));
}

int UndirectedGraph::VerticesCount() const {
    return verticesNumber;
}

void UndirectedGraph::GetNextVertices(int vertex, std::vector<std::pair<int, int>>& nextVertices) const {
    for (auto v : vertices[vertex]) {
        nextVertices.push_back(v);
    }
}

void UndirectedGraph::GetPrevVertices(int vertex, std::vector<std::pair<int, int>>& prevVertices) const {
    for (int v = 0; v < verticesNumber; ++v) {
        for (auto w : vertices[v]) {
            if (w.first == vertex) {
                prevVertices.push_back(std::make_pair(v, w.second));
            }
        }
    }
}

class Edge {
public:
    Edge(int From_, int To_, int Weight_) : From(From_), To(To_), Weight(Weight_) {}
    int From;
    int To;
    int Weight;
};

class DisjointSets {
public:
    DisjointSets(int Quantity);
    int GetLeader(int v);
    bool UnionSets(int u, int v);

private:
    int numberOfElements;
    std::vector<int> leader;
};

DisjointSets::DisjointSets(int Quantity) : numberOfElements(Quantity) {
    for (int i = 0; i < Quantity; ++i) {
        leader.push_back(i);
    }
}

int DisjointSets::GetLeader(int v) {
    if (v == leader[v]) {
        return v;
    }
    return leader[v] = GetLeader(leader[v]);
}

bool DisjointSets::UnionSets(int u, int v) {
    int leader_u = GetLeader(u), leader_v = GetLeader(v);
    if (leader_u == leader_v) {
        return false;
    }
    if (rand() % 2 == 0) {
        std::swap(leader_u, leader_v);
    }
    leader[leader_u] = leader_v;
    return true;
}

class Kruskal {
public:
    Kruskal(UndirectedGraph* Graph) : graph(Graph) {}
    int FindMSTWeight();

private:
    UndirectedGraph* graph;
    void searchEdges(std::vector<Edge>& edges);
};

bool comp(Edge First, Edge Second) { // compares 2 edges
    if (First.Weight < Second.Weight || First.Weight == Second.Weight && (First.From < Second.From || First.From == Second.From && First.To < Second.To)) {
        return true;
    }
    return false;
}

void Kruskal::searchEdges(std::vector<Edge>& edges) { // get all edges from this graph
    std::queue<int> vert;
    std::vector<bool> visited(graph -> VerticesCount(), false);
    vert.push(0);
    int currVertex = 0;
    while(vert.empty() == false) {
        int currVertex = vert.front();
        vert.pop();
        while (visited[currVertex] == true && vert.empty() == false) { // if all vertices have already been viewed, all edges have been found
            currVertex = vert.front();
            vert.pop();
            if (vert.empty() == true) {
                sort(edges.begin(), edges.end(), comp);
                return;
            }
        }
        visited[currVertex] = true;
        std::vector<std::pair<int, int>> nextGen;
        graph -> GetNextVertices(currVertex, nextGen);
        for (int i = 0; i < nextGen.size(); ++i) {
            if (visited[nextGen[i].first] == false) {
                Edge newEdge(currVertex, nextGen[i].first, nextGen[i].second);
                edges.push_back(newEdge);
                vert.push(nextGen[i].first);
            }
        }
    }
    sort(edges.begin(), edges.end(), comp); // sort all graph edges in ascending order
}

int Kruskal::FindMSTWeight() {
    std::vector<Edge> edges;
    searchEdges(edges);
    DisjointSets vertSets(graph -> VerticesCount());
    std::vector<Edge> edgesInMST;
    for (int i = 0; i < edges.size(); ++i) {
        if (vertSets.UnionSets(edges[i].From, edges[i].To) == true) {
            edgesInMST.push_back(edges[i]);
        }
    }
    int answer = 0;
    for (int i = 0; i < edgesInMST.size(); ++i) {
        answer += edgesInMST[i].Weight;
    }
    return answer;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    UndirectedGraph Graph(n);
    for (int i = 0; i < m; ++i) {
        int from, to, weight;
        std::cin >> from >> to >> weight;
        Graph.AddEdge(from - 1, to - 1, weight);
    }
    Kruskal MSTWeight(&Graph);
    int answer = MSTWeight.FindMSTWeight();
    std::cout << answer;
    return 0;
}
