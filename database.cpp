#include "database.h"

bool DataBase::connectDB(QSqlDatabase aDb, const QString &aLogin, const QString &aPassword, bool isShowMessage) {
    aDb = QSqlDatabase::addDatabase("QODBC3");
    aDb.setDatabaseName("DRIVER={SQL Server};"
                        "SERVER=" + aLogin + ";"
                        "DATABASE=agzs;"
                        "Persist Security Info=true;"
                        "uid=sa;"
                        "pwd=" + aPassword);
    if (aDb.open()) {
        //qInfo () << "DB is open";
        return true;
    } else {
        qInfo () << "DB isn't open";
        if (isShowMessage) {
            QMessageBox::warning(0, tr("Ошибка!"), tr("Не удалось подключиться к базе данных %1!").arg(aLogin));
        }
        return false;
    }
}

QList<Agzs> DataBase::getAgzs(QDateTime dateStart, QDateTime dateEnd, const QString &aLogin, const QString &aPassword, bool isShowMessage) {
    QSqlDatabase db;
    QList<Agzs> agzss;
    if (!connectDB(db, aLogin, aPassword, isShowMessage)) {
        qWarning() << "getAgzs db isn't open";
        return agzss;
    }
    QSqlQuery query(db);
    query.exec("SELECT DISTINCT "
                      "t.agzs "
                      ",t.AGZSName "
                      ",p.Name "
                  "FROM [agzs].[dbo].[Lexema_Standard_Подразделения] p, ADAST_TRKTransaction t "
                  "WHERE p.vcode < 99 "
                  "and p.vcode not in (66,91) "
                  "and p.ПолноеНаименование = t.AGZSName "
                  "and t.CDate BETWEEN '" + dateStart.toString("yyyyMMdd hh:mm:ss.zzz") + "'"
                                " and '" + dateEnd.toString("yyyyMMdd hh:mm:ss.zzz") + "' "
                  "ORDER BY p.name ASC ");
    while(query.next()) {
        Agzs agzs;
        agzs.number  = query.value(0).toInt();
        agzs.name    = query.value(1).toString();
        agzs.engName = query.value(2).toString();
        agzss.append(agzs);
    }
    db.close();
    return agzss;
}

AgzsPrice DataBase::getPrice(const QString &aAgzs, int aPartnerIndex, const QString &aLogin, const QString &aPassword, bool isShowMessage) {
    QSqlDatabase db;
    AgzsPrice price;
    if (!connectDB(db, aLogin, aPassword, isShowMessage)) {
        qWarning() << "getPrice db isn't open";
        return price;
    }
    QSqlQuery query(db);
    query.exec("SELECT TOP 1 "
                  "p.[AGZS] "
                  ",[diesel] "
                  ",[diesel_premium] "
                  ",[a80] "
                  ",[a92] "
                  ",[a92_premium] "
                  ",[a95] "
                  ",[a95_premium] "
                  ",[a98] "
                  ",[a98_premium] "
                  ",[a100] "
                  ",[a100_premium] "
                  ",[propane] "
                  ",[metan] "
                  ",[iPartner] "
              "FROM [agzs].[dbo].[PR_AGZSPrice] p, [agzs].[dbo].[PR_AGZSData] d "
              "where p.Link = d.VCode "
                  "and p.iPartner = " + QString::number(aPartnerIndex) + " "
                  "and d.AGZSL = '" + aAgzs + "' "
              "order by p.DateStart desc");
    if (query.next()) {
        price.agzs              = query.value(0).toInt();
        price.diesel            = query.value(1).toFloat();
        price.diesel_premium    = query.value(2).toFloat();
        price.a80               = query.value(3).toFloat();
        price.a92               = query.value(4).toFloat();
        price.a92_premium       = query.value(5).toFloat();
        price.a95               = query.value(6).toFloat();
        price.a95_premium       = query.value(7).toFloat();
        price.a98               = query.value(8).toFloat();
        price.a98_premium       = query.value(9).toFloat();
        price.a100              = query.value(10).toFloat();
        price.a100_premium      = query.value(11).toFloat();
        price.propane           = query.value(12).toFloat();
        price.metan             = query.value(13).toFloat();
        price.iPartner          = query.value(14).toInt();
    }
    db.close();
    return price;
}

