#include "../include/AudioCapturer.hpp"

AudioCapturer::AudioCapturer(Client& client) 
    : offset(0), client(client)
{
    initialize(1, 44100);
}

bool AudioCapturer::onGetData(sf::SoundStream::Chunk& data)
{
    while (offset >= samples.size())
        sf::sleep(sf::milliseconds(10));

    {
        std::scoped_lock lock(mutex);
        tempbuffer.assign(samples.begin() + static_cast<std::vector<std::int64_t>::difference_type>(offset),
                            samples.end());
        std::cout << "Tempbuffer address: " << &tempbuffer << std::endl;
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
    while (true)
    {
        if (client.receivedAudio)
        {
            sf::Packet copy = packet;
            std::uint8_t command;

            if (copy >> command)
            {
                std::size_t sampleCount = (copy.getDataSize() - 1) / sizeof(std::int16_t);
                {
                    std::scoped_lock lock(mutex);
                    std::size_t      oldSize = samples.size();
                    samples.resize(oldSize + sampleCount);
                    std::cout << "Samples address: " << &samples << std::endl;
                    std::cout << "Last item address: " << &samples[oldSize] << std::endl;
                    std::memcpy(&(samples[oldSize]),
                                static_cast<const char*>(copy.getData()) + 1,
                                sampleCount * sizeof(std::int16_t));
                }

                client.receivedAudio = false;
            }
        }
    }
}