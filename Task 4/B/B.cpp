#include<iostream>
#include<vector>
#include<utility>

struct Node {
    int maxNumber; // the greatest number of '1' standing in a row in the current subsegment
    int leftNumber; // a number of '1' standing in a row from the left side of the subsegment (0 if the first element of subsegment is 0)
    int rightNumber; // the same for the right side
};

int BuildSegmentTree(std::vector<int>& Segment, std::vector<Node>& Tree, int currVertex, int& currElem, int segmentSize) {
    if (segmentSize == 1) {
        Tree[currVertex].maxNumber = Tree[currVertex].leftNumber = Tree[currVertex].rightNumber = Segment[currElem++];
        return Tree[currVertex].maxNumber;
    }
    int leftSegmentSize = segmentSize / 2 + segmentSize % 2;
    int rightSegmentSize = segmentSize / 2;
    BuildSegmentTree(Segment, Tree, 2 * currVertex + 1, currElem, leftSegmentSize);
    BuildSegmentTree(Segment, Tree, 2 * currVertex + 2, currElem, rightSegmentSize);
    Tree[currVertex].maxNumber = std::max(std::max(Tree[2 * currVertex + 1].maxNumber, Tree[2 * currVertex + 2].maxNumber), Tree[2 * currVertex + 1].rightNumber + Tree[2 * currVertex + 2].leftNumber);
    Tree[currVertex].leftNumber = (Tree[2 * currVertex + 1].leftNumber == leftSegmentSize ? Tree[2 * currVertex + 1].leftNumber + Tree[2 * currVertex + 2].leftNumber : Tree[2 * currVertex + 1].leftNumber);
    Tree[currVertex].rightNumber = (Tree[2 * currVertex + 2].rightNumber == rightSegmentSize ? Tree[2 * currVertex + 2].rightNumber + Tree[2 * currVertex + 1].rightNumber : Tree[2 * currVertex + 2].rightNumber);
    return Tree[currVertex].maxNumber;
}

void SequencesOfUnits(std::vector<int>& Array, std::vector<std::pair<int, int>>& Requests, std::vector<int>& Answer) {
    const int numberOfRequests = Requests.size();
    for (int i = 0; i < numberOfRequests; ++i) {
        int numberOfElements = Requests[i].second - Requests[i].first + 1;
        std::vector<int> currSegment(numberOfElements);
        for (int j = 0; j < numberOfElements; ++j) {
            currSegment[j] = Array[j + Requests[i].first];
        }
        std::vector<Node> Tree(2 * numberOfElements); // the number of vertices in segment tree cannot exceed 2 * number of elements in the initial array
        int currSize = 0;
        int currAnswer = BuildSegmentTree(currSegment, Tree, 0, currSize, numberOfElements);
        Answer[i] = currAnswer;
    }
}

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<int> Array(n);
    for (int i = 0; i < n; ++i) {
        int elem;
        std::cin >> elem;
        Array[i] = elem;
    }
    std::vector<std::pair<int, int>> Requests(m);
    for (int i = 0; i < m; ++i) {
        int left, right;
        std::cin >> left >> right;
        Requests[i] = std::make_pair(left, right);
    }
    std::vector<int> Answer(m);
    SequencesOfUnits(Array, Requests, Answer);
    for (int i = 0; i < m; ++i) {
        std::cout << Answer[i] << std::endl;
    }
    return 0;
}
