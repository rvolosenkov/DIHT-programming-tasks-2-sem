#include<iostream>
#include<queue>
#include<vector>
#include<set>
#include<map>
#include<utility>
#include<string>
#include<algorithm>

class FindSequenceOfMoves { // solver
public:
    FindSequenceOfMoves(std::vector<int> StartState) : startState(StartState), correctState({1, 2, 3, 4, 5, 6, 7, 8, 0}) {}
    bool SearchForCorrectState(std::string& answer); // main solving function

private:
    std::vector<int> startState; // vector for start state, filled upside down, from left to right
    std::vector<int> correctState; // vector for correct state
    std::map<std::vector<int>, std::string> ways; // storage of state+way how this state was reached from the start (i.e. UDDL means you should move up, down, down and left from the start to reach the current state)
    int findPos(std::vector<int>& currState); // returns a number of zero position (i.e. for correct state returns 8)
    int countDisorders(); // returns a number of disorders in the start state; this is necessary to figure out whether the start state is solvable
    void getNeighbors(std::vector<int>& currState, std::vector<std::vector<int>>& neighbors); // search all neighboring states (in relation to the current one)
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
            if (startState[i] > startState[j]) {
                ++result;
            }
        }
    }
    return result;
}

void FindSequenceOfMoves::getNeighbors(std::vector<int>& currState, std::vector<std::vector<int>>& neighbors) {
    int pos = this -> findPos(currState);
    if (pos > 2) { // if zero is not in the top line, we can move it up
        std::vector<int> createdState = currState;
        std::swap(createdState[pos], createdState[pos - 3]);
        neighbors.push_back(createdState);
    }
    if (pos < 6) { // if zero is not in the bottom line, we can move it down
        std::vector<int> createdState = currState;
        std::swap(createdState[pos], createdState[pos + 3]);
        neighbors.push_back(createdState);
    }
    if (pos % 3 != 0) { // if zero is not in the left line, we can move it left
        std::vector<int> createdState = currState;
        std::swap(createdState[pos], createdState[pos - 1]);
        neighbors.push_back(createdState);
    }
    if (pos % 3 != 2) { // if zero is not in the right line, we can move it right
        std::vector<int> createdState = currState;
        std::swap(createdState[pos], createdState[pos + 1]);
        neighbors.push_back(createdState);
    }
}

bool FindSequenceOfMoves::SearchForCorrectState(std::string& answer) { // main solving function
    std::queue<std::vector<int>> newStates; // queue for all noticed but unviewed states
    std::set<std::vector<int>> allStates; // set for all noticed states
    if ((countDisorders() + findPos(startState)) % 2 != 0) { // check an ability to solve
        return false;
    }
    if (startState == correctState) { // check if a start state is correct
        return true;
    }
    newStates.push(startState);
    allStates.insert(startState);
    ways.insert(std::make_pair(startState, "")); // start state don't require some moves to reach the same one => a string is empty
    while (newStates.empty() == false) {
        std::vector<int> currState = newStates.front(); // view next state
        newStates.pop();
        std::vector<std::vector<int>> neighbors;
        getNeighbors(currState, neighbors); // find all neighbors of the current state
        for (int i = 0; i < neighbors.size(); ++i) { // look through all neighboring states
            if (allStates.count(neighbors[i]) == 0) { // work with this neighboring state in case it has not been viewed yet
                newStates.push(neighbors[i]); // push this state to noticed ones
                allStates.insert(neighbors[i]);
                int posFrom = this -> findPos(currState);
                int posTo = this -> findPos(neighbors[i]); // compare old and new zero positions
                if (posTo - posFrom == 1) {
                    ways.insert(std::make_pair(neighbors[i], ways[currState] + "R")); // construct a way to the new state
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
                if (neighbors[i] == correctState) { // check if a reached state is correct
                    answer = ways[neighbors[i]];
                    for ( ; newStates.empty() == false; newStates.pop()); // clear queue
                    allStates.clear(); // clear set
                    ways.clear(); // clear map
                    return true;
                }
            }
        }
        ways.erase(startState); // clear a way to the current state as now it is useless
    }
}

int main() {
    std::vector<int> StartState;
    for (int i = 0; i < 9; ++i) {
        int value;
        std::cin >> value;
        StartState.push_back(value);
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
