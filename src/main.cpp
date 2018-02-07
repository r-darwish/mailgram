#include "server.hpp"
#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>

int main(int argc, char * argv[])
{
    try {
        if (argc != 2) {
            std::cerr << "Usage: mailgram <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;
        mailgram::Server server(io_service, std::atoi(argv[1]));
        io_service.run();
    } catch (std::exception & e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
