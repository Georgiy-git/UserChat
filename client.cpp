#include "client.hpp"

Client::Client(B::io_context& io_context) : io_context{ io_context }, socket(io_context) {
    auto address = B::ip::make_address_v4("192.168.166.31", ec);
    if (ec) {
        std::wcerr << converter.from_bytes("Ошибка: "); std::cout << ec.message() << std::endl;
        std::exit(1);
    }

    tcp::endpoint endpoint(address, 53888);

    socket.connect(endpoint, ec);
    if (ec == B::error::connection_refused) {
        std::wcout << converter.from_bytes("Сервер не ответил.") << std::endl;
        std::exit(1);
    }
    if (ec) {
        std::wcerr << converter.from_bytes("Ошибка подключения: "); std::cout << ec.message() << std::endl;
        std::exit(1);
    }
}

void Client::async_read()
{
    B::async_read_until(socket, streambuf, '\f',
        [this](error_code error, std::size_t bytes) {
            read_from_buffer(error, bytes);
        });
}

void Client::read_from_buffer(const error_code& error, std::size_t bytes)
{
    if (error) {
        if (error == B::error::eof) {
            std::wcerr << converter.from_bytes("Сервер разорвал соединение.") << std::endl;
        }
        else {
            std::wcout << converter.from_bytes("Ошибка: "); std::cout << error.message() << std::endl;
        }
        return;
    }

    std::istream input(&streambuf);
    std::string line;
    std::getline(input, line, '\f');

    if (can_chat)
    {
        int size_delete = name.size() + 10;
        std::string delete_name(size_delete, ' ');
        std::cout << '\r' << delete_name << '\r' << line << name << ": " << std::flush;
    }
    else
    {
        std::cout << line;
    }

    async_read();
}

void Client::async_write(std::string message)
{
    B::async_write(socket, B::buffer(message.data(), message.size()),
        [this](error_code error, std::size_t bytes) {
            if (error) {
                std::wcerr << converter.from_bytes("Ошибка отправки: "); 
                    std::cout << error.message() << std::endl;
                return;
            }
        });
}

void Client::input()
{
    std::thread([this]() {
        std::string message;
        while (true) {
            std::cout << name << ": ";
            std::getline(std::cin, message);
            if (message.starts_with("/send file")) {
                send_file(message.substr(12));
            }
            else {
                async_write(message + '\n');
            }
        }
        }).detach();
}

void Client::input_name()
{
    std::thread([this]() {
        std::string message;
        std::getline(std::cin, message);
        async_write(message + '\n');
        name = message;
        can_chat = true; 
        input();
        }).detach();
}

void Client::send_file(std::string&& file_name)
{
    std::ifstream file(file_name, std::ios::binary);
    if (!file) {
        std::wcout << converter.from_bytes("Не удалось открыть файл.") << std::endl;
        return;
    }


}
