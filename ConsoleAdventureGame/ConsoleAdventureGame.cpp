// ConsoleAdventureGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "Map.h"
#include "Characters.h"
#include "Spell.h"
#include "FSM.h"



char* createWindow(Map& map)
{
    size_t windowSize = map.width * map.height + map.height;

    char* window = new char[windowSize];

    // Initialize just to be safe
    for (int i = 0; i < windowSize; ++i)
        window[i] = 'o';

    // End of lines
    {
        int i = 0;
        while (i < windowSize)
        {
            window[i + map.width] = '\n';
            i += map.width + 1; // width + '\n'
        }
    }

    // End of word/string
    window[windowSize - 1] = '\0';

    return window;
}

void displayMap(char* window, Map& map, Characters& characters, Spell& spell)
{
    system("cls"); //clear

    size_t mapSize = map.width * map.height;

    // Map cells
    int i = 0; // map index
    int textOffset = 0; // offset for each /n at the end of each line
    while (i < mapSize)
    {
        for (int j = 0; j < map.width; ++j)
            window[i + j + textOffset] = g_cellInfos[map.cells[i + j]].symbol;

        textOffset++;
        i += map.width;
    }

    // Spell range
    // Would be far more lisible if range was Vector2 and we had operator function
    if (spell.isVisible)
        for (int y = (spell.origin.y - spell.range); 
            y <= (spell.origin.y + spell.range); ++y)
            for (int x = (spell.origin.x - spell.range);
                x <= (spell.origin.x + spell.range); ++x)
        {
            int index = get1DCell({ x, y }, map.width);
            window[index + y] = '*';
        }
    
    // Characters
    for (int i = 0; i < characters.size; ++i)
    {
        Character& character = characters.data[i]; // no copy
        if (character.isAlive)
        {
            int index = get1DCell(character.pos, map.width);
            window[index + character.pos.y] = characterSymbols[character.type];
        }
    }

    //Spell cursor
    if (spell.isVisible)
    {
        int index = get1DCell({ spell.cursor.x, spell.cursor.y }, map.width);
        window[index + spell.cursor.y] = '^';
    }

    std::cout << window;
}





void initializeLevel(
    int levelIndex, char** window,
    Map** currentMap, std::vector<Map*> allMaps,
    Character** player, Characters** characters)
{
    *currentMap = allMaps[levelIndex];

    *characters = createCharacters(**currentMap);
    *player = &((*characters)->data[0]);

    *window = createWindow(**currentMap);
}

void releaseLevelData(
    char* window,
    Characters* characters)
{
    delete window;
    delete characters;
}




int gameLoop(
    FSM& fsm,
    char* window, Map& currentMap,
    Character& player, Characters& characters, Spell& spell)
{
    {
        Character& player = characters.data[0];
        int fsmState = 0;
        fsm.onEnter[fsmState](player, spell);

        do {
            int newState = fsm.onUpdate[fsmState](currentMap, player, characters, spell);
            displayMap(window, currentMap, characters, spell);

            if (newState != fsmState)
            {
                fsm.onExit[fsmState](player, spell);
                fsmState = newState;
                if (fsmState != 2)
                    fsm.onEnter[fsmState](player, spell);
                displayMap(window, currentMap, characters, spell);
            }
        } while (fsmState != 2);

        int index = get1DCell(player.pos, (currentMap).width);
        if ((currentMap).cells[index] == E_CellType::Exit)
            return 1;
    }

    // Move enemies
    for (int i = 1; i < characters.size; ++i)
    {
        Vector2 dir;
        bool moved = false;
        do {
            dir = getRandomDirection();
            moved = characterMove(characters.data[i], dir, currentMap, characters);
        } while (!moved);
    }

    if (!player.isAlive)
        return 2;

    displayMap(window, currentMap, characters, spell);

    return 0;
}







int main()
{
    std::vector<Map*> allMaps = LoadAllLevels();

    FSM fsm = {
        { movement_onEnter, spell_onEnter },	// onEnter
        { movement_onExit, spell_onExit },		// onExit
        { movement_onUpdate, spell_onUpdate }	// onUpdate
    };
    int mapIndex = 0;

    Spell spell{
        false,
        Vector2 {0, 0},
        Vector2 {0, 0},
        1,
        1
    };
    char* window;
    Map* currentMap;
    Characters* characters;
    Character* player;

    initializeLevel(mapIndex, &window, &currentMap, allMaps, &player, &characters);
    displayMap(window, *currentMap, *characters, spell);

    while (true)
    {
        int status = gameLoop(fsm, window, *currentMap, *player, *characters, spell);
        if (status == 1) // ChangeLevel
        {
            releaseLevelData(window, characters);
            mapIndex++;
            if (mapIndex >= allMaps.size())
            {
                // Victory
                system("cls");
                std::cout << "==== VICTORY ====" << std::endl;
                break;
            }
            initializeLevel(mapIndex, &window, &currentMap, allMaps, &player, &characters);
            displayMap(window, *currentMap, *characters, spell);
        }
        else if (status == 2) // Game Over
        {
            system("cls");
            std::cout << "==== YOU'RE DEAD ====" << std::endl;
            break;
        }
    }

    releaseAllMaps(allMaps);

    return 0;
}
