#include "server.hpp"
#include "session.hpp"

Server::Server(boost::asio::io_context& io_context, int port)
    :io_context(io_context),
    endpoint(boost::asio::ip::tcp::v4(), port), 
    acceptor(io_context, endpoint)
{}

void Server::scan(Session* incoming_session){
    std::cout << "Open for connections ... " << std::endl;
    acceptor.async_accept(incoming_session->get_socket(), 
    [this, incoming_session](const boost::system::error_code& error){
        if (error){
            std::cerr << "(Server) Connection Error: " << error.message() << std::endl;
            std::cout << "Retry? (y/n)" << std::endl;
            //checks to see if they want to retry again
            scan(incoming_session);
        }
        else if (!error){
            incoming_session->handshake();
        }
    });
}