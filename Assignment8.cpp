//Baliram Hallale
//PRN: 123B1F027
//Date : 06-10-2025

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;

const int INF = 9999999;

class Node {
public:
    int level;             
    int pathCost;          
    int reducedCost;       
    int vertex;            
    vector<int> path;    
    vector<vector<int>> reducedMatrix; 

    Node(int n) {
        reducedMatrix.resize(n, vector<int>(n));
        path.clear();
    }

    // Comparator for priority queue (min-heap based on reducedCost)
    bool operator>(const Node& other) const {
        return this->reducedCost > other.reducedCost;
    }
};

void copyMatrix(const vector<vector<int>>& src, vector<vector<int>>& dest) {
    int n = src.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

int reduceMatrix(vector<vector<int>>& matrix, int n) {
    int reductionCost = 0;

    // Row reduction
    for (int i = 0; i < n; i++) {
        int rowMin = INF;
        for (int j = 0; j < n; j++)
            if (matrix[i][j] < rowMin)
                rowMin = matrix[i][j];

        if (rowMin != INF && rowMin != 0) {
            reductionCost += rowMin;
            for (int j = 0; j < n; j++)
                if (matrix[i][j] != INF)
                    matrix[i][j] -= rowMin;
        }
    }

    // Column reduction
    for (int j = 0; j < n; j++) {
        int colMin = INF;
        for (int i = 0; i < n; i++)
            if (matrix[i][j] < colMin)
                colMin = matrix[i][j];

        if (colMin != INF && colMin != 0) {
            reductionCost += colMin;
            for (int i = 0; i < n; i++)
                if (matrix[i][j] != INF)
                    matrix[i][j] -= colMin;
        }
    }
    return reductionCost;
}

Node createNode(const vector<vector<int>>& parentMatrix, const vector<int>& path, 
                int level, int i, int j, int n) {
    Node node(n);
    copyMatrix(parentMatrix, node.reducedMatrix);

    // Set outgoing edges from i to INF
    for (int k = 0; level != 0 && k < n; k++)
        node.reducedMatrix[i][k] = INF;

    // Set incoming edges to j to INF
    for (int k = 0; k < n; k++)
        node.reducedMatrix[k][j] = INF;

    // Prevent returning to start city prematurely
    if (level + 1 < n)
        node.reducedMatrix[j][0] = INF;

    node.path = path;
    node.path.push_back(j);
    node.level = level;
    node.vertex = j;
    return node;
}

void solveTSP(vector<vector<int>>& costMatrix, int n) {
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    vector<int> path;
    path.push_back(0);

    Node root(n);
    copyMatrix(costMatrix, root.reducedMatrix);

    root.path = path;
    root.level = 0;
    root.vertex = 0;
    root.pathCost = 0;
    root.reducedCost = reduceMatrix(root.reducedMatrix, n);

    pq.push(root);
    int minCost = INF;
    vector<int> finalPath;

    while (!pq.empty()) {
        Node minNode = pq.top();
        pq.pop();
        int i = minNode.vertex;

        if (minNode.level == n - 1) {
            // Complete the tour by returning to start
            minNode.path.push_back(0);
            int totalCost = minNode.pathCost + costMatrix[i][0];
            if (totalCost < minCost) {
                minCost = totalCost;
                finalPath = minNode.path;
            }
            continue;
        }

        for (int j = 0; j < n; j++) {
            if (minNode.reducedMatrix[i][j] != INF) {
                Node child = createNode(minNode.reducedMatrix, minNode.path, 
                                       minNode.level + 1, i, j, n);
                child.pathCost = minNode.pathCost + costMatrix[i][j];
                child.reducedCost = child.pathCost + reduceMatrix(child.reducedMatrix, n);
                pq.push(child);
            }
        }
    }

    cout << "\nOptimal Delivery Route (SwiftShip): ";
    for (int i = 0; i < finalPath.size(); i++) {
        cout << finalPath[i];
        if (i < finalPath.size() - 1) cout << " ";
    }
    cout << endl;
    cout << "Minimum Total Delivery Cost: " << minCost << endl;
}

int main() {
    int n;
    cout << "Enter number of cities: ";
    cin >> n;

    vector<vector<int>> costMatrix(n, vector<int>(n));
    cout << "Enter cost matrix (use large number for no direct route):" << endl;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> costMatrix[i][j];

    solveTSP(costMatrix, n);
    
    return 0;

}