AgzsData DataBase::getData(const QString &aAgzs, const QString &aLogin, const QString &aPassword, bool isShowMessage) {
    QSqlDatabase db;
    AgzsData data;
    if (!connectDB(db, aLogin, aPassword, isShowMessage)) {
        qWarning() << "getData db isn't open";
        return data;
    }
    QSqlQuery query(db);
    query.exec("SELECT TOP 1 [AGZSName]"
                  ",[AGZS] "
                  ",[CDate] "
                  ",[VCode] "
                  ",[Id] "
                  ",[Adress] "
                  ",[latitude] "
                  ",[longitude] "
                  ",[ColumnsCount] "
                  ",[AGZSL] "
                  ",[AGZSP] "
                  ",[CityMobileToken] "
                  ",[YandexToken] "
              "FROM [agzs].[dbo].[PR_AGZSData] "
              "where AGZSL = '" + aAgzs + "' "
              "order by CDate desc");
    if (query.next()) {
        data.agzsName           = query.value(0).toString();
        data.agzs               = query.value(1).toInt();
        data.cDate              = QDateTime::fromString(query.value(2).toString().left(23), "yyyy-MM-ddThh:mm:ss.zzz");
        data.vCode              = query.value(3).toInt();
        data.id                 = query.value(4).toString();
        data.adress             = query.value(5).toString();
        data.latitude           = query.value(6).toFloat();
        data.longitude          = query.value(7).toFloat();
        data.columnsCount       = query.value(8).toInt();
        data.AgzsL              = query.value(9).toString();
        data.AgzsP              = query.value(10).toString();
        data.cityMobileToken    = query.value(11).toString();
        data.yandexToken        = query.value(12).toString();
    }
    db.close();
    return data;
}

QList<AgzsColumn> DataBase::getColumnsDb(const QString &aAgzs, const QString &aLogin, const QString &aPassword, bool isShowMessage) {
    QSqlDatabase db;
    QList<AgzsColumn> columns;
    if (!connectDB(db, aLogin, aPassword, isShowMessage)) {
        qWarning() << "getColumnsDb db isn't open";
        return columns;
    }
    QSqlQuery query(db);
    query.exec("SELECT TOP 20 c.[agzs] "
                  ",c.[CDate] "
                  ",c.[VCode] "
                  ",[SideAdress] "
                  ",[Partner_TrkNum] "
                  ",[Visible] "
                  ",[Side] "
                  ",[Nozzle1Fuel] "
                  ",[Nozzle2Fuel] "
                  ",[Nozzle3Fuel] "
                  ",[Nozzle4Fuel] "
                  ",[Nozzle5Fuel] "
              "FROM [agzs].[dbo].[PR_Columns] c, [agzs].[dbo].[PR_AGZSData] d "
              "where c.agzs = d.AGZS and d.agzsL = '" + aAgzs + "' "
              "order by CDate desc");
    while(query.next()) {
        AgzsColumn column;
        column.agzs = query.value(0).toInt();
        column.cDate = QDateTime::fromString(query.value(1).toString().left(23), "yyyy-MM-ddThh:mm:ss.zzz");
        column.vCode = query.value(2).toInt();
        column.sideAdress = query.value(3).toInt();
        column.partnerTrkNum = query.value(4).toInt();
        column.visible = query.value(5).toInt();
        column.side = query.value(6).toString();
        column.nozzle1Fuel = query.value(7).toInt();
        column.nozzle2Fuel = query.value(8).toInt();
        column.nozzle3Fuel = query.value(9).toInt();
        column.nozzle4Fuel = query.value(10).toInt();
        column.nozzle5Fuel = query.value(11).toInt();
        columns.append(column);
    }
    db.close();
    return columns;
}

