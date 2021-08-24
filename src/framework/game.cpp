#include "game.h"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

Chess::Chess(const int& chess_camp, const int& chess_role, Game* game,
    Node* node, const bool& is_hidden)
    : camp { chess_camp }
    , role { chess_role }
    , game_ { game }
    , node_ { node }
    , hidden { is_hidden } {
    game_->CountRole(camp, role, 1);
}

void Chess::Delete() {
    game_->CountRole(camp, role, -1);
    if (role == 0)
        game_->SetWinner(camp ^ 1 ^ game_->GetOwnCamp());
}

bool Chess::isMovable() {
    if (hidden)
        return 1;
    if (game_->GetOwnCamp() == -1)
        return 0;
    if (camp == game_->GetCurrentPlayer() ^ game_->GetOwnCamp()) {
        if (role != -2 && role != 0 && role != 10)
            return 1;
    }
    return 0;
}

int Chess::Attack(Chess* other) {
#ifdef DEBUG
    assert(role != 10);
    assert(role != 0);
    assert(role != -2);
#endif
    if (other == nullptr)
        return 1;
    if (other->hidden)
        return -1;
    if (camp == other->camp)
        return -1;
    if (other->node_->isSafe())
        return -1;
    if (role == -1 || other->role == -1)
        return 0;
    if (other->role == -2) {
        if (role == 1)
            return 1;
        return -1;
    }
    return other->role < role ? 1 : (other->role == role ? 0 : -1);
}

void Node::DeleteChess() {
    if (chess != nullptr) {
        chess->Delete();
        delete chess;
        chess = nullptr;
    }
}

void Game::InitGraph() {
    // The number of safe nodes in the upper half
    std::vector<int> SAFE_NODE = { 11, 13, 17, 21, 23 };
    // Set safe nodes
    for (auto x : SAFE_NODE) {
        nodes[x].setSafe(true);
        nodes[x + 30].setSafe(true);
    }

    // Set coordinate as follows
    //  0,  1, ...,  4
    //  :   :        :
    // 25, 26, ..., 29
    // ---------------
    // 59, 58, ..., 55
    //  :   :        :
    // 34, 33, ..., 30
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 5; ++j) {
            index[i][j] = Index(i, j);
            index[11 - i][4 - j] = Index(i, j) + 30;
            nodes[Index(i, j)].setCoordinate(i, j);
            nodes[Index(i, j) + 30].setCoordinate(11 - i, 4 - j);
        }
    }

    // Set edge
    for (int j = 0; j < 5; ++j) {
        if (j != 4) {
            nodes[index[0][j]].road.push_back(index[0][j + 1]);
            nodes[index[0][j] + 30].road.push_back(index[0][j + 1] + 30);
            nodes[index[1][j]].railway[1].push_back(index[1][j + 1]);
            nodes[index[1][j] + 30].railway[1].push_back(index[1][j + 1] + 30);
            nodes[index[5][j]].railway[1].push_back(index[5][j + 1]);
            nodes[index[5][j] + 30].railway[1].push_back(index[5][j + 1] + 30);
        }
        if (j != 0) {
            nodes[index[0][j]].road.push_back(index[0][j - 1]);
            nodes[index[0][j] + 30].road.push_back(index[0][j - 1] + 30);
            nodes[index[1][j]].railway[1].push_back(index[1][j - 1]);
            nodes[index[1][j] + 30].railway[1].push_back(index[1][j - 1] + 30);
            nodes[index[5][j]].railway[1].push_back(index[5][j - 1]);
            nodes[index[5][j] + 30].railway[1].push_back(index[5][j - 1] + 30);
        }
        nodes[index[0][j]].road.push_back(index[1][j]);
        nodes[index[0][j] + 30].road.push_back(index[1][j] + 30);
        nodes[index[1][j]].road.push_back(index[0][j]);
        nodes[index[1][j] + 30].road.push_back(index[0][j] + 30);
    }
    for (int i = 1; i < 6; ++i) {
        if (i != 5) {
            nodes[index[i][0]].railway[0].push_back(index[i + 1][0]);
            nodes[index[i][0] + 30].railway[0].push_back(index[i + 1][0] + 30);
            nodes[index[i][4]].railway[0].push_back(index[i + 1][4]);
            nodes[index[i][4] + 30].railway[0].push_back(index[i + 1][4] + 30);
        }
        if (i != 1) {
            nodes[index[i][0]].railway[0].push_back(index[i - 1][0]);
            nodes[index[i][0] + 30].railway[0].push_back(index[i - 1][0] + 30);
            nodes[index[i][4]].railway[0].push_back(index[i - 1][4]);
            nodes[index[i][4] + 30].railway[0].push_back(index[i - 1][4] + 30);
        }
    }
    for (int j = 0; j < 5; j += 2) {
        nodes[index[5][j]].railway[0].push_back(index[6][j]);
        nodes[index[6][j]].railway[0].push_back(index[5][j]);
    }

    // Coordinate change list used in the BFS
    int dx[] = { -1, 0, 0, 1, -1, -1, 1, 1 };
    int dy[] = { 0, -1, 1, 0, -1, 1, -1, 1 };

    for (int i = 2; i <= 4; ++i) {
        for (int j = 1; j <= 3; ++j) {
            int lim = nodes[index[i][j]].isSafe() ? 8 : 4;
            for (int k = 0; k < lim; ++k) {
                int x = i + dx[k], y = j + dy[k];
                nodes[index[i][j]].road.push_back(index[x][y]);
                nodes[index[i][j] + 30].road.push_back(index[x][y] + 30);
                if (!(2 <= x && x <= 4 && 1 <= y && y <= 3)) {
                    nodes[index[x][y]].road.push_back(index[i][j]);
                    nodes[index[x][y] + 30].road.push_back(index[i][j] + 30);
                }
            }
        }
    }
}

