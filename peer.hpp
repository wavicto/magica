#include "server.hpp"
#include "session.hpp"

class Peer {
    public:

    Peer(boost::asio::io_context& io_context, int port);

    void appear_online();

    void connect();
    
    private:
    Server server;
    Session incoming_session;
    Session outgoing_session;
};

