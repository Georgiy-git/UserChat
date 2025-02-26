#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include <string>
#include <optional>
#include <thread>
#include <fstream>
#include <codecvt>

namespace B = boost::asio;
using tcp = B::ip::tcp;
using error_code = boost::system::error_code;

class Client {

public:

    Client(B::io_context& io_context);

    //Асинхронное ожидание сообщения
    void async_read();

    //Чтение с буфера
    void read_from_buffer(const error_code& error, std::size_t bytes);

    //Отправка сообщения на сервер
    void async_write(std::string message);

    //Обычный ввод
    void input();

    //Ввод имени
    void input_name();

    //Отправка файла
    void send_file(std::string&& file_name);

private:
    boost::system::error_code ec;
    tcp::socket socket;
    B::streambuf streambuf;
    B::io_context& io_context;
    std::string name;
    bool can_chat = false;
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
};