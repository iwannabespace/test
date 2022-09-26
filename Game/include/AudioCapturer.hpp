#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <cstring>
#include <iostream>
#include <iterator>
#include <mutex>
#include "Client.hpp"

class AudioCapturer : public sf::SoundStream
{
public:
    AudioCapturer(Client& client);
    void receiveLoop(sf::Packet& packet);
private:
    bool onGetData(sf::SoundStream::Chunk& data) override;
    void onSeek(sf::Time timeOffset) override;
private:
    std::recursive_mutex mutex;
    std::vector<std::int16_t> samples;
    std::vector<std::int16_t> tempbuffer;
    size_t offset;
    Client& client;
};