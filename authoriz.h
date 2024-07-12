#ifndef AUTHORIZ_H
#define AUTHORIZ_H

#include <QDialog>
#include <QSqlDatabase>
#include"DataBaseManager.h"
#include"mainwindow.h"
#include"sotrudnik.h"
#include"customer_window.h"
namespace Ui {
class authoriz;
}

class authoriz : public QDialog
{
    Q_OBJECT

public:
    explicit authoriz(QWidget *parent = nullptr);
    ~authoriz();
private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked(bool checked);

private:
    Ui::authoriz *ui;
    QSqlDatabase db = DatabaseManager::getDatabase();
    MainWindow *sotr;
    sotrudnik *dir;
};

#endif // AUTHORIZ_H
