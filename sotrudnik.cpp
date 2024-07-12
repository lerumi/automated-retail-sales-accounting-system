//#include "sotrudnik.h"
#include "ui_sotrudnik.h"
#include"authoriz.h"
sotrudnik::sotrudnik(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::sotrudnik)
{
    ui->setupUi(this);
    tabWidget_ship = new ShipmentsTab();
    tabWidget_order= new orderstab();
    tabWidget_pr = new productstab();
    tabWidget_customer= new customerstab();
    tabWidget_provider = new providerstab();
    tabWidget_worker = new workertab();
    tabWidget_storage = new storagestab();
    ui->tabWidget->addTab(tabWidget_ship, tr("Поставки"));

    ui->tabWidget->addTab(tabWidget_pr, tr("Товары"));
    ui->tabWidget->addTab(tabWidget_order, tr("Чеки"));
    ui->tabWidget->addTab(tabWidget_provider, tr("Поставщики"));
    ui->tabWidget->addTab(tabWidget_worker, tr("Работники"));
    ui->tabWidget->addTab(tabWidget_storage, tr("Магазины"));

    connect(tabWidget_pr, SIGNAL(stringChanged(QString, QString)),tabWidget_ship, SLOT(selectAll(QString, QString)));
    connect(tabWidget_pr, SIGNAL(stringChangedOrders(QString, QString)),tabWidget_order, SLOT(selectAll(QString, QString)));
    connect(tabWidget_pr, SIGNAL(setTabIndex(int)), this, SLOT(connection(int)));
    connect(tabWidget_storage, SIGNAL(stringChangedCustomer(QString)), tabWidget_worker, SLOT(selectAll(QString)));

    connect(tabWidget_storage, SIGNAL(stringChanged(QString, QString)),tabWidget_ship, SLOT(selectAll(QString, QString)));
    connect(tabWidget_storage, SIGNAL(setTabIndex(int)), this, SLOT(connection(int)));

    connect(tabWidget_provider, SIGNAL(stringChanged(QString, QString)),tabWidget_ship, SLOT(selectAll(QString, QString)));
    connect(tabWidget_provider, SIGNAL(setTabIndex(int)), this, SLOT(connection(int)));

}
void sotrudnik::connection(const int index)
{
    ui->tabWidget->setCurrentIndex(index);
}
sotrudnik::~sotrudnik()
{
    delete ui;
}

void sotrudnik::on_pushButton_clicked(bool checked)
{
    authoriz *a=new authoriz();
    a->show();
    this->close();


}

