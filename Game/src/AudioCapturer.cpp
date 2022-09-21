#include "../include/AudioCapturer.hpp"

AudioCapturer::AudioCapturer(Client& client) 
    : offset(0), finished(false), client(client)
{
    initialize(1, 44100);
}

void AudioCapturer::start()
{
    if (!finished)
        play();
}

bool AudioCapturer::onGetData(sf::SoundStream::Chunk& data)
{
    if ((offset >= samples.size()) && finished)
        return false;

    while ((offset >= samples.size()) && !finished)
        sf::sleep(sf::milliseconds(10));

    {
        std::scoped_lock lock(mutex);
        tempbuffer.assign(samples.begin() + static_cast<std::vector<std::int64_t>::difference_type>(offset),
                            samples.end());
    }

    data.samples     = tempbuffer.data();
    data.sampleCount = tempbuffer.size();

    offset += tempbuffer.size();

    return true;
}

void AudioCapturer::onSeek(sf::Time timeOffset)
{
    offset = static_cast<std::size_t>(timeOffset.asMilliseconds()) * getSampleRate() * getChannelCount() / 1000;
}

void AudioCapturer::receiveLoop(sf::Packet& packet)
{
    while (!finished)
    {    
        sf::Packet copy = packet;

        if (client.receivedData)
        {
            std::cout << "Evet" << std::endl;
        
            std::uint8_t command;
            copy >> command;

            if (command == ServerCommand::RECEIVE_AUDIO)
            {
                std::size_t sampleCount = (copy.getDataSize() - 1) / sizeof(std::int16_t);

                {
                    std::scoped_lock lock(mutex);
                    std::size_t      oldSize = samples.size();
                    samples.resize(oldSize + sampleCount);
                    std::memcpy(&(samples[oldSize]),
                                static_cast<const char*>(copy.getData()) + 1,
                                sampleCount * sizeof(std::int16_t));
                    client.receivedData = false;
                }
            }

            else if (command == ServerCommand::END_OF_AUDIO_STREAM)
            {
                std::cout << "Audio data has been 100% received!" << std::endl;
                finished = true;
                client.receivedData = false;
            }

            else
                finished = true;
        }
    }
}