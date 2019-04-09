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
    FindSequenceOfMoves(int StartState) : startState(StartState), correctState(123456780) {}
    bool SearchForCorrectState(std::string& answer); // main solving function

private:
    int startState; // number for start state, filled upside down, from left to right
    int correctState; // number for correct state
    std::map<int, std::string> ways; // storage of state+way how this state was reached from the start (i.e. UDDL means you should move up, down, down and left from the start to reach the current state)
    int findPos(int currState); // returns a number of zero position (i.e. for correct state returns 8)
    int countDisorders(); // returns a number of disorders in the start state; this is necessary to figure out whether the start state is solvable
    void getNeighbors(int currState, std::vector<int>& neighbors); // search all neighboring states (in relation to the current one)
};

int FindSequenceOfMoves::findPos(int currState) {
    for (int i = 8; i >= 0; --i) {
        if (currState % 10 == 0) {
            return i;
        }
        currState /= 10;
    }
}

int FindSequenceOfMoves::countDisorders() {
    int result = 0, state = startState;
    std::vector<int> start;
    for (int i = 0; i < 9; ++i) {
        start.push_back(state % 10);
        state /= 10;
    }
    for (int i = 8; i >= 1; --i) {
        for (int j = i - 1; j >= 0; --j) {
            if (start[i] < start[j]) {
                ++result;
            }
        }
    }
    start.clear();
    return result;
}

void FindSequenceOfMoves::getNeighbors(int currState, std::vector<int>& neighbors) {
    int pos = this -> findPos(currState);
    int tenInPowerPos = 1;
    for (int i = 0; i < 8 - pos; ++i) {
        tenInPowerPos *= 10;
    }
    if (pos > 2) { // if zero is not in the top line, we can move it up
        int sufferedDigit = (currState / (tenInPowerPos * 1000)) % 10;
        neighbors.push_back(currState - sufferedDigit * tenInPowerPos * 999);
    }
    if (pos < 6) { // if zero is not in the bottom line, we can move it down
        int sufferedDigit = (currState / (tenInPowerPos / 1000)) % 10;
        neighbors.push_back(currState - sufferedDigit * (tenInPowerPos / 1000) + sufferedDigit * tenInPowerPos);
    }
    if (pos % 3 != 0) { // if zero is not in the left line, we can move it left
        int sufferedDigit = (currState / (tenInPowerPos * 10)) % 10;
        neighbors.push_back(currState - sufferedDigit * tenInPowerPos * 9);
    }
    if (pos % 3 != 2) { // if zero is not in the right line, we can move it right
        int sufferedDigit = (currState / (tenInPowerPos / 10)) % 10;
        neighbors.push_back(currState - sufferedDigit * (tenInPowerPos / 10) + sufferedDigit * tenInPowerPos);
    }
}

bool FindSequenceOfMoves::SearchForCorrectState(std::string& answer) { // main solving function
    std::queue<int> newStates; // queue for all noticed but unviewed states
    std::set<int> allStates; // set for all noticed states
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
        int currState = newStates.front(); // view next state
        newStates.pop();
        std::vector<int> neighbors;
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
    int StartState = 0;
    for (int i = 0; i < 9; ++i) {
        int value;
        std::cin >> value;
        StartState = StartState * 10 + value;
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
