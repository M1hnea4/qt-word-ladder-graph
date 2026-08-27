#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <vector>

template <typename T>
class Graph {
private:
    std::unordered_map<T, std::vector<T>> adjList;

public:
    void addEdge(const T& u, const T& v) {
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }

    std::vector<T> getNeighbors(const T& node) const {
        if (adjList.find(node) != adjList.end()) {
            return adjList.at(node);
        }
        return {};
    }

    bool contains(const T& node) const {
        return adjList.find(node) != adjList.end();
    }
};

#endif // GRAPH_H