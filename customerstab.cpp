#include "customerstab.h"
#include "ui_customerstab.h"
#include <QSqlQuery>
#include <QSqlError>
#include "QMessageBox"

customerstab::customerstab(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::customerstab)
{
    ui->setupUi(this);
    //dbconnect();
    //selectAll();
    connect(ui->pb_new_pr,SIGNAL(clicked(bool)),this, SLOT(add()));
    connect(ui->pbDeletePr,SIGNAL(clicked(bool)),this, SLOT(del()));
    connect(ui->pbEditPr, SIGNAL(clicked(bool)), this, SLOT(edit()));
    connect(ui->twCustomers, SIGNAL(cellClicked(int, int)), this, SLOT(FilledTextFiledFromTableRow()));
    connect(ui->pbSearch, SIGNAL(clicked(bool)), this, SLOT(search()));
    connect(ui->pb_order, SIGNAL(clicked(bool)), this, SLOT(getOrders()));
    connect(ui->pbUpdateColumn, SIGNAL(clicked(bool)), this, SLOT(selectAll()));

    ui->twCustomers->setColumnCount(8);
    ui->twCustomers->setAutoScroll(true);
    ui->twCustomers->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twCustomers->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->twCustomers->setHorizontalHeaderItem(0,new QTableWidgetItem("Код покупателя"));
    ui->twCustomers->setHorizontalHeaderItem(1,new QTableWidgetItem("Название компании"));
    ui->twCustomers->setHorizontalHeaderItem(2,new QTableWidgetItem("Фамилия"));
    ui->twCustomers->setHorizontalHeaderItem(3,new QTableWidgetItem("Имя"));
    //ui->twCustomers->setHorizontalHeaderItem(4,new QTableWidgetItem("Адрес доставки"));
    ui->twCustomers->setHorizontalHeaderItem(4,new QTableWidgetItem("Город"));
    ui->twCustomers->setHorizontalHeaderItem(5,new QTableWidgetItem("Индекс"));
    ui->twCustomers->setHorizontalHeaderItem(6,new QTableWidgetItem("Телефон"));
    ui->twCustomers->setHorizontalHeaderItem(7,new QTableWidgetItem("Почта"));
    selectAll();
    ui->twCustomers->horizontalHeader()->setStretchLastSection(true);
    ui->twCustomers->setSortingEnabled(true);
    ui->twCustomers->setEditTriggers(QAbstractItemView::NoEditTriggers);




}
void customerstab:: getOrders()
{
    int selectedRow = ui->twCustomers->currentRow();
    if( selectedRow < 0 )
    {
        QMessageBox::critical(this,"Ошибка!","Ни один столбец не выбран!");
        return;
    }
    emit stringChangedOrders(ui->twCustomers->item(selectedRow, 0)->text(), "customers");
    emit setTabIndex(2);

}
void customerstab:: FilledTextFiledFromTableRow()
{
    int selectedRow = ui->twCustomers->currentRow();
    if (selectedRow >= 0){
        ui->le_company_name->setText(ui->twCustomers->item(selectedRow, 1)->text());
        ui->le_last_name->setText(ui->twCustomers->item(selectedRow, 2)->text());
        ui->le_first_name->setText(ui->twCustomers->item(selectedRow, 3)->text());
       // ui->le_address->setText(ui->twCustomers->item(selectedRow, 4)->text());
        ui->le_city->setText(ui->twCustomers->item(selectedRow, 4)->text());
        ui->le_index->setText(ui->twCustomers->item(selectedRow, 5)->text());
        ui->le_phone->setText(ui->twCustomers->item(selectedRow, 6)->text());
        ui->le_mail->setText(ui->twCustomers->item(selectedRow, 7)->text());

    }
    else{
        ui->le_company_name->clear();
        ui->le_last_name->clear();
        ui->le_first_name->clear();
        //ui->le_address->clear();
        ui->le_city->clear();
        ui->le_index->clear();
        ui->le_phone->clear();
        ui->le_mail->clear();
    }



}

