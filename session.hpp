#include "server.hpp"
#include <string>

class Session {
    public:

    Session(boost::asio::io_context& io_context, Server* ptr);

    void incoming_handshake();

    void outgoing_handshake();

    void kill_session();

    void restart_session();

    boost::asio::ip::tcp::socket& get_socket();

    private:

    //Displays messages from this session (asynchronous)
    void read();

    //Sends messages from this
    void write();

    boost::asio::ip::tcp::socket socket;
    Server* server_ptr;
    bool status;
    char data[1024];
};