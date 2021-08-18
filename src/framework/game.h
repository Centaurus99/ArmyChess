#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>

class Chess {
public:
    bool hiden; // Whether this chess is hiden

    // The camp of current player
    //  1: Red
    //  0: Blue
    int camp;

    // The role of this chess
    //      0:  Flag
    //  1 ~ 9:  GongBing ~ SiLing
    //     -1:  Bomb
    //     -2:  Landmine
    int role;

    /// @param chess_camp The camp of current player
    /// @param chess_role The role of this chess
    /// @param is_hiden Whether this chess is hiden
    Chess(const int& chess_camp, const int& chess_role, const bool& is_hiden = 1)
        : camp { chess_camp }
        , role { chess_role }
        , hiden { is_hiden } { }
};

class Node {
private:
    int x_, y_; // coordinate
    bool safe_; // whether this node is safe

public:
    Chess* chess;
    std::vector<int> road; // nodes linked with road
    std::vector<int> railway_vertical; // nodes linked with vertical railway
    std::vector<int> railway_horizontal; // nodes linked with horizontal railway

    /// @param x Row number
    /// @param y Column number
    /// @param safe Whether this node is safe
    /// @param which_chess Chess on this node
    Node(const int& x = 0, const int& y = 0, const bool& safe = 0, Chess* which_chess = nullptr)
        : x_ { x }
        , y_ { y }
        , safe_ { safe }
        , chess { which_chess } { }

    Node(const Node& other) = delete;
    ~Node() { delete chess; }

    void setCoordinate(const int& x, const int& y) { x_ = x, y_ = y_; }
    void setSafe(const bool& safe) { safe_ = safe; }

    bool isSafe() { return safe_; }
};

class Game {
private:
    // Get number of node in the upper half from Coordinate
    inline int Index(const int& x, const int& y) { return 5 * x + y; }

    // Initialize the node connection
    void InitGraph();
    // Initialize the chess on the chessboard
    void InitChess();

public:
    Node nodes[60]; // Nodes list
    int index[12][5]; // Number of node from Coordinate
    Game();
};

#endif // GAME_H
