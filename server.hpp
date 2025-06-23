#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/this_coro.hpp>
#include <iostream>

//Forward Declaration
class Session;

class Server {
    public:

    //Initializes the user's server with a chosen port
    Server(boost::asio::io_context& io_context, int port);

    //Starts scanning for connections (asynchronous)
    void scan(Session* incoming_session);

    private:

    boost::asio::awaitable<void> scan_helper(Session* incoming_session);

    boost::asio::io_context& io;
    boost::asio::ip::tcp::endpoint endpoint;
    boost::asio::ip::tcp::acceptor acceptor;
};
