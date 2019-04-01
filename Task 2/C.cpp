#include<iostream>
#include<queue>
#include<vector>
#include<set>
#include<map>
#include<utility>
#include<string>
#include<algorithm>

class FindSequenceOfMoves {
public:
    FindSequenceOfMoves(std::vector<int> StartState) : beginState(StartState), correctState({1, 2, 3, 4, 5, 6, 7, 8, 0}) {}
    bool SearchForCorrectState(std::string& answer);

private:
    std::vector<int> beginState;
    std::vector<int> correctState;
    std::map<std::vector<int>, std::string> ways;
    int findPos(std::vector<int>& currState);
    int countDisorders();
    void getNeighbors(std::vector<int>& currState, std::vector<std::vector<int>>& neighbors); // search all neighboring states
};

int FindSequenceOfMoves::findPos(std::vector<int>& currState) {
    for (int i = 0; i < 9; ++i) {
        if (currState[i] == 0) {
            return i;
        }
    }
}

int FindSequenceOfMoves::countDisorders() {
    int result = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = i + 1; j < 9; ++j) {
            if (beginState[i] > beginState[j]) {
                ++result;
            }
        }
    }
    return result;
}
//¿¿¿¿¿
void FindSequenceOfMoves::getNeighbors(std::vector<int>& currState, std::vector<std::vector<int>>& neighbors) {
    int pos = this -> findPos(currState);
    if (pos > 2) {
        std::vector<int> createdState = currState;
        std::swap(createdState[pos], createdState[pos - 3]);
        neighbors.push_back(createdState);
    }
    if (pos < 6) {
        std::vector<int> createdState = currState;
        std::swap(createdState[pos], createdState[pos + 3]);
        neighbors.push_back(createdState);
    }
    if (pos % 3 != 0) {
        std::vector<int> createdState = currState;
        std::swap(createdState[pos], createdState[pos - 1]);
        neighbors.push_back(createdState);
    }
    if (pos % 3 != 2) {
        std::vector<int> createdState = currState;
        std::swap(createdState[pos], createdState[pos + 1]);
        neighbors.push_back(createdState);
    }
}

bool FindSequenceOfMoves::SearchForCorrectState(std::string& answer) {
    std::queue<std::vector<int>> newStates;
    std::set<std::vector<int>> allStates;
    if ((countDisorders() + findPos(beginState)) % 2 != 0) {
        return false;
    }
    newStates.push(beginState);
    allStates.insert(beginState);
    ways.insert(std::make_pair(beginState, ""));
    std::vector<int> currState = newStates.front();
    while (newStates.empty() == false) {
        std::vector<int> currState = newStates.front(); // view next state
        newStates.pop();
        if (currState == correctState) {
            answer = ways[currState];
            return true;
        }
        std::vector<std::vector<int>> neighbors;
        getNeighbors(currState, neighbors);
        for (int i = 0; i < neighbors.size(); ++i) { // look through all neighboring (for a current one) states
            if (allStates.count(neighbors[i]) == 0) { // new found state shouldn't be in allStates as it will be added there
                newStates.push(neighbors[i]);
                allStates.insert(neighbors[i]);
                int posFrom = this -> findPos(currState);
                int posTo = this -> findPos(neighbors[i]);
                if (posTo - posFrom == 1) {
                    ways.insert(std::make_pair(neighbors[i], ways[currState] + "R")); // construct a letter sequence
                }
                if (posTo - posFrom == -1) {
                    ways.insert(std::make_pair(neighbors[i], ways[currState] + "L"));
                }
                if (posTo - posFrom == 3) {
                    ways.insert(std::make_pair(neighbors[i], ways[currState] + "D"));
                }
                if (posTo - posFrom == -3) {
                    ways.insert(std::make_pair(neighbors[i], ways[currState] + "U"));
                }
            }
        }
    }
}

int main() {
    std::vector<int> StartState(9);
    for (int i = 0; i < 9; ++i) {
        std::cin >> StartState[i];
    }
    FindSequenceOfMoves SolveBarleyBreak(StartState);
    std::string answer;
    bool IsPossible = SolveBarleyBreak.SearchForCorrectState(answer);
    if (IsPossible == true) {
        std::cout << answer.size() << std::endl;
        std::cout << answer;
    } else {
        std::cout << -1;
    }
    return 0;
}
