#include <iostream>
#include <thread>
#include <chrono>
#include <game.h>

using namespace ftxui;

int main() {
    std::cout << "Завантаження гри...\n";
    std::cout << "Використовуйте стрілки або WASD для руху\n";
    std::cout << "ESC або Q для виходу\n";
    // std::cout << "Натисніть Enter для початку...\n";
    // std::cin.get();
    
    Game game;
    auto screen = ScreenInteractive::Fullscreen();
    auto component = game.CreateGameComponent();
    
    Loop loop(&screen, component);
    
    // Запускаємо гру в окремому потоці
    std::thread game_thread([&]() {
        while (game.isRunning()) {
            screen.PostEvent(Event::Custom);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        screen.ExitLoopClosure()();
    });
    
    loop.Run();
    game_thread.join();
    return 0;
}