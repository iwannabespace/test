#include "../include/Client.hpp"
#include <sstream>
#include <iostream>

Client::Client(const sf::IpAddress& ip, int port)
    : ip(ip), port(port), receivedAudio(false), receivedPosition(false)
{
}

Client::~Client()
{
}

bool Client::connect()
{
    if (socket.connect(ip, port) != sf::Socket::Done)
        return false;

    return true;
}

void Client::disconnect()
{
    socket.disconnect();
}

bool Client::send(sf::Packet& packet)
{
    if (socket.send(packet) != sf::Socket::Done)
        return false;
    
    return true;
}

bool Client::receivePacket(sf::Packet& packet, std::unordered_map<std::uint32_t, Player>& players, std::uint32_t& thisPlayer)
{
    sf::Packet recv;
    sf::Packet copy;

    while (true)
    {
        if (socket.receive(recv) != sf::Socket::Done)
            return false;

        if (receivedAudio)
            std::cout << "Data is not used!" << std::endl;
        
        std::uint8_t command;
        std::uint32_t id;
        copy = recv;

        if (copy >> command)
        {    
            switch (command)
            {    
                case ServerCommand::ADD_PLAYER:
                    copy >> id;
                    if (thisPlayer == 0)
                    {    
                        thisPlayer = id;
                        std::cout << "My id is " << thisPlayer << std::endl;
                    }
                    players[id] = Player(5.f, id);
                    std::cout << "Player joined!" << std::endl;
                    break;

                case ServerCommand::ADD_PLAYERS:
                    while (copy >> id)
                        players[id] = Player(5.f, id);
                    break;

                case ServerCommand::RECEIVE_AUDIO:
                    receivedAudio = true;
                    packet = recv;
                    break;

                case ServerCommand::RECEIVE_POSITION:
                    receivedPosition = true;
                    packet = recv;
                    break;
        
                default:
                    break;
            }
        }
    }

    return true;
}