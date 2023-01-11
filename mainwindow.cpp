#include "mainwindow.h"


/*when ledger submitted:
 * clear tmpLedger table
 * add sku id and receipt id, create receipt
 * add date
*/

/*autofill
 * pop up window to select row
 * connect inputting chinese / english name / alias to the method
*/

/*Filter data based off of type
 *
*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    mysql = new mySqlite;
    if(mysql->initSql())
        qDebug() << "Connection successful";
    else
        qDebug() << "Connection unsuccessful";
    currPage = HOME;

    initializeModel();
    calcTotalReceiptPage();
    currReceiptPage = 1;
    createNaviBar();
    createStackWidget();
    connect(homeAction, &QAction::triggered, this, &MainWindow::switchPages);
    connect(invAction, &QAction::triggered, this, &MainWindow::switchPages);
    connect(inputAction, &QAction::triggered, this, &MainWindow::switchPages);
    connect(outputAction, &QAction::triggered, this, &MainWindow::switchPages);
    connect(monitorInAction, &QAction::triggered, this, &MainWindow::switchPages);
    connect(monitorOutAction, &QAction::triggered, this, &MainWindow::switchPages);
    connect(receipt, &QAction::triggered, this, &MainWindow::switchPages);

    currRow = -1;
    exist = false;


}

MainWindow::~MainWindow()
{
}

void MainWindow::initializeModel(){
    skuModel = new QSqlTableModel(this, mysql->myDB);
    skuModel->setTable("SKU");
    skuModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    skuModel->setHeaderData(0, Qt::Horizontal, "SKU ID");
    skuModel->setHeaderData(1, Qt::Horizontal, "Name (CN)");
    skuModel->setHeaderData(2, Qt::Horizontal, "Name (EN)");
    skuModel->setHeaderData(3, Qt::Horizontal, "Alias (CN)");
    skuModel->setHeaderData(4, Qt::Horizontal, "Alias (EN)");
    skuModel->setHeaderData(5, Qt::Horizontal, "Dose Type");
    skuModel->setHeaderData(6, Qt::Horizontal, "Prescription");
    skuModel->setHeaderData(7, Qt::Horizontal, "Spec Unit");
    skuModel->setHeaderData(8, Qt::Horizontal, "Spec Quantity");
    skuModel->setHeaderData(9, Qt::Horizontal, "Package");
    skuModel->setHeaderData(10, Qt::Horizontal, "Manufacturer (CN)");
    skuModel->setHeaderData(11, Qt::Horizontal, "Manufacturer (EN)");
    skuModel->setHeaderData(12, Qt::Horizontal, "Product of Origin");

    ioModel = new QSqlTableModel(this, mysql->myDB);
    ioModel->setTable("tmp_ledger");
    ioModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    ioModel->setHeaderData(0, Qt::Horizontal, "Sku ID");
    ioModel->setHeaderData(1, Qt::Horizontal, "Name (CN)");
    ioModel->setHeaderData(2, Qt::Horizontal, "Name (EN)");
    ioModel->setHeaderData(3, Qt::Horizontal, "Alias (CN)");
    ioModel->setHeaderData(4, Qt::Horizontal, "Alias (EN)");
    ioModel->setHeaderData(5, Qt::Horizontal, "Dose Type");
    ioModel->setHeaderData(6, Qt::Horizontal, "Prescription");
    ioModel->setHeaderData(7, Qt::Horizontal, "Spec Unit");
    ioModel->setHeaderData(8, Qt::Horizontal, "Spec Quantity");
    ioModel->setHeaderData(9, Qt::Horizontal, "Package");
    ioModel->setHeaderData(10, Qt::Horizontal, "Manufacturer (CN)");
    ioModel->setHeaderData(11, Qt::Horizontal, "Manufacturer (EN)");
    ioModel->setHeaderData(12, Qt::Horizontal, "Product of Origin");
    ioModel->setHeaderData(13, Qt::Horizontal, "Quantity");
    ioModel->setHeaderData(14, Qt::Horizontal, "Price");
    ioModel->setHeaderData(15, Qt::Horizontal, "Type");
    ioModel->select();

    inventoryModel = new QSqlTableModel(this, mysql->myDB);
    inventoryModel->setTable("tmp_inventory");
    inventoryModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    inventoryModel->setHeaderData(0, Qt::Horizontal, "Name (CN)");
    inventoryModel->setHeaderData(1, Qt::Horizontal, "Name (EN)");
    inventoryModel->setHeaderData(2, Qt::Horizontal, "Alias (CN)");
    inventoryModel->setHeaderData(3, Qt::Horizontal, "Alias (EN)");
    inventoryModel->setHeaderData(4, Qt::Horizontal, "Dose Type");
    inventoryModel->setHeaderData(5, Qt::Horizontal, "Prescription");
    inventoryModel->setHeaderData(6, Qt::Horizontal, "Spec Unit");
    inventoryModel->setHeaderData(7, Qt::Horizontal, "Spec Quantity");
    inventoryModel->setHeaderData(8, Qt::Horizontal, "Package");
    inventoryModel->setHeaderData(9, Qt::Horizontal, "Manufacturer (CN)");
    inventoryModel->setHeaderData(10, Qt::Horizontal, "Manufacturer (EN)");
    inventoryModel->setHeaderData(11, Qt::Horizontal, "Product of Origin");
    inventoryModel->setHeaderData(12, Qt::Horizontal, "Quantity");
    inventoryModel->setHeaderData(13, Qt::Horizontal, "Price");
    inventoryModel->select();

    receiptModel = new QSqlTableModel(this, mysql->myDB);
    receiptModel->setTable("tmp_receipt");
    receiptModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    receiptModel->setHeaderData(0, Qt::Horizontal, "Date");
    receiptModel->setHeaderData(1, Qt::Horizontal, "Type");
    receiptModel->setHeaderData(2, Qt::Horizontal, "Name (CN)");
    receiptModel->setHeaderData(3, Qt::Horizontal, "Name (EN)");
    receiptModel->setHeaderData(4, Qt::Horizontal, "Alias (CN)");
    receiptModel->setHeaderData(5, Qt::Horizontal, "Alias (EN)");
    receiptModel->setHeaderData(6, Qt::Horizontal, "Dose Type");
    receiptModel->setHeaderData(7, Qt::Horizontal, "Prescription");
    receiptModel->setHeaderData(8, Qt::Horizontal, "Spec Unit");
    receiptModel->setHeaderData(9, Qt::Horizontal, "Spec Quantity");
    receiptModel->setHeaderData(10, Qt::Horizontal, "Package");
    receiptModel->setHeaderData(11, Qt::Horizontal, "Manufacturer (CN)");
    receiptModel->setHeaderData(12, Qt::Horizontal, "Manufacturer (EN)");
    receiptModel->setHeaderData(13, Qt::Horizontal, "Product of Origin");
    receiptModel->setHeaderData(14, Qt::Horizontal, "Quantity");
    receiptModel->setHeaderData(15, Qt::Horizontal, "Price");
    receiptModel->select();
}

void MainWindow::createNaviBar(){
    QToolBar *naviBar = new QToolBar("Navigation", this);
    naviBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    addToolBar(Qt::LeftToolBarArea, naviBar);

    const QIcon homeIcon = QIcon::fromTheme("home", QIcon(":/images/home.png"));
    homeAction = new QAction(homeIcon, HOME, this);
    homeAction->setCheckable(true);
    naviBar->addAction(homeAction);

    const QIcon invIcon = QIcon::fromTheme("inventory", QIcon(":/images/inventory.png"));
    invAction = new QAction(invIcon, INV, this);
    invAction->setCheckable(true);
    naviBar->addAction(invAction);

    const QIcon inputIcon = QIcon::fromTheme("input", QIcon(":/images/input.jpeg"));
    inputAction = new QAction(inputIcon, INPUT, this);
    inputAction->setCheckable(true);
    naviBar->addAction(inputAction);

    const QIcon outputIcon = QIcon::fromTheme("output", QIcon(":/images/output.jpeg"));
    outputAction = new QAction(outputIcon,OUTPUT, this);
    outputAction->setCheckable(true);
    naviBar->addAction(outputAction);

    const QIcon monitorInIcon = QIcon::fromTheme("monitor input", QIcon(":/images/monitorIn.png"));
    monitorInAction = new QAction(monitorInIcon,MONINPUT, this);
    monitorInAction->setCheckable(true);
    naviBar->addAction(monitorInAction);

    const QIcon monitorOutIcon = QIcon::fromTheme("monitor output", QIcon(":/images/monitorOut.jpeg"));
    monitorOutAction = new QAction(monitorOutIcon, MONOUTPUT, this);
    monitorOutAction->setCheckable(true);
    naviBar->addAction(monitorOutAction);

    const QIcon receiptIcon = QIcon::fromTheme("receipt", QIcon(":/images/record.png"));
    receipt = new QAction(receiptIcon, RECEIPT, this);
    receipt->setCheckable(true);
    naviBar->addAction(receipt);

    naviBar->setMovable(false);

    QActionGroup *actionGroup = new QActionGroup(this);
    actionGroup->addAction(homeAction);
    actionGroup->addAction(invAction);
    actionGroup->addAction(inputAction);
    actionGroup->addAction(outputAction);
    actionGroup->addAction(monitorInAction);
    actionGroup->addAction(monitorOutAction);
    actionGroup->addAction(receipt);
}

void MainWindow::createStackWidget(){
    stackWidget = new QStackedWidget;
    QLabel *label1 = new QLabel("1");
    QWidget *inventoryPage = createInventoryPage();
    QWidget *inputPage = createIOPage();
    QLabel *label4 = new QLabel("4");
    QLabel *label5 = new QLabel("5");
    QLabel *label6 = new QLabel("6");
    QWidget *receiptPage = createReceiptPage();
    stackWidget->addWidget(label1);
    stackWidget->addWidget(inventoryPage);
    stackWidget->addWidget(inputPage);
    stackWidget->addWidget(label4);
    stackWidget->addWidget(label5);
    stackWidget->addWidget(label6);
    stackWidget->addWidget(receiptPage);
    setCentralWidget(stackWidget);
}

void MainWindow::switchPages(){
    QAction *action = qobject_cast<QAction *>(sender());
    if(action->text() == HOME){
        currPage = HOME;
        titleText->setText(currPage);
        stackWidget->setCurrentIndex(0);

    }
    else if(action->text() == INV){
        currPage = INV;
        titleText->setText(currPage);
        createTmpInventoryTable();
        inventoryModel->select();
        stackWidget->setCurrentIndex(1);
    }
    else if(action->text() == INPUT){
        currPage = INPUT;
        titleText->setText(currPage);
        stackWidget->setCurrentIndex(2);
    }
    else if(action->text() == OUTPUT){
        currPage = OUTPUT;
        titleText->setText(currPage);
        stackWidget->setCurrentIndex(2);
    }
    else if(action->text() == MONINPUT){
        currPage = MONINPUT;
        titleText->setText(currPage);
        stackWidget->setCurrentIndex(2);
    }
    else if(action->text() == MONOUTPUT){
        currPage = MONOUTPUT;
        titleText->setText(currPage);
        stackWidget->setCurrentIndex(2);
    }
    else if(action->text() == RECEIPT){
        currPage = RECEIPT;
        titleText->setText(currPage);
        calcTotalReceiptPage();
        currReceiptPageDisplay->setText(QString("%1 | %2").arg(currReceiptPage).arg(totalReceiptPage));
        stackWidget->setCurrentIndex(6);
    }
    filterSetting();
}

void MainWindow::checkDB(){
    QString dbFilePath = "/Users/yuqiancao/Documents/CS/build-myStorageSystem-Desktop_arm_darwin_generic_mach_o_64bit-Debug/Database/myMSS.db";
    QFile file(dbFilePath);
    if(!file.exists()){
        QMessageBox messageWarning(QMessageBox::Warning, "Warning","Database pairing failed, initialize database", QMessageBox::Yes|QMessageBox::No);
        if(messageWarning.exec()==QMessageBox::Yes){
            mysql = new mySqlite;
            if(mysql->initSql())
                QMessageBox::information(this, "Database info", "Database reset successful.");
            else
                QMessageBox::information(this, "Database info", "Database reset failed, please contact administrator");
        }
    }
    else{
        mysql = new mySqlite;
        if(!mysql->connectDB())
            QMessageBox::information(this, "database info", "Database reset failed, please contact administrator");
        else
            qDebug() << "Connection successful";
    }
}

QWidget *MainWindow::createIOPage(){
    ioPage = new QWidget;

    titleText = new QLabel();
    titleText->setFont(QFont("Times", 16, QFont::Bold));

    QPushButton *addButton = new QPushButton("Add");
    QPushButton *clearButton = new QPushButton("Clear");
    QPushButton *queryButton = new QPushButton("Search");
    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->addWidget(titleText, 0, Qt::AlignHCenter);
    //titleLayout->addStretch();
    //titleLayout->addWidget(addButton);

    QLabel *nameCNLabel = new QLabel("Name (CN)*");
    nameCNEdit = new QLineEdit;
    QLabel *nameENLabel = new QLabel("Name (EN)*");
    nameENEdit = new QLineEdit;
    QLabel *aliasCNLabel = new QLabel("Alias (CN)");
    aliasCNEdit = new QLineEdit;
    QLabel *aliasENLabel = new QLabel("Alias (EN)");
    aliasENEdit = new QLineEdit;
    QLabel *doseLabel = new QLabel("Dose Type*");
    doseEdit = new QComboBox;
    QStringList doseList;
    doseList << "Granule" << "Capsule" << "Tablet" << "Raw" << "Other";
    doseEdit->addItems(doseList);
    QLabel *presLabel = new QLabel("Formula*");
    presEdit = new QComboBox;
    QStringList presList;
    presList << "Single Herb" << "Formula";
    presEdit->addItems(presList);
    QLabel *specUnitLabel = new QLabel("Spec Unit*");
    specUnitEdit = new QComboBox;
    QStringList specUnitList;
    specUnitList << "Gram" << "Ounces" << "Tablet" << "Pill" << "Capsule" << "";
    specUnitEdit->addItems(specUnitList);
    QLabel *specQuantLabel = new QLabel("Spec Quantity*");
    specQuantEdit = new QLineEdit;
    QLabel *packageLabel = new QLabel("Package*");
    packageEdit = new QComboBox;
    QStringList packageList;
    packageList << "Bottle" << "Box" << "Bag";
    packageEdit->addItems(packageList);
    QLabel *manufCNLabel = new QLabel("Manufacturer (CN)*");
    manufCNEdit = new QLineEdit;
    QLabel *manufENLabel = new QLabel("Manufacturer (EN)*");
    manufENEdit = new QLineEdit;
    QLabel *originLabel = new QLabel("Product of Origin");
    originEdit = new QLineEdit;
    QLabel *quantityLabel = new QLabel("Quantity*");
    quantityEdit = new QLineEdit;
    QLabel *priceLabel = new QLabel("Price");
    priceEdit = new QLineEdit;

    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->addWidget(nameCNLabel, 0, 0);
    gridLayout->addWidget(nameCNEdit, 0, 1);
    gridLayout->addWidget(nameENLabel, 1, 0);
    gridLayout->addWidget(nameENEdit, 1, 1);
    gridLayout->addWidget(aliasCNLabel, 2, 0);
    gridLayout->addWidget(aliasCNEdit, 2, 1);
    gridLayout->addWidget(aliasENLabel, 3, 0);
    gridLayout->addWidget(aliasENEdit, 3, 1);

    gridLayout->addWidget(doseLabel, 0, 2);
    gridLayout->addWidget(doseEdit, 0, 3);
    gridLayout->addWidget(presLabel, 1, 2);
    gridLayout->addWidget(presEdit, 1, 3);
    gridLayout->addWidget(specUnitLabel, 2, 2);
    gridLayout->addWidget(specUnitEdit, 2, 3);
    gridLayout->addWidget(packageLabel, 3, 2);
    gridLayout->addWidget(packageEdit, 3, 3);

    gridLayout->addWidget(manufCNLabel, 0, 4);
    gridLayout->addWidget(manufCNEdit, 0, 5);
    gridLayout->addWidget(manufENLabel, 1, 4);
    gridLayout->addWidget(manufENEdit, 1, 5);
    gridLayout->addWidget(specQuantLabel, 2, 4);
    gridLayout->addWidget(specQuantEdit, 2, 5);
    gridLayout->addWidget(originLabel, 3, 4);
    gridLayout->addWidget(originEdit, 3, 5);

    gridLayout->addWidget(quantityLabel, 0, 6);
    gridLayout->addWidget(quantityEdit, 0, 7);
    gridLayout->addWidget(priceLabel, 1, 6);
    gridLayout->addWidget(priceEdit, 1, 7);
    gridLayout->addWidget(queryButton, 3, 6);
    gridLayout->addWidget(addButton, 2, 7);
    gridLayout->addWidget(clearButton, 3, 7);

    QFrame *sFrame = new QFrame;
    sFrame->setFrameStyle(QFrame::StyledPanel);
    sFrame->setLayout(gridLayout);

/*    //---------------------------------------------------------
    * QHBoxLayout *inputLayout_1 = new QHBoxLayout;
    inputLayout_1->addWidget(nameCNLabel);
    inputLayout_1->addWidget(nameCNEdit);
    inputLayout_1->addStretch();
    inputLayout_1->addWidget(nameENLabel);
    inputLayout_1->addWidget(nameENEdit);
    inputLayout_1->addStretch();
    inputLayout_1->addWidget(aliasCNLabel);
    inputLayout_1->addWidget(aliasCNEdit);
    inputLayout_1->addStretch();
    inputLayout_1->addWidget(aliasENLabel);
    inputLayout_1->addWidget(aliasENEdit);

    QHBoxLayout *inputLayout_2 = new QHBoxLayout;
    inputLayout_2->addWidget(doseLabel);
    inputLayout_2->addWidget(doseEdit);
    inputLayout_2->addStretch();
    inputLayout_2->addWidget(presLabel);
    inputLayout_2->addWidget(presEdit);
    inputLayout_2->addStretch();
    inputLayout_2->addWidget(specUnitLabel);
    inputLayout_2->addWidget(specUnitEdit);
    inputLayout_2->addStretch();
    inputLayout_2->addWidget(specQuantLabel);
    inputLayout_2->addWidget(specQuantEdit);

    QHBoxLayout *inputLayout_3 = new QHBoxLayout;
    inputLayout_3->addWidget(packageLabel);
    inputLayout_3->addWidget(packageEdit);
    inputLayout_3->addStretch();
    inputLayout_3->addWidget(manufCNLabel);
    inputLayout_3->addWidget(manufCNEdit);
    inputLayout_3->addStretch();
    inputLayout_3->addWidget(manufENLabel);
    inputLayout_3->addWidget(manufENEdit);

    QHBoxLayout *inputLayout_4 = new QHBoxLayout;
    inputLayout_4->addWidget(originLabel);
    inputLayout_4->addWidget(originEdit);
    inputLayout_4->addStretch();
    inputLayout_4->addWidget(quantityLabel);
    inputLayout_4->addWidget(quantityEdit);
    inputLayout_4->addStretch();
    inputLayout_4->addWidget(priceLabel);
    inputLayout_4->addWidget(priceEdit);
    inputLayout_4->addStretch();
    inputLayout_4->addWidget(clearButton);
    inputLayout_4->addWidget(queryButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(inputLayout_1);
    layout->addLayout(inputLayout_2);
    layout->addLayout(inputLayout_3);
    layout->addLayout(inputLayout_4);
    QFrame *sFrame = new QFrame;
    sFrame->setFrameStyle(QFrame::StyledPanel);
    sFrame->setLayout(layout);
*/

    ioTableView = new QTableView(this);
    ioTableView->setModel(ioModel);
    ioTableView->setColumnHidden(0, true);
    ioTableView->setColumnHidden(15, true);
    int columns = ioModel->columnCount();
    for(int c = 0; c < columns - 3; c++){
        ioTableView->setItemDelegateForColumn(c, new setNoEdit(ioTableView));
    }
    ioTableView->setEditTriggers(QAbstractItemView::DoubleClicked);

    QLabel* editLabel = new QLabel(">> Double click to edit, press enter to submit change");
    QPushButton *deleteButton = new QPushButton("Delete");
    QPushButton *deleteAllButton = new QPushButton("Delete All");
    QPushButton *submitButton = new QPushButton("Submit");
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(editLabel);
    buttonLayout->addStretch();
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(deleteAllButton);
    QHBoxLayout *buttonLayout_2 = new QHBoxLayout;
    buttonLayout_2->addStretch();
    buttonLayout_2->addWidget(submitButton);

    QVBoxLayout *tableLayout = new QVBoxLayout;
    tableLayout->addLayout(buttonLayout);
    tableLayout->addWidget(ioTableView);
    tableLayout->addLayout(buttonLayout_2);
    QFrame *sFrame_2 = new QFrame;
    sFrame_2->setFrameStyle(QFrame::StyledPanel);
    sFrame_2->setLayout(tableLayout);
