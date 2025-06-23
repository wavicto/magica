#include "server.hpp"
#include <string>

class Session {
    public:

    //Initiates a session
    Session(boost::asio::io_context& io_context, Server* ptr);

    //Verifies incoming session
    void incoming_handshake();

    //Verifies outgoing session
    void outgoing_handshake();

    //Renders session inactive
    void kill_session();

    //Restarts session
    void restart_session();

    //Returns socket
    boost::asio::ip::tcp::socket& get_socket();

    private:

    //Displays messages from this session (asynchronous)
    void read();

    //Sends messages from this session (asynchronous)
    void send();

    boost::asio::ip::tcp::socket socket;
    Server* server_ptr;
    bool status;
    boost::asio::io_context& io;
    char output[1024];
    char input[1024];
};