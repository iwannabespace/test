#pragma once

#include <SFML/Graphics.hpp>
#include <array>

class StartScreen : public sf::Drawable
{
    public:
        StartScreen(const sf::RenderWindow& window, const sf::Color& color = sf::Color::Cyan);
        ~StartScreen();
        void update(const sf::RenderWindow& window);
        void updateOnResize();
        explicit operator bool() const noexcept;
    private:
        void draw(sf::RenderTarget& target, const sf::RenderStates& states) const override;
    private:
        const sf::RenderWindow& window;
        sf::RectangleShape background;
        std::array<sf::Text, 3> options;
        sf::Font font;
        bool connected;
};