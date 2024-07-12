#include "orderstab.h"
#include "ui_orderstab.h"
#include <QSqlQuery>
#include <QSqlError>
#include "QMessageBox"
#include "QZXing.h"
#include <QPainter>
#include <QPrintDialog>
#include <QPrinter>
#include <QDateTime>
#include <QRect>

orderstab::orderstab(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::orderstab)
{
    ui->setupUi(this);
    connect(ui->pbNew,SIGNAL(clicked(bool)),this, SLOT(add()));
    connect(ui->pbDeleteShip,SIGNAL(clicked(bool)),this, SLOT(del()));
    connect(ui->pbEditShip, SIGNAL(clicked(bool)), this, SLOT(edit()));
    connect(ui->twOrg, SIGNAL(cellClicked(int, int)), this, SLOT(FilledTextFiledFromTableRow()));
    connect(ui->twOrg, SIGNAL(cellClicked(int, int)), this, SLOT(selectItems()));
    connect(ui->pbSearch, SIGNAL(clicked(bool)), this, SLOT(search()));
    connect(ui->pb_all_row, SIGNAL(clicked(bool)), this, SLOT(callEmptySelectAll()));
    connect(ui->twItems, SIGNAL(cellClicked(int, int)), this, SLOT(FilledTextFiledFromTableRowItems()));
    connect(ui->pbAddItem, SIGNAL(clicked(bool)), this, SLOT(addItems()));
    connect(ui->pbDelItems, SIGNAL(clicked(bool)), this, SLOT(delItem()));
    connect(ui->pbEditItem, SIGNAL(clicked(bool)), this, SLOT(editItem()));

    ui->twOrg->setColumnCount(6);
    ui->twOrg->setAutoScroll(true);
    ui->twOrg->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twOrg->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->twOrg->setHorizontalHeaderItem(0,new QTableWidgetItem("Код чека"));
    ui->twOrg->setHorizontalHeaderItem(1,new QTableWidgetItem("Код покупателя"));
    ui->twOrg->setHorizontalHeaderItem(2,new QTableWidgetItem("Почта покупателя"));
    ui->twOrg->setHorizontalHeaderItem(3,new QTableWidgetItem("Код сотрудника"));
    ui->twOrg->setHorizontalHeaderItem(4,new QTableWidgetItem("Сотрудник"));
    ui->twOrg->setHorizontalHeaderItem(5,new QTableWidgetItem("Дата создания чека"));
   // ui->twOrg->setHorizontalHeaderItem(6,new QTableWidgetItem("Дата доставки"));

    selectAll("", "");
    reloadComboBox();

    ui->twItems->setColumnCount(6);
    ui->twItems->setAutoScroll(true);
    ui->twItems->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twItems->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->twItems->setHorizontalHeaderItem(0,new QTableWidgetItem("Код"));
    ui->twItems->setHorizontalHeaderItem(1,new QTableWidgetItem("Код чека"));
    ui->twItems->setHorizontalHeaderItem(2,new QTableWidgetItem("код продукта"));
    ui->twItems->setHorizontalHeaderItem(3,new QTableWidgetItem("Продукт"));
    ui->twItems->setHorizontalHeaderItem(4,new QTableWidgetItem("Количество"));
    ui->twItems->setHorizontalHeaderItem(5,new QTableWidgetItem("Стоимость"));

    ui->twItems->horizontalHeader()->setStretchLastSection(true);
    ui->twItems->setSortingEnabled(true);

    ui->twItems->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->twOrg->horizontalHeader()->setStretchLastSection(true);
    ui->twOrg->setSortingEnabled(true);

    ui->twOrg->setEditTriggers(QAbstractItemView::NoEditTriggers);


}
void orderstab::reloadComboBox()
{

    QSqlQuery query(db);
    if( !query.exec("select product_id, pr_name from products") )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }
    while(query.next()){
        ui->cbPrItem->addItem(query.value(0).toString()+". "+query.value(1).toString());
    }
    if( !query.exec("select customer_id, e_mail from customers") )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }
    while(query.next()){
        ui->cbCustomer->addItem(query.value(0).toString()+". "+query.value(1).toString());
    }
    if( !query.exec("select worker_id, worker_fio from workers") )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }
    while(query.next()){
        ui->cbWorker->addItem(query.value(0).toString()+". "+query.value(1).toString());
    }


}
void orderstab:: callEmptySelectAll()
{
    selectAll("", "");
}
void orderstab:: FilledTextFiledFromTableRow()
{
    int selectedRow = ui->twOrg->currentRow();
    if (selectedRow >= 0){
        ui->leCustomer->setText(ui->twOrg->item(selectedRow, 1)->text());
        ui->leWorker->setText(ui->twOrg->item(selectedRow, 3)->text());
        ui->leOrderPaidDate->setText(ui->twOrg->item(selectedRow, 5)->text());
       // ui->leShipDate->setText(ui->twOrg->item(selectedRow, 6)->text());

    }
    else{
        ui->leCustomer->clear();
        ui->leWorker->clear();
        ui->leOrderPaidDate->clear();
       // ui->leShipDate->clear();

    }
}
void orderstab:: FilledTextFiledFromTableRowItems()
{
    int selectedRow = ui->twItems->currentRow();
    if (selectedRow >= 0){
        ui->le_prId->setText(ui->twItems->item(selectedRow, 2)->text());
        ui->leQuantity->setText(ui->twItems->item(selectedRow, 4)->text());
        ui->leTotal->setText(ui->twItems->item(selectedRow, 5)->text());
    }
    else{
        ui->le_prId->clear();
        ui->leQuantity->clear();
        ui->leTotal->clear();
    }
}
void orderstab::selectItems()
{
   ui->twItems->clearContents();

    int selectedRow = ui->twOrg->currentRow();

    QSqlQuery query(db);
    QString sqlstr;
    sqlstr = "select * from items where order_id = ?";
    query.prepare(sqlstr);
    query.bindValue(0,ui->twOrg->item(selectedRow, 0)->text());

    QSqlQuery queryGetName(db);
    if( !query.exec() )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }

    if( query.isActive())
        ui->twItems->setRowCount( query.size());
    else
        ui->twItems->setRowCount( 0);
    int i=0;
    while(query.next())
    {
        ui->twItems->setItem(i,0,new
                           QTableWidgetItem(query.value("item_id").toString()));
        ui->twItems->setItem(i,1,new
                           QTableWidgetItem(query.value("order_id").toString()));
        ui->twItems->setItem(i,2,new
                             QTableWidgetItem(query.value("product_id").toString()));
        queryGetName.prepare("select pr_name from products where product_id=?");
        queryGetName.bindValue(0,query.value("product_id").toString());
        if( queryGetName.exec() )
        {
            queryGetName.next();
            ui->twItems->setItem(i,3,new
                               QTableWidgetItem( queryGetName.value(0).toString()));

        }
        ui->twItems->setItem(i,4,new
                           QTableWidgetItem(query.value("quantity").toString()));

        ui->twItems->setItem(i,5,new
                           QTableWidgetItem(query.value("total").toString()));

        i++;
    }

}
void orderstab::selectAll(const QString &d, const QString &from)
{

    QSqlQuery query(db);
    QString sqlstr;
    if(d.isEmpty())
    {
        sqlstr = "select * from orders";
    }
    else if(from == "products")
    {
        sqlstr="select * from orders where order_id in (select order_id from items where product_id='"
                 + d +"')";
    }
    else if(from == "customers")
    {
        sqlstr="select * from orders where customer_id ='"
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
    int i=0;
    while(query.next())
    {
        ui->twOrg->setItem(i,0,new
                           QTableWidgetItem(query.value("order_id").toString()));
        ui->twOrg->setItem(i,1,new
                           QTableWidgetItem(query.value("customer_id").toString()));
        queryGetName.prepare("select e_mail from customers where customer_id=?");
        queryGetName.bindValue(0,query.value("customer_id").toString());
        if( queryGetName.exec() )
        {
            queryGetName.next();
            ui->twOrg->setItem(i,2,new
                               QTableWidgetItem( queryGetName.value(0).toString()));

        }
        ui->twOrg->setItem(i,3,new
                           QTableWidgetItem(query.value("worker_id").toString()));
        queryGetName.prepare("select worker_fio from workers where worker_id=?");
        queryGetName.bindValue(0,query.value("worker_id").toString());
        if( queryGetName.exec() )
        {
            queryGetName.next();
            ui->twOrg->setItem(i,4,new
                               QTableWidgetItem( queryGetName.value(0).toString()));

        }
        ui->twOrg->setItem(i,5,new
                           QTableWidgetItem(query.value("order_paid_date").toString()));




        i++;
    }
    ui->twItems->clearContents();

}
void orderstab::search()
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
void orderstab::addItems()
{
    int selectedRow = ui->twOrg->currentRow();
    QSqlQuery query(db);
    QSqlQuery get(db);
    get.prepare("select (select in_stock from products where product_id='"
                +  ui->le_prId->text() + "') -('"
                + ui->leQuantity->text() + "') as popa");
    if( !get.exec() )
    {
        QMessageBox::critical(this,"Error",get.lastError().text());
        return;
    }
    get.next();
    if(get.value(0).toString().toInt()<0){
        query.prepare("select in_stock from products where product_id='"
                      +  ui->le_prId->text() + "'");
        if( !query.exec() )
        {
            QMessageBox::critical(this,"Error",query.lastError().text());
            return;
        }
        query.next();
        QString str = "Товара осталось %1 единиц";
        str = str.arg(query.value(0).toInt());

        QMessageBox::critical(this,"Ошибка!",str);
        return;
    }


    else
    {
        QString sqlstr = "insert into items(order_id, product_id, quantity, total) values(?,?,?,?)";
        query.prepare(sqlstr);
        query.bindValue(0,ui->twOrg->item(selectedRow, 0)->text());
        query.bindValue(1,ui->le_prId->text());
        query.bindValue(2,ui->leQuantity->text());

        QSqlQuery query1(db);


        if( !query1.exec("select pr_price from products where product_id='"+ui->le_prId->text()+"'") )
        {
            QMessageBox::critical(this,"Error",query.lastError().text());
            return;
        }
        query1.next();
        query.bindValue(3,ui->leQuantity->text().toInt()*query1.value(0).toString().toInt());

        if( !query.exec() )
        {
            //ui->teResult->append( query.lastQuery());
            QMessageBox::critical(this,"Error",query.lastError().text());
            return;
        }
        query.prepare("update products set in_stock = ? where product_id='"
                      +  ui->le_prId->text() + "'");
        query.bindValue(0, get.value(0).toString());
        if( !query.exec() )
        {
            QMessageBox::critical(this,"Error",query.lastError().text());
            return;
        }

    }
    selectItems();


}
void orderstab::add()
{
    QSqlQuery query(db);
    QString sqlstr = "insert into orders(customer_id, worker_id, order_paid_date) values(?,?,?)";
    query.prepare(sqlstr);
    query.bindValue(0,ui->leCustomer->text());
    query.bindValue(1,ui->leWorker->text());
    query.bindValue(2,ui->leOrderPaidDate->text());
  //  query.bindValue(3,ui->leShipDate->text());

    if( !query.exec() )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    callEmptySelectAll();
}
void orderstab::delItem()
{
    int currow = ui->twItems->currentRow();
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
                +  ui->le_prId->text() + "') +('"
                + ui->leQuantity->text() + "') as popa");
    if( !get.exec() )
    {
        QMessageBox::critical(this,"Error",get.lastError().text());
        return;
    }
    else
    {
        get.next();


        QString sqlstr = "delete from items where item_id = '"
                         + ui->twItems->item(currow,0)->text() + "'";
        if( !query.exec(sqlstr) )
        {
            QMessageBox::critical(this,"Error",query.lastError().text());
            return;
        }
        query.prepare("update products set in_stock = ? where product_id='"
                      +  ui->le_prId->text() + "'");
        query.bindValue(0, (get.value(0).toString().toInt()));
        if( !query.exec() )
        {
            QMessageBox::critical(this,"Error",query.lastError().text());
            return;
        }
    }
    selectItems();
}
void orderstab::del()
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


    QString sqlstr = "delete from orders where order_id = '"
                     + ui->twOrg->item(currow,0)->text() + "'";
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    callEmptySelectAll();
}
void orderstab::editItem()
{
    QSqlQuery query(db);
    QSqlQuery get(db);
    int currow = ui->twItems->currentRow();

    if( currow < 0 )
    {
        QMessageBox::critical(this,"Error","Not selected row!");
        return;
    }
    get.prepare("select (select in_stock from products where product_id='"
                +  ui->le_prId->text() + "') -('"
                + ui->leQuantity->text() + "')+('"
                + ui->twItems->item(currow,4)->text() + "') as popa");
    if( !get.exec() )
    {
        QMessageBox::critical(this,"Error",get.lastError().text());
        return;
    }
    get.next();
    if(get.value(0).toString().toInt()<0){
        query.prepare("select in_stock from products where product_id='"
                      +  ui->le_prId->text() + "'");
        if( !query.exec() )
        {
            QMessageBox::critical(this,"Error",query.lastError().text());
            return;
        }
        query.next();

        QString str = "Товара осталось %1 единиц";
        str = str.arg(query.value(0).toInt());

        QMessageBox::critical(this,"Ошибка!",str);
        return;
    }


    else
    {


        if( QMessageBox::question(this,"Edit","Edit row?",
                                  QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
            return;


        QSqlQuery query1(db);
        QString sqlstr = "update items set (product_id, quantity, total) = (?,?,?) where item_id = '"
                         + ui->twItems->item(currow,0)->text() + "'";

        query.prepare(sqlstr);

        query.bindValue(0,ui->le_prId->text().toInt());
        query.bindValue(1,ui->leQuantity->text().toInt());
        if( !query1.exec("select pr_price from products where product_id='"+ui->le_prId->text()+"'") )
        {
            QMessageBox::critical(this,"Error",query.lastError().text());
            return;
        }
        query1.next();
        query.bindValue(2,ui->leQuantity->text().toInt()*query1.value(0).toString().toInt());

        if( !query.exec() )
        {
            QMessageBox::critical(this,"Error",query.lastError().text());
            return;
        }

        query.prepare("update products set in_stock = ? where product_id='"
                      +  ui->le_prId->text() + "'");
        query.bindValue(0, (get.value(0).toString().toInt()));
        if( !query.exec() )
        {
            QMessageBox::critical(this,"Error",query.lastError().text());
            return;
        }
    }


    selectItems();

}
void orderstab::edit()
{
    int currow = ui->twOrg->currentRow();

    if( currow < 0 )
    {
        QMessageBox::critical(this,"Error","Not selected row!");
        return;
    }

    if( QMessageBox::question(this,"Edit","Edit row?",
                              QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
        return;

    QSqlQuery query(db);
    QString sqlstr = "update orders set (customer_id, worker_id, order_paid_date) = (?,?,?) where order_id = '"
                     + ui->twOrg->item(currow,0)->text() + "'";

    query.prepare(sqlstr);

    query.bindValue(0,ui->leCustomer->text().toInt());
    query.bindValue(1,ui->leWorker->text().toInt());
    query.bindValue(2,ui->leOrderPaidDate->text());
    //query.bindValue(3,ui->leShipDate->text());


    if( !query.exec() )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }

    callEmptySelectAll();

}

orderstab::~orderstab()
{
    if( db.isOpen())
        db.close();
    delete ui;
}

QString modifStr1(const QString &arg1){
    int dotPos = arg1.indexOf('.');
    QString substr;

    if (dotPos != -1) {
        substr = arg1.left(dotPos);
    } else {
        substr = arg1;
    }
    return substr;
}
void orderstab::on_cbPrItem_currentTextChanged(const QString &arg1)
{
    ui->le_prId->setText( modifStr1(arg1));

}


void orderstab::on_cbCustomer_currentTextChanged(const QString &arg1)
{
    ui->leCustomer->setText( modifStr1(arg1));
}


void orderstab::on_cbWorker_currentTextChanged(const QString &arg1)
{
    ui->leWorker->setText( modifStr1(arg1));
}


void orderstab::on_leCustomer_textChanged(const QString &arg1)
{
    if(!arg1.isEmpty())
    {
        QSqlQuery query(db);
        if( !query.exec("select e_mail from customers where customer_id = '"
                        +  arg1 + "'") )
        {
            QMessageBox::critical(this,"Error", query.lastError().text());
            return;
        }
        query.next();
        ui->cbCustomer->setCurrentText(arg1+". "+query.value(0).toString());
    }
}


void orderstab::on_leWorker_textChanged(const QString &arg1)
{
    if(!arg1.isEmpty())
    {
        QSqlQuery query(db);
        if( !query.exec("select worker_fio from workers where worker_id = '"
                        +  arg1 + "'") )
        {
            QMessageBox::critical(this,"Error", query.lastError().text());
            return;
        }
        query.next();
        ui->cbWorker->setCurrentText(arg1+". "+query.value(0).toString());
    }
}


void orderstab::on_le_prId_textChanged(const QString &arg1)
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
        ui->cbPrItem->setCurrentText(arg1+". "+query.value(0).toString());
    }

}


void orderstab::on_pbReport_clicked(bool checked)
{
    int currow = ui->twOrg->currentRow();
    if( currow < 0 )
    {
        QMessageBox::critical(this,"Error","Not selected row!");
        return;
    }
    // Получение order_id (предположим, что он уже известен)
    int order_id = ui->twOrg->item(currow,0)->text().toInt(); // Функция для получения order_id

    // Запрос к базе данных для получения списка продуктов по order_id
    QSqlQuery query(db);
    query.prepare("SELECT  pr_name, total, quantity FROM items JOIN products ON items.product_id = products.product_id  WHERE order_id = :order_id");
    query.bindValue(":order_id", order_id);

    if (!query.exec()) {
        qDebug() << "Ошибка при выполнении запроса:" << query.lastError().text();
            return;
    }
    struct ProductInfo {
        QString name;
        double total;
        int quantity;
    };


    // Создание списка для хранения продуктов
    QMap<QString, ProductInfo> productMap;

    // Обработка результатов запроса
    while (query.next()) {
        QString productName = query.value(0).toString();
        double total = query.value(1).toDouble();
        int quantity = query.value(2).toInt();

        // Если товар уже есть в словаре, увеличиваем количество и обновляем суммарную стоимость
        if (productMap.contains(productName)) {
            productMap[productName].quantity += quantity;
            productMap[productName].total += total;
        } else { // Иначе, добавляем новую запись о товаре в словарь
            ProductInfo info;
            info.name = productName;
            info.quantity = quantity;
            info.total = total;
            productMap.insert(productName, info);
        }
    }

    // Продолжаем оформление чека...

    // Инициализация принтера и т.д.
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog printDialog(&printer, nullptr);
    if (printDialog.exec() != QDialog::Accepted)
        return;

    QPainter painter;
    painter.begin(&printer);




    // Получаем размеры страницы для вычисления центра
    //QRect rect = printer.pageRect();
    int pageWidth = 4000;

    // Рисуем заголовок "Спасибо за покупку"
    QString thankYou = "Спасибо за покупку!";
    // Вычисляем ширину текста
    QFont font("Arial", 18, QFont::Bold); // Увеличиваем размер шрифта и делаем его жирным
    painter.setFont(font);
    painter.drawText((pageWidth) / 2, 300, thankYou); // Рисуем текст по центру по горизонтали
    QFont font2("Arial", 14, QFont::Bold); // Увеличиваем размер шрифта и делаем его жирным
    painter.setFont(font2);

    thankYou = "Товары:";
    painter.drawText((pageWidth) / 2+100, 500, thankYou); // Рисуем текст по центру по горизонтали

    QFont font1("Arial", 12, QFont::Bold); // Увеличиваем размер шрифта и делаем его жирным
    painter.setFont(font1);

    int totalAll = 0;
    // Рисование списка купленных товаров
    int y = 700;
    for (auto it = productMap.begin(); it != productMap.end(); ++it) {
        QString productName = it.key();
        int quantity = it.value().quantity;
        double total = it.value().total;
        QString productInfo = productName + " x " + QString::number(quantity) + " - " + QString::number(total) + " руб.";

        painter.drawText((pageWidth) / 2+100, y, productInfo); // Рисуем текст по центру по горизонтали
        totalAll+= total;
        y += 200; // Увеличиваем отступ для следующего товара
    }



    QDateTime currentDateTime = QDateTime::currentDateTime();


    QString data = "Спасибо за покупку! Сумма покупки ="+ QString::number(totalAll)+"руб. Код покупки: "+ QString::number(order_id) +". Время покупки: "+currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
    QImage barcode = QZXing::encodeData(data);
    QSize newSize(barcode.width() * 3, barcode.height() * 3); // Увеличиваем изображение в два раза

    barcode = barcode.scaled((newSize), Qt::KeepAspectRatio);

    painter.drawImage((pageWidth) / 2+300, y, barcode);
    thankYou = "Покупка совершена  ";
    painter.drawText((pageWidth) / 2+100, y+1000, thankYou);
     thankYou = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
    painter.drawText((pageWidth) / 2+100, y+1200, thankYou);

    painter.end();
}

