#include "background.h"
#include <iostream>
#include <sstream>

GameWorld::GameWorld() : rng(std::random_device{}()) {
    loadWorldFromFile();
}

GameWorld::~GameWorld() {
    saveWorldToFile();
}

char GameWorld::getTerrain(int x, int y) {
    auto key = std::make_pair(x, y);
    
    // Спочатку перевіряємо кеш
    auto cache_it = world_cache.find(key);
    if (cache_it != world_cache.end()) {
        return cache_it->second;
    }
    
    // Намагаємося завантажити з файлу
    char terrain = loadTerrainFromFile(x, y);
    
    // Якщо не знайдено в файлі, генеруємо новий
    if (terrain == '\0') {
        std::discrete_distribution<> dist(terrain_weights.begin(), terrain_weights.end());
        terrain = terrain_chars[dist(rng)];
        // Одразу зберігаємо в файл
        saveTerrainToFile(x, y, terrain);
    }
    
    // Додаємо в кеш
    world_cache[key] = terrain;
    
    // Очищуємо кеш якщо він занадто великий
    if (world_cache.size() > MAX_CACHE_SIZE) {
        clearOldCache();
    }
    
    return terrain;
}

void GameWorld::saveTerrain(int x, int y, char terrain) {
    auto key = std::make_pair(x, y);
    world_cache[key] = terrain;
    saveTerrainToFile(x, y, terrain);
}

char GameWorld::loadTerrainFromFile(int x, int y) {
    std::ifstream file(world_file);
    if (!file.is_open()) {
        return '\0'; // Файл не існує
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int file_x, file_y;
        char terrain;
        
        if (iss >> file_x >> file_y >> terrain) {
            if (file_x == x && file_y == y) {
                file.close();
                return terrain;
            }
        }
    }
    
    file.close();
    return '\0'; // Не знайдено
}

void GameWorld::saveTerrainToFile(int x, int y, char terrain) {
    // Перевіряємо чи вже існує цей запис
    char existing = loadTerrainFromFile(x, y);
    if (existing != '\0') {
        return; // Вже існує, не перезаписуємо
    }
    
    // Додаємо новий запис
    std::ofstream file(world_file, std::ios::app);
    if (file.is_open()) {
        file << x << " " << y << " " << terrain << "\n";
        file.close();
    }
}

void GameWorld::clearOldCache() {
    // Простий спосіб - очищуємо половину кешу
    auto it = world_cache.begin();
    std::advance(it, world_cache.size() / 2);
    world_cache.erase(world_cache.begin(), it);
}

void GameWorld::saveWorldToFile() {
    // Зберігаємо весь кеш у файл
    std::ofstream temp_file("world_temp.txt");
    std::ifstream original_file(world_file);
    
    if (!temp_file.is_open()) {
        return;
    }
    
    // Спочатку копіюємо існуючі дані
    if (original_file.is_open()) {
        std::string line;
        while (std::getline(original_file, line)) {
            temp_file << line << "\n";
        }
        original_file.close();
    }
    
    // Додаємо дані з кешу (якщо їх ще немає у файлі)
    for (const auto& entry : world_cache) {
        int x = entry.first.first;
        int y = entry.first.second;
        char terrain = entry.second;
        
        // Перевіряємо чи вже є цей запис
        if (loadTerrainFromFile(x, y) == '\0') {
            temp_file << x << " " << y << " " << terrain << "\n";
        }
    }
    
    temp_file.close();
    
    // Замінюємо оригінальний файл
    std::rename("world_temp.txt", world_file.c_str());
}

void GameWorld::loadWorldFromFile() {
    // При запуску просто очищуємо кеш - дані будуть завантажуватися по потребі
    world_cache.clear();
}