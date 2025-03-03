#ifndef NEWCHATLINE_H
#define NEWCHATLINE_H

#include <QLineEdit>
#include <QKeyEvent>

class NewChatLine : public QLineEdit
{
    Q_OBJECT
public:
    NewChatLine();

    void keyPressEvent(QKeyEvent *event) override;

signals:
    void signal_create_chat(QString name);
};

#endif // NEWCHATLINE_H
