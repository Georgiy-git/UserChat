#include "client.hpp"
#include "window.hpp"
#include "authorization.hpp"

#include <QMessageBox>
#include <format>
#include <iostream>
#include <QKeyEvent>
#include <string>

Authorization::Authorization()
{
    setFixedSize(400, 350);
    setWindowTitle("Авторизация");
    layout = std::make_unique<QVBoxLayout>();
    layout->setSpacing(10);
    layout->setContentsMargins(30, 10, 30, 20);
    layout->setAlignment(Qt::AlignTop);
    setLayout(layout.get());


    //Логин
    login_label = std::make_unique<QLabel>("Введите Ваше имя:");

    login_edit = std::make_unique<QLineEdit>();
    QFont font1 = login_edit->font();
    font1.setPointSize(12);
    login_edit->setFont(font1);
    login_edit->setFixedHeight(30);


    //Пароль
    password_label = std::make_unique<QLabel>("Введите пароль:");

    password_edit = std::make_unique<QLineEdit>();
    QFont font2 = password_edit->font();
    font2.setPointSize(12);
    password_edit->setFont(font2);
    password_edit->setFixedHeight(30);


    //Ip сервера
    ip_label = std::make_unique<QLabel>("Введите IPv4 Сервера:");

    ip_edit = std::make_unique<QLineEdit>();
    QFont font3 = ip_edit->font();
    font3.setPointSize(12);
    ip_edit->setFont(font2);
    ip_edit->setFixedHeight(30);


    //Кнопка запомнить
    memory_label = std::make_unique<QLabel>("Запомнить");
    memory_button = std::make_unique<QCheckBox>();
    memory_layout = std::make_unique<QHBoxLayout>();
    memory_layout->setAlignment(Qt::AlignLeft);
    memory_layout->setContentsMargins(0, 5, 5, 5);
    memory_layout->addWidget(memory_label.get());
    memory_layout->addWidget(memory_button.get());


    //Кнопка входа
    button_accept = std::make_unique<QPushButton>("Вход");
    button_accept->setStyleSheet("QPushButton {"
                                "background-color: blue; "
                                "border: none; "
                                "color: white; "
                                "margin: 1px 1px; "
                                "border-radius: 5px; "
                                "        } "
                                "QPushButton:hover {"
                                "background-color: lightblue; "
                                "        }"
                                "QPushButton:pressed {"
                                "background-color: #21618C;"
                                "        }");
    button_accept->setFixedSize(100, 30);
    layout_button = std::make_unique<QVBoxLayout>();
    layout_button->setAlignment(Qt::AlignHCenter);
    layout_button->setContentsMargins(0, 30, 0, 0);
    layout_button->addWidget(button_accept.get());
    connect(button_accept.get(), &QPushButton::clicked, this, &Authorization::accept_button);


    //Добавление в виджет
    layout->addWidget(login_label.get());
    layout->addWidget(login_edit.get());
    layout->addWidget(password_label.get());
    layout->addWidget(password_edit.get());
    layout->addWidget(ip_label.get());
    layout->addWidget(ip_edit.get());
    layout->addLayout(memory_layout.get());
    layout->addLayout(layout_button.get());


    //База данных
    sqlite3_open("info.db", &db);
    int ok = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS info(login TEXT, "
                                     "password TEXT, ip TEXT, memory TEXT);",
                                       nullptr, nullptr, nullptr);

    ok = sqlite3_exec(db, "SELECT * FROM info;", callback, nullptr, nullptr);
    if (!callback_result.empty()) {
        login_edit->setText(callback_result[0]);
        password_edit->setText(callback_result[1]);
        ip_edit->setText(callback_result[2]);
        memory_button->setCheckState(Qt::Checked);
    }

    if (ok) { QMessageBox::critical(nullptr, " ", "Ошибка Базы Данных ");}
}

QString Authorization::def(QString str)
{
    QChar v[] {'&', '<', '>', ' ', '|', '@', '!', '\"', '\'', '%', '#', '$', '^', '?', '\\',
              '+', '=', '-', '(', ')', '*', ',', '/', '~'};

    if (str.isEmpty()) {
        QMessageBox::information(nullptr, " ", "Поля не должны быть пустыми  ");
        throw "empty";
    }

    for (QChar& i : v) {
        if (str.contains(i)) {
            QMessageBox::information(nullptr, " ", "Использованы неразрешённые символы  ");
            throw "simvols";
        }
    }

    return str;
}

QString Authorization::def1(QString str)
{
    if (str.size() < 4) {
        QMessageBox::information(nullptr, " ", "В поле меньше 4 символов  ");
        throw "litle";
    }
    return str;
}

int Authorization::callback(void *data, int argc, char **argv, char **azColName)
{
    char* y = argv[3];
    if (std::string(y) == "Yes") {
        QString login_ = QString::fromUtf8(argv[0]);
        QString password_ = QString::fromUtf8(argv[1]);
        QString ip_ = QString::fromUtf8(argv[2]);
        callback_result.push_back(login_);
        callback_result.push_back(password_);
        callback_result.push_back(ip_);
    }

    return 0;
}

void Authorization::accept_button()
{
    try{ //Нач

    if (memory_button->checkState() == Qt::Checked) {
        memory();
    } else {
        sqlite3_exec(db, "DELETE FROM info;", nullptr, nullptr, nullptr);
    }

    name = def1(def(login_edit->text().trimmed()));
    QString password = def1(def(password_edit->text().trimmed()));
    QString ip = def(ip_edit->text().trimmed());
    std::string ip_ = ip.toUtf8().constData();

    try
    {
        if (client == nullptr)
        {
            io_context = std::make_shared<B::io_context>();
            client = std::make_shared<Client>(*io_context, ip_);

            connect(client.get(), &Client::signal_server_ok, this, server_ok);
            connect(client.get(), &Client::signal_server_off, this, server_off);

            thread = std::make_shared<std::thread>
                    ([this] { client->async_read(); io_context->run(); });

            client->write("&ENTER&"+name+'|'+password);
        }
        else {
            client->write("&ENTER&"+name+'|'+password);
        }
    } catch (...) {}

    } //Кон
    catch (...) {}
}

void Authorization::memory()
{
    QString login = login_edit->text().trimmed();
    QString password = password_edit->text().trimmed();
    QString ip = ip_edit->text().trimmed();

    std::string login_ = login.toUtf8().constData();
    std::string password_ = password.toUtf8().constData();
    std::string ip_ = ip.toUtf8().constData();

    int ok=sqlite3_exec(db, "DELETE FROM info;", nullptr, nullptr, nullptr);
    std::string sql = std::format("INSERT INTO info (login, password, ip, memory) "
                                  "VALUES ('{}', '{}', '{}', 'Yes');",
                    login_, password_, ip_);
    ok=sqlite3_exec(db, sql.c_str(),
                    nullptr, nullptr, nullptr);

    if (ok) { QMessageBox::critical(nullptr, " ", "Ошибка Базы Данных ");}
}

void Authorization::server_ok()
{
    Window *window = new Window(name);

    //Передаю указатели для продления жизни
    window->io_context = io_context;
    window->client = client;
    window->thread = thread;

    connect(client.get(), &Client::signal_new_mess, window, Window::new_message);
    connect(client.get(), &Client::signal_stop, window,
            [window]{window->flag_can_chat=false;});

    window->show();
    sqlite3_close(db);
    delete this;
}

void Authorization::server_off()
{
    password_edit->setText("");
    password_edit->setPlaceholderText("неправильный пароль");
}

void Authorization::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return) {
        accept_button();
    }
    else {
        QWidget::keyPressEvent(event);
    }
}