\
    connect(nameCNEdit, &QLineEdit::editingFinished, this, &MainWindow::skuExist);

    connect(nameENEdit, &QLineEdit::editingFinished, this, &MainWindow::skuExist);

    connect(aliasCNEdit, &QLineEdit::editingFinished, this, &MainWindow::skuExist);

    connect(aliasENEdit, &QLineEdit::editingFinished, this, &MainWindow::skuExist);


    QVBoxLayout *wholeLayout = new QVBoxLayout;
    wholeLayout->addLayout(titleLayout);
    wholeLayout->addWidget(sFrame);
    wholeLayout->addWidget(sFrame_2);
    ioPage->setLayout(wholeLayout);

    connect(addButton, &QPushButton::clicked, this, &MainWindow::checkIOFields);
    connect(queryButton, &QPushButton::clicked, this, &MainWindow::searchIOData);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearIOFields);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteInputData);
    connect(deleteAllButton, &QPushButton::clicked, this, &MainWindow::deleteAllInputData);
    connect(submitButton, &QPushButton::clicked, this, &MainWindow::submitData);

    return ioPage;
}

void MainWindow::filterSetting(){
    ioModel->setFilter("");
    ioModel->setFilter(QString("io_type = '%1'").arg(currPage));
    ioModel->select();
}

