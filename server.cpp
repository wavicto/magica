#include "server.hpp"
#include "session.hpp"

Server::Server(boost::asio::io_context& io_context, int port)
    :endpoint(boost::asio::ip::tcp::v4(), port), 
    acceptor(io_context, endpoint)
{
    scan(io_context);
}

void Server::scan(boost::asio::io_context& io_context){
    std::shared_ptr<Session> session = std::make_shared<Session>(io_context, this);
    acceptor.async_accept(session->get_socket(), 
    [this, &io_context, session](const boost::system::error_code& error){
        if (!error){
            session->read();
        }
        else {
            std::cerr << "Incoming Connection Error: " << error.message() << std::endl;
            std::cout << "Retrying...\n";
            scan(io_context);
        }
    });
}