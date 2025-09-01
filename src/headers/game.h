#ifndef GAME_H
#define GAME_H

#include <ftxui/component/component.hpp>
#include <ftxui/component/loop.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include "background.h"

using namespace ftxui;

class Game {
private:
    int player_x = 0, player_y = 0;  // Позиція персонажа у світі
    int camera_x = 0, camera_y = 0; // Позиція камери
    GameWorld world;
    bool running = true;
    
    // Методи для збереження/завантаження позиції гравця
    void savePlayerPosition();
    void loadPlayerPosition();
    
public:
    Game();
    ~Game();
    Component CreateGameComponent();
    bool isRunning() const { return running; }
};

#endif // GAME_H