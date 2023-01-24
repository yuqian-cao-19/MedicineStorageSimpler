#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mysqlite.h"
#include "autofillSku.h"
#include "setnoedit.h"
#include "adjustinvinput.h"

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
#include <QCheckBox>
#include <QShortcut>


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
    QTableView *invInputAdjustView;
    QTableView *invOutputAdjustView;
    bool inputAdjShown;
    bool outputAdjShown;

    QLineEdit *nameEditInv;
    QComboBox *doseEditInv;
    QComboBox *presEditInv;
    //QComboBox *specUnitEditInv;
    //QLineEdit *specQuantEditInv;
    QComboBox *packageEditInv;
    QComboBox *manufEditInv;
    //QLineEdit *originEditInv;
    QLineEdit *quantityEditInvLow;
    QLineEdit *quantityEditInvHigh;
    QLineEdit *priceEditInvLow;
    QLineEdit *priceEditInvHigh;
    QCheckBox *nameCheckBox;
    QCheckBox *doseCheckBox;
    QCheckBox *presCheckBox;
    QCheckBox *packageCheckBox;
    QCheckBox *manufCheckBox;
    QCheckBox *quantityCheckBox;
    QCheckBox *priceCheckBox;
    QLabel *invInputLabel;
    QLabel *invOutputLabel;
    QPushButton *invInputAddButton;
    QPushButton *invOutputAddButton;
    QPushButton *invInputDeleteButton;
    QPushButton *invOutputDeleteButton;
    QPushButton *invInputSubmitButton;
    QPushButton *invOutputSubmitButton;
    QPushButton *invInputDeleteAllButton;
    QPushButton *invOutputDeleteAllButton;

    adjustInvInput *invInputDialog;

    /*QLineEdit *nameCNEditReceipt;
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
    QLineEdit *priceEditReceipt;*/
    QLineEdit *nameEditReceipt;
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
    QWidget *createHomePage();
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
    void showInvInput();
    void showInvOutput();
    void addInvInput();
    void addInvOutput();
    void insertInvAdjustData();
    void hideInvAdjustWindow();
    void hideInvInput();
    void hideInvOutput();
    void adjustIOFilter();
    void submitInvInputData();
    void submitInvOutputData();
    void deleteInvInputData();
    void deleteInvOutputData();
    void deleteAllInvInputData();
    void deleteAllInvOutputData();

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