void Game::InitChess() {
    // Generate a random list
    std::vector<int> random_list;
    random_list.reserve(50);
    for (int i = 0; i < 60; ++i) {
        if (!nodes[i].isSafe())
            random_list.push_back(i);
    }
    std::mt19937 g(time(0));
    std::shuffle(random_list.begin(), random_list.end(), g);
    start_player_ = current_player_ = g() & 1;

    // Generate chess
    memset(role_count_, 0, sizeof(role_count_));
    int tip = 0;
    for (int camp = 0; camp < 2; ++camp) {
        nodes[random_list[tip++]].chess = new Chess { camp, 10, this };
        flag_[camp] = nodes[random_list[tip - 1]].chess;
        nodes[random_list[tip++]].chess = new Chess { camp, 9, this };
        nodes[random_list[tip++]].chess = new Chess { camp, 8, this };
        for (int k = 0; k < 3; ++k) {
            nodes[random_list[tip++]].chess = new Chess { camp, -2, this };
            for (int i = 1; i <= 3; ++i)
                nodes[random_list[tip++]].chess = new Chess { camp, i, this };
        }
        for (int k = 0; k < 2; ++k) {
            nodes[random_list[tip++]].chess = new Chess { camp, -1, this };
            for (int i = 4; i <= 7; ++i)
                nodes[random_list[tip++]].chess = new Chess { camp, i, this };
        }
    }
    for (int i = 0; i < 60; ++i) {
        if (nodes[i].chess != nullptr)
            nodes[i].chess->setNode(&nodes[i]);
    }
}

Game::Game() {
    InitGraph();
    InitChess();
}

