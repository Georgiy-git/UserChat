#include "window.hpp"
#include "authorization.hpp"

Authorization::Authorization()
{
    setFixedSize(400, 280);
    setWindowTitle("Авторизация");
    layout = std::make_unique<QVBoxLayout>();
    layout->setSpacing(10);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setAlignment(Qt::AlignTop);
    setLayout(layout.get());

    login_label = std::make_unique<QLabel>("Введите Ваше имя:");

    login_edit = std::make_unique<QLineEdit>();
    QFont font1 = login_edit->font();
    font1.setPointSize(12);
    login_edit->setFont(font1);
    login_edit->setFixedHeight(30);

    password_label = std::make_unique<QLabel>("Введите пароль:");

    password_edit = std::make_unique<QLineEdit>();
    QFont font2 = password_edit->font();
    font2.setPointSize(12);
    password_edit->setFont(font2);
    password_edit->setFixedHeight(30);

    button_accept = std::make_unique<QPushButton>("Вход");
    layout_button = std::make_unique<QVBoxLayout>();
    layout_button->setAlignment(Qt::AlignHCenter);
    layout_button->setContentsMargins(0, 30, 0, 0);
    layout_button->addWidget(button_accept.get());
    connect(button_accept.get(), &QPushButton::clicked, this, &Authorization::accept);

    layout->addWidget(login_label.get());
    layout->addWidget(login_edit.get());
    layout->addWidget(password_label.get());
    layout->addWidget(password_edit.get());
    layout->addLayout(layout_button.get());
}

void Authorization::accept()
{

    Window *window = new Window;
    window->show();
    delete this;
}