QList<AgzsColumn> DataBase::getColumnsAdast(const QString &aAgzs, const QString &aPass, int aAgzsNum, bool isShowMessage) {
    QSqlDatabase db;
    QList<AgzsColumn> columns;
    if (!connectDB(db, aAgzs, aPass, isShowMessage)) {
        qWarning() << "getColumnsAdast db isn't open";
        return columns;
    }
    QSqlQuery queryA(db);
    queryA.exec("SELECT TOP 1000 "
                    "[AGZS] "
                    ",[SideA_Address] "
                    ",[SideA_Nozzle1Fuel] "
                    ",[SideA_Nozzle2Fuel] "
                    ",[SideA_Nozzle3Fuel] "
                    ",[SideA_Nozzle4Fuel] "
                    ",[SideA_Nozzle5Fuel] "
                    ",[CDate] "
                    ",[VCode] "
               "FROM [agzs].[dbo].[ADAST_AdastTRK] "
               "where agzs = '" + QString::number(aAgzsNum) + "' "
               "order by CDate desc");
    QSqlQuery queryB(db);
    queryB.exec("SELECT TOP 1000 "
                    "[AGZS] "
                    ",[SideB_Address] "
                    ",[SideB_Nozzle1Fuel] "
                    ",[SideB_Nozzle2Fuel] "
                    ",[SideB_Nozzle3Fuel] "
                    ",[SideB_Nozzle4Fuel] "
                    ",[SideB_Nozzle5Fuel] "
                    ",[CDate] "
                    ",[VCode] "
               "FROM [agzs].[dbo].[ADAST_AdastTRK] "
               "where agzs = '" + QString::number(aAgzsNum) + "' "
               "order by CDate desc");

    while (queryA.next()) {
        if (queryA.value(1).toInt() > 0) {
            AgzsColumn column;
            column.agzs = queryA.value(0).toInt();
            column.sideAdress = queryA.value(1).toInt();
            column.partnerTrkNum = queryA.value(1).toInt();
            column.visible = 1;
            column.side = "A";
            column.nozzle1Fuel = queryA.value(2).toInt();
            column.nozzle2Fuel = queryA.value(3).toInt();
            column.nozzle3Fuel = queryA.value(4).toInt();
            column.nozzle4Fuel = queryA.value(5).toInt();
            column.nozzle5Fuel = queryA.value(6).toInt();
            columns.append(column);
        }
    }
    while (queryB.next()) {
        if (queryB.value(1).toInt() > 0) {
            AgzsColumn column;
            column.agzs = queryB.value(0).toInt();
            column.sideAdress = queryB.value(1).toInt();
            column.partnerTrkNum = queryB.value(1).toInt();
            column.visible = 1;
            column.side = "B";
            column.nozzle1Fuel = queryB.value(2).toInt();
            column.nozzle2Fuel = queryB.value(3).toInt();
            column.nozzle3Fuel = queryB.value(4).toInt();
            column.nozzle4Fuel = queryB.value(5).toInt();
            column.nozzle5Fuel = queryB.value(6).toInt();
            columns.append(column);
        }
    }
    db.close();
    return columns;
}

int DataBase::getLxKeyCode(const QString &key, bool autoIncrement, const QString &aLogin, const QString &aPassword, bool isShowMessage) {
    QSqlDatabase db;
    int value = -1;
    if (!connectDB(db, aLogin, aPassword, isShowMessage)) {
        qWarning() << "getLxKeyCode db isn't open";
        return value;
    }
    QSqlQuery query(db);
    query.prepare("SELECT [Value] FROM [agzs].[dbo].[LxKeysOfCodes] "
                  "WHERE [Key] = :Key");
    query.bindValue(":Key", key);
    query.exec();
    if (query.next()) {
        value = query.value(0).toInt();

        if (autoIncrement) {
            query.prepare("UPDATE [agzs].[dbo].[LxKeysOfCodes] SET "
                       "Value = Value + 1 "
                       "WHERE [Key] = :Key");
            query.bindValue(":Key", key);
            query.exec();
            if (query.lastError().type() != QSqlError::NoError) {
                qWarning() << "getLxKeyCode don't increment value" << key;
            }
        }
    }
    db.close();
    return value;
}

