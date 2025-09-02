#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <map>
#include <vector>
#include <random>
#include <string>
#include <fstream>
#include <cmath>
#include <utility>
#include <algorithm>

// Заголовні файли потрібно розміщувати на початку
// (ftxui та інші, якщо вони використовуються).

// Клас для роботи з 2D-векторами.
class Vector2 {
public:
    double x, y;
    Vector2(double x_, double y_);
    double dot(const Vector2& other) const;
};

// Клас для генерації шуму Перліна.
class PerlinNoise {
private:
    std::vector<int> permutation;

public:
    PerlinNoise(unsigned int seed);
    double Fade(double t) const;
    double Lerp(double t, double a1, double a2) const;
    Vector2 GetConstantVector(int v) const;
    double Noise2D(double x, double y) const;
};

// Клас, що представляє ігровий світ та керує генерацією ландшафту.
class GameWorld {
private:
    std::map<std::pair<int, int>, char> world_cache;
    std::mt19937 rng;
    std::string world_file = "world.txt";
    std::string seed_file = "seed.txt";

    // Поля для генерації ландшафту
    PerlinNoise perlin_noise;
    unsigned int seed;
    double frequency = 0.05;

    const std::vector<char> terrain_chars = {'.', ',', '~', '^', '*', 'o', '+', '#'};
    const std::vector<int> terrain_weights = {5, 6, 8, 8, 8, 10, 25, 30};

    // Методи для роботи з файлом
    void saveTerrainToFile(int x, int y, char terrain);
    void loadSeed();
    void saveSeed();

public:
    GameWorld();
    ~GameWorld();
    char getTerrain(int x, int y);
    void saveTerrain(int x, int y, char terrain);
    void saveWorldToFile();
    void loadWorldFromFile();
};

#endif // BACKGROUND_H