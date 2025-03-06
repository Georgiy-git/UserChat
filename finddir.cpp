#include "client.hpp"
#include "finddir.hpp"
#include "ui_finddir.h"

#include <QMessageBox>
#include <QFileDialog>

FindDir::FindDir(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDir)
{
    ui->setupUi(this);
    setFixedSize(400, 140);
    setWindowTitle("Поиск папки");
    setWindowIcon(QIcon(":/Images/mine.svg"));
}

FindDir::~FindDir()
{
    delete ui;
}

void FindDir::accept()
{

}

void FindDir::on_pushButton_clicked()
{
    ui->lineEdit->setText(QFileDialog::getExistingDirectory(this,
        tr("Выбрать папку"), QDir::homePath(),
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));
}


void FindDir::on_buttonBox_2_accepted()
{
    Dir = ui->lineEdit->text().trimmed();
    if (Dir.isEmpty()) {
        QMessageBox::information(nullptr, " ", "Поле не должно быть пустым  ");
    }
    else if (file.isEmpty()) {
        QMessageBox::information(nullptr, " ", "Название файла не должно быть пустым  ");
    }
    else {
        client->dir = Dir.toStdString();
        client->write("&LOADMEFILE&"+file);
        delete this;
    }
}

