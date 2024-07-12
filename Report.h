#ifndef REPORT_H
#define REPORT_H

#include <QSqlDatabase>
#include <QPainter>
#include"DataBaseManager.h"
//#include <QApplication>
#include <QPrinter>
#include <QPrintDialog>

void Report1(QString &a, int countRow)
{
    QSqlDatabase db = DatabaseManager::getDatabase();
    QSqlQuery query(db);
    if (!query.exec(a)) {
        qDebug() << "Failed to execute query:";
        qDebug() << query.lastError().text();
        return;
    }
    QSqlRecord rec = query.record();

    int columnCount = rec.count();
    QStringList columnNames;
    for (int i = 0; i < columnCount; ++i) {
        QString columnName = rec.fieldName(i);
        columnNames.append(columnName);
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
    painter.drawText(columnNames[0].size(), y, columnNames[0]);
    for(int i=1; i<countRow; i++){
        painter.drawText(i*500+columnNames[i-1].size(), y, columnNames[i]);

    }
    y+=150;
    while (query.next()) {
        QString data;
        int predData;
        data = query.value(0).toString();

        painter.drawText(data.size(), y, data);
        for(int i=1; i<countRow; i++){

            data = query.value(i).toString();
            predData=query.value(i-1).toString().size();
            painter.drawText(i*500+predData, y, data);
        }

        y += 150;
    }

    // Заканчиваем рисование
    painter.end();
}
#endif // REPORT_H
