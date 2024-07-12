#include "authoriz.h"
#include "ui_authoriz.h"
#include <QSqlQuery>
#include <QSqlError>
#include "QMessageBox"

authoriz::authoriz(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::authoriz)
{
    ui->setupUi(this);
}
authoriz::~authoriz()
{
    delete ui;
}


void authoriz::on_pushButton_4_clicked()
{

    sotr =new MainWindow();
    dir = new sotrudnik();
    QString loginDir ="direktor1";
    QString passwordDir = "1234";

    QString username = ui->login->text();
    QString password = ui->password->text();
    if(username == loginDir)
    {

        if(password==passwordDir)
        {
            dir->show();
            this->close();
        }
        else{
            QMessageBox::critical(this,"Ошибка!", "Неверный пароль!");
            return;
        }

    }

    else
    {
        QSqlQuery query(db);
        query.prepare("SELECT password, user_id FROM authoriz WHERE login = :username");
        query.bindValue(":username", username);

        if (!query.exec()) {

            QMessageBox::critical(this,"Ошибка!", "Не удалось выполнить запрос. Попробуйте позднее.");
            return;

        }
        else {
            if (query.next()) {
                if( query.value(0).toString() != password)
                {
                    QMessageBox::critical(this,"Ошибка!", "Неверный пароль!");
                    return;
                }
                else
                {
                    sotr->show();
                    this->close();
                }

            } else {
                qDebug() << "Авторизация провалилась!";
                QMessageBox::critical(this,"Ошибка!", "Не удалось найти пользователя.");
                return;

            }
        }

    }



}


void authoriz::on_pushButton_5_clicked(bool checked)
{
    this->close();
}

