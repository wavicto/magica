#include "server.hpp"
#include "session.hpp"

using boost::asio::co_spawn;
using boost::asio::io_context;
using boost::asio::awaitable;
using boost::asio::use_awaitable;
using boost::asio::detached;
using boost::asio::ip::tcp;

Server::Server(io_context& io, int port)
    :io(io),
    endpoint(tcp::v4(), port), 
    acceptor(io, endpoint)
{}

awaitable<void> Server::scan_helper(Session* incoming_session){
    std::cout << "Open for connections ... " << std::endl;
    try {
        co_await acceptor.async_accept(incoming_session->get_socket(), use_awaitable);
        incoming_session->incoming_handshake();
    }
    catch (const boost::system::system_error& error){
        std::cerr << "(Server) Connection Error: " << error.what() << std::endl;
        std::cout << "Retry? (y/n)" << std::endl;
        //checks to see if they want to retry again (will add functionality later)
    }
    co_return;
}

void Server::scan(Session* incoming_session){
    co_spawn(
        io, 
        [this, incoming_session]() -> awaitable<void> {
            co_await scan_helper(incoming_session);
        }, 
        detached);
}

