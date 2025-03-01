#ifndef AUTHORIZATION_HPP
#define AUTHORIZATION_HPP

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include "client.hpp"
#include <QCheckBox>
#include <QHBoxLayout>
#include "sqlite3.h"
#include <vector>
#include <QString>

class Authorization : public QWidget
{
    Q_OBJECT
public:
    explicit Authorization();

    static QString def(QString str); //Общая проверка полей
    static QString def1(QString str); //Проверка на количество символов

private:
    std::unique_ptr<QVBoxLayout> layout;
    std::unique_ptr<QLabel> login_label;
    std::unique_ptr<QLineEdit> login_edit;
    std::unique_ptr<QLabel> password_label;
    std::unique_ptr<QLineEdit> password_edit;
    std::unique_ptr<QLabel> ip_label;
    std::unique_ptr<QLineEdit> ip_edit;
    std::unique_ptr<QHBoxLayout> memory_layout;
    std::unique_ptr<QLabel> memory_label;
    std::unique_ptr<QCheckBox> memory_button;
    std::unique_ptr<QPushButton> button_accept;
    std::unique_ptr<QVBoxLayout> layout_button;

    QString name;
    std::shared_ptr<Client> client;
    std::shared_ptr<B::io_context> io_context;
    std::shared_ptr<std::thread> thread;
    sqlite3 *db;

    static int callback(void *data, int argc, char **argv, char **azColName);
    static inline std::vector<QString> callback_result;

    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void memory(); //Запомнить

public slots:
    void server_ok(); //Положительный ответ от сервена на вход
    void server_off(); //Отрицательный ответ от сервена на вход
    void accept_button(); //Кнопка вход
};

#endif // AUTHORIZATION_HPP
