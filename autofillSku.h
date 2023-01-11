#ifndef AUTOFILLSKU_H
#define AUTOFILLSKU_H

#include <QTableView>
#include <QGridLayout>
#include <QString>
#include <QSqlQuery>
#include <QDialog>
#include <QSqlTableModel>
#include <mysqlite.h>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>



class autofillSku : public QDialog
{
    Q_OBJECT
public:
    autofillSku(QSqlTableModel *x,// mySqlite *m, int &skuId, QLineEdit *&nameCNEdit, QLineEdit *&nameENEdit, QLineEdit *&aliasCNEdit, QLineEdit *&aliasENEdit, QComboBox *&doseEdit, QComboBox *&presEdit, QComboBox *&specUnitEdit, QLineEdit *&specQuantEdit, QLineEdit *&packageEdit, QLineEdit *&manufCNEdit, QLineEdit *&manufENEdit, QLineEdit *&originEdit,
                QWidget *parent = nullptr);
    ~autofillSku();

    QPushButton *addButton;
    QPushButton *cancelButton;
    QTableView* tmpTableView ;
};

#endif // AUTOFILLSKU_H
