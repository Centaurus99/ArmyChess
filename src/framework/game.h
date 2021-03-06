#ifndef GAME_H
#define GAME_H

#include <cassert>
#include <exception>
#include <iostream>
#include <vector>

class Class;
class Node;
class Game;

class Chess {
private:
    // The Game this chess belong to
    Game* game_;
    // The Node this chess belong to
    Node* node_;

public:
    bool hidden; // Whether this chess is hidden

    // The camp of this chess
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
    /// @param game The Game this chess belong to
    /// @param node The Node this chess belong to
    /// @param is_hidden Whether this chess is hidden
    Chess(const int& chess_camp, const int& chess_role, Game* game = nullptr,
        Node* node = nullptr, const bool& is_hidden = 1);

    // Delete chess from the game
    void Delete();

    void setNode(Node* node) { node_ = node; }

    // Whether this chess can be turned over or moved
    bool isMovable();

    /// @return -1, 0, 1 respectively represent
    /// cannot attack, perish together, able to beat
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

    void setCoordinate(const int& x, const int& y) { x_ = x, y_ = y; }
    void setSafe(const bool& safe) { safe_ = safe; }
    void setChess(Chess* new_chess) {
        chess = new_chess;
        chess->setNode(this);
    }

    bool isSafe() { return safe_; }
    int x() { return x_; }
    int y() { return y_; }

    // Delete the chess (if exist) on this node from game
    void DeleteChess();
};

class Game {
private:
    // Get number of node in the upper half from Coordinate
    inline int Index(const int& x, const int& y) { return 5 * x + y; }

    // Initialize the node connection
    void InitGraph();
    // Initialize the chess on the chessboard
    void InitChess();

    // Flags of both sides
    // 1: Red
    // 0: Blue
    // Set in InitChess
    Chess* flag_[2];

    // The remaining number of corresponding roles
    // Add 2 to the index when in use
    // Initialed in InitChess
    int role_count_[2][13];

    // 0: Own side
    // 1: Opposite
    // Initialed in InitChess
    int start_player_;
    int current_player_;

    // Which camp own side belong to
    //  1: Red
    //  0: Blue
    // -1: Undecided
    int own_camp_ = -1;

    // Which camp the player turn over in the last time
    // Index:
    //      0: Own side
    //      1: Opposite
    // Value:
    //      1: Red
    //      0: Blue
    //     -1: Undecided
    int last_take_[2] = { -1, -1 };

    // Which camp win the game
    //  1: Red
    //  0: Blue
    // -1: Undecided
    int winner_ = -1;

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

    // Get accessible nodes list
    std::vector<int> GetList(const int& number);

    // Change the remaining number of corresponding roles
    void CountRole(const int& camp, const int& role, const int& value);

    // Whether player can operate
    bool Movable(const int& player);

    // Turn chess from hidden to shown
    void TurnOver(const int& number);

    // Capture and delete/move related chess
    void Capture(const int& now, const int& to);

    // One player win the game
    void SetWinner(const int& winner);

    int GetStartPlayer() { return start_player_; }
    int GetCurrentPlayer() { return current_player_; }
    int GetOwnCamp() { return own_camp_; }
    int GetWinner() { return winner_; }

    // Game mantain function
    void BeforeTurn();
    void AfterTurn();

    // Conversion with binary packages
    std::string ExportToByte();
    void LoadFromByte(const std::string& data);
};

#endif // GAME_H
