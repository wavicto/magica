#include "server.hpp"
#include "session.hpp"

Server::Server(boost::asio::io_context& io_context, int port)
    :io_context(io_context),
    endpoint(boost::asio::ip::tcp::v4(), port), 
    acceptor(io_context, endpoint)
{}

void Server::scan(){
    std::cout << "Open for connections ... " << std::endl;
    std::shared_ptr<Session> session = std::make_shared<Session>(io_context, this);
    acceptor.async_accept(session->get_socket(), 
    [this, session](const boost::system::error_code& error){
        if (error){
            std::cerr << "Connection Error: " << error.message() << std::endl;
            std::cout << "Retrying ..." << std::endl;
            scan();
        }
        else if (!error){
            session->handshake();
        }
    });
}