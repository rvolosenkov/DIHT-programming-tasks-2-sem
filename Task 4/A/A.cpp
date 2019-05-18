#include<iostream>
#include<vector>
#include<utility>

int SecondMin(int a, int b, int c, int d) { // find second minimum among a, b, c, d (a < b and c < d if b != -1)
    if (b == -1) {
        return (a < c ? c : a);
    }
    if (b <= c) {
        return b;
    }
    if (a < c) {
        return c;
    }
    if (a == c) {
        if (b < d) {
            return b;
        } else {
            return d;
        }
    }
    if (a > c) {
        if (a <= d) {
            return a;
        } else {
            return d;
        }
    }
}

void BuildSparseTable(std::vector<std::vector<std::pair<int, int>>>& sparseTable, std::vector<int>& sequence) {
    std::vector<std::pair<int, int>> nextLayer;
    for (int i = 0; i < sequence.size(); ++i) {
        nextLayer.push_back(std::make_pair(sequence[i], -1));
    }
    sparseTable.push_back(nextLayer);
    int currLen = sequence.size() / 2;
    int twoInCurrentPower = 1;
    nextLayer.clear();
    for (int i = 1; currLen > 0; ++i) {
        for (int j = 0; j + twoInCurrentPower < sparseTable[i - 1].size(); ++j) {
            int newFirst = std::min(sparseTable[i - 1][j].first, sparseTable[i - 1][j + twoInCurrentPower].first);
            int newSecond = SecondMin(sparseTable[i - 1][j].first, sparseTable[i - 1][j].second, sparseTable[i - 1][j + twoInCurrentPower].first, sparseTable[i - 1][j + twoInCurrentPower].second);
            nextLayer.push_back(std::make_pair(newFirst, newSecond));
        }
        sparseTable.push_back(nextLayer);
        nextLayer.clear();
        currLen /= 2;
        twoInCurrentPower *= 2;
    }
}

void RMQ(std::vector<int>& sequence, std::vector<std::pair<int, int>>& ranges, std::vector<int>& answer) {
    std::vector<std::vector<std::pair<int, int>>> sparseTable; // pairs of first minimum and second minimum
    BuildSparseTable(sparseTable, sequence);
    int n = sequence.size();
    std::vector<int> logarithmOfBaseTwo(n);
    logarithmOfBaseTwo[0] = 0;
    int currNumber = 1;
    int twoInCurrentPower = 1;
    for (int i = 0; currNumber < n; ++i) {
        int currBorder = currNumber + twoInCurrentPower;
        for ( ; currNumber < std::min(currBorder, n); ++currNumber) {
            logarithmOfBaseTwo[currNumber] = i;
        }
        twoInCurrentPower *= 2;
    }
    std::vector<int> degreesOfTwo(logarithmOfBaseTwo[n - 1] + 1);
    degreesOfTwo[0] = 1;
    for (int i = 1; i < logarithmOfBaseTwo[n - 1] + 1; ++i) {
        degreesOfTwo[i] = degreesOfTwo[i - 1] * 2;
    }
    for (int i = 0; i < ranges.size(); ++i) {
        int k = logarithmOfBaseTwo[ranges[i].second - ranges[i].first];
        answer[i] = SecondMin(sparseTable[k][ranges[i].first].first, sparseTable[k][ranges[i].first].second, sparseTable[k][ranges[i].second - degreesOfTwo[k] + 1].first, sparseTable[k][ranges[i].second - degreesOfTwo[k] + 1].second);
    }
}

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<int> sequence(n);
    std::vector<std::pair<int, int>> ranges(m);
    for (int i = 0; i < n; ++i) {
        int newNumber;
        std::cin >> newNumber;
        sequence[i] = newNumber;
    }
    for (int i = 0; i < m; ++i) {
        int leftBorder, rightBorder;
        std::cin >> leftBorder >> rightBorder;
        ranges[i] = std::make_pair(leftBorder - 1, rightBorder - 1);
    }
    std::vector<int> answer(m);
    RMQ(sequence, ranges, answer);
    for (int i = 0; i < m; ++i) {
        std::cout << answer[i] << std::endl;
    }
    return 0;
}
