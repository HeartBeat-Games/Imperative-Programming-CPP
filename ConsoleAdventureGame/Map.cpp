#include "Map.h"

namespace fs = std::filesystem;


int get1DCell(Vector2 pos, int width)
{
    return pos.y * width + pos.x;
}


Map* LoadLevel(std::ifstream& file)
{
    // No error handling => require the file to be correct

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string content = buffer.str();
    int width = content.find_first_of('\n');
    int height = content.size() / width;

    Map* map = new Map{
        new int[width * height],
        width,
        height,
        std::vector<Vector2>(),
    };

    // Make sur player is first
    map->characterSpawns.push_back({});

    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
        {
            int index = y * width + x;

            // Default
            map->cells[index] = E_CellType::Ground;

            switch (content[index + y]) // +y for '\n' offset
            {
            case '~': map->cells[index] = E_CellType::Water; break;
            case 'A': map->cells[index] = E_CellType::Mountain; break;
            case 'E': map->cells[index] = E_CellType::Exit; break;

            case 'X':
                map->characterSpawns.push_back(Vector2{ x, y });
                break;
            case 'H':
                map->characterSpawns[0] = Vector2{ x, y };
                break;
            }
        }

    return map;
}

std::vector<Map*> LoadAllLevels()
{
    std::string path = "levels";
    size_t mapCount = 0;

    for (const auto& entry : fs::directory_iterator(path))
        if (entry.is_regular_file())
            mapCount++;

    std::vector<Map*> allMaps(mapCount);

    int i = 0;
    for (const auto& entry : fs::directory_iterator(path))
        if (entry.is_regular_file())
        {
            std::ifstream levelFile(entry.path());
            allMaps[i] = LoadLevel(levelFile);
            i++;
        }

    return allMaps;
}

void releaseAllMaps(std::vector<Map*> allMaps)
{
    for (int i = 0; i < allMaps.size(); ++i)
    {
        delete allMaps[i]->cells;
        delete allMaps[i];
    }
}