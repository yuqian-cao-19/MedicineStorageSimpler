#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mysqlite.h"
#include "autofillSku.h"
#include "setnoedit.h"

#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QStackedWidget>
#include <QLabel>
#include <QFile>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QTableView>
#include <QSqlRecord>
#include <QMessageBox>
#include <QDateTime>
#include <iostream>
#include <QSqlError>
#include <QActionGroup>
#include <QFont>
#include <QStandardItemModel>
#include <QListView>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    const QString HOME = "Home";
    const QString INV = "Inventory";
    const QString INPUT = "Inventory Restock";
    const QString OUTPUT = "Inventory Consumption";
    const QString MONINPUT = "Monitor Input";
    const QString MONOUTPUT = "Monitor Output";
    const QString RECEIPT = "Receipt";
    QString currPage;

    mySqlite *mysql;
    QStackedWidget *stackWidget;
    QSqlTableModel* ioModel;
    QSqlTableModel* skuModel;
    QSqlTableModel* inventoryModel;
    QSqlTableModel* receiptModel;
    QAction *homeAction;
    QAction *invAction;
    QAction *inputAction;
    QAction *outputAction;
    QAction *monitorInAction;
    QAction *monitorOutAction;
    QAction *receipt;

    QLineEdit *nameCNEdit;
    QLineEdit *nameENEdit;
    QLineEdit *aliasCNEdit;
    QLineEdit *aliasENEdit;
    QComboBox *doseEdit;
    QComboBox *presEdit;
    QComboBox *specUnitEdit;
    QLineEdit *specQuantEdit;
    QComboBox *packageEdit;
    QLineEdit *manufCNEdit;
    QLineEdit *manufENEdit;
    QLineEdit *originEdit;
    QLineEdit *quantityEdit;
    QLineEdit *priceEdit;
    QTableView *ioTableView;
    QTableView *invTableView;
    QTableView *receiptTableView;

    QLineEdit *nameCNEditInv;
    QLineEdit *nameENEditInv;
    QLineEdit *aliasCNEditInv;
    QLineEdit *aliasENEditInv;
    QComboBox *doseEditInv;
    QComboBox *presEditInv;
    QComboBox *specUnitEditInv;
    QLineEdit *specQuantEditInv;
    QComboBox *packageEditInv;
    QLineEdit *manufCNEditInv;
    QLineEdit *manufENEditInv;
    QLineEdit *originEditInv;
    QLineEdit *quantityEditInv;
    QLineEdit *priceEditInv;

    QLineEdit *nameCNEditReceipt;
    QLineEdit *nameENEditReceipt;
    QLineEdit *aliasCNEditReceipt;
    QLineEdit *aliasENEditReceipt;
    QComboBox *doseEditReceipt;
    QComboBox *presEditReceipt;
    QComboBox *specUnitEditReceipt;
    QLineEdit *specQuantEditReceipt;
    QComboBox *packageEditReceipt;
    QLineEdit *manufCNEditReceipt;
    QLineEdit *manufENEditReceipt;
    QLineEdit *originEditReceipt;
    QLineEdit *quantityEditReceipt;
    QLineEdit *priceEditReceipt;
    QComboBox *dateSelection;
    QComboBox *typeSelection;

    autofillSku *autoSkuData;
    QWidget *ioPage;
    int currRow;
    bool exist;
    QLabel *titleText;

    int totalReceiptPage;
    int currReceiptPage;
    QString currReceiptDate;
    QLabel *currReceiptPageDisplay;

    void createNaviBar();
    void createStackWidget();
    void switchPages();
    void checkDB();
    QWidget *createIOPage();

    void insertStockData();
    void checkIOFields();
    void searchIOData();
    void clearIOFields();
    void deleteInputData();
    void deleteAllInputData();
    void filterSetting();
    void submitData();
    void skuExist();
    void createTmpSkuView();
    void fillLines();
    void tmpFunc();
    void initializeModel();
    QWidget* createInventoryPage();
    void createTmpInventoryTable();
    void clearInvFields();
    void searchInvData();
    QWidget* createReceiptPage();//Receipt page
    void createTmpReceiptTable();
    void searchReceiptData();
    void nextReceiptPage();
    void prevReceiptPage();
    void calcTotalReceiptPage();
    void clearReceiptData();
    void resetReceiptView();
};
#endif // MAINWINDOW_H
