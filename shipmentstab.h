#ifndef SHIPMENTSTAB_H
#define SHIPMENTSTAB_H

#include <QWidget>
#include <QSqlDatabase>
#include"DataBaseManager.h"

namespace Ui {
class ShipmentsTab;
}


class ShipmentsTab : public QWidget
{
    Q_OBJECT

public:
    explicit ShipmentsTab(QWidget *parent = nullptr);
    ~ShipmentsTab();
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
    void fillTextFromTableReOrder();

    void on_cb_pr_currentTextChanged(const QString &arg1);

    void on_cbShip_currentTextChanged(const QString &arg1);

    void on_cbStorage_currentTextChanged(const QString &arg1);

    void on_leProvider_textChanged(const QString &arg1);

    void on_lestorage_textChanged(const QString &arg1);

    void on_leProduct_textChanged(const QString &arg1);


    void on_pbReport_clicked(bool checked);

signals:
    void selectPr();
private:
    Ui::ShipmentsTab *ui;
    QSqlDatabase db = DatabaseManager::getDatabase();
};

#endif // SHIPMENTSTAB_H
