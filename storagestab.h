#ifndef STORAGESTAB_H
#define STORAGESTAB_H

#include <QWidget>
#include <QSqlDatabase>
#include"DataBaseManager.h"
namespace Ui {
class storagestab;
}

class storagestab : public QWidget
{
    Q_OBJECT

public:
    explicit storagestab(QWidget *parent = nullptr);
    ~storagestab();
signals:
    void setTabIndex(const int index);
    void stringChanged(const QString &d, const QString &from);
    void stringChangedCustomer(const QString &d);
public slots:
    void selectAll();
private slots:
    void add();
    void del();
    void edit();
    void FilledTextFiledFromTableRow();
    void search();
    void getOrders();
    void on_pb_ship_clicked(bool checked);

    void on_pb_worker_clicked(bool checked);

    void on_pbReport_clicked(bool checked);

private:
    Ui::storagestab *ui;
    QSqlDatabase db = DatabaseManager::getDatabase();
};

#endif // STORAGESTAB_H