bool DataBase::updateAgzsData(const QString &aAgzsName, const int &aAgzs, const QString &aId,
                              const QString &aAdress, const double &aLatitude, const double &aLongitude,
                              const int &aColumnsCount, const QString &aAgzsL, const QString &aAgzsP,
                              const QString &aCityMobileToken, const QString &aYandexToken, const int &aVcode,
                              const QString &aLogin, const QString &aPassword, bool isShowMessage) {
    QSqlDatabase db;
    if (!connectDB(db, aLogin, aPassword, isShowMessage)) {
        qWarning() << "updateAgzsData db isn't open";
        return false;
    }
    QSqlQuery query(db);
    if (aVcode > 0) {
        query.prepare("UPDATE [agzs].[dbo].[PR_AGZSData] SET "
                      "AGZSName = :AgzsName "
                      ",AGZS = :AgzsNumber "
                      ",Id = :Id "
                      ",Adress = :Adress "
                      ",latitude = :Latitude "
                      ",longitude = :Longitude "
                      ",ColumnsCount = :Columns "
                      ",AGZSL = :AGZSL "
                      ",AGZSP = :AGZSP "
                      ",CityMobileToken = :CityMobileToken "
                      ",YandexToken = :YandexToken "
                      "WHERE VCode = :VCode");
        query.bindValue(":AgzsName", aAgzsName);
        query.bindValue(":AgzsNumber", aAgzs);
        query.bindValue(":Id", aId);
        query.bindValue(":Adress", aAdress);
        query.bindValue(":Latitude", aLatitude);
        query.bindValue(":Longitude", aLongitude);
        query.bindValue(":Columns", aColumnsCount);
        query.bindValue(":AGZSL", aAgzsL);
        query.bindValue(":AGZSP", aAgzsP);
        query.bindValue(":CityMobileToken", aCityMobileToken);
        query.bindValue(":YandexToken", aYandexToken);
        query.bindValue(":VCode", aVcode);
        query.exec();
        if(query.lastError().type() == QSqlError::NoError) {
            db.close();
            return true;
        }
    }
    db.close();
    return false;
}

bool DataBase::updateAgzsData(const AgzsData &aData, const QString &aLogin, const QString &aPassword, bool isShowMessage) {
    return updateAgzsData(aData.agzsName, aData.agzs, aData.id, aData.adress, aData.latitude, aData.longitude,
                   aData.columnsCount, aData.AgzsL, aData.AgzsP, aData.cityMobileToken, aData.yandexToken, aData.vCode,
                   aLogin, aPassword, isShowMessage);
}

bool DataBase::insertAgzsData(const QString &aAgzsName, const int &aAgzs, const QDateTime &aCdate, const QString &aId,
                              const QString &aAdress, const float &aLatitude, const float &aLongitude,
                              const int &aColumnsCount, const QString &aAgzsL, const QString &aAgzsP,
                              const QString &aCityMobileToken, const QString &aYandexToken, const int &aVcode,
                              const QString &aLogin, const QString &aPassword, bool isShowMessage) {
    QSqlDatabase db;
    if (!connectDB(db, aLogin, aPassword, isShowMessage)) {
        qWarning() << "insertAgzsData db isn't open";
        return false;
    }
    QSqlQuery query(db);
    if (aVcode > 0) {
        query.prepare("INSERT INTO [agzs].[dbo].[PR_AGZSData] VALUES( "
                      ":AgzsName "
                      ",:AgzsNumber "
                      ",:CDate "
                      ",:VCode "
                      ",:Id "
                      ",:Adress "
                      ",:Latitude "
                      ",:Longitude "
                      ",:Columns "
                      ",:AGZSL "
                      ",:AGZSP "
                      ",DEFAULT "
                      ",:CityMobileToken "
                      ",:YandexToken "
                      ")");
        query.bindValue(":AgzsName", aAgzsName);
        query.bindValue(":AgzsNumber", aAgzs);
        query.bindValue(":CDate", aCdate.toString("yyyyMMdd hh:mm:ss"));
        query.bindValue(":VCode", aVcode);
        query.bindValue(":Id", aId);
        query.bindValue(":Adress", aAdress);
        query.bindValue(":Latitude", aLatitude);
        query.bindValue(":Longitude", aLongitude);
        query.bindValue(":Columns", aColumnsCount);
        query.bindValue(":AGZSL", aAgzsL);
        query.bindValue(":AGZSP", aAgzsP);
        query.bindValue(":CityMobileToken", aCityMobileToken);
        query.bindValue(":YandexToken", aYandexToken);
        query.exec();
        if(query.lastError().type() == QSqlError::NoError) {
            db.close();
            return true;
        }
    }
    db.close();
    return false;
}

