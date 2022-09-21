#pragma once

#include <cstdint>

namespace ClientCommand
{
    constexpr std::uint8_t RECEIVE_POSITION    = 0;
    constexpr std::uint8_t RECEIVE_AUDIO       = 1;
    constexpr std::uint8_t END_OF_AUDIO_STREAM = 2;
}

namespace ServerCommand
{
    constexpr std::uint8_t ADD_PLAYER          = 3;
    constexpr std::uint8_t ADD_PLAYERS         = 4;
    constexpr std::uint8_t SEND_POSITION       = 5;
    constexpr std::uint8_t RECEIVE_AUDIO       = 6;
    constexpr std::uint8_t END_OF_AUDIO_STREAM = 7;
    constexpr std::uint8_t RECEIVE_POSITION    = 8;
}