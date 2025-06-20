#include "server.hpp"
#include <string>

//Design choice (inheriting from share from this) allows connection objects to live long enough 
//such that acceptor async operation's callback handler to safely use them
class Session : public std::enable_shared_from_this<Session>
{
    public:

    Session(boost::asio::io_context& io_context, Server* ptr);

    void handshake();

    boost::asio::ip::tcp::socket& get_socket();

    private:

    //Displays messages from this session (asynchronous)
    void read();

    boost::asio::ip::tcp::socket socket;
    Server* server_ptr;
    char data[1024];
};