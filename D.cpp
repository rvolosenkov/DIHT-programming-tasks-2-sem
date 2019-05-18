#include<iostream>
#include<iomanip>

class ImplicitTreap {
public:
    ImplicitTreap() : root(nullptr) {}
    ~ImplicitTreap() {
        clearTree(root);
        root = nullptr;
    }

    void InsertAt(int position, const std::string& value);
    void DeleteAt(int position);
    std::string GetAt(int position);

private:
    struct Node{
        Node(const std::string& value);
        void fix(); // update a size of the tree
        size_t treeSize;
        int priority;
        std::string value;
        Node* left;
        Node* right;
    };

    std::pair<Node*, Node*> split(Node* node, int position);
    Node* merge(Node* node_1, Node* node_2);
    void clearTree(Node* node);

    Node* root;
};

void ImplicitTreap::clearTree(Node* node) {
    if (node == nullptr)
        return;
    clearTree(node -> left);
    clearTree(node -> right);
    delete node;
}

void ImplicitTreap::InsertAt(int position, const std::string& value) { // split tree by position and merge new node with right tree and then with left tree
    Node* currNode = new Node(value);
    std::pair<Node*, Node*> ans = split(root, position);
    Node* right = merge(currNode, ans.second);
    root = merge(ans.first, right);
}

void ImplicitTreap::DeleteAt(int position) { // split tree by position and then split the result to separate node (to remove) from the tree
    std::pair<Node*, Node*> ans1 = split(root, position);
    std::pair<Node*, Node*> ans2 = split(ans1.second, 1);
    delete ans2.first;
    root = merge(ans1.first, ans2.second);
}

std::string ImplicitTreap::GetAt(int position) { // going down the tree
    int currPosition;
    Node* node = root;
    do {
        currPosition = ((node -> left != nullptr) ? (node -> left -> treeSize) : 0);
        if (currPosition == position) {
            break;
        }
        if (position < currPosition) {
            node = node -> left;
        } else {
            position -= currPosition + 1;
            node = node -> right;
        }
    } while (true);
    return node -> value;
}

std::pair<ImplicitTreap::Node*, ImplicitTreap::Node*> ImplicitTreap::split(Node* node, int position) {
    if (node == nullptr) {
        return std::pair<Node*, Node*>(nullptr, nullptr);
    }
    int currPosition = ((node -> left != nullptr) ? (node -> left -> treeSize) : 0);
    if (currPosition >= position) {
        std::pair<Node*, Node*> ans = split(node -> left, position);
        node -> left = ans.second;
        node -> fix();
        return std::make_pair(ans.first, node);
    }
    else {
        std::pair<Node*, Node*> ans = split(node -> right, position - currPosition - 1);
        node -> right = ans.first;
        node -> fix();
        return std::make_pair(node, ans.second);
    }
}

ImplicitTreap::Node* ImplicitTreap::merge(Node* node_1, Node* node_2) {
    if (node_1 == nullptr || node_2 == nullptr) {
        return (node_1 == nullptr ? node_2 : node_1);
    }
    if (node_1 -> priority > node_2 -> priority) {
        node_1 -> right = merge(node_1 -> right, node_2);
        node_1 -> fix();
        return node_1;
    }
    else {
        node_2 -> left = merge(node_1, node_2 -> left);
        node_2 -> fix();
        return node_2;
    }
}

ImplicitTreap::Node::Node(const std::string& value) :
    value(value),
    priority(rand()),
    left(nullptr),
    right(nullptr),
    treeSize(1) {}

void ImplicitTreap::Node::fix() {
    treeSize = 1;
    if (left != nullptr)
        treeSize += left -> treeSize;
    if (right != nullptr)
        treeSize += right -> treeSize;
}

int main() {
    int k = 0;
    std::cin >> k;
    ImplicitTreap stringArray;
    for (int i = 0; i < k; i++) {
        char command;
        std::cin >> command;
        switch(command){
            case '+': {
                int position;
                std::string value;
                std::cin >> position >> value;
                stringArray.InsertAt(position, value);
                break;
            }
            case '-': {
                int position_1, position_2;
                std::cin >> position_1 >> position_2;
                for (int i = position_1; i <= position_2; ++i) {
                    stringArray.DeleteAt(i);
                }
                break;
            }
            case '?': {
                int position;
                std::cin >> position;
                std::cout << stringArray.GetAt(position) << std::endl;
                break;
            }
        }
    }
    return 0;
}
