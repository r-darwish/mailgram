#pragma once
#include <boost/asio.hpp>
#include <iostream>
#include <string>

namespace mailgram {

class Session {
public:
    Session(boost::asio::ip::tcp::socket && socket) : socket(std::move(socket)) {}

    void start(std::unique_ptr<Session> && self) { read_line(std::move(self)); }

private:
    void read_line(std::unique_ptr<Session> && self)
    {
        auto line_handler = [this, self = std::move(self)](boost::system::error_code ec, std::size_t)
        {
            if (!ec) {
                std::istream is(&data);
                is.unsetf(std::ios_base::skipws);
                std::string line;
                is >> line;
                std::cout << line << '\n';
                //read_line(std::move(self));
            }
        };

        boost::asio::async_read_until(socket, data, "\r\n", std::move(line_handler));
    }

private:
    boost::asio::streambuf data;
    boost::asio::ip::tcp::socket socket;
};
} // namespace mailgram
