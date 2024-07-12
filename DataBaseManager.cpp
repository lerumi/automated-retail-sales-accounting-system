#include "DataBaseManager.h"
#include <QSqlQuery>
#include <QSqlError>

QSqlDatabase& DatabaseManager::getDatabase() {
    static QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    if (!db.isOpen()) {
        db.setDatabaseName("ASsales");
        db.setHostName("localhost");
        db.setUserName("postgres");
        db.setPort(5432);
        db.setPassword("235711");

        if (!db.open()) {
            qDebug() << "Failed to open database.";
        }
    }
    qDebug() << "open database.";
    return db;
}

QSqlQuery DatabaseManager::executeQuery(const QString& queryString) {
    QSqlQuery query(getDatabase());
    query.exec(queryString);
    return query;
}