void MainWindow::clearIOFields(){
    nameCNEdit->clear();
     nameENEdit->clear();
     aliasCNEdit->clear();
     aliasENEdit->clear();
     doseEdit->setCurrentIndex(0);
     presEdit->setCurrentIndex(0);
     specUnitEdit->setCurrentIndex(0);
     specQuantEdit->clear();
     packageEdit->clear();
     manufCNEdit->clear();
     manufENEdit->clear();
     originEdit->clear();
     quantityEdit->clear();
     priceEdit->clear();
     exist = false;
}

//Adding to tmp_ledger table -------------------------------------------------

//check if needed fields are filled in
//If not, generate error
void MainWindow::checkIOFields(){
    /*if(nameCNEdit->text() == "" || nameENEdit->text() == "" || doseEdit->currentText() == "" || presEdit->currentText() == ""
    || specUnitEdit->currentText() == "" || specQuantEdit->text() == "" || packageEdit->text() == "" ||
    quantityEdit->text() == "" || manufCNEdit->text() == "" || manufENEdit->text() == ""){
        QMessageBox::warning(this, "Error", "Please fill in all required fields before proceeding", QMessageBox::Ok);
    }
    else{*/
        //Change this part depending on page
        //if(currPage == INPUT)
            insertStockData();
    //}

}

