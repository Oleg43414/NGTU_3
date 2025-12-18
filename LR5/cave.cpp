#include "cave.h"
#include <random>

Cave::Cave(const CaveConfig& cfg) : config(cfg), grid(cfg.height, vector<int>(cfg.width, 0)) {}

void Cave::randomInitialize() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, 100);
    for (int i = 0; i < config.height; ++i) {
        for (int j = 0; j < config.width; ++j) {
            grid[i][j] = (dist(gen) < config.chanceToStartAlive) ? 1 : 0;
        }
    }
}

int Cave::countAliveNeighbors(int x, int y) const {
    static int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    static int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    int cnt = 0;
    for (int d = 0; d < 8; ++d) {
        int nx = x + dx[d], ny = y + dy[d];
        if (nx >= 0 && nx < config.height && ny >= 0 && ny < config.width)
            cnt += grid[nx][ny];
    }
    return cnt;
}

void Cave::doSimulationStep() {
    vector<vector<int>> newGrid = grid;
    for (int i = 0; i < config.height; ++i) {
        for (int j = 0; j < config.width; ++j) {
            int neighbors = countAliveNeighbors(i, j);
            if (grid[i][j] == 1) {
                newGrid[i][j] = (neighbors < config.deathLimit) ? 0 : 1;
            } else {
                newGrid[i][j] = (neighbors > config.birthLimit) ? 1 : 0;
            }
        }
    }
    grid = newGrid;
}

const vector<vector<int>>& Cave::getGrid() const {
    return grid;
}
