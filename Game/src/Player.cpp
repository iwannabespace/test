#include "../include/Player.hpp"
#include <iostream>

Player::Player()
{
}

Player::Player(float speed, sf::Uint32 id)
    : player({ 50, 50 }), speed(speed), id(id)
{
    player.setPosition({ 0, 0 });
    player.setFillColor(sf::Color::Red);
}

Player::~Player()
{
}

void Player::move(Client& client)
{
    sf::Packet packet;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {    
        player.move({ 0, -speed });
    
        packet << static_cast<std::uint8_t>(ClientCommand::RECEIVE_POSITION)
            << player.getPosition().x << player.getPosition().y;

        if (!client.send(packet))
            std::cout << "Position couldn't be sent!" << std::endl;
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {    
        player.move({ 0, speed });

        packet << static_cast<std::uint8_t>(ClientCommand::RECEIVE_POSITION)
            << player.getPosition().x << player.getPosition().y;

        if (!client.send(packet))
            std::cout << "Position couldn't be sent!" << std::endl;
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {    
        player.move({ -speed, 0 });

        packet << static_cast<std::uint8_t>(ClientCommand::RECEIVE_POSITION)
            << player.getPosition().x << player.getPosition().y;

        if (!client.send(packet))
            std::cout << "Position couldn't be sent!" << std::endl;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        player.move({ speed, 0 });

        packet << static_cast<std::uint8_t>(ClientCommand::RECEIVE_POSITION)
            << player.getPosition().x << player.getPosition().y;
            
        if (!client.send(packet))
            std::cout << "Position couldn't be sent!" << std::endl;
    }
}

void Player::setPosition(const sf::Vector2f& position)
{
    player.setPosition(position);
}

void Player::setID(sf::Uint32 id)
{
    this->id = id;
}

sf::Vector2f Player::getPosition() const
{
    return player.getPosition();
}

sf::Uint32 Player::getID() const
{
    return id;
}

void Player::draw(sf::RenderTarget& target, const sf::RenderStates& states) const
{
    target.draw(player, states);
}