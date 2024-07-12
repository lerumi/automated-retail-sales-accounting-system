#include "providerstab.h"
#include "ui_providerstab.h"
#include <QSqlQuery>
#include <QSqlError>
#include "QMessageBox"

providerstab::providerstab(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::providerstab)
{
    ui->setupUi(this);    connect(ui->pb_new_pr,SIGNAL(clicked(bool)),this, SLOT(add()));
    connect(ui->pbDeletePr,SIGNAL(clicked(bool)),this, SLOT(del()));
    connect(ui->pbEditPr, SIGNAL(clicked(bool)), this, SLOT(edit()));
    connect(ui->twCustomers, SIGNAL(cellClicked(int, int)), this, SLOT(FilledTextFiledFromTableRow()));
    connect(ui->pbSearch, SIGNAL(clicked(bool)), this, SLOT(search()));
    connect(ui->pb_order, SIGNAL(clicked(bool)), this, SLOT(getOrders()));
    connect(ui->pbUpdateColumn, SIGNAL(clicked(bool)), this, SLOT(selectAll()));

    ui->twCustomers->setColumnCount(6);
    ui->twCustomers->setAutoScroll(true);
    ui->twCustomers->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twCustomers->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->twCustomers->setHorizontalHeaderItem(0,new QTableWidgetItem("Код поставщика"));
    ui->twCustomers->setHorizontalHeaderItem(1,new QTableWidgetItem("Название компании"));
    ui->twCustomers->setHorizontalHeaderItem(2,new QTableWidgetItem("Телефон"));
    ui->twCustomers->setHorizontalHeaderItem(3,new QTableWidgetItem("Дата заключения договора"));
    ui->twCustomers->setHorizontalHeaderItem(4,new QTableWidgetItem("Дата истечения договора"));
    ui->twCustomers->setHorizontalHeaderItem(5,new QTableWidgetItem("ФИО представителя"));
    selectAll();
    ui->twCustomers->horizontalHeader()->setStretchLastSection(true);
    ui->twCustomers->setSortingEnabled(true);
    ui->twCustomers->setEditTriggers(QAbstractItemView::NoEditTriggers);




}
void providerstab:: getOrders()
{
    int selectedRow = ui->twCustomers->currentRow();
    if( selectedRow < 0 )
    {
        QMessageBox::critical(this,"Error","Not selected row!");
        return;
    }
    emit stringChanged(ui->twCustomers->item(selectedRow, 0)->text(), "providers");
    emit setTabIndex(0);

}
void providerstab:: FilledTextFiledFromTableRow()
{
    int selectedRow = ui->twCustomers->currentRow();
    if (selectedRow >= 0){
        ui->le_company_name->setText(ui->twCustomers->item(selectedRow, 1)->text());
        ui->le_phone->setText(ui->twCustomers->item(selectedRow, 2)->text());
        ui->le_conclusion_date->setText(ui->twCustomers->item(selectedRow, 3)->text());
        ui->le_expiration_date->setText(ui->twCustomers->item(selectedRow, 4)->text());
        ui->le_agent_fio->setText(ui->twCustomers->item(selectedRow, 5)->text());

    }
    else{
        ui->le_company_name->clear();
        ui->le_phone->clear();
        ui->le_conclusion_date->clear();
        ui->le_expiration_date->clear();
        ui->le_agent_fio->clear();

    }



}

void providerstab::selectAll()
{
    QString sqlstr = "select * from providers";
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
                               QTableWidgetItem(query.value("provider_id").toString()));
        ui->twCustomers->setItem(i,1,new
                               QTableWidgetItem(query.value("firm_name").toString()));
        ui->twCustomers->setItem(i,2,new
                               QTableWidgetItem(query.value("phone_number").toString()));
        ui->twCustomers->setItem(i,3,new
                               QTableWidgetItem(query.value("conclusion_date").toString()));
        ui->twCustomers->setItem(i,4,new
                               QTableWidgetItem(query.value("expiration_date").toString()));
        ui->twCustomers->setItem(i,5,new
                               QTableWidgetItem(query.value("agent_fio").toString()));
        i++;
    }

}
void providerstab::search()
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
void providerstab::add()
{
    QSqlQuery query(db);
    QString sqlstr = "insert into providers(firm_name,phone_number,conclusion_date,expiration_date, agent_fio) values(?, ?, ?, ? ,?)";
    query.prepare(sqlstr);
    query.bindValue(0,ui->le_company_name->text());
    query.bindValue(1,ui->le_phone->text());
    query.bindValue(2,ui->le_conclusion_date->text());
    query.bindValue(3, ui->le_expiration_date->text());
    query.bindValue(4,ui->le_agent_fio->text());
    if( !query.exec() )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    selectAll();
}
void providerstab::del()
{
    int currow = ui->twCustomers->currentRow();
    if( currow < 0 )
    {
        QMessageBox::critical(this,"Error","Not selected row!");
        return;
    }

    if( QMessageBox::question(this,"Delete","Delete row?",
                              QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
        return;
    QSqlQuery query(db);

    QString sqlstr = "delete from providers where provider_id = '"
                     + ui->twCustomers->item(currow,0)->text() + "'";
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    selectAll();
}
void providerstab::edit()
{
    int currow = ui->twCustomers->currentRow();

    if( currow < 0 )
    {
        QMessageBox::critical(this,"Error","Not selected row!");
        return;
    }

    if( QMessageBox::question(this,"Edit","Edit row?",
                              QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
        return;

    QSqlQuery query(db);
    QString sqlstr = "update providers set (firm_name, phone_number, conclusion_date, expiration_date, agent_fio) =(?, ?, ?, ? ,?) where provider_id = '"
                     + ui->twCustomers->item(currow,0)->text() + "'";

    query.prepare(sqlstr);
    query.bindValue(0,ui->le_company_name->text());
    query.bindValue(1,ui->le_phone->text());
    query.bindValue(2,ui->le_conclusion_date->text());
    query.bindValue(3, ui->le_expiration_date->text());
    query.bindValue(4,ui->le_agent_fio->text());


    if( !query.exec() )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    selectAll();

}
providerstab::~providerstab()
{
    delete ui;
}

void providerstab::on_pbReport_clicked(bool checked)
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

