#include <SFML/Network.hpp>
#include <iostream>
#include "include/Server.hpp"

int main()
{
    Server server(4242);

    server.receive();

    return 0;
}