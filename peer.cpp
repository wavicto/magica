#include "peer.hpp"

Peer::Peer(boost::asio::io_context& io_context, int port)
    :server(io_context, port), 
    incoming_session(io_context, &server), 
    outgoing_session(io_context, &server)
{}

void Peer::appear_online(){
    server.scan(&incoming_session);
}