//user types in either nameCNEdit, nameENEdit, aliasCNEdit, aliasENEdit
//check if corresponding data exists in SKU table
void MainWindow::skuExist(){
    qDebug() << "skuExist...";
    qDebug() << "exist?: " << exist;
    QSqlQuery *query = new QSqlQuery(mysql->myDB);
    int count = 0;
    if(!nameCNEdit->text().isEmpty()){
        QString tmpStr = QString("select count(*) from SKU where name_cn = '%1'").arg(nameCNEdit->text());
        if(!query->exec(tmpStr)){
            qDebug() << "query execute selection from SKU failed" << query->lastError().text();
        }
        if(query->next()){
            count = query->value(0).toInt();
            qDebug() << tmpStr << " count: " << count;
            if(count > 0 && !exist){
                exist = true;
                createTmpSkuView();}
        }

    }
    else if(!nameENEdit->text().isEmpty()){
        QString tmpStr = QString("select count(*) from SKU where name_en = '%1'").arg(nameENEdit->text());
        query->exec(tmpStr);
        if(query->next()){
            count = query->value(0).toInt();
            qDebug() << tmpStr << " count: " << count;
            if(count > 0 && !exist){
                exist = true;
                createTmpSkuView();}
        }
    }
    else if(!aliasCNEdit->text().isEmpty()){
        QString tmpStr = QString("select count(*) from SKU where alias_cn = '%1'").arg(aliasCNEdit->text());
        query->exec(tmpStr);
        if(query->next()){
            count = query->value(0).toInt();
            qDebug() << tmpStr << " count: " << count;
            if(count > 0 && !exist){
                exist = true;
                createTmpSkuView();}
        }
    }
    else if(!aliasENEdit->text().isEmpty()){
        QString tmpStr = QString("select count(*) from SKU where alias_en = '%1'").arg(aliasENEdit->text());
        query->exec(tmpStr);
        if(query->next()){
            count = query->value(0).toInt();
            qDebug() << tmpStr << " count: " << count;
            if(count > 0 && !exist){
                exist = true;
                createTmpSkuView();}
        }
    }
    delete query;
    qDebug() << "skuExist exiting...";
}

//called if matching field(s) exist
//pop up window
void MainWindow::createTmpSkuView(){
    qDebug() << "createTmpSkuView called";

    //filter out sku table
    QString filterStr = "";

    if(!nameCNEdit->text().isEmpty())
        filterStr.append(QString("name_cn like '\%%1\%'").arg(nameCNEdit->text()));
    if(!nameENEdit->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(QString("name_en like '\%%1\%'").arg(nameENEdit->text()));
    }
    if(!aliasCNEdit->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(QString("alias_cn like '\%%1\%'").arg(aliasCNEdit->text()));
    }
    if(!aliasENEdit->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(QString("alias_en like '\%%1\%'").arg(aliasENEdit->text()));
    }
    qDebug() << "filter str: " << filterStr << "\n";
    skuModel->setFilter("");
    skuModel->setFilter(filterStr);
    skuModel->select();

    autoSkuData = new autofillSku(skuModel, this);//mysql, skuId, nameCNEdit, nameENEdit, aliasCNEdit, aliasENEdit, doseEdit, presEdit, specUnitEdit, specQuantEdit, packageEdit, manufCNEdit, manufENEdit, originEdit, this);
    autoSkuData->show();

    //currRow = autoSkuData->tmpTableView->currentIndex().row();

    connect(autoSkuData->addButton, &QPushButton::clicked, this, &MainWindow::fillLines);
    connect(autoSkuData->cancelButton, &QPushButton::clicked, this, &MainWindow::tmpFunc);

}

void MainWindow::fillLines(){
    currRow = autoSkuData->tmpTableView->currentIndex().row();
    qDebug() << "currRow: " << currRow;
    //QString tmpStr = QString("select * from SKU where sku_id = %1").arg(tmpSkuId);
        //QSqlQuery *query = new QSqlQuery(mysql->myDB);
       // query->exec(tmpStr);
       // query->next();
        nameCNEdit->setText(skuModel->record(currRow).value(1).toString());
        nameENEdit->setText(skuModel->record(currRow).value(2).toString());
        aliasCNEdit->setText(skuModel->record(currRow).value(3).toString());
        aliasENEdit->setText(skuModel->record(currRow).value(4).toString());
        doseEdit->setCurrentText(skuModel->record(currRow).value(5).toString());
        presEdit->setCurrentText(skuModel->record(currRow).value(6).toString());
        specUnitEdit->setCurrentText(skuModel->record(currRow).value(7).toString());
        specQuantEdit->setText(skuModel->record(currRow).value(8).toString());
        packageEdit->setCurrentText(skuModel->record(currRow).value(9).toString());
        manufCNEdit->setText(skuModel->record(currRow).value(10).toString());
        manufENEdit->setText(skuModel->record(currRow).value(11).toString());
        originEdit->setText(skuModel->record(currRow).value(12).toString());
        autoSkuData->close();
}

void MainWindow::tmpFunc(){
    autoSkuData->close();
}

void MainWindow::insertStockData(){

    int skuId = 0;
    QString nameCN = nameCNEdit->text();
    QString nameEN = nameENEdit->text();
    QString aliasCN = aliasCNEdit->text();
    QString aliasEN = aliasENEdit->text();
    QString dose = doseEdit->currentText();
    QString pres = presEdit->currentText();
    QString specUnit = specUnitEdit->currentText();
    float specQuant = specQuantEdit->text().toFloat();
    QString package = packageEdit->currentText();
    QString manufCN = manufCNEdit->text();
    QString manufEN = manufENEdit->text();
    QString origin = originEdit->text();
    int quantity = quantityEdit->text().toInt();
    float price = priceEdit->text().toFloat();

    //search in SKU table, name + manufacturer
    QString querySKUStr = QString("select sku_id from SKU where name_cn = '%1' and manufacturer_cn = '%2'").arg(nameCN).arg(manufCN);
    QSqlQuery *query = new QSqlQuery(mysql->myDB);
    if(!query->exec(querySKUStr)){
        qDebug() << "query sku failed";
        return;}
    if(query->next()){
        //sku exist
        skuId = query->value(0).toInt();
    }
    else{
        //sku does not exist, insert one new record into SKU
        QSqlRecord skuRecord = skuModel->record();
        skuRecord.setValue(1, nameCN);
        skuRecord.setValue(2, nameEN);
        skuRecord.setValue(3, aliasCN);
        skuRecord.setValue(4, aliasEN);
        skuRecord.setValue(5, dose);
        skuRecord.setValue(6, pres);
        skuRecord.setValue(7, specUnit);
        skuRecord.setValue(8, specQuant);
        skuRecord.setValue(9, package);
        skuRecord.setValue(10, manufCN);
        skuRecord.setValue(11, manufEN);
        skuRecord.setValue(12, origin);
        skuModel->insertRecord(skuModel->rowCount(), skuRecord);
        skuModel->submitAll();
        QString tmpFilter = QString("name_cn = '%1' and manufacturer_cn = '%2'").arg(nameCN).arg(manufCN);

        skuModel->setFilter(tmpFilter);
        skuModel->select();
        skuId = skuModel->record(0).value(0).toInt();
    }


    QSqlRecord io_record = ioModel->record();
    io_record.setValue(0, skuId);
    io_record.setValue(1, nameCN);
    io_record.setValue(2, nameEN);
    io_record.setValue(3, aliasCN);
    io_record.setValue(4, aliasEN);
    io_record.setValue(5, dose);
    io_record.setValue(6, pres);
    io_record.setValue(7, specUnit);
    io_record.setValue(8, specQuant);
    io_record.setValue(9, package);
    io_record.setValue(10, manufCN);
    io_record.setValue(11, manufEN);
    io_record.setValue(12, origin);
    io_record.setValue(13, quantity);
    io_record.setValue(14, price);
    io_record.setValue(15, currPage);
    ioModel->insertRecord(ioModel->rowCount(), io_record);
    ioModel->submitAll();
    ioModel->select();

    //clearIOFields();
    nameCNEdit->clear();
     nameENEdit->clear();
     aliasCNEdit->clear();
     aliasENEdit->clear();
     quantityEdit->setText("1");
    exist = false;

}

//-------------------------------------------------------------------------------

