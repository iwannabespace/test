#pragma once

#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include "Player.hpp"
#include "../../Common/Headers.hpp"
#include "AudioCapturer.hpp"

class Player;
class AudioCapturer;

class Client
{
    public:
        Client(const sf::IpAddress& ip, int port);
        ~Client();
        bool connect();
        void disconnect();
        bool send(sf::Packet& packet);
        bool receivePacket(sf::Packet& packet, std::unordered_map<std::uint32_t, Player>& players, std::uint32_t& thisPlayer);
    public:
        bool receivedAudio;
        bool receivedPosition;
    private:
        int port;
        sf::IpAddress ip;
        sf::TcpSocket socket;
};