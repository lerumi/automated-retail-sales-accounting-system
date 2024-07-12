//#include "customer_window.h"
#include "ui_customer_window.h"
#include "authoriz.h"
customer_window::customer_window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::customer_window)
{
    ui->setupUi(this);
}

customer_window::~customer_window()
{
    delete ui;
}