bool DataBase::insertAgzsData(const AgzsData &aData, const QString &aLogin, const QString &aPassword, bool isShowMessage) {
    return insertAgzsData(aData.agzsName, aData.agzs, aData.cDate, aData.id, aData.adress, aData.latitude, aData.longitude,
                   aData.columnsCount, aData.AgzsL, aData.AgzsP, aData.cityMobileToken, aData.yandexToken, aData.vCode,
                   aLogin, aPassword, isShowMessage);
}

bool DataBase::updateAgzsColumn(const int &aAgzs, const int &aVcode, const int &aSideAdress,
                                const int &aPartnerTrkNum, const int &aVisible, const QString &aSide,
                                const int &aNozzle1Fuel, const int &aNozzle2Fuel, const int &aNozzle3Fuel,
                                const int &aNozzle4Fuel, const int &aNozzle5Fuel, const QString &aLogin,
                                const QString &aPassword, bool isShowMessage) {
    QSqlDatabase db;
    if (!connectDB(db, aLogin, aPassword, isShowMessage)) {
        qWarning() << "updateAgzsColumns db isn't open";
        return false;
    }
    QSqlQuery query(db);
    if (aVcode > 0) {
        query.prepare("UPDATE [agzs].[dbo].[PR_Columns] SET "
                      "AGZS = :AgzsNumber "
                      ",SideAdress = :SideAdress "
                      ",Partner_TrkNum = :PartnerTrkNum "
                      ",Visible = :Visible "
                      ",Side = :SideS "
                      ",Nozzle1Fuel = :Nozzle1Fuel "
                      ",Nozzle2Fuel = :Nozzle2Fuel "
                      ",Nozzle3Fuel = :Nozzle3Fuel "
                      ",Nozzle4Fuel = :Nozzle4Fuel "
                      ",Nozzle5Fuel = :Nozzle5Fuel "
                      "WHERE VCode = :VCode");
        query.bindValue(":AgzsNumber", aAgzs);
        query.bindValue(":SideAdress", aSideAdress);
        query.bindValue(":PartnerTrkNum", aPartnerTrkNum);
        query.bindValue(":Visible", aVisible);
        query.bindValue(":SideS", aSide);
        query.bindValue(":Nozzle1Fuel", aNozzle1Fuel);
        query.bindValue(":Nozzle2Fuel", aNozzle2Fuel);
        query.bindValue(":Nozzle3Fuel", aNozzle3Fuel);
        query.bindValue(":Nozzle4Fuel", aNozzle4Fuel);
        query.bindValue(":Nozzle5Fuel", aNozzle5Fuel);
        query.bindValue(":VCode", aVcode);
        query.exec();
        if(query.lastError().type() == QSqlError::NoError) {
            db.close();
            return true;
        }
    }
    db.close();
    return false;
}

bool DataBase::updateAgzsColumn(const AgzsColumn &aColumn, const QString &aLogin, const QString &aPassword, bool isShowMessage) {
    return updateAgzsColumn(aColumn.agzs, aColumn.vCode, aColumn.sideAdress, aColumn.partnerTrkNum,
                            aColumn.visible, aColumn.side, aColumn.nozzle1Fuel, aColumn.nozzle2Fuel,
                            aColumn.nozzle3Fuel, aColumn.nozzle4Fuel, aColumn.nozzle5Fuel,
                            aLogin, aPassword, isShowMessage);
}

