#include <game.h>
#include <string>
#include <fstream>

Game::Game() {
    loadPlayerPosition();
}

Game::~Game() {
    savePlayerPosition();
}

void Game::savePlayerPosition() {
    std::ofstream file(player_file);
    if (file.is_open()) {
        file << player_x << " " << player_y << "\n";
        file.close();
    }
}

void Game::loadPlayerPosition() {
    std::ifstream file(player_file);
    if (!file.is_open()) {
        return; // Залишити позицію за замовчуванням (0, 0)
    }
    
    file >> player_x >> player_y;
    file.close();
}

Component Game::CreateGameComponent() {
    auto component = Renderer([&] {
        auto terminal_size = Terminal::Size();
        int width = terminal_size.dimx - 2;  // -2 для border
        int height = terminal_size.dimy - 2; // -2 для border
        
        // Розраховуємо позицію камери (персонаж завжди в центрі)
        camera_x = player_x - width / 2;
        camera_y = player_y - height / 2;
        
        Elements rows;
        
        for (int y = 0; y < height; y++) {
            Elements row;
            for (int x = 0; x < width; x++) {
                int world_x = camera_x + x;
                int world_y = camera_y + y;
                
                unsigned char display_char;
                Color text_color = Color::White;
                
                // Перевіряємо, чи це позиція персонажа
                if (world_x == player_x && world_y == player_y) {
                    display_char = '8';
                    text_color = Color::White;
                } else {
                    display_char = world.getTerrain(world_x, world_y);
                    
                    // Додаємо кольори для різних типів поверхні
                    switch (display_char) {
                        case '.': text_color = Color::Blue3Bis; break;
                        case ',': text_color = Color::Blue; break;
                        case '~': text_color = Color::Yellow; break;
                        case '^': text_color = Color::Yellow3Bis; break;
                        case '*': text_color = Color::GreenLight; break;
                        case 'o': text_color = Color::Green3Bis; break;
                        case '+': text_color = Color::Green; break;
                        case '#': text_color = Color::GrayDark; break;
                    }
                }
                
                row.push_back(text(std::string(1, display_char)) | color(text_color));
            }
            rows.push_back(hbox(std::move(row)));
        }
        
        auto game_field = vbox(std::move(rows));
        
        // Додаємо інформацію про позицію
        auto info = hbox({
            text("Позиція: (") | color(Color::Cyan),
            text(std::to_string(player_x)) | color(Color::White),
            text(", ") | color(Color::Cyan),
            text(std::to_string(player_y)) | color(Color::White),
            text(")") | color(Color::Cyan),
            separator(),
            text(" ESC/Q - вихід") | color(Color::GrayLight)
        });
        
        return vbox({
            info,
            separator(),
            game_field | flex
        }) | border;
    });
    
    // Додаємо обробку клавіш
    component = CatchEvent(component, [&](Event event) {
        if (event == Event::ArrowUp || event == Event::Character('w') || event == Event::Character('W')) {
            player_y--;
            return true;
        }
        if (event == Event::ArrowDown || event == Event::Character('s') || event == Event::Character('S')) {
            player_y++;
            return true;
        }
        if (event == Event::ArrowLeft || event == Event::Character('a') || event == Event::Character('A')) {
            player_x--;
            return true;
        }
        if (event == Event::ArrowRight || event == Event::Character('d') || event == Event::Character('D')) {
            player_x++;
            return true;
        }
        if (event == Event::Escape || event == Event::Character('q') || event == Event::Character('Q')) {
            running = false;
            return true;
        }
        return false;
    });
    
    return component;
}