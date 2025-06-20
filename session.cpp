#include "session.hpp"

Session::Session(boost::asio::io_context& io_context, Server* ptr)
    :socket(io_context), server_ptr(ptr), status(true)
{}

boost::asio::ip::tcp::socket& Session::get_socket(){
    return socket;
}

void Session::incoming_handshake(){
    socket.async_read_some(boost::asio::buffer(data, 1024), 
    [this](boost::system::error_code error, size_t length) {
        if (!error) {
            std::string msg(this->data, length);
            if (msg == "REQUEST"){
                std::cout << "Incoming chat request. Accept (y/n)?" << std::endl;
                char response;
                std::cin >> response;
                while (response != 'y' && response != 'n'){
                    std::cout << "Incoming chat request. Accept (y/n)?" << std::endl;
                    std::cin >> response;
                }
                if (response == 'y'){
                    this->read();
                }
                else if (response == 'n'){
                    this->socket.close();
                    std::cout << "Connection severed." << std::endl;
                }
            }
            else {
                std::cout << "Non-User attempted connect. Ending session" << std::endl;
                this->socket.close();
            }
        }
        else {
            std::cerr << "(Session) Failed handshake: " << error.message() << std::endl;
        }
    });
}

void Session::read(){
    socket.async_read_some(boost::asio::buffer(data, 1024), 
    [this](boost::system::error_code error, size_t length) {
        if (!error) {
            std::cout << "User: ";
            std::cout.write(this->data, length);
            std::cout << std::endl;
            this->read();
        } else if (error == boost::asio::error::eof) {
            std::cout << "User disconnected." << std::endl;
            this->server_ptr->scan(this);
        } else {
            std::cerr << "(Session) Failed to receive message: " << error.message() << std::endl;
        }
    });
}

void Session::kill_session(){
    status = false;
    socket.close();
}

void Session::restart_session(){
    status = true;
    socket = boost::asio::ip::tcp::socket(io_context);
}