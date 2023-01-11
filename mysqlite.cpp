#include "mysqlite.h"

mySqlite::mySqlite()
{

}

bool mySqlite::initSql(){
    if(!connectDB()){
        qDebug() << "db not open";
        return false;
    }
    else if(!createTable()){
        qDebug() << "cannot create table";
        return false;
    }
    return true;
}

bool mySqlite::connectDB(){
    myDB = QSqlDatabase::addDatabase("QSQLITE");
    myDB.setDatabaseName("/Users/yuqiancao/Documents/CS/build-myStorageSystem-Desktop_arm_darwin_generic_mach_o_64bit-Debug/Database/myMSS.db");
    if(!myDB.open())
        return false;
    return true;
}

bool mySqlite::createTable(){
    //SKU table
    query = new QSqlQuery(myDB);
    QString skuString = "create table if not exists SKU("
                        "[sku_id] integer primary key autoincrement," //SKU ID
                        "[name_cn] varchar(30),"
                        "[name_en] varchar(30),"
                        "[alias_cn] varchar(30),"
                        "[alias_en] varchar(30),"
                        "[dose_type] varchar(30),"
                        "[prescription] varchar(30),"
                        "[spec_unit] varchar(30),"
                        "[spec_quantity] float,"
                        "[package] varchar(30),"
                        "[manufacturer_cn] varchar(30),"
                        "[manufacturer_en] varchar(30),"
                        "[product_of_origin] varchar(30),"
                        "UNIQUE (name_cn, manufacturer_cn) ON CONFLICT IGNORE"
                        ")";

    //Inventory Table
    QString invString = "create table if not exists inventory("
                        "[sku_id] integer primary key,"
                        "[quantity] integer,"
                        "[price] double,"
                        "FOREIGN KEY (sku_id) REFERENCES SKU (sku_id)"
                        ")";

    //Receipt Table
    QString receiptString = "create table if not exists receipt("
                            "[receipt_id] integer primary key autoincrement,"
                            "[io_type] varchar(30),"
                            "[receipt_date] varchar(30)"
                            //"[state] varchar(30)"
                            ")";

    //Ledger Table
    QString ledgerString = "create table if not exists ledger("
                           "[sku_id] integer,"
                           "[receipt_id] integer,"
                           "[quantity] integer,"
                           "[price] double,"
                           "[io_type] varchar(30),"
                           "[receipt_date] varchar(30),"
                           "FOREIGN KEY (sku_id) REFERENCES SKU (sku_id),"
                           "FOREIGN KEY (receipt_id) REFERENCES receipt (receipt_id)"
                           ")";

    //tmp ledger table
    QString tmpLedgerString = "create table if not exists tmp_ledger("
                              "[sku_id] integer,"
                              "[name_cn] varchar(30),"
                              "[name_en] varchar(30),"
                              "[alias_cn] varchar(30),"
                              "[alias_en] varchar(30),"
                              "[dose_type] varchar(30),"
                              "[prescription] varchar(30),"
                              "[spec_unit] varchar(30),"
                              "[spec_quantity] float,"
                              "[package] varchar(30),"
                              "[manufacturer_cn] varchar(30),"
                              "[manufacturer_en] varchar(30),"
                              "[product_of_origin] varchar(30),"
                              "[quantity] integer,"
                              "[price] double,"
                              "[io_type] varchar(30),"
                              "FOREIGN KEY (sku_id) REFERENCES SKU (sku_id)"
                              ")";

    //tmp inventory table
    QString tmpInventoryTable = "create table if not exists tmp_inventory("
                                "[name_cn] varchar(30),"
                                "[name_en] varchar(30),"
                                "[alias_cn] varchar(30),"
                                "[alias_en] varchar(30),"
                                "[dose_type] varchar(30),"
                                "[prescription] varchar(30),"
                                "[spec_unit] varchar(30),"
                                "[spec_quantity] float,"
                                "[package] varchar(30),"
                                "[manufacturer_cn] varchar(30),"
                                "[manufacturer_en] varchar(30),"
                                "[product_of_origin] varchar(30),"
                                "[quantity] integer,"
                                "[price] double"
                                ")";

    QString tmpReceipt = "create table if not exists tmp_receipt("
                         "[receipt_date] varchar(30),"
                         "[io_type] varchar(30),"
                         "[name_cn] varchar(30),"
                         "[name_en] varchar(30),"
                         "[alias_cn] varchar(30),"
                         "[alias_en] varchar(30),"
                         "[dose_type] varchar(30),"
                         "[prescription] varchar(30),"
                         "[spec_unit] varchar(30),"
                         "[spec_quantity] float,"
                         "[package] varchar(30),"
                         "[manufacturer_cn] varchar(30),"
                         "[manufacturer_en] varchar(30),"
                         "[product_of_origin] varchar(30),"
                         "[quantity] integer,"
                         "[price] double"
                         ")";


    //create trigger on ledger
    QString triggerString = "create trigger if not exists trigger_upon_ledger after insert on ledger "
                            "begin "
                                "insert or replace into inventory(sku_id, quantity, price) "
                                "values ("
                                        "NEW.sku_id, "
                                        "CASE "
                                            "WHEN NEW.io_type = 'Inventory Restock' OR NEW.io_type = 'Monitor Input' THEN NEW.quantity + IFNULL((SELECT quantity from inventory AS i WHERE i.sku_id = NEW.sku_id), 0) "
                                            "WHEN NEW.io_type = 'Inventory Consumption' OR NEW.io_type = 'Monitor Output' THEN (SELECT quantity from inventory AS i WHERE i.sku_id = NEW.sku_id) - NEW.quantity "
                                        "END,"
                                        "NEW.price"
                                        "); "
                            "end;";

    //QString tmpInvTriggerString = "create trigger";

    if(!(query->exec(skuString) && query->exec(invString) && query->exec(receiptString) && query->exec(ledgerString) && query->exec(tmpLedgerString) && query->exec(tmpInventoryTable) && query->exec(tmpReceipt)))
    {
        qDebug() << "table error" << query->lastError().text();
        return false;
    }
    if(!(query->exec(triggerString)))
    {
        qDebug() << "create trigger error" << query->lastError().text();
        return false;
    }
    return true;
}

