#include "session.hpp"

using boost::asio::co_spawn;
using boost::asio::io_context;
using boost::asio::awaitable;
using boost::asio::use_awaitable;
using boost::asio::detached;
using boost::asio::ip::tcp;

Session::Session(io_context& io, Server* ptr)
    :socket(io), server_ptr(ptr), status(true), io(io)
{}

tcp::socket& Session::get_socket(){
    return socket;
}

void Session::process_handshake(){
    co_spawn(
        io, 
        [this]() -> awaitable<void> {
            co_await incoming_handshake();
        }, 
        detached);
}

awaitable<void> Session::incoming_handshake(){
    try {
        size_t length = co_await socket.async_read_some(boost::asio::buffer(output, 1024), use_awaitable);
        std::string msg(this->output, length);
        msg_confirmation(msg);
        if (status){
            this->read();
        }
    }
    catch (const boost::system::system_error& error){
        std::cerr << "(Session) Failed handshake: " << error.what() << std::endl;
    }
    co_return;
}

void Session::read(){

}

awaitable<void> Session::read(){
    try{
        size_t length = co_await socket.async_read_some(boost::asio::buffer(output, 1024), use_awaitable);
        std::cout << "User: ";
        std::cout.write(this->output, length);
        std::cout << std::endl;
        this->read();
    }
    catch (const boost::system::system_error& error){
        if (error == boost::asio::error::eof) {
            std::cout << "User disconnected." << std::endl;
        }
        else {
            std::cerr << "(Session) Failed to receive message: " << error.what() << std::endl;
        }
    }
}



void Session::kill_session(){
    status = false;
    socket.close();
}

void Session::restart_session(){
    status = true;
    socket = tcp::socket(io);
}

void Session::msg_confirmation(std::string msg){
    if (msg == "REQUEST"){
        std::cout << "Incoming chat request. Accept (y/n)?" << std::endl;
        char response;
        std::cin >> response;
        while (response != 'y' && response != 'n'){
            std::cout << "Incoming chat request. Accept (y/n)?" << std::endl;
            std::cin >> response;
        }
        if (response == 'y'){
            return;
        }
        else if (response == 'n'){
            kill_session();
            std::cout << "Connection severed." << std::endl;
        }
    }
    else {
        kill_session();
        std::cout << "Incorrect request token. Connection severed.";
    }
}