bool DataBase::insertAgzsColumn(const int &aAgzs, const QDateTime &aCdate, const int &aVcode, const int &aSideAdress,
                                const int &aPartnerTrkNum, const int &aVisible, const QString &aSide,
                                const int &aNozzle1Fuel, const int &aNozzle2Fuel, const int &aNozzle3Fuel,
                                const int &aNozzle4Fuel, const int &aNozzle5Fuel, const QString &aLogin,
                                const QString &aPassword, bool isShowMessage) {
    QSqlDatabase db;
    if (!connectDB(db, aLogin, aPassword, isShowMessage)) {
        qWarning() << "insertAgzsColumn db isn't open";
        return false;
    }
    QSqlQuery query(db);
    if (aVcode > 0) {
        query.prepare("INSERT INTO [agzs].[dbo].[PR_Columns] VALUES( "
                      ":AgzsNumber "
                      ",:CDate "
                      ",:VCode "
                      ",:SideAdress "
                      ",:PartnerTrkNum "
                      ",:Visible "
                      ",:Side "
                      ",:Nozzle1Fuel "
                      ",:Nozzle2Fuel "
                      ",:Nozzle3Fuel "
                      ",:Nozzle4Fuel "
                      ",:Nozzle5Fuel "
                      ")");
        query.bindValue(":AgzsNumber", aAgzs);
        query.bindValue(":CDate", aCdate.toString("yyyyMMdd hh:mm:ss"));
        query.bindValue(":VCode", aVcode);
        query.bindValue(":SideAdress", aSideAdress);
        query.bindValue(":PartnerTrkNum", aPartnerTrkNum);
        query.bindValue(":Visible", aVisible);
        query.bindValue(":Side", aSide);
        query.bindValue(":Nozzle1Fuel", aNozzle1Fuel);
        query.bindValue(":Nozzle2Fuel", aNozzle2Fuel);
        query.bindValue(":Nozzle3Fuel", aNozzle3Fuel);
        query.bindValue(":Nozzle4Fuel", aNozzle4Fuel);
        query.bindValue(":Nozzle5Fuel", aNozzle5Fuel);
        query.exec();
        if(query.lastError().type() == QSqlError::NoError) {
            db.close();
            return true;
        }
    }
    db.close();
    return false;
}

bool DataBase::insertAgzsColumn(const AgzsColumn &aColumn, const QString &aLogin, const QString &aPassword, bool isShowMessage) {
    return insertAgzsColumn(aColumn.agzs, aColumn.cDate ,aColumn.vCode, aColumn.sideAdress, aColumn.partnerTrkNum,
                            aColumn.visible, aColumn.side, aColumn.nozzle1Fuel, aColumn.nozzle2Fuel,
                            aColumn.nozzle3Fuel, aColumn.nozzle4Fuel, aColumn.nozzle5Fuel,
                            aLogin, aPassword, isShowMessage);
}

bool DataBase::deleteAgzsColumn(const int &aVcode, const QString &aLogin, const QString &aPassword, bool isShowMessage) {
    QSqlDatabase db;
    if (!connectDB(db, aLogin, aPassword, isShowMessage)) {
        qWarning() << "deleteAgzsColumn db isn't open";
        return false;
    }
    QSqlQuery query(db);
    if (aVcode > 0) {
        query.prepare("DELETE FROM [agzs].[dbo].[PR_Columns] "
                      "WHERE VCode = :VCode ");
        query.bindValue(":VCode", aVcode);
        query.exec();
        if(query.lastError().type() == QSqlError::NoError) {
            db.close();
            return true;
        }
    }
    db.close();
    return false;
}

