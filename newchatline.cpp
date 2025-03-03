#include "authorization.hpp"
#include "newchatline.hpp"

NewChatLine::NewChatLine()
{
    setStyleSheet("border: none;");
    setFixedSize(250, 25);
}

void NewChatLine::keyPressEvent(QKeyEvent *event)
{
    try {

    if(event->key() == Qt::Key_Return) {

        emit signal_create_chat(Authorization::def1(Authorization::def(text().trimmed())));

    } else {
        QLineEdit::keyPressEvent(event);
    }

    } catch(...) {}
}
