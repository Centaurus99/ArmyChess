#ifndef GAME_H
#define GAME_H

#include <vector>

class Node {
private:
    int x_, y_; // coordinate
    int type_; // the type of chess on this node
    bool safe_; // whether this node is safe

public:
    std::vector<int> road; // nodes linked with road
    std::vector<int> railway_vertical; // nodes linked with vertical railway
    std::vector<int> railway_horizontal; // nodes linked with horizontal railway

    /// @param x Row number
    /// @param y Column number
    /// @param type Chess type
    /// @param safe Whether this node is safe
    Node(const int& x = 0, const int& y = 0, const int& type = 0, const bool& safe = 0)
        : x_ { x }
        , y_ { y }
        , type_ { type }
        , safe_ { safe } { }

    Node(const Node& other) = delete;

    void setCoordinate(const int& x, const int& y) { x_ = x, y_ = y_; }
    void setType(const int& type) { type_ = type; }
    void setSafe(const bool& safe) { safe_ = safe; }

    bool isSafe() { return safe_; }
};

class Game {
private:
public:
    Node nodes[60]; // Nodes list
    int index[12][5]; // Number of node from Coordinate
    Game();
};

#endif // GAME_H
