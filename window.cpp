#include "authorization.hpp"
#include "newchatline.hpp"
#include "client.hpp"
#include "window.hpp"
#include "ui_window.h"
#include "onlyacceptlineedit.hpp"

#include <thread>
#include <iostream>
#include <QScrollBar>
#include <QTimer>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDialog>


Window::Window(QString name) : ui(new Ui::Window), name{name}
{
    setMinimumSize(600, 400);
    ui->setupUi(this);
    setWindowTitle("Чат \"Локальная сеть\"");
    ui->verticalLayout_2->setAlignment(Qt::AlignTop);
    ui->label_3->setText("Имя: "+name+"\nРоль: Пользователь");
    chat_layout = std::make_shared<QVBoxLayout>();
    chat_layout->setAlignment(Qt::AlignTop);
    ui->scrollArea->widget()->setStyleSheet("background-color: white;");
    ui->scrollArea->widget()->setLayout(chat_layout.get());

    ui->scrollArea_2->setFixedWidth(0);
    chats_layout = std::make_shared<QVBoxLayout>();
    chats_layout->setAlignment(Qt::AlignTop);
    ui->scrollArea_2->widget()->setLayout(chats_layout.get());
    ui->label_4->setFixedWidth(0);
    ui->pushButton_5->setFixedWidth(0);
    ui->pushButton_6->setFixedWidth(0);
    ui->pushButton_7->setFixedHeight(0);
    ui->pushButton_8->setFixedHeight(0);
    ui->label_5->setText("");
}

Window::~Window()
{
    delete ui;
    thread->detach();
}

QString Window::def2(QString str)
{
    QChar v[] {'&', '<', '>', '|', '\\'};

    if (str.trimmed().isEmpty()) {
        QMessageBox::information(nullptr, " ", "Сообщение не должно быть пустыми  ");
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

void Window::on_pushButton_clicked()
{
    exit(0);
}

void Window::on_pushButton_2_clicked()
{
    QString text { ui->lineEdit->text() };
    write_message(text);
    ui->lineEdit->setText("");

}

void Window::new_message(QString message_)
{
    std::shared_ptr<QLabel> message = std::make_shared<QLabel>(message_);
    message->setStyleSheet("border: 1px solid black; border-radius: 6px; "
                           "padding: 5px;");
    QFont font = message->font();
    font.setPointSize(12);
    message->setFont(font);

    std::shared_ptr<QVBoxLayout> message_box = std::make_shared<QVBoxLayout>();
    message_box->addWidget(message.get());
    message_box->setAlignment(Qt::AlignLeft);
    chat_layout->addLayout(message_box.get());

    message_boxes.push_back(message_box);
    message_labels.push_back(message);

    QTimer::singleShot(10, [this]() {
            ui->scrollArea->verticalScrollBar()->setValue(
                        ui->scrollArea->verticalScrollBar()->maximum());
        });
}

void Window::load_chat(QString chat)
{
    QPushButton* button = new QPushButton(chat);
    connect(button, &QPushButton::clicked, this,
            [chat, this, button]{delete_chat(chat, button);});
    connect(button, &QPushButton::clicked, this, [chat, this]{open_local_chat(chat);});

    button->setStyleSheet("QPushButton {"
                              "background-color: green;"
                              "border: none;"
                              "color: white;"
                              "margin: 1px 1px;"
                              "border-radius: 5px;"
                              "        }"
                              "QPushButton:hover {"
                              "background-color: lightgreen;"
                              "        }"
                              "QPushButton:pressed {"
                              "background-color: #388E3C;"
                              "        }");
    chats_buttons.push_back(button);
    chats_layout->addWidget(button);
}

void Window::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return ) {
        on_pushButton_2_clicked();
    } else {
        QWidget::keyPressEvent(event);
    }
}

void Window::write_message(QString message)
{
    try{
    def2(message);

    if (!flag_can_chat)
    {
        message = "Сообщение не отправлено";
    }

    std::shared_ptr<QLabel> message_ = std::make_shared<QLabel>(message);
    message_->setStyleSheet("background-color: blue; color: white; "
                            "border: 1px solid white; border-radius: 6px; padding: 5px; ");
    QFont font = message_->font();
    font.setPointSize(12);
    message_->setFont(font);

    std::shared_ptr<QVBoxLayout> message_box = std::make_shared<QVBoxLayout>();
    message_box->addWidget(message_.get());
    message_box->setAlignment(Qt::AlignRight);
    chat_layout->addLayout(message_box.get());

    message_boxes.push_back(message_box);
    message_labels.push_back(message_);

    QTimer::singleShot(10, [this]() {
            ui->scrollArea->verticalScrollBar()->setValue(
                        ui->scrollArea->verticalScrollBar()->maximum());
        });

    if (flag_can_chat)
    {
        client->write(message);
    }

    } catch(...) {}
}

void Window::open_local_chat(QString str)
{
    if (!flag_del_chat)
    {
    this_local_chat = str;
    message_labels.clear();
    message_boxes.clear();
    ui->label->setText(str);
    ui->label->setStyleSheet("background-color: green; color: white;");
    ui->pushButton_7->setFixedHeight(18);
    ui->pushButton_8->setFixedHeight(18);
    on_pushButton_4_clicked();
    ui->label_5->setText("Чат: "+str);
    client->write("&OPENLOCALCHAT&"+str);
    }
}