void MainWindow::searchIOData(){
    QString nameCN = QString("name_cn = '%1' or name_en = '%1' or alias_cn = '%1' or alias_en = '%1'").arg(nameCNEdit->text());
    QString nameEN = QString("name_cn = '%1' or name_en = '%1' or alias_cn = '%1' or alias_en = '%1'").arg(nameENEdit->text());
    QString aliasCN = QString("name_cn = '%1' or name_en = '%1' or alias_cn = '%1' or alias_en = '%1'").arg(aliasCNEdit->text());
    QString aliasEN = QString("name_cn = '%1' or name_en = '%1' or alias_cn = '%1' or alias_en = '%1'").arg(aliasENEdit->text());
    QString type = QString("io_type = '%1'").arg(currPage);
    QString filterStr = "";
    if(!nameCNEdit->text().isEmpty()){
        filterStr.append(nameCN);
    }
    if(!nameENEdit->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(nameEN);
    }
    if(!aliasCNEdit->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(aliasCN);
    }
    if(!aliasENEdit->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(aliasEN);
    }
    /*
    if(!doseEdit->currentText().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(dose);
    }
    if(!presEdit->currentText().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(pres);
    }
    if(!specUnitEdit->currentText().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(specUnit);
    }
    if(!specQuantEdit->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(specQuant);
    }
    if(!packageEdit->currentText().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(package);
    }
    if(!manufCNEdit->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(manufCN);
    }
    if(!manufENEdit->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(manufEN);
    }
    if(!originEdit->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(origin);
    }
    if(!quantityEdit->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(quantity);
    }
    if(!priceEdit->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(price);
    }
*/
    if(!filterStr.isEmpty())
        filterStr.append(" and ");
    filterStr.append(type);

    ioModel->setFilter("");
    ioModel->setFilter(filterStr);
    ioModel->select();


}

void MainWindow::deleteInputData(){
    int curRow = ioTableView->currentIndex().row();
    ioModel->removeRow(curRow);
    ioModel->submitAll();
}

void MainWindow::deleteAllInputData(){
    QString data_str = QString("delete from tmp_ledger where io_type = '%1'").arg(currPage);
    QSqlQuery *query = new QSqlQuery(mysql->myDB);
    if(!query->exec(data_str))
        qDebug() << "delete all failed";
    ioModel->select();
    qDebug() << query->lastError().text();
}

//Submit Data, Update Record, Ledger, Inventory, SKU-------------------------------
void MainWindow::submitData(){

    qDebug() << "\nsubmitting data...";
 /*
    //add to SKU ---------------------
  QString data_str = QString("insert into SKU(name_cn, name_en, alias_cn, alias_en, dose_type, prescription, spec_unit, spec_quantity, package, manufacturer_cn, manufacturer_en, product_of_origin)"
                               " select name_cn, name_en, alias_cn, alias_en, dose_type, prescription, spec_unit, spec_quantity, package, manufacturer_cn, manufacturer_en, product_of_origin"
                                 " from tmp_ledger where io_type = '%1' and sku_id = -1").arg(currPage);

    QSqlQuery *query = new QSqlQuery(mysql->myDB);
    qDebug() << "SKU: " << data_str;
    query->exec(data_str);
    //--------------------------------


    //update tmp_ledger------------------------------
    data_str = QString("update tmp_ledger set sku_id = (select sku_id from SKU where SKU.name_cn = tmp_ledger.name_cn and SKU.name_en = tmp_ledger.name_en and SKU.alias_cn = tmp_ledger.alias_cn and SKU.alias_en = tmp_ledger.alias_en and SKU.manufacturer_cn = tmp_ledger.manufacturer_cn)");
    query->exec(data_str);
    //---------------------------------------------

*/
    //add one new record to receipt ---------------------
    QSqlQuery *query = new QSqlQuery(mysql->myDB);
    QDate currDate = QDateTime::currentDateTime().date();

    QString tmpTimeStr = QDateTime::currentDateTime().time().toString("h:m");
    qDebug() << "tmpTimeStr: " << tmpTimeStr;

    QString receipt_date = currDate.toString("yyyy_MM_dd");

    //QString data_str = QString("insert into receipt (io_type, receipt_date) values('%1', '%2')").arg(currPage).arg(receipt_date);
    QString data_str = QString("insert into receipt (io_type, receipt_date) values('%1', '%2')").arg(currPage).arg(tmpTimeStr);
    qDebug() << "receipt: " << data_str;
    //qDebug() << "receipt ID: " << receiptId;
    query->exec(data_str);

    data_str = QString("select max(receipt_id) from receipt");
    query->exec(data_str);
    int receiptId;
    if(query->next())
        receiptId = query->value(0).toInt();
    else{
        qDebug() << "extracting receipt_id from receipt failed";
        return;
    }
    qDebug() << "receipt ID: " << receiptId;
    //------------------------------------------


    //add to ledger ---------------------
    //data_str = QString("insert into ledger (receipt_id, receipt_date, sku_id, quantity, price, io_type) select %1, '%2', sku_id, quantity, price, io_type from tmp_ledger where io_type = '%3'").arg(receiptId).arg(receipt_date).arg(currPage);
    data_str = QString("insert into ledger (receipt_id, receipt_date, sku_id, quantity, price, io_type) select %1, '%2', sku_id, quantity, price, io_type from tmp_ledger where io_type = '%3'").arg(receiptId).arg(tmpTimeStr).arg(currPage);

    if(!query->exec(data_str)){
        qDebug() << "add record to query not successful" << query->lastError().text();
    }

    qDebug() << "dateSelection count: " << dateSelection->count();
    qDebug() << "dateSelection last text: " << dateSelection->itemText(dateSelection->count()-1);
    if(dateSelection->itemText(dateSelection->count()-1) != tmpTimeStr)
        dateSelection->addItem(tmpTimeStr);
    createTmpReceiptTable();
    deleteAllInputData();

}
//----------------------------------------------------------------------------------

