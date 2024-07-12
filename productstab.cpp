#include "productstab.h"
#include "ui_productstab.h"
#include <QSqlQuery>
#include <QSqlError>
#include "QMessageBox"
#include"DataBaseManager.h"

productstab::productstab(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::productstab)
{
    ui->setupUi(this);
    //dbconnect();
    //selectAll();
    connect(ui->pb_new_pr,SIGNAL(clicked(bool)),this, SLOT(add()));
    connect(ui->pbDeletePr,SIGNAL(clicked(bool)),this, SLOT(del()));
    connect(ui->pbEditPr, SIGNAL(clicked(bool)), this, SLOT(edit()));
    connect(ui->twProduct, SIGNAL(cellClicked(int, int)), this, SLOT(FilledTextFiledFromTableRow()));
    connect(ui->pbSearch, SIGNAL(clicked(bool)), this, SLOT(search()));
    connect(ui->pb_ship, SIGNAL(clicked(bool)), this, SLOT(getShip()));
    connect(ui->pbUpdateColumn, SIGNAL(clicked(bool)), this, SLOT(selectAll()));

    ui->twProduct->setColumnCount(6);
    ui->twProduct->setAutoScroll(true);
    ui->twProduct->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twProduct->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->twProduct->setHorizontalHeaderItem(0,new QTableWidgetItem("Код товара"));
    ui->twProduct->setHorizontalHeaderItem(1,new QTableWidgetItem("Название товара"));
    ui->twProduct->setHorizontalHeaderItem(2,new QTableWidgetItem("Цена"));
    ui->twProduct->setHorizontalHeaderItem(3,new QTableWidgetItem("Остаток на складе"));
    ui->twProduct->setHorizontalHeaderItem(4,new QTableWidgetItem("Перезаказ"));
    ui->twProduct->setHorizontalHeaderItem(5,new QTableWidgetItem("Описание"));
    selectAll();
    ui->twProduct->horizontalHeader()->setStretchLastSection(true);
    ui->twProduct->setSortingEnabled(true);
    ui->twProduct->setEditTriggers(QAbstractItemView::NoEditTriggers);

}
void productstab:: getShip()
{
    int selectedRow = ui->twProduct->currentRow();
    if( selectedRow < 0 )
    {
        QMessageBox::critical(this,"Error","Not selected row!");
        return;
    }
    emit stringChanged(ui->twProduct->item(selectedRow, 0)->text(), "products");
    emit setTabIndex(0);
}
void productstab:: FilledTextFiledFromTableRow()
{
    int selectedRow = ui->twProduct->currentRow();
    if (selectedRow >= 0){
        ui->le_pr_name->setText(ui->twProduct->item(selectedRow, 1)->text());
        ui->le_price->setText(ui->twProduct->item(selectedRow, 2)->text());
        ui->le_in_stock->setText(ui->twProduct->item(selectedRow, 3)->text());

        QString checkedValue = ui->twProduct->item(selectedRow, 4)->text();
        if (checkedValue == "true") {
            ui->cb_re_order->setCheckState(Qt::Checked);
        } else {
            ui->cb_re_order->setCheckState(Qt::Unchecked);
        }
        ui->le_description->setText(ui->twProduct->item(selectedRow, 5)->text());
    }
    else{
        ui->le_pr_name->clear();
        ui->le_price->clear();
        ui->le_in_stock->clear();
         ui->cb_re_order->setCheckState(Qt::Unchecked);
        ui->le_description->clear();
    }



}
void productstab::checkNullInStock(){
    QSqlQuery query(db);
    QSqlQuery query1(db);
    if( !query.exec("select product_id, in_stock from products") )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    while(query.next())
    {
        if(query.value(1).toString()=="0")
        {
            if( !query1.exec("update products set re_order = true where product_id='"+query.value(0).toString()+"'") )
            {
                QMessageBox::critical(this,"Error",query.lastError().text());
                return;
            }

        }
        else{
            if( !query1.exec("update products set re_order = false where product_id='"+query.value(0).toString()+"'") )
            {
                QMessageBox::critical(this,"Error",query.lastError().text());
                return;
            }

        }

    }

}
void productstab::selectAll()
{
    checkNullInStock();
    //ui->twProduct->clearContents();
    QString sqlstr = "select * from products";
    QSqlQuery query(db);
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    if( query.isActive())
        ui->twProduct->setRowCount( query.size());
    else
        ui->twProduct->setRowCount(0);

    int i=0;
    while(query.next())
    {
        ui->twProduct->setItem(i,0,new
                           QTableWidgetItem(query.value("product_id").toString()));
        ui->twProduct->setItem(i,1,new
                           QTableWidgetItem(query.value("pr_name").toString()));
        ui->twProduct->setItem(i,2,new
                           QTableWidgetItem(query.value("pr_price").toString()));
        ui->twProduct->setItem(i,3,new
                           QTableWidgetItem(query.value("in_stock").toString()));
        ui->twProduct->setItem(i,4,new
                           QTableWidgetItem(query.value("re_order").toString()));
       ui->twProduct->setItem(i,5,new
                               QTableWidgetItem(query.value("description").toString()));

        i++;
    }

}
void productstab::search()
{
    QString keyword = ui->leSearchPr->text();

    for(int row = 0; row < ui->twProduct->rowCount(); ++row) {
        bool match = false;
        for(int col = 0; col < ui->twProduct->columnCount(); ++col) {
            QTableWidgetItem *item = ui->twProduct->item(row, col);
            if(item && item->text().contains(keyword, Qt::CaseInsensitive)) {
                match = true;
                break;
            }
        }
        ui->twProduct->setRowHidden(row, !match);
    }
}
void productstab::add()
{
    QSqlQuery query(db);
    QString sqlstr = "insert into products(pr_name,pr_price,in_stock, re_order, description) values(?,?,?,?,?)";
    query.prepare(sqlstr);
    query.bindValue(0,ui->le_pr_name->text());
    query.bindValue(1,ui->le_price->text());
    query.bindValue(2,ui->le_in_stock->text());
    if(ui->cb_re_order->checkState() ==0)
    {
        query.bindValue(3, false);
    }
    else{
        query.bindValue(3, true);
    }
    query.bindValue(4,ui->le_description->text());
    if( !query.exec() )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    selectAll();
}
void productstab::del()
{
    int currow = ui->twProduct->currentRow();
    if( currow < 0 )
    {
        QMessageBox::critical(this,"Error","Not selected row!");
        return;
    }

    if( QMessageBox::question(this,"Delete","Delete row?",
                              QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
        return;
    QSqlQuery query(db);

    QString sqlstr = "delete from products where product_id = '"
                     + ui->twProduct->item(currow,0)->text() + "'";
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    selectAll();
}
void productstab::edit()
{
    int currow = ui->twProduct->currentRow();

    if( currow < 0 )
    {
        QMessageBox::critical(this,"Error","Not selected row!");
        return;
    }

    if( QMessageBox::question(this,"Edit","Edit row?",
                              QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
        return;

    QSqlQuery query(db);
    QString sqlstr = "update products set (pr_name,pr_price,in_stock, re_order, description) = (?,?,?,?,?) where product_id = '"
                     + ui->twProduct->item(currow,0)->text() + "'";

    query.prepare(sqlstr);

    query.bindValue(0,ui->le_pr_name->text());
    query.bindValue(1,ui->le_price->text().toInt());
    query.bindValue(2,ui->le_in_stock->text().toInt());
    if(ui->cb_re_order->checkState() ==0)
    {
         query.bindValue(3, false);
    }
    else{
        query.bindValue(3, true);
    }

    query.bindValue(4,ui->le_description->text());


    if( !query.exec() )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    selectAll();

}
productstab::~productstab()
{
    if( db.isOpen())
        db.close();
    delete ui;
}

void productstab::on_pb_order_clicked(bool checked)
{
    int selectedRow = ui->twProduct->currentRow();
    if( selectedRow < 0 )
    {
        QMessageBox::critical(this,"Error","Not selected row!");
        return;
    }
    emit stringChangedOrders(ui->twProduct->item(selectedRow, 0)->text(), "products");
    emit setTabIndex(2);
}


void productstab::on_pbReport_clicked(bool checked)
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

        QTextTable *table = cursor.insertTable(ui->twProduct->rowCount() + 1, ui->twProduct->columnCount(), tableFormat);
        for (int column = 0; column < ui->twProduct->columnCount(); ++column) {
            QTextTableCell headerCell = table->cellAt(0, column);
            QTextCursor cellCursor = headerCell.firstCursorPosition();
            cellCursor.insertText(ui->twProduct->horizontalHeaderItem(column)->text());
        }
        for (int row = 0; row < ui->twProduct->rowCount(); ++row) {
            for (int column = 0; column < ui->twProduct->columnCount(); ++column) {
                if (ui->twProduct->item(row, column)) {
                    QTextTableCell dataCell = table->cellAt(row + 1, column);
                    QTextCursor cellCursor = dataCell.firstCursorPosition();
                    cellCursor.insertText(ui->twProduct->item(row, column)->text());
                }
            }
        }
        document.print(&printer);
    }
}