bool DataBase::updateLastAgzsPrice(const QString &aAgzsName, const int &aAgzs, const int &aLink, const QString &aPartner,
                               const double &aDiesel, const double &aDiesel_premium, const double &aA80,
                               const double &aA92, const double &aA92_premium, const double &aA95,
                               const double &aA95_premium, const double &aA98, const double &aA98_premium,
                               const double &aA100, const double &aA100_premium, const double &aPropane,
                               const double &aMetan, const int &aiPartner, const QString &aLogin,
                               const QString &aPassword, bool isShowMessage) {
    QSqlDatabase db;
    if (!connectDB(db, aLogin, aPassword, isShowMessage)) {
        qWarning() << "updateLastAgzsPrice db isn't open";
        return false;
    }
    QSqlQuery query(db);
    query.exec("SELECT TOP 1 "
                    "[VCode] "
               "FROM [agzs].[dbo].[PR_AGZSPrice] "
               "where agzs = '" + QString::number(aAgzs) + "' "
                     "and iPartner = " + QString::number(aiPartner) +
               "order by DateStart desc");
    int vCode = -1;
    if (query.next()) {
        vCode = query.value(0).toInt();
    }
    if (vCode > 0) {
        query.prepare("UPDATE [agzs].[dbo].[PR_AGZSPrice] SET "
                      "[AGZSName] = :AgzsName "
                      ",[AGZS] = :AgzsNumber "
                      ",[Link] = :Link "
                      ",[Partner] = :Partner "
                      ",[diesel] = :Diesel "
                      ",[diesel_premium] = :DieselPremium "
                      ",[a80] = :A80 "
                      ",[a92] = :A92 "
                      ",[a92_premium] = :a92Premium "
                      ",[a95] = :A95 "
                      ",[a95_premium] = :a95Premium "
                      ",[a98] = :A98 "
                      ",[a98_premium] = :a98Premium "
                      ",[a100] = :A100 "
                      ",[a100_premium] = :a100Premium "
                      ",[propane] = :Propane "
                      ",[metan] = :Metan "
                      ",[iPartner] = :iPartner "
                      "WHERE VCode = :VCode");
        query.bindValue(":AgzsName", aAgzsName);
        query.bindValue(":AgzsNumber", aAgzs);
        query.bindValue(":Link", aLink);
        query.bindValue(":Partner", aPartner);
        query.bindValue(":Diesel", aDiesel);
        query.bindValue(":DieselPremium", aDiesel_premium);
        query.bindValue(":A80", aA80);
        query.bindValue(":A92", aA92);
        query.bindValue(":a92Premium", aA92_premium);
        query.bindValue(":A95", aA95);
        query.bindValue(":a95Premium", aA95_premium);
        query.bindValue(":A98", aA98);
        query.bindValue(":a98Premium", aA98_premium);
        query.bindValue(":A100", aA100);
        query.bindValue(":a100Premium", aA100_premium);
        query.bindValue(":Propane", aPropane);
        query.bindValue(":Metan", aMetan);
        query.bindValue(":iPartner", aiPartner);
        query.bindValue(":VCode", vCode);
        query.exec();
        if(query.lastError().type() == QSqlError::NoError) {
            db.close();
            return true;
        }
    }
    db.close();
    return false;
}

bool DataBase::updateLastAgzsPrice(const AgzsPrice &aPrice, const QString &aLogin, const QString &aPassword, bool isShowMessage) {
    return updateLastAgzsPrice(aPrice.agzsName, aPrice.agzs, aPrice.link, aPrice.partner, aPrice.diesel,
                           aPrice.diesel_premium, aPrice.a80, aPrice.a92, aPrice.a92_premium, aPrice.a95,
                           aPrice.a95_premium, aPrice.a98, aPrice.a98_premium, aPrice.a100, aPrice.a100_premium,
                           aPrice.propane, aPrice.metan, aPrice.iPartner, aLogin, aPassword, isShowMessage);
}