QWidget* MainWindow::createInventoryPage(){
    QWidget *inventoryPage = new QWidget();
    QLabel *inventoryTitle = new QLabel("Inventory Page");
    inventoryTitle->setFont(QFont("Times", 16, QFont::Bold));
    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->addWidget(inventoryTitle, 0, Qt::AlignHCenter);


    QLabel *nameCNLabel = new QLabel("Name (CN)");
    nameCNEditInv = new QLineEdit;
    QLabel *nameENLabel = new QLabel("Name (EN)");
    nameENEditInv = new QLineEdit;
    QLabel *aliasCNLabel = new QLabel("Alias (CN)");
    aliasCNEditInv = new QLineEdit;
    QLabel *aliasENLabel = new QLabel("Alias (EN)");
    aliasENEditInv = new QLineEdit;
    QLabel *doseLabel = new QLabel("Dose Type");
    doseEditInv = new QComboBox;
    QStringList doseList;
    doseList << "" << "Granule" << "Capsule" << "Tablet" << "Taw" << "Other";
    doseEditInv->addItems(doseList);
    QLabel *presLabel = new QLabel("Formula*");
    presEditInv = new QComboBox;
    QStringList presList;
    presList << "" << "Single Herb" << "Formula";
    presEditInv->addItems(presList);
    QLabel *specUnitLabel = new QLabel("Spec Unit*");
    specUnitEditInv = new QComboBox;
    QStringList specUnitList;
    specUnitList << "" << "Gram" << "Ounces" << "Tablet" << "Pill" << "Capsule" ;
    specUnitEditInv->addItems(specUnitList);
    QLabel *specQuantLabel = new QLabel("Spec Quantity*");
    specQuantEditInv = new QLineEdit;
    QLabel *packageLabel = new QLabel("Package*");
    packageEditInv = new QComboBox;
    QStringList packageList;
    packageList << "" << "Bottle" << "Box" << "Bag";
    packageEditInv->addItems(packageList);
    QLabel *manufCNLabel = new QLabel("Manufacturer (CN)*");
    manufCNEditInv = new QLineEdit;
    QLabel *manufENLabel = new QLabel("Manufacturer (EN)*");
    manufENEditInv = new QLineEdit;
    QLabel *originLabel = new QLabel("Product of Origin");
    originEditInv = new QLineEdit;
    QLabel *quantityLabel = new QLabel("Quantity*");
    quantityEditInv = new QLineEdit;
    QLabel *priceLabel = new QLabel("Price");
    priceEditInv = new QLineEdit;

    QPushButton *queryButton = new QPushButton("Search");
    QPushButton *clearButton = new QPushButton("Clear");

    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->addWidget(nameCNLabel, 0, 0);
    gridLayout->addWidget(nameCNEditInv, 0, 1);
    gridLayout->addWidget(nameENLabel, 1, 0);
    gridLayout->addWidget(nameENEditInv, 1, 1);
    gridLayout->addWidget(aliasCNLabel, 2, 0);
    gridLayout->addWidget(aliasCNEditInv, 2, 1);
    gridLayout->addWidget(aliasENLabel, 3, 0);
    gridLayout->addWidget(aliasENEditInv, 3, 1);

    gridLayout->addWidget(doseLabel, 0, 2);
    gridLayout->addWidget(doseEditInv, 0, 3);
    gridLayout->addWidget(presLabel, 1, 2);
    gridLayout->addWidget(presEditInv, 1, 3);
    gridLayout->addWidget(specUnitLabel, 2, 2);
    gridLayout->addWidget(specUnitEditInv, 2, 3);
    gridLayout->addWidget(packageLabel, 3, 2);
    gridLayout->addWidget(packageEditInv, 3, 3);

    gridLayout->addWidget(manufCNLabel, 0, 4);
    gridLayout->addWidget(manufCNEditInv, 0, 5);
    gridLayout->addWidget(manufENLabel, 1, 4);
    gridLayout->addWidget(manufENEditInv, 1, 5);
    gridLayout->addWidget(specQuantLabel, 2, 4);
    gridLayout->addWidget(specQuantEditInv, 2, 5);
    gridLayout->addWidget(originLabel, 3, 4);
    gridLayout->addWidget(originEditInv, 3, 5);

    gridLayout->addWidget(quantityLabel, 0, 6);
    gridLayout->addWidget(quantityEditInv, 0, 7);
    gridLayout->addWidget(priceLabel, 1, 6);
    gridLayout->addWidget(priceEditInv, 1, 7);
    gridLayout->addWidget(queryButton, 2, 7);
    gridLayout->addWidget(clearButton, 3, 7);

    invTableView = new QTableView(this);
    invTableView->setModel(inventoryModel);
    invTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget(invTableView);

    QFrame *layout = new QFrame();
    layout->setFrameStyle(QFrame::StyledPanel);
    layout->setLayout(gridLayout);

    QFrame *layout_2 = new QFrame();
    layout_2->setFrameStyle(QFrame::StyledPanel);
    layout_2->setLayout(vLayout);

    QVBoxLayout *wholeLayout = new QVBoxLayout();
    wholeLayout->addLayout(titleLayout);
    wholeLayout->addWidget(layout);
    wholeLayout->addWidget(layout_2);

    inventoryPage->setLayout(wholeLayout);

    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearInvFields);
    connect(queryButton, &QPushButton::clicked, this, &MainWindow::searchInvData);

    return inventoryPage;
}

void MainWindow::createTmpInventoryTable(){
    QString createTmpTableStr = QString("delete from tmp_inventory");
    QSqlQuery *query = new QSqlQuery(mysql->myDB);
    if(!query->exec(createTmpTableStr)){
        qDebug() << "failed to delete from tmp_inventory: " << query->lastError().text();
    }
    createTmpTableStr = QString("insert into tmp_inventory (name_cn, name_en, alias_cn, alias_en, dose_type, prescription, spec_unit, spec_quantity, package, manufacturer_cn, manufacturer_en, product_of_origin, price, quantity) "
                                "select name_cn, name_en, alias_cn, alias_en, dose_type, prescription, spec_unit, spec_quantity, package, manufacturer_cn, manufacturer_en, product_of_origin, price, quantity "
                                "from SKU inner join inventory on SKU.sku_id = inventory.sku_id "
                                );
    if(!query->exec(createTmpTableStr)){
        qDebug() << "insert into tmp_inventory through sku join inventory failed...\n" << query->lastError().text();
    }
}

void MainWindow::clearInvFields(){
    nameCNEditInv->clear();
    nameENEditInv->clear();
    aliasCNEditInv->clear();
    aliasENEditInv->clear();
    doseEditInv->setCurrentIndex(0);
    presEditInv->setCurrentIndex(0);
    specUnitEditInv->setCurrentIndex(0);
    specQuantEditInv->clear();
    packageEditInv->clear();
    manufCNEditInv->clear();
    manufENEditInv->clear();
    originEditInv->clear();
    quantityEditInv->clear();
    priceEditInv->clear();
}

void MainWindow::searchInvData(){
    QString nameCN = QString("name_cn = '%1'").arg(nameCNEditInv->text());
    QString nameEN = QString("name_en = '%1'").arg(nameENEditInv->text());
    QString aliasCN = QString("alias_cn = '%1'").arg(aliasCNEditInv->text());
    QString aliasEN = QString("alias_en = '%1'").arg(aliasENEditInv->text());
    QString dose = QString("dose_type = '%1'").arg(doseEditInv->currentText());
    QString pres = QString("prescription = '%1'").arg(presEditInv->currentText());
    QString specUnit = QString("spec_unit = '%1'").arg(specUnitEditInv->currentText());
    QString specQuant = QString("spec_quantity = '%1'").arg(specQuantEditInv->text());
    QString package = QString("package = '%1'").arg(packageEditInv->currentText());
    QString manufCN = QString("manufucturer_cn = '%1'").arg(manufCNEditInv->text());
    QString manufEN = QString("manufucturer_en = '%1'").arg(manufENEditInv->text());
    QString origin = QString("product_of_origin = '%1'").arg(originEditInv->text());
    QString quantity = QString("quantity = '%1'").arg(quantityEditInv->text());
    QString price = QString("price = '%1'").arg(priceEditInv->text());
    QString filterStr = "";
    if(!nameCNEditInv->text().isEmpty())
        filterStr.append(nameCN);
    if(!nameENEditInv->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(nameEN);
    }
    if(!aliasCNEditInv->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(aliasCN);
    }
    if(!aliasENEditInv->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(aliasEN);
    }
    if(!doseEditInv->currentText().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(dose);
    }
    if(!presEditInv->currentText().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(pres);
    }
    if(!specUnitEditInv->currentText().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(specUnit);
    }
    if(!specQuantEditInv->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(specQuant);
    }
    if(!packageEditInv->currentText().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(package);
    }
    if(!manufCNEditInv->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(manufCN);
    }
    if(!manufENEditInv->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(manufEN);
    }
    if(!originEditInv->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(origin);
    }
    if(!quantityEditInv->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(quantity);
    }
    if(!priceEditInv->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(price);
    }
    inventoryModel->setFilter("");
    inventoryModel->setFilter(filterStr);
    inventoryModel->select();

}

