#include "window.hpp"
#include "client.hpp"

#include <QVBoxLayout>
#include <QLabel>

Client::Client(B::io_context& io_context, std::string ip_adres) :
    io_context{ io_context }, socket(io_context), ip_adres{ip_adres} {
    auto address = B::ip::make_address_v4(ip_adres, ec);
    if (ec) {
        //std::wcerr << converter.from_bytes("Ошибка: "); std::cout << ec.message() << std::endl;
        std::exit(1);
    }

    tcp::endpoint endpoint(address, 53888);

    socket.connect(endpoint, ec);
    if (ec == B::error::connection_refused) {
        //std::wcout << converter.from_bytes("Сервер не ответил.") << std::endl;
        std::exit(1);
    }
    if (ec) {
        //std::wcerr << converter.from_bytes("Ошибка подключения: "); std::cout << ec.message() << std::endl;
        std::exit(1);
    }
}

void Client::check_error(error_code error, std::size_t bytes)
{

}

void Client::write(QString message)
{
    message+=QChar('\n');
    std::string message_ = message.toUtf8().constData();
    socket.send(B::buffer(message_.data(), message_.size()));
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
            //std::wcerr << converter.from_bytes("Сервер разорвал соединение.") << std::endl;
        }
        else {
            //std::wcout << converter.from_bytes("Ошибка: "); std::cout << error.message() << std::endl;
        }
        return;
    }

    std::istream input(&streambuf);
    std::string line;
    std::getline(input, line, '\f');

    QString message = QString::fromUtf8(line.c_str());

    emit signal_new_mess(message);

    async_read();
}

void Client::async_write(QString message)
{
    std::string message_ = message.toUtf8().constData();
    B::async_write(socket, B::buffer(message_.data(), message_.size()),
            [this](error_code error, std::size_t bytes) {
            read_from_buffer(error, bytes);
            });
}

void Client::send_file(std::string&& file_name)
{
    std::ifstream file(file_name, std::ios::binary);
    if (!file) {
        //std::wcout << converter.from_bytes("Не удалось открыть файл.") << std::endl;
        return;
    }
}
