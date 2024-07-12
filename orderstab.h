#ifndef ORDERSTAB_H
#define ORDERSTAB_H

#include <QWidget>
#include <QSqlDatabase>
#include"DataBaseManager.h"

namespace Ui {
class orderstab;
}

class orderstab : public QWidget
{
    Q_OBJECT

public:
    explicit orderstab(QWidget *parent = nullptr);
    ~orderstab();
public slots:
    void selectAll(const QString &d, const QString &from);
    void reloadComboBox();
private slots:

    void add();
    void del();
    void edit();
    void FilledTextFiledFromTableRow();
    void search();
    void callEmptySelectAll();
    void selectItems();
    void FilledTextFiledFromTableRowItems();
    void addItems();
    void delItem();
    void editItem();

    void on_cbPrItem_currentTextChanged(const QString &arg1);

    void on_cbCustomer_currentTextChanged(const QString &arg1);

    void on_cbWorker_currentTextChanged(const QString &arg1);

    void on_leCustomer_textChanged(const QString &arg1);

    void on_leWorker_textChanged(const QString &arg1);

    void on_le_prId_textChanged(const QString &arg1);

    void on_pbReport_clicked(bool checked);

private:
    Ui::orderstab *ui;
    QSqlDatabase db = DatabaseManager::getDatabase();
};

#endif // ORDERSTAB_H
