#include "session.hpp"

Session::Session(boost::asio::io_context& io_context, Server* ptr)
    :socket(io_context), server_ptr(ptr)
{}

boost::asio::ip::tcp::socket& Session::get_socket(){
    return socket;
}

void Session:read(){

}