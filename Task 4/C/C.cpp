#include<iostream>
#include<vector>
#include<utility>
#include<cmath>

class Requests {
public:
    Requests(long long a_1, long long a_2, long long x, long long y, long long z, long long n) : a_1_(a_1), a_2_(a_2), x_(x), y_(y), z_(z), n_(n) {}
    std::pair<long long, long long> GetNextRequest(long long lastResult);

private:
    long long a_1_;
    long long a_2_;
    long long x_;
    long long y_;
    long long z_;
    long long n_;
};

std::pair<long long, long long> Requests::GetNextRequest(long long lastResult) {
    std::pair<long long, long long> answer = std::make_pair((a_1_ + lastResult) % n_, a_2_);
    a_1_ = (a_1_ * x_ + a_2_ * y_ + z_) % n_;
    a_2_ = (a_2_ * x_ + a_1_ * y_ + z_) % n_;
    return answer;
}

class LCA {
public:
    LCA(std::vector<std::vector<long long>>& Parents);
    long long GetResult(std::pair<long long, long long> Request);

private:
    long long numberOfVertices;
    long long logNumberOfVertices;
    long long timer;
    std::vector<long long> timeIn;
    std::vector<long long> timeOut;
    std::vector<std::vector<long long>>& treeParents; // treeParents[i] - vector for descendants of the vertex i
    std::vector<std::vector<long long>> ancestors; // ancestors[v][i] = ancestor for v in 2^i steps (for the root: ancestor = root)

    bool isAncestor(long long a, long long b); // check if a is an ancestor for b
    void dfs(long long vertex, long long parent);
};

LCA::LCA(std::vector<std::vector<long long>>& Parents) :
    treeParents(Parents),
    numberOfVertices(Parents.size()),
    logNumberOfVertices(ceil(log2(Parents.size()))),
    ancestors(numberOfVertices,
    std::vector<long long>(logNumberOfVertices + 1)),
    timeIn(Parents.size()),
    timeOut(Parents.size()),
    timer(0) {
        dfs(0, 0);
    }

long long LCA::GetResult(std::pair<long long, long long> Request) {
    long long a = Request.first;
    long long b = Request.second;
    if (isAncestor(a, b) == true) {
        return a;
    }
    if (isAncestor(b, a) == true) {
        return b;
    }
    for (long long i = logNumberOfVertices; i >= 0; --i) {
        if (!isAncestor(ancestors[a][i], b)) {
            a = ancestors[a][i];
        }
    }
    return ancestors[a][0];
}

bool LCA::isAncestor(long long a, long long b) {
    return timeIn[a] <= timeIn[b] && timeOut[b] <= timeOut[a];
}

void LCA::dfs(long long vertex, long long parent) {
    timeIn[vertex] = timer++;
    ancestors[vertex][0] = parent;
    for (long long i = 1; i <= logNumberOfVertices; i++)
        ancestors[vertex][i] = ancestors[ancestors[vertex][i - 1]][i - 1];
    for (int i = 0; i < treeParents[vertex].size(); ++i) {
        long long to = treeParents[vertex][i];
        if (to != parent)
            dfs(to, vertex);
    }
    timeOut[vertex] = timer++;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<long long>> Parents(n); // vector Parents[i] is for descendants of the vertex i
    for (long long i = 1; i < n; ++i) {
        long long currParent;
        std::cin >> currParent;
        Parents[currParent].push_back(i);
    }
    LCA solveLCA(Parents);
    long long a_1, a_2, x, y, z;
    std::cin >> a_1 >> a_2 >> x >> y >> z;
    Requests RequestReceiver(a_1, a_2, x, y, z, n);
    long long answer = 0;
    long long currResult = 0;
    for (long long i = 0; i < m; ++i) {
        currResult = solveLCA.GetResult(RequestReceiver.GetNextRequest(currResult));
        answer += currResult;
    }
    std::cout << answer;
    return 0;
}
