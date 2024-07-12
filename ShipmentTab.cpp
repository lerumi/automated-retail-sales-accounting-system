#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_ShipmentTab.h"

Fo:ShipmentTab(QWidget *parent)
    : ShipmentTab(parent)
    , ui(new Ui::ShipmentTab)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    // page_one = new ShipmentTab();
    //Ui_ShipmentTab ship;
    ui->tabWidgetShipments->widget(index)->layout()->addWidget(ui(new Ui::ShipmentTab));

    //ui->tabWidgetShipments->tab;
}

