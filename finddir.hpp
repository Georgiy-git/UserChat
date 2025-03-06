#ifndef FINDDIR_HPP
#define FINDDIR_HPP

#include <QDialog>
class Client;

namespace Ui {
class FindDir;
}

class FindDir : public QDialog
{
    Q_OBJECT

public:
    explicit FindDir(QWidget *parent = nullptr);
    ~FindDir();

    void accept() override;

    QString Dir;
    std::shared_ptr<Client> client;
    QString file;

private slots:
    void on_pushButton_clicked();

    void on_buttonBox_2_accepted();

private:
    Ui::FindDir *ui;
};

#endif // FINDDIR_HPP
