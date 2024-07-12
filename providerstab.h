#ifndef PROVIDERSTAB_H
#define PROVIDERSTAB_H

#include <QWidget>
#include <QSqlDatabase>
#include"DataBaseManager.h"

namespace Ui {
class providerstab;
}

class providerstab : public QWidget
{
    Q_OBJECT

public:
    explicit providerstab(QWidget *parent = nullptr);
    ~providerstab();
signals:
    void setTabIndex(const int index);
    void stringChanged(const QString &d, const QString &from);
public slots:
    void selectAll();
private slots:
    void add();
    void del();
    void edit();
    void FilledTextFiledFromTableRow();
    void search();
    void getOrders();
    void on_pbReport_clicked(bool checked);

private:
    Ui::providerstab *ui;
    QSqlDatabase db = DatabaseManager::getDatabase();
};

#endif // PROVIDERSTAB_H
