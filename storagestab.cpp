#include "storagestab.h"
#include "ui_storagestab.h"
#include <QSqlQuery>
#include <QSqlError>
#include "QMessageBox"

storagestab::storagestab(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::storagestab)
{
    ui->setupUi(this);
    connect(ui->pb_new_pr,SIGNAL(clicked(bool)),this, SLOT(add()));
    connect(ui->pbDeletePr,SIGNAL(clicked(bool)),this, SLOT(del()));
    connect(ui->pbEditPr, SIGNAL(clicked(bool)), this, SLOT(edit()));
    connect(ui->twCustomers, SIGNAL(cellClicked(int, int)), this, SLOT(FilledTextFiledFromTableRow()));
    connect(ui->pbSearch, SIGNAL(clicked(bool)), this, SLOT(search()));
    connect(ui->pbUpdateColumn, SIGNAL(clicked(bool)), this, SLOT(selectAll()));

    ui->twCustomers->setColumnCount(2);
    ui->twCustomers->setAutoScroll(true);
    ui->twCustomers->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twCustomers->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->twCustomers->setHorizontalHeaderItem(0,new QTableWidgetItem("Код магазина"));
    ui->twCustomers->setHorizontalHeaderItem(1,new QTableWidgetItem("Адрес магазина"));
    selectAll();
    ui->twCustomers->horizontalHeader()->setStretchLastSection(true);
    ui->twCustomers->setSortingEnabled(true);
    ui->twCustomers->setEditTriggers(QAbstractItemView::NoEditTriggers);

}
void storagestab:: getOrders()
{


}
void storagestab:: FilledTextFiledFromTableRow()
{
    int selectedRow = ui->twCustomers->currentRow();
    if (selectedRow >= 0){
        ui->le_address->setText(ui->twCustomers->item(selectedRow, 1)->text());

    }
    else{
        ui->le_address->clear();

    }

}

void storagestab::selectAll()
{
    QString sqlstr = "select * from storages";
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
                                 QTableWidgetItem(query.value("storage_id").toString()));
        ui->twCustomers->setItem(i,1,new
                                 QTableWidgetItem(query.value("storage_address").toString()));

        i++;
    }

}
void storagestab::search()
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
void storagestab::add()
{
    QSqlQuery query(db);
    QString sqlstr = "insert into storages(storage_address) values(?)";
    query.prepare(sqlstr);
    query.bindValue(0,ui->le_address->text());

    if( !query.exec() )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    selectAll();
}
void storagestab::del()
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

    QString sqlstr = "delete from storages where storage_id = '"
                     + ui->twCustomers->item(currow,0)->text() + "'";
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    selectAll();
}
void storagestab::edit()
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
    QString sqlstr = "update storages set storage_address = ? where storage_id = '"
                     + ui->twCustomers->item(currow,0)->text() + "'";

    query.prepare(sqlstr);
    query.bindValue(0,ui->le_address->text());


    if( !query.exec() )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    selectAll();

}

storagestab::~storagestab()
{
    delete ui;
}

void storagestab::on_pb_ship_clicked(bool checked)
{
    int selectedRow = ui->twCustomers->currentRow();
    if( selectedRow < 0 )
    {
        QMessageBox::critical(this,"Error","Not selected row!");
        return;
    }
    emit stringChanged(ui->twCustomers->item(selectedRow, 0)->text(), "storages");
    emit setTabIndex(0);
}


void storagestab::on_pb_worker_clicked(bool checked)
{
    int selectedRow = ui->twCustomers->currentRow();
    if( selectedRow < 0 )
    {
        QMessageBox::critical(this,"Error","Not selected row!");
        return;
    }
    emit stringChangedCustomer(ui->twCustomers->item(selectedRow, 0)->text());
    emit setTabIndex(4);

}


void storagestab::on_pbReport_clicked(bool checked)
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

