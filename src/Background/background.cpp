#include "../headers/background.h"
#include <fstream>
#include <sstream>
#include <ctime>

// Реалізація методів Vector2.
Vector2::Vector2(double x_, double y_) : x(x_), y(y_) {}

double Vector2::dot(const Vector2& other) const {
    return x * other.x + y * other.y;
}

// Реалізація методів PerlinNoise.
PerlinNoise::PerlinNoise(unsigned int seed) {
    std::vector<int> p(256);
    for (int i = 0; i < 256; i++) p[i] = i;
    std::mt19937 gen(seed);
    std::shuffle(p.begin(), p.end(), gen);
    
    permutation.resize(512);
    for (int i = 0; i < 256; i++) {
        permutation[i] = p[i];
        permutation[i + 256] = p[i];
    }
}

double PerlinNoise::Fade(double t) const {
    return ((6 * t - 15) * t + 10) * t * t * t;
}

double PerlinNoise::Lerp(double t, double a1, double a2) const {
    return a1 + t * (a2 - a1);
}

Vector2 PerlinNoise::GetConstantVector(int v) const {
    int h = v & 3;
    if (h == 0) return Vector2(1.0, 1.0);
    else if (h == 1) return Vector2(-1.0, 1.0);
    else if (h == 2) return Vector2(-1.0, -1.0);
    else return Vector2(1.0, -1.0);
}

double PerlinNoise::Noise2D(double x, double y) const {
    int X = static_cast<int>(std::floor(x)) & 255;
    int Y = static_cast<int>(std::floor(y)) & 255;
    
    double xf = x - std::floor(x);
    double yf = y - std::floor(y);

    Vector2 topRight(xf - 1.0, yf - 1.0);
    Vector2 topLeft(xf, yf - 1.0);
    Vector2 bottomRight(xf - 1.0, yf);
    Vector2 bottomLeft(xf, yf);

    int valueTopRight = permutation[permutation[X + 1] + Y + 1];
    int valueTopLeft = permutation[permutation[X] + Y + 1];
    int valueBottomRight = permutation[permutation[X + 1] + Y];
    int valueBottomLeft = permutation[permutation[X] + Y];

    double dotTopRight = topRight.dot(GetConstantVector(valueTopRight));
    double dotTopLeft = topLeft.dot(GetConstantVector(valueTopLeft));
    double dotBottomRight = bottomRight.dot(GetConstantVector(valueBottomRight));
    double dotBottomLeft = bottomLeft.dot(GetConstantVector(valueBottomLeft));
    
    double u = Fade(xf);
    double v = Fade(yf);

    return Lerp(u,
                Lerp(v, dotBottomLeft, dotTopLeft),
                Lerp(v, dotBottomRight, dotTopRight));
}

// Реалізація методів GameWorld.
GameWorld::GameWorld() : perlin_noise(0), seed(0) {
    loadSeed();
    perlin_noise = PerlinNoise(seed);
    loadWorldFromFile();
}

GameWorld::~GameWorld() {
    saveWorldToFile();
}

char GameWorld::getTerrain(int x, int y) {
    auto key = std::make_pair(x, y);
    
    auto cache_it = world_cache.find(key);
    if (cache_it != world_cache.end()) {
        return cache_it->second;
    }
    
    double nx = static_cast<double>(x) * frequency;
    double ny = static_cast<double>(y) * frequency;
    double noise_value = perlin_noise.Noise2D(nx, ny);
    noise_value = (noise_value + 1.0) / 2.0;

    double select = noise_value * 100.0;
    int cumulative = 0;
    char terrain = terrain_chars.back();
    for (size_t i = 0; i < terrain_weights.size(); ++i) {
        cumulative += terrain_weights[i];
        if (select < cumulative) {
            terrain = terrain_chars[i];
            break;
        }
    }
    
    saveTerrainToFile(x, y, terrain);
    world_cache[key] = terrain;
    
    return terrain;
}

void GameWorld::saveTerrain(int x, int y, char terrain) {
    auto key = std::make_pair(x, y);
    if (world_cache.find(key) != world_cache.end()) {
        return;
    }
    world_cache[key] = terrain;
    saveTerrainToFile(x, y, terrain);
}

void GameWorld::saveTerrainToFile(int x, int y, char terrain) {
    std::ofstream file(world_file, std::ios::app);
    if (file.is_open()) {
        file << x << " " << y << " " << terrain << "\n";
        file.close();
    }
}

void GameWorld::saveWorldToFile() {
    std::ofstream file(world_file);
    if (!file.is_open()) {
        return;
    }
    
    for (const auto& entry : world_cache) {
        int x = entry.first.first;
        int y = entry.first.second;
        char terrain = entry.second;
        file << x << " " << y << " " << terrain << "\n";
    }
    
    file.close();
}

void GameWorld::loadWorldFromFile() {
    world_cache.clear();
    std::ifstream file(world_file);
    if (!file.is_open()) {
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int x, y;
        char terrain;
        
        if (iss >> x >> y >> terrain) {
            auto key = std::make_pair(x, y);
            world_cache[key] = terrain;
        }
    }
    
    file.close();
}

void GameWorld::loadSeed() {
    std::ifstream file(seed_file);
    if (file.is_open()) {
        file >> seed;
        file.close();
    } else {
        seed = static_cast<unsigned int>(std::time(0));
        saveSeed();
    }
}

void GameWorld::saveSeed() {
    std::ofstream file(seed_file);
    if (file.is_open()) {
        file << seed << "\n";
        file.close();
    }
}