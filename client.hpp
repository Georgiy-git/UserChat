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
class Authorization;

class Client : public QObject {
    Q_OBJECT

public:
    Client(B::io_context& io_context, std::string ip_adres);

    tcp::socket socket;
    std::string dir;

    //Отправка сообщения на сервер
    void write(QString message);

    //Асинхронное ожидание сообщения
    void async_read();

    void async_read_file();

    //Чтение с буфера
    void read_from_buffer(const error_code& error, std::size_t bytes);

    void read_file_from_buffer(const error_code& error, std::size_t bytes);

    //Отправка сообщения на сервер асинхронно
    void async_write(QString message);

    //Отправка файла
    void send_file(std::string&& file_name);

    void load_me(std::string);

private:
    boost::system::error_code ec;
    B::streambuf streambuf;
    B::io_context& io_context;
    std::string name;
    bool can_chat = false;
    std::string ip_adres;
    std::ofstream file;

signals:
    void signal_new_mess(QString message);
    void signal_server_ok(); //Разрешение на подключение
    void signal_server_off(); //Отказ
    void signal_stop(); //Отключение возможность отправлять сообщения
    void signal_load_chat(QString str); //Загрузка чата
    void signal_create_chat_ok(QString);
    void signal_create_chat_off(QString);
    void signal_in_ban();
    void signal_ok_load_file(QString);
};
