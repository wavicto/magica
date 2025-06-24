#include "server.hpp"
#include <boost/asio/posix/stream_descriptor.hpp>
#include <string>

class Session {
    public:

    //Initiates a session
    Session(boost::asio::io_context& io_context, Server* ptr);

    //Verifies incoming session
    void process_handshake();

    //Verifies outgoing session
    void send_handshake();

    //Renders session inactive
    void kill_session();

    //Restarts session
    void restart_session();

    //Returns socket
    boost::asio::ip::tcp::socket& get_socket();

    private:

    //Displays messages from this session (asynchronous)
    boost::asio::awaitable<void> read();

    //Sends messages from this session (asynchronous)
    boost::asio::awaitable<void> send(boost::asio::posix::stream_descriptor& input_stream);

    boost::asio::awaitable<void> incoming_handshake();
    void msg_confirmation(std::string msg);

    boost::asio::awaitable<void> outgoing_handshake();
    void send_confirmation(std::string msg);

    boost::asio::ip::tcp::socket socket;
    Server* server_ptr;
    bool status;
    boost::asio::io_context& io;
    boost::asio::streambuf output;
    boost::asio::streambuf input;
};