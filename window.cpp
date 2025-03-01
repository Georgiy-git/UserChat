#include "client.hpp"
#include "window.hpp"
#include "ui_window.h"

#include <thread>
#include <iostream>
#include <QScrollBar>
#include <QTimer>
#include <QKeyEvent>
#include <QMessageBox>

Window::Window(QString name) : ui(new Ui::Window), name{name}
{
    setFixedSize(800, 600);
    ui->setupUi(this);
    setWindowTitle("Чат \"Локальная сеть\"");
    ui->verticalLayout_2->setAlignment(Qt::AlignTop);
    ui->label_3->setText("Имя: "+name+"\nРоль: Пользователь");
    chat_layout = std::make_shared<QVBoxLayout>();
    chat_layout->setAlignment(Qt::AlignTop);
    ui->scrollArea->widget()->setStyleSheet("background-color: white;");
    ui->scrollArea->widget()->setLayout(chat_layout.get());
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

void Window::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return ) {
        on_pushButton_2_clicked();
    } else {
        QWidget::keyPressEvent(event);
    }
}

void Window::write_message(QString &message)
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

void Window::on_pushButton_3_clicked()
{
    message_labels.clear();
    message_boxes.clear();
}

