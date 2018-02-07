#pragma once
#include "session.hpp"
#include <boost/asio.hpp>
#include <memory>

namespace mailgram {

class Server {
public:
    Server(boost::asio::io_service & io_service, const short port) :
        acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
        socket(io_service)
    {
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor.async_accept(socket, [this](boost::system::error_code ec) {
            if (!ec) {
                auto session = std::make_unique<Session>(std::move(socket));
                session->start(std::move(session));
            }

            do_accept();
        });
    }

private:
    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::ip::tcp::socket socket;
};

} // namespace mailgram
