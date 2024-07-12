#include "shipmentstab.h"
#include "ui_shipmentstab.h"
#include <QSqlQuery>
#include <QSqlError>
#include "QMessageBox"

//#include"mainwindow.h"
#include"DataBaseManager.h"

ShipmentsTab::ShipmentsTab(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ShipmentsTab)
{
    ui->setupUi(this);
    //dbconnect();
    //selectAll();
    connect(ui->pbNew,SIGNAL(clicked(bool)),this, SLOT(add()));
    connect(ui->pbDeleteShip,SIGNAL(clicked(bool)),this, SLOT(del()));
    connect(ui->pbEditShip, SIGNAL(clicked(bool)), this, SLOT(edit()));
    connect(ui->twOrg, SIGNAL(cellClicked(int, int)), this, SLOT(FilledTextFiledFromTableRow()));
    connect(ui->pbSearch, SIGNAL(clicked(bool)), this, SLOT(search()));
    connect(ui->pb_all_row, SIGNAL(clicked(bool)), this, SLOT(callEmptySelectAll()));
    connect(ui->twNeedShip, SIGNAL(cellClicked(int, int)), this, SLOT(fillTextFromTableReOrder()));

    ui->twOrg->setColumnCount(11);
    ui->twOrg->setAutoScroll(true);
    ui->twOrg->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twOrg->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->twOrg->setHorizontalHeaderItem(0,new QTableWidgetItem("Код поставки"));
    ui->twOrg->setHorizontalHeaderItem(1,new QTableWidgetItem("Количество товара"));
    ui->twOrg->setHorizontalHeaderItem(2,new QTableWidgetItem("Дата поставки"));
    ui->twOrg->setHorizontalHeaderItem(3,new QTableWidgetItem("Сумма поставки"));
    ui->twOrg->setHorizontalHeaderItem(4,new QTableWidgetItem("Код продукта"));
    ui->twOrg->setHorizontalHeaderItem(5,new QTableWidgetItem("Продукт"));
    ui->twOrg->setHorizontalHeaderItem(6,new QTableWidgetItem("Код магазина"));
    ui->twOrg->setHorizontalHeaderItem(7,new QTableWidgetItem("Адрес магазина"));
     ui->twOrg->setHorizontalHeaderItem(8,new QTableWidgetItem("Код поставщика"));
    ui->twOrg->setHorizontalHeaderItem(9,new QTableWidgetItem("Поставщик"));
     ui->twOrg->setHorizontalHeaderItem(10,new QTableWidgetItem("Стоимость единицы товара"));

    ui->twNeedShip->setColumnCount(3);
    ui->twNeedShip->setAutoScroll(true);
    ui->twNeedShip->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twNeedShip->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->twNeedShip->setHorizontalHeaderItem(0,new QTableWidgetItem("Код товара"));
    ui->twNeedShip->setHorizontalHeaderItem(1,new QTableWidgetItem("Название товара"));
    ui->twNeedShip->setHorizontalHeaderItem(2,new QTableWidgetItem("Стоимость единицы товара"));
    selectAll("", "");
    reloadComboBox();
    ui->twOrg->horizontalHeader()->setStretchLastSection(true);
    ui->twOrg->setSortingEnabled(true);
    ui->twOrg->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->twNeedShip->horizontalHeader()->setStretchLastSection(true);
    ui->twNeedShip->setSortingEnabled(true);
    ui->twNeedShip->setEditTriggers(QAbstractItemView::NoEditTriggers);

}
void ShipmentsTab::reloadComboBox()
{

    QSqlQuery query(db);
    if( !query.exec("select product_id, pr_name from products") )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }
    while(query.next()){
        ui->cb_pr->addItem(query.value(0).toString()+". "+query.value(1).toString());
    }
    if( !query.exec("select storage_id, storage_address from storages") )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }
    while(query.next()){
        ui->cbStorage->addItem(query.value(0).toString()+". "+query.value(1).toString());
    }
    if( !query.exec("select provider_id, firm_name from providers") )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }
    while(query.next()){
        ui->cbShip->addItem(query.value(0).toString()+". "+query.value(1).toString());
    }
}
void ShipmentsTab:: callEmptySelectAll()
{
    selectAll("", "");
}
void ShipmentsTab:: FilledTextFiledFromTableRow()
{
    int selectedRow = ui->twOrg->currentRow();
    if (selectedRow >= 0){
        ui->leQuantity->setText(ui->twOrg->item(selectedRow, 1)->text());
        ui->deShipment->setText(ui->twOrg->item(selectedRow, 2)->text());
        ui->leTotalShip->setText(ui->twOrg->item(selectedRow, 3)->text());
        ui->leProvider->setText(ui->twOrg->item(selectedRow, 8)->text());
        ui->lestorage->setText(ui->twOrg->item(selectedRow, 6)->text());
        ui->leProduct->setText(ui->twOrg->item(selectedRow, 4)->text());
        ui->leOneCost->setText(ui->twOrg->item(selectedRow, 10)->text());
       // ui->cb_pr->setCurrentText(ui->twOrg->item(selectedRow, 4)->text()+". "+ui->twOrg->item(selectedRow, 5)->text());
    }
    else{
        ui->leQuantity->clear();
        ui->deShipment->clear();
        ui->leTotalShip->clear();
        ui->leProvider->clear();
        ui->lestorage->clear();
        ui->leProduct->clear();
        ui->leOneCost->clear();
    }
}
void ShipmentsTab:: fillTextFromTableReOrder(){
    int selectedRow = ui->twNeedShip->currentRow();
    if (selectedRow >= 0){
        ui->leProduct->setText(ui->twNeedShip->item(selectedRow, 0)->text());
        ui->leOneCost->setText(ui->twNeedShip->item(selectedRow, 2)->text());
    }
    else{

        ui->leProduct->clear();
        ui->leOneCost->clear();
    }
}
void ShipmentsTab::selectAll(const QString &d, const QString &from)
{

    QSqlQuery query(db);
    QString sqlstr;
    if(d.isEmpty())
    {
        sqlstr = "select * from shipments";
    }
    else if(from=="products")
    {
        sqlstr="select * from shipments where product_id = '"
                 + d + "'";
    }
    else if(from=="providers")
    {
        sqlstr="select * from shipments where provider_id = '"
                 + d + "'";
    }
    else if(from=="storages")
    {
        sqlstr="select * from shipments where storage_id = '"
                 + d + "'";
    }

    QSqlQuery queryGetName(db);
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }

    if( query.isActive())
        ui->twOrg->setRowCount( query.size());
    else
        ui->twOrg->setRowCount( 0);
    ui->teResult->append( QString("Read %1 rows").arg(query.size()));
    int i=0;
    while(query.next())
    {
        ui->twOrg->setItem(i,0,new
                           QTableWidgetItem(query.value("shipment_id").toString()));
        ui->twOrg->setItem(i,1,new
                           QTableWidgetItem(query.value("product_quantity").toString()));
        ui->twOrg->setItem(i,2,new
                           QTableWidgetItem(query.value("shipment_date").toString()));
        ui->twOrg->setItem(i,3,new
                           QTableWidgetItem(query.value("shipment_total").toString()));
        ui->twOrg->setItem(i,4,new
                           QTableWidgetItem(query.value("product_id").toString()));
        queryGetName.prepare("select pr_name from products where product_id=?");
        queryGetName.bindValue(0,query.value("product_id").toString());
        if( queryGetName.exec() )
        {
            queryGetName.next();
            ui->twOrg->setItem(i,5,new
                               QTableWidgetItem( queryGetName.value(0).toString()));

        }
        ui->twOrg->setItem(i,6,new
                           QTableWidgetItem(query.value("storage_id").toString()));
        queryGetName.prepare("select storage_address from storages where storage_id=?");
        queryGetName.bindValue(0,query.value("storage_id").toString());
        if( queryGetName.exec() )
        {
            queryGetName.next();
            ui->twOrg->setItem(i,7,new
                               QTableWidgetItem( queryGetName.value(0).toString()));

        }

        ui->twOrg->setItem(i,8,new
                           QTableWidgetItem(query.value("provider_id").toString()));

        queryGetName.prepare("select firm_name from providers where provider_id=?");
        queryGetName.bindValue(0,query.value("provider_id").toString());
        if( queryGetName.exec() )
        {
            queryGetName.next();
            ui->twOrg->setItem(i,9,new
                               QTableWidgetItem( queryGetName.value(0).toString()));

        }
        queryGetName.prepare("select pr_price from products where product_id=?");
        queryGetName.bindValue(0,query.value("product_id").toString());
        if( queryGetName.exec() )
        {
            queryGetName.next();
            ui->twOrg->setItem(i,10,new
                               QTableWidgetItem( queryGetName.value(0).toString()));

        }
        i++;
    }

    sqlstr="select product_id, pr_name, pr_price from products where re_order = true";
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }

    if( query.isActive())
        ui->twNeedShip->setRowCount( query.size());
    else
        ui->twNeedShip->setRowCount( 0);
    ui->teResult->append( QString("Read %1 rows").arg(query.size()));
    i=0;
    while(query.next())
    {
        ui->twNeedShip->setItem(i,0,new
                           QTableWidgetItem(query.value("product_id").toString()));
        ui->twNeedShip->setItem(i,1,new
                           QTableWidgetItem(query.value("pr_name").toString()));
        ui->twNeedShip->setItem(i,2,new
                                QTableWidgetItem(query.value("pr_price").toString()));

        i++;
    }




}
void ShipmentsTab::search()
{
    QString keyword = ui->leSearchShip->text();

    for(int row = 0; row < ui->twOrg->rowCount(); ++row) {
        bool match = false;
        for(int col = 0; col < ui->twOrg->columnCount(); ++col) {
            QTableWidgetItem *item = ui->twOrg->item(row, col);
            if(item && item->text().contains(keyword, Qt::CaseInsensitive)) {
                match = true;
                break;
            }
        }
        ui->twOrg->setRowHidden(row, !match);
    }
}
void ShipmentsTab::add()
{
    if( ui->leQuantity->text().toInt()<0 )
    {
        QMessageBox::critical(this,"Error","Нельзя отрицательные!");
        return;
    }
    int selectedRow = ui->twOrg->currentRow();
    QSqlQuery query(db);
    QSqlQuery get(db);
    QString sqlstr = "insert into shipments(product_quantity,shipment_date, provider_id, storage_id, product_id, shipment_total) values(?,?,?,?,?,?)";
    query.prepare(sqlstr);
    query.bindValue(0,ui->leQuantity->text());
    query.bindValue(1,ui->deShipment->text());
    query.bindValue(2,ui->leProvider->text());
    query.bindValue(3,ui->lestorage->text());
    query.bindValue(4,ui->leProduct->text());
    get.prepare("select pr_price from products where product_id='"
                + ui->leProduct->text() + "'");
    if( !get.exec() )
    {
        ui->teResult->append( get.lastQuery());
        QMessageBox::critical(this,"Error",get.lastError().text());
        return;
    }
    get.next();
    query.bindValue(5, ui->leQuantity->text().toInt() * get.value(0).toString().toInt());
    if( !query.exec() )
    {
        ui->teResult->append( query.lastQuery());
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    get.prepare("select (select in_stock from products where product_id='"
                +  ui->leProduct->text() + "') +('"
                + ui->leQuantity->text() + "') as popa");
    if( !get.exec() )
    {
        ui->teResult->append( get.lastQuery());
        QMessageBox::critical(this,"Error",get.lastError().text());
        return;
    }
    get.next();
    query.prepare("update products set in_stock = ? where product_id='"
                  +  ui->leProduct->text() + "'");
    query.bindValue(0, get.value(0).toString());
    if( !query.exec() )
    {
        ui->teResult->append( query.lastQuery());
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    ui->teResult->append( QString("AddRead %1rows").arg(query.numRowsAffected()) );
    callEmptySelectAll();
}
void ShipmentsTab::del()
{
    int currow = ui->twOrg->currentRow();
    if( currow < 0 )
    {
        QMessageBox::critical(this,"Error","Not selected row!");
        return;
    }

    if( QMessageBox::question(this,"Delete","Delete row?",
                              QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
        return;
    QSqlQuery query(db);

    QSqlQuery get(db);
    get.prepare("select (select in_stock from products where product_id='"
                +  ui->leProduct->text() + "') -('"
                + ui->leQuantity->text() + "') as popa");


    QString sqlstr = "delete from shipments where shipment_id = '"
                     + ui->twOrg->item(currow,0)->text() + "'";
    if( !query.exec(sqlstr) )
    {
        ui->teResult->append( query.lastQuery());
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    if( !get.exec() )
    {
        ui->teResult->append( get.lastQuery());
        QMessageBox::critical(this,"Error",get.lastError().text());
        return;
    }
    get.next();
    query.prepare("update products set in_stock = ? where product_id='"
                  +  ui->leProduct->text() + "'");
    query.bindValue(0, get.value(0).toString());
    ui->teResult->append( QString("Del %1 rows").arg(query.numRowsAffected()) );
    callEmptySelectAll();
}
void ShipmentsTab::edit()
{
    //ui->leProduct->setReadOnly(1);
    QSqlQuery get(db);
    int currow = ui->twOrg->currentRow();

    if( currow < 0 )
    {
        QMessageBox::critical(this,"Error","Not selected row!");
        return;
    }

    if( QMessageBox::question(this,"Edit","Edit row?",
                              QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
        return;
    get.prepare("select (select in_stock from products where product_id='"
                +  ui->leProduct->text() + "') +('"
                + ui->leQuantity->text() + "')-('"
                + ui->twOrg->item(currow,1)->text() + "') as popa");

    QSqlQuery query(db);
    QString sqlstr = "update shipments set (product_quantity,shipment_date,shipment_total, provider_id, storage_id, product_id) = (?,?,?,?,?,?) where shipment_id = '"
                     + ui->twOrg->item(currow,0)->text() + "'";

    query.prepare(sqlstr);

    query.bindValue(0,ui->leQuantity->text().toInt());
    query.bindValue(1,ui->deShipment->text());
    query.bindValue(2,ui->leQuantity->text().toInt() * ui->twOrg->item(currow, 10)->text().toInt());
    query.bindValue(3,ui->leProvider->text().toInt());
    query.bindValue(4,ui->lestorage->text().toInt());
    query.bindValue(5,ui->leProduct->text().toInt());


    if( !query.exec() )
    {
        ui->teResult->append( query.lastQuery());
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    if( !get.exec() )
    {
        ui->teResult->append( get.lastQuery());
        QMessageBox::critical(this,"Error",get.lastError().text());
        return;
    }
    get.next();
    query.prepare("update products set in_stock = ? where product_id='"
                  +  ui->leProduct->text() + "'");
    query.bindValue(0, get.value(0).toString());
    if( !query.exec() )
    {
        ui->teResult->append( query.lastQuery());
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }


    ui->teResult->append( QString("Edit %1 rows").arg(query.numRowsAffected()) );
    callEmptySelectAll();

}

ShipmentsTab::~ShipmentsTab()
{
    if( db.isOpen())
        db.close();
    delete ui;
}
QString modifStr(const QString &arg1){
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
void ShipmentsTab::on_cb_pr_currentTextChanged(const QString &arg1)
{

    ui->leProduct->setText(modifStr(arg1));
}


void ShipmentsTab::on_cbShip_currentTextChanged(const QString &arg1)
{
    ui->leProvider->setText(modifStr(arg1));

}


void ShipmentsTab::on_cbStorage_currentTextChanged(const QString &arg1)
{
    ui->lestorage->setText(modifStr(arg1));
}


void ShipmentsTab::on_leProvider_textChanged(const QString &arg1)
{

    if(!arg1.isEmpty())
    {
        QSqlQuery query(db);
        if( !query.exec("select firm_name from providers where provider_id = '"
                        +  arg1 + "'") )
        {
            QMessageBox::critical(this,"Error", query.lastError().text());
            return;
        }
        query.next();
        ui->cbShip->setCurrentText(arg1+". "+query.value(0).toString());
    }


}


void ShipmentsTab::on_lestorage_textChanged(const QString &arg1)
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
        ui->cbStorage->setCurrentText(arg1+". "+query.value(0).toString());
    }
}


void ShipmentsTab::on_leProduct_textChanged(const QString &arg1)
{
    if(!arg1.isEmpty())
    {
        QSqlQuery query(db);
        if( !query.exec("select pr_name from products where product_id = '"
                        +  arg1 + "'") )
        {
            QMessageBox::critical(this,"Error", query.lastError().text());
            return;
        }
        query.next();
        ui->cb_pr->setCurrentText(arg1+". "+query.value(0).toString());
    }
}


void ShipmentsTab::on_pbReport_clicked(bool checked)
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

        QTextTable *table = cursor.insertTable(ui->twOrg->rowCount() + 1, ui->twOrg->columnCount(), tableFormat);
        for (int column = 0; column < ui->twOrg->columnCount(); ++column) {
            QTextTableCell headerCell = table->cellAt(0, column);
            QTextCursor cellCursor = headerCell.firstCursorPosition();
            cellCursor.insertText(ui->twOrg->horizontalHeaderItem(column)->text());
        }
        for (int row = 0; row < ui->twOrg->rowCount(); ++row) {
            for (int column = 0; column < ui->twOrg->columnCount(); ++column) {
                if (ui->twOrg->item(row, column)) {
                    QTextTableCell dataCell = table->cellAt(row + 1, column);
                    QTextCursor cellCursor = dataCell.firstCursorPosition();
                    cellCursor.insertText(ui->twOrg->item(row, column)->text());
                }
            }
        }
        document.print(&printer);
    }
}

