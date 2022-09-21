#pragma once

#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Client.hpp"

class AudioRecorder : public sf::SoundRecorder
{
public:
    AudioRecorder(Client& client);
    ~AudioRecorder() override;
    void activeness();
    void setActive(bool active);
    bool isActive() const;
private:
    bool onStart() override;
    bool onProcessSamples(const std::int16_t* samples, std::size_t sampleCount) override;
    void onStop() override;
private:
    Client& client;
    bool active;
    std::mutex mutex;
};