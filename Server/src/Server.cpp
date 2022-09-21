#include "../include/Server.hpp"
#include <SFML/Audio.hpp>
#include <iostream>

Server::Server(int port)
    : port(port)
{
    generator.seed(random_device());
}

Server::~Server()
{
}

bool Server::listen()
{
    if (listener.listen(port) != sf::Socket::Done)
        return false;

    return true;
}

void Server::receive()
{
    if (this->listen())
    {
        selector.add(listener);

        while (true)
        {
            if (selector.wait())
            {
                if (selector.isReady(listener))
                {
                    std::pair<std::uint32_t, sf::TcpSocket*> client;
                    client.second = new sf::TcpSocket;

                    if (listener.accept(*client.second) == sf::Socket::Done)
                    {
                        this->setID(client);
                        clients.push_back(client);
                        selector.add(*client.second);

                        sf::Packet packet;
                        packet << static_cast<std::uint8_t>(ServerCommand::ADD_PLAYER) << client.first;

                        send_all(packet);
                        packet.clear();

                        if (clients.size() > 1)
                        {
                            packet << static_cast<std::uint8_t>(ServerCommand::ADD_PLAYERS);

                            for (const auto [fid, fclient] : clients)
                                if (fid != client.first)
                                    packet << fid;
                                
                            if (client.second->send(packet) != sf::Socket::Done)
                                std::cerr << "Packet couldn't be sent to client: " << client.first << std::endl;
                        }
                    }

                    else
                        delete client.second;
                }
                
                else
                {
                    for (auto [id, client] : clients)
                    {
                        if (selector.isReady(*client))
                        {
                            sf::Packet recvPacket;
                            sf::Packet sendPacket;

                            sf::Socket::Status status = client->receive(recvPacket);

                            if (status == sf::Socket::Done)
                            {
                                std::uint8_t command;
                                recvPacket >> command;

                                if (command == ClientCommand::RECEIVE_POSITION)
                                {
                                    float x, y;

                                    recvPacket >> x >> y;
                                    sendPacket << ServerCommand::RECEIVE_POSITION << id << x << y;
                                }

                                else if (command == ServerCommand::RECEIVE_AUDIO)
                                {
                                    std::cout << "Received audio samples" << std::endl;
                                    sendPacket = recvPacket;
                                }

                                for (size_t i = 0; i < clients.size(); i++)
                                {
                                    if (clients.at(i).first != id)
                                    {
                                        if (clients.at(i).second->send(sendPacket) != sf::Socket::Done)
                                            std::cout << "Couldn't send!" << std::endl;
                                    }
                                }
                            }

                            else if (status == sf::Socket::Disconnected)
                            {
                                selector.remove(*client);
                                delete client;
                                clients.erase(clients.begin() + index(id));
                            }
                        }
                    }
                }
            }
        }
    }
}

void Server::send_all(sf::Packet& packet)
{
    for (auto [fid, fclient] : clients)
        if (fclient->send(packet) != sf::Socket::Done)
            std::cerr << "Packet couldn't be sent to client: " << fid << std::endl;
}

void Server::send_except(sf::Packet& packet, const Client& client)
{
    for (auto [fid, fclient] : clients)
        if (fid != client.first)
            if (fclient->send(packet) != sf::Socket::Done)
                std::cerr << "Packet couldn't be sent to client: " << fid << std::endl;
}

void Server::setID(std::pair<std::uint32_t, sf::TcpSocket*>& client)
{
    std::uniform_int_distribution<std::uint32_t> distribution(0, UINT32_MAX);
    std::uint32_t id = distribution(generator);

    while (contains(id) || id == 0)
        id = distribution(generator);
    
    client.first = id;
}

bool Server::contains(std::uint32_t id) const
{
    for (const auto [lid, client] : clients)
        if (id == lid)
            return true;

    return false;
}


int Server::index(std::uint32_t id) const
{
    for (size_t i = 0; i < clients.size(); i++)
        if (clients.at(i).first == id)
            return i;
    
    return -1;
}