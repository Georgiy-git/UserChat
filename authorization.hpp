#ifndef AUTHORIZATION_HPP
#define AUTHORIZATION_HPP

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>

class Authorization : public QWidget
{
    Q_OBJECT
public:
    explicit Authorization();

private:
    std::unique_ptr<QVBoxLayout> layout;
    std::unique_ptr<QLabel> login_label;
    std::unique_ptr<QLineEdit> login_edit;
    std::unique_ptr<QLabel> password_label;
    std::unique_ptr<QLineEdit> password_edit;
    std::unique_ptr<QPushButton> button_accept;
    std::unique_ptr<QVBoxLayout> layout_button;

    std::string name;

private slots:
    void accept();

signals:

};

#endif // AUTHORIZATION_HPP
