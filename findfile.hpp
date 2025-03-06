#ifndef FINDFILE_HPP
#define FINDFILE_HPP

#include <QDialog>
#include <QFileDialog>

class Client;

namespace Ui {
class FindFile;
}

class FindFile : public QDialog
{
    Q_OBJECT

public:
    explicit FindFile(QWidget *parent = nullptr);
    ~FindFile();

    std::shared_ptr<Client> client;

    void accept() override;

private slots:
    void on_buttonBox_accepted();

    void on_pushButton_clicked();

private:
    Ui::FindFile *ui;
};

#endif // FINDFILE_HPP
