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
    void start_scan(Session* incoming_session);

    void start_connect(Session* outgoing_session);

    private:

    boost::asio::awaitable<void> scan(Session* incoming_session);
    boost::asio::awaitable<void> connect();

    boost::asio::io_context& io;
    boost::asio::ip::tcp::endpoint endpoint;
    boost::asio::ip::tcp::acceptor acceptor;
};
