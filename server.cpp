#include "server.hpp"

Server::Server(boost::asio::io_context& io_context, int port)
    :endpoint(boost::asio::ip::tcp::v4(), port), 
    acceptor(io_context, endpoint)
{
    scan();
}

void Server::scan(){

}


/*

