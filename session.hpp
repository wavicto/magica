#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include "server.hpp"

//Design choice (inheriting from share from this) allows connection objects to live long enough 
//such that acceptor async operation's callback handler to safely use them
class Session : public std::enable_shared_from_this<Session>
{
    public:

    Session(boost::asio::io_context& io_context, Server* ptr);

    //Displays messages from this session (asynchronous)
    void read();

    boost::asio::ip::tcp::socket& get_socket();

    private:
    boost::asio::ip::tcp::socket socket;
    Server* server_ptr;
    std::string message;
};