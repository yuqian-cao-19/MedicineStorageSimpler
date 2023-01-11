#include "autofillSku.h"

autofillSku::autofillSku(QSqlTableModel *skuModel,//mySqlite *mysql, int &skuId, QLineEdit *&nameCNEdit, QLineEdit *&nameENEdit, QLineEdit *&aliasCNEdit, QLineEdit *&aliasENEdit, QComboBox *&doseEdit, QComboBox *&presEdit, QComboBox *&specUnitEdit, QLineEdit *&specQuantEdit, QLineEdit *&packageEdit, QLineEdit *&manufCNEdit, QLineEdit *&manufENEdit, QLineEdit *&originEdit,
                         QWidget *parent) : QDialog(parent)
{
    //pop up window
    tmpTableView = new QTableView(this);
    tmpTableView->setModel(skuModel);
    //ioTableView->setColumnHidden(0, true);
    tmpTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    addButton = new QPushButton("Add");
    cancelButton = new QPushButton("Cancel");
    cancelButton->setFocusPolicy(Qt::NoFocus);
    QHBoxLayout *h_layout = new QHBoxLayout;
    h_layout->addStretch();
    h_layout->addWidget(cancelButton);
    h_layout->addWidget(addButton);

    QVBoxLayout *v_layout = new QVBoxLayout;
    v_layout->addWidget(tmpTableView);
    v_layout->addLayout(h_layout);

   /* QGridLayout *layout = new QGridLayout;
    layout->addWidget(tmpTableView, 0,0);
    layout->addWidget(cancelButton,1, 0);
    layout->addWidget(addButton, 1, 1);*/

    //layout->addLayout(v_layout);
    setLayout(v_layout);
    //setLayout(layout);
    setWindowModality(Qt::WindowModal);

    setGeometry(parent->x()+200, parent->y()/2, parent->width() - parent->x()/3, parent->height()/4);
}

autofillSku::~autofillSku(){}