QWidget* MainWindow::createReceiptPage(){
    QWidget *receipt = new QWidget();
    QLabel *inventoryTitle = new QLabel("Receipt Page");
    inventoryTitle->setFont(QFont("Times", 16, QFont::Bold));
    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->addWidget(inventoryTitle, 0, Qt::AlignHCenter);

    //date selection drop down checkbox
    QLabel *dateLabel = new QLabel("Date");
    dateSelection = new QComboBox();
    QStringList dateList;
    dateList << "";
    QString dateStr = QString("select receipt_date from receipt");
    QSqlQuery *query = new QSqlQuery(mysql->myDB);
    if(!query->exec(dateStr)){
        qDebug() << "fail to select receipt_date from receipt: " << query->lastError().text();
    }
    QString tmpDateStr = "";
    while(query->next()){
        if(query->value(0).toString() != tmpDateStr){
            dateList << query->value(0).toString();
            tmpDateStr = query->value(0).toString();
        }
    }
    dateSelection->addItems(dateList);

    //io type selection drop down
    QLabel *typeLabel = new QLabel("Type");
    typeSelection = new QComboBox();
    QStringList typeList;
    typeList << "" << INPUT << OUTPUT << MONINPUT << MONOUTPUT;
    typeSelection->addItems(typeList);

    QPushButton *queryButton = new QPushButton("Search");
    QPushButton *clearButton = new QPushButton("Clear");
    QPushButton *resetButton = new QPushButton("Reset");

    QLabel *nameCNLabel = new QLabel("Name (CN)*");
    nameCNEditReceipt = new QLineEdit;
    QLabel *nameENLabel = new QLabel("Name (EN)*");
    nameENEditReceipt = new QLineEdit;
    QLabel *aliasCNLabel = new QLabel("Alias (CN)");
    aliasCNEditReceipt = new QLineEdit;
    QLabel *aliasENLabel = new QLabel("Alias (EN)");
    aliasENEditReceipt = new QLineEdit;
    QLabel *doseLabel = new QLabel("Dose Type*");
    doseEditReceipt = new QComboBox;
    QStringList doseList;
    doseList << "" << "Granule" << "Capsule" << "Tablet" << "Taw" << "Other";
    doseEditReceipt->addItems(doseList);
    QLabel *presLabel = new QLabel("Formula*");
    presEditReceipt = new QComboBox;
    QStringList presList;
    presList << "" << "Single Herb" << "Formula";
    presEditReceipt->addItems(presList);
    QLabel *specUnitLabel = new QLabel("Spec Unit*");
    specUnitEditReceipt = new QComboBox;
    QStringList specUnitList;
    specUnitList << "" << "Gram" << "Ounces" << "Tablet" << "Pill" << "Capsule" << "";
    specUnitEditReceipt->addItems(specUnitList);
    QLabel *specQuantLabel = new QLabel("Spec Quantity*");
    specQuantEditReceipt = new QLineEdit;
    QLabel *packageLabel = new QLabel("Package*");
    packageEditReceipt = new QComboBox;
    QStringList packageList;
    packageList << "" << "Bottle" << "Box" << "Bag";
    packageEditReceipt->addItems(packageList);
    QLabel *manufCNLabel = new QLabel("Manufacturer (CN)*");
    manufCNEditReceipt = new QLineEdit;
    QLabel *manufENLabel = new QLabel("Manufacturer (EN)*");
    manufENEditReceipt = new QLineEdit;
    QLabel *originLabel = new QLabel("Product of Origin");
    originEditReceipt = new QLineEdit;
    QLabel *quantityLabel = new QLabel("Quantity*");
    quantityEditReceipt = new QLineEdit;
    QLabel *priceLabel = new QLabel("Price");
    priceEditReceipt = new QLineEdit;

    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->addWidget(nameCNLabel, 1, 0);
    gridLayout->addWidget(nameCNEditReceipt, 1, 1);
    gridLayout->addWidget(nameENLabel, 2, 0);
    gridLayout->addWidget(nameENEditReceipt, 2, 1);
    gridLayout->addWidget(aliasCNLabel, 3, 0);
    gridLayout->addWidget(aliasCNEditReceipt, 3, 1);
    gridLayout->addWidget(aliasENLabel, 4, 0);
    gridLayout->addWidget(aliasENEditReceipt, 4, 1);

    gridLayout->addWidget(doseLabel, 1, 2);
    gridLayout->addWidget(doseEditReceipt, 1, 3);
    gridLayout->addWidget(presLabel, 2, 2);
    gridLayout->addWidget(presEditReceipt, 2, 3);
    gridLayout->addWidget(specUnitLabel, 3, 2);
    gridLayout->addWidget(specUnitEditReceipt, 3, 3);
    gridLayout->addWidget(packageLabel, 4, 2);
    gridLayout->addWidget(packageEditReceipt, 4, 3);

    gridLayout->addWidget(manufCNLabel, 1, 4);
    gridLayout->addWidget(manufCNEditReceipt, 1, 5);
    gridLayout->addWidget(manufENLabel, 2, 4);
    gridLayout->addWidget(manufENEditReceipt, 2, 5);
    gridLayout->addWidget(specQuantLabel, 3, 4);
    gridLayout->addWidget(specQuantEditReceipt, 3, 5);
    gridLayout->addWidget(originLabel, 4, 4);
    gridLayout->addWidget(originEditReceipt, 4, 5);

    gridLayout->addWidget(quantityLabel, 1, 6);
    gridLayout->addWidget(quantityEditReceipt, 1, 7);
    gridLayout->addWidget(priceLabel, 2, 6);
    gridLayout->addWidget(priceEditReceipt, 2, 7);
    gridLayout->addWidget(queryButton, 3, 7);
    gridLayout->addWidget(clearButton, 4, 7);
    gridLayout->addWidget(resetButton, 4, 6);

    QGridLayout *gridLayout_2 = new QGridLayout();
    gridLayout_2->addWidget(dateLabel, 0, 0);
    gridLayout_2->addWidget(dateSelection, 0, 1);
    gridLayout_2->addWidget(typeLabel, 0, 2);
    gridLayout_2->addWidget(typeSelection, 0, 3);
    QLabel *nullLabel = new QLabel("    ");
    gridLayout_2->addWidget(nullLabel, 0, 4);
    gridLayout_2->addWidget(nullLabel, 0, 5);
    gridLayout_2->addWidget(nullLabel, 0, 6);
    gridLayout_2->addWidget(nullLabel, 0, 7);

    createTmpReceiptTable();
    receiptTableView = new QTableView(this);
    receiptTableView->setModel(receiptModel);
    receiptTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    receiptTableView->setColumnWidth(1, receiptTableView->columnWidth(1)*3);
    receiptTableView->resizeColumnToContents(1);
    query->exec(dateStr);
    if(query->next()){
        currReceiptDate = query->value(0).toString();
        receiptModel->setFilter(QString("receipt_date = '%1'").arg(query->value(0).toString()));
    }

    QPushButton *nextPageButton = new QPushButton("Next");
    QPushButton *prevPageButton = new QPushButton("Previous");
    currReceiptPageDisplay = new QLabel(QString("%1 | %2").arg(currReceiptPage).arg(totalReceiptPage));
    QHBoxLayout *hLayout_2 = new QHBoxLayout;
    hLayout_2->addWidget(prevPageButton);
    hLayout_2->addStretch();
    hLayout_2->addWidget(currReceiptPageDisplay);
    hLayout_2->addStretch();
    hLayout_2->addWidget(nextPageButton);

    QVBoxLayout *vLayout_1 = new QVBoxLayout;
    vLayout_1->addLayout(gridLayout_2);
    vLayout_1->addLayout(gridLayout);
    vLayout_1->addWidget(receiptTableView);
    vLayout_1->addLayout(hLayout_2);
    QFrame *frame = new QFrame();
    frame->setFrameStyle(QFrame::StyledPanel);
    frame->setLayout(vLayout_1);

    QVBoxLayout *vLayout_2 = new QVBoxLayout;
    vLayout_2->addLayout(titleLayout);
    vLayout_2->addWidget(frame);
    receipt->setLayout(vLayout_2);

    connect(nextPageButton, &QPushButton::clicked, this, &MainWindow::nextReceiptPage);
    connect(prevPageButton, &QPushButton::clicked, this, &MainWindow::prevReceiptPage);
    connect(queryButton, &QPushButton::clicked, this, &MainWindow::searchReceiptData);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearReceiptData);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetReceiptView);

    return receipt;
}

void MainWindow::resetReceiptView(){
    QString dateStr = QString("select receipt_date from receipt");
    QSqlQuery *query = new QSqlQuery(mysql->myDB);
    if(!query->exec(dateStr)){
        qDebug() << "fail to select receipt_date from receipt: " << query->lastError().text();
    }
    if(query->next()){
        currReceiptDate = query->value(0).toString();
        receiptModel->setFilter(QString("receipt_date = '%1'").arg(query->value(0).toString()));
    }
    receiptModel->select();
}

