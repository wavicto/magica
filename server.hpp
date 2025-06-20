#include <boost/asio.hpp>
#include <iostream>

//Forward Declaration
class Session;

class Server {
    public:

    //Initializes the user's server with a chosen port
    Server(boost::asio::io_context& io_context, int port);

    //Starts scanning for connections (asynchronous)
    void scan(Session* session);

    private:

    boost::asio::io_context& io_context;
    boost::asio::ip::tcp::endpoint endpoint;
    boost::asio::ip::tcp::acceptor acceptor;
};
