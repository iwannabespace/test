#include "../include/StartScreen.hpp"
#include <iostream>

StartScreen::StartScreen(const sf::RenderWindow& window, const sf::Color& color)
    : window(window), connected(false)
{
    if (color == sf::Color::Black)
        throw std::string("Background can't be black!");

    if (!font.loadFromFile("fonts/Cascadia.ttf"))
        throw std::string("Font could't be loaded!");

    background.setSize(sf::Vector2f(window.getSize()));
    background.setFillColor(color);

    sf::Vector2f winsize = sf::Vector2f(window.getSize());
    options.at(0).setFont(font);
    options.at(0).setString("Connect to Server");
    options.at(0).setCharacterSize(20);
    options.at(0).setFillColor(sf::Color::Black);
    options.at(0).setPosition({
        std::round((winsize.x - options.at(0).getGlobalBounds().width) / 2),
        std::round((winsize.y - options.at(0).getGlobalBounds().height * 3) / 2),
    });

    options.at(1).setFont(font);
    options.at(1).setString("Options");
    options.at(1).setCharacterSize(20);
    options.at(1).setFillColor(sf::Color::Black);
    options.at(1).setPosition({
        std::round((winsize.x - options.at(1).getGlobalBounds().width) / 2),
        std::round(options.at(0).getPosition().y + options.at(0).getGlobalBounds().height + 20)
    });

    options.at(2).setFont(font);
    options.at(2).setString("Quit");
    options.at(2).setCharacterSize(20);
    options.at(2).setFillColor(sf::Color::Black);
    options.at(2).setPosition({
        std::round((winsize.x - options.at(2).getGlobalBounds().width) / 2),
        std::round(options.at(1).getPosition().y + options.at(1).getGlobalBounds().height + 20)
    });
}

StartScreen::~StartScreen()
{
}

void StartScreen::update(const sf::RenderWindow& window)
{
    if (options.at(0).getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
    {
        options.at(0).setFillColor(sf::Color::White);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            
        }
    }

    else if (options.at(1).getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
    {
        options.at(1).setFillColor(sf::Color::White);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {

        }
    }

    else if (options.at(2).getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
    {
        options.at(2).setFillColor(sf::Color::White);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
        }
    }

    else
    {
        for (sf::Text& option : options)
        {    
            option.setFillColor(sf::Color::Black);
        }
    }
}

void StartScreen::updateOnResize()
{
    sf::Vector2f winsize = sf::Vector2f(window.getSize());

    background.setSize(winsize);
    
    options.at(0).setPosition({
        std::round((winsize.x - options.at(0).getGlobalBounds().width) / 2),
        std::round((winsize.y - options.at(0).getGlobalBounds().height * 3) / 2),
    });

    options.at(1).setPosition({
        std::round((winsize.x - options.at(1).getGlobalBounds().width) / 2),
        std::round(options.at(0).getPosition().y + options.at(0).getGlobalBounds().height + 20)
    });

    options.at(2).setPosition({
        std::round((winsize.x - options.at(2).getGlobalBounds().width) / 2),
        std::round(options.at(1).getPosition().y + options.at(1).getGlobalBounds().height + 20)
    });
}

StartScreen::operator bool() const noexcept
{
    return !connected;
}

void StartScreen::draw(sf::RenderTarget& target, const sf::RenderStates& states) const
{
    target.draw(background, states);
    target.draw(options.at(0), states);
    target.draw(options.at(1), states);
    target.draw(options.at(2), states);
}