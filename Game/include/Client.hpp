#pragma once

#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include "Player.hpp"
#include "../../Common/Headers.hpp"

class Player;

class Client
{
    public:
        Client(const sf::IpAddress& ip, int port);
        ~Client();
        bool connect();
        bool send(sf::Packet& packet);
        bool receivePacket(sf::Packet& packet, std::unordered_map<std::uint32_t, Player>& players, std::uint32_t& thisPlayer);
    public:
        bool receivedAudio;
        bool receivedData;
    private:
        int port;
        sf::IpAddress ip;
        sf::TcpSocket socket;
        std::mutex mutex;
};