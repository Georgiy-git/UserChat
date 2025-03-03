#include "onlyacceptlineedit.hpp"

OnlyAcceptLineEdit::OnlyAcceptLineEdit()
{

}

void OnlyAcceptLineEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return)  {
        emit signal_key_return();
    } else {
        QLineEdit::keyPressEvent(event);
    }
}
