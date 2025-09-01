#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>

int main() {
    using namespace ftxui;

    // Створюємо елемент — вертикальний бокс (vbox), що містить кілька рядків тексту.
    auto document = vbox({
        text("Привіт, це FTXUI!"),
        text("Ми створюємо консольний інтерфейс."),
        text("Натисніть Enter, щоб вийти.")
    }) | border; // Застосовуємо модифікатор `border` для створення рамки.

    // Створюємо екран і рендеримо на ньому наш документ.
    auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
    Render(screen, document);

    // Виводимо вміст екрана в консоль.
    screen.Print();
    
    // Чекаємо, поки користувач натисне Enter.
    std::cin.get();

    return 0;
}