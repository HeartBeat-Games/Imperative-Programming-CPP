#pragma once

#include <vector>
#include <fstream>
#include <filesystem>

#include "Utilities.h"

enum E_CellType
{
    Ground = 0,
    Water,
    Mountain,
    Exit,
    CellType_Count
};

struct CellInfo {
    char symbol;
    bool isWalkable;
};

const CellInfo g_cellInfos[E_CellType::CellType_Count] =
{
    { '.', true },
    { '~', false },
    { 'A', false },
    { 'E', true },
};

struct Map {
    int* cells;
    int  width;
    int  height;

    std::vector<Vector2> characterSpawns;
};

int get1DCell(Vector2 pos, int width);
Map* LoadLevel(std::ifstream& file);
std::vector<Map*> LoadAllLevels();
void releaseAllMaps(std::vector<Map*> allMaps);