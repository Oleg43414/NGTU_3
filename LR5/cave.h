#pragma once
#include <vector>
using namespace std;

class CaveConfig {
public:
    int width, height;
    int birthLimit, deathLimit;
    int chanceToStartAlive;

    CaveConfig(int w, int h, int birth, int death, int chance)
        : width(w), height(h), birthLimit(birth), deathLimit(death), chanceToStartAlive(chance) {}
};

class Cave {
    CaveConfig config;
    vector<vector<int>> grid;
public:
    Cave(const CaveConfig& cfg);
    void randomInitialize();
    void doSimulationStep();
    int countAliveNeighbors(int x, int y) const;
    const vector<vector<int>>& getGrid() const;
};

//какие принципы ООП, какой паттерны, плюсы минусы