std::vector<int> Game::GetRailwayList(
    const int& now, const bool& direction, const bool& type, const int& start) {

    std::vector<int> ans;
    if (vis[now])
        return ans;
    vis[now] = 1;
    if (nodes[start].chess->Attack(nodes[now].chess) >= 0)
        ans.push_back(now);

    // Target node has chess on it
    if (nodes[now].chess != nullptr)
        return ans;

    // DFS
    for (auto to : nodes[now].railway[direction]) {
        std::vector<int> tmp = GetRailwayList(to, direction, type, start);
        ans.insert(ans.end(), tmp.begin(), tmp.end());
    }
    if (type) {
        for (auto to : nodes[now].railway[direction ^ 1]) {
            std::vector<int> tmp
                = GetRailwayList(to, direction ^ 1, type, start);
            ans.insert(ans.end(), tmp.begin(), tmp.end());
        }
    }

    return ans;
}

std::vector<int> Game::GetList(const int& number) {
    std::vector<int> ans;
    memset(vis, 0, sizeof(vis));
#ifdef DEBUG
    assert(nodes[number].chess);
#endif
    // Linked with road
    for (auto to : nodes[number].road) {
        if (nodes[number].chess->Attack(nodes[to].chess) >= 0)
            ans.push_back(to);
    }
    // Linked with railway
    for (int k = 0; k < 2; ++k) {
        for (auto to : nodes[number].railway[k]) {
            std::vector<int> tmp
                = GetRailwayList(to, k, nodes[number].chess->role == 1, number);
            ans.insert(ans.end(), tmp.begin(), tmp.end());
        }
    }
    return ans;
}

void Game::CountRole(const int& camp, const int& role, const int& value) {
    role_count_[camp][role + 2] += value;
    if (role == -2 && role_count_[camp][role + 2] == 0) {
#ifdef DEBUG
        assert(flag_[camp]->role == 10);
#endif
        --role_count_[camp][flag_[camp]->role + 2];
        flag_[camp]->role = 0;
        ++role_count_[camp][flag_[camp]->role + 2];
    }
}

bool Game::Movable(const int& player) {
    for (int i = 0; i < 60; ++i) {
        if (nodes[i].chess != nullptr && nodes[i].chess->hidden)
            return 1;
    }
    if (own_camp_ == -1)
        return 0;

    for (int i = 0; i < 60; ++i) {
        if (nodes[i].chess != nullptr
            && nodes[i].chess->camp == (own_camp_ ^ player)) {
            int role = nodes[i].chess->role;
            if (role != -2 && role != 0 && role != 10 && GetList(i).size() != 0)
                return 1;
        }
    }
    return 0;
}

void Game::TurnOver(const int& number) {
#ifdef DEBUG
    assert(nodes[number].chess->hidden == 1);
#endif
    nodes[number].chess->hidden = 0;
    if (own_camp_ == -1) {
        if (last_take_[current_player_] == nodes[number].chess->camp) {
            own_camp_ = last_take_[current_player_] ^ current_player_;
        }
        last_take_[current_player_] = nodes[number].chess->camp;
    }
}

void Game::Capture(const int& now, const int& to) {
#ifdef DEBUG
    assert(nodes[now].chess);
    assert(nodes[now].chess->Attack(nodes[to].chess) >= 0);
#endif
    if (nodes[now].chess->Attack(nodes[to].chess) == 0) {
        nodes[to].DeleteChess();
        nodes[now].DeleteChess();
    } else {
        nodes[to].DeleteChess();
        nodes[to].setChess(nodes[now].chess);
        nodes[now].chess = nullptr;
    }
}

void Game::BeforeTurn() { current_player_ ^= 1; }
void Game::AfterTurn() {
    if (!Movable(current_player_ ^ 1))
        SetWinner(current_player_);
    if (!Movable(current_player_))
        SetWinner(current_player_ ^ 1);
}

void Game::SetWinner(const int& winner) { winner_ = winner; }

// int main() {
//     Game g;
//     g.TurnOver(1, 1);
//     // freopen("log.log", "w", stdout);
//     // Chess* chess;
//     // for (int i = 0; i < 60; ++i) {
//     //     std::cout << i << ':';
//     //     if (chess = g.nodes[i].chess) {
//     //         std::cout << chess->camp << ' ' << chess->role << ' ' <<
//     //         chess->hidden;
//     //     }
//     //     std::cout << std::endl;
//     // }
// }
