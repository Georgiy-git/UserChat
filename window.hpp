#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QMainWindow>
#include <thread>
#include <QVBoxLayout>
#include <boost/asio.hpp>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <list>

class Client;
class NewChatLine;
class FindFile;

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
    bool flag_del_chat{false};
    bool flag_can_files{true};
    bool flag_del_files{false};

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

    //Локальные чаты
    void on_pushButton_4_clicked();

    //Создать чат
    void on_pushButton_5_clicked();

    //Синхронная отправка сообщения
    void write_message(QString message);

    //Открыть локальный чат
    void open_local_chat(QString name);

    //Вернуться в глобальный чат
    void on_pushButton_7_clicked();

    //Добавить участника чата
    void on_pushButton_8_clicked();

    //Добавить пользователя в чат
    void add_user_to_chat(QString user_name, QString chat_name);

    //Удалить чаты
    void on_pushButton_6_clicked();

    //Слот удаления чата
    void delete_chat(QString chat, QPushButton *button);

    //Команды
    void on_pushButton_9_clicked();

    //Файловая система
    void on_pushButton_10_clicked();

    //Загрузить файл на сервер
    void on_pushButton_11_clicked();

public slots:
    void new_message(QString message_);

    void load_chat(QString chat);

    void keyPressEvent(QKeyEvent *event) override;

    //Слот создания чата
    void create_chat(QString str);

    //Создать сообщение
    void message_box_info(QString);

    //Создать кнопу файла
    void new_file(QString);

    //Выгрузить файл
    void load_file(QString);

private:
    Ui::Window *ui;
    QString name; //Имя пользователя
    QLineEdit* line_write; //Линия создания сообщения
    bool flag_open_local_chat{true};
    QString this_local_chat;
    FindFile* ffile {nullptr};

    std::shared_ptr<QVBoxLayout> chat_layout; //Область чата
    std::shared_ptr<QVBoxLayout> chats_layout; //Область локальных чатов
    std::shared_ptr<QVBoxLayout> files_layout; //Область файлов
    NewChatLine* new_chat_line = nullptr; //Линия создания чата

    std::vector<std::shared_ptr<QVBoxLayout>> message_boxes;
    std::vector<std::shared_ptr<QLabel>> message_labels;
    std::vector<QPushButton*> chats_buttons;
    std::vector<QPushButton*> files_buttons;

    //Анимации плавного открытия

    //Открытие чатов
    void _anim_1();

    //Закрытие чатов
    void _anim_2();

    //Открытие файлов
    void _anim_3();

    //Закрытие файлов
    void _anim_4();

signals:
    void signal_1();
    void signal_2();
    void signal_3();
    void signal_4();
};

#endif // WINDOW_HPP
