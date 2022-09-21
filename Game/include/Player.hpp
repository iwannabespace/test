#pragma once

#include <SFML/Graphics.hpp>
#include "Client.hpp"

class Client;

class Player : public sf::Drawable
{
    public:
        Player();
        Player(float speed, std::uint32_t id = 0);
        ~Player();
        void move(Client& client);
        void setPosition(const sf::Vector2f& position);
        void setID(sf::Uint32 id);
        sf::Vector2f getPosition() const;
        sf::Uint32 getID() const;
    private:
        void draw(sf::RenderTarget& target, const sf::RenderStates& states) const override;
    private:
        std::uint32_t id;
        sf::RectangleShape player;
        float speed;
};