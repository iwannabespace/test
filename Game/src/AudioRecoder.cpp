
#include "../include/AudioRecorder.hpp"


AudioRecorder::AudioRecorder(Client& client) 
    : client(client), active(false)
{
}

AudioRecorder::~AudioRecorder()
{
    stop();
}

void AudioRecorder::activeness()
{
    while (true)
    {   
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !active)
        {
            std::scoped_lock lock(mutex);
            active = true;
            if (start()) {}
        }

        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && active)
        {
            std::scoped_lock lock(mutex);
            stop();
            active = false;
        }
    }
}

void AudioRecorder::setActive(bool active)
{
    this->active = active;
}
    
bool AudioRecorder::isActive() const
{
    return active;
}

bool AudioRecorder::onStart()
{
    std::cout << "Started!" << std::endl;

    return true;
}

bool AudioRecorder::onProcessSamples(const std::int16_t* samples, std::size_t sampleCount)
{
    sf::Packet packet;
    packet << ServerCommand::RECEIVE_AUDIO;
    packet.append(samples, sampleCount * sizeof(std::int16_t));

    return client.send(packet);
}
    
void AudioRecorder::onStop()
{
    std::cout << "Stopped!" << std::endl;
}