void customerstab::selectAll()
{
    QString sqlstr = "select * from customers";
    QSqlQuery query(db);
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    if( query.isActive())
        ui->twCustomers->setRowCount( query.size());
    else
        ui->twCustomers->setRowCount(0);

    int i=0;
    while(query.next())
    {
        ui->twCustomers->setItem(i,0,new
                               QTableWidgetItem(query.value("customer_id").toString()));
        ui->twCustomers->setItem(i,1,new
                               QTableWidgetItem(query.value("company_name").toString()));
        ui->twCustomers->setItem(i,2,new
                               QTableWidgetItem(query.value("last_name").toString()));
        ui->twCustomers->setItem(i,3,new
                               QTableWidgetItem(query.value("first_name").toString()));

        ui->twCustomers->setItem(i,4,new
                               QTableWidgetItem(query.value("city").toString()));
        ui->twCustomers->setItem(i,5,new
                                 QTableWidgetItem(query.value("index_code").toString()));
        ui->twCustomers->setItem(i,6,new
                                 QTableWidgetItem(query.value("phone").toString()));
        ui->twCustomers->setItem(i,7,new
                                 QTableWidgetItem(query.value("e_mail").toString()));

        i++;
    }

}
void customerstab::search()
{
    QString keyword = ui->leSearchPr->text();

    for(int row = 0; row < ui->twCustomers->rowCount(); ++row) {
        bool match = false;
        for(int col = 0; col < ui->twCustomers->columnCount(); ++col) {
            QTableWidgetItem *item = ui->twCustomers->item(row, col);
            if(item && item->text().contains(keyword, Qt::CaseInsensitive)) {
                match = true;
                break;
            }
        }
        ui->twCustomers->setRowHidden(row, !match);
    }
}
void customerstab::add()
{
    QSqlQuery query(db);
    QString sqlstr = "insert into customers(company_name,last_name,first_name,city, index_code, phone, e_mail) values( ?, ?, ? ,?, ?, ?, ?)";
    query.prepare(sqlstr);
    query.bindValue(0,ui->le_company_name->text());
    query.bindValue(1,ui->le_last_name->text());
    query.bindValue(2,ui->le_first_name->text());
    //query.bindValue(3, ui->le_address->text());
    query.bindValue(3,ui->le_city->text());
    query.bindValue(4,ui->le_index->text());
    query.bindValue(5,ui->le_phone->text());
    query.bindValue(6, ui->le_mail->text());
    if( !query.exec() )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    selectAll();
}
void customerstab::del()
{
    int currow = ui->twCustomers->currentRow();
    if( currow < 0 )
    {
        QMessageBox::critical(this,"Ошибка!","Ни один столбец не выбран!");
        return;
    }

    if( QMessageBox::question(this,"Удаление","Удалить запись?",
                              QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
        return;
    QSqlQuery query(db);

    QString sqlstr = "delete from customers where customer_id = '"
                     + ui->twCustomers->item(currow,0)->text() + "'";
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    selectAll();
}
void customerstab::edit()
{
    int currow = ui->twCustomers->currentRow();

    if( currow < 0 )
    {
        QMessageBox::critical(this,"Ошибка!","Ни один столбец не выбран!");
        return;
    }

    if( QMessageBox::question(this,"Редактирование","редактировать запись?",
                              QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
        return;

    QSqlQuery query(db);
    QString sqlstr = "update customers set (company_name,last_name,first_name,city, index_code, phone, e_mail) = (?, ?, ? ,?, ?, ?, ?) where customer_id = '"
                     + ui->twCustomers->item(currow,0)->text() + "'";

    query.prepare(sqlstr);
    query.bindValue(0,ui->le_company_name->text());
    query.bindValue(1,ui->le_last_name->text());
    query.bindValue(2,ui->le_first_name->text());
    //query.bindValue(3, ui->le_address->text());
    query.bindValue(3,ui->le_city->text());
    query.bindValue(4,ui->le_index->text().toInt());
    query.bindValue(5,ui->le_phone->text());
    query.bindValue(6, ui->le_mail->text());


    if( !query.exec() )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    selectAll();

}
customerstab::~customerstab()
{
    delete ui;
}


void customerstab::on_pbReport_clicked(bool checked)
{
    QPrinter printer;

    // Setting page orientation through QPageLayout
    QPageLayout layout = printer.pageLayout();
    layout.setOrientation(QPageLayout::Portrait);  // Set the orientation to Portrait
    printer.setPageLayout(layout);

    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        QTextDocument document;
        QTextCursor cursor(&document);
        QTextTableFormat tableFormat;
        tableFormat.setBorder(1);
        tableFormat.setCellPadding(4);
        tableFormat.setCellSpacing(0);
        tableFormat.setAlignment(Qt::AlignCenter);

        QTextTable *table = cursor.insertTable(ui->twCustomers->rowCount() + 1, ui->twCustomers->columnCount(), tableFormat);
        for (int column = 0; column < ui->twCustomers->columnCount(); ++column) {
            QTextTableCell headerCell = table->cellAt(0, column);
            QTextCursor cellCursor = headerCell.firstCursorPosition();
            cellCursor.insertText(ui->twCustomers->horizontalHeaderItem(column)->text());
        }
        for (int row = 0; row < ui->twCustomers->rowCount(); ++row) {
            for (int column = 0; column < ui->twCustomers->columnCount(); ++column) {
                if (ui->twCustomers->item(row, column)) {
                    QTextTableCell dataCell = table->cellAt(row + 1, column);
                    QTextCursor cellCursor = dataCell.firstCursorPosition();
                    cellCursor.insertText(ui->twCustomers->item(row, column)->text());
                }
            }
        }
        document.print(&printer);
    }
}

