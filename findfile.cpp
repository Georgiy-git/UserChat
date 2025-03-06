#include "client.hpp"
#include "findfile.hpp"
#include "client.hpp"
#include "ui_findfile.h"

#include <fstream>
#include <iostream>
#include <QMessageBox>
#include <QString>

FindFile::FindFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindFile)
{
    ui->setupUi(this);
}

FindFile::~FindFile()
{
    delete ui;
}

void FindFile::accept()
{

}

void FindFile::on_buttonBox_accepted()
{
    QString qfile = ui->lineEdit->text().trimmed();
    std::string file_path = qfile.toStdString();

    qfile.remove(0, qfile.lastIndexOf('/')+1);
    client->write("&LOADFILE&"+qfile);

    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        QMessageBox::information(nullptr, " ", "Файл для передачи не коткрыт  ");
        return;
    }

    char vek[100];

    while (file.good()) {
        std::streamsize size = file.read(vek, 100).gcount();
        if (size == 0) {
            break;
        }
        boost::asio::write(client->socket, boost::asio::buffer(vek, size));
    }

    client->write("\f");
}


void FindFile::on_pushButton_clicked()
{
    ui->lineEdit->setText(QFileDialog::getOpenFileName(this, "Поиск файла", QDir::homePath(),
        "Все файлы (*.*));;"));
}

