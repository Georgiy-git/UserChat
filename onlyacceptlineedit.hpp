#ifndef ONLYACCEPTLINEEDIT_HPP
#define ONLYACCEPTLINEEDIT_HPP

#include <QLineEdit>
#include <QKeyEvent>

class OnlyAcceptLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    OnlyAcceptLineEdit();

    void keyPressEvent(QKeyEvent *event) override;

signals:
    void signal_key_return();
};

#endif // ONLYACCEPTLINEEDIT_HPP