bool DataBase::insertAgzsPrice(const QString &aAgzsName, const int &aAgzs, const int &aVCode, const int &aLink,
                               const QDateTime &aCDate, const QString &aPartner, const double &aDiesel,
                               const double &aDiesel_premium, const double &aA80, const double &aA92,
                               const double &aA92_premium, const double &aA95, const double &aA95_premium,
                               const double &aA98, const double &aA98_premium, const double &aA100,
                               const double &aA100_premium, const double &aPropane, const double &aMetan,
                               const int &aiPartner, const QDateTime &aDateStart, const QString &aLogin,
                               const QString &aPassword, bool isShowMessage) {
    QSqlDatabase db;
    if (!connectDB(db, aLogin, aPassword, isShowMessage)) {
        qWarning() << "insertAgzsPrice db isn't open";
        return false;
    }
    QSqlQuery query(db);
    if (aVCode > 0) {
        query.prepare("INSERT INTO [agzs].[dbo].[PR_AGZSPrice] VALUES( "
                      ":AGZSName "
                      ",:AGZS "
                      ",:VCode "
                      ",:Link "
                      ",:CDate "
                      ",:Partner "
                      ",:diesel "
                      ",:diesel_premium "
                      ",:A80 "
                      ",:A92 "
                      ",:a92_premium "
                      ",:A95 "
                      ",:a95_premium "
                      ",:A98 "
                      ",:a98_premium "
                      ",:A100 "
                      ",:a100_premium "
                      ",:propane "
                      ",:metan "
                      ",:iPartner "
                      ",:DateStart "
                      ")");
        query.bindValue(":AGZSName", aAgzsName);
        query.bindValue(":AGZS", aAgzs);
        query.bindValue(":VCode", aVCode);
        query.bindValue(":Link", aLink);
        query.bindValue(":CDate", aCDate.toString("yyyyMMdd hh:mm:ss"));
        query.bindValue(":Partner", aPartner);
        query.bindValue(":diesel", aDiesel);
        query.bindValue(":diesel_premium", aDiesel_premium);
        query.bindValue(":A80", aA80);
        query.bindValue(":A92", aA92);
        query.bindValue(":a92_premium", aA92_premium);
        query.bindValue(":A95", aA95);
        query.bindValue(":a95_premium", aA95_premium);
        query.bindValue(":A98", aA98);
        query.bindValue(":a98_premium", aA98_premium);
        query.bindValue(":A100", aA100);
        query.bindValue(":a100_premium", aA100_premium);
        query.bindValue(":propane", aPropane);
        query.bindValue(":metan", aMetan);
        query.bindValue(":iPartner", aiPartner);
        query.bindValue(":DateStart", aDateStart.toString("yyyyMMdd hh:mm:ss"));
        query.exec();
        if(query.lastError().type() == QSqlError::NoError) {
            db.close();
            return true;
        }
    }
    db.close();
    return false;
}

bool DataBase::insertAgzsPrice(const AgzsPrice &aPrice, const QString &aLogin, const QString &aPassword, bool isShowMessage) {
    return insertAgzsPrice(aPrice.agzsName, aPrice.agzs, aPrice.vCode, aPrice.link, aPrice.cDate, aPrice.partner,
                           aPrice.diesel, aPrice.diesel_premium, aPrice.a80, aPrice.a92, aPrice.a92_premium, aPrice.a95,
                           aPrice.a95_premium, aPrice.a98, aPrice.a98_premium, aPrice.a100, aPrice.a100_premium,
                           aPrice.propane, aPrice.metan, aPrice.iPartner, aPrice.dateStart, aLogin, aPassword,
                           isShowMessage);
}

QList<QList<QVariant> > DataBase::getBadTransaction(const int &aAgzsNumber, const QDateTime &aDateStart,
                                                    const QDateTime &aDateEnd, const int &aFuel, const QString &aLogin,
                                                    const QString &aPassword, bool isShowMessage) {
    QList<QList<QVariant>> list;
    QSqlDatabase db;
    if (!connectDB(db, aLogin, aPassword, isShowMessage)) {
        qWarning() << "getBadTransaction db isn't open";
        return list;
    }
    QSqlQuery query(db);
    query.exec("{CALL [grimnir].[agzs].[dbo].[PR_CheckTransError] (" +
               QString::number(aAgzsNumber) + ",'" +
               aDateStart.toString("yyyyMMdd hh:mm:ss") + "','" +
               aDateEnd.toString("yyyyMMdd hh:mm:ss") + "'," +
               QString::number(aFuel) + ")}");
    qDebug() << query.lastQuery() << query.lastError();
    if (query.next()) {
        QList<QVariant> row;
        for (int column = 0; column < query.record().count(); ++column) {
            row.append(query.value(column));
        }
        list.append(row);
    }
    db.close();
    return list;

}
