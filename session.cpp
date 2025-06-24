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

void Session::send_handshake(){
    co_spawn(
        io, 
        [this]() -> awaitable<void> {
            co_await outgoing_handshake();
        }, 
        detached);
}

awaitable<void> Session::incoming_handshake(){
    try {
        size_t length = co_await boost::asio::async_read_until(socket, output, '\n', use_awaitable);
        std::string message;
        std::istream stream(&output);
        std::getline(stream, message);
        msg_confirmation(message);
        output.consume(length);
        if (status){
            boost::asio::posix::stream_descriptor input_stream(io, ::dup(STDIN_FILENO));
            co_spawn(
                io, 
                [this]() -> awaitable<void> {
                    co_await read();
                }, 
                detached);
            co_spawn(
                io, 
                [this, &input_stream]() -> awaitable<void> {
                    co_await send(input_stream);
                }, 
                detached);
        }
    }
    catch (const boost::system::system_error& error){
        std::cerr << "(Session) Failed handshake: " << error.what() << std::endl;
    }
    co_return;
}

awaitable<void> Session::outgoing_handshake(){
    try {
        std::string key = "REQUEST\n";
        co_await boost::asio::async_write(socket, boost::asio::buffer(key), use_awaitable);
        boost::asio::posix::stream_descriptor input_stream(io, ::dup(STDIN_FILENO));
        co_spawn(
            io, 
            [this]() -> awaitable<void> {
                co_await read();
            }, 
            detached);
        co_spawn(
            io, 
            [this, &input_stream]() -> awaitable<void> {
                co_await send(input_stream);
            }, 
            detached);
    }
    catch (const boost::system::system_error& error){
        std::cerr << "(Session) Failed to send message: " << error.what() << std::endl;
    }
}

boost::asio::awaitable<void> Session::send(boost::asio::posix::stream_descriptor& input_stream){
    try {
        while (true){
            size_t length = co_await boost::asio::async_read_until(input_stream, input, '\n', use_awaitable);
            co_await boost::asio::async_write(socket, input, use_awaitable);
            input.consume(length);
        }
    }
    catch (const boost::system::system_error& error){
        if (error.code() == boost::asio::error::eof) {
            std::cout << "User disconnected." << std::endl;
        }
        else {
            std::cerr << "(Session) Failed to send message: " << error.what() << std::endl;
        }
    }
}

awaitable<void> Session::read(){
    try{
        while (true){
            size_t length = co_await boost::asio::async_read_until(socket, output, '\n', use_awaitable);
            std::string message;
            std::istream stream(&output);
            std::getline(stream, message);
            std::cout << "User: " << message << std::endl;
            output.consume(length);
        }
    }
    catch (const boost::system::system_error& error){
        if (error.code() == boost::asio::error::eof) {
            std::cout << "User disconnected." << std::endl;
        }
        else {
            std::cerr << "(Session) Failed to receive message: " << error.what() << std::endl;
        }
    }
    co_return;
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