#include <QSqlDatabase>
#include <QPainter>
#include"DataBaseManager.h"
//#include <QApplication>
#include <QPrinter>
#include <QPrintDialog>
void Report()
{
    QSqlDatabase db = DatabaseManager::getDatabase();
    QSqlQuery query(db);
    if (!query.exec("SELECT * FROM your_table")) {
        qDebug() << "Failed to execute query:";
        qDebug() << query.lastError().text();
        return;
    }

    // Создаем диалог выбора принтера
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog printDialog(&printer, nullptr);
    if (printDialog.exec() != QDialog::Accepted)
        return;

    // Создаем объект QPainter для рисования на принтере
    QPainter painter;
    painter.begin(&printer);

    // Рисуем данные отчета
    int y = 100;
    while (query.next()) {
        QString data = query.value(0).toString(); // Пример получения данных из запроса
        painter.drawText(100, y, data);
        y += 20; // Увеличиваем координату Y для следующей строки
    }

    // Заканчиваем рисование
    painter.end();
}
