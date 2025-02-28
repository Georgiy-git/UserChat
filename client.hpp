#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include <string>
#include <optional>
#include <thread>
#include <fstream>
#include <QString>

#include <QObject>
#include <QMetaType>

namespace B = boost::asio;
using tcp = B::ip::tcp;
using error_code = boost::system::error_code;

class Window;

class Client : public QObject {
    Q_OBJECT

public:
    Window* window;

    Client(B::io_context& io_context, std::string ip_adres);

    //Обработка ошибки
    void check_error(error_code error, std::size_t bytes);

    //Отправка сообщения на сервер
    void write(QString message);

    //Асинхронное ожидание сообщения
    void async_read();

    //Чтение с буфера
    void read_from_buffer(const error_code& error, std::size_t bytes);

    //Отправка сообщения на сервер асинхронно
    void async_write(QString message);

    //Отправка файла
    void send_file(std::string&& file_name);

private:
    boost::system::error_code ec;
    tcp::socket socket;
    B::streambuf streambuf;
    B::io_context& io_context;
    std::string name;
    bool can_chat = false;
    std::string ip_adres;

signals:
    void signal_new_mess(QString message);
};
