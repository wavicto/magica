#include "session.hpp"

Session::Session(boost::asio::io_context& io_context, Server* ptr)
    :socket(io_context), server_ptr(ptr)
{}

boost::asio::ip::tcp::socket& Session::get_socket(){
    return socket;
}

void Session::handshake(){
    auto current_session = shared_from_this();
    socket.async_read_some(boost::asio::buffer(data, 1024), 
    [current_session](boost::system::error_code error, size_t length) {
        if (!error) {
            std::string msg(current_session->data, length);
            if (msg == "REQUEST"){
                std::cout << "Incoming chat request. Accept (y/n)?" << std::endl;
                char response;
                std::cin >> response;
                while (response != 'y' && response != 'n'){
                    std::cout << "Incoming chat request. Accept (y/n)?" << std::endl;
                    std::cin >> response;
                }
                if (response == 'y'){
                    current_session->read();
                }
                else if (response == 'n'){
                    current_session->socket.close();
                    std::cout << "Connection severed" << std::endl;
                }
            }
        }
        else {
            std::cerr << "Failed handshake: " << error.message() << std::endl;
        }
    });
}

void Session::read(){
    auto current_session = shared_from_this();
    socket.async_read_some(boost::asio::buffer(data, 1024), 
    [current_session](boost::system::error_code error, size_t length) {
        if (!error) {
            std::cout << "User: ";
            std::cout.write(current_session->data, length);
            std::cout << std::endl;
            current_session->read();
        } else if (error == boost::asio::error::eof) {
            std::cout << "User disconnected." << std::endl;
            current_session->server_ptr->scan();
        } else {
            std::cerr << "Failed to receive message: " << error.message() << std::endl;
        }
    });
}