#ifndef PRODUCTSTAB_H
#define PRODUCTSTAB_H

#include <QWidget>
#include <QSqlDatabase>
#include"DataBaseManager.h"

#include"shipmentstab.h"
namespace Ui {
class productstab;
}

class productstab : public QWidget
{
    Q_OBJECT

public:
    explicit productstab(QWidget *parent = nullptr);
    ~productstab();
signals:
    void stringChanged(const QString &d, const QString &from);
    void setTabIndex(const int index);
    void callReloadComboBoxShip();
    void stringChangedOrders(const QString &d, const QString &from);
public slots:
    void selectAll();
private slots:
    void checkNullInStock();
    void add();
    void del();
    void edit();
    void FilledTextFiledFromTableRow();
    void search();
    void getShip();
    void on_pb_order_clicked(bool checked);

    void on_pbReport_clicked(bool checked);

private:
    Ui::productstab *ui;
    QSqlDatabase db = DatabaseManager::getDatabase();
   // ShipmentsTab s;
};

#endif // PRODUCTSTAB_H
