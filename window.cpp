#include "window.hpp"
#include "ui_window.h"

Window::Window() : ui(new Ui::Window)
{
    setFixedSize(800, 600);
    ui->setupUi(this);
    setWindowTitle("Чат по локальной сети");
    ui->verticalLayout_2->setAlignment(Qt::AlignTop);
    ui->label_3->setText("Имя: Гога\nРоль: Пользователь");
}

Window::~Window()
{
    delete ui;
}

void Window::on_pushButton_clicked()
{
    exit(0);
}

