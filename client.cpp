#include "authorization.hpp"
#include "window.hpp"
#include "client.hpp"

#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QWidget>
#include <QStandardPaths>
#include <fstream>

Client::Client(B::io_context& io_context, std::string ip_adres) :
    io_context{ io_context }, socket(io_context), ip_adres{ip_adres} {
    auto address = B::ip::make_address_v4(ip_adres, ec);
    if (ec) {

        std::exit(1);
    }

    tcp::endpoint endpoint(address, 53888);

    socket.connect(endpoint, ec);
    if (ec == B::error::connection_refused) {
        QMessageBox::information(nullptr, " ", "Сервер не ответил   ");
        throw "сервер не ответил";
    }
    if (ec) {
        QMessageBox::information(nullptr, " ", "Ошибка подключения");
        throw "Ошибка подключения";
    }
}

void Client::write(QString message)
{
    message+=QChar('\n');
    std::string message_ = message.toStdString();
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
            emit signal_new_mess("<snap style='color: blue;'>"
                            "Сервер разорвал соединение</snap>");
            emit signal_stop();
        }
        else {
            emit signal_new_mess("<span style='color: blue;'>"
                            "На сервере произошёл сбой</snap>");
            emit signal_stop();
        }
        return;
    }

    std::istream input(&streambuf);
    std::string line;
    std::getline(input, line, '\f');

    qDebug() << line;

    if (line == "&NON_PASSWORD&") {
        emit signal_server_off();

    } else if (line == "&OK_PASSWORD&") {
        emit signal_server_ok();

    } else if (line.starts_with("&MYMESSAGE&")) {
        line.erase(0, line.rfind('&') + 1);
        emit signal_my_mess(QString::fromUtf8(line.c_str()));

    } else if (line.starts_with("&LOADCHAT&")) {
        line.erase(0, line.rfind('&') + 1);
        emit signal_load_chat(QString::fromUtf8(line.c_str()));

    } else if (line.starts_with("&CHATCREATEOK&")) {
        line.erase(0, line.rfind('&') + 1);
        emit signal_create_chat_ok(QString::fromUtf8(line.c_str()));

    } else if (line == "&CHATALREADYCREATED&") {
        emit signal_create_chat_off("Чат с таким именем уже существует ");

    } else if (line == "&USERNOINDB&") {
        emit signal_create_chat_off("Пользователь не найден или уже в чате ");

    } else if (line == "&PROFILINBANLIST&") {
        emit signal_in_ban();

    } else if (line.starts_with("&OKLOADFILE&")) {
        line.erase(0, line.rfind('&') + 1);
        emit signal_ok_load_file(QString::fromUtf8(line.c_str()));

    } else if (line.starts_with("&LOADFILE&")) {
        line.erase(0, line.rfind('&') + 1);
        emit signal_ok_load_file(QString::fromUtf8(line.c_str()));

    } else if (line.starts_with("&LOADMEFILE&")) {
        line.erase(0, line.rfind('&') + 1);
        load_me(line);
        return;

    } else {
        emit signal_new_mess(QString::fromUtf8(line.c_str()));
    }

    async_read();
}

void Client::async_write(QString message)
{
    std::string message_ = message.toStdString();
    B::async_write(socket, B::buffer(message_.data(), message_.size()),
            [this](error_code error, std::size_t bytes) {
            read_from_buffer(error, bytes);
            });
}

void Client::load_me(std::string line)
{
    file.open(dir+"/"+line, std::ios::binary);
    dir.clear();

    if (!file.is_open()) {
        std::cerr << "Error open file" << std::endl;
        async_read();
        return;
    }

    B::async_read_until(socket, streambuf, '\f',
            [this](boost::system::error_code ec, std::size_t bytes_transferred)
        {
            if (ec) {
                std::cerr << "Error during async_read_until: " << ec.message() << std::endl;
                file.close();
                async_read();
                return;
            }

            std::istream input(&streambuf);
            std::string str;
            std::getline(input, str, '\f');

            file.write(str.data(), str.size());
            file.close();

            async_read();
        });
}
