#pragma once

#include "../../Common/Headers.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include <utility>
#include <random>

using Client = std::pair<std::uint32_t, sf::TcpSocket*>;

class Server
{
    public:
        Server(int port);
        ~Server();
        bool listen();
        void receive();
    private:
        void send_all(sf::Packet& packet);
        void send_except(sf::Packet& packet, const Client& client);
        void setID(Client& client);
        bool contains(std::uint32_t id) const;
        int index(std::uint32_t id) const;
    private:
        int port;
        sf::TcpListener listener;
        sf::SocketSelector selector;
        std::vector<Client> clients;
        size_t received;
    private:
        std::random_device random_device;
        std::mt19937 generator;
};