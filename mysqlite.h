#ifndef MYSQLITE_H
#define MYSQLITE_H

#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

class mySqlite
{
public:
    mySqlite();
    bool initSql();
    bool createTable();
    bool connectDB();

    QSqlDatabase myDB;

private:
    QSqlQuery *query;

};

#endif // MYSQLITE_H
