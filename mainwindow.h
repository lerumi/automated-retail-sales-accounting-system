#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include "shipmentstab.h"
#include "productstab.h"
#include "orderstab.h"
#include"customerstab.h"
#include"providerstab.h"
#include"workertab.h"
#include"storagestab.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    void connection(const int index);
private slots:
    void on_pushButton_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    ShipmentsTab *tabWidget_ship;
    productstab *tabWidget_pr;
    orderstab *tabWidget_order;
    customerstab *tabWidget_customer;
    providerstab *tabWidget_provider;
    workertab *tabWidget_worker;
    storagestab *tabWidget_storage;
    //authoriz *autho;

};
#endif // MAINWINDOW_H
