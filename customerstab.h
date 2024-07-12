#ifndef CUSTOMERSTAB_H
#define CUSTOMERSTAB_H

#include <QWidget>
#include <QSqlDatabase>
#include"DataBaseManager.h"
namespace Ui {
class customerstab;
}

class customerstab : public QWidget
{
    Q_OBJECT

public:
    explicit customerstab(QWidget *parent = nullptr);
    ~customerstab();
signals:
    void setTabIndex(const int index);
    void stringChangedOrders(const QString &d, const QString &from);
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
    Ui::customerstab *ui;
    QSqlDatabase db = DatabaseManager::getDatabase();
};

#endif // CUSTOMERSTAB_H
