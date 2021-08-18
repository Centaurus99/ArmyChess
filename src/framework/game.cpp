#include "game.h"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

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
            nodes[index[1][j]].railway_horizontal.push_back(index[1][j + 1]);
            nodes[index[1][j] + 30].railway_horizontal.push_back(index[1][j + 1] + 30);
            nodes[index[5][j]].railway_horizontal.push_back(index[5][j + 1]);
            nodes[index[5][j] + 30].railway_horizontal.push_back(index[5][j + 1] + 30);
        }
        if (j != 0) {
            nodes[index[0][j]].road.push_back(index[0][j - 1]);
            nodes[index[0][j] + 30].road.push_back(index[0][j - 1] + 30);
            nodes[index[1][j]].railway_horizontal.push_back(index[1][j - 1]);
            nodes[index[1][j] + 30].railway_horizontal.push_back(index[1][j - 1] + 30);
            nodes[index[5][j]].railway_horizontal.push_back(index[5][j - 1]);
            nodes[index[5][j] + 30].railway_horizontal.push_back(index[5][j - 1] + 30);
        }
        nodes[index[0][j]].road.push_back(index[1][j]);
        nodes[index[0][j] + 30].road.push_back(index[1][j] + 30);
        nodes[index[1][j]].road.push_back(index[0][j]);
        nodes[index[1][j] + 30].road.push_back(index[0][j] + 30);
    }
    for (int i = 1; i < 6; ++i) {
        if (i != 5) {
            nodes[index[i][0]].railway_vertical.push_back(index[i + 1][0]);
            nodes[index[i][0] + 30].railway_vertical.push_back(index[i + 1][0] + 30);
            nodes[index[i][4]].railway_vertical.push_back(index[i + 1][4]);
            nodes[index[i][4] + 30].railway_vertical.push_back(index[i + 1][4] + 30);
        }
        if (i != 1) {
            nodes[index[i][0]].railway_vertical.push_back(index[i - 1][0]);
            nodes[index[i][0] + 30].railway_vertical.push_back(index[i - 1][0] + 30);
            nodes[index[i][4]].railway_vertical.push_back(index[i - 1][4]);
            nodes[index[i][4] + 30].railway_vertical.push_back(index[i - 1][4] + 30);
        }
    }
    for (int j = 0; j < 5; j += 2) {
        nodes[index[5][j]].railway_vertical.push_back(index[6][j]);
        nodes[index[6][j]].railway_vertical.push_back(index[5][j]);
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
    for (int i = 0; i < 60; ++i) {
        if (!nodes[i].isSafe())
            random_list.push_back(i);
    }
    std::mt19937 g(time(0));
    std::shuffle(random_list.begin(), random_list.end(), g);

    // Generate chess
    int tip = 0;
    for (int camp = 0; camp < 2; ++camp) {
        nodes[random_list[tip++]].chess = new Chess { camp, 0 };
        nodes[random_list[tip++]].chess = new Chess { camp, 7 };
        nodes[random_list[tip++]].chess = new Chess { camp, 8 };
        for (int k = 0; k < 3; ++k) {
            nodes[random_list[tip++]].chess = new Chess { camp, -2 };
            for (int i = 1; i <= 3; ++i)
                nodes[random_list[tip++]].chess = new Chess { camp, i };
        }
        for (int k = 0; k < 2; ++k) {
            nodes[random_list[tip++]].chess = new Chess { camp, -1 };
            for (int i = 4; i <= 7; ++i)
                nodes[random_list[tip++]].chess = new Chess { camp, i };
        }
    }
}

Game::Game() {
    InitGraph();
    InitChess();
}

int main() {
    Game g;
    // freopen("log.log", "w", stdout);
    // Chess* chess;
    // for (int i = 0; i < 60; ++i) {
    //     std::cout << i << ':';
    //     if (chess = g.nodes[i].chess) {
    //         std::cout << chess->camp << ' ' << chess->role << ' ' << chess->hiden;
    //     }
    //     std::cout << std::endl;
    // }
}