#include "workertab.h"
#include "ui_workertab.h"
#include <QSqlQuery>
#include <QSqlError>
#include "QMessageBox"

workertab::workertab(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::workertab)
{
    ui->setupUi(this);
    connect(ui->pb_new_pr,SIGNAL(clicked(bool)),this, SLOT(add()));
    connect(ui->pbDeletePr,SIGNAL(clicked(bool)),this, SLOT(del()));
    connect(ui->pbEditPr, SIGNAL(clicked(bool)), this, SLOT(edit()));
    connect(ui->twCustomers, SIGNAL(cellClicked(int, int)), this, SLOT(FilledTextFiledFromTableRow()));
    connect(ui->pbSearch, SIGNAL(clicked(bool)), this, SLOT(search()));
    connect(ui->pbUpdateColumn, SIGNAL(clicked(bool)), this, SLOT(callEmptySelectAll()));

    ui->twCustomers->setColumnCount(5);
    ui->twCustomers->setAutoScroll(true);
    ui->twCustomers->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twCustomers->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->twCustomers->setHorizontalHeaderItem(0,new QTableWidgetItem("Код работника"));
    ui->twCustomers->setHorizontalHeaderItem(1,new QTableWidgetItem("Код магазина"));
    ui->twCustomers->setHorizontalHeaderItem(2,new QTableWidgetItem("Должность"));
    ui->twCustomers->setHorizontalHeaderItem(3,new QTableWidgetItem("ФИО работника"));
    ui->twCustomers->setHorizontalHeaderItem(4,new QTableWidgetItem("Зарплата"));
    callEmptySelectAll();
    reloadComboBox();
    ui->twCustomers->horizontalHeader()->setStretchLastSection(true);
    ui->twCustomers->setSortingEnabled(true);
    ui->twCustomers->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
void workertab:: callEmptySelectAll()
{
    selectAll("");
}
void workertab::reloadComboBox()
{

    QSqlQuery query(db);

    if( !query.exec("select storage_id, storage_address from storages") )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }
    while(query.next()){
        ui->cb_storage->addItem(query.value(0).toString()+". "+query.value(1).toString());
    }

}
void workertab:: getOrders()
{
    int selectedRow = ui->twCustomers->currentRow();
    if( selectedRow < 0 )
    {
        QMessageBox::critical(this,"Error","Not selected row!");
        return;
    }
}
void workertab:: FilledTextFiledFromTableRow()
{
    int selectedRow = ui->twCustomers->currentRow();
    if (selectedRow >= 0){
        ui->le_storage_id->setText(ui->twCustomers->item(selectedRow, 1)->text());
        ui->le_post->setText(ui->twCustomers->item(selectedRow, 2)->text());
        ui->le_worker_fio->setText(ui->twCustomers->item(selectedRow, 3)->text());
        ui->le_salary->setText(ui->twCustomers->item(selectedRow, 4)->text());
    }

    else{
        ui->le_storage_id->clear();
        ui->le_post->clear();
        ui->le_worker_fio->clear();
        ui->le_salary->clear();
    }

}

void workertab::selectAll(const QString &d)
{
    QString sqlstr ;
    if(d.isEmpty())
    {
        sqlstr = "select * from workers";
    }
    else
    {
        sqlstr="select * from workers where storage_id = '"
                 + d + "'";
    }
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
                                 QTableWidgetItem(query.value("worker_id").toString()));
        ui->twCustomers->setItem(i,1,new
                                 QTableWidgetItem(query.value("storage_id").toString()));
        ui->twCustomers->setItem(i,2,new
                                 QTableWidgetItem(query.value("post").toString()));
        ui->twCustomers->setItem(i,3,new
                                 QTableWidgetItem(query.value("worker_fio").toString()));
        ui->twCustomers->setItem(i,4,new
                                 QTableWidgetItem(query.value("salary").toString()));

        i++;
    }

}
void workertab::search()
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
void workertab::add()
{
    QSqlQuery query(db);
    QString sqlstr = "insert into workers(storage_id, post,worker_fio,salary) values(?, ?, ?, ?)";
    query.prepare(sqlstr);
    query.bindValue(0,ui->le_storage_id->text());
    query.bindValue(1,ui->le_post->text());
    query.bindValue(2,ui->le_worker_fio->text());
    query.bindValue(3, ui->le_salary->text());
    if( !query.exec() )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    callEmptySelectAll();
}
void workertab::del()
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

    QString sqlstr = "delete from workers where worker_id = '"
                     + ui->twCustomers->item(currow,0)->text() + "'";
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    callEmptySelectAll();
}
void workertab::edit()
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
    QString sqlstr = "update workers set (storage_id, post,worker_fio,salary) = (?, ?, ?, ?) where worker_id = '"
                     + ui->twCustomers->item(currow,0)->text() + "'";

    query.prepare(sqlstr);
    query.bindValue(0,ui->le_storage_id->text());
    query.bindValue(1,ui->le_post->text());
    query.bindValue(2,ui->le_worker_fio->text());
    query.bindValue(3, ui->le_salary->text());


    if( !query.exec() )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    callEmptySelectAll();

}

workertab::~workertab()
{
    delete ui;
}
QString modifStr2(const QString &arg1){
    int dotPos = arg1.indexOf('.');
    QString substr;

    if (dotPos != -1) {
        substr = arg1.left(dotPos);
    } else {
        // Если точка не найдена, то берем всю строку
        substr = arg1;
    }
    return substr;
}

void workertab::on_cb_storage_currentTextChanged(const QString &arg1)
{
    ui->le_storage_id->setText(modifStr2(arg1));

}


void workertab::on_le_storage_id_textChanged(const QString &arg1)
{
    if(!arg1.isEmpty())
    {
        QSqlQuery query(db);
        if( !query.exec("select storage_address from storages where storage_id = '"
                        +  arg1 + "'") )
        {
            QMessageBox::critical(this,"Error", query.lastError().text());
            return;
        }
        query.next();
        ui->cb_storage->setCurrentText(arg1+". "+query.value(0).toString());
    }

}


void workertab::on_pbReport_clicked(bool checked)
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

