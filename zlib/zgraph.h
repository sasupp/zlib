#ifndef ZGRAPH_H
#define ZGRAPH_H

#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <queue>
#include <functional>

namespace ZLib {

namespace ZGraph {

using namespace std;

template<typename Node_t>
class IGraph {
public:
    virtual const vector<Node_t> Neighbors(const Node_t& n) const = 0;
    virtual int Weight(const Node_t& from, const Node_t& to) const = 0;
};

template<typename Node_t>
class Graph : public IGraph<Node_t> {

    map<Node_t, map<Node_t, int > > m_edgeMap;

public:
    void AddEdge(const Node_t& from, const Node_t& to, int w) {
        m_edgeMap[from][to] = w;
    }

    void RemoveEdge(const Node_t& from, const Node_t& to) {
        m_edgeMap[from].erase(to);
    }

    const vector<Node_t> Neighbors(const Node_t& n) const {
        vector<Node_t> result;
        auto nodeIt = m_edgeMap.find(n);
        if (nodeIt != m_edgeMap.end()) {
            for (auto it = (nodeIt->second).begin(); it != (nodeIt->second).end(); ++it) {
                result.push_back(it->first);
            }
        }
        return result;
    }

    int Weight(const Node_t& from, const Node_t& to) const {
        auto fromNodeIt = m_edgeMap.find(from);
        auto toNodeIt = (fromNodeIt->second).find(to);
        return toNodeIt->second;
    }
};

typedef pair<int, int > Cell_t;

class GridGraph : public IGraph<Cell_t> {
    vector< vector< int > > m_grid;
    int m_rowCount, m_colCount;
public:
    GridGraph(int r, int c) {
        m_rowCount = r;
        m_colCount = c;
        m_grid = vector< vector< int > >(r, vector<int >(c));
    }

    int& Cell(int r, int c) {
        return m_grid[r][c];
    }

    const int& Cell(int r, int c) const {
        return m_grid[r][c];
    }

    bool IsValid(int r, int c) const {
        return r >= 0 && r < m_rowCount && c >= 0 && c < m_colCount;
    }

    const vector<Cell_t> Neighbors(const Cell_t& n) const {
        vector<Cell_t> result;
        static int dr[4] = { 0, 1, 0, -1 };
        static int dc[4] = { 1, 0, -1, 0 };
        for (auto i = 0; i < 4; i++) {
            auto cell = make_pair(n.first + dr[i], n.second + dc[i]);
            if (IsValid(cell.first, cell.second)) {
                result.push_back(cell);
            }
        }
        return result;
    }

    int Weight(const Cell_t& from __attribute__((unused)),
               const Cell_t& to __attribute__((unused))) const {
        return 1;
    }
};

enum NodeState {
    Opened,
    Visited
};

template <typename Node_t >
void bfs(const IGraph<Node_t>& graph,
         const Node_t& start,
         function<bool(const Node_t&)> nodeVisitFunc = [](const Node_t) { return false; },
         function<bool(const Node_t&, const Node_t&)> nodeOpenFunc = [](const Node_t&, const Node_t&) { return false; }) {

    map<Node_t, NodeState > nodeStateMap;

    queue<Node_t> q;
    q.push(start);

    while (!q.empty()) {
        auto currentNode = q.front();
        q.pop();

        // mark visited
        nodeStateMap[currentNode] = Visited;

        // visit the node
        if (nodeVisitFunc(currentNode)) {
            return;
        }

        for (auto n : graph.Neighbors(currentNode)) {
            if (nodeStateMap.find(n) != nodeStateMap.end()) {
                // continue if already processed
                continue;
            }

            // mark opened
            nodeStateMap[n] = Opened;

            q.push(n);

            // Open visitor
            if (nodeOpenFunc(n, currentNode)) {
                return;
            }
        }
    }
}

template <typename Node_t >
void astar(const IGraph<Node_t>& graph,
           const Node_t& start,
           function<int(const Node_t&)> heuristicFunc,
           function<bool(const Node_t&)> nodeVisitFunc = [](const Node_t) { return false; },
           function<bool(const Node_t&, const Node_t&)> nodeOpenFunc = [](const Node_t&, const Node_t&) { return false; }) {

    typedef pair<int, Node_t > pqElement_t;
    priority_queue<pqElement_t, vector<pqElement_t>, greater<pqElement_t> > pq;

    unordered_map<Node_t, int > cost_so_far;
    cost_so_far[start] = 0;

    while (!pq.empty()) {
        auto current = pq.top().second;
        pq.pop();

        // call visitFunc
        if (nodeVisitFunc(current)) {
            return;
        }

        for (auto next : graph.Neighbors(current)) {
            int newCost = cost_so_far[current] + graph.Weight(current, next);
            if (!cost_so_far.count(next) || newCost < cost_so_far[next]) {
                cost_so_far[next] = newCost;
                int f = newCost + heuristicFunc(next);
                pq.push(f, next);

                if (nodeOpenFunc(next, current)) {
                    return;
                }
            }
        }
    }
}
}
}

#endif
