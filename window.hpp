#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QMainWindow>

namespace Ui {
class Window;
}

class Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Window();
    ~Window();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Window *ui;
};

#endif // WINDOW_HPP