void MainWindow::createTmpReceiptTable(){
    QString createTmpReceiptStr = QString("delete from tmp_receipt");
    QSqlQuery *query = new QSqlQuery(mysql->myDB);
    if(!query->exec(createTmpReceiptStr)){
        qDebug() << "failed to delete from tmp_receipt: " << query->lastError().text();
    }
    createTmpReceiptStr = QString("insert into tmp_receipt (receipt_date, io_type, name_cn, name_en, alias_cn, alias_en, dose_type, prescription, spec_unit, spec_quantity, package, manufacturer_cn, manufacturer_en, product_of_origin, price, quantity) "
                                  "select receipt_date, io_type, name_cn, name_en, alias_cn, alias_en, dose_type, prescription, spec_unit, spec_quantity, package, manufacturer_cn, manufacturer_en, product_of_origin, price, quantity "
                                  "from ledger inner join SKU on ledger.sku_id = SKU.sku_id "
                                  );
    if(!query->exec(createTmpReceiptStr)){
        qDebug() << "insert into tmp_receipt through ledger join sku failed...\n" << query->lastError().text();
    }

    receiptModel->select();
}

void MainWindow::searchReceiptData(){
    QString dateString = QString("receipt_date = '%1'").arg(dateSelection->currentText());
    QString typeString = QString("io_type = '%1'").arg(typeSelection->currentText());
    QString nameCN = QString("name_cn = '%1'").arg(nameCNEditReceipt->text());
    QString nameEN = QString("name_en = '%1'").arg(nameENEditReceipt->text());
    QString aliasCN = QString("alias_cn = '%1'").arg(aliasCNEditReceipt->text());
    QString aliasEN = QString("alias_en = '%1'").arg(aliasENEditReceipt->text());
    QString dose = QString("dose_type = '%1'").arg(doseEditReceipt->currentText());
    QString pres = QString("prescription = '%1'").arg(presEditReceipt->currentText());
    QString specUnit = QString("spec_unit = '%1'").arg(specUnitEditReceipt->currentText());
    QString specQuant = QString("spec_quantity = '%1'").arg(specQuantEditReceipt->text());
    QString package = QString("package = '%1'").arg(packageEditReceipt->currentText());
    QString manufCN = QString("manufucturer_cn = '%1'").arg(manufCNEditReceipt->text());
    QString manufEN = QString("manufucturer_en = '%1'").arg(manufENEditReceipt->text());
    QString origin = QString("product_of_origin = '%1'").arg(originEditReceipt->text());
    QString quantity = QString("quantity = '%1'").arg(quantityEditReceipt->text());
    QString price = QString("price = '%1'").arg(priceEditReceipt->text());
    QString filterStr = "";
    if(!dateSelection->currentText().isEmpty()){
        filterStr.append(dateString);
        currReceiptDate = dateSelection->currentText();
    }
    if(!typeSelection->currentText().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(typeString);
    }
    if(!nameCNEditReceipt->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(nameCN);
    }
    if(!nameENEditReceipt->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(nameEN);
    }
    if(!aliasCNEditReceipt->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(aliasCN);
    }
    if(!aliasENEditReceipt->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(aliasEN);
    }
    if(!doseEditReceipt->currentText().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(dose);
    }
    if(!presEditReceipt->currentText().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(pres);
    }
    if(!specUnitEditReceipt->currentText().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(specUnit);
    }
    if(!specQuantEditReceipt->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(specQuant);
    }
    if(!packageEditReceipt->currentText().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(package);
    }
    if(!manufCNEditReceipt->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(manufCN);
    }
    if(!manufENEditReceipt->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(manufEN);
    }
    if(!originEditReceipt->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(origin);
    }
    if(!quantityEditReceipt->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(quantity);
    }
    if(!priceEditReceipt->text().isEmpty()){
        if(!filterStr.isEmpty())
            filterStr.append(" and ");
        filterStr.append(price);
    }

    if(!dateSelection->currentText().isEmpty()){
        currReceiptPage = dateSelection->currentIndex();
        currReceiptPageDisplay->setText(QString("%1 | %2").arg(currReceiptPage).arg(totalReceiptPage));
    }

    receiptModel->setFilter(filterStr);
    receiptModel->select();
}

void MainWindow::nextReceiptPage(){
    qDebug() << "\n nextReceiptPage...";
    if(currReceiptPage == totalReceiptPage)
        return;

    QString dateStr = QString("select receipt_date from receipt");
    QSqlQuery *query = new QSqlQuery(mysql->myDB);
    if(!query->exec(dateStr)){
        qDebug() << "fail to select receipt_date from receipt: " << query->lastError().text();
    }
    query->next();
    QString tmpStr = query->value(0).toString();
    while(query->next()){
        if(tmpStr == currReceiptDate && tmpStr != query->value(0).toString()){
            currReceiptDate = query->value(0).toString();
            currReceiptPage++;
            break;
        }
        tmpStr = query->value(0).toString();
    }
    QString receiptFilter = receiptModel->filter();
    qDebug() << "filter was: " << receiptFilter;
    if(!receiptFilter.isEmpty()){
        receiptFilter = "receipt_date = '" + currReceiptDate + "'";
    }
    else{
        receiptFilter = currReceiptDate;
    }
    qDebug() << "new filter is: " << receiptFilter;
    receiptModel->setFilter(receiptFilter);
    receiptModel->select();
    currReceiptPageDisplay->setText(QString("%1 | %2").arg(currReceiptPage).arg(totalReceiptPage));

}

void MainWindow::prevReceiptPage(){
    qDebug() << "\n prevReceiptPage...";
    if(currReceiptPage == 1)
        return;

    QString dateStr = QString("select receipt_date from receipt");
    QSqlQuery *query = new QSqlQuery(mysql->myDB);
    if(!query->exec(dateStr)){
        qDebug() << "fail to select receipt_date from receipt: " << query->lastError().text();
    }
    query->next();
    QString tmpStr = query->value(0).toString();
    while(query->next()){
        if(query->value(0).toString() == currReceiptDate){
            currReceiptDate = tmpStr;
            currReceiptPage--;
            break;
        }
        tmpStr = query->value(0).toString();
    }
    QString receiptFilter = receiptModel->filter();
    qDebug() << "filter was: " << receiptFilter;
    if(!receiptFilter.isEmpty()){
        receiptFilter = "receipt_date = '" + currReceiptDate + "'";
    }
    else{
        receiptFilter = currReceiptDate;
    }
    qDebug() << "new filter is: " << receiptFilter;
    receiptModel->setFilter(receiptFilter);
    receiptModel->select();
    currReceiptPageDisplay->setText(QString("%1 | %2").arg(currReceiptPage).arg(totalReceiptPage));
}

void MainWindow::calcTotalReceiptPage(){
    QString dateStr = QString("select receipt_date from receipt");
    QSqlQuery *query = new QSqlQuery(mysql->myDB);
    if(!query->exec(dateStr)){
        qDebug() << "fail to select receipt_date from receipt: " << query->lastError().text();
    }
    totalReceiptPage = 0;
    QString tmpDateStr = "";
    while(query->next()){
        if(query->value(0).toString() != tmpDateStr){
            totalReceiptPage++;
            tmpDateStr = query->value(0).toString();
        }
    }
}

void MainWindow::clearReceiptData(){
    nameCNEditReceipt->clear();
    nameENEditReceipt->clear();
    aliasCNEditReceipt->clear();
    aliasENEditReceipt->clear();
    doseEditReceipt->setCurrentIndex(0);
    presEditReceipt->setCurrentIndex(0);
    specUnitEditReceipt->setCurrentIndex(0);
    specQuantEditReceipt->clear();
    packageEditReceipt->clear();
    manufCNEditReceipt->clear();
    manufENEditReceipt->clear();
    originEditReceipt->clear();
    quantityEditReceipt->clear();
    priceEditReceipt->clear();
    dateSelection->setCurrentIndex(0);
    typeSelection->setCurrentIndex(0);
}
/* Different date on different page
 *  implement a filter function that filters different dates
 *  next button connects to the filter function, changing the dates
 *  change currPageDisplay text
 *  change date drop down combobox
*/
