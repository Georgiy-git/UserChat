#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QMainWindow>
#include <thread>
#include <QVBoxLayout>
#include <boost/asio.hpp>
#include <QLineEdit>
#include <QLabel>

class Client;

namespace Ui {
class Window;
}

class Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Window(QString name);
    ~Window();

    bool flag_can_chat{true};

    std::shared_ptr<boost::asio::io_context> io_context;
    std::shared_ptr<Client> client;
    std::shared_ptr<std::thread> thread;

    static QString def2(QString str);

private slots:
    //Выйти
    void on_pushButton_clicked();

    //Отправить сообщение
    void on_pushButton_2_clicked();

    //Очистить чат
    void on_pushButton_3_clicked();

public slots:
    void new_message(QString message_);

    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::Window *ui;
    QString name; //Имя пользователя
    QLineEdit* line_write; //Линия создания сообщения

    std::shared_ptr<QVBoxLayout> chat_layout; //Область чата

    std::vector<std::shared_ptr<QVBoxLayout>> message_boxes;
    std::vector<std::shared_ptr<QLabel>> message_labels;

    void write_message(QString &message);
};

#endif // WINDOW_HPP
