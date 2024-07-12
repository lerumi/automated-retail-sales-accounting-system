#ifndef WORKERTAB_H
#define WORKERTAB_H

#include <QWidget>
#include <QSqlDatabase>
#include"DataBaseManager.h"

namespace Ui {
class workertab;
}

class workertab : public QWidget
{
    Q_OBJECT

public:
    explicit workertab(QWidget *parent = nullptr);
    ~workertab();
signals:
    void setTabIndex(const int index);
    void stringChanged(const QString &d);
public slots:
    void selectAll(const QString &d);
private slots:
    void add();
    void del();
    void edit();
    void FilledTextFiledFromTableRow();
    void search();
    void getOrders();
    void callEmptySelectAll();
    void reloadComboBox();
    void on_cb_storage_currentTextChanged(const QString &arg1);

    void on_le_storage_id_textChanged(const QString &arg1);

    void on_pbReport_clicked(bool checked);

private:
    Ui::workertab *ui;
    QSqlDatabase db = DatabaseManager::getDatabase();
};

#endif // WORKERTAB_H
