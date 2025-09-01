#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <map>
#include <vector>
#include <random>
#include <string>
#include <fstream>

class GameWorld {
private:
    std::map<std::pair<int, int>, char> world_cache; // Кеш для швидкого доступу
    std::mt19937 rng;
    std::string world_file = "world.txt";
    
    // Символи для генерації поверхні
    const std::vector<char> terrain_chars = {'.', ',', '~', '^', '*', 'o', '+', '#'};
    const std::vector<int> terrain_weights = {30, 25, 10, 8, 8, 8, 6, 5}; // Ваги для різних символів
    
    // Максимальний розмір кешу
    static const size_t MAX_CACHE_SIZE = 10000;
    
    // Методи для роботи з файлом
    char loadTerrainFromFile(int x, int y);
    void saveTerrainToFile(int x, int y, char terrain);
    void clearOldCache();
    
public:
    GameWorld();
    ~GameWorld();
    char getTerrain(int x, int y);
    void saveTerrain(int x, int y, char terrain);
    void saveWorldToFile();
    void loadWorldFromFile();
};

#endif // BACKGROUND_H