#pragma once
#include "text.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <string>

namespace mailgram {

class Session {
public:
    Session(boost::asio::ip::tcp::socket socket) : socket(std::move(socket)) {}

    void start()
    {
        write_line("220 mailgram -- Server ESMTP (MSG)\r\n");
        read_line();
    }

private:
    void write_line(const std::string line)
    {
        boost::asio::async_write(
            socket, boost::asio::buffer(line),
            [line = std::move(line)](boost::system::error_code /* ec */, std::size_t /* length */){});
    }

    void write_ok() { write_line("250 OK\r\n"); }

    void write_error() { write_line("500 Error\r\n"); }

    void read_line()
    {
        auto line_handler = [this](boost::system::error_code ec, std::size_t /* size */) {
            if (!ec) {
                const auto line = [&] {
                    std::string line;
                    std::istream is(&data);
                    std::getline(is, line);
                    const auto last_char = line.size() - 1;
                    if (line[last_char] == '\r') {
                        line.erase(last_char, 1);
                    }
                    return std::move(line);
                }();

                if (handle_line(line)) {
                    read_line();
                    return;
                } else {
                    std::cout << "Disconnection\n";
                }
            }
            delete this;
        };

        boost::asio::async_read_until(socket, data, "\r\n", std::move(line_handler));
    }

    bool handle_command(const std::string & line, const std::string_view & command,
                        WordExtractor & word_extractor);
    bool handle_line(const std::string & line);

private:
    enum class State { Connected, Negotiated, Data, BodyReceived };
    State state = State::Connected;
    std::string from;
    std::string to;
    std::ostringstream body;
    boost::asio::streambuf data;
    boost::asio::ip::tcp::socket socket;
};

} // namespace mailgram
