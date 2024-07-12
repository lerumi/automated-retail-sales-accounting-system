#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QtSql>
#include <QDebug>
//#include <QApplication>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextDocument>
#include <QTextTable>
#include <QTextCursor>

class DatabaseManager {
public:

    static QSqlDatabase& getDatabase();
    static QSqlQuery executeQuery(const QString& queryString);
};
#endif // DATABASEMANAGER_H
