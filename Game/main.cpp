#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <thread>
#include <future>
#include <unordered_map>
#include "include/Client.hpp"
#include "include/Player.hpp"
#include "include/StartScreen.hpp"
#include "include/AudioRecorder.hpp"
#include "include/AudioCapturer.hpp"
#include <cmath>
#include <array>

int main()
{
    std::srand(std::time(0));

    Client client("127.0.0.1", 4242);

    if (client.connect())
    {
        std::cout << "Connected to server!" << std::endl;

        sf::RenderWindow window(sf::VideoMode({ 1280, 720 }), "Multiplayer Game", sf::Style::Default);
        window.setVerticalSyncEnabled(true);

        sf::View view;
        view.setCenter({ 640, 360 });
        view.setSize(sf::Vector2f(window.getSize()));

        sf::Packet packet;

        std::unordered_map<std::uint32_t, Player> players;
        std::uint32_t thisPlayer = 0;

        StartScreen startscreen(window, sf::Color(172, 192, 146));
        AudioRecorder recorder(client);
        AudioCapturer capturer(client);
        capturer.play();

        std::thread receivePacket;
        std::thread receiverloop;
        std::thread activeness;

        while (window.isOpen())
        {
            sf::Event event;

            while (window.pollEvent(event))
            {
                switch (event.type)
                {
                    case sf::Event::Closed:
                        window.close();
                        break;

                    case sf::Event::Resized:
                        if (startscreen)
                        {    
                            view.setCenter({ 
                                static_cast<float>(window.getSize().x) / 2,
                                static_cast<float>(window.getSize().y) / 2
                            });
                            view.setSize(sf::Vector2f(window.getSize()));
                            
                            startscreen.updateOnResize();
                        }
                        break;
                }
            }

            if (!receivePacket.joinable())
                receivePacket = std::thread(&Client::receivePacket, std::ref(client), std::ref(packet), std::ref(players), std::ref(thisPlayer));
            
            if (!receiverloop.joinable())
                receiverloop = std::thread(&AudioCapturer::receiveLoop, std::ref(capturer), std::ref(packet));

            if (!activeness.joinable())
                activeness = std::thread(&AudioRecorder::activeness, std::ref(recorder));

            if (client.receivedPosition)
            {
                sf::Packet copy = packet;
                std::uint8_t command;

                if (copy >> command)
                {
                    float x, y;
                    std::uint32_t id;

                    copy >> id >> x >> y;

                    for (auto& player : players)
                        if (player.first == id)
                            player.second.setPosition({ x, y });

                    client.receivedPosition = false;
                }
            }

            if (window.hasFocus())
                players[thisPlayer].move(client);

            window.clear(sf::Color::White);
            window.setView(view);
            
            for (const auto& player : players)
                window.draw(player.second);

            window.display();
        }

        client.disconnect();
        std::terminate();
    }

    return 0;
}