void Window::on_pushButton_3_clicked()
{
    message_labels.clear();
    message_boxes.clear();
}


void Window::on_pushButton_4_clicked()
{
    //При открытии
    if (flag_open_local_chat) {
        ui->scrollArea_2->setFixedWidth(250);
        ui->label_4->setFixedWidth(250);
        ui->pushButton_5->setFixedWidth(250);
        ui->pushButton_6->setFixedWidth(250);
        flag_open_local_chat = false;
        if (new_chat_line != nullptr) {
            new_chat_line->setFixedWidth(250);
        }
    } else {
        //При закрытии
        ui->scrollArea_2->setFixedWidth(0);
        ui->label_4->setFixedWidth(0);
        ui->pushButton_5->setFixedWidth(0);
        ui->pushButton_6->setFixedWidth(0);
        flag_open_local_chat = true;
        if (new_chat_line != nullptr) {
            new_chat_line->setFixedWidth(0);
        }
    }
}


void Window::on_pushButton_5_clicked()
{
    ui->pushButton_5->setFixedHeight(0);
    new_chat_line = new NewChatLine;
    new_chat_line->setPlaceholderText("Введите название");
    ui->verticalLayout_3->insertWidget(3, new_chat_line);
    connect(new_chat_line, &NewChatLine::signal_create_chat, this,
            [this](QString str){client->write("&CREATECHAT&"+str);});
}

void Window::create_chat(QString str)
{
    QPushButton* button = new QPushButton(str);
    connect(button, &QPushButton::clicked, this,
            [str, this, button]{delete_chat(str, button);});
    connect(button, &QPushButton::clicked, this, [str, this]{open_local_chat(str);});
    button->setStyleSheet("QPushButton {"
                              "background-color: green;"
                              "border: none;"
                              "color: white;"
                              "margin: 1px 1px;"
                              "border-radius: 5px;"
                              "        }"
                              "QPushButton:hover {"
                              "background-color: lightgreen;"
                              "        }"
                              "QPushButton:pressed {"
                              "background-color: #388E3C;"
                              "        }");
    chats_buttons.push_back(button);
    chats_layout->addWidget(button);
    ui->pushButton_5->setFixedHeight(18);
    delete new_chat_line;
    new_chat_line = nullptr;
}

void Window::message_box_info(QString mess)
{
    QMessageBox::information(nullptr, " ", mess);
}


void Window::on_pushButton_7_clicked()
{
    message_labels.clear();
    message_boxes.clear();
    ui->label->setText("Глобальный чат");
    ui->label->setStyleSheet("background-color: blue; color: white;");
    ui->pushButton_7->setFixedHeight(0);
    ui->pushButton_8->setFixedHeight(0);

    ui->scrollArea_2->setFixedWidth(0);
    ui->label_4->setFixedWidth(0);
    ui->pushButton_5->setFixedWidth(0);
    ui->pushButton_6->setFixedWidth(0);
    flag_open_local_chat = true;
    if (new_chat_line != nullptr) {
        new_chat_line->setFixedWidth(0);
    }
    ui->label_5->setText("");
    this_local_chat.clear();
    client->write("&OPENGLOBALCHAT&");
}

void Window::on_pushButton_8_clicked()
{
    QDialog *dialog = new QDialog;
    dialog->setWindowTitle("Добавить участника");
    dialog->setWindowIcon(QIcon(":/Images/mine.svg"));
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    OnlyAcceptLineEdit *line = new OnlyAcceptLineEdit;
    line->setPlaceholderText("Введите имя пользователя    ");
    layout->addWidget(line);
    dialog->setLayout(layout);
    connect(line, &OnlyAcceptLineEdit::signal_key_return, this,
            [this, line, dialog]{add_user_to_chat(Authorization::def(def2(line->text().trimmed())), this_local_chat);
        dialog->close();} );
    dialog->show();
}

void Window::add_user_to_chat(QString user_name, QString chat_name)
{
    client->write("&ADDUSERTOCHAT&"+user_name+'|'+chat_name);
}

void Window::on_pushButton_6_clicked()
{
    if (!flag_del_chat) {
        for (auto &i : chats_buttons) {
            i->setStyleSheet("QPushButton {"
                             "background-color: red;"
                             "border: none;"
                             "color: white;"
                             "margin: 1px 1px;"
                             "border-radius: 5px;"
                             "        }"
                             "QPushButton:hover {"
                             "background-color: #FFB6C1;"
                             "        }");
        }
        flag_del_chat = true;
    } else {
        for (auto &i : chats_buttons) {
            i->setStyleSheet("QPushButton {"
                             "background-color: green;"
                             "border: none;"
                             "color: white;"
                             "margin: 1px 1px;"
                             "border-radius: 5px;"
                             "        }"
                             "QPushButton:hover {"
                             "background-color: lightgreen;"
                             "        }");
        }
        flag_del_chat = false;
    }
}

void Window::delete_chat(QString chat, QPushButton* button)
{
    if (flag_del_chat) {
        client->write("&DELETECHAT&"+chat);
        auto it = std::remove(chats_buttons.begin(), chats_buttons.end(), button);
        chats_buttons.erase(it, chats_buttons.end());
        delete button;
        button = nullptr;
    }
}

