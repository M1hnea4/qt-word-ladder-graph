# Word Ladder Solver & Game (Qt/C++) 🧗‍♂️

![C++](https://img.shields.io/badge/C%2B%2B-STL-00599C?style=for-the-badge&logo=c%2B%2B)
![Qt](https://img.shields.io/badge/Qt-GUI-41CD52?style=for-the-badge&logo=qt)

A complete C++ desktop application built with Qt that implements Lewis Carroll's classic Word Ladder puzzle. This project heavily utilizes Graph Theory, advanced STL containers, and algorithmic optimization to find the shortest transformation path between two words.

## 🧠 Algorithmic Core
* **Template Graph Data Structure:** Custom implementation of an unweighted graph using adjacency lists and C++ templates for generic data types.
* **Optimized Graph Construction:** Avoids the naive $O(N^2)$ comparison loop by using wildcard pattern matching (e.g., `*AME`) mapped via `std::map`, drastically reducing edge-building time.
* **Shortest Path (BFS):** Utilizes Breadth-First Search and `std::queue` to traverse the graph and reconstruct the optimal sequence of words using a parent-tracking array.

## 🖥️ UI & Analytics Features
* **Automatic Mode:** Instantly calculates and displays the optimal transformation path between a randomly selected start and target word.
* **Interactive Playing Mode:** Allows users to play the game with real-time validation, enforcing single-letter mutations against a valid dictionary.
* **Smart Hints:** Provides algorithmic suggestions on which letter to modify to reach the target fastest, using color-coded UI feedback.
* **Analytics Engine:** Tracks user performance (hints used, moves vs. optimal moves, unique words via `std::set`) and persistently stores session data in CSV format.

## 🛠️ File Structure
* `Graph.h`: Template-based generic graph data structure.
* `WordLadderLogic.h`: Core game rules, BFS algorithm implementation, and wildcard mapping logic.
* `mainwindow.h / .cpp / .ui`: Qt widgets, signal/slot mechanics, and visual layout configuration.
