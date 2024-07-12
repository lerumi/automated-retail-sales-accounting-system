#ifndef CUSTOMER_WINDOW_H
#define CUSTOMER_WINDOW_H

#include <QMainWindow>

namespace Ui {
class customer_window;
}

class customer_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit customer_window(QWidget *parent = nullptr);
    ~customer_window();

private:
    Ui::customer_window *ui;
};

#endif // CUSTOMER_WINDOW_H
