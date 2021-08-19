#ifndef GAME_H
#define GAME_H

#include <cassert>
#include <iostream>
#include <vector>

class Chess {
public:
    bool hidden; // Whether this chess is hidden

    // The camp of current player
    //  1: Red
    //  0: Blue
    int camp;

    // The role of this chess
    //   0/10:  Flag(can be carried or not)(default is 10)
    //  1 ~ 9:  GongBing ~ SiLing
    //     -1:  Bomb
    //     -2:  Landmine
    int role;

    /// @param chess_camp The camp of current player
    /// @param chess_role The role of this chess
    /// @param is_hidden Whether this chess is hidden
    Chess(
        const int& chess_camp, const int& chess_role, const bool& is_hidden = 1)
        : camp { chess_camp }
        , role { chess_role }
        , hidden { is_hidden } { }

    /// @return -1, 0, 1 respectively represent cannot attack, perish together,
    /// able to beat
    int Attack(Chess* other);
};

class Node {
private:
    int x_, y_; // coordinate
    bool safe_; // whether this node is safe

public:
    Chess* chess;
    std::vector<int> road; // nodes linked with road

    // nodes linked with railway
    // 0 is vertical, 1 is horizontal
    std::vector<int> railway[2];

    /// @param x Row number
    /// @param y Column number
    /// @param safe Whether this node is safe
    /// @param which_chess Chess on this node
    Node(const int& x = 0, const int& y = 0, const bool& safe = 0,
        Chess* which_chess = nullptr)
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

    // Which player is play in this turn
    // 0: Own side
    // 1: Opposite
    int now_player_ = 0;

    // Array used in DFS. Whether this node has been visited.
    bool vis[60];

    /// Get accessible nodes on the railway via DFS
    /// @param now Current node
    /// @param direction 0 is vertical, 1 is horizontal
    /// @param type 1 represent GongBing which can turn on the railway
    /// @param start Start node
    std::vector<int> GetRailwayList(const int& now, const bool& direction,
        const bool& type, const int& start);

public:
    Node nodes[60]; // Nodes list
    int index[12][5]; // Number of node from Coordinate

    Game();
    Game(const Game& other) = delete;

    // Turn chess from hidden to shown
    void TurnOver(const int& x, const int& y);
    // Get accessible nodes list
    std::vector<int> GetList(const int& x, const int& y);
};

#endif // GAME_H
