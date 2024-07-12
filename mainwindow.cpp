//#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include"Report.h"
#include"authoriz.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    tabWidget_ship =new ShipmentsTab();
    tabWidget_pr = new productstab();
    tabWidget_order = new orderstab();
    tabWidget_customer= new customerstab();
    tabWidget_provider = new providerstab();

    ui->tabWidgetShipments->addTab(tabWidget_ship, tr("Поставки"));
    ui->tabWidgetShipments->addTab(tabWidget_pr, tr("Товары"));
    ui->tabWidgetShipments->addTab(tabWidget_order, tr("Чеки"));
    ui->tabWidgetShipments->addTab(tabWidget_provider, tr("Поставщики"));
    ui->tabWidgetShipments->addTab(tabWidget_customer, tr("Покупатели"));



    connect(tabWidget_pr, SIGNAL(stringChanged(QString, QString)),tabWidget_ship, SLOT(selectAll(QString, QString)));
    connect(tabWidget_provider, SIGNAL(stringChanged(QString, QString)),tabWidget_ship, SLOT(selectAll(QString, QString)));

    connect(tabWidget_customer, SIGNAL(stringChangedOrders(QString, QString)),tabWidget_order, SLOT(selectAll(QString, QString)));
    connect(tabWidget_pr, SIGNAL(stringChangedOrders(QString, QString)),tabWidget_order, SLOT(selectAll(QString, QString)));

    connect(tabWidget_pr, SIGNAL(setTabIndex(int)), this, SLOT(connection(int)));
    connect(tabWidget_provider, SIGNAL(setTabIndex(int)), this, SLOT(connection(int)));
    connect(tabWidget_customer, SIGNAL(setTabIndex(int)), this, SLOT(connection(int)));



}
void MainWindow::connection(const int index)
{
    ui->tabWidgetShipments->setCurrentIndex(index);
}
MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_pushButton_clicked(bool checked)
{
    authoriz *a=new authoriz();
    a->show();
    this->close();

}

