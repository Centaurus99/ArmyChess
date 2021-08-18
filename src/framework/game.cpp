#include "game.h"
#include <iostream>
#include <vector>

// The number of safe nodes in the upper half
std::vector<int> SAFE_NODE = { 11, 13, 17, 21, 23 };

// Get number of node from Coordinate
inline int Index(const int& x, const int& y) { return 5 * x + y; }

// Coordinate change list used in the BFS
int dx[] = { -1, 0, 0, 1, -1, -1, 1, 1 };
int dy[] = { 0, -1, 1, 0, -1, 1, -1, 1 };

Game::Game() {
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

int main() {
    Game g;
    freopen("log.log", "w", stdout);
    for (int i = 0; i < 60; ++i) {
        std::cout << i << ':';
        for (auto x : g.nodes[i].railway_horizontal) {
            std::cout << x << ' ';
        }
        std::cout << std::endl;
    }
}