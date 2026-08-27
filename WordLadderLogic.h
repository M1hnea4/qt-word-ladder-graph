#ifndef WORDLADDERLOGIC_H
#define WORDLADDERLOGIC_H

#include "Graph.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <cstdlib>

class WordLadderLogic {
private:
    Graph<std::string> graph;
    std::vector<std::string> allWords;

public:
    void buildGraph(const std::vector<std::string>& dictionary) {
        allWords = dictionary;
        std::unordered_map<std::string, std::vector<std::string>> patternMap;
        for (const std::string& word : dictionary) {
            for (size_t i = 0; i < word.length(); ++i) {
                std::string pattern = word;
                pattern[i] = '*';
                patternMap[pattern].push_back(word);
            }
        }
        for (const auto& pair : patternMap) {
            const std::vector<std::string>& words = pair.second;
            for (size_t i = 0; i < words.size(); ++i) {
                for (size_t j = i + 1; j < words.size(); ++j) {
                    graph.addEdge(words[i], words[j]);
                }
            }
        }
    }

    bool isValidWord(const std::string& word) const {
        return graph.contains(word);
    }

    std::string getRandomWord() const {
        if (allWords.empty()) return "";
        return allWords[rand() % allWords.size()];
    }

    std::vector<std::string> findShortestPath(const std::string& start, const std::string& target) const {
        if (!graph.contains(start) || !graph.contains(target)) return {};

        std::queue<std::string> q;
        std::unordered_map<std::string, std::string> parent;
        std::unordered_map<std::string, bool> visited;

        q.push(start);
        visited[start] = true;
        bool found = false;

        while (!q.empty()) {
            std::string current = q.front();
            q.pop();

            if (current == target) {
                found = true;
                break;
            }
            for (const std::string& neighbor : graph.getNeighbors(current)) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    parent[neighbor] = current;
                    q.push(neighbor);
                }
            }
        }

        std::vector<std::string> path;
        if (found) {
            for (std::string step = target; !step.empty(); step = parent[step]) {
                path.push_back(step);
                if (step == start) break;
            }
            std::reverse(path.begin(), path.end());
        }
        return path;
    }
};

#endif // WORDLADDERLOGIC_H