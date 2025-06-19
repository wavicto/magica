#include <boost/asio.hpp>
#include <iostream>
#include <vector>
#include <memory>

class Server {
    public:

    //Initializes the user's server with a chosen port
    //Starts scanning for possible connections
    Server(boost::asio::io_context& io_context, int port);

    private:

    //Starts scanning for connections (asynchronous)
    void scan();
    
    void handle_scan(std::shared_ptr<Session> ptr, const boost::system::error_code& error);

    boost::asio::ip::tcp::endpoint endpoint;
    boost::asio::ip::tcp::acceptor acceptor;
};

//Design choice (inheriting from share from this) allows connection objects to live long enough 
//such that acceptor async operation's callback handler to safely use them
class Session : public std::enable_shared_from_this<Session>
{
    public:

    //Displays messages from this session (asynchronous)
    void read();

    private:
    std::string message;
    boost::asio::ip::tcp::socket socket;
};