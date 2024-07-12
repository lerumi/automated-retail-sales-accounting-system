#ifndef SOTRUDNIK_H
#define SOTRUDNIK_H

#include <QMainWindow>
#include <QSqlDatabase>
#include "shipmentstab.h"
#include "productstab.h"
#include "orderstab.h"
#include"customerstab.h"
#include"providerstab.h"
#include"workertab.h"
#include"storagestab.h"
//#include"authoriz.h"


namespace Ui {
class sotrudnik;
}

class sotrudnik : public QMainWindow
{
    Q_OBJECT

public:
    explicit sotrudnik(QWidget *parent = nullptr);
    ~sotrudnik();
public slots:
    void connection(const int index);
private slots:
    void on_pushButton_clicked(bool checked);

private:
    Ui::sotrudnik *ui;
    ShipmentsTab *tabWidget_ship;
    productstab *tabWidget_pr;
    orderstab *tabWidget_order;
    customerstab *tabWidget_customer;
    providerstab *tabWidget_provider;
    workertab *tabWidget_worker;
    storagestab *tabWidget_storage;
   // authoriz *autho;
};

#endif // SOTRUDNIK_H
