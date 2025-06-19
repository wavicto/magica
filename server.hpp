#include <boost/asio.hpp>
#include <iostream>
#include <vector>
#include <memory>

//Forward Declaration
class Session;

class Server {
    public:

    //Initializes the user's server with a chosen port
    //Starts scanning for possible connections
    Server(boost::asio::io_context& io_context, int port);

    private:

    //Starts scanning for connections (asynchronous)
    void scan(boost::asio::io_context& io_context);
    
    boost::asio::ip::tcp::endpoint endpoint;
    boost::asio::ip::tcp::acceptor acceptor;